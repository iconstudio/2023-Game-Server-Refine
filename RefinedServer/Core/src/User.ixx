export module Core.User;
import Net.Socket;
import Net.Context;
import Net.Promise;
import Core.User.Basic;
export import Core.User.Identifier;

export namespace core
{
	class User
		: public BasicUser
	{
	protected:
		static inline constexpr size_t BufferSize = 512;

	public:
		constexpr User(const userid_t& id, net::Socket&& socket) noexcept
			: BasicUser(id)
			, mySocket(static_cast<net::Socket&&>(socket))
		{}

		~User() noexcept = default;

		inline net::Proxy Welcome() noexcept
		{
			return BeginWelcome().if_then([this]() noexcept {
				// end immediately (sent welcome message just now)
				EndWelcome();
			}).or_else([this]() noexcept {
				// end failure immediately
				EndFailedWelcome();
			});
		}

		net::Proxy BeginWelcome() noexcept
		{
			if (!IsTaken())
			{
				return net::io::failure;
			}

			if (!SetOperation(Operation::NONE, Operation::WELCOME))
			{
				return net::io::failure;
			}

			// immediately send welcome message
			return net::io::success;
		}

		inline bool EndWelcome() noexcept
		{
			Clear();

			return SetOperation(Operation::WELCOME, Operation::NONE);
		}

		void EndFailedWelcome() noexcept
		{
			Clear();

			SetOperation(Operation::NONE);
		}

		void Cleanup() noexcept
		{
			Clear();
		}

		User(const User& other) = delete;
		User(User&& other) = delete;
		User& operator=(const User& other) = delete;
		User& operator=(User&& other) = delete;

		net::Socket mySocket;
	};
}
