#include "hooks.h"
#include "globals.h"
#include "veh_handlers.h"

bool SetSoftwareBreakpoint(uint8_t* address, uint8_t& original_byte) 
{
  DWORD old;
  if (!VirtualProtect(address, 1, PAGE_EXECUTE_READWRITE, &old)) 
  {
    return false;
  }
  original_byte = *address;
  *address = 0xCC;  // INT3
  FlushInstructionCache(GetCurrentProcess(), address, 1);
  VirtualProtect(address, 1, old, &old);
  return true;
}

bool RemoveSoftwareBreakpoint(uint8_t* address, uint8_t original_byte) 
{
  DWORD old;
  if (!VirtualProtect(address, 1, PAGE_EXECUTE_READWRITE, &old)) 
  {
    return false;
  }
  *address = original_byte;
  FlushInstructionCache(GetCurrentProcess(), address, 1);
  VirtualProtect(address, 1, old, &old);
  return true;
}

bool InstallBreakAndHandler(uint8_t* address, uint8_t& original_byte) 
{
  if (!SetSoftwareBreakpoint(address, original_byte)) 
  {
    return false;
  }
  if (veh_handle != nullptr)
  {
    return true;
  }
  veh_handle = AddVectoredExceptionHandler(1, VectoredHandler);
  if (veh_handle == nullptr) 
  {
    RemoveSoftwareBreakpoint(address, original_byte);
    return false;
  }
  return true;
}