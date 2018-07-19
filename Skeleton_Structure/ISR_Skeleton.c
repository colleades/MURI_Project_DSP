//GLOBAL VARIABLES
//**********************************************
//Recording
int recIndex = 0;
int bufferEnd = (24000*8)-1;//default to 120 bpm

//Wet Playback
int activeButton = 0;
int playbackIndex = 0;

//Time Travel
float playbackSpeed = 1;
float startSpeed = 1;
float endSpeed = 1;
int glideCounter = 0;

//BeatRepeat
int loopStart = 0;
int loopEnd = 24000;

//Tap Tempo
int sampleCounter = 144000;
int oneBeat = 24000;
int halfBeat = 12000;
int thirdBeat = 8000;

//SET ACTIVE BUTTON
//*******************************
void setActiveButton(int newButtonValue)
{
	activeButton = newButtonValue;
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
           	bufferEnd = ((oneBeat*8)-1);
		 
	 }
	
	 sampleCounter = 0;
}
         
//SET BEAT REPEAT
//***************
void setBeatRepeat(intSerialTest){
	
        setActiveButton(1);
	
	//set playback index to current record spot
	playbackIndex = recIndex;
          
        //set loop start and end points
        loopStart = playbackIndex;
        loopEnd = ((playbackIndex+oneBeat)%bufferEnd);
                   
}
         
//TIME TRAVEL
//***********
void setTimeTravel(intSerialTest){
	
	
          
        setActiveButton(2);
	
	//set playback to current recording spot
	playbackIndex = recIndex;
          
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




//ISR
//**********************************************
//**********************************************
interrupt ISR()
{
	recordCurrentSample();
	incTapTempoCounter();
	
	if(activeButton == 0)//No Buttons Pressed
	}
		dryPlayback();
	}
	else if(activeButton == 1)//BeatRepeat Button Pressed
	}
		beatRepeat();
	}
	else if(activeButton == 2)//PlaybackSlider Pressed
	}
		playbackSpeed();
	}
	
}//end ISR


//RECORD CURRENT SAMPLE
//************************
void recordCurrentSample()
{
	buffer[LEFT][recIndex] = CodecDataIn.Channel[LEFT];
	buffer[RIGHT][recIndex] = CodecDataIn.Channel[RIGHT];
	
	if (++recIndex >= bufferEnd) // implement circular buffer
		recIndex = 0;
}//end record current sample


//INCREMENT TAP TEMPO COUNTER CURRENT SAMPLE
//************************
void incTapTempoCounter()
{
	if(sampleCounter != 144000)
	{
		sampleCounter++;
	}
}//end incTapTempoCounter


//DRY PLAYBACK
//************************
void dryPlayback()
{
	CodecDataOut.Channel[LEFT] = CodecDataIn.Channel[LEFT];
	CodecDataOut.Channel[RIGHT] = CodecDataIn.Channel[RIGHT];
}//end dry playbackIndex


//BEAT REPEAT
//************************
void beatRepeat()
{
	//output buffer audio
	int intPlaybackIndex = (int)(playbackIndex);
	
	CodecDataOut.Channel[LEFT] = buffer[LEFT][intPlaybackIndex];
	CodecDataOut.Channel[RIGHT] = buffer[RIGHT][intPlaybackIndex];
	
	playbackIndex++;
	
	if(playbackIndex == (loopEnd + 1))//loop points
		playbackIndex = loopStart;
	
	if (playbackIndex >= bufferEnd) //wrap around buffer
		playbackIndex = 0;		
	
}//end beat repeat

//PLAYBACK SPEED
//************************
void playbackSpeed()
{
	if (playbackIndex >= bufferEnd) // Forward and Backward circular playback
	{
		playbackIndex = 0;
	}
	else if (playbackIndex < 0)
	{
		playbackIndex = bufferEnd;
	}
	
	int intPlaybackIndex = (int)(playbackIndex);
	
	CodecDataOut.Channel[LEFT] = buffer[LEFT][intPlaybackIndex];
	CodecDataOut.Channel[RIGHT] = buffer[RIGHT][intPlaybackIndex];
	
	if(playbackSpeed != endSpeed)//glide playback speed
	{
		playbackSpeed = ((endSpeed - startSpeed)*(glideCounter/oneBeat)+startSpeed);
		glideCounter++;
	}
	
	//get the new playback index
	playbackIndex = playbackIndex + playbackSpeed;
}//end playback speed


