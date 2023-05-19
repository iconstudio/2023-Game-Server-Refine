export module Core.User.Basic;
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

		constexpr bool operator==(const BasicUser& other) const noexcept
		{
			return myID == other.myID;
		}

		BasicUser(const BasicUser& other) noexcept = delete;
		BasicUser& operator=(const BasicUser& other) noexcept = delete;
	};
}
