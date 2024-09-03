#include "ruby_pch.h"

#include "Renderer3D.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Ruby
{
    namespace Renderer3D
    {
        // The code in this file will not last long, as when
        // I add materials, I will need to use pointers to
        // Meshes, because they will hold shaders, textures,
        // and more data, but for now this is a means of getting things
        // progressed.
        static constexpr size_t MAX_INSTANCE_CNT = 100;
        static glm::mat4* s_Models{nullptr};
        static glm::mat4* s_ModelInsert{nullptr};
        static size_t s_InstanceCount{0};
        static SharedPtr<VertexArray> s_MeshVAO{nullptr};
        static SharedPtr<Shader> s_Shader{nullptr};
        static SharedPtr<UniformBuffer> s_CamUBO{nullptr};
        static SharedPtr<UniformBuffer> s_MeshUBO{nullptr};
        static const PerspCamera* s_CamInUse{nullptr};


        void init()
        {
            clear();

            s_Models = new glm::mat4[MAX_INSTANCE_CNT];
            s_ModelInsert = s_Models;

            const std::string& RubyDir = App::getInstance().getRubyDir();
            s_Shader = Shader::create(RubyDir + "/assets/shaders/mesh.glsl");

            s_CamUBO = UniformBuffer::create(sizeof(glm::mat4), 0);
            s_MeshUBO = UniformBuffer::create(sizeof(glm::mat4) * MAX_INSTANCE_CNT, 1);
        }

        void clear()
        {
            delete[] s_Models;
            s_ModelInsert = nullptr;
            s_CamInUse = nullptr;
            s_MeshVAO = nullptr;
            s_Shader = nullptr;
            s_CamUBO = nullptr;
            s_MeshUBO = nullptr;
            s_InstanceCount = 0;
        }

        void useCamera(const PerspCamera& cam)
        {
            s_CamInUse = &cam;
        }
        
        void setMesh(const Mesh& mesh)
        {
            s_MeshVAO = mesh.getVAO();
            s_ModelInsert = s_Models;
            s_InstanceCount = 0;
        }

        void addInstance(const glm::vec3& position, const glm::vec3& scale, float rotation, const glm::vec3& rotationalAxis)
        {
            if(s_InstanceCount >= MAX_INSTANCE_CNT)
                renderInstanced();

            *s_ModelInsert = glm::mat4(1.0f);
            *s_ModelInsert = glm::translate(*s_ModelInsert, position);
            *s_ModelInsert = glm::rotate(*s_ModelInsert, rotation, rotationalAxis);
            *s_ModelInsert = glm::scale(*s_ModelInsert, scale);

            ++s_ModelInsert;
            ++s_InstanceCount;
        }

        void renderInstanced()
        {
            if(!s_CamInUse)
            {
                RB_ERROR("Renderer was not provided a camera.");
                goto exit;
            }


            if(s_MeshVAO && s_InstanceCount)
            {
                s_CamUBO->setData(&s_CamInUse->getViewProjectionMatrix()[0][0], sizeof(glm::mat4), 0);
                s_MeshUBO->setData(s_Models, sizeof(glm::mat4) * s_InstanceCount, 0);
                s_Shader->bind();
                Context::drawInstanced(s_MeshVAO, s_InstanceCount);
            }
        exit:
            s_ModelInsert = s_Models;
            s_InstanceCount = 0;
        }
    }
}
