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
#include <vector>

namespace seq {

class MemoryProtectionInfo {
 public:
  MemoryProtectionInfo() noexcept;
  MemoryProtectionInfo(protect::Protections protection) noexcept;

  MemoryProtectionInfo(MemoryProtectionInfo&& other) noexcept;
  MemoryProtectionInfo& operator=(MemoryProtectionInfo&& other) noexcept;

  MemoryProtectionInfo(const MemoryProtectionInfo&) = default;
  MemoryProtectionInfo& operator=(const MemoryProtectionInfo&) = default;

  inline explicit operator std::string() const noexcept {
    return string();
  }
  const std::string string() const noexcept;

  inline operator bool() const noexcept {
    return has_value();
  }
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

class IOperandInfo {
 public:
  virtual const MemoryProtectionInfo& protection() const noexcept = 0;
  virtual std::uint32_t accessingAddress() const noexcept = 0;
  virtual std::uint32_t value() const noexcept = 0;

  virtual bool isNone() const noexcept = 0;
  virtual bool isMemoryAccessing() const noexcept = 0;
  virtual bool isAccessViolation() const noexcept = 0;

  virtual OperandType getOperandType() const noexcept = 0;
};

template <OperandType T = OperandType::None>
class OperandInfo : public IOperandInfo {
 public:
  OperandInfo() noexcept;
  OperandInfo(HANDLE hProcess, const Registers& regs, const ZydisDecodedInstruction& inst, const ZydisDecodedOperand& op);

  OperandInfo(OperandInfo<T>&& other) noexcept;
  OperandInfo<T>& operator=(OperandInfo<T>&& other) noexcept;

  OperandInfo(const OperandInfo<T>&) = default;
  OperandInfo<T>& operator=(const OperandInfo<T>&) = default;

  inline const MemoryProtectionInfo& protection() const noexcept override {
    return protection_;
  }
  inline std::uint32_t accessingAddress() const noexcept override {
    return address_;
  }
  inline std::uint32_t value() const noexcept override {
    return value_;
  }

  bool isNone() const noexcept override;
  bool isMemoryAccessing() const noexcept override;
  bool isAccessViolation() const noexcept override;

  OperandType getOperandType() const noexcept override;

 private:
  std::uint32_t address_;
  std::uint32_t value_;
  MemoryProtectionInfo protection_;
  bool isAccessViolation_;
};

using DestInfo = OperandInfo<OperandType::Destination>;
using SrcInfo = OperandInfo<OperandType::Source>;

class OperandSet {
 public:
  OperandSet(HANDLE hProcess, const Registers& regs, const ZydisDisassembledInstruction& inst);

  const DestInfo destAt(std::size_t no) const;
  const SrcInfo srcAt(std::size_t no) const;

  inline const std::vector<DestInfo>& dest() const noexcept {
    return destOps_;
  }
  inline const std::vector<SrcInfo>& src() const noexcept {
    return srcOps_;
  }

  inline bool isReadAccessViolation() const noexcept {
    return isReadAccessViolation_;
  }

  inline bool isWriteAccessViolation() const noexcept {
    return isWriteAccessViolation_;
  }

 private:
  std::vector<DestInfo> destOps_;
  std::vector<SrcInfo> srcOps_;
  bool isReadAccessViolation_;
  bool isWriteAccessViolation_;
};

}  // namespace seq
