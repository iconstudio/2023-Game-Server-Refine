export module Utility.Datagram.Packer;
import Utility;
import Utility.Constraints;
import Utility.Memory;
import Utility.Array;
import Utility.Union;
import Utility.Datagram.Serialization;

export namespace util
{
	template<typename... Ts>
	class [[nodiscard]] DataPacker
	{
	public:
		constexpr DataPacker() noexcept = default;
		constexpr ~DataPacker() noexcept = default;

		DataPacker(Ts&&... args) noexcept
		{}

		char myBuffer[1024];
	};

	template<typename... Ts>
	DataPacker(Ts...) -> DataPacker<Ts...>;
}
