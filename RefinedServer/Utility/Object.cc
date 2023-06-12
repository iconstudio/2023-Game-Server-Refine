import <vector>;
import Utility.Constraints;
import Utility.Classifier;
import Utility.Indexer;
import Utility.Named;

namespace util
{
	/// <summary>
	/// Reflected base class for all objects.
	/// </summary>
	class Object :
		public Named,
		public Classifier<Object>,
		public Indexer<Object>
	{
	public:
		constexpr ~Object() noexcept = default;

		constexpr Object& operator=(const Object& other) noexcept = default;
		constexpr Object& operator=(Object&& other) noexcept = default;

	protected:
		inline Object() noexcept
			: Named()
			, Classifier<Object>()
			, Indexer<Object>()
		{
			everyInstance.push_back(this);
		}

		constexpr Object(const Object& other) noexcept
			: Named(other.GetName())
			, Classifier<Object>()
			, Indexer<Object>(other)
		{
			everyInstance.push_back(this);
		}

		constexpr Object(Object&& other) noexcept
			: Named(static_cast<Object&&>(other).GetName())
			, Classifier<Object>()
			, Indexer<Object>(static_cast<Object&&>(other).GetIndex())
		{
			everyInstance.push_back((this));
		}

		static inline std::vector<Object*> everyInstance{};
	};
}

template<>
struct util::class_id<util::Object>
{
	static constexpr size_t value = static_cast<size_t>(-1);
};
