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
int sampleCounter = 0;
int oneBeat = 24000;
int halfBeat = 12000;
int thirdBeat = 8000;

//ISR
//**********************************************
interrupt ISR()
{
	recordCurrentSample();
	
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
	CodecDataOut.Channel[LEFT] = buffer[LEFT][playbackIndex];
	CodecDataOut.Channel[RIGHT] = buffer[RIGHT][playbackIndex];
	
	if (++playbackIndex >= loopEnd) // implement circular buffer
		playbackIndexIndex = 0;
}//end beat repeat

//PLAYBACK SPEED
//************************
void playbackSpeed()
{
	
}//end playback speed

//SET ACTIVE BUTTON
//*******************************
void setActiveButton(int newButtonValue)
{
	activeButton = newButtonValue;
}
