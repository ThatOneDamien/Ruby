#pragma once

#include "Ruby/Main/Core.h"

#include <glm/glm.hpp>
#include <unordered_map>
#include <string>

namespace Ruby 
{
    enum class ShaderType
    {
        Vertex,
        TessControl,
        TessEvaluation,
        Geometry,
        Fragment,
        Compute,
    };

    struct ProgramStages
    {
        const char* VertexPath{nullptr};
        const char* FragmentPath{nullptr};
        const char* GeometryPath{nullptr};
        const char* ComputePath{nullptr};
        const char* TessControlPath{nullptr};
        const char* TessEvaluationPath{nullptr};
    };

    class RB_NOVTABLE Shader
    {
    public:
        virtual ~Shader() = default;

        virtual void bind() const = 0;

        virtual void setUniformInt     (const char* name, int value)                     const = 0;
        virtual void setUniformFloat   (const char* name, float value)                   const = 0;
        virtual void setUniformVec2    (const char* name, const glm::vec2& values)       const = 0;
        virtual void setUniformVec3    (const char* name, const glm::vec3& values)       const = 0;
        virtual void setUniformVec4    (const char* name, const glm::vec4& values)       const = 0;
        virtual void setUniformMat4    (const char* name, const glm::mat4& values)       const = 0;
        virtual void setUniformIntArr  (const char* name, uint32_t count, int* values)   const = 0;
        virtual void setUniformFloatArr(const char* name, uint32_t count, float* values) const = 0;

        static SharedPtr<Shader> create(const ProgramStages& stageSrcs, const ProgramStages& stageCaches); 
    };
}
