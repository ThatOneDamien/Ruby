#pragma once
#include "EditorApp.h"

namespace Ruby
{

    namespace EditorUtils
    {
        inline void updateWindowTitle()
        {
            std::string newTitle = "Ruby Editor \xE2\x80\xA2 " 
                                + g_ScenePath.substr(g_ScenePath.find_last_of("/\\") + 1);
            if(g_SceneModified)
                newTitle += " (Modified)";

            App::getInstance().getWindow().setWindowTitle(newTitle.c_str());
        }

        inline void openScene(std::string filepath = "")
        {
            if(filepath.empty())
                filepath = FileUtils::loadFile("Ruby Scene\0*.rusc");
            RB_ENSURE_RET_VOID(!filepath.empty(), "Attempted to open empty filepath.");
            RB_TRACE("OPENING SCENE: %s", filepath.c_str());
            SharedPtr<Scene> temp = createShared<Scene>();
            if(temp->deserializeFile(filepath))
            {
                g_LoadedScene = temp;
                std::vector<Entity> entities = g_LoadedScene->getAllEntities();
                g_EntityList.clear();

                for(const Entity& e : entities)
                {
                    std::string s = std::string("Bruh") + std::to_string(g_EntityList.size() + 1);
                    g_EntityList.push_back({e, s, ULLONG_MAX});
                }
                g_ScenePath = filepath;
                g_SceneModified = false;
                updateWindowTitle();
                RB_TRACE("OPENED %s!", filepath.c_str());
            }
        }

        inline void saveSceneAs(bool promptUser)
        {
            if(promptUser)
                g_ScenePath = FileUtils::saveFile("");

            RB_ENSURE_RET_VOID(!g_ScenePath.empty(), "Attempted to save file at empty filepath.");
            RB_TRACE("SAVE AT: %s", g_ScenePath.c_str());
            g_LoadedScene->serialize(g_ScenePath);
            g_SceneModified = false;
            updateWindowTitle();
        }
    }

}
