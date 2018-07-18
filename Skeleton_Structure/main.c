//this is the main


int main()
{
  
  
  //Initialize DSP board and call StartUp() function here
  
  //while loop
   while(1)
    {
        Uint8 dataReady = IsDataReady_UART2();

        if(dataReady != 0)
        {
           Uint8 serialTest = Read_UART2();
           
           //cast unsigned to int
           int intSerialTest = (int)(serialTest);
          
           //select which function is called in ISR
           if (intSerialTest == 100){
             
             tapTempo();
           
           }else if (intSerialTest !=0 && <4){
             
             //call beat repeat function and pass the specific beat repeat
             setBeatRepeat(intSerialTest);
             
           }else if (intSerialTest < 28 && > 4){
             
             //call time travel and pass specific time travel 
             setTimeTravel(intSerialTest);
             
           }else{
             
             //if no recognizable serial number is inputted, continue dry playback
             setActiveButton(0);
               
           }
     
        }

    }
