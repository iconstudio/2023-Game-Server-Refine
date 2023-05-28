module;
#include "pch.hpp"
#include <memory>

export module Game.Scene;
export import Game.Camera;
import Utility.Singleton;
import System.PipelineObject;

export extern "C++" namespace game
{
	template<typename S>
	class Scene;

	template<typename S>
	struct SceneTraits
	{
		using type = S;
		using wrapper = Scene<type>;
		using singletone = util::Singleton<type>;
		using pointer = std::shared_ptr<wrapper>;
	};

	template<typename S>
	class Scene
		: public SceneTraits<S>::singletone
		, public sys::PipelineObject
		, public std::enable_shared_from_this<Scene<S>>
	{
	public:
		using scene_type = SceneTraits<S>::type;
		using wrapper_type = SceneTraits<S>::wrapper;
		using singletone = SceneTraits<S>::singletone;
		using handle_type = SceneTraits<S>::pointer;

		constexpr Scene(S *const& scene) noexcept
			: singletone(scene)
			, PipelineObject()
			, std::enable_shared_from_this<wrapper_type>()
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
