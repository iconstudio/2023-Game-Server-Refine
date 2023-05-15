export module Core.User;
import Net;
import Net.Socket;
import Core.Session;
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
		constexpr User(const userid_t& id) noexcept
			: BasicUser(id)
		{}

		constexpr ~User() noexcept
		{}

		User(const User& other) = delete;
		User(User&& other) = delete;
		User& operator=(const User& other) = delete;
		User& operator=(User&& other) = delete;
	};
}
