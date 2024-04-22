#pragma once

#include <Registers.hpp>

#include <Zydis/Zydis.h>

#include <cstdint>

namespace seq {

class MemoryMaster {
 public:
  MemoryMaster() = delete;
  MemoryMaster(MemoryMaster&&) = delete;
  MemoryMaster(const MemoryMaster&) = delete;
  MemoryMaster operator=(MemoryMaster&&) = delete;
  MemoryMaster operator=(const MemoryMaster&) = delete;

  // FSセグメントを用いたアドレス指定の場合は例外送出
  static std::uint32_t calcMemoryAddress(const Registers& regs, const ZydisDecodedOperandMem& mem);
};

}  // namespace seq
