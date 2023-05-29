export module Utility.Indexer;

export namespace util
{
	/// <summary>
	/// Make a static index for T.
	/// </summary>
	template<typename T>
	class Indexer
	{
	public:
		constexpr Indexer() noexcept
			: myIndex(_index++)
		{}


		[[nodiscard]]
		constexpr size_t GetIndex() const noexcept
		{
			return myIndex;
		}

		constexpr Indexer(Indexer&& other) noexcept = default;
		constexpr Indexer& operator=(Indexer&& other) noexcept = default;
		constexpr Indexer(const Indexer& other) noexcept = default;
		constexpr Indexer& operator=(const Indexer& other) noexcept = default;
		constexpr ~Indexer() noexcept = default;

	private:
		size_t myIndex;
		inline constinit static size_t _index = 0;
	};
}
