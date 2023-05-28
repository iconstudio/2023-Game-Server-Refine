module;
#include "pch.hpp"
#include <memory>
#include <string>
#include <string_view>

export module Game.Scene;
export import Game.Camera;
import Utility.Singleton;
import System.PipelineObject;

export extern "C++" namespace game
{
	class SceneBasis;

	template<typename S>
	class Scene;

	template<typename S>
	struct SceneTraits
	{
		using type = S;
		using wrapper = Scene<type>;
		using singletone = util::Singleton<type>;
		using pointer = std::shared_ptr<SceneBasis>;
	};

	class SceneBasis
		: public sys::PipelineObject
		, public std::enable_shared_from_this<SceneBasis>
	{
	public:
		constexpr SceneBasis() noexcept = default;

		constexpr SceneBasis(std::string_view name) noexcept
			: PipelineObject()
			, std::enable_shared_from_this<SceneBasis>()
			, myName(name)
		{}

		~SceneBasis() noexcept
		{}

		constexpr const std::string& GetName() const& noexcept
		{
			return myName;
		}

		constexpr std::string&& GetName() && noexcept
		{
			return static_cast<std::string&&>(myName);
		}

		SceneBasis(const SceneBasis& other) = delete;
		SceneBasis(SceneBasis&& other) = delete;
		SceneBasis& operator=(const SceneBasis& other) = delete;
		SceneBasis& operator=(SceneBasis&& other) = delete;

	private:
		std::string myName;
	};

	template<typename S>
	class Scene
		: public SceneBasis
		, public SceneTraits<S>::singletone
	{
	public:
		using scene_type = SceneTraits<S>::type;
		using wrapper_type = SceneTraits<S>::wrapper;
		using singletone = SceneTraits<S>::singletone;
		using handle_type = SceneTraits<S>::pointer;

		constexpr Scene(S *const& scene) noexcept
			: SceneBasis()
			, singletone(scene)
		{}

		constexpr Scene(S* const& scene, std::string_view name) noexcept
			: SceneBasis(name)
			, singletone(scene)
		{}

		~Scene() noexcept
		{
			SceneBasis::~SceneBasis();
		}

		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		std::shared_ptr<Camera> mainCamera;
	};
}
