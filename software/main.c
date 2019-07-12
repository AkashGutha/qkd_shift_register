#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>

#include "errno.h"
#include "hwlib.h"
#include "soc_cv_av/socal/socal.h"
#include "soc_cv_av/socal/hps.h"
#include "soc_cv_av/socal/alt_gpio.h"

#include "hps.h"
#include "quartus.h"

// pointers for virtual bases
void *h2p_lw_axi_vbase;
void *h2p_hw_axi_vbase;

// pointers for shift registers
void *shift_reg_1_on_axi_base;
void *shift_reg_2_on_axi_base;

// time variables
clock_t start, end;
double cpu_time_used;

// FILE pointer and other requirements for the input file
FILE *file;
UINT32 buffer[256];

int main(int argc, char const *argv[])
{
    // file descriptor
    int fd;
    unsigned int i = 0;

    // get the file path from arguments and open the file
    if (argc != 2)
    {
        printf("Please pass in 2 arguments instead of %d arguments\r\n", argc);
        return -1;
    }
    file = fopen(argv[1], "r");

    if (file == NULL)
    {
        printf("ERROR: Opening the file at %s \r\nError code: %d \r\n", argv[1], errno);
        return -1;
    }

    // OPEN "/dev/mem" -> Map the device peripherals into a linux vbase
    fd = open("/dev/mem", (O_RDWR | O_SYNC));
    if (fd == -1)
    {
        printf("ERROR: Could not open \"/dev/mem\"...\n");
        return 1;
    }

    // get the base address for the LW axi bridge
    h2p_lw_axi_vbase = mmap(NULL, HW_REGS_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, HW_REGS_BASE);

    if (h2p_lw_axi_vbase == MAP_FAILED)
    {
        printf("ERROR: lw axi mmap() failed..................\r\n");
        close(fd);
        return 1;
    }

    // get the base address for the HW axi bridge
    h2p_hw_axi_vbase = mmap(NULL, HW_FPGA_AXI_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, ALT_AXI_FPGASLVS_OFST);

    if (h2p_hw_axi_vbase == MAP_FAILED)
    {
        printf("ERROR: hw axi mmap() failed....................\r\n");
        close(fd);
        return 1;
    }

    //-------------------------------------------------------------
    // Read from the sd card and write into ram on the fpga
    //-------------------------------------------------------------

    printf("Starting the write process...........................\r\n");
    shift_reg_1_on_axi_base = h2p_hw_axi_vbase + ((ULONG)SHIFT_REG_MM_BASE & (ULONG)HW_FPGA_AXI_MASK);
    shift_reg_2_on_axi_base = h2p_hw_axi_vbase + ((ULONG)SHIFT_REG_MM_1_BASE & (ULONG)HW_FPGA_AXI_MASK);
    unsigned int index = -1;
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        index++;
        for (i = 0; i < sizeof(buffer); i++)
        {
            // start = clock();
            *((UP32)shift_reg_1_on_axi_base) = buffer[i];
            // *((UP32)shift_reg_2_on_axi_base) = buffer[i + 1];
            // end = clock();

            // double a = ((double)(end - start));
            // if (a > 20)
            // {
            //     printf("Write time for %dth byte transfer: ", i + (index * 256));
            //     printf("%.0f cycles \r\n", a);
            // }
        }
    }

    //-------------------------------------------------------------
    // clean up our memory mapping and exit
    //-------------------------------------------------------------
    if (munmap(h2p_hw_axi_vbase, HW_FPGA_AXI_SPAN) != 0)
    {
        printf("ERROR: axi munmap() failed.............................\r\n");
        close(fd);
        return 1;
    }

    fclose(file);
    close(fd);

    return 0;
}

long GetFileSize(const char *filename)
{
    long size;
    FILE *f;

    f = fopen(filename, "rb");
    if (f == NULL)
        return -1;
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fclose(f);

    return size;
}
