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

		CS_SIGNIN, // 로그인
		CS_SIGNOUT, // 로그아웃
		CS_SIGNUP, // 계정 가입

		CS_REQUEST_ROOMS, // 방 목록 요청
		CS_REQUEST_USERS, // 대기실에서 모든 유저의 목록 요청
		CS_REQUEST_VERSION, // 게임 서버의 버전 요청

		CS_CREATE_A_ROOM, // 새로운 방을 만드는 요청
		CS_DESTROY_A_ROOM, // 방장이 방을 없애는 요청
		CS_JOIN_A_ROOM, // 방을 들어가는 요청
		CS_LEAVE_A_ROOM, // 현재 방을 나가는 요청
		CS_MATCH_A_ROOM, // 유저가 무작위 방에 입장 요청

		CS_DISPOSE, // 현재 상태에서 나가기 (방, 게임 등)
		CS_GAME_START, // 방장의 게임 시작 신호 또는, 멤버의 게임 준비 신호
		CS_GAME_EXIT = CS_DISPOSE, // 유저의 게임을 나가는 신호
		CS_MOVE_CHARACTER = 14, // 캐릭터 이동

		CS_CHAT = 80, // 채팅 메시지

		SC_SIGNIN_SUCCESS = 100, // 로그인 성공 알림
		SC_SIGNIN_FAILURE, // 로그인 실패 알림
		SC_SIGNUP_SUCCESS, // 가입 성공 알림
		SC_SIGNUP_FAILURE, // 가입 실패 알림

		SC_SERVER_INFO, // 종합적인 서버 상태
		SC_RESPOND_ROOMS, // 방 목록 응답
		SC_RESPOND_USERS, // 대기실에서 모든 유저의 목록 응답
		SC_RESPOND_VERSION, // 게임 서버의 버전 응답

		SC_ROOM_CREATED_BY_SELF, // 새로운 방을 만든 것을 알림
		SC_ROOM_CREATED, // 새로운 방이 만들어진 것을 알림
		SC_ROOM_CREATE_FAILED, // 방 만들기가 실패했음을 알림
		SC_ROOM_DESTROYED, // 방이 사라졌음을 알림
		SC_ROOM_JOINED, // 방에 입장했음을 알림
		SC_ROOM_JOIN_FAILED, // 방에 입장했음을 알림
		SC_ROOM_LEAVE, // 자기가 속한 방에서 나갔음을 알림

		SC_GAME_START, // 진짜 게임 시작
		SC_GAME_VICTORY, // 게임 승리
		SC_GAME_DEFEAT, // 게임 패배
		SC_CREATE_VFX, // 시작 효과 생성
		SC_PLAY_SFX, // 효과음 재생
		SC_PLAY_MUSIC, // 음악 재생
		SC_ANIMATION_START, // 애니메이션 시작
		SC_CREATE_PLAYER, // 캐릭터 생성
		SC_CREATE_ENTITY, // 유저 캐릭터 이외의 캐릭터 생성 (적, 함정 등)
		SC_CREATE_OBJET, // 게임에 영향을 끼치지 않는 개체 생성
		SC_MOVE_CHARACTER, // 유저 캐릭터를 이동
		SC_MOVE_ENTITY, // 기타 캐릭터를 이동
		SC_MOVE_OBJET, // 게임에 영향을 끼치지 않는 개체를 이동
		SC_UPDATE_CHARACTER, // 유저를 비롯한 캐릭터의 상태 변경 (사망, 상태이상 등)
		SC_UPDATE_OBJET, // 게임에 영향을 끼치지 않는 개체의 상태 변경
		SC_REMOVE_CHARACTER, // 유저를 비롯한 캐릭터를 삭제
		SC_REMOVE_OBJET, // 게임에 영향을 끼치지 않는 개체를 삭제

		SC_CHAT, // 메시지 (시스템 알림, 방 대화, 대기실 대화, 1:1대화 등)

		SC_MEMBERS, // 방에 있는 유저 목록
		CS_MOVE_CHARACTER_VALID, // 캐릭터 이동에 대한 UDP 검증 응답
		CS_PLAYER_ACTION, // 유저 능력 사용
		/// <summary>
		/// CS_GAME_START -> SC_MAKE_PLAYERS_READY -> CS_PLAYER_LOADING_COMPLETE(s) -> SC_GAME_START
		/// </summary>
		CS_PLAYER_LOADING_COMPLETE,
		/// <summary>
		/// <listheader>게임 시작 과정 요약</listheader>
		/// <list type="number">
		/// <item>멤버가 <seealso cref="CS_GAME_START"/>를 서버에 보낸다. (준비)</item>
		/// <item>방장이 <seealso cref="CS_GAME_START"/>를 서버에 보낸다. (시작)</item>
		/// <item>서버는 <seealso cref="SC_MAKE_PLAYERS_READY"/>를 모든 클라에 보낸다.</item>
		/// <item>클라는 <seealso cref="CS_PLAYER_LOADING_COMPLETE"/>를 서버에 보낸다.</item>
		/// <item>준비가 완료된 클라의 수가 방 안의 멤버 수보다 많으면 <seealso cref="SC_GAME_START"/>를 보낸다.</item>
		/// </list>
		/// </summary>
		SC_MAKE_PLAYERS_READY,
		SC_PLAYER_UPDATE_TRANSFORM, // 변환 컴포넌트 공유
		CS_PLAYER_UPDATE_TRANSFORM, // 변환 컴포넌트 공유
		SC_GAME_STATE, // 방장에게 게임 시작이 가능한지 여부를 알림
		SC_GAME_LOADING_FAILED, // 게임 로딩이 불가능한 상태임을 알림
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

			// 게임 준비 또는 시작
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
