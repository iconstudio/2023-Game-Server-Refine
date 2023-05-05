module;
#include <utility>
#include <string>
#include <istream>
#include <format>
#include <limits>
export module Net.User.Identifier;

export namespace net
{
	inline consteval unsigned long long GetMaxUsers() noexcept
	{
#if _DEBUG
		return 50;
#else // _DEBUG
		return 1000;
#endif // _DEBUG
	}

	namespace constants
	{
		inline constexpr size_t USERS_MAX = GetMaxUsers();

		inline constexpr unsigned int USER_ID_MIN = 0;
		inline constexpr unsigned int USER_ID_MAX = static_cast<unsigned int>(USERS_MAX);
	}

	enum class userid_t : unsigned int
	{
		begin = 0,
		end = constants::USERS_MAX + 1,
		invalid = std::numeric_limits<unsigned int>::max()
	};

	inline constexpr userid_t operator+(const userid_t& lhs, const userid_t& rhs) noexcept
	{
		return static_cast<userid_t>(std::to_underlying(lhs) + std::to_underlying(rhs));
	}

	inline constexpr userid_t operator-(const userid_t& lhs, const userid_t& rhs) noexcept
	{
		return static_cast<userid_t>(std::to_underlying(lhs) - std::to_underlying(rhs));
	}

	template<std::integral T>
	inline constexpr userid_t operator+(const userid_t& id, const T value) noexcept
	{
		return static_cast<userid_t>(std::to_underlying(id) + value);
	}

	template<std::integral T>
	inline constexpr userid_t operator-(const userid_t& id, const T value) noexcept
	{
		return static_cast<userid_t>(std::to_underlying(id) - value);
	}

	inline constexpr userid_t& operator++(userid_t& id) noexcept
	{
		return id = id + 1;
	}

	inline constexpr userid_t operator++(userid_t& id, int) noexcept
	{
		const userid_t temp = id;
		++id;
		return temp;
	}

	inline constexpr userid_t& operator--(userid_t& id) noexcept
	{
		return id = id - 1;
	}

	inline constexpr userid_t operator--(userid_t& id, int) noexcept
	{
		const userid_t temp = id;
		--id;
		return temp;
	}

	inline constexpr std::strong_ordering operator<=>(const userid_t& lhs, const userid_t& rhs) noexcept
	{
		return std::to_underlying(lhs) <=> std::to_underlying(rhs);
	}

	inline constexpr unsigned long long CastID(const userid_t& id) noexcept
	{
		return static_cast<unsigned long long>(id);
	}
}

export namespace std
{
	inline string to_string(const kj::userid_t& pid) noexcept
	{
		if (kj::userid_t::invalid == pid)
		{
			return "Invalid ID";
		}
		else
		{
			return to_string(std::to_underlying(pid));
		}
	}

	inline string to_string(const kj::account_t& account) noexcept
	{
		if (kj::account_t::invalid == account)
		{
			return "Invalid AC";
		}
		else
		{
			return std::to_string(std::to_underlying(account));
		}
	}

	inline ostream& operator<<(ostream& stream, const kj::userid_t& pid) noexcept
	{
		return stream << to_string(pid);
	}

	inline ostream& operator<<(ostream& stream, const kj::account_t& account) noexcept
	{
		return stream << to_string(account);
	}

	inline istream& operator>>(istream& stream, kj::userid_t& pid) noexcept
	{
		unsigned int buffer{};
		stream >> buffer;

		pid = static_cast<kj::userid_t>(buffer);
		return stream;
	}

	inline istream& operator>>(istream& stream, kj::account_t& account) noexcept
	{
		unsigned long long buffer{};
		stream >> buffer;

		account = static_cast<kj::account_t>(buffer);
		return stream;
	}

	template<>
	struct formatter<kj::userid_t>
	{
		inline format_parse_context::iterator parse(format_parse_context& context) const noexcept
		{
			return context.begin();
		}

		template<int = 0>
		inline auto format(const kj::userid_t& id, format_context& context) const noexcept
		{
			return std::format_to(context.out(), "ID<{}>", to_string(id));
		}
	};

	template<>
	struct formatter<kj::account_t>
	{
		inline format_parse_context::iterator parse(format_parse_context& context) const noexcept
		{
			return context.begin();
		}

		template<int = 0>
		inline auto format(const kj::account_t& account, format_context& context) const noexcept
		{
			return std::format_to(context.out(), "AC<{}>", to_string(account));
		}
	};
}
