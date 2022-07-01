#pragma once

class anim_state {
public:
	void* pthis;
	char pad2[91];
	void* base_entity; //0x60
	void* active_weapon; //0x64
	void* last_active_weapon; //0x68
	float last_client_side_animation_update_time; //0x6C 
	int last_client_side_animation_update_framecount; //0x70
	float eye_pitch; //0x74
	float eye_yaw; //0x78
	float pitch; //0x7C
	float goal_feet_yaw; //0x80
	float current_feet_yaw; //0x84
	float current_torso_yaw; //0x88
	float unknown_velocity; //0x8C //changes when moving/jumping/hitting ground
	float lean_amount; //0x90
	char pad4[4]; //NaN
	float feet_cycle; //0x98 0 to 1
	float feet_yaw_rate; //0x9C 0 to 1
	float unkown_2;
	float duck_amount; //0xA4
	float landing_duck_additive_something; //0xA8
	float unkown_3; //0xAC
	vec3_t origin; //0xB0, 0xB4, 0xB8
	vec3_t last_origin; //0xBC, 0xC0, 0xC4
	float veclocity_x; //0xC8
	float velocity_y; //0xCC
	char pad5[4];
	float unkown_float_1; //0xD4 Affected by movement and direction
	char pad6[8];
	float unkown_float_2; //0xE0 //from -1 to 1 when moving and affected by direction
	float unkown_float_3; //0xE4 //from -1 to 1 when moving and affected by direction
	float unkown; //0xE8
	float speed_2d; //0xEC
	float up_velocity; //0xF0
	float speed_normalized; //0xF4 //from 0 to 1
	float feet_speed_forwards_or_sideways; //0xF8 //from 0 to 2. something  is 1 when walking, 2.something when running, 0.653 when crouch walking
	float feet_speed_unknown_forward_or_sideways; //0xFC //from 0 to 3. something
	float times_incestarted_moving; //0x100
	float times_incestopped_moving; //0x104
	unsigned char on_ground; //0x108
	unsigned char in_hit_ground_animation; //0x109
	char pad7[10];
	float last_origin_z; //0x114
	float head_height_or_offset_from_hitting_ground_animation; //0x118 from 0 to 1, is 1 when standing
	float stop_to_full_running; //0x11C from 0 to 1, doesnt change when walking or crouching, only running
	char pad8[4]; //NaN
	float unkown_fraction; //0x124 affected while jumping and running, or when just jumping, 0 to 1
	char pad9[4]; //NaN
	float unkown_3;
	char pad10[516];
	float velocity_subtract_y;
};

class anim_layer {
public:
	char  pad_0000[20];
	// These should also be present in the padding, don't see the use for it though
	//float	m_flLayerAnimtime;
	//float	m_flLayerFadeOuttime;
	uint32_t order; //0x0014
	uint32_t sequence; //0x0018
	float_t prev_cycle; //0x001C
	float_t weight; //0x0020
	float_t wieght_delat_rate; //0x0024
	float_t playback_rate; //0x0028
	float_t cycle; //0x002C
	void* owener; //0x0030 // player's thisptr
	char  pad_0038[4]; //0x0034
};

