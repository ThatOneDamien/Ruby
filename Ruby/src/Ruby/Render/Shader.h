#pragma once

#include <glm/glm.hpp>
#include <unordered_map>
#include <string>

namespace Ruby {

    class Shader
    {
    public:
        virtual ~Shader() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual inline const std::string& getName() const = 0;

        virtual void setUniformInt(const char* name, int value) const = 0;
        virtual void setUniformIntArray(const char* name, uint32_t count, int* arr) const = 0;
        virtual void setUniformFloat(const char* name, float value) const = 0;
        virtual void setUniformFloatArray(const char* name, uint32_t count, float* arr) const = 0;
        virtual void setUniformFloat2(const char* name, float v0, float v1) const = 0;
        virtual void setUniformFloat3(const char* name, float v0, float v1, float v2) const = 0;
        virtual void setUniformFloat4(const char* name, float v0, float v1, float v2, float v3) const = 0;
        virtual void setUniformMat4(const char* name, const glm::mat4& mat) const = 0;
        /*
        * @param filepath Path to file which will be loaded and parsed into one or multiple shaders.
        * @return Returns SharedPtr of a shader object that will not be added to the shader library.
        * 
        */
        static SharedPtr<Shader> createShader(const std::string& filepath);
        static SharedPtr<Shader> createShader(const std::string& name, const std::string& filepath);
        static SharedPtr<Shader> createShader(const std::string& name, const std::string& vertSrc, const std::string& fragSrc);

        static inline void addShaderToLibrary(const SharedPtr<Shader>& shader) { s_ShaderLibrary[shader->getName()] = shader; }
        static inline void addShaderToLibrary(const std::string& name, const SharedPtr<Shader>& shader) { s_ShaderLibrary[name] = shader; }
        static SharedPtr<Shader> createAndAddShaderToLibrary(const std::string& filepath);
        static SharedPtr<Shader> createAndAddShaderToLibrary(const std::string& name, const std::string& filepath);
        static SharedPtr<Shader> createAndAddShaderToLibrary(const std::string& name, const std::string& vertSrc, const std::string& fragSrc);

        static inline SharedPtr<Shader> getShaderFromLibrary(const std::string& name)
        {
            RB_ASSERT(shaderLibraryContains(name), "Shader not found in library.");
            return s_ShaderLibrary[name];
        }

        static inline bool shaderLibraryContains(const std::string& name) 
        { 
            return s_ShaderLibrary.find(name) != s_ShaderLibrary.end(); 
        }

    private:
        static std::unordered_map<std::string, SharedPtr<Shader>> s_ShaderLibrary;
    };
}