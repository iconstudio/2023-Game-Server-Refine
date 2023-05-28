module;
#include "pch.hpp"
#include <memory>

export module Game.Scene;
export import Game.Camera;
import Utility.Singleton;
import System.PipelineObject;

export extern "C" namespace game
{
	class Scene
		: public util::Singleton<Scene>
		, public sys::PipelineObject
		, public std::enable_shared_from_this<Scene>
	{
	public:
		constexpr Scene() noexcept
			: Singleton(this)
			, PipelineObject()
			, enable_shared_from_this()
		{}

		virtual constexpr ~Scene() noexcept = default;

		virtual void Awake() override
		{}

		virtual void Start() override
		{}

		virtual void Update(const float& delta_time) override
		{}

		virtual void LateUpdate(const float& delta_time) override
		{}

		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		std::shared_ptr<Camera> mainCamera;
	};
}
