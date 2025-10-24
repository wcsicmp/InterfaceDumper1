#include <windows.h>
#include <fstream>
#include <iomanip>
#include <string_view>
#include "utils.h"
#include "hooks.h"
#include "globals.h"

std::ofstream out_file;
std::ofstream log_file;
uint8_t* engine2_create_interface_end_address = nullptr;
uint8_t* client_create_interface_end_address = nullptr;
uint8_t* schemasystem_create_interface_end_address = nullptr;
uint8_t* tier0_create_interface_end_address = nullptr;
uint8_t* inputsystem_create_interface_end_address = nullptr;
PVOID veh_handle = nullptr;
uint8_t original_byte = 0;

static bool execute() 
{
  out_file.open(out_file_name.data());
  log_file.open(log_file_name.data());
  if (!out_file.is_open()) 
  {
    if (!log_file.is_open()) 
    {
      return false;
    } 
    else 
    {
      log_file
          << "[ERROR] Failed to create file with interfaces strings [ERROR]\n";
      return false;
    }
  }

  FARPROC engine2_create_interface_address = NULL;
  FARPROC client_create_interface_address = NULL;
  FARPROC schemasystem_create_interface_address = NULL;
  FARPROC tier0_create_interface_address = NULL;
  FARPROC inputsystem_create_interface_address = NULL;

  HMODULE engine2_dll_handle = GetModuleHandleA("engine2.dll");
  if (engine2_dll_handle == NULL) 
  {
    log_file << "[ERROR] Failed to get handle of engine2 dll [ERROR]\n";
  } 
  else 
  {
    engine2_create_interface_address =
        GetProcAddress(engine2_dll_handle, "CreateInterface");
    if (engine2_create_interface_address == NULL) 
    {
      log_file << "[ERROR] Failed to get address of engine2 create interface "
                  "function [ERROR]\n";
    } 
    else 
    {
      out_file << std::left << std::setw(indent)
               << "[INFO] Engine2 create interface address: " << std::hex
               << (uintptr_t)engine2_create_interface_address << " [INFO]\n";
    }
  }
  HMODULE client_dll_handle = GetModuleHandleA("client.dll");
  if (client_dll_handle == NULL) 
  {
    log_file << "[ERROR] Failed to get handle of client dll [ERROR]\n";
  } 
  else 
  {
    client_create_interface_address =
        GetProcAddress(client_dll_handle, "CreateInterface");
    if (client_create_interface_address == NULL) 
    {
      log_file << "[ERROR] Failed to get address of client create interface "
                  "function [ERROR]\n";
    } 
    else 
    {
      out_file << std::left << std::setw(indent)
               << "[INFO] Client create interface address: "
               << (uintptr_t)client_create_interface_address << " [INFO]\n";
    }
  }
  HMODULE schemasystem_dll_handle = GetModuleHandleA("schemasystem.dll");
  if (schemasystem_dll_handle == NULL) 
  {
    log_file << "[ERROR] Failed to get handle of schemasystem dll [ERROR]\n";
  } 
  else 
  {
    schemasystem_create_interface_address =
        GetProcAddress(schemasystem_dll_handle, "CreateInterface");
    if (schemasystem_create_interface_address == NULL) {
      log_file
          << "[ERROR] Failed to get address of schemasystem create interface "
             "function [ERROR]\n";
    } 
    else 
    {
      out_file << std::left << std::setw(indent)
               << "[INFO] Schemasystem create interface address: "
               << (uintptr_t)schemasystem_create_interface_address
               << " [INFO]\n";
    }
  }
  HMODULE tier0_dll_handle = GetModuleHandleA("tier0.dll");
  if (tier0_dll_handle == NULL) 
  {
    log_file << "[ERROR] Failed to get handle of tier0 dll [ERROR]\n";
  } 
  else 
  {
    tier0_create_interface_address =
        GetProcAddress(tier0_dll_handle, "CreateInterface");
    if (tier0_create_interface_address == NULL) 
    {
      log_file << "[ERROR] Failed to get address of tier0 create interface "
                  "function [ERROR]\n";
    } 
    else 
    {
      out_file << std::left << std::setw(indent)
               << "[INFO] Tier0 create interface address: "
               << (uintptr_t)tier0_create_interface_address << " [INFO]\n";
    }
  }
  HMODULE inputsystem_dll_handle = GetModuleHandleA("inputsystem");
  if (inputsystem_dll_handle == NULL) 
  {
    log_file << "[ERROR] Failed to get handle of inputsystem [ERROR]\n";
  } 
  else 
  {
    inputsystem_create_interface_address =
        GetProcAddress(inputsystem_dll_handle, "CreateInterface");
    if (inputsystem_create_interface_address == NULL) 
    {
      log_file << "[ERROR] Failed to get address of inputsystem create "
                  "interface function [ERROR]\n";
    } 
    else 
    {
      out_file << std::left << std::setw(indent)
               << "[INFO] Inputsystem create interface address: "
               << (uintptr_t)inputsystem_create_interface_address
               << " [INFO]\n";
    }
  }

  if (engine2_create_interface_address != NULL) 
  {
    engine2_create_interface_end_address = signatureScan(
        function_end_signature, (uint8_t*)engine2_create_interface_address);
    if (engine2_create_interface_end_address != nullptr) 
    {
      out_file << std::left << std::setw(indent)
               << "[INFO] Engine 2 create interface end address: "
               << (uintptr_t)engine2_create_interface_end_address
               << " [INFO]\n";
    } 
    else 
    {
      log_file << "[ERROR] Failed to find engine 2 create interface end "
                  "address [ERROR]\n";
    }
  }
  if (client_create_interface_address != NULL) 
  {
    client_create_interface_end_address = signatureScan(
        function_end_signature, (uint8_t*)client_create_interface_address);
    if (client_create_interface_end_address != nullptr) 
    {
      out_file << std::left << std::setw(indent)
               << "[INFO] Client create interface end address: "
               << (uintptr_t)client_create_interface_end_address << " [INFO]\n";
    } 
    else 
    {
      log_file << "[ERROR] Failed to find client create interface end address "
                  "[ERROR]\n";
    }
  }
  if (schemasystem_create_interface_address != NULL) 
  {
    schemasystem_create_interface_end_address =
        signatureScan(function_end_signature,
                      (uint8_t*)schemasystem_create_interface_address);
    if (schemasystem_create_interface_end_address != nullptr) 
    {
      out_file << std::left << std::setw(indent)
               << "[INFO] Schemasystem create interface end address: "
               << (uintptr_t)schemasystem_create_interface_end_address
               << " [INFO]\n";
    } 
    else 
    {
      log_file << "[ERROR] Failed to find schemasystem create interface end "
                  "address [ERROR]\n";
    }
  }
  if (tier0_create_interface_address != NULL) 
  {
    tier0_create_interface_end_address = signatureScan(
        function_end_signature, (uint8_t*)tier0_create_interface_address);
    if (tier0_create_interface_end_address != nullptr) 
    {
      out_file << std::left << std::setw(indent)
               << "[INFO] Tier0 create interface end address: "
               << (uintptr_t)tier0_create_interface_end_address << " [INFO]\n";
    } 
    else 
    {
      log_file << "[ERROR] Failed to find tier0 create interface end address "
                  "[ERROR]\n";
    }
  }
  if (inputsystem_create_interface_address != NULL) 
  {
    inputsystem_create_interface_end_address = signatureScan(
        function_end_signature, (uint8_t*)inputsystem_create_interface_address);
    if (inputsystem_create_interface_end_address != nullptr) 
    {
      out_file << std::left << std::setw(indent)
               << "[INFO] Inputsystem create interface end address: "
               << (uintptr_t)inputsystem_create_interface_end_address
               << " [INFO]\n";
    } 
    else 
    {
      log_file << "[ERROR] Failed to find Inputsystem create interface end "
                  "address [ERROR]\n";
    }
  }



  if (InstallBreakAndHandler(engine2_create_interface_end_address, original_byte))
  {
    log_file << "[ERROR] Failed to set VEH for engine2 [ERROR]\n";
  }

  if (InstallBreakAndHandler(client_create_interface_end_address,
                             original_byte)) {
    log_file << "[ERROR] Failed to set VEH for client [ERROR]\n";
  }

  if (InstallBreakAndHandler(schemasystem_create_interface_end_address,
                             original_byte)) {
    log_file << "[ERROR] Failed to set VEH for schemasystem [ERROR]\n";
  }

  if (InstallBreakAndHandler(tier0_create_interface_end_address,
                             original_byte)) {
    log_file << "[ERROR] Failed to set VEH for tier0 [ERROR]\n";
  }

  if (InstallBreakAndHandler(inputsystem_create_interface_end_address,
                             original_byte)) {
    log_file << "[ERROR] Failed to set VEH for inputsystem [ERROR]\n";
  }

  return true;
}

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD ul_reason_for_call,
                      LPVOID lpReserved) {
  switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
      execute();
      break;
    case DLL_THREAD_ATTACH:
      break;
    case DLL_THREAD_DETACH:
      break;
    case DLL_PROCESS_DETACH:
      break;
  }

  return TRUE;
}
