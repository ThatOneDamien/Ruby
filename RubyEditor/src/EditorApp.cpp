#include "EditorApp.h"
#include "Ruby/Scene/Components.h"
#include "Ruby/Utility/PlatformFileUtils.h"
#include <Ruby/Main/MainFunctionEntry.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <ImGui/misc/cpp/imgui_stdlib.h>

namespace Ruby 
{
    static bool s_ShowDebugWindow = false;

    inline void beginImGuiRow(const char* label, float columnWidth)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 0.0f);
        ImGui::PushID(label);
        std::string temp = std::string(label) + "Table";
        ImGui::BeginTable(temp.c_str(), 2, ImGuiTableFlags_BordersInnerV, {0.0f, 0.0f});
        RB_INFO("Column Width: %f", columnWidth);
        ImGui::TableSetupColumn("0", ImGuiTableColumnFlags_WidthFixed, columnWidth);
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text(label);
        ImGui::TableNextColumn();
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 10.0f });
    }

    inline void endImGuiRow()
    {
        ImGui::PopStyleVar(2);
        ImGui::EndTable();
        ImGui::PopID();
    }

    void EditorApp::onStart()
    {
        m_LoadedScene = createShared<Scene>();

        auto& wind = App::getInstance().getWindow();
        uint32_t width = wind.getWidth(), height = wind.getHeight();
        m_FBO = Framebuffer::create(width, height);
        m_InvAspectRatio = (float)height / (float)width;


        m_Cam.setProjection((float)width / (float)height);
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF("res/fonts/Nunito-Regular.ttf", 20);
        io.ConfigFlags &= ~ImGuiConfigFlags_ViewportsEnable;

        Renderer::useCamera(m_Cam);
    }

    void EditorApp::update(double deltaMillis)
    {
        if (m_SceneSelected)
        {
            glm::vec2 pos = m_Cam.getPosition();
            pos.x += m_Scale * 0.02f * (Input::isKeyDown(KeyCode::D) - Input::isKeyDown(KeyCode::A));
            pos.y += m_Scale * 0.02f * (Input::isKeyDown(KeyCode::W) - Input::isKeyDown(KeyCode::S));
            m_Cam.setPosition({pos.x, pos.y, 0.0f});
        }

        Renderer::API::clear();
        m_FBO->bind();
        Renderer::API::clear();
        Renderer::resetBatch();
        if (m_LoadedScene)
            m_LoadedScene->updateScene(deltaMillis);
        Renderer::renderBatch();
        m_FBO->unbind();
    }

    void EditorApp::ImGuiRender(double deltaMillis)
    {
        static bool dockspaceOpen = true;
        static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
            windowFlags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Ruby2D Dockspace", &dockspaceOpen, windowFlags);
        ImGui::PopStyleVar(3);

        // DockSpace
        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, {144.0f, 144.0f});
            ImGuiID dockspace_id = ImGui::GetID("Ruby2D Dockspace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags);
            ImGui::PopStyleVar();
        }

        if(s_ShowDebugWindow)
            ImGui::ShowDemoWindow();


        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open Scene"))
                {
                    std::string openedFile = FileUtils::loadFile("Ruby Scene\0*.rusc");
                    if (!openedFile.empty())
                    {
                        m_LoadedScene = createShared<Scene>(openedFile);
                        openedFile = "Ruby Editor \xE2\x80\xA2 " + openedFile.substr(openedFile.find_last_of("/\\") + 1);
                        App::getInstance().getWindow().setWindowTitle(openedFile.c_str());
                    }
                }
                else if(ImGui::MenuItem("Save Scene"))
                {
                    std::string saveFile = FileUtils::saveFile();
                    RB_INFO("SAVE AT: %s", saveFile.c_str());
                    if(!saveFile.empty())
                    {
                        m_LoadedScene->serialize(saveFile);
                    }
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        // For now not showing the full tree
        // i.e. the parent entities have no effect
        // all entities are at the same level.
        if (ImGui::Begin("Entity Tree")) 
        {
            for (size_t i = 0; i < m_EntityList.size(); ++i)
            {
                if (ImGui::Selectable(m_EntityList[i].Name.c_str(), m_SelectedEntity == i))
                    m_SelectedEntity = i;
            }
            if (ImGui::Button("Add Entity"))
            {
                std::string s("Bruh");
                s += std::to_string(m_EntityList.size() + 1);
                m_EntityList.push_back({ m_LoadedScene->createEntity(), s.c_str() });
            }
        }
        ImGui::End();

        if (ImGui::Begin("Inspector"))
        {
            renderInspector();
        }
        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        if (ImGui::Begin("Scene"))
        {
            m_SceneSelected = ImGui::IsWindowFocused();
            ImVec2 viewportMaxRegion = ImGui::GetWindowContentRegionMax();

            ImGui::Image((ImTextureID)(uint64_t)m_FBO->getTextureID(),
                    ImVec2{ viewportMaxRegion.x, viewportMaxRegion.x * m_InvAspectRatio },
                    { 0.0f, 1.0f }, { 1.0f, 0.0f });
        }
        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::End();
    }

    void EditorApp::onEvent(Event& e)
    {
        switch (e.getType())
        {
            case EventType::WindowResized:
                {
                    WindowResizeEvent& wrEvent = static_cast<WindowResizeEvent&>(e);
                    uint32_t width = wrEvent.getWidth(), height = wrEvent.getHeight();
                    if (width && height)
                    {
                        m_FBO->regenerate(width, height);
                        m_InvAspectRatio = (float)height / (float)width;
                        m_Cam.setProjection((float)width / (float)height);
                    }
                    break;
                }
            case EventType::MouseScroll:
                {
                    if (m_SceneSelected)
                    {
                        MouseScrollEvent& msEvent = static_cast<MouseScrollEvent&>(e);
                        m_Scale -= 0.05f * m_Scale * msEvent.getYOffset();
                        m_Cam.setProjection(1 / m_InvAspectRatio, m_Scale);
                    }
                    break;
                }
            case EventType::KeyPressed:
                {
                    KeyPressedEvent& kpEvent = static_cast<KeyPressedEvent&>(e);
                    if(kpEvent.getKeyCode() == KeyCode::D && Input::isKeyDown(KeyCode::LeftControl))
                        s_ShowDebugWindow = !s_ShowDebugWindow;
                    break;
                }
            default: 
                break;
        }
    }

    void EditorApp::onExit()
    {

    }

    void EditorApp::renderInspector()
    {
        if (m_EntityList.empty() || m_SelectedEntity >= m_EntityList.size())
            return;


        Entity& e = m_EntityList[m_SelectedEntity].EntityID;


        if(ImGui::BeginCombo("Add Component", "", ImGuiComboFlags_NoPreview))
        {
            if(ImGui::MenuItem("Transform") && 
                    !e.hasComponents<Components::Transform>())
            {
                e.addComponent<Components::Transform>(m_Cam.getPosition(), 0.0f, glm::vec2{1.0f, 1.0f});
            }
            else if(ImGui::MenuItem("Sprite") &&
                    !e.hasComponents<Components::Sprite>())
            {
                e.addComponent<Components::Sprite>();
            }
            ImGui::EndCombo();
        }


        float width = ImGui::CalcItemWidth();
        if (e.hasComponents<Components::Transform>())
        {
            Components::Transform& transform = e.getComponent<Components::Transform>();
            if (ImGui::TreeNode("Transform"))
            {
                FloatControl fc[2] = 
                {
                    {"##X", {0.8f, 0.15f, 0.2f, 1.0f}}, 
                    {"##Y", {0.2f, 0.8f, 0.15f, 1.0f}} 
                };
                drawStyledControl("Position", &transform.Position.x, fc, 2, 0.0f, 0.02f, 0.0f, 0.0f, 80.0f);
                beginImGuiRow("Rotation", 80.0f);
                ImGui::DragFloat("", &transform.Rotation, 0.02f, 0.0f, 6.28318f, "%.2f");
                endImGuiRow();
                drawStyledControl("Scale", &transform.Scale.x, fc, 2, 1.0f, 0.02f, 0.0f, FLT_MAX / INT_MAX, 80.0f);
                
                ImGui::TreePop();
            }
        }

        if (e.hasComponents<Components::Sprite>())
        {
            Components::Sprite& sprite = e.getComponent<Components::Sprite>();
            if (ImGui::TreeNode("Sprite"))
            {
                beginImGuiRow("Color", 80.0f);
                ImGui::ColorEdit4("", &sprite.Color.r);
                endImGuiRow();
                // TODO: MAKE THIS LOOK BETTER
                if(ImGui::Button("Texture"))
                {
                    std::string path = FileUtils::loadFile(".png\0.jpg\0.jpeg\0.bmp");
                    if(!path.empty())
                        sprite.Tex = Texture::createTexture(path);
                }
                ImGui::TreePop();
            }
        }
    }
    
    void EditorApp::drawStyledControl(const char* label, float* values, 
                                      const FloatControl* styles, size_t count, 
                                      float resetValue, float dragSpeed,
                                      float minValue, float maxValue, float columnWidth)
    {
        if(count - 2 > 2)
            return;

        beginImGuiRow(label, columnWidth);
        
        ImGui::PushMultiItemsWidths(count, ImGui::CalcItemWidth());

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
        
        for(size_t i = 0; i < count; ++i)
        {
            const glm::vec4& c = styles[i].Color;
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ c.r, c.g, c.b, c.a });
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ c.r + 0.1f, c.g + 0.1f, c.b + 0.1f, c.a });
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ c.r, c.g, c.b, c.a });
            if (ImGui::Button(&styles[i].Label[2], buttonSize))
                values[i] = resetValue;
            ImGui::PopStyleColor(3);

            ImGui::SameLine();
            ImGui::DragFloat(styles[i].Label, values + i, dragSpeed, minValue, maxValue, "%.2f");
            ImGui::PopItemWidth();
            if(i != count - 1)
                ImGui::SameLine();

        }

        endImGuiRow();
    }

}

Ruby::App* createApp(int argc, char** argv)
{
    Ruby::AppSpec spec;
    spec.MainDirectory = "../../../../RubyEditor";
    spec.WinSpec.Name = "Ruby Editor";
    spec.WinSpec.Width = 1280;
    spec.WinSpec.Height = 720;
    spec.WinSpec.MinWidth = 640;
    spec.WinSpec.MinHeight = 360;
    spec.WinSpec.VSync = true;
    spec.WinSpec.HasTitleBar = true;
    return new Ruby::EditorApp(argc, argv, spec);
}
