export module Net.User;
import Net.Session;
import Net.Socket;

export namespace net
{
	export enum class UserStates : unsigned char
	{
		NONE = 0,
		ACCEPTED,
	};

	inline constexpr UserStates user_npos = UserStates::NONE;

	export class BasicUser
		: public ISession<BasicUser, userid_t>
		, public IManagedStatus<BasicUser, UserStates>
	{
	protected:
		static inline constexpr size_t BufferSize = 512;

	public:
		using base = Asynchron<BasicUser, userid_t>;
		using status_basis = IManagedStatus<BasicUser, UserStates>;
		using managed_status = status_basis::managed_status;
		using status_type = status_basis::status_type;

		constexpr BasicUser(const userid_t& id) noexcept
			: Asynchron(id), IManagedStatus(user_npos)
			, isGameReady(false)
			, accountID(account_t::invalid)
			, roomID(roomid_t::invalid), prevRoomID(roomid_t::invalid)
			, myTransform()
		{}

		~BasicUser() noexc
	};
}
