export module Core.Service.UserManager;
import Utility;
import Utility.Singleton;
import Utility.Array;
import Utility.Monad;
import Net;
import Net.Socket;
import Net.Context;
import Net.Intrinsics;
import Core.User;
import Core.User.Basic;

export namespace core::service
{
	class UserManager : util::Singleton<UserManager>
	{
		static inline constexpr size_t maxSessions = GetMaxSessions();
		static inline constexpr size_t maxUsers = GetMaxUsers();
		static inline constexpr size_t maxNPCs = maxSessions - maxUsers;

	public:
		UserManager(net::Socket& listener) noexcept;
		~UserManager() noexcept;

		void Awake(net::CompletionPort& port);
		void Start() noexcept;

		net::ioError BeginAccept(const userid_t& start);
		void EndAccept() noexcept;
		void EndFailedAccept() noexcept;

		util::Monad<BasicUser*> SessionAt(const size_t& index) noexcept;
		util::Monad<User*> UserAt(const size_t& index) noexcept;
		util::Monad<BasicUser*> NpcAt(const size_t& index) noexcept;

		util::Monad<BasicUser*> SessionOf(const userid_t& id) noexcept;
		util::Monad<User*> UserOf(const userid_t& id) noexcept;
		util::Monad<BasicUser*> NpcOf(const userid_t& id) noexcept;

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
