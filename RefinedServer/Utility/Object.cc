import <string>;
import <string_view>;
import <vector>;
import Utility.Constraints;
import Utility.Classifier;
import Utility.Meta;
import Utility.Indexer;
import Utility.Named;

namespace util
{
	/// <summary>
	/// Reflected base class for all objects.
	/// </summary>
	class Reflection :
		public Named,
		public Classifier<Reflection>,
		public Indexer<Reflection>
	{
	public:
		constexpr Reflection() noexcept = default;
		constexpr ~Reflection() noexcept = default;

		template<typename Self>
		consteval std::string_view GetName(this Self&& self) noexcept
		{

		}


		constexpr Reflection(const Reflection& other) noexcept = default;
		constexpr Reflection& operator=(const Reflection& other) noexcept = default;
		constexpr Reflection(Reflection&& other) noexcept = default;
		constexpr Reflection& operator=(Reflection&& other) noexcept = default;
	};
}

template<>
struct util::class_id<util::Reflection>
{
	static constexpr size_t value = static_cast<size_t>(-1);
};
