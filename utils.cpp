#include <windows.h>
#include <winternl.h>
#pragma comment(lib, "Shlwapi.lib")
#include <Shlwapi.h>
#include <string_view>
#include <unordered_map>
#include "utils.h"

uint8_t* signatureScan(const std::string_view& signature, uint8_t* address) {
  std::vector<int> signatureBytes = convertSignatureToBytes(signature);
  size_t signatureLength = signatureBytes.size();
  if (std::find(signatureBytes.begin(), signatureBytes.end(), -1) !=
      signatureBytes.end()) {
    for (uint64_t index = 0; index <= 15000 - signatureLength; ++index) {
      bool found = true;
      for (size_t i = 0; i < signatureLength; ++i) {
        if (signatureBytes[i] != -1 &&
            address[i + index] != signatureBytes[i]) {
          found = false;

          break;
        }
      }
      if (found == true) {
        return &address[index];
      }
    }
    return nullptr;
  }
  std::unordered_map<uint8_t, size_t> badCharactersShift;
  for (size_t index = 0; index < signatureLength - 1; ++index) {
    badCharactersShift[signatureBytes[index]] = signatureLength - index - 1;
  }
  for (size_t index = 0; index <= 1500 - signatureLength;) {
    int matchLength = signatureLength - 1;
    while (matchLength >= 0 &&
           address[index + matchLength] == signatureBytes[matchLength]) {
      --matchLength;
    }
    if (matchLength < 0) {
      return &address[index];
    }
    uint8_t nextByte = address[index + signatureLength - 1];
    if (badCharactersShift.count(nextByte) == 0) {
      index += signatureLength;
    } else {
      index += badCharactersShift[nextByte];
    }
  }
  return nullptr;
}

uintptr_t getModuleBase(const wchar_t* moduleName) {
  auto peb = reinterpret_cast<PEB*>(__readgsqword(0x60));
  PPEB_LDR_DATA ldr = peb->Ldr;
  PLIST_ENTRY list = &ldr->InMemoryOrderModuleList;
  for (auto item = list->Flink; item != list; item = item->Flink) {
    PLDR_DATA_TABLE_ENTRY entry =
        CONTAINING_RECORD(item, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
    WCHAR buffer[MAX_PATH];
    wcsncpy_s(buffer, MAX_PATH, entry->FullDllName.Buffer,
              entry->FullDllName.Length / sizeof(WCHAR));
    buffer[entry->FullDllName.Length / sizeof(WCHAR)] = L'\0';
    const WCHAR* moduleFileName = PathFindFileNameW(buffer);
    if (_wcsicmp(moduleFileName, moduleName) == 0) {
      return reinterpret_cast<uintptr_t>(entry->DllBase);
    }
  }
  return 0;
}

std::vector<int> convertSignatureToBytes(const std::string_view& signature) {
  std::vector<int> signatureBytes;
  const char* current = signature.data();
  const char* end = signature.data() + signature.length();
  while (current != end) {
    if (*current == '?') {
      ++current;
      if (current < end && *current == '?') {
        ++current;
      }
      signatureBytes.emplace_back(-1);
    } else {
      char* next = nullptr;
      int value = strtoul(current, &next, 16);
      signatureBytes.emplace_back(value);
      current = next;
    }
    while (current < end && isspace(*current)) {
      ++current;
    }
  }
  return signatureBytes;
}