#include <Registers.hpp>

#include <stdexcept>

using namespace seq;

std::uint32_t RegisterMaster::getRegisterValueFromZydis(const Registers& regs, const ZydisRegister& reg) {
  switch (reg) {
    case ZYDIS_REGISTER_EAX:
      return regs.EAX;
    case ZYDIS_REGISTER_EBX:
      return regs.EBX;
    case ZYDIS_REGISTER_ECX:
      return regs.ECX;
    case ZYDIS_REGISTER_EDX:
      return regs.EDX;
    case ZYDIS_REGISTER_EBP:
      return regs.EBP;
    case ZYDIS_REGISTER_ESP:
      return regs.ESP;
    case ZYDIS_REGISTER_ESI:
      return regs.ESI;
    case ZYDIS_REGISTER_EDI:
      return regs.EDI;
    case ZYDIS_REGISTER_EIP:
      return regs.EIP;
    case ZYDIS_REGISTER_DS:
      return regs.DS;
    case ZYDIS_REGISTER_ES:
      return regs.ES;
    case ZYDIS_REGISTER_CS:
      return regs.CS;
    case ZYDIS_REGISTER_SS:
      return regs.SS;
    case ZYDIS_REGISTER_FS:
      return regs.FS;
    case ZYDIS_REGISTER_GS:
      return regs.GS;
    default:
      throw std::runtime_error("不明なオペランドが指定されました．");
  }
}
