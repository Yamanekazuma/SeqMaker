#include <Memory.hpp>

#include <stdexcept>

using namespace seq;

std::uint32_t MemoryMaster::calcMemoryAddress(const Registers& regs, const ZydisDecodedOperandMem& mem) {
  if (mem.segment == ZYDIS_REGISTER_FS) {
    throw std::runtime_error("FSセグメントのアドレス計算には対応していません．");
  }

  // base取得
  std::uint32_t base = 0;
  if (mem.base != ZYDIS_REGISTER_NONE) {
    base = RegisterMaster::getRegisterValueFromZydis(regs, mem.base);
  }

  // disp取得
  std::uint32_t disp = 0;
  if (mem.disp.has_displacement) {
    disp = static_cast<std::uint32_t>(mem.disp.value);
  }

  // index取得
  std::uint32_t index = 0;
  if (mem.index != ZYDIS_REGISTER_NONE) {
    index = RegisterMaster::getRegisterValueFromZydis(regs, mem.index);
  }

  // scale取得
  std::uint32_t scale = mem.scale;

  return disp + base + index * scale;
}