#pragma once

#include "Ruby/Render/Shader.h"

#include <vector>

namespace Ruby
{
    namespace SPIRV
    {

        // Data must be freed using delete[] after use

        struct CombinedBinaries
        {
            std::vector<uint32_t> VertexBin;
            std::vector<uint32_t> FragmentBin;
            std::vector<uint32_t> ComputeBin;
            std::vector<uint32_t> GeometryBin;
            std::vector<uint32_t> TessControlBin;
            std::vector<uint32_t> TessEvalBin;
        };

        // Checks if file at cacheFilePath exists, and if so, treats it as
        // a SPIRV binary and reads normally. If it doesn't exist, reads
        // srcFilePath, compiles it with glslang, stores it at cacheFilePath,
        // and outputs the shaderstage.
        std::vector<uint32_t> getOrCacheSingleGlsl(const std::string& srcFilePath, 
                                          const std::string& cacheFilePath, 
                                          ShaderType type);

        // Checks if file at cacheFilePath exists, and if so, treats it as
        // a 'combined' SPIRV binary and reads normally. A combined SPIRV binary
        // has a unique encoding to determine the shader type at runtime, and stores
        // all shader stages for a program in one file. If the cached file doesn't exist, 
        // it reads srcFilePath, compiles it with glslang, stores it at cacheFilePath,
        // and outputs the full composition of shader stages.
        CombinedBinaries getOrCacheCombinedGlsl(const std::string& srcFilePath,
                                            const std::string& cacheFilePath);

        // Checks if the file binFilePath exists, and if so, treates it as a SPIRV
        // binary and reads normally. If it doesn't exist, it prints an error and
        // gives back an invalid shader stage.
        std::vector<uint32_t> getSingleSPIRV(const std::string& binFilePath,
                                    ShaderType type);

        // Checks if the file binFilePath exists, and if so, reads it as a
        // 'combined' SPIRV binary. A combined SPIRV binary has a unique encoding 
        // to determine the shader type at runtime, and stores all shader stages 
        // for a program in one file. If it doesn't exist, it prints an error and
        // gives back a shader stage composition with all invalid shader stages.
        CombinedBinaries getCombinedSPIRV(const std::string& binFilePath);
    }
}
