#include "globals.h"

LONG WINAPI VectoredHandler(EXCEPTION_POINTERS* exception_info) 
{
  if (exception_info == nullptr || exception_info->ExceptionRecord == nullptr) 
  {
    return EXCEPTION_CONTINUE_SEARCH;
  }
  DWORD code = exception_info->ExceptionRecord->ExceptionCode;
  if (code != EXCEPTION_BREAKPOINT) 
  {
    return EXCEPTION_CONTINUE_SEARCH;
  }
  void* break_address = exception_info->ExceptionRecord->ExceptionAddress;
  if (break_address == (void*)engine2_create_interface_end_address ||
      break_address == (void*)client_create_interface_end_address ||
      break_address == (void*)schemasystem_create_interface_end_address ||
      break_address == (void*)tier0_create_interface_end_address ||
      break_address == (void*)inputsystem_create_interface_end_address) 
  {
    CONTEXT* context = exception_info->ContextRecord;
    if (context != nullptr) 
    {
      out_file << "[VEH] Breakpoint at: "
               << exception_info->ExceptionRecord->ExceptionAddress
               << " | R11: " << std::hex << context->R11 << " | String: \""
               << (char*)context->R11 << "\"" << std::endl;
      context->Rip = *(uintptr_t*)context->R9;
      return EXCEPTION_CONTINUE_EXECUTION;
    }
  }

  return EXCEPTION_CONTINUE_SEARCH;
}