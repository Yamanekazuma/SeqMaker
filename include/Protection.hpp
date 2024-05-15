#pragma once

#include <Windows.h>

#include <cstdint>

namespace seq::protect {

using protect_t = std::uint8_t;

enum class Protections : protect_t {
  None = 0x0,
  R = 0x1,
  W = 0x2,
  X = 0x4,
  RW = static_cast<protect_t>(Protections::R) | static_cast<protect_t>(Protections::W),
  RX = static_cast<protect_t>(Protections::R) | static_cast<protect_t>(Protections::X),
  WX = static_cast<protect_t>(Protections::W) | static_cast<protect_t>(Protections::X),
  RWX = static_cast<protect_t>(Protections::R) | static_cast<protect_t>(Protections::W) | static_cast<protect_t>(Protections::X),
};

class ProtectionMaster {
 public:
  ProtectionMaster() = delete;
  ProtectionMaster(ProtectionMaster&&) = delete;
  ProtectionMaster(const ProtectionMaster&) = delete;
  ProtectionMaster operator=(ProtectionMaster&&) = delete;
  ProtectionMaster operator=(const ProtectionMaster&) = delete;

  static bool isReadable(Protections protect) noexcept;
  static bool isWritable(Protections protect) noexcept;

  static Protections convertFromWinConsts(ULONG win_memory_protection_consts) noexcept;
  static Protections queryMemoryProtection(HANDLE hProcess, std::uint32_t address);
};

}  // namespace seq::protect
