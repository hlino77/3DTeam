#pragma once
#include "Session.h"
#include "SendBuffer.h"

using PacketHandlerFunc = std::function<bool(PacketSessionRef&, BYTE*, int32)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];


// TODO
enum : uint16
{
	PKT_S_TIME = 1,
	PKT_S_LOGIN = 2,
	PKT_S_OPENLEVEL = 3,
	PKT_S_LEVELSTATE = 4,
	PKT_S_CREATEOBJECT = 5,
	PKT_S_ANIMATION = 6,
	PKT_S_OBJECTINFO = 7,
	PKT_S_STATE = 8,
	PKT_S_COLLIDERSTATE = 9,
	PKT_S_COLLISION = 10,
	PKT_S_USERINFO = 11,
	PKT_S_NEARTARGET = 12,
	PKT_S_SLOWMOTION = 13,
	PKT_S_EVENT = 14,
	PKT_S_HP = 15,
	PKT_S_CREATEPLAYER = 16,
	PKT_S_CHAT = 17,
	PKT_S_MONSTERSTATE = 18,
	PKT_S_PLAYERLEVELMOVE = 19,
	PKT_S_DELETEGAMEOBJECT = 20,
	PKT_S_IDENTITY = 21,
	PKT_S_PARTY = 22,
	PKT_S_NPC = 23,
	PKT_S_NAVIGATION = 24,
	PKT_S_CREATESKILL = 25,
	PKT_S_BUFFSKILL = 26,
	PKT_S_PLAYERTELEPORT = 27,
	PKT_S_CHANGEEQUIP = 28,
	PKT_S_DUNGEANRATIO = 29,
	PKT_S_ESTHERGAGE = 30,
};

//TODO
bool Handle_INVALID_Server(PacketSessionRef& session, BYTE* buffer, int32 len);
bool Handle_S_TIME_Server(PacketSessionRef& session, Protocol::S_TIME& pkt);
bool Handle_S_LOGIN_Server(PacketSessionRef& session, Protocol::S_LOGIN& pkt);
bool Handel_S_OPENLEVEL_Server(PacketSessionRef& session, Protocol::S_OPEN_LEVEL& pkt);
bool Handel_S_LEVELSTATE_Server(PacketSessionRef& session, Protocol::S_LEVEL_STATE& pkt);
bool Handel_S_CREATEOBJECT_Server(PacketSessionRef& session, Protocol::S_CREATE_OBJCECT& pkt);
bool Handel_S_ANIMATION_Server(PacketSessionRef& session, Protocol::S_ANIMATION& pkt);
bool Handel_S_OBJECTINFO_Server(PacketSessionRef& session, Protocol::S_OBJECTINFO& pkt);
bool Handel_S_STATE_Server(PacketSessionRef& session, Protocol::S_STATE& pkt);
bool Handel_S_COLLIDERSTATE_Server(PacketSessionRef& session, Protocol::S_COLLIDERSTATE& pkt);
bool Handel_S_COLLISION_Server(PacketSessionRef& session, Protocol::S_COLLISION& pkt);
bool Handel_S_USERINFO_Server(PacketSessionRef& session, Protocol::S_USERINFO& pkt);
bool Handel_S_NEARTARGET_Server(PacketSessionRef& session, Protocol::S_NEARTARGET& pkt);
bool Handel_S_SLOWMOTION_Server(PacketSessionRef& session, Protocol::S_SLOWMOTION& pkt);
bool Handel_S_EVENT_Server(PacketSessionRef& session, Protocol::S_EVENT& pkt);
bool Handel_S_HP_Server(PacketSessionRef& session, Protocol::S_HP& pkt);
bool Handel_S_CREATEPLAYER_Server(PacketSessionRef& session, Protocol::S_CREATE_PLAYER& pkt);
bool Handel_S_CHAT_Server(PacketSessionRef& session, Protocol::S_CHAT& pkt);
bool Handel_S_MONSTERSTATE_Server(PacketSessionRef& session, Protocol::S_MONSTERSTATE& pkt);
bool Handel_S_PLAYERLEVELMOVE_Server(PacketSessionRef& session, Protocol::S_PLAYERLEVELMOVE& pkt);
bool Handel_S_DELETEGAMEOBJECT_Server(PacketSessionRef& session, Protocol::S_DELETEGAMEOBJECT& pkt);
bool Handel_S_IDENTITY_Server(PacketSessionRef& session, Protocol::S_IDENTITY& pkt);
bool Handel_S_PARTY_Server(PacketSessionRef& session, Protocol::S_PARTY& pkt);
bool Handel_S_NPC_Server(PacketSessionRef& session, Protocol::S_NPC& pkt);
bool Handel_S_NAVIGATION_Server(PacketSessionRef& session, Protocol::S_NAVIGATION& pkt);
bool Handel_S_CREATESKILL_Server(PacketSessionRef& session, Protocol::S_CREATE_SKILL& pkt);
bool Handel_S_BUFFSKILL_Server(PacketSessionRef& session, Protocol::S_BUFF_SKILL& pkt);
bool Handel_S_PLAYERTELEPORT_Server(PacketSessionRef& session, Protocol::S_PLAYERTELEPORT& pkt);
bool Handel_S_CHANGEEQUIP_Server(PacketSessionRef& session, Protocol::S_CHANGEEQUIP& pkt);
bool Handel_S_DUNGEANRATIO_Server(PacketSessionRef& session, Protocol::S_DUNGEANRATIO& pkt);
bool Handel_S_ESTHERGAGE_Server(PacketSessionRef& session, Protocol::S_ESTHER& pkt);


class CServerPacketHandler
{
public:
	// TODO
	static void Init()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = Handle_INVALID_Server;

		GPacketHandler[PKT_S_TIME] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_TIME>(Handle_S_TIME_Server, session, buffer, len); };
		GPacketHandler[PKT_S_OPENLEVEL] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_OPEN_LEVEL>(Handel_S_OPENLEVEL_Server, session, buffer, len); };
		GPacketHandler[PKT_S_LOGIN] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_LOGIN>(Handle_S_LOGIN_Server, session, buffer, len); };
		GPacketHandler[PKT_S_LEVELSTATE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_LEVEL_STATE>(Handel_S_LEVELSTATE_Server, session, buffer, len); };
		GPacketHandler[PKT_S_CREATEOBJECT] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_CREATE_OBJCECT>(Handel_S_CREATEOBJECT_Server, session, buffer, len); };
		GPacketHandler[PKT_S_ANIMATION] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_ANIMATION>(Handel_S_ANIMATION_Server, session, buffer, len); };
		GPacketHandler[PKT_S_OBJECTINFO] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_OBJECTINFO>(Handel_S_OBJECTINFO_Server, session, buffer, len); };
		GPacketHandler[PKT_S_STATE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_STATE>(Handel_S_STATE_Server, session, buffer, len); };
		GPacketHandler[PKT_S_COLLIDERSTATE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_COLLIDERSTATE>(Handel_S_COLLIDERSTATE_Server, session, buffer, len); };
		GPacketHandler[PKT_S_COLLISION] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_COLLISION>(Handel_S_COLLISION_Server, session, buffer, len); };
		GPacketHandler[PKT_S_USERINFO] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_USERINFO>(Handel_S_USERINFO_Server, session, buffer, len); };
		GPacketHandler[PKT_S_NEARTARGET] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_NEARTARGET>(Handel_S_NEARTARGET_Server, session, buffer, len); };
		GPacketHandler[PKT_S_SLOWMOTION] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_SLOWMOTION>(Handel_S_SLOWMOTION_Server, session, buffer, len); };
		GPacketHandler[PKT_S_EVENT] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_EVENT>(Handel_S_EVENT_Server, session, buffer, len); };
		GPacketHandler[PKT_S_HP] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_HP>(Handel_S_HP_Server, session, buffer, len); };
		GPacketHandler[PKT_S_CREATEPLAYER] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_CREATE_PLAYER>(Handel_S_CREATEPLAYER_Server, session, buffer, len); };
		GPacketHandler[PKT_S_CHAT] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_CHAT>(Handel_S_CHAT_Server, session, buffer, len); };
		GPacketHandler[PKT_S_MONSTERSTATE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_MONSTERSTATE>(Handel_S_MONSTERSTATE_Server, session, buffer, len); };
		GPacketHandler[PKT_S_PLAYERLEVELMOVE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_PLAYERLEVELMOVE>(Handel_S_PLAYERLEVELMOVE_Server, session, buffer, len); };
		GPacketHandler[PKT_S_DELETEGAMEOBJECT] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_DELETEGAMEOBJECT>(Handel_S_DELETEGAMEOBJECT_Server, session, buffer, len); };
		GPacketHandler[PKT_S_IDENTITY] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_IDENTITY>(Handel_S_IDENTITY_Server, session, buffer, len); };
		GPacketHandler[PKT_S_PARTY] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_PARTY>(Handel_S_PARTY_Server, session, buffer, len); };
		GPacketHandler[PKT_S_NPC] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_NPC>(Handel_S_NPC_Server, session, buffer, len); };
		GPacketHandler[PKT_S_NAVIGATION] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_NAVIGATION>(Handel_S_NAVIGATION_Server, session, buffer, len); };
		GPacketHandler[PKT_S_CREATESKILL] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_CREATE_SKILL>(Handel_S_CREATESKILL_Server, session, buffer, len); };
		GPacketHandler[PKT_S_BUFFSKILL] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_BUFF_SKILL>(Handel_S_BUFFSKILL_Server, session, buffer, len); };
		GPacketHandler[PKT_S_PLAYERTELEPORT] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_PLAYERTELEPORT>(Handel_S_PLAYERTELEPORT_Server, session, buffer, len); };
		GPacketHandler[PKT_S_CHANGEEQUIP] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_CHANGEEQUIP>(Handel_S_CHANGEEQUIP_Server, session, buffer, len); };
		GPacketHandler[PKT_S_DUNGEANRATIO] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_DUNGEANRATIO>(Handel_S_DUNGEANRATIO_Server, session, buffer, len); };
		GPacketHandler[PKT_S_ESTHERGAGE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_ESTHER>(Handel_S_ESTHERGAGE_Server, session, buffer, len); };

	}

	static bool HandlePacket(PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}

	// TODO
	static SendBufferRef MakeSendBuffer(Protocol::S_TIME& pkt) { return MakeSendBuffer(pkt, PKT_S_TIME); }
	static SendBufferRef MakeSendBuffer(Protocol::S_OPEN_LEVEL& pkt) { return MakeSendBuffer(pkt, PKT_S_OPENLEVEL); }
	static SendBufferRef MakeSendBuffer(Protocol::S_LEVEL_STATE& pkt) { return MakeSendBuffer(pkt, PKT_S_LEVELSTATE); }
	static SendBufferRef MakeSendBuffer(Protocol::S_LOGIN& pkt) { return MakeSendBuffer(pkt, PKT_S_LOGIN); }
	static SendBufferRef MakeSendBuffer(Protocol::S_CREATE_OBJCECT& pkt) { return MakeSendBuffer(pkt, PKT_S_CREATEOBJECT); }
	static SendBufferRef MakeSendBuffer(Protocol::S_ANIMATION& pkt) { return MakeSendBuffer(pkt, PKT_S_ANIMATION); }
	static SendBufferRef MakeSendBuffer(Protocol::S_OBJECTINFO& pkt) { return MakeSendBuffer(pkt, PKT_S_OBJECTINFO); }
	static SendBufferRef MakeSendBuffer(Protocol::S_STATE& pkt) { return MakeSendBuffer(pkt, PKT_S_STATE); }
	static SendBufferRef MakeSendBuffer(Protocol::S_COLLIDERSTATE& pkt) { return MakeSendBuffer(pkt, PKT_S_COLLIDERSTATE); }
	static SendBufferRef MakeSendBuffer(Protocol::S_COLLISION& pkt) { return MakeSendBuffer(pkt, PKT_S_COLLISION); }
	static SendBufferRef MakeSendBuffer(Protocol::S_USERINFO& pkt) { return MakeSendBuffer(pkt, PKT_S_USERINFO); }
	static SendBufferRef MakeSendBuffer(Protocol::S_NEARTARGET& pkt) { return MakeSendBuffer(pkt, PKT_S_NEARTARGET); }
	static SendBufferRef MakeSendBuffer(Protocol::S_SLOWMOTION& pkt) { return MakeSendBuffer(pkt, PKT_S_SLOWMOTION); }
	static SendBufferRef MakeSendBuffer(Protocol::S_EVENT& pkt) { return MakeSendBuffer(pkt, PKT_S_EVENT); }
	static SendBufferRef MakeSendBuffer(Protocol::S_HP& pkt) { return MakeSendBuffer(pkt, PKT_S_HP); }
	static SendBufferRef MakeSendBuffer(Protocol::S_CREATE_PLAYER& pkt) { return MakeSendBuffer(pkt, PKT_S_CREATEPLAYER); }
	static SendBufferRef MakeSendBuffer(Protocol::S_CHAT& pkt) { return MakeSendBuffer(pkt, PKT_S_CHAT); }
	static SendBufferRef MakeSendBuffer(Protocol::S_MONSTERSTATE& pkt) { return MakeSendBuffer(pkt, PKT_S_MONSTERSTATE); }
	static SendBufferRef MakeSendBuffer(Protocol::S_PLAYERLEVELMOVE& pkt) { return MakeSendBuffer(pkt, PKT_S_PLAYERLEVELMOVE); }
	static SendBufferRef MakeSendBuffer(Protocol::S_DELETEGAMEOBJECT& pkt) { return MakeSendBuffer(pkt, PKT_S_DELETEGAMEOBJECT); }
	static SendBufferRef MakeSendBuffer(Protocol::S_IDENTITY& pkt) { return MakeSendBuffer(pkt, PKT_S_IDENTITY); }
	static SendBufferRef MakeSendBuffer(Protocol::S_PARTY& pkt) { return MakeSendBuffer(pkt, PKT_S_PARTY); }
	static SendBufferRef MakeSendBuffer(Protocol::S_NPC& pkt) { return MakeSendBuffer(pkt, PKT_S_NPC); }
	static SendBufferRef MakeSendBuffer(Protocol::S_NAVIGATION& pkt) { return MakeSendBuffer(pkt, PKT_S_NAVIGATION); }
	static SendBufferRef MakeSendBuffer(Protocol::S_CREATE_SKILL& pkt) { return MakeSendBuffer(pkt, PKT_S_CREATESKILL); }
	static SendBufferRef MakeSendBuffer(Protocol::S_BUFF_SKILL& pkt) { return MakeSendBuffer(pkt, PKT_S_BUFFSKILL); }
	static SendBufferRef MakeSendBuffer(Protocol::S_PLAYERTELEPORT& pkt) { return MakeSendBuffer(pkt, PKT_S_PLAYERTELEPORT); }
	static SendBufferRef MakeSendBuffer(Protocol::S_CHANGEEQUIP& pkt) { return MakeSendBuffer(pkt, PKT_S_CHANGEEQUIP); }
	static SendBufferRef MakeSendBuffer(Protocol::S_DUNGEANRATIO& pkt) { return MakeSendBuffer(pkt, PKT_S_DUNGEANRATIO); }
	static SendBufferRef MakeSendBuffer(Protocol::S_ESTHER& pkt) { return MakeSendBuffer(pkt, PKT_S_ESTHERGAGE); }

private:
	template<typename PacketType, typename ProcessFunc>
	static bool HandlePacket(ProcessFunc func, PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketType pkt;
		if (pkt.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)) == false)
			return false;

		return func(session, pkt);
	}

	template<typename T>
	static SendBufferRef MakeSendBuffer(T& pkt, uint16 pktId)
	{
		const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
		const uint16 packetSize = dataSize + sizeof(PacketHeader);

		SendBufferRef sendBuffer = SendBufferManager::GetInstance()->Open(packetSize);
		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
		header->size = packetSize;
		header->id = pktId;
		ASSERT_CRASH(pkt.SerializeToArray(&header[1], dataSize));
		sendBuffer->Close(packetSize);

		return sendBuffer;
	}
};

