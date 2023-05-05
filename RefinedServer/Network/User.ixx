export module Net.User;
import Net.Session;
import Net.Socket;
import Net.Session.Asynchron;
export import Net.User.Identifier;

export namespace net
{
	class [[nodiscard]] User
		: public Asynchron<User, userid_t>
	{
	protected:
		static inline constexpr size_t BufferSize = 512;

	public:
		using base = Asynchron<User, userid_t>;

		constexpr User(const userid_t& id) noexcept
			: Asynchron(id)
		{}

		constexpr ~User() noexcept = default;

	};
}
