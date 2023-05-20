export module Core.User;
import Net.Socket;
import Core.User.Basic;
export import Core.User.Identifier;

export namespace core
{
	class [[nodiscard]] User
		: public BasicUser
	{
	protected:
		static inline constexpr size_t BufferSize = 512;

	public:
		constexpr User(const userid_t& id, net::Socket&& socket) noexcept
			: BasicUser(id)
			//, mySocket(static_cast<net::Socket&&>(socket))
		{}

		~User() noexcept = default;

		User(const User& other) = delete;
		User(User&& other) = delete;
		User& operator=(const User& other) = delete;
		User& operator=(User&& other) = delete;

		//net::Socket mySocket;
	};
}
