#include "ruby_pch.h"

#include "API/OpenGL/OpenGLShader.h"

#include "Ruby/Main/Core.h"

#include <glad/glad.h>
#include <fstream>


namespace Ruby {

    using ShaderCollection = std::unordered_map<GLenum, std::string>;

    static ShaderCollection parseShaders(const std::string& fileSourceCode)
    {
        ShaderCollection shaders;

        size_t header = fileSourceCode.find("#shader");

        while (header != std::string::npos)
        {
            size_t firstLineEnd = fileSourceCode.find_first_of("\r\n", header);
            RB_ASSERT(firstLineEnd != std::string::npos, "Syntax error no line end found.");

            size_t end = fileSourceCode.find_last_not_of(" \t\r\n", firstLineEnd-1);
            RB_ASSERT(end != std::string::npos && end > header + 7, "Shader syntax error, incomplete shader type");

            size_t beg = fileSourceCode.find_last_of(" \t", end - 1) + 1;
            RB_ASSERT(beg != std::string::npos && beg > header + 7, "Syntax error.");
            std::string type = fileSourceCode.substr(beg, end - beg + 1);

            size_t nextLine = fileSourceCode.find_first_not_of("\r\n\t ", firstLineEnd + 1);
            RB_ASSERT(nextLine != std::string::npos, "Syntax error");
            header = fileSourceCode.find("#shader", nextLine);

            GLenum typeEnum = GL_NONE;

            if (type == "vertex")
                typeEnum = GL_VERTEX_SHADER;
            else if (type == "fragment")
                typeEnum = GL_FRAGMENT_SHADER;
            else if (type == "geometry" || type == "geo")
                typeEnum = GL_GEOMETRY_SHADER;
            else
                RB_ASSERT(false, "Unknown/unsupported shader type.");

            shaders[typeEnum] = header == std::string::npos ? fileSourceCode.substr(nextLine, fileSourceCode.size() - nextLine)
                                                            : fileSourceCode.substr(nextLine, header - nextLine);
        }

        return shaders;
    }

    static std::string loadShaderFromFile(const std::string& filepath)
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
                RB_ERROR("Unable to read file %s", filepath);
        }
        else
            RB_ERROR("Unable to open file %s", filepath);

        return source;

    }

    OpenGLShader::OpenGLShader(const std::string& filepath)
        : m_Filepath(filepath)
    {
        std::string fileSourceCode = loadShaderFromFile(filepath);
        ShaderCollection shaderCollection = parseShaders(fileSourceCode);
        compileShader(shaderCollection);

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
        : m_Filepath(filepath), m_Name(name)
    {
        std::string fileSourceCode = loadShaderFromFile(filepath);
        ShaderCollection shaderCollection = parseShaders(fileSourceCode);
        compileShader(shaderCollection);
    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertSrc, const std::string& fragSrc)
        : m_Name(name)
    {
        ShaderCollection shaderCollection;
        shaderCollection[GL_VERTEX_SHADER] = vertSrc;
        shaderCollection[GL_FRAGMENT_SHADER] = fragSrc;
        compileShader(shaderCollection);

    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_ProgramID);
    }

    void OpenGLShader::compileShader(const ShaderCollection& collection)
    {
        GLuint programID = glCreateProgram();

        RB_ASSERT(collection.size() < 4, "Number of shaders exceeds 3, which is the max number supported.");

        // The number 4294967293 is used to identify unused shader indexes, as no ID should
        // be 4294967293. 
        GLenum shaderIDList[3] = { 4294967293, 4294967293, 4294967293 };
        int listInsert = 0;

        for (const auto& [type, source] : collection)
        {
            GLuint shaderID = glCreateShader(type);

            const GLchar* sourceCStr = source.c_str();
            glShaderSource(shaderID, 1, &sourceCStr, 0);

            glCompileShader(shaderID);

            GLint compileStatus = 0;
            glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);

            if (!compileStatus)
            {
                GLint length;
                glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
                GLchar message[1000];
                glGetShaderInfoLog(shaderID, 1000, &length, message);

                glDeleteShader(shaderID);

                const char* shaderTypeString = "Vertex Shader";
                if (type == GL_FRAGMENT_SHADER)
                    shaderTypeString = "Fragment Shader";
                else if (type == GL_GEOMETRY_SHADER)
                    shaderTypeString = "Geometry Shader";

                RB_ERROR("%s failed to compile!", shaderTypeString);
                RB_ASSERT(false, "%s", message);
                break;
            }
            
            glAttachShader(programID, shaderID);

            size_t lineLocation = source.find("uniform");
            while (lineLocation != std::string::npos && source.find_last_not_of(" \t\r\n", lineLocation - 1) == source.find_last_of(';', lineLocation - 1))
            {
                // This should be guaranteed to be found because the shader has already been successfully compiled,
                // and the syntax requires the uniforms be layed out as such when not put into blocks.

                size_t dataType = source.find_first_not_of(" \t", lineLocation + 7);
                size_t uniformName = source.find_first_not_of(" \t", source.find_first_of(" \t", dataType + 1) + 1);
                size_t endUniformName = source.find_first_of(" =([;{\r\n\t", uniformName + 1);
               
                m_CachedUniforms[source.substr(uniformName, endUniformName - uniformName)] = -1;
                lineLocation = source.find("uniform", endUniformName);
            }

            shaderIDList[listInsert++] = shaderID;
        }

        glLinkProgram(programID);
        
        GLint linkStatus = 0;
        glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);
        if (!linkStatus)
        {
            GLint length;
            glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &length);
            GLchar message[1000];
            glGetProgramInfoLog(programID, 1000, &length, message);
        
            glDeleteProgram(programID);
        
            for (int i = 0; i < 3 && shaderIDList[i] != 4294967293; i++)
                glDeleteShader(shaderIDList[i]);
        
            RB_ERROR("Program failed to link successfully.");
            RB_ASSERT(false, "%s", message);
            return;
        }

        for (int i = 0; i < 3 && shaderIDList[i] != 4294967293; i++)
            glDetachShader(programID, shaderIDList[i]);

        for (auto& pair : m_CachedUniforms)
            pair.second = glGetUniformLocation(programID, pair.first.c_str());

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

    void OpenGLShader::setUniformIntArray(const char* name, uint32_t count, int* arr) const
    {
        glUniform1iv(getUniformLocation(name), (int)count, arr);
    }

    void OpenGLShader::setUniformFloat(const char* name, float value) const
    {
        glUniform1f(getUniformLocation(name), value);
    }

    void OpenGLShader::setUniformFloatArray(const char* name, uint32_t count, float* arr) const
    {
        glUniform1fv(getUniformLocation(name), (int)count, arr);
    }

    void OpenGLShader::setUniformFloat2(const char* name, float v0, float v1) const
    {
        glUniform2f(getUniformLocation(name), v0, v1);
    }

    void OpenGLShader::setUniformFloat3(const char* name, float v0, float v1, float v2) const
    {
        glUniform3f(getUniformLocation(name), v0, v1, v2);
    }

    void OpenGLShader::setUniformFloat4(const char* name, float v0, float v1, float v2, float v3) const
    {
        glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
    }

    void OpenGLShader::setUniformMat4(const char* name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
    }
}