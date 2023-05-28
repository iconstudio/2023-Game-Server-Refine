module;
#include "pch.hpp"
#include <memory>

export module Game.Scene;
export import Game.Camera;
import Utility.Singleton;
import System.PipelineObject;

export extern "C" namespace game
{
	class Scene : public util::Singleton<Scene>, public sys::PipelineObject
	{
	public:
		std::shared_ptr<Camera> mainCamera;
	};
}
