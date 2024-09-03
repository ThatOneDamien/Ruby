#pragma once

#include "Ruby/Render/Shader.h"

typedef unsigned int GLenum;
//typedef std::unordered_map<GLenum, std::string> ShaderCollection;

namespace Ruby 
{
    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& filepath);
        OpenGLShader(const std::string& name, const std::string& filepath);
        virtual ~OpenGLShader();

        virtual void bind() const override;
        virtual void unbind() const override;

        virtual inline const std::string& getName() const override { return m_Name; }

        virtual void setUniformInt(const char* name, int value) const override;
        virtual void setUniformFloat(const char* name, float value) const override;
        virtual void setUniformVec2(const char* name, const glm::vec2& values) const override;
        virtual void setUniformVec3(const char* name, const glm::vec3& values) const override;
        virtual void setUniformVec4(const char* name, const glm::vec4& values) const override;
        virtual void setUniformMat4(const char* name, const glm::mat4& mat) const override;
        virtual void setUniformIntArr(const char* name, uint32_t count, int* arr) const override;
        virtual void setUniformFloatArr(const char* name, uint32_t count, float* arr) const override;

    private:
        void compileShader(const std::string collection[6]);
        inline int getUniformLocation(const std::string& name) const
        {
            RB_ENSURE_RET(m_CachedUniforms.count(name), -1, "Unrecognized uniform name '%s'", name.c_str());
            return m_CachedUniforms[name];
        }

        RendererID m_ProgramID;
        std::string m_Name, m_Filepath;
        mutable std::unordered_map<std::string, int> m_CachedUniforms;
    };
}
