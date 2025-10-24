#pragma once
#include <cstdint>

bool SetSoftwareBreakpoint(uint8_t*, uint8_t&);
bool RemoveSoftwareBreakpoint(uint8_t*, uint8_t);
bool InstallBreakAndHandler(uint8_t*, uint8_t&);