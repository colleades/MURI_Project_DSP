// Welch, Wright, & Morrow, 
// Real-time Digital Signal Processing, 2017
 
///////////////////////////////////////////////////////////////////////
// Filename: main.c
//
// Synopsis: Main program file for demonstration code
//
///////////////////////////////////////////////////////////////////////

#include "DSP_Config.h"   

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
           Uint8 serialIn = Read_UART2();
         
           //change unsigned to int
           int serialNumber = (int)(serialIn);
          
           //select which function is called in ISR
           if (serialNumber == 100){
             
             tapTempo();
           
           }else if (serialNumber !=0 && <4){
             
             //call beat repeat function and pass the specific beat repeat
             setBeatRepeat(serialNumber);
             
           }else if (serialNumber < 28 && > 4){
             
             //call time travel and pass specific time travel 
             setTimeTravel(serialNumber);
             
           }else{
             
             //if no recognizable serial number is inputted, continue dry playback
             setActiveButton(0);
               
           }//else
  
        }//if

    }//while
 
}//main


