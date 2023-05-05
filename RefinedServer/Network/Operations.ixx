export module Net.Context:Operations;

export extern "C" namespace net
{
	enum class Operation
	{
		NONE = 0,
		ACCEPT,
		/// <summary>
		/// ���� ������ ������ ID �ο� �� ID �۽�
		/// </summary>
		WELCOME,
		RECV,
		SEND,
		/// <summary>
		/// ���� ���ǿ� ���ؼ��� ���� �ݱ� �� ���ҽ� ������ ����
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
		/// [TCP] ���� �غ� ��ȣ
		/// </summary>
		MARK_USER_READY,
		MARK_USER_UNREADY,
		/// <summary>
		/// [TCP] ���� �غ� ���
		/// </summary>
		GAME_LOADING,
		GAME_CANNOT_LOAD,
		GAME_VOTE,
		/// <summary>
		/// [TCP] ������ ��� �������� ���ÿ� ������ ��ȣ
		/// </summary>
		GAME_START,
		CREATE_CHARACTERS,
		GAME_FINISH,

		/// <summary>
		/// ������ ���� ����, ����� ������� ����
		/// </summary>
		DISPOSE = 255
	};
}
