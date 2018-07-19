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
     
        //TAP TEMPO
        //*********
        void tapTempo(){
          
          //if sampleCounter is less than maximum beat size (20 samples), set oneBeat to current number of samples
          if (sampleCounter < 144000){
            
            //1beat
            oneBeat = sampleCounter;
            halfBeat = (sampleCounter/2);
            thirdBeat = (sampleCounter/3);
            
            //set buffer size
            bufferEnd = (oneBeat*8);
          }
          
          sampleCounter = 0;
          ;
          
        }
          
        //SET BEAT REPEAT
        //***************
        void setBeatRepeat(intSerialTest){
          
          setActiveButton(1);
          
          //set loop start and end points
          loopStart = recIndex;
          loopEnd = ((recIndex+oneBeat)%bufferEnd);
          
          
          
          
        }
          
        //TIME TRAVEL
        //***********
        void setTimeTravel(intSerialTest){
          
          setActiveButton(2);
          
          //change playback speed
          if (intSerialTest == 20){
            
            //-2
            playbackSpeed = -2;
            
          }else if (intSerialTest == 21){
            
            //-1
            playbackSpeed = -1;
          
            
          }else if (intSerialTest == 22){
            
            //-0.5
            playbackSpeed = -0.5;
            
          }else if (intSerialTest == 23){
            
            //tape stoppppppppp (0)
            playbackSpeed = 0;
         
          }else if (intSerialTest == 24){
            
            //0.5
            playbackSpeed = 0.5;
            
          }else if (intSerialTest == 25){
            
            //1
            playbackSpeed = 1
            
          }else if (intSerialTest == 26){
            
            //2
            playbackSpeed = 2
            
          }else if (intSerialTest == 27){
            
            //4
            playbackSpeed = 4;
            
          }else{
            
            playbackSpeed = 0;
            
          }
                 
        }

    }
