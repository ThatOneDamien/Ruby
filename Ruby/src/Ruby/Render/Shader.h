#pragma once

#include <glm/glm.hpp>
#include <unordered_map>
#include <string>

namespace Ruby {

    struct ShaderComponents
    {
        std::string vertShader, fragShader;
    };

    class Shader
    {
    public:
        virtual ~Shader() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual void setUniformInt(const char* name, int value) const = 0;
        virtual void setUniformFloat(const char* name, float value) const = 0;
        virtual void setUniformFloat2(const char* name, float v0, float v1) const = 0;
        virtual void setUniformFloat3(const char* name, float v0, float v1, float v2) const = 0;
        virtual void setUniformFloat4(const char* name, float v0, float v1, float v2, float v3) const = 0;
        virtual void setUniformMat4(const char* name, const glm::mat4& mat) const = 0;

        static std::shared_ptr<Shader> create(const std::string& filepath);
        static std::shared_ptr<Shader> create(const std::string& name, const std::string& vertSrc, const std::string& fragSrc);

    private:
        uint32_t m_ProgramID, m_VertID, m_FragID;
        std::unordered_map<std::string, int> m_Uniforms;
    };
}