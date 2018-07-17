// Welch, Wright, & Morrow, 
// Real-time Digital Signal Processing, 2017
 
///////////////////////////////////////////////////////////////////////
// Filename: main.c
//
// Synopsis: Main program file for demonstration code
//
///////////////////////////////////////////////////////////////////////

#include "DSP_Config.h"
#include <stdio.h>

int main()
  {

    // initialize DSP board
    DSP_Init();
    Init_UART2(115200);

    // call StartUp for application specific code
    // defined in each application directory
    StartUp();

    // main stalls here, interrupts drive operation
    while(1)
    {
        Uint8 dataReady = IsDataReady_UART2();

        if(dataReady != 0)
        {
           Uint8 serialTest = Read_UART2();
           printf("%u", serialTest);
           puts("\n");
           //puts("Cats");

        }

    }


}


