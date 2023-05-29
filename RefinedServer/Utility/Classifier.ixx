export module Utility.Classifier;

export namespace util
{
	template<typename T>
	struct class_id;

	template<typename T>
	class Classifier
	{
	public:
		[[nodiscard]]
		static consteval size_t GetID() noexcept
		{
			return class_id<T>::template id;
		}
	};
}
