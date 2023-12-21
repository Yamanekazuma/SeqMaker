#pragma once

#include "SeqUnit.hpp"

#include "Protection.hpp"
#include "Registers.hpp"

#include <format>
#include <optional>

namespace Seq {

class OperandInfo {
public:
  virtual ~OperandInfo() = 0;

  enum class Values {
    None,
    NonMemoryAccess,
    MemoryAccessR,
    MemoryAccessW,
    MemoryAccessX,
    MemoryAccessRW,
    MemoryAccessRX,
    MemoryAccessWX,
    MemoryAccessRWX,
  };

  inline const std::string toString() const noexcept {
    switch (val_) {
      case Values::NonMemoryAccess:
        return "NonMemoryAccess";
      case Values::MemoryAccessR:
        return "MemoryAccess{R}";
      case Values::MemoryAccessW:
        return "MemoryAccess{W}";
      case Values::MemoryAccessX:
        return "MemoryAccess{X}";
      case Values::MemoryAccessRW:
        return "MemoryAccess{RW}";
      case Values::MemoryAccessRX:
        return "MemoryAccess{RX}";
      case Values::MemoryAccessWX:
        return "MemoryAccess{WX}";
      case Values::MemoryAccessRWX:
        return "MemoryAccess{RWX}";
      default:
        return "";
    }
  }

  inline bool isMemoryAccess() const noexcept { return val_ != Values::NonMemoryAccess; }

protected:
  Values val_;

  static std::uint32_t calcMemoryAddress(const Registers& regs, const ZydisDecodedOperandMem& mem);
  static Protection::Protections getMemoryProtection(HANDLE hProcess, std::uint32_t address);
};

class DestInfo : public OperandInfo {
public:
  DestInfo(HANDLE hProcess, const Registers& regs, const ZydisDecodedOperand& op);

  DestInfo(DestInfo&&) = default;
  DestInfo(const DestInfo&) = default;
  DestInfo& operator=(DestInfo&&) = default;
  DestInfo& operator=(const DestInfo&) = default;
};

class SrcInfo : public OperandInfo {
public:
  SrcInfo(HANDLE hProcess, const Registers& regs, const ZydisDecodedOperand& op);

  SrcInfo(SrcInfo&&) = default;
  SrcInfo(const SrcInfo&) = default;
  SrcInfo& operator=(SrcInfo&&) = default;
  SrcInfo& operator=(const SrcInfo&) = default;
};

class SeqUnitOriginal : public SeqUnit {
public:
  inline SeqUnitOriginal() noexcept {}
  SeqUnitOriginal(HANDLE hProcess, const Registers& regs, ZydisDisassembledInstruction&& inst);

  SeqUnitOriginal(SeqUnitOriginal&&) = default;
  SeqUnitOriginal(const SeqUnitOriginal&) = default;
  SeqUnitOriginal& operator=(SeqUnitOriginal&&) = default;
  SeqUnitOriginal& operator=(const SeqUnitOriginal&) = default;

  inline const std::string toString() const noexcept override {
    /*
    {
      mnemonic: ニーモニック,
      dest: None|NonMemoryAccess|MemoryAccess
      src1: None|NonMemoryAccess|MemoryAccess
      src2: None|NonMemoryAccess|MemoryAccess
    }
    */
    return std::vformat("{mnemonic: {}, dest: {}, src1: {}, src2: {}}",
                        std::make_format_args(mnemonic_, dest_->toString(), src1_->toString(), src2_->toString()));
  }

private:
  std::string mnemonic_;
  std::optional<DestInfo> dest_;
  std::optional<SrcInfo> src1_;
  std::optional<SrcInfo> src2_;

  inline bool isDestOperand(const ZydisDecodedOperand& op) const noexcept {
    return !dest_.has_value() && (op.actions & ZYDIS_OPERAND_ACTION_WRITE);
  }
  inline bool isSrc1Operand(const ZydisDecodedOperand& op) const noexcept {
    return !src1_.has_value() && (op.actions & ZYDIS_OPERAND_ACTION_READ);
  }
  inline bool isSrc2Operand(const ZydisDecodedOperand& op) const noexcept {
    return !src2_.has_value() && (op.actions & ZYDIS_OPERAND_ACTION_READ);
  }
};

};  // namespace Seq
