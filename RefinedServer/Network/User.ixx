export module Net.User;
import Net.Session;
import Net.Socket;
import Net.Session.Asynchron;
import Net.User.Basic;
export import Net.User.Identifier;

export namespace net
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

		constexpr ~User() noexcept = default;

	};
}
