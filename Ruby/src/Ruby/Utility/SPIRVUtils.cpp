#include "ruby_pch.h"

#include "SPIRVUtils.h"
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
    }
}
