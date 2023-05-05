export module Net.Context:Operations;

export extern "C" namespace net
{
	enum class Operation
	{
		NONE = 0,
		ACCEPT,
		/// <summary>
		/// 새로 접속한 유저에 ID 부여 및 ID 송신
		/// </summary>
		WELCOME,
		RECV,
		SEND,
		/// <summary>
		/// 현재 세션에 대해서만 소켓 닫기 및 리소스 해제를 수행
		/// </summary>
		CLOSE,

		PACKET_PROCESS,

		ROOM_CREATE,
		ROOM_CREATE_FAILED,

		ROOM_JOIN,
		ROOM_JOIN_NOTIFY,
		ROOM_JOIN_BLOCKED,

		ROOM_LEAVE,

		ROOM_BROADCASTING,
		ROOM_BROADCAST_END,

		DATABASE,
		TIMER,

		/// <summary>
		/// [TCP] 게임 준비 신호
		/// </summary>
		MARK_USER_READY,
		MARK_USER_UNREADY,
		/// <summary>
		/// [TCP] 방의 준비 명령
		/// </summary>
		GAME_LOADING,
		GAME_CANNOT_LOAD,
		GAME_VOTE,
		/// <summary>
		/// [TCP] 서버가 모든 유저에게 동시에 보내는 신호
		/// </summary>
		GAME_START,
		CREATE_CHARACTERS,
		GAME_FINISH,

		/// <summary>
		/// 완전한 소켓 종료, 현재는 사용하지 않음
		/// </summary>
		DISPOSE = 255
	};
}
