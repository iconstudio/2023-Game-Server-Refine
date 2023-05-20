module;
#include <utility>
#include <string>
#include <string_view>

export module Net.Addressing;
export import Net.EndPoint;

export namespace net
{
	class [[nodiscard]] alignas(unsigned long long) Address
	{
	public:
		explicit constexpr Address(const AddressFamily& family, const char(&ip)[16], const unsigned short& port) noexcept
			: isStatic(true)
			, addressFamily(family), protocolType(IProtocolTypes::TCP)
			, myIP(), myPort(std::move(port))
		{
			if (std::is_constant_evaluated())
			{
				std::copy(ip, ip + sizeof(myIP), myIP);
			}
			else
			{
				std::memcpy(myIP, ip, sizeof(myIP));
			}
		}

		explicit constexpr Address(const AddressFamily& family, const IProtocolTypes& protocol, const unsigned short& port) noexcept
			: isStatic(true)
			, addressFamily(family), protocolType(protocol)
			, myIP(), myPort(port)
		{}

		inline EndPoint Serialize() const noexcept
		{
			if (isStatic)
			{
				if (IProtocolTypes::TCP == protocolType)
				{
					return EndPoint::CreateStaticTCP(addressFamily, myPort);
				}
				else if (IProtocolTypes::UDP == protocolType)
				{
					return EndPoint::CreateStaticUDP(addressFamily, myPort);
				}
			}

			return EndPoint::Create(addressFamily, myIP, myPort);
		}

		inline constexpr AddressFamily GetFamily() const noexcept
		{
			return addressFamily;
		}

		inline constexpr IProtocolTypes GetProtocol() const noexcept
		{
			return protocolType;
		}

		inline constexpr std::string_view GetIP() const noexcept
		{
			return myIP;
		}

		inline constexpr unsigned short GetPort() const noexcept
		{
			return myPort;
		}

		inline constexpr bool IsStatic() const noexcept
		{
			return isStatic;
		}

		constexpr Address(const Address& other) noexcept = default;
		constexpr Address(Address&& other) noexcept = default;
		constexpr Address& operator=(const Address& other) noexcept = default;
		constexpr Address& operator=(Address&& other) noexcept = default;
		constexpr ~Address() noexcept = default;

	private:
		bool isStatic;
		AddressFamily addressFamily;
		IProtocolTypes protocolType;
		char myIP[16];
		unsigned short myPort;
	};
}
