export module Game.Scene.Basis;
import <string>;
import <memory>;
import Utility.Indexer;

export namespace game
{
	/// <summary>
	/// satisfies sys::PipelineModel
	/// </summary>
	class SceneBasis
		: public util::Indexer<SceneBasis>
	{
	public:
		constexpr SceneBasis() noexcept = default;

		constexpr SceneBasis(std::string_view name) noexcept
			: util::Indexer<SceneBasis>()
			, myName(name)
		{}

		constexpr ~SceneBasis() noexcept = default;

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

	using SceneHandle = std::shared_ptr<SceneBasis>;
}
