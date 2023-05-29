export module Game.Scene.Basis;
import <string>;
import <memory>;
import System.PipelineObject;

export namespace game
{
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

	using SceneHandle = std::shared_ptr<SceneBasis>;
}
