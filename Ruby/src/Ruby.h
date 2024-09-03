#pragma once

// Tells logger to log with the client logger
#define RB_EXTERNAL_DEF


// Core components
#include "Ruby/Utility/Pointers.h"
#include "Ruby/Utility/PlatformFileUtils.h"
#include "Ruby/Utility/ObjUtils.h"
#include "Ruby/Main/Core.h"
#include "Ruby/Main/Logging.h"
#include "Ruby/Main/Time.h"
#include "Ruby/Main/App.h"
#include "Ruby/Main/Window.h"
#include "Ruby/Main/Input.h"



// Rendering
#include "Ruby/Render/Context.h"
#include "Ruby/Render/Renderer2D.h"
#include "Ruby/Render/Renderer3D.h"
#include "Ruby/Render/Buffers.h"
#include "Ruby/Render/VertexArray.h"
#include "Ruby/Render/Framebuffer.h"
#include "Ruby/Render/Mesh.h"
#include "Ruby/Render/Shader.h"
#include "Ruby/Render/Texture.h"
#include "Ruby/Render/Camera.h"
#include "Ruby/Render/Font.h"


// Scene
#include "Ruby/Scene/Scene.h"
#include "Ruby/Scene/Entity.h"
#include "Ruby/Scene/Components.h"


// IMGUI
#include "Ruby/GUI/ImGuiUtil.h"


// Audio
#include "Ruby/Audio/Audio.h"
