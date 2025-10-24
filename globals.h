#pragma once
#include <fstream>
#include <windows.h>

extern std::ofstream out_file;
extern std::ofstream log_file;
inline static constexpr int indent = 50;
inline static constexpr std::string_view out_file_name = "Interfaces Strings.txt";
inline constexpr std::string_view log_file_name = "Errors.txt";
inline constexpr std::string_view function_end_signature = "49 FF 21";
extern uint8_t* engine2_create_interface_end_address;
extern uint8_t* client_create_interface_end_address;
extern uint8_t* schemasystem_create_interface_end_address;
extern uint8_t* tier0_create_interface_end_address;
extern uint8_t* inputsystem_create_interface_end_address;
extern PVOID veh_handle;
extern uint8_t original_byte;