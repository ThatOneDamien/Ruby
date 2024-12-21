#include "ruby_pch.h"

#include "Renderer3D.h"
#include "Buffers.h"
#include "Camera.h"
#include "Font.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"

#include <glm/gtc/matrix_transform.hpp>
#include <fstream>

namespace Ruby
{
    namespace Renderer3D
    {
        static constexpr size_t MAX_INSTANCE_CNT = 100;
        static constexpr size_t MAX_MATERIAL_CNT = 16;


        static glm::mat4* s_Models{nullptr};
        static glm::mat4* s_ModelInsert{nullptr};
        static size_t s_InstanceCount{0};
        static SharedPtr<VertexArray> s_MeshVAO{nullptr};
        static SharedPtr<Shader> s_Shader{nullptr};
        static SharedPtr<Shader> s_BasicShader{nullptr};
        static SharedPtr<UniformBuffer> s_CamUBO{nullptr};
        static SharedPtr<UniformBuffer> s_MeshUBO{nullptr};
        static SharedPtr<UniformBuffer> s_MatUBO{nullptr};
        static SharedPtr<UniformBuffer> s_LightUBO{nullptr};
        static const PerspCamera* s_CamInUse{nullptr};
        static SharedPtr<Cubemap> s_Cubemap{nullptr};
        static SharedPtr<Shader> s_CubemapShader{nullptr};
        static Mesh* s_CubeMesh{nullptr};


        void init()
        {
            clear();

            s_Models = new glm::mat4[MAX_INSTANCE_CNT];
            s_ModelInsert = s_Models;

            const std::string& RubyDir = App::getInstance().getRubyDir();
            s_Shader = Shader::createCombined(RubyDir + "/assets/shaders/mesh.glsl", false);
            s_CubemapShader = Shader::createCombined(RubyDir + "/assets/shaders/cubemap.glsl", false);

            s_CamUBO = UniformBuffer::create(sizeof(glm::mat4), 0);
            s_MeshUBO = UniformBuffer::create(sizeof(glm::mat4) * MAX_INSTANCE_CNT, 1);
            s_MatUBO = UniformBuffer::create(sizeof(Material) * MAX_MATERIAL_CNT, 2);
            s_LightUBO = UniformBuffer::create(sizeof(glm::vec4) * 2, 3);

            glm::vec3 lightpos = { 0.0f, 0.0f, 0.0f };
            s_LightUBO->setData(&lightpos.x, sizeof(glm::vec3), 0);

            float cubeVertices[] = {
                // positions          
                -1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f,  1.0f,
                -1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f,  1.0f,
                 1.0f, -1.0f, -1.0f,
                 1.0f, -1.0f,  1.0f,
                 1.0f,  1.0f, -1.0f,
                 1.0f,  1.0f,  1.0f,
            };

            uint32_t cubeIndices[] = {
                0, 1, 3,
                3, 2, 0,

                1, 5, 7,
                7, 3, 1,

                5, 4, 6,
                6, 7, 5,

                4, 0, 2,
                2, 6, 4,

                0, 4, 5,
                5, 1, 0,

                3, 7, 6,
                6, 2, 3
            };

            SharedPtr<VertexBuffer> cbmpVBO = VertexBuffer::create(cubeVertices, sizeof(cubeVertices));
            VertexLayout layout;
            layout.push(LayoutType::Float, 3, false);
            cbmpVBO->setLayout(layout);

            SharedPtr<IndexBuffer> cbmpIBO = IndexBuffer::create(cubeIndices, sizeof(cubeIndices) / sizeof(uint32_t));
            SharedPtr<VertexArray> cbmpVAO = VertexArray::create();

            cbmpVAO->setVertexBuffer(cbmpVBO);
            cbmpVAO->setIndexBuffer(cbmpIBO);
            s_CubeMesh = new Mesh(cbmpVAO, {});

        }

        void reload()
        {
            s_Shader->recompile();
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
            s_MatUBO = nullptr;
            s_LightUBO = nullptr;
            s_Cubemap = nullptr;
            s_CubemapShader = nullptr;
            s_CubeMesh = nullptr;
            s_InstanceCount = 0;
        }

        void useCamera(const PerspCamera& cam)
        {
            s_CamInUse = &cam;
        }

        void useCubemap(const SharedPtr<Cubemap>& cubemap)
        {
            s_Cubemap = cubemap;
        }
        
        void setMesh(const Mesh& mesh)
        {
            const std::vector<Material>& mats = mesh.getMaterials();
            RB_ASSERT(mats.size() <= MAX_MATERIAL_CNT, "Mesh has more materials than the limit.");
            s_MeshVAO = mesh.getVAO();
            if(!mats.empty())
                s_MatUBO->setData(mats.data(), sizeof(Material) * mats.size(), 0);
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
                glm::vec4 pos = { s_CamInUse->getPosition(), 0.0f };
                s_LightUBO->setData(&pos, sizeof(glm::vec4), sizeof(glm::vec4));
                s_Shader->bind();
                Context::drawInstanced(s_MeshVAO, s_InstanceCount);
            }
        exit:
            s_ModelInsert = s_Models;
            s_InstanceCount = 0;
        }

        void renderCubemap()
        {
            if(s_Cubemap && s_CamInUse)
            {
                s_CubemapShader->bind();
                glm::mat4 viewProj = s_CamInUse->getProjectionMatrix() * glm::mat4(glm::mat3(s_CamInUse->getViewMatrix()));
                s_CamUBO->setData(&viewProj[0][0], sizeof(glm::mat4), 0);
                Context::setFaceCulling(false);
                Context::drawCall(s_CubeMesh->getVAO());
                Context::setFaceCulling(true);
            }
        }
    }
}
