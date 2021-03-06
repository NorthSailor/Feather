#include "testbench.h"
#include "Vreg_file.h"

class TB_REG_FILE : public TB_SEQUENTIAL<Vreg_file>
{
public:
  bool verify()
  {
    // Set every register to its index.
    // e.g. R0 = 0, R1 = 1, etc.
    for (int i = 0; i < 15; i++) {
      m_dut->write_enable1_i = 1;
      m_dut->address_write1_i = i;
      m_dut->write_data1_i = i;
      tick();
    }

    // Do the same for the PC.
    m_dut->r15_i = 0xF;
    tick();

    // Disable the write_enable bit and try to set every register to 0.
    for (int i = 0; i < 16; i++) {
      m_dut->write_enable2_i = 0;
      m_dut->address_write2_i = i;
      m_dut->write_data2_i = 0;
      tick();
    }

    // Read the pairs (i, 15 - i) and check the results.
    for (int i = 0; i < 16; i++) {
      m_dut->address1_i = i;
      m_dut->address2_i = (15 - i);
      tick();
      if ((m_dut->output1_o != i) || (m_dut->output2_o != (15 - i))) {
        printf("Test failed.\n");
        return false;
      }
    }
    return true;
  }
};

int
main(int argc, char** argv)
{
  Verilated::commandArgs(argc, argv);
  TB_REG_FILE tb;
  tb.opentrace("out.vcd");
  return tb.verify() ? 0 : 1;
}