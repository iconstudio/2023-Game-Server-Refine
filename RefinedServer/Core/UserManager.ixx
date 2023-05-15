export module Core.Service.UserManager;
import Utility;
import Utility.Singleton;
import Utility.Array;
import Net.Socket;
import Core.User;
import Core.User.Basic;

export namespace core::service
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

		util::Array<BasicUser*, maxSessions> everySession;
		util::Array<User*, maxUsers> everyUser;
		util::Array<BasicUser*, maxNPCs> everyNPCs;
	};
}
