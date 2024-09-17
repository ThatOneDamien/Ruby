#pragma once

#include "Ruby/Main/Core.h"

#include <glm/glm.hpp>
#include <unordered_map>
#include <string>

namespace Ruby 
{
    enum class ShaderType
    {
        Vertex = 0,
        TessControl,
        TessEvaluation,
        Geometry,
        Fragment,
        Compute,
        NUMBER_OF_SHADER_TYPES
    };

    const char* shaderTypeToString(ShaderType type);

    struct ProgramStages
    {
        std::string VertexPath;
        std::string TessControlPath;
        std::string TessEvaluationPath;
        std::string GeometryPath;
        std::string FragmentPath;
        std::string ComputePath;

        inline const std::string& get(size_t index) const
        {
            RB_ASSERT(index < (size_t)ShaderType::NUMBER_OF_SHADER_TYPES, "Index out of bounds.");
            return *((&VertexPath) + index);
        }
    };

    class RB_NOVTABLE Shader
    {
    public:
        virtual ~Shader() = default;

        virtual void bind() const = 0;
        virtual bool recompile(bool force = false) = 0;

        virtual void setUniformInt     (const char* name, int value)                     const = 0;
        virtual void setUniformFloat   (const char* name, float value)                   const = 0;
        virtual void setUniformVec2    (const char* name, const glm::vec2& values)       const = 0;
        virtual void setUniformVec3    (const char* name, const glm::vec3& values)       const = 0;
        virtual void setUniformVec4    (const char* name, const glm::vec4& values)       const = 0;
        virtual void setUniformMat4    (const char* name, const glm::mat4& values)       const = 0;
        virtual void setUniformIntArr  (const char* name, uint32_t count, int* values)   const = 0;
        virtual void setUniformFloatArr(const char* name, uint32_t count, float* values) const = 0;

        // static SharedPtr<Shader> create(const ProgramStages& stagePaths, const ProgramStages& cachePaths = {}); 
        static SharedPtr<Shader> createCombined(const std::string& filepath, 
                                                bool shouldCache = true, 
                                                const std::string& cachePath = "");
    };
}
