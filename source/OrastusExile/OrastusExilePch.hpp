/**
See licence file in root folder, MIT.txt
*/
#pragma once

#include <CastorUtils/Config/BeginExternHeaderGuard.hpp>
#ifdef _WIN32
#	include <winsock2.h>
#endif
#include <wx/wx.h>
#include <wx/app.h>
#include <wx/cmdline.h>
#include <wx/display.h>
#include <wx/frame.h>
#include <wx/image.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/timer.h>
#include <wx/windowptr.h>
#include <CastorUtils/Config/EndExternHeaderGuard.hpp>

#include <Castor3D/Engine.hpp>
#include <Castor3D/Event/Frame/FrameListener.hpp>
#include <Castor3D/Event/Frame/CpuFunctorEvent.hpp>
#include <Castor3D/Event/UserInput/UserInputListener.hpp>
#include <Castor3D/Material/Material.hpp>
#include <Castor3D/Material/Pass/Pass.hpp>
#include <Castor3D/Model/Mesh/Mesh.hpp>
#include <Castor3D/Model/Mesh/Submesh/Submesh.hpp>
#include <Castor3D/Render/Ray.hpp>
#include <Castor3D/Render/RenderLoop.hpp>
#include <Castor3D/Render/RenderWindow.hpp>
#include <Castor3D/Scene/Camera.hpp>
#include <Castor3D/Scene/Geometry.hpp>
#include <Castor3D/Scene/Scene.hpp>
#include <Castor3D/Scene/SceneFileParser.hpp>
#include <Castor3D/Scene/SceneNode.hpp>
#include <Castor3D/Material/Texture/TextureUnit.hpp>

#include <CastorUtils/Log/Logger.hpp>
#include <CastorUtils/Math/Angle.hpp>
#include <CastorUtils/Math/Point.hpp>
#include <CastorUtils/Math/Quaternion.hpp>
#include <CastorUtils/Math/RangedValue.hpp>

#include <GameEngine/GameEnginePrerequisites.hpp>
#include <GameEngine/Game.hpp>
#include <GameEngine/GridCell.hpp>
#include <GameEngine/ECS/Entity.hpp>

#include <CastorUtils/Config/BeginExternHeaderGuard.hpp>
#include <chrono>
#include <cstdlib>
#include <thread>
#include <vector>
#include <CastorUtils/Config/EndExternHeaderGuard.hpp>
