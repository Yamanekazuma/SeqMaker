#include <Protection.hpp>

#include <memoryapi.h>

#include <bit>
#include <stdexcept>

using namespace seq::protect;

bool ProtectionMaster::isReadable(Protections protect) noexcept {
  return (static_cast<protect_t>(protect) & static_cast<protect_t>(Protections::R));
}

bool ProtectionMaster::isWritable(Protections protect) noexcept {
  return (static_cast<protect_t>(protect) & static_cast<protect_t>(Protections::W));
}

Protections ProtectionMaster::convertFromWinConsts(ULONG win_memory_protection_consts) noexcept {
  switch (win_memory_protection_consts) {
    case PAGE_READONLY:
      return Protections::R;
    case PAGE_EXECUTE:
      return Protections::X;
    case PAGE_READWRITE:
      [[fallthrough]];
    case PAGE_WRITECOPY:
      return Protections::RW;
    case PAGE_EXECUTE_READ:
      return Protections::RX;
    case PAGE_EXECUTE_READWRITE:
      [[fallthrough]];
    case PAGE_EXECUTE_WRITECOPY:
      return Protections::RWX;
    default:
      return Protections::None;
  }
}

Protections ProtectionMaster::queryMemoryProtection(HANDLE hProcess, std::uint32_t address) {
  MEMORY_BASIC_INFORMATION mbi{};
  if (VirtualQueryEx(hProcess, std::bit_cast<LPCVOID>(static_cast<std::uintptr_t>(address)), &mbi, sizeof(mbi)) == 0) {
    throw std::runtime_error("メモリ保護属性の取得に失敗しました．");
  }
  return ProtectionMaster::convertFromWinConsts(mbi.Protect);
}
