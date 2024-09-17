#pragma once

#include "Ruby/Render/Shader.h"
#include "Ruby/Utility/SPIRVCaching.h"

#include <vector>

typedef unsigned int GLenum;
//typedef std::unordered_map<GLenum, std::string> ShaderCollection;

namespace Ruby 
{
    class OpenGLShader : public Shader
    {
    public:
        // OpenGLShader(const ProgramStages& stagePaths, const ProgramStages& cachePaths = {});
        OpenGLShader(const std::string& filepath, bool shouldCache = true, const std::string& cachePath = "");
        virtual ~OpenGLShader();

        virtual void bind() const override;
        virtual bool recompile(bool force = false) override;

        virtual void setUniformInt(const char* name, int value) const override;
        virtual void setUniformFloat(const char* name, float value) const override;
        virtual void setUniformVec2(const char* name, const glm::vec2& values) const override;
        virtual void setUniformVec3(const char* name, const glm::vec3& values) const override;
        virtual void setUniformVec4(const char* name, const glm::vec4& values) const override;
        virtual void setUniformMat4(const char* name, const glm::mat4& mat) const override;
        virtual void setUniformIntArr(const char* name, uint32_t count, int* arr) const override;
        virtual void setUniformFloatArr(const char* name, uint32_t count, float* arr) const override;

    private:
        int getUniformLocation(const std::string& name) const;

        bool needToRecompile();

        RendererID m_ProgramID;
        ProgramStages m_StageFilePaths;
        ProgramStages m_CacheFilePaths;
        bool m_Combined;
        bool m_ShouldCache;
        std::filesystem::file_time_type m_LastWrittenTime;
        mutable std::unordered_map<std::string, int> m_CachedUniforms;
    };
}
