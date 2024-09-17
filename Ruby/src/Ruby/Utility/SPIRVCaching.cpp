#include "ruby_pch.h"

#include "SPIRVCaching.h"
#include "Ruby/Main/Core.h"
#include "Ruby/Render/Context.h"

#include <glslang/Public/ShaderLang.h>
#include <glslang/Public/ResourceLimits.h>
#include <glslang/SPIRV/spirv.hpp>
#include <glslang/SPIRV/GlslangToSpv.h>
#include <fstream>

namespace Ruby
{
    namespace SPIRV
    {
        static bool s_Initialized = false;

        std::vector<uint32_t> compileToSPIRV(const char* shaderSrc, size_t size, ShaderType type)
        {
            if(!s_Initialized)
                RB_ENSURE(s_Initialized = glslang::InitializeProcess(), "Unable to initialize glslang.");

            glslang::TShader shader{(EShLanguage)type};
            shader.setStringsWithLengths(&shaderSrc, (const int*)&size, 1);
            shader.setEnvInput(glslang::EShSourceGlsl, (EShLanguage)type, glslang::EShClientOpenGL, 450);
            shader.setEnvClient(glslang::EShClientOpenGL, glslang::EShTargetOpenGL_450);
            shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_6);
            shader.setSourceEntryPoint("main");
            shader.setEntryPoint("main"); 
            glslang::TShader::ForbidIncluder inc;
            std::string out;
            if(!shader.preprocess(GetDefaultResources(), 450, EProfile::ECoreProfile, 
                                false, false, EShMsgDefault, &out, inc))
            {
                RB_ERROR("Failed to preprocess %s shader:\n%s", shaderTypeToString(type), shader.getInfoLog());
                return {};
            }
            if(!shader.parse(GetDefaultResources(), 450, false, EShMsgDefault))
            {
                RB_ERROR("Failed to parse %s shader:\n%s", shaderTypeToString(type), shader.getInfoLog());
                return {};
            }
            
            glslang::TProgram prog;

            prog.addShader(&shader);
            if(!prog.link(EShMsgDefault))
            {
                RB_ERROR("Failed to link %s shader:\n%s", shaderTypeToString(type), prog.getInfoLog());
                return {};
            }
            
            std::vector<uint32_t> outData;
            glslang::GlslangToSpv(*prog.getIntermediate((EShLanguage)type), outData);

            return outData;
        }


        // CombinedBinaries getSPIRVOrCompile(const ProgramStages& stages, const ProgramStages& cachePaths)
        // {
        //     CombinedBinaries res;
        //
        //     for(int i = 0; i < 6; ++i)
        //     {
        //         ShaderType type = (ShaderType)i;
        //
        //         if(stages.get(type).empty())
        //             continue;
        //
        //         std::string cacheFilePath = stages.get(type) + ".spv";
        //
        //         std::ifstream is{cacheFilePath, std::ios::in | std::ios::ate | std::ios::binary};
        //         if(is)
        //         {
        //             size_t fileSize = is.tellg();
        //             RB_ENSURE_RET(fileSize > 0, {}, "Found cached shader \'%s\', but it was empty.", cacheFilePath.c_str());
        //             std::vector<uint32_t> tmp(((fileSize - 1) / sizeof(uint32_t)) + 1);
        //             is.seekg(0);
        //             is.read((char*)tmp.data(), fileSize);
        //             res.get(type) = std::move(tmp);
        //         }
        //         else
        //         {
        //             is.open(stages.get(type), std::ios::in | std::ios::ate | std::ios::binary);
        //             RB_ENSURE_RET(is, {}, "Unable to locate shader or its cached counterpart: \'%s\'", stages.get(type).c_str());
        //             size_t fileSize = is.tellg();
        //             RB_ENSURE_RET(fileSize > 0, {}, "Found shader \'%s\', but it was empty.", stages.get(type).c_str());
        //             char* shaderSrc = new char[fileSize];
        //             is.seekg(0);
        //             is.read((char*)shaderSrc, fileSize);
        //             res.get(type) = Utils::compileToSPIRV(shaderSrc, fileSize, type);
        //             delete[] shaderSrc;
        //
        //             if(!res.get(type).empty())
        //             {
        //                 std::ofstream out{cacheFilePath, std::ios::out | std::ios::binary};
        //                 out.write((char*)res.get(type).data(), res.get(type).size() * sizeof(uint32_t));
        //                 out.close();
        //             }
        //         }
        //         is.close();
        //     }
        //
        //     return res;
        // }
        //
        // CombinedBinaries getSPIRVOrCompileCombined(const std::string& srcFilePath)
        // {
        //     return CombinedBinaries{};
        // }
        //
        // CombinedBinaries compileToSPIRV(const ProgramStages& stages)
        // {
        //     CombinedBinaries res;
        //     for(int i = 0; i < 6; ++i)
        //     {
        //         ShaderType type = (ShaderType)i;
        //
        //         if(stages.get(type).empty())
        //             continue;
        //
        //         std::ifstream is{stages.get(type), std::ios::in | std::ios::ate | std::ios::binary};
        //         RB_ENSURE_RET(is, {}, "Unable to locate %s shader: \'%s\'", shaderTypeToString(type), stages.get(type).c_str());
        //         size_t fileSize = is.tellg();
        //         RB_ENSURE_RET(fileSize > 0, {}, "Found shader \'%s\', but it was empty.", stages.get(type).c_str());
        //         char* shaderSrc = new char[fileSize];
        //         is.seekg(0);
        //         is.read((char*)shaderSrc, fileSize);
        //         res.get(type) = Utils::compileToSPIRV(shaderSrc, fileSize, type);
        //         delete[] shaderSrc;
        //
        //     }
        //
        //     return res;
        // }
    }
}
