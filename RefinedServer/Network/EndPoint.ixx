module;
#define NOMINMAX
#include <WS2tcpip.h>

export module Net.EndPoint;
import Utility;
import Net;

export namespace net
{
	export namespace abi
	{
		using ::inet_ntop;
		using ::inet_pton;
	}

	enum AddressFamily : int
	{
		IPv4 = AF_INET,
		IPv6 = AF_INET6,
	};

	enum IProtocolTypes : int
	{
		TCP = IPPROTO::IPPROTO_TCP,
		UDP = IPPROTO::IPPROTO_UDP,
	};

	class [[nodiscard]] alignas(unsigned long long) EndPoint
	{
	public:
		constexpr EndPoint() noexcept
			: endPoint()
		{}

		constexpr EndPoint(const ::SOCKADDR_IN& address) noexcept
			: endPoint(address)
		{}

		constexpr EndPoint(::SOCKADDR_IN&& address) noexcept
			: endPoint(util::move(address))
		{}

		constexpr EndPoint(const AddressFamily& family, const ::IN_ADDR& adress, const unsigned short& network_port) noexcept
			: EndPoint()
		{
			endPoint.sin_family = static_cast<unsigned short>(family);
			endPoint.sin_addr = adress;
			endPoint.sin_port = network_port;
		}

		constexpr EndPoint(const AddressFamily& family, ::IN_ADDR&& adress, const unsigned short& network_port) noexcept
			: EndPoint()
		{
			endPoint.sin_family = static_cast<unsigned short>(family);
			endPoint.sin_addr = util::move(adress);
			endPoint.sin_port = network_port;
		}

		constexpr EndPoint(const AddressFamily& family, const unsigned long&& network_serialized_adress, const unsigned short&& network_port) noexcept
			: EndPoint()
		{
			endPoint.sin_family = static_cast<unsigned short>(family);
			endPoint.sin_addr.S_un.S_addr = network_serialized_adress;
			endPoint.sin_port = network_port;
		}

		constexpr EndPoint(const AddressFamily& family, const unsigned long& network_serialized_adress, const unsigned short& network_port) noexcept
			: EndPoint(family, util::move(network_serialized_adress), util::move(network_port))
		{}

		inline constexpr ::SOCKADDR_IN& operator*() & noexcept
		{
			return endPoint;
		}

		inline constexpr const ::SOCKADDR_IN& operator*() const& noexcept
		{
			return endPoint;
		}

		inline constexpr ::SOCKADDR_IN&& operator*() && noexcept
		{
			return util::move(endPoint);
		}

		inline constexpr const ::SOCKADDR_IN&& operator*() const&& noexcept
		{
			return util::move(endPoint);
		}

		inline constexpr ::SOCKADDR_IN* GetAddress() & noexcept
		{
			return util::addressof(endPoint);
		}

		inline constexpr const ::SOCKADDR_IN* GetAddress() const& noexcept
		{
			return util::addressof(endPoint);
		}

		inline constexpr size_t GetSize() const noexcept
		{
			return sizeof(endPoint);
		}

		inline constexpr int GetiSize() const noexcept
		{
			return sizeof(endPoint);
		}

		static inline EndPoint CreateStaticTCP(const AddressFamily& family, const unsigned short& port) noexcept
		{
			return EndPoint{ family, ::htonl(INADDR_ANY), ::htons(port) };
		}

		static inline EndPoint CreateStaticUDP(const AddressFamily& family, const unsigned short& port) noexcept
		{
			return EndPoint{ family, ::htonl(INADDR_LOOPBACK), ::htons(port) };
		}

		static inline EndPoint Create(const AddressFamily& family, const util::string_view ip, const unsigned short& port) noexcept
		{
			::IN_ADDR sr_address{};
			::inet_pton(static_cast<int>(family), ip.data(), util::addressof(sr_address));

			return EndPoint{ family, util::move(sr_address), ::htons(port) };
		}

	protected:
		::SOCKADDR_IN endPoint;
	};
}
