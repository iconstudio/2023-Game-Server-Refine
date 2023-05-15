export module Core.User.Basic;
import Net.Socket;
import Core.Session;
import Core.Asynchron;
export import Core.User.Identifier;

export namespace core
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
