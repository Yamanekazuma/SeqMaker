#pragma once

#include "OperandInfo.hpp"
#include "Registers.hpp"

#include <Zydis/Zydis.h>

#include <windef.h>

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace seq {

class SeqUnit;

template <class U>
  requires std::is_base_of_v<SeqUnit, U>
class SeqUnitFactory {
 public:
  static U create(HANDLE hProcess, const Registers& regs, const ZydisDisassembledInstruction& inst);
};

class SeqUnit {
 public:
  SeqUnit(HANDLE hProcess, const Registers& regs, const ZydisDisassembledInstruction& inst);

  virtual ~SeqUnit() = 0;

  inline explicit operator std::string() const noexcept {
    return string();
  }
  inline const std::string& string() const noexcept {
    return str_;
  }

  bool isInstructionOf(ZydisMnemonic mnemonic) const noexcept;

  const DestInfo destination(std::size_t no) const;
  const SrcInfo source(std::size_t no) const;

  bool setNote(const char* note) noexcept;
  inline const std::string& note() const noexcept {
    return note_;
  }

  inline const std::string& isa() const noexcept {
    return isa_;
  }

  inline std::size_t length() const noexcept {
    return length_;
  }

  inline const OperandSet& operands() const noexcept {
    return operands_;
  }

 protected:
  inline const ZydisMnemonic& mnemonic() const noexcept {
    return mnemonic_;
  }

 private:
  std::string str_{};
  std::string note_{};
  std::string isa_{};
  const std::size_t length_;
  const ZydisMnemonic mnemonic_;
  const OperandSet operands_;

  // Factoryを介さずにSeqUnitクラス群を作成してほしくないので
  // Factoryをfriendクラスにしつつ、子クラスのコンストラクタをprivateに記述
  // friendは継承されないので各子クラスでもfriend宣言が必要なのがアレ
  template <class U>
    requires std::is_base_of_v<SeqUnit, U>
  friend class SeqUnitFactory;

  virtual std::string makeString() noexcept = 0;
};

static inline std::string isaSetToString(ZydisISASet isaset) noexcept {
  switch (isaset) {
    case ZYDIS_ISA_SET_INVALID:
      return std::string{"INVALID"};
    case ZYDIS_ISA_SET_ADOX_ADCX:
      return std::string{"ADOX_ADCX"};
    case ZYDIS_ISA_SET_AES:
      return std::string{"AES"};
    case ZYDIS_ISA_SET_AMD:
      return std::string{"AMD"};
    case ZYDIS_ISA_SET_AMD3DNOW:
      return std::string{"AMD3DNOW"};
    case ZYDIS_ISA_SET_AMD_INVLPGB:
      return std::string{"AMD_INVLPGB"};
    case ZYDIS_ISA_SET_AMX_BF16:
      return std::string{"AMX_BF16"};
    case ZYDIS_ISA_SET_AMX_FP16:
      return std::string{"AMX_FP16"};
    case ZYDIS_ISA_SET_AMX_INT8:
      return std::string{"AMX_INT8"};
    case ZYDIS_ISA_SET_AMX_TILE:
      return std::string{"AMX_TILE"};
    case ZYDIS_ISA_SET_AVX:
      return std::string{"AVX"};
    case ZYDIS_ISA_SET_AVX2:
      return std::string{"AVX2"};
    case ZYDIS_ISA_SET_AVX2GATHER:
      return std::string{"AVX2GATHER"};
    case ZYDIS_ISA_SET_AVX512BW_128:
      return std::string{"AVX512BW_128"};
    case ZYDIS_ISA_SET_AVX512BW_128N:
      return std::string{"AVX512BW_128N"};
    case ZYDIS_ISA_SET_AVX512BW_256:
      return std::string{"AVX512BW_256"};
    case ZYDIS_ISA_SET_AVX512BW_512:
      return std::string{"AVX512BW_512"};
    case ZYDIS_ISA_SET_AVX512BW_KOP:
      return std::string{"AVX512BW_KOP"};
    case ZYDIS_ISA_SET_AVX512CD_128:
      return std::string{"AVX512CD_128"};
    case ZYDIS_ISA_SET_AVX512CD_256:
      return std::string{"AVX512CD_256"};
    case ZYDIS_ISA_SET_AVX512CD_512:
      return std::string{"AVX512CD_512"};
    case ZYDIS_ISA_SET_AVX512DQ_128:
      return std::string{"AVX512DQ_128"};
    case ZYDIS_ISA_SET_AVX512DQ_128N:
      return std::string{"AVX512DQ_128N"};
    case ZYDIS_ISA_SET_AVX512DQ_256:
      return std::string{"AVX512DQ_256"};
    case ZYDIS_ISA_SET_AVX512DQ_512:
      return std::string{"AVX512DQ_512"};
    case ZYDIS_ISA_SET_AVX512DQ_KOP:
      return std::string{"AVX512DQ_KOP"};
    case ZYDIS_ISA_SET_AVX512DQ_SCALAR:
      return std::string{"AVX512DQ_SCALAR"};
    case ZYDIS_ISA_SET_AVX512ER_512:
      return std::string{"AVX512ER_512"};
    case ZYDIS_ISA_SET_AVX512ER_SCALAR:
      return std::string{"AVX512ER_SCALAR"};
    case ZYDIS_ISA_SET_AVX512F_128:
      return std::string{"AVX512F_128"};
    case ZYDIS_ISA_SET_AVX512F_128N:
      return std::string{"AVX512F_128N"};
    case ZYDIS_ISA_SET_AVX512F_256:
      return std::string{"AVX512F_256"};
    case ZYDIS_ISA_SET_AVX512F_512:
      return std::string{"AVX512F_512"};
    case ZYDIS_ISA_SET_AVX512F_KOP:
      return std::string{"AVX512F_KOP"};
    case ZYDIS_ISA_SET_AVX512F_SCALAR:
      return std::string{"AVX512F_SCALAR"};
    case ZYDIS_ISA_SET_AVX512PF_512:
      return std::string{"AVX512PF_512"};
    case ZYDIS_ISA_SET_AVX512_4FMAPS_512:
      return std::string{"AVX512_4FMAPS_512"};
    case ZYDIS_ISA_SET_AVX512_4FMAPS_SCALAR:
      return std::string{"AVX512_4FMAPS_SCALAR"};
    case ZYDIS_ISA_SET_AVX512_4VNNIW_512:
      return std::string{"AVX512_4VNNIW_512"};
    case ZYDIS_ISA_SET_AVX512_BF16_128:
      return std::string{"AVX512_BF16_128"};
    case ZYDIS_ISA_SET_AVX512_BF16_256:
      return std::string{"AVX512_BF16_256"};
    case ZYDIS_ISA_SET_AVX512_BF16_512:
      return std::string{"AVX512_BF16_512"};
    case ZYDIS_ISA_SET_AVX512_BITALG_128:
      return std::string{"AVX512_BITALG_128"};
    case ZYDIS_ISA_SET_AVX512_BITALG_256:
      return std::string{"AVX512_BITALG_256"};
    case ZYDIS_ISA_SET_AVX512_BITALG_512:
      return std::string{"AVX512_BITALG_512"};
    case ZYDIS_ISA_SET_AVX512_FP16_128:
      return std::string{"AVX512_FP16_128"};
    case ZYDIS_ISA_SET_AVX512_FP16_128N:
      return std::string{"AVX512_FP16_128N"};
    case ZYDIS_ISA_SET_AVX512_FP16_256:
      return std::string{"AVX512_FP16_256"};
    case ZYDIS_ISA_SET_AVX512_FP16_512:
      return std::string{"AVX512_FP16_512"};
    case ZYDIS_ISA_SET_AVX512_FP16_SCALAR:
      return std::string{"AVX512_FP16_SCALAR"};
    case ZYDIS_ISA_SET_AVX512_GFNI_128:
      return std::string{"AVX512_GFNI_128"};
    case ZYDIS_ISA_SET_AVX512_GFNI_256:
      return std::string{"AVX512_GFNI_256"};
    case ZYDIS_ISA_SET_AVX512_GFNI_512:
      return std::string{"AVX512_GFNI_512"};
    case ZYDIS_ISA_SET_AVX512_IFMA_128:
      return std::string{"AVX512_IFMA_128"};
    case ZYDIS_ISA_SET_AVX512_IFMA_256:
      return std::string{"AVX512_IFMA_256"};
    case ZYDIS_ISA_SET_AVX512_IFMA_512:
      return std::string{"AVX512_IFMA_512"};
    case ZYDIS_ISA_SET_AVX512_VAES_128:
      return std::string{"AVX512_VAES_128"};
    case ZYDIS_ISA_SET_AVX512_VAES_256:
      return std::string{"AVX512_VAES_256"};
    case ZYDIS_ISA_SET_AVX512_VAES_512:
      return std::string{"AVX512_VAES_512"};
    case ZYDIS_ISA_SET_AVX512_VBMI2_128:
      return std::string{"AVX512_VBMI2_128"};
    case ZYDIS_ISA_SET_AVX512_VBMI2_256:
      return std::string{"AVX512_VBMI2_256"};
    case ZYDIS_ISA_SET_AVX512_VBMI2_512:
      return std::string{"AVX512_VBMI2_512"};
    case ZYDIS_ISA_SET_AVX512_VBMI_128:
      return std::string{"AVX512_VBMI_128"};
    case ZYDIS_ISA_SET_AVX512_VBMI_256:
      return std::string{"AVX512_VBMI_256"};
    case ZYDIS_ISA_SET_AVX512_VBMI_512:
      return std::string{"AVX512_VBMI_512"};
    case ZYDIS_ISA_SET_AVX512_VNNI_128:
      return std::string{"AVX512_VNNI_128"};
    case ZYDIS_ISA_SET_AVX512_VNNI_256:
      return std::string{"AVX512_VNNI_256"};
    case ZYDIS_ISA_SET_AVX512_VNNI_512:
      return std::string{"AVX512_VNNI_512"};
    case ZYDIS_ISA_SET_AVX512_VP2INTERSECT_128:
      return std::string{"AVX512_VP2INTERSECT_128"};
    case ZYDIS_ISA_SET_AVX512_VP2INTERSECT_256:
      return std::string{"AVX512_VP2INTERSECT_256"};
    case ZYDIS_ISA_SET_AVX512_VP2INTERSECT_512:
      return std::string{"AVX512_VP2INTERSECT_512"};
    case ZYDIS_ISA_SET_AVX512_VPCLMULQDQ_128:
      return std::string{"AVX512_VPCLMULQDQ_128"};
    case ZYDIS_ISA_SET_AVX512_VPCLMULQDQ_256:
      return std::string{"AVX512_VPCLMULQDQ_256"};
    case ZYDIS_ISA_SET_AVX512_VPCLMULQDQ_512:
      return std::string{"AVX512_VPCLMULQDQ_512"};
    case ZYDIS_ISA_SET_AVX512_VPOPCNTDQ_128:
      return std::string{"AVX512_VPOPCNTDQ_128"};
    case ZYDIS_ISA_SET_AVX512_VPOPCNTDQ_256:
      return std::string{"AVX512_VPOPCNTDQ_256"};
    case ZYDIS_ISA_SET_AVX512_VPOPCNTDQ_512:
      return std::string{"AVX512_VPOPCNTDQ_512"};
    case ZYDIS_ISA_SET_AVXAES:
      return std::string{"AVXAES"};
    case ZYDIS_ISA_SET_AVX_GFNI:
      return std::string{"AVX_GFNI"};
    case ZYDIS_ISA_SET_AVX_IFMA:
      return std::string{"AVX_IFMA"};
    case ZYDIS_ISA_SET_AVX_NE_CONVERT:
      return std::string{"AVX_NE_CONVERT"};
    case ZYDIS_ISA_SET_AVX_VNNI:
      return std::string{"AVX_VNNI"};
    case ZYDIS_ISA_SET_AVX_VNNI_INT16:
      return std::string{"AVX_VNNI_INT16"};
    case ZYDIS_ISA_SET_AVX_VNNI_INT8:
      return std::string{"AVX_VNNI_INT8"};
    case ZYDIS_ISA_SET_BMI1:
      return std::string{"BMI1"};
    case ZYDIS_ISA_SET_BMI2:
      return std::string{"BMI2"};
    case ZYDIS_ISA_SET_CET:
      return std::string{"CET"};
    case ZYDIS_ISA_SET_CLDEMOTE:
      return std::string{"CLDEMOTE"};
    case ZYDIS_ISA_SET_CLFLUSHOPT:
      return std::string{"CLFLUSHOPT"};
    case ZYDIS_ISA_SET_CLFSH:
      return std::string{"CLFSH"};
    case ZYDIS_ISA_SET_CLWB:
      return std::string{"CLWB"};
    case ZYDIS_ISA_SET_CLZERO:
      return std::string{"CLZERO"};
    case ZYDIS_ISA_SET_CMOV:
      return std::string{"CMOV"};
    case ZYDIS_ISA_SET_CMPXCHG16B:
      return std::string{"CMPXCHG16B"};
    case ZYDIS_ISA_SET_ENQCMD:
      return std::string{"ENQCMD"};
    case ZYDIS_ISA_SET_F16C:
      return std::string{"F16C"};
    case ZYDIS_ISA_SET_FAT_NOP:
      return std::string{"FAT_NOP"};
    case ZYDIS_ISA_SET_FCMOV:
      return std::string{"FCMOV"};
    case ZYDIS_ISA_SET_FCOMI:
      return std::string{"FCOMI"};
    case ZYDIS_ISA_SET_FMA:
      return std::string{"FMA"};
    case ZYDIS_ISA_SET_FMA4:
      return std::string{"FMA4"};
    case ZYDIS_ISA_SET_FXSAVE:
      return std::string{"FXSAVE"};
    case ZYDIS_ISA_SET_FXSAVE64:
      return std::string{"FXSAVE64"};
    case ZYDIS_ISA_SET_GFNI:
      return std::string{"GFNI"};
    case ZYDIS_ISA_SET_HRESET:
      return std::string{"HRESET"};
    case ZYDIS_ISA_SET_I186:
      return std::string{"I186"};
    case ZYDIS_ISA_SET_I286PROTECTED:
      return std::string{"I286PROTECTED"};
    case ZYDIS_ISA_SET_I286REAL:
      return std::string{"I286REAL"};
    case ZYDIS_ISA_SET_I386:
      return std::string{"I386"};
    case ZYDIS_ISA_SET_I486:
      return std::string{"I486"};
    case ZYDIS_ISA_SET_I486REAL:
      return std::string{"I486REAL"};
    case ZYDIS_ISA_SET_I86:
      return std::string{"I86"};
    case ZYDIS_ISA_SET_ICACHE_PREFETCH:
      return std::string{"ICACHE_PREFETCH"};
    case ZYDIS_ISA_SET_INVPCID:
      return std::string{"INVPCID"};
    case ZYDIS_ISA_SET_KEYLOCKER:
      return std::string{"KEYLOCKER"};
    case ZYDIS_ISA_SET_KEYLOCKER_WIDE:
      return std::string{"KEYLOCKER_WIDE"};
    case ZYDIS_ISA_SET_KNCE:
      return std::string{"KNCE"};
    case ZYDIS_ISA_SET_KNCJKBR:
      return std::string{"KNCJKBR"};
    case ZYDIS_ISA_SET_KNCSTREAM:
      return std::string{"KNCSTREAM"};
    case ZYDIS_ISA_SET_KNCV:
      return std::string{"KNCV"};
    case ZYDIS_ISA_SET_KNC_MISC:
      return std::string{"KNC_MISC"};
    case ZYDIS_ISA_SET_KNC_PF_HINT:
      return std::string{"KNC_PF_HINT"};
    case ZYDIS_ISA_SET_LAHF:
      return std::string{"LAHF"};
    case ZYDIS_ISA_SET_LONGMODE:
      return std::string{"LONGMODE"};
    case ZYDIS_ISA_SET_LWP:
      return std::string{"LWP"};
    case ZYDIS_ISA_SET_LZCNT:
      return std::string{"LZCNT"};
    case ZYDIS_ISA_SET_MCOMMIT:
      return std::string{"MCOMMIT"};
    case ZYDIS_ISA_SET_MONITOR:
      return std::string{"MONITOR"};
    case ZYDIS_ISA_SET_MONITORX:
      return std::string{"MONITORX"};
    case ZYDIS_ISA_SET_MOVBE:
      return std::string{"MOVBE"};
    case ZYDIS_ISA_SET_MOVDIR:
      return std::string{"MOVDIR"};
    case ZYDIS_ISA_SET_MPX:
      return std::string{"MPX"};
    case ZYDIS_ISA_SET_MSRLIST:
      return std::string{"MSRLIST"};
    case ZYDIS_ISA_SET_PADLOCK_ACE:
      return std::string{"PADLOCK_ACE"};
    case ZYDIS_ISA_SET_PADLOCK_PHE:
      return std::string{"PADLOCK_PHE"};
    case ZYDIS_ISA_SET_PADLOCK_PMM:
      return std::string{"PADLOCK_PMM"};
    case ZYDIS_ISA_SET_PADLOCK_RNG:
      return std::string{"PADLOCK_RNG"};
    case ZYDIS_ISA_SET_PAUSE:
      return std::string{"PAUSE"};
    case ZYDIS_ISA_SET_PBNDKB:
      return std::string{"PBNDKB"};
    case ZYDIS_ISA_SET_PCLMULQDQ:
      return std::string{"PCLMULQDQ"};
    case ZYDIS_ISA_SET_PCOMMIT:
      return std::string{"PCOMMIT"};
    case ZYDIS_ISA_SET_PCONFIG:
      return std::string{"PCONFIG"};
    case ZYDIS_ISA_SET_PENTIUMMMX:
      return std::string{"PENTIUMMMX"};
    case ZYDIS_ISA_SET_PENTIUMREAL:
      return std::string{"PENTIUMREAL"};
    case ZYDIS_ISA_SET_PKU:
      return std::string{"PKU"};
    case ZYDIS_ISA_SET_POPCNT:
      return std::string{"POPCNT"};
    case ZYDIS_ISA_SET_PPRO:
      return std::string{"PPRO"};
    case ZYDIS_ISA_SET_PREFETCHWT1:
      return std::string{"PREFETCHWT1"};
    case ZYDIS_ISA_SET_PREFETCH_NOP:
      return std::string{"PREFETCH_NOP"};
    case ZYDIS_ISA_SET_PT:
      return std::string{"PT"};
    case ZYDIS_ISA_SET_RAO_INT:
      return std::string{"RAO_INT"};
    case ZYDIS_ISA_SET_RDPID:
      return std::string{"RDPID"};
    case ZYDIS_ISA_SET_RDPMC:
      return std::string{"RDPMC"};
    case ZYDIS_ISA_SET_RDPRU:
      return std::string{"RDPRU"};
    case ZYDIS_ISA_SET_RDRAND:
      return std::string{"RDRAND"};
    case ZYDIS_ISA_SET_RDSEED:
      return std::string{"RDSEED"};
    case ZYDIS_ISA_SET_RDTSCP:
      return std::string{"RDTSCP"};
    case ZYDIS_ISA_SET_RDWRFSGS:
      return std::string{"RDWRFSGS"};
    case ZYDIS_ISA_SET_RTM:
      return std::string{"RTM"};
    case ZYDIS_ISA_SET_SERIALIZE:
      return std::string{"SERIALIZE"};
    case ZYDIS_ISA_SET_SGX:
      return std::string{"SGX"};
    case ZYDIS_ISA_SET_SGX_ENCLV:
      return std::string{"SGX_ENCLV"};
    case ZYDIS_ISA_SET_SHA:
      return std::string{"SHA"};
    case ZYDIS_ISA_SET_SHA512:
      return std::string{"SHA512"};
    case ZYDIS_ISA_SET_SM3:
      return std::string{"SM3"};
    case ZYDIS_ISA_SET_SM4:
      return std::string{"SM4"};
    case ZYDIS_ISA_SET_SMAP:
      return std::string{"SMAP"};
    case ZYDIS_ISA_SET_SMX:
      return std::string{"SMX"};
    case ZYDIS_ISA_SET_SNP:
      return std::string{"SNP"};
    case ZYDIS_ISA_SET_SSE:
      return std::string{"SSE"};
    case ZYDIS_ISA_SET_SSE2:
      return std::string{"SSE2"};
    case ZYDIS_ISA_SET_SSE2MMX:
      return std::string{"SSE2MMX"};
    case ZYDIS_ISA_SET_SSE3:
      return std::string{"SSE3"};
    case ZYDIS_ISA_SET_SSE3X87:
      return std::string{"SSE3X87"};
    case ZYDIS_ISA_SET_SSE4:
      return std::string{"SSE4"};
    case ZYDIS_ISA_SET_SSE42:
      return std::string{"SSE42"};
    case ZYDIS_ISA_SET_SSE4A:
      return std::string{"SSE4A"};
    case ZYDIS_ISA_SET_SSEMXCSR:
      return std::string{"SSEMXCSR"};
    case ZYDIS_ISA_SET_SSE_PREFETCH:
      return std::string{"SSE_PREFETCH"};
    case ZYDIS_ISA_SET_SSSE3:
      return std::string{"SSSE3"};
    case ZYDIS_ISA_SET_SSSE3MMX:
      return std::string{"SSSE3MMX"};
    case ZYDIS_ISA_SET_SVM:
      return std::string{"SVM"};
    case ZYDIS_ISA_SET_TBM:
      return std::string{"TBM"};
    case ZYDIS_ISA_SET_TDX:
      return std::string{"TDX"};
    case ZYDIS_ISA_SET_TSX_LDTRK:
      return std::string{"TSX_LDTRK"};
    case ZYDIS_ISA_SET_UINTR:
      return std::string{"UINTR"};
    case ZYDIS_ISA_SET_VAES:
      return std::string{"VAES"};
    case ZYDIS_ISA_SET_VMFUNC:
      return std::string{"VMFUNC"};
    case ZYDIS_ISA_SET_VPCLMULQDQ:
      return std::string{"VPCLMULQDQ"};
    case ZYDIS_ISA_SET_VTX:
      return std::string{"VTX"};
    case ZYDIS_ISA_SET_WAITPKG:
      return std::string{"WAITPKG"};
    case ZYDIS_ISA_SET_WRMSRNS:
      return std::string{"WRMSRNS"};
    case ZYDIS_ISA_SET_X87:
      return std::string{"X87"};
    case ZYDIS_ISA_SET_XOP:
      return std::string{"XOP"};
    case ZYDIS_ISA_SET_XSAVE:
      return std::string{"XSAVE"};
    case ZYDIS_ISA_SET_XSAVEC:
      return std::string{"XSAVEC"};
    case ZYDIS_ISA_SET_XSAVEOPT:
      return std::string{"XSAVEOPT"};
    case ZYDIS_ISA_SET_XSAVES:
      return std::string{"XSAVES"};
    default:
      return std::string{"NONE"};
  }
}

template <class U>
  requires std::is_base_of_v<SeqUnit, U>
U SeqUnitFactory<U>::create(HANDLE hProcess, const Registers& regs, const ZydisDisassembledInstruction& inst) {
  try {
    U unit{hProcess, regs, std::move(inst)};
    unit.str_ = static_cast<SeqUnit&>(unit).makeString();
    unit.isa_ = isaSetToString(inst.info.meta.isa_set);
    return unit;
  } catch (...) {
    throw;
  }
}

};  // namespace seq
