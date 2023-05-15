export module Core.UserManager;
import Utility;
import Utility.Singleton;
import Utility.Array;
import Net.Socket;
import Net.User;
import Net.User.Basic;

export namespace core
{
	class UserManager : util::Singleton<UserManager>
	{
		static inline constexpr size_t maxSessions = 3000;
		static inline constexpr size_t maxUsers = 1000;
		static inline constexpr size_t maxNPCs = maxSessions - maxUsers;

	public:
		UserManager() noexcept;
		~UserManager() noexcept;

		void Awake();
		void Start() noexcept;

		util::Array<net::BasicUser*, maxSessions> everySession;
		util::Array<net::User*, maxUsers> everyUser;
		util::Array<net::BasicUser*, maxNPCs> everyNPCs;
	};
}
