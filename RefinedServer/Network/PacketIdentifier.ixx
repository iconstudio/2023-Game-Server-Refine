module;
#include <utility>
#include <string>
#include <istream>
#include <format>
#include <limits>
export module Net.Packet.Identifier;

export namespace net
{
	enum class packetid_t : unsigned char
	{
		NONE = 0,

		CS_SIGNIN, // �α���
		CS_SIGNOUT, // �α׾ƿ�
		CS_SIGNUP, // ���� ����

		CS_REQUEST_ROOMS, // �� ��� ��û
		CS_REQUEST_USERS, // ���ǿ��� ��� ������ ��� ��û
		CS_REQUEST_VERSION, // ���� ������ ���� ��û

		CS_CREATE_A_ROOM, // ���ο� ���� ����� ��û
		CS_DESTROY_A_ROOM, // ������ ���� ���ִ� ��û
		CS_JOIN_A_ROOM, // ���� ���� ��û
		CS_LEAVE_A_ROOM, // ���� ���� ������ ��û
		CS_MATCH_A_ROOM, // ������ ������ �濡 ���� ��û

		CS_DISPOSE, // ���� ���¿��� ������ (��, ���� ��)
		CS_GAME_START, // ������ ���� ���� ��ȣ �Ǵ�, ����� ���� �غ� ��ȣ
		CS_GAME_EXIT = CS_DISPOSE, // ������ ������ ������ ��ȣ
		CS_MOVE_CHARACTER = 14, // ĳ���� �̵�

		CS_CHAT = 80, // ä�� �޽���

		SC_SIGNIN_SUCCESS = 100, // �α��� ���� �˸�
		SC_SIGNIN_FAILURE, // �α��� ���� �˸�
		SC_SIGNUP_SUCCESS, // ���� ���� �˸�
		SC_SIGNUP_FAILURE, // ���� ���� �˸�

		SC_SERVER_INFO, // �������� ���� ����
		SC_RESPOND_ROOMS, // �� ��� ����
		SC_RESPOND_USERS, // ���ǿ��� ��� ������ ��� ����
		SC_RESPOND_VERSION, // ���� ������ ���� ����

		SC_ROOM_CREATED_BY_SELF, // ���ο� ���� ���� ���� �˸�
		SC_ROOM_CREATED, // ���ο� ���� ������� ���� �˸�
		SC_ROOM_CREATE_FAILED, // �� ����Ⱑ ���������� �˸�
		SC_ROOM_DESTROYED, // ���� ��������� �˸�
		SC_ROOM_JOINED, // �濡 ���������� �˸�
		SC_ROOM_JOIN_FAILED, // �濡 ���������� �˸�
		SC_ROOM_LEAVE, // �ڱⰡ ���� �濡�� �������� �˸�

		SC_GAME_START, // ��¥ ���� ����
		SC_GAME_VICTORY, // ���� �¸�
		SC_GAME_DEFEAT, // ���� �й�
		SC_CREATE_VFX, // ���� ȿ�� ����
		SC_PLAY_SFX, // ȿ���� ���
		SC_PLAY_MUSIC, // ���� ���
		SC_ANIMATION_START, // �ִϸ��̼� ����
		SC_CREATE_PLAYER, // ĳ���� ����
		SC_CREATE_ENTITY, // ���� ĳ���� �̿��� ĳ���� ���� (��, ���� ��)
		SC_CREATE_OBJET, // ���ӿ� ������ ��ġ�� �ʴ� ��ü ����
		SC_MOVE_CHARACTER, // ���� ĳ���͸� �̵�
		SC_MOVE_ENTITY, // ��Ÿ ĳ���͸� �̵�
		SC_MOVE_OBJET, // ���ӿ� ������ ��ġ�� �ʴ� ��ü�� �̵�
		SC_UPDATE_CHARACTER, // ������ ����� ĳ������ ���� ���� (���, �����̻� ��)
		SC_UPDATE_OBJET, // ���ӿ� ������ ��ġ�� �ʴ� ��ü�� ���� ����
		SC_REMOVE_CHARACTER, // ������ ����� ĳ���͸� ����
		SC_REMOVE_OBJET, // ���ӿ� ������ ��ġ�� �ʴ� ��ü�� ����

		SC_CHAT, // �޽��� (�ý��� �˸�, �� ��ȭ, ���� ��ȭ, 1:1��ȭ ��)

		SC_MEMBERS, // �濡 �ִ� ���� ���
		CS_MOVE_CHARACTER_VALID, // ĳ���� �̵��� ���� UDP ���� ����
		CS_PLAYER_ACTION, // ���� �ɷ� ���
		/// <summary>
		/// CS_GAME_START -> SC_MAKE_PLAYERS_READY -> CS_PLAYER_LOADING_COMPLETE(s) -> SC_GAME_START
		/// </summary>
		CS_PLAYER_LOADING_COMPLETE,
		/// <summary>
		/// <listheader>���� ���� ���� ���</listheader>
		/// <list type="number">
		/// <item>����� <seealso cref="CS_GAME_START"/>�� ������ ������. (�غ�)</item>
		/// <item>������ <seealso cref="CS_GAME_START"/>�� ������ ������. (����)</item>
		/// <item>������ <seealso cref="SC_MAKE_PLAYERS_READY"/>�� ��� Ŭ�� ������.</item>
		/// <item>Ŭ��� <seealso cref="CS_PLAYER_LOADING_COMPLETE"/>�� ������ ������.</item>
		/// <item>�غ� �Ϸ�� Ŭ���� ���� �� ���� ��� ������ ������ <seealso cref="SC_GAME_START"/>�� ������.</item>
		/// </list>
		/// </summary>
		SC_MAKE_PLAYERS_READY,
		SC_PLAYER_UPDATE_TRANSFORM, // ��ȯ ������Ʈ ����
		CS_PLAYER_UPDATE_TRANSFORM, // ��ȯ ������Ʈ ����
		SC_GAME_STATE, // ���忡�� ���� ������ �������� ���θ� �˸�
		SC_GAME_LOADING_FAILED, // ���� �ε��� �Ұ����� �������� �˸�
		SC_STAGE_COUNTDOWN,
		SC_STAGE_START,
	};
}

export namespace std
{
	inline constexpr string to_string(const net::packetid_t& protocol) noexcept
	{
		switch (protocol)
		{
			case net::packetid_t::NONE:
			return "NONE";

			case net::packetid_t::SC_RESPOND_VERSION:
			return "RESPOND_VERSION";

			case net::packetid_t::SC_RESPOND_USERS:
			return "RESPOND_USERS";

			case net::packetid_t::SC_RESPOND_ROOMS:
			return "RESPOND_ROOMS";

			case net::packetid_t::SC_SIGNIN_SUCCESS:
			return "SIGNIN_SUCCESS";

			case net::packetid_t::SC_SIGNIN_FAILURE:
			return "SIGNIN_FAILURE";

			case net::packetid_t::SC_SIGNUP_SUCCESS:
			return "SIGNUP_SUCCESS";

			case net::packetid_t::SC_SIGNUP_FAILURE:
			return "SIGNUP_FAILURE";

			case net::packetid_t::SC_GAME_START:
			return "START";

			case net::packetid_t::SC_GAME_STATE:
			return "STATE";

			case net::packetid_t::SC_GAME_LOADING_FAILED:
			return "FAILED_GAME_LOAD";

			case net::packetid_t::SC_GAME_VICTORY:
			return "VICTORY";

			case net::packetid_t::SC_GAME_DEFEAT:
			return "DEFEAT";

			case net::packetid_t::SC_CREATE_PLAYER:
			return "CREATE_PLAYER";

			case net::packetid_t::SC_CREATE_ENTITY:
			return "CREATE_ENTITY";

			case net::packetid_t::SC_CREATE_OBJET:
			return "CREATE_OBJET";

			case net::packetid_t::SC_MOVE_CHARACTER:
			return "STAMOVE_CHARACTERRT";

			case net::packetid_t::SC_MOVE_ENTITY:
			return "MOVE_ENTITY";

			case net::packetid_t::SC_MOVE_OBJET:
			return "MOVE_OBJET";

			case net::packetid_t::SC_UPDATE_CHARACTER:
			return "UPDATE_CHARACTER";

			case net::packetid_t::SC_PLAYER_UPDATE_TRANSFORM:
			return "UPDATE_TRANSFORM";

			case net::packetid_t::SC_ROOM_CREATED:
			return "CREATE_ROOM";

			case net::packetid_t::SC_ROOM_CREATE_FAILED:
			return "CANNOT_CREATE_ROOM";

			case net::packetid_t::SC_ROOM_JOINED:
			return "JOIN_ROOM";

			case net::packetid_t::SC_ROOM_JOIN_FAILED:
			return "CANNOT_JOIN_ROOM";

			case net::packetid_t::SC_ROOM_LEAVE:
			return "LEAVE_ROOM";

			case net::packetid_t::CS_SIGNIN:
			return "CL:SIGNIN";

			case net::packetid_t::CS_SIGNOUT:
			return "CL:SIGNOUT";

			case net::packetid_t::CS_SIGNUP:
			return "CL:SIGNUP";

			// ���� �غ� �Ǵ� ����
			case net::packetid_t::CS_GAME_START:
			return "CL:READY";

			case net::packetid_t::CS_PLAYER_LOADING_COMPLETE:
			return "CL:LOADING_IS_DONE";

			case net::packetid_t::CS_GAME_EXIT:
			return "CL:EXIT";

			case net::packetid_t::CS_CREATE_A_ROOM:
			return "CL:ROOM_CREATE";

			case net::packetid_t::CS_JOIN_A_ROOM:
			return "CL:ROOM_JOIN";

			case net::packetid_t::CS_MATCH_A_ROOM:
			return "CL:MATCH";

			case net::packetid_t::CS_LEAVE_A_ROOM:
			return "CL:LEAVE";

			case net::packetid_t::CS_MOVE_CHARACTER:
			return "CL:MOVE";

			case net::packetid_t::CS_PLAYER_UPDATE_TRANSFORM:
			return "CL:UPDATE_TFRM";

			case net::packetid_t::CS_PLAYER_ACTION:
			return "CL:ACTION";

			default:
			{
				return "Unknown packetid_t";
			}
		}
	}

	template<>
	struct hash<net::packetid_t>
	{
		[[nodiscard]]
		size_t operator()(const net::packetid_t& protocol) const noexcept
		{
			return hash<unsigned char>::_Do_hash(static_cast<unsigned char>(protocol));
		}
	};

	template<>
	struct formatter<net::packetid_t>
	{
		inline format_parse_context::iterator parse(format_parse_context& context) const noexcept
		{
			return context.begin();
		}

		inline auto format(const net::packetid_t& protocol, format_context& context) const noexcept
		{
			return format_to(context.out(), "{}", to_string(protocol));
		}
	};

	inline ostream& operator<<(ostream& stream, const net::packetid_t& protocol)
	{
		return stream << to_string(protocol);
	}

	inline istream& operator>>(istream& stream, net::packetid_t& protocol)
	{
		unsigned char buffer{};
		stream >> buffer;

		protocol = static_cast<net::packetid_t>(buffer);
		return stream;
	}
}
