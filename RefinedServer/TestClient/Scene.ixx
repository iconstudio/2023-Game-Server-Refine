module;
#include "pch.hpp"

export module Game.Scene;
export module Game.Camera;
import Utility.Singleton;
import System.PipelineObject;

export namespace game
{
	class Scene : public util::Singleton<Scene>, public sys::PipelineObject
	{
	public:

	};
}
