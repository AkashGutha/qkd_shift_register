#include "hwlib.h"

// unsigned type defs
#define UCHAR unsigned char
#define UINT unsigned int
#define UFLOAT unsigned float
#define ULONG unsigned long

// unsigned types for int
#define UINT8 uint8_t
#define UINT16 uint16_t
#define UINT32 uint32_t
#define UINT64 uint64_t

// unsigned pointer types for int
#define UP8 uint8_t *
#define UP16 uint16_t *
#define UP32 uint32_t *
#define UP64 uint64_t *

//setting for the HPS2FPGA AXI Bridge
#define ALT_AXI_FPGASLVS_OFST (0xC0000000) // axi_master
#define HW_FPGA_AXI_SPAN (0x40000000) // Bridge span 1GB
#define HW_FPGA_AXI_MASK ( HW_FPGA_AXI_SPAN - 1 )

//settings for the lightweight HPS-to-FPGA bridge
#define HW_REGS_BASE ( ALT_STM_OFST )
#define HW_REGS_SPAN ( 0x04000000 ) //64 MB with 32 bit adress space this is 256 MB
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 )

