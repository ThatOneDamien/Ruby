#include "ruby_pch.h"

#include "Ruby/Main/Core.h"
#include "Ruby/Main/App.h"
#include "Ruby/Utility/SPIRVCaching.h"
#include "API/OpenGL/OpenGLShader.h"

#include <glad/glad.h>
#include <fstream>



namespace Ruby
{
    static constexpr size_t SHADER_CNT = (size_t)ShaderType::NUMBER_OF_SHADER_TYPES;

    typedef std::string           ShaderSources[SHADER_CNT];
    typedef std::vector<uint32_t> SPIRVBinaries[SHADER_CNT];

    static bool splitCombinedIntoSrcs(const std::string& filepath, ShaderSources o_Srcs);
    static GLuint compileShader(const SPIRVBinaries binaries);


    OpenGLShader::OpenGLShader(const std::string& filepath, bool shouldCache, const std::string& cachePath)
        : m_Combined(true), m_ShouldCache(shouldCache)
    {
        m_StageFilePaths.VertexPath = filepath;
        m_CacheFilePaths.VertexPath = cachePath.empty() ? filepath + ".spv" : cachePath;

        m_LastWrittenTime = std::filesystem::last_write_time(filepath);
        m_ProgramID = 0;
        recompile(true);
    }

    bool OpenGLShader::recompile(bool force)
    {
        if(!force && !needToRecompile())
            return true;

        SPIRVBinaries binaries;
        if(m_Combined)
        {
            ShaderSources srcs;
            if(!splitCombinedIntoSrcs(m_StageFilePaths.VertexPath, srcs))
                return false;
            for(int i = 0; i < SHADER_CNT; ++i)
                if(!srcs[i].empty())
                {
                    binaries[i] = SPIRV::compileToSPIRV(srcs[i].c_str(), srcs[i].size(), (ShaderType)i);
                    if(binaries[i].empty())
                        return false;
                }
        }
        else
            return false;

        GLuint programID = compileShader(binaries);
        if(programID == (GLuint)-1)
            return false;

        if(m_ProgramID)
            glDeleteProgram(m_ProgramID);
        m_ProgramID = programID;
        return true;
    }
    

    OpenGLShader::~OpenGLShader()
    {
        if(App::instanceExists())
            glDeleteProgram(m_ProgramID);
    }

    void OpenGLShader::bind() const
    {
        glUseProgram(m_ProgramID);
    }


    void OpenGLShader::setUniformInt(const char* name, int value) const
    {
        glUniform1i(getUniformLocation(name), value);
    }


    void OpenGLShader::setUniformFloat(const char* name, float value) const
    {
        glUniform1f(getUniformLocation(name), value);
    }

    void OpenGLShader::setUniformVec2(const char* name, const glm::vec2& values) const
    {
        glUniform2f(getUniformLocation(name), values.x, values.y);
    }

    void OpenGLShader::setUniformVec3(const char* name, const glm::vec3& values) const
    {
        glUniform3f(getUniformLocation(name), values.x, values.y, values.z);
    }

    void OpenGLShader::setUniformVec4(const char* name, const glm::vec4& values) const
    {
        glUniform4f(getUniformLocation(name), values.x, values.y, values.z, values.w);
    }

    void OpenGLShader::setUniformMat4(const char* name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
    }

    void OpenGLShader::setUniformIntArr(const char* name, uint32_t count, int* arr) const
    {
        glUniform1iv(getUniformLocation(name), (int)count, arr);
    }

    void OpenGLShader::setUniformFloatArr(const char* name, uint32_t count, float* arr) const
    {
        glUniform1fv(getUniformLocation(name), (int)count, arr);
    }

    int OpenGLShader::getUniformLocation(const std::string& name) const
    {
        if(m_CachedUniforms.count(name))
        {
            GLint location = glGetUniformLocation(m_ProgramID, name.c_str());
            RB_ASSERT(location != -1, "Unrecognized uniform name %s!", name.c_str());
            m_CachedUniforms.emplace(name, location);
        }
        return m_CachedUniforms[name];
    }

    bool OpenGLShader::needToRecompile()
    {
        if(m_Combined)
        {
            auto time = std::filesystem::last_write_time(m_StageFilePaths.VertexPath);
            if(m_ShouldCache)
            {
                auto time2 = std::filesystem::last_write_time(m_CacheFilePaths.VertexPath);
                return time > time2;
            }
            bool res = time != m_LastWrittenTime;
            m_LastWrittenTime = time;
            return res;
        }
        return false;
    }
    
    static bool splitCombinedIntoSrcs(const std::string& filepath, ShaderSources o_Srcs)
    {
        std::string combinedSrcs;
        {
            std::ifstream is(filepath);
            RB_ENSURE_RET(is, false, "Failed to open shader file \'%s\'.", filepath.c_str());
            std::stringstream buffer;
            buffer << is.rdbuf();
            combinedSrcs = buffer.str();
            RB_ENSURE_RET(combinedSrcs.size(), false, "Shader file \'%s\' was empty.", filepath.c_str());
        }

        size_t header = combinedSrcs.find("#shader");

        while (header != std::string::npos)
        {
            size_t firstLineEnd = combinedSrcs.find_first_of("\r\n", header);
            RB_ASSERT(firstLineEnd != std::string::npos, "Syntax error, \'#shader\' directive at the end of file.");

            size_t end = combinedSrcs.find_last_not_of(" \t", firstLineEnd - 1);
            RB_ASSERT(end != std::string::npos && end > header + 7, "Shader syntax error, incomplete shader type");

            size_t beg = combinedSrcs.find_last_of(" \t", end - 1) + 1;
            RB_ASSERT(beg != std::string::npos && beg > header + 7, "Syntax error.");
            std::string typeStr = combinedSrcs.substr(beg, end - beg + 1);

            ShaderType type;

            if (typeStr == "vertex" || typeStr == "vert")
                type = ShaderType::Vertex;
            else if (typeStr == "fragment" || typeStr == "frag")
                type = ShaderType::Fragment;
            else if (typeStr == "geometry" || typeStr == "geo")
                type = ShaderType::Geometry;
            else if (typeStr == "compute" || typeStr == "comp")
                type = ShaderType::Compute;
            else if (typeStr == "tesscontrol" || typeStr == "tessc")
                type = ShaderType::TessControl;
            else if (typeStr == "tesseval" || typeStr == "tesse")
                type = ShaderType::TessEvaluation;
            else
            {
                RB_ERROR("Unknown/unsupported shader type.");
                return false;
            }

            RB_ASSERT(o_Srcs[(size_t)type].empty(), "Shader type '%s' was defined twice. Remove one of the definitions.", shaderTypeToString(type));

            size_t nextLine = combinedSrcs.find_first_not_of("\r\n\t ", firstLineEnd + 1);
            RB_ASSERT(nextLine != std::string::npos, "Syntax error");
            header = combinedSrcs.find("#shader", nextLine);

            o_Srcs[(size_t)type] = combinedSrcs.substr(nextLine, (header == std::string::npos ? 
                                                                      combinedSrcs.size() : 
                                                                      header) - nextLine);
        }
        return true;
    }

    static GLuint compileShader(const SPIRVBinaries binaries)
    {
        GLuint programID = glCreateProgram();

        constexpr GLenum INVALID_ID = 0xFFFFFFFD;
        constexpr GLenum INDEX_TO_GL_SHADER_TYPE[SHADER_CNT] = 
        {
            GL_VERTEX_SHADER, 
            GL_TESS_CONTROL_SHADER,
            GL_TESS_EVALUATION_SHADER,
            GL_GEOMETRY_SHADER,
            GL_FRAGMENT_SHADER,
            GL_COMPUTE_SHADER
        };

        GLenum shaderIDList[SHADER_CNT] = { INVALID_ID, INVALID_ID, INVALID_ID, INVALID_ID, INVALID_ID, INVALID_ID };
        size_t listInsert = 0;
        bool compilationSuccess = true;

        for (size_t i = 0; i < SHADER_CNT; ++i)
        {
            if (binaries[i].empty())
                continue;

            GLuint shaderID = glCreateShader(INDEX_TO_GL_SHADER_TYPE[i]);

            glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, binaries[i].data(), binaries[i].size() * sizeof(uint32_t));
            glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);

            GLint compileStatus = 0;
            glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);

            if (!compileStatus)
            {
                glDeleteShader(shaderID);
                RB_ERROR_DEBUG("%s shader failed to compile!", shaderTypeToString((ShaderType)i));
                compilationSuccess = false;
                continue; // We keep going here so that if multiple shaders are bugged we see all errors.
            }
            
            glAttachShader(programID, shaderID);

            shaderIDList[listInsert++] = shaderID;
        }

        if(compilationSuccess)
        {
            glLinkProgram(programID);

            GLint linkStatus = 0;
            glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);
            if (!linkStatus)
            {
                glDeleteProgram(programID);
                RB_ERROR_DEBUG("Program failed to link successfully.");
                programID = (GLuint)-1;
            }
        }
        else
            programID = (GLuint)-1;

        for (int i = 0; i < SHADER_CNT && shaderIDList[i] != INVALID_ID; ++i)
        {
            if(programID != (GLuint)-1)
                glDetachShader(programID, shaderIDList[i]);
            glDeleteShader(shaderIDList[i]);
        }
        

        return programID;
    }

}



// OLD CODE 
/*
namespace Ruby 
{
    namespace Internal 
    {
        void parseShaders(const std::string& fileSource, std::string outShaders[6])
        {
            size_t header = fileSource.find("#shader");
               
            // Tells whether the shader type corresponding with the index has appeared
            // in the file yet or not.
            bool used[6] = { false, false, false, false, false, false };

            while (header != std::string::npos)
            {
                size_t firstLineEnd = fileSource.find_first_of("\r\n", header);
                RB_ASSERT(firstLineEnd != std::string::npos, "Syntax error no line end found.");

                size_t end = fileSource.find_last_not_of(" \t\r\n", firstLineEnd - 1);
                RB_ASSERT(end != std::string::npos && end > header + 7, "Shader syntax error, incomplete shader type");

                size_t beg = fileSource.find_last_of(" \t", end - 1) + 1;
                RB_ASSERT(beg != std::string::npos && beg > header + 7, "Syntax error.");
                std::string type = fileSource.substr(beg, end - beg + 1);

                size_t nextLine = fileSource.find_first_not_of("\r\n\t ", firstLineEnd + 1);
                RB_ASSERT(nextLine != std::string::npos, "Syntax error");
                header = fileSource.find("#shader", nextLine);

                std::string shaderSource = fileSource.substr(nextLine, (header == std::string::npos) ? 
                                                                        fileSource.size() - nextLine : 
                                                                        header - nextLine);
                size_t index;

                if (type == "vertex" || type == "vert")
                    index = 0;
                else if (type == "fragment" || type == "frag")
                    index = 1;
                else if (type == "geometry" || type == "geo")
                    index = 2;
                else if (type == "compute" || type == "comp")
                    index = 3;
                else if (type == "tesscontrol" || type == "tessc")
                    index = 4;
                else if (type == "tesseval" || type == "tesse")
                    index = 5;
                else
                {
                    RB_ERROR("Unknown/unsupported shader type.");
                    continue;
                }

                if (used[index])
                    RB_WARN("Shader type '%s' was defined twice, overwriting first definition. You should remove one of the definitions.", type.c_str());

                used[index] = true;
                outShaders[index] = shaderSource;

            }
        }

        std::string loadShaderFromFile(const std::string& filepath)
        {
            // Input stream is read-only, reads in binary, and starts at the end of the file.
            std::ifstream is(filepath, std::ios::in | std::ios::binary | std::ios::ate);
            std::string source;

            if (is)
            {
                size_t size = is.tellg();
                if (size != -1)
                {
                    // Source is resized to the size of the file, and the entire contents are read in.
                    source.resize(size);
                    is.seekg(0, std::ios::beg);
                    is.read(&source[0], size);
                }
                else
                    RB_ERROR("Unable to read file %s", filepath.c_str());
            }
            else
                RB_ERROR("Unable to open file %s", filepath.c_str());

            return source;

        }
    }

    OpenGLShader::OpenGLShader(const std::string& filepath)
        : OpenGLShader::OpenGLShader("", filepath)
    {
        glslang::InitializeProcess();
        size_t lastSlash = 0, extension = 0;
        for (size_t i = filepath.size(); i-- > 0; )
        {
            if (!lastSlash && (filepath[i] == '\\' || filepath[i] == '/'))
            {
                lastSlash = i + 1;
                break;
            }
            else if (!extension && filepath[i] == '.')
                extension = i;
        }


        m_Name = filepath.substr(lastSlash, extension ? extension - lastSlash : filepath.size() - lastSlash);
        
    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& filepath)
        : m_Name(name), m_Filepath(filepath)
    {
        std::string fileSource = Internal::loadShaderFromFile(filepath);
        std::string shaderSources[6];
        Internal::parseShaders(fileSource, shaderSources);
        compileShader(shaderSources);
    }

    OpenGLShader::~OpenGLShader()
    {
        if(App::instanceExists())
            glDeleteProgram(m_ProgramID);
    }

    void OpenGLShader::compileShader(const std::string collection[6])
    {
        GLuint programID = glCreateProgram();

        // 
        constexpr GLenum INVALID_ID = 0xFFFFFFFD;
        constexpr GLenum INDEX_TO_GL_SHADER_TYPE[6] = 
        {
            GL_VERTEX_SHADER, 
            GL_FRAGMENT_SHADER,
            GL_GEOMETRY_SHADER,
            GL_COMPUTE_SHADER,
            GL_TESS_CONTROL_SHADER,
            GL_TESS_EVALUATION_SHADER
        };
        constexpr const char* INDEX_TO_GL_SHADER_STR[6] =
        {
            "Vertex",
            "Fragment",
            "Geometry",
            "Compute",
            "Tesselation Control",
            "Tesselation Evaluation"
        };

        GLenum shaderIDList[6] = { INVALID_ID, INVALID_ID, INVALID_ID, INVALID_ID, INVALID_ID, INVALID_ID };
        size_t listInsert = 0;

        for (size_t i = 0; i < 6; ++i)
        {
            if (collection[i] == "")
                continue;

            GLuint shaderID = glCreateShader(INDEX_TO_GL_SHADER_TYPE[i]);

            const GLchar* const sourceCStr = collection[i].c_str();
            glShaderSource(shaderID, 1, &sourceCStr, 0);

            glCompileShader(shaderID);

            GLint compileStatus = 0;
            glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);

            if (!compileStatus)
            {
                glDeleteShader(shaderID);

                RB_ERROR_DEBUG("%s shader failed to compile!", INDEX_TO_GL_SHADER_STR[i]);
                break;
            }
            
            glAttachShader(programID, shaderID);

            size_t lineLocation;
            size_t endUniformName = 0;
            while ((lineLocation = collection[i].find("uniform", endUniformName)) != std::string::npos)
            {
                // This should be guaranteed to be found because the shader has already been successfully compiled,
                // and the syntax requires the uniforms be layed out as such when not put into blocks.

                size_t dataType = collection[i].find_first_not_of(" \t", lineLocation + 7);
                size_t uniformName = collection[i].find_first_not_of(" \t", collection[i].find_first_of(" \t", dataType + 1) + 1);
                endUniformName = collection[i].find_first_of(" =([;{\r\n\t", uniformName + 1);
               
                m_CachedUniforms[collection[i].substr(uniformName, endUniformName - uniformName)] = -1;
            }

            shaderIDList[listInsert++] = shaderID;
        }

        glLinkProgram(programID);
        
        GLint linkStatus = 0;
        glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);
        if (!linkStatus)
        {
            glDeleteProgram(programID);
        
            for (int i = 0; i < 6 && shaderIDList[i] != INVALID_ID; ++i)
                glDeleteShader(shaderIDList[i]);
        
            RB_ERROR_DEBUG("Program failed to link successfully.");
            return;
        }

        for (int i = 0; i < 6 && shaderIDList[i] != INVALID_ID; ++i)
            glDetachShader(programID, shaderIDList[i]);

        for (auto& [name, location] : m_CachedUniforms)
            location = glGetUniformLocation(programID, name.c_str());

        m_ProgramID = programID;
    }

    void OpenGLShader::bind() const
    {
        glUseProgram(m_ProgramID);
    }

    void OpenGLShader::unbind() const
    {
        glUseProgram(0);
    }

    void OpenGLShader::setUniformInt(const char* name, int value) const
    {
        glUniform1i(getUniformLocation(name), value);
    }


    void OpenGLShader::setUniformFloat(const char* name, float value) const
    {
        glUniform1f(getUniformLocation(name), value);
    }

    void OpenGLShader::setUniformVec2(const char* name, const glm::vec2& values) const
    {
        glUniform2f(getUniformLocation(name), values.x, values.y);
    }

    void OpenGLShader::setUniformVec3(const char* name, const glm::vec3& values) const
    {
        glUniform3f(getUniformLocation(name), values.x, values.y, values.z);
    }

    void OpenGLShader::setUniformVec4(const char* name, const glm::vec4& values) const
    {
        glUniform4f(getUniformLocation(name), values.x, values.y, values.z, values.w);
    }

    void OpenGLShader::setUniformMat4(const char* name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
    }

    void OpenGLShader::setUniformIntArr(const char* name, uint32_t count, int* arr) const
    {
        glUniform1iv(getUniformLocation(name), (int)count, arr);
    }

    void OpenGLShader::setUniformFloatArr(const char* name, uint32_t count, float* arr) const
    {
        glUniform1fv(getUniformLocation(name), (int)count, arr);
    }
}*/
