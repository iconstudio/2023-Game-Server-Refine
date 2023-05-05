export module Network.Addressing;
import Utility;
import Network;
export import Network.EndPoint;

export namespace net
{
	class [[nodiscard]] alignas(unsigned long long) Address
	{
	public:
		constexpr Address(const AddressFamily& family, const char(&ip)[16], const unsigned short& port) noexcept
			: addressFamily(family), protocolType(IProtocolTypes::TCP)
			, myIP(), myPort(util::move(port))
			, isStatic(false)
		{
			if (util::is_constant_evaluated())
			{
				util::copy(ip, ip + sizeof(myIP), myIP);
			}
			else
			{
				util::memcpy(myIP, ip, sizeof(myIP));
			}
		}

		constexpr Address(const AddressFamily& family, const IProtocolTypes& protocol, const unsigned short& port) noexcept
			: addressFamily(family), protocolType(protocol)
			, myIP(), myPort(port)
			, isStatic(true)
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

		inline constexpr util::string_view GetIP() const noexcept
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

	private:
		const AddressFamily addressFamily;
		const IProtocolTypes protocolType;
		char myIP[16];
		unsigned short myPort;

		const bool isStatic;
	};
}
