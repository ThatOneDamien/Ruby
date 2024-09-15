#include "ruby_pch.h"

#include "SPIRVCaching.h"
#include "Ruby/Main/Core.h"

#include <glslang/Public/ShaderLang.h>
#include <glslang/Public/ResourceLimits.h>
#include <glslang/SPIRV/spirv.hpp>
#include <glslang/SPIRV/GlslangToSpv.h>
#include <fstream>

namespace Ruby
{
    namespace SPIRV
    {

        namespace Utils
        {
            std::vector<uint32_t> compileToSPIRV(const char* shaderSrc, size_t size, ShaderType type)
            {
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
                    RB_ERROR("%s", shader.getInfoLog());
                if(!shader.parse(GetDefaultResources(), 450, false, EShMsgDefault))
                    RB_ERROR("%s", shader.getInfoLog());
                
                glslang::TProgram prog;
                prog.addShader(&shader);
                prog.link(EShMsgDefault);
                
                std::vector<uint32_t> outData;
                glslang::GlslangToSpv(*prog.getIntermediate((EShLanguage)type), outData);
    
                return outData;
            }
        }

        static bool s_Initialized = false;

        std::vector<uint32_t> getOrCacheSingleGlsl(const std::string& srcFilePath, 
                                          const std::string& cacheFilePath, 
                                          ShaderType type)
        {
            if(!s_Initialized)
                RB_ENSURE(s_Initialized = glslang::InitializeProcess(), "Unable to initialize glslang.");

            std::vector<uint32_t> res;

            std::ifstream is{cacheFilePath, std::ios::ate | std::ios::binary};
            if(is)
            {
                size_t fileSize = is.tellg();
                RB_ENSURE_RET(fileSize > 0, res, "Found cached shader \'%s\', but it was empty.", cacheFilePath.c_str());
                std::vector<uint32_t> tmp(fileSize);
                is.seekg(0);
                is.read((char*)tmp.data(), fileSize);
                res = std::move(tmp);
            }
            else
            {
                is.open(srcFilePath, std::ios::ate | std::ios::binary);
                RB_ENSURE_RET(is, res, "Unable to locate shader or its cached counterpart: \'%s\'", srcFilePath.c_str());
                size_t fileSize = is.tellg();
                RB_ENSURE_RET(fileSize > 0, res, "Found shader \'%s\', but it was empty.", srcFilePath.c_str());
                char* shaderSrc = new char[fileSize];
                is.seekg(0);
                is.read((char*)shaderSrc, fileSize);
                res = Utils::compileToSPIRV(shaderSrc, fileSize, type);
            }

            is.close();
            return res;
        }

        CombinedBinaries getOrCacheCombinedGlsl(const std::string& srcFilePath,
                                            const std::string& cacheFilePath)
        {
            if(!s_Initialized)
                RB_ENSURE(s_Initialized = glslang::InitializeProcess(), "Unable to initialize glslang.");
        }

        std::vector<uint32_t> getSingleSPIRV(const std::string& binFilePath,
                                    ShaderType type)
        {
            if(!s_Initialized)
                RB_ENSURE(s_Initialized = glslang::InitializeProcess(), "Unable to initialize glslang.");

        }

        CombinedBinaries getCombinedSPIRV(const std::string& binFilePath)
        {
            if(!s_Initialized)
                RB_ENSURE(s_Initialized = glslang::InitializeProcess(), "Unable to initialize glslang.");

        }
    }
}
