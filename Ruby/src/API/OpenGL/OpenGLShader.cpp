#include "ruby_pch.h"

#include "API/OpenGL/OpenGLShader.h"

#include <glad/glad.h>
#include <fstream>

namespace Ruby {

    static uint32_t compileShader(uint32_t type, const std::string& src)
    {
        uint32_t id = glCreateShader(type);
        const char* source = src.c_str();
        glShaderSource(id, 1, &source, nullptr);
        glCompileShader(id);

        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE)
        {
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            char* message = (char*)_malloca(length * sizeof(char));
            glGetShaderInfoLog(id, length, &length, message);
            std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
            return 0;
        }
        return id;
    }


    static ShaderComponents loadShader(const std::string& filepath)
    {
        std::ifstream is(filepath);
        std::stringstream s[2];
        std::string line;
        char a = -1;

        if (!is)
        {
            std::cout << "Unable to read file: " << filepath << std::endl;
            return {};
        }

        while (std::getline(is, line))
        {
            if (line.find("#shader") != std::string::npos)
            {
                if (line.find(" vertex") != std::string::npos) a = 0;
                else if (line.find(" fragment") != std::string::npos) a = 1;
            }
            else if (a == -1) return {};
            else
            {
                s[a] << line << '\n';
            }
        }

        return { s[0].str(), s[1].str() };

    }

    OpenGLShader::OpenGLShader(const std::string& filepath)
        : m_Name("Unnamed Shader")
    {
        m_RendererID = glCreateProgram();
        if (!m_RendererID)
        {
            std::cout << "An error occurred when creating the program." << std::endl;
        }

        ShaderComponents components = loadShader(filepath);

        uint32_t vertID = compileShader(GL_VERTEX_SHADER, components.vertShader);
        uint32_t fragID = compileShader(GL_FRAGMENT_SHADER, components.fragShader);

        glAttachShader(m_RendererID, vertID);
        glAttachShader(m_RendererID, fragID);

        glLinkProgram(m_RendererID);
        glValidateProgram(m_RendererID);

        glDeleteShader(vertID);
        glDeleteShader(fragID);
    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertSrc, const std::string& fragSrc)
    {
        m_RendererID = glCreateProgram();
        if (!m_RendererID)
        {
            std::cout << "An error occurred when creating the program." << std::endl;
        }

        uint32_t vertID = compileShader(GL_VERTEX_SHADER, vertSrc);
        uint32_t fragID = compileShader(GL_FRAGMENT_SHADER, fragSrc);

        glAttachShader(m_RendererID, vertID);
        glAttachShader(m_RendererID, fragID);

        glLinkProgram(m_RendererID);
        glValidateProgram(m_RendererID);

        glDeleteShader(m_RendererID);
        glDeleteShader(m_RendererID);
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_RendererID);
    }

    void OpenGLShader::bind() const
    {
        glUseProgram(m_RendererID);
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

    int OpenGLShader::getUniformLocation(const char* name) const
    {
        if (m_CachedUniformLocations.count(name)) return m_CachedUniformLocations[name];

        int res = glGetUniformLocation(m_RendererID, name);
        if (res == -1)
            std::cout << name << " uniform not found. Program ID: " << m_RendererID;

        m_CachedUniformLocations[name] = res;

        return res;
    }
}