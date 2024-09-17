#pragma once

#include "Ruby/Render/Shader.h"

#include <vector>

namespace Ruby
{
    namespace SPIRV
    {
        std::vector<uint32_t> compileToSPIRV(const char* src, size_t size, ShaderType type);
    }
}
