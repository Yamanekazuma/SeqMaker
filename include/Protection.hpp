#pragma once

#include <windef.h>
#include <winnt.h>

#include <cstdint>
#include <stdexcept>

namespace Protection {

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

  static bool isReadable(Protections protect) { return (static_cast<protect_t>(protect) & static_cast<protect_t>(Protections::R)); }
  static bool isWritable(Protections protect) { return (static_cast<protect_t>(protect) & static_cast<protect_t>(Protections::W)); }

  static Protections convertFromWinConsts(ULONG win_memory_protection_consts) {
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
};

}  // namespace Protection
