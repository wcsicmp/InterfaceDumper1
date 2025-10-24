#pragma once
#include <cstdint>
#include <vector>

uintptr_t getModuleBase(const wchar_t*);
uint8_t* signatureScan(const std::string_view&, uint8_t*);
std::vector<int> convertSignatureToBytes(const std::string_view&);