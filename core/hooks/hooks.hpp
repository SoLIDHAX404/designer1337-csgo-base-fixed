#pragma once

namespace hooks {
	bool initialize();
	void release();

	inline unsigned int get_virtual(void* _class, unsigned int index) { return static_cast<unsigned int>((*static_cast<int**>(_class))[index]); }

	namespace alloc_key_values {
		using fn = void* (__thiscall*)(void*, const std::int32_t);
		void* __stdcall hook(const std::int32_t size);
	}

	namespace create_move_proxy {
		using fn = void(__stdcall*)(int, float, bool);
		void __stdcall hook(int sequence_number, float input_sample_time, bool is_active);
	};

	namespace paint_traverse {
		using fn = void(__thiscall*)(i_panel*, unsigned int, bool, bool);
		void __stdcall hook(unsigned int panel, bool force_repaint, bool allow_force);
	}
}
