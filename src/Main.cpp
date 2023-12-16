#include "SeqMaker.hpp"

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;
using namespace Seq;

int main() {
  uint32_t pid;
  cout << "PID: ";
  cin >> pid;

  try {
    SeqMaker maker{pid};

    Registers regs;
    regs.EAX = 0x00000000;
    regs.EBX = 0x00000000;
    regs.ECX = 0x00000000;
    regs.EDX = 0x00000000;
    regs.EBP = 0x00000000;
    regs.ESP = 0x00000000;
    regs.ESI = 0x00000000;
    regs.EDI = 0x00000000;
    regs.EIP = 0x00000000;
    regs.DS = 0x0000;
    regs.ES = 0x0000;
    regs.CS = 0x0000;
    regs.SS = 0x0000;
    regs.FS = 0x0000;
    regs.GS = 0x0000;
    maker.addInstruction(regs);
    maker.addInstruction(regs);
    maker.addInstruction(regs);
    maker.addInstruction(regs);

    auto&& ngram = maker.createNGram<2>();

    cout << "N-gram: " << static_cast<string>(ngram) << endl;
  } catch (const runtime_error& e) {
    cerr << e.what() << endl;
    return 1;
  }

  return 0;
}
