module;
#define NOMINMAX
#include <winsock2.h>
#include <type_traits>
#include <format>

export module Net.Context;
#pragma warning(push)
#pragma warning(disable: 4005 5106)
import Utility;
import Utility.Atomic;
export import :Operations;

namespace net
{
	export using ::_OVERLAPPED;
	export using ::OVERLAPPED;

	export class [[nodiscard]] Context : public OVERLAPPED
	{
	public:
		constexpr Context() noexcept
			: Context(Operation::NONE)
		{}

		constexpr Context(Context&& other) noexcept
			: OVERLAPPED(), myOperation(util::move(other.myOperation))
		{}

		explicit constexpr Context(const Operation& op) noexcept
			: OVERLAPPED(), myOperation(op)
		{}

		explicit constexpr Context(Operation&& op) noexcept
			: OVERLAPPED(), myOperation(util::move(op))
		{}

		Context& operator=(Context&& other) noexcept
		{
			myOperation = util::move(other.myOperation);
			return *this;
		}

		constexpr ~Context() noexcept = default;

		inline constexpr void Clear() & noexcept
		{
			static_cast<OVERLAPPED&>(*this) = OVERLAPPED{};
		}

		inline bool SetOperation(Operation prev_op, const Operation& op) & noexcept
		{
			return util::atomic_compare_exchange_strong
			(
				reinterpret_cast<util::atomic<Operation>*>(&myOperation),
				&prev_op,
				op
			);
		}

		inline bool SetOperation(Operation prev_op, Operation&& op) & noexcept
		{
			return util::atomic_compare_exchange_strong
			(
				reinterpret_cast<util::atomic<Operation>*>(&myOperation),
				&prev_op,
				util::move(op)
			);
		}

		inline constexpr Context& SetOperation(const Operation& op) & noexcept
		{
			myOperation = op;

			return *this;
		}

		inline constexpr Context& SetOperation(Operation&& op) & noexcept
		{
			myOperation = util::move(op);

			return *this;
		}

		inline constexpr const Operation& GetOperation() const& noexcept
		{
			return myOperation;
		}

		inline constexpr Operation&& GetOperation() && noexcept
		{
			return util::move(myOperation);
		}

		Context(const Context& other) noexcept = delete;
		Context& operator=(const Context& other) noexcept = delete;

		Operation myOperation;
	};

	inline namespace ctx_names
	{
		constexpr util::string_view caption_none = "NONE";
		constexpr util::string_view caption_accept = "ACCEPT";
		constexpr util::string_view caption_recv = "RECV";
		constexpr util::string_view caption_send = "SEND";
		constexpr util::string_view caption_close = "CLOSE";

		constexpr util::string_view caption_welcome = "WELCOME";
		constexpr util::string_view caption_dispose = "DISPOSE";

		constexpr util::string_view caption_rm_created = "ROOM_CREATE";
		constexpr util::string_view caption_rm_join = "ROOM_JOIN";
		constexpr util::string_view caption_rm_nojoin = "ROOM_JOIN_BLOCKED";
		constexpr util::string_view caption_rm_left = "ROOM_LEAVE";
		constexpr util::string_view caption_rm_bcast = "ROOM_BROADCASTING";

		constexpr util::string_view caption_game_start = "GAME_START";
		constexpr util::string_view caption_user_ready = "USER_READY";
		constexpr util::string_view caption_user_unready = "USER_UNREADY";
		constexpr util::string_view caption_loading = "MAKE_GAMESCENE_LOAD";
		constexpr util::string_view caption_create_chrs = "CREATE_CHARACTERS";
		constexpr util::string_view caption_voted = "GAME_LOADING_DONE";
		constexpr util::string_view caption_game_done = "GAME_FINISH";

		constexpr util::string_view caption_db = "DATABASE";

		constexpr util::string_view caption_unknown = "UNKNOWN";
	}

	export inline constexpr util::string_view ToString(const Operation& op) noexcept
	{
		switch (op)
		{
			case Operation::NONE:
			return ctx_names::caption_none;

			case Operation::ACCEPT:
			return ctx_names::caption_accept;

			case Operation::RECV:
			return ctx_names::caption_recv;

			case Operation::SEND:
			return ctx_names::caption_send;

			case Operation::CLOSE:
			return ctx_names::caption_close;

			case Operation::WELCOME:
			return ctx_names::caption_welcome;

			case Operation::DISPOSE:
			return ctx_names::caption_dispose;

			case Operation::GAME_START:
			return ctx_names::caption_game_start;

			case Operation::MARK_USER_READY:
			return ctx_names::caption_user_ready;

			case Operation::MARK_USER_UNREADY:
			return ctx_names::caption_user_unready;

			case Operation::GAME_LOADING:
			return ctx_names::caption_loading;

			case Operation::GAME_VOTE:
			return ctx_names::caption_voted;

			case Operation::GAME_FINISH:
			return ctx_names::caption_game_done;

			case Operation::ROOM_CREATE:
			return ctx_names::caption_rm_created;

			case Operation::ROOM_CREATE_FAILED:
			return ctx_names::caption_rm_created;

			case Operation::ROOM_JOIN:
			return ctx_names::caption_rm_join;

			case Operation::ROOM_JOIN_BLOCKED:
			return ctx_names::caption_rm_nojoin;

			case Operation::ROOM_LEAVE:
			return ctx_names::caption_rm_left;

			case Operation::ROOM_BROADCASTING:
			return ctx_names::caption_rm_bcast;

			case Operation::DATABASE:
			return ctx_names::caption_db;

			case Operation::CREATE_CHARACTERS:
			return ctx_names::caption_create_chrs;

			default:
			{
				return ctx_names::caption_unknown;
			}
		}
	}
}

export namespace std
{
	template<>
	struct formatter<net::Operation> : formatter<util::string>
	{
		inline auto format(const net::Operation op, format_context& context) const noexcept
		{
			return format_to(context.out(), "{}", net::ToString(op));
		}
	};

	template<>
	struct formatter<net::Context> : formatter<util::string>
	{
		inline auto format(const net::Context& ct, format_context& context) const noexcept
		{
			return format_to(context.out(), "Context<{}>", net::ToString(ct.GetOperation()));
		}
	};

	template<>
	struct hash<net::Context>
	{
		[[nodiscard]]
		size_t operator()(const net::Context& ctx) const noexcept
		{
			return _Hash_representation(ctx.Internal + ctx.Offset + static_cast<unsigned long long>(ctx.GetOperation()));
		}
	};

	template<>
	struct hash<net::Context*>
	{
		[[nodiscard]]
		size_t operator()(const net::Context* ctx) const noexcept
		{
			return _Hash_representation(ctx);
		}
	};
}

namespace net
{
	static void test_basic_context()
	{
		Context ctx1{ Operation::WELCOME };
		const Context ctx2{ Operation::RECV };
		constexpr Context ctx3{ Operation::SEND };

		const auto caption1 = ToString(Operation::WELCOME);
		//Context ctx4{ ctx3 };
		//const Context ctx5{ ctx3 };
		//constexpr Context ctx6{ ctx3 };

		//constexpr Context ctx7{ ctx2 };
		//constexpr Context ctx8{ ctx2 };
		//constexpr Context ctx9{ ctx2 };

		//constexpr Context ctx10{ util::move(ctx2) };
		//constexpr Context ctx11{ util::move(ctx2) };
		//constexpr Context ctx12{ util::move(ctx2) };

		constexpr Context ctx13{ Context{} };
		constexpr Context ctx14{ Context{} };
		constexpr Context ctx15{ Context{} };

		//constexpr Context ctx16{ util::move(ctx3) };
		//constexpr Context ctx17{ util::move(ctx3) };
		//constexpr Context ctx18{ util::move(ctx3) };
	}
}
#pragma warning(pop)
