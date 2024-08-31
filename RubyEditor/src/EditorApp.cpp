#include "EditorApp.h"
#include "EditorUtils.h"

#include <Ruby/Main/MainFunctionEntry.h>

#include <ImGui/imgui_internal.h>
// #include <ImGui/misc/cpp/imgui_stdlib.h>

namespace Ruby 
{
    bool        g_ShowDebugWindow{ false };
    ImVec2      g_PrevViewportSize{ 0.0f, 0.0f };
    OrthoCamera g_EditorCam{ 1.0f, 1.0f };
    float       g_CameraZoom{ 1.0f };

    std::string g_ScenePath;
    bool        g_SceneModified{ false };

    size_t      g_SelectedEntity{ ULLONG_MAX };
    bool        g_SceneSelected{ false };

    SharedPtr<Framebuffer>    g_FBO;
    bool                      g_InvalidFB{ false };

    SharedPtr<Scene>          g_LoadedScene;
    std::vector<EditorEntity> g_EntityList;

#include "EditorUtils.h"

    inline void beginImGuiRow(const char* label, float columnWidth)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 0.0f);
        ImGui::PushID(label);
        std::string temp = std::string(label) + "Table";
        ImGui::BeginTable(temp.c_str(), 2, ImGuiTableFlags_BordersInnerV, {0.0f, 0.0f});
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
        Renderer2D::init();
        EditorUtils::openScene("Balls.rusc");

        auto& wind = App::getInstance().getWindow();
        uint32_t width = wind.getWidth(), height = wind.getHeight();
        g_FBO = Framebuffer::create(width, height);


        g_EditorCam.setProjection((float)width / (float)height);
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF("res/fonts/Nunito-Regular.ttf", 20);
        io.ConfigFlags &= ~ImGuiConfigFlags_ViewportsEnable;

        Renderer2D::useCamera(g_EditorCam);
    }

    void EditorApp::update(double deltaMillis)
    {
        if(g_InvalidFB)
        {
            Context::setViewport(0, 0, (int)g_PrevViewportSize.x, (int)g_PrevViewportSize.y);
            g_FBO->regenerate(g_PrevViewportSize.x, g_PrevViewportSize.y);
            g_EditorCam.setProjection(g_PrevViewportSize.x / g_PrevViewportSize.y, g_CameraZoom);
        }

        if (g_SceneSelected)
        {
            glm::vec2 pos = g_EditorCam.getPosition();
            pos.x += g_CameraZoom * 0.02f * (Input::isKeyDown(KeyCode::D) - Input::isKeyDown(KeyCode::A));
            pos.y += g_CameraZoom * 0.02f * (Input::isKeyDown(KeyCode::W) - Input::isKeyDown(KeyCode::S));
            g_EditorCam.setPosition({pos.x, pos.y, 0.0f});
        }

        Context::clear();
        g_FBO->bind();
        Context::clear();
        Renderer2D::resetBatch();
        if (g_LoadedScene)
            g_LoadedScene->updateStatic(deltaMillis);
        Renderer2D::renderBatch();
        g_FBO->unbind();
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

        if(g_ShowDebugWindow)
            ImGui::ShowDemoWindow();


        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open"))
                    EditorUtils::openScene();
                else if(ImGui::MenuItem("Save") && (g_ScenePath.empty() || g_SceneModified))
                    EditorUtils::saveSceneAs(g_ScenePath.empty());
                else if(ImGui::MenuItem("Save As"))
                    EditorUtils::saveSceneAs(true);
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        // For now not showing the full tree
        // i.e. the parent entities have no effect
        // all entities are at the same level.
        if (ImGui::Begin("Entity Tree")) 
        {
            ImVec2 size = ImGui::GetContentRegionAvail();
            if (ImGui::Button("Add Entity", { size.x, 0.0f }))
            {
                if(!g_SceneModified)
                {
                    g_SceneModified = true;
                    EditorUtils::updateWindowTitle();
                }
                std::string s("Bruh");
                s += std::to_string(g_EntityList.size() + 1);
                g_EntityList.push_back({ g_LoadedScene->createEntity(), s.c_str() });
            }
            for (size_t i = 0; i < g_EntityList.size(); ++i)
            {
                if (ImGui::Selectable(g_EntityList[i].Name.c_str(), g_SelectedEntity == i))
                    g_SelectedEntity = i;
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
            g_SceneSelected = ImGui::IsWindowFocused();
            ImVec2 windowRegion = ImGui::GetContentRegionAvail();

            if(windowRegion.x != g_PrevViewportSize.x ||
               windowRegion.y != g_PrevViewportSize.y)
            {
                g_InvalidFB = true;
                g_PrevViewportSize = windowRegion;
            }

            ImGui::Image((ImTextureID)(uint64_t)g_FBO->getTextureID(),
                    ImVec2{ windowRegion.x, windowRegion.y },
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
            case EventType::MouseScroll:
            {
                if (g_SceneSelected)
                {
                    MouseScrollEvent& msEvent = static_cast<MouseScrollEvent&>(e);
                    g_CameraZoom -= 0.05f * g_CameraZoom * msEvent.getYOffset();
                    g_EditorCam.setProjection(g_PrevViewportSize.x / g_PrevViewportSize.y, g_CameraZoom);
                }
                break;
            }
            case EventType::KeyPressed:
            {
                KeyPressedEvent& kpEvent = static_cast<KeyPressedEvent&>(e);
                if(kpEvent.getKeyCode() == KeyCode::D && Input::isKeyDown(KeyCode::LeftControl))
                    g_ShowDebugWindow = !g_ShowDebugWindow;
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
        if (g_EntityList.empty() || g_SelectedEntity >= g_EntityList.size())
            return;


        Entity& e = g_EntityList[g_SelectedEntity].EntityID;


        if(ImGui::BeginCombo("Add Component", "", ImGuiComboFlags_NoPreview))
        {
            if(ImGui::MenuItem("Transform") && 
                    !e.hasComponents<Components::Transform>())
            {
                e.addComponent<Components::Transform>(g_EditorCam.getPosition(), 0.0f, glm::vec2{1.0f, 1.0f});
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

                Components::Transform copy = transform;

                drawStyledControl("Position", &transform.Position.x, fc, 2, 0.0f, 0.02f, 0.0f, 0.0f, 80.0f);
                beginImGuiRow("Rotation", 80.0f);
                float degrees = glm::degrees(transform.Rotation);
                ImGui::DragFloat("", &degrees, 1.0f, 0.0f, 359.0f, "%.0f");
                transform.Rotation = glm::radians(degrees);
                endImGuiRow();
                drawStyledControl("Scale", &transform.Scale.x, fc, 2, 1.0f, 0.02f, 0.0f, FLT_MAX / INT_MAX, 80.0f);
               
                if(!g_SceneModified && 
                   copy.Position != transform.Position || 
                   copy.Scale != transform.Scale ||
                   copy.Rotation != transform.Rotation)
                {
                    g_SceneModified = true;
                    EditorUtils::updateWindowTitle();
                }
            
                ImGui::TreePop();
            }
        }

        if (e.hasComponents<Components::Sprite>())
        {
            Components::Sprite& sprite = e.getComponent<Components::Sprite>();
            if (ImGui::TreeNode("Sprite"))
            {
                Components::Sprite copy = sprite;

                beginImGuiRow("Color", 80.0f);
                ImGui::ColorEdit4("", &sprite.Color.r);
                endImGuiRow();
                // TODO: MAKE THIS LOOK BETTER
                if(ImGui::Button("Texture"))
                {
                    std::string path = FileUtils::loadFile(".png\0.jpg\0.jpeg\0.bmp");
                    if(!path.empty())
                        sprite.Tex = Texture::create(path);
                }

                if(!g_SceneModified && 
                   copy.Color != sprite.Color || 
                   copy.Tex != sprite.Tex)
                {
                    g_SceneModified = true;
                    EditorUtils::updateWindowTitle();
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
    spec.RubyDirectory = "../Ruby";
    spec.WinSpec.Name = "Ruby Editor";
    spec.WinSpec.Width = 1280;
    spec.WinSpec.Height = 720;
    spec.WinSpec.MinWidth = 640;
    spec.WinSpec.MinHeight = 360;
    spec.WinSpec.VSync = true;
    spec.WinSpec.HasTitleBar = true;
    return new Ruby::EditorApp(argc, argv, spec);
}
