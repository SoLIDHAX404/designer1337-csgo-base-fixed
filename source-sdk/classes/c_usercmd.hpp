#pragma once
#include "../../dependencies/utilities/crc32.hpp"


#define in_attack  (1 << 0)
#define in_jump   (1 << 1)
#define in_duck   (1 << 2)
#define in_forward  (1 << 3)
#define in_back   (1 << 4)
#define in_use   (1 << 5)
#define in_cancel  (1 << 6)
#define in_left   (1 << 7)
#define in_right  (1 << 8)
#define in_moveleft  (1 << 9)
#define in_moveright (1 << 10)
#define in_attack2  (1 << 11)
#define in_run   (1 << 12)
#define in_reload  (1 << 13)
#define in_alt1   (1 << 14)
#define in_alt2   (1 << 15)
#define in_score  (1 << 16)   // Used by client.dll for when scoreboard is held down
#define in_speed  (1 << 17) // Player is holding the speed key
#define in_walk   (1 << 18) // Player holding walk key
#define in_zoom   (1 << 19) // Zoom key for HUD zoom
#define in_weapon1  (1 << 20) // weapon defines these bits
#define in_weapon2  (1 << 21) // weapon defines these bits
#define in_bullrush  (1 << 22)
#define in_grenade1  (1 << 23) // grenade 1
#define in_grenade2  (1 << 24) // grenade 2
#define in_lookspin  (1 << 25)

struct c_usercmd {
	[[nodiscard]] CRC32_t get_checksum() const
	{
		CRC32_t uHashCRC = 0UL;

		CRC32::Init(&uHashCRC);
		CRC32::ProcessBuffer(&uHashCRC, &command_number, sizeof(command_number));
		CRC32::ProcessBuffer(&uHashCRC, &tick_count, sizeof(tick_count));
		CRC32::ProcessBuffer(&uHashCRC, &viewangles, sizeof(viewangles));
		CRC32::ProcessBuffer(&uHashCRC, &aimdirection, sizeof(aimdirection));
		CRC32::ProcessBuffer(&uHashCRC, &forwardmove, sizeof(forwardmove));
		CRC32::ProcessBuffer(&uHashCRC, &sidemove, sizeof(sidemove));
		CRC32::ProcessBuffer(&uHashCRC, &upmove, sizeof(upmove));
		CRC32::ProcessBuffer(&uHashCRC, &buttons, sizeof(buttons));
		CRC32::ProcessBuffer(&uHashCRC, &impulse, sizeof(impulse));
		CRC32::ProcessBuffer(&uHashCRC, &weaponselect, sizeof(weaponselect));
		CRC32::ProcessBuffer(&uHashCRC, &weaponsubtype, sizeof(weaponsubtype));
		CRC32::ProcessBuffer(&uHashCRC, &randomseed, sizeof(randomseed));
		CRC32::ProcessBuffer(&uHashCRC, &mousedx, sizeof(mousedx));
		CRC32::ProcessBuffer(&uHashCRC, &mousedy, sizeof(mousedy));
		CRC32::Final(&uHashCRC);

		return uHashCRC;
	}

	int pad;
	int command_number;
	int tick_count;
	vec3_t viewangles;
	vec3_t aimdirection;
	float forwardmove;
	float sidemove;
	float upmove;
	int buttons;
	char impulse;
	int weaponselect;
	int weaponsubtype;
	int randomseed;
	short mousedx;
	short mousedy;
	bool predicted;
	char pad_0x4C[0x18];
};

class c_verified_user_cmd
{
public:
	c_usercmd user_cmd;
	CRC32_t u_hash_crc;
};