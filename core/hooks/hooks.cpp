#include "../../dependencies/utilities/csgo.hpp"
#include "../features/features.hpp"
#include "../features/misc/engine_prediction.hpp"
#include "../menu/menu.hpp"

hooks::alloc_key_values::fn alloc_key_values_original = nullptr;
hooks::create_move_proxy::fn create_move_proxy_original = nullptr;
hooks::paint_traverse::fn paint_traverse_original = nullptr;

bool hooks::initialize() {
	const auto alloc_key_values_target = reinterpret_cast<void*>(get_virtual(interfaces::key_values_system, 2));
	const auto create_move_proxy_target = reinterpret_cast<void*>(get_virtual(interfaces::client, 22));
	const auto paint_traverse_target = reinterpret_cast<void*>(get_virtual(interfaces::panel, 41));

	if (MH_Initialize() != MH_OK)
		throw std::runtime_error("failed to initialize MH_Initialize.");

	if (MH_CreateHook(alloc_key_values_target, &alloc_key_values::hook, reinterpret_cast<void**>(&alloc_key_values_original)) != MH_OK)
		throw std::runtime_error("failed to initialize alloc_key_values. (outdated index?)");

	if (MH_CreateHook(create_move_proxy_target, &create_move_proxy::hook, reinterpret_cast<void**>(&create_move_proxy_original)) != MH_OK)
		throw std::runtime_error("failed to initialize create_move_proxy. (outdated index?)");

	if (MH_CreateHook(paint_traverse_target, &paint_traverse::hook, reinterpret_cast<void**>(&paint_traverse_original)) != MH_OK)
		throw std::runtime_error("failed to initialize paint_traverse. (outdated index?)");

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
		throw std::runtime_error("failed to enable hooks.");

	console::log("[setup] hooks initialized!\n");
	return true;
}

void hooks::release() {
	MH_Uninitialize();

	MH_DisableHook(MH_ALL_HOOKS);
}

std::uint8_t* key_values_engine = utilities::pattern_scan("engine.dll", sig_key_values_engine);
std::uint8_t* key_values_client = utilities::pattern_scan("client.dll", sig_key_values_client);

void* __stdcall hooks::alloc_key_values::hook(const std::int32_t size) {
	if (const std::uint32_t address = reinterpret_cast<std::uint32_t>(_ReturnAddress());
		address == reinterpret_cast<std::uint32_t>(key_values_engine) || address == reinterpret_cast<std::uint32_t>(key_values_client))
		return nullptr;

	return alloc_key_values_original(interfaces::key_values_system, size);
}

static bool __stdcall create_move(float input_sample_frametime, c_usercmd* cmd, bool& send_packet) {
	csgo::local_player = static_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	auto net_channel = interfaces::clientstate->net_channel;
	auto old_viewangles = cmd->viewangles;
	auto old_forwardmove = cmd->forwardmove;
	auto old_sidemove = cmd->sidemove;

	/*prediction::start(cmd); {
	} prediction::end();*/

	std::cout << "test" << "\n";

	math::correct_movement(old_viewangles, cmd, old_forwardmove, old_sidemove);

	cmd->forwardmove = std::clamp(cmd->forwardmove, -450.0f, 450.0f);
	cmd->sidemove = std::clamp(cmd->sidemove, -450.0f, 450.0f);
	cmd->upmove = std::clamp(cmd->upmove, -320.0f, 320.0f);

	cmd->viewangles.normalize();
	cmd->viewangles.x = std::clamp(cmd->viewangles.x, -89.0f, 89.0f);
	cmd->viewangles.y = std::clamp(cmd->viewangles.y, -180.0f, 180.0f);
	cmd->viewangles.z = 0.0f;

	return false;
}

static void __stdcall chl_create_move(int sequence_number, float input_sample_time, bool active, bool& send_packet)
{
	create_move_proxy_original(sequence_number, input_sample_time, active);

	c_usercmd* cmd = interfaces::input->get_user_cmd(0, sequence_number);
	if (!cmd || !cmd->command_number)
		return;

	c_verified_user_cmd* verified_cmd = interfaces::input->get_verified_cmd(sequence_number);
	if (!verified_cmd)
		return;

	bool create_move_active = create_move(input_sample_time, cmd, send_packet);

	verified_cmd->user_cmd = *cmd;
	verified_cmd->u_hash_crc = cmd->get_checksum();
}

__declspec(naked) void __stdcall hooks::create_move_proxy::hook(int sequence_number, float input_sample_time, bool active)
{
	__asm
	{
		push ebp
		mov  ebp, esp
		push ebx;
		push esp
		push dword ptr[active]
		push dword ptr[input_sample_time]
		push dword ptr[sequence_number]
		call chl_create_move
		pop  ebx
		pop  ebp
		retn 0Ch
	}
}

void __stdcall hooks::paint_traverse::hook(unsigned int panel, bool force_repaint, bool allow_force) {
	auto panel_to_draw = fnv::hash(interfaces::panel->get_panel_name(panel));

	switch (panel_to_draw) {
	case fnv::hash("MatSystemTopPanel"):
		
		render::text(10, 10, render::fonts::watermark_font, "csgo-cheat", false, color::white(255));

		menu::toggle();
		menu::render();

		break;

	case fnv::hash("FocusOverlayPanel"):
		interfaces::panel->set_keyboard_input_enabled(panel, variables::menu::opened);
		interfaces::panel->set_mouse_input_enabled(panel, variables::menu::opened);
		break;
	}

	paint_traverse_original(interfaces::panel, panel, force_repaint, allow_force);
}
