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

		const size_t myIndex;

	private:
		static size_t _index;
	};
}
