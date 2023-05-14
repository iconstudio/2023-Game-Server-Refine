#pragma once
import Utility;
import Utility.Singleton;
import Utility.Array;
import Net;
import Net.Socket;

namespace service
{
	class UserManager : util::Singleton<UserManager>
	{
		static inline constexpr size_t maxNPCs = 1000;
		static inline constexpr size_t maxUsers = 1000;

	public:
		UserManager() noexcept;
		~UserManager() noexcept;

		void Awake();
		void Start() noexcept;

		util::Array<net::BasicSession*, maxNPCs> everySession;
		util::Array<net::User*, maxUsers> everyUser;
	};
}
