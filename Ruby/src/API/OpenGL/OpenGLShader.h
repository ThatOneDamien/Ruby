#pragma once

#include "Ruby/Render/Shader.h"

namespace Ruby {

    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& filepath);
        OpenGLShader(const std::string& name, const std::string& vertSrc, const std::string& fragSrc);
        virtual ~OpenGLShader();

        virtual void bind() const override;
        virtual void unbind() const override;

        virtual void setUniformInt(const char* name, int value) const override;
        virtual void setUniformIntArray(const char* name, uint32_t count, int* arr) const override;
        virtual void setUniformFloat(const char* name, float value) const override;
        virtual void setUniformFloatArray(const char* name, uint32_t count, float* arr) const override;
        virtual void setUniformFloat2(const char* name, float v0, float v1) const override;
        virtual void setUniformFloat3(const char* name, float v0, float v1, float v2) const override;
        virtual void setUniformFloat4(const char* name, float v0, float v1, float v2, float v3) const override;
        virtual void setUniformMat4(const char* name, const glm::mat4& mat) const override;

        int getUniformLocation(const char* name) const;

    private:
        uint32_t m_RendererID;
        std::string m_Filepath, m_Name;
        mutable std::unordered_map<const char*, int> m_CachedUniformLocations;
    };
}
