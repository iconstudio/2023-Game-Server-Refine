export module Net.User.Basic;
import Net.Session;
import Net.Socket;
import Net.Session.Asynchron;
export import Net.User.Identifier;

export namespace net
{
	class [[nodiscard]] BasicUser
		: public Asynchron<BasicUser, userid_t>
	{
	public:
		using base = Asynchron<BasicUser, userid_t>;

		constexpr BasicUser(const userid_t& id) noexcept
			: Asynchron(id)
		{}

		constexpr ~BasicUser() noexcept = default;

		BasicUser(const BasicUser& other) noexcept = delete;
		BasicUser& operator=(const BasicUser& other) noexcept = delete;
	};
}
