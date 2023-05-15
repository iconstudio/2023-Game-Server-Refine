export module Core.Service.UserManager;
import Utility;
import Utility.Singleton;
import Utility.Array;
import Net.Socket;
import Net.Intrinsics;
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
		UserManager(net::Socket& listener) noexcept;
		~UserManager() noexcept;

		void Awake();
		net::ioError Start() noexcept;

		net::ioError BeginAccept(const userid_t& start);
		void EndAccept() noexcept;
		void EndFailedAccept() noexcept;

	private:
		net::Socket& nameListener;

		util::Array<BasicUser*, maxSessions> everySession;
		util::Array<User*, maxUsers> everyUser;
		util::Array<BasicUser*, maxNPCs> everyNPCs;

		net::Context acceptContext;
		char acceptBuffer[64];
		unsigned long accceptResultSize;
	};
}
