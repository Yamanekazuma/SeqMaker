#pragma once

#include <Memory.hpp>
#include <Protection.hpp>
#include <Registers.hpp>

#include <Zydis/Zydis.h>

#include <windef.h>

#include <concepts>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

namespace seq {

class MemoryProtectionInfo {
 public:
  MemoryProtectionInfo() noexcept;
  MemoryProtectionInfo(protect::Protections protection) noexcept;

  MemoryProtectionInfo(MemoryProtectionInfo&& other) noexcept;
  MemoryProtectionInfo& operator=(MemoryProtectionInfo&& other) noexcept;

  MemoryProtectionInfo(const MemoryProtectionInfo&) = default;
  MemoryProtectionInfo& operator=(const MemoryProtectionInfo&) = default;

  inline explicit operator std::string() const noexcept { return string(); }
  const std::string string() const noexcept;

  inline operator bool() const noexcept { return has_value(); }
  bool has_value() const noexcept;

  bool isMemoryAccessing() const noexcept;

 private:
  std::optional<protect::Protections> protection_;
};

enum class OperandType {
  None,
  Destination,
  Source,
};

template <OperandType T = OperandType::None>
class OperandInfo {
 public:
  OperandInfo() noexcept;
  OperandInfo(HANDLE hProcess, const Registers& regs, const ZydisDecodedOperand& op);

  OperandInfo(OperandInfo<T>&& other) noexcept;
  OperandInfo<T>& operator=(OperandInfo<T>&& other) noexcept;

  OperandInfo(const OperandInfo<T>&) = default;
  OperandInfo<T>& operator=(const OperandInfo<T>&) = default;

  inline const MemoryProtectionInfo& protection() const noexcept { return protection_; }
  inline std::uint32_t accessingAddress() const noexcept { return address_; }

  bool isNone() const noexcept;
  bool isMemoryAccessing() const noexcept;

 private:
  MemoryProtectionInfo protection_;
  std::uint32_t address_;
};

using DestInfo = OperandInfo<OperandType::Destination>;
using SrcInfo = OperandInfo<OperandType::Source>;

}  // namespace seq
