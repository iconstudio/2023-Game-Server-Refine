export module Game.Scene.Basis;
import <string>;
import <memory>;
import Utility.Named;
import Utility.Indexer;

export namespace game
{
	/// <summary>
	/// satisfies sys::PipelineModel
	/// </summary>
	class SceneBasis
		: public util::Named
		, public util::Indexer<SceneBasis>
	{
	public:
		constexpr SceneBasis() noexcept = default;

		constexpr SceneBasis(std::string_view name) noexcept
			: Named(name)
			, Indexer()
		{}

		constexpr ~SceneBasis() noexcept = default;

		SceneBasis(const SceneBasis& other) = delete;
		SceneBasis(SceneBasis&& other) = delete;
		SceneBasis& operator=(const SceneBasis& other) = delete;
		SceneBasis& operator=(SceneBasis&& other) = delete;
	};

	using SceneHandle = std::shared_ptr<SceneBasis>;
}
