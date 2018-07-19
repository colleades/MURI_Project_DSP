// Welch, Wright, & Morrow, 
// Real-time Digital Signal Processing, 2017

///////////////////////////////////////////////////////////////////////
// Filename: ISRs.c
//
// Synopsis: Interrupt service routine for codec data transmit/receive
//
///////////////////////////////////////////////////////////////////////

#include "DSP_Config.h" 
#include "Echo.h" 
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
  
// Data is received as 2 16-bit words (left/right) packed into one
// 32-bit word.  The union allows the data to be accessed as a single 
// entity when transferring to and from the serial port, but still be 
// able to manipulate the left and right channels independently.

#define LEFT  0
#define RIGHT 1

volatile union {
    Uint32 UINT;
    Int16 Channel[2];
} CodecDataIn, CodecDataOut;


//GLOBAL VARIABLES
//**********************************************
//Recording
int recIndex = 0;
int bufferEnd = (24000*8)-1;//default to 120 bpm
float xLeft, xRight, yLeft, yRight;
#define BUFFER_LENGTH   1152000 // buffer length in samples
#pragma DATA_SECTION (buffer, "CE0"); // put "buffer" in SDRAM
volatile float buffer[2][BUFFER_LENGTH]; // space for left + right

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


void ZeroBuffer()
///////////////////////////////////////////////////////////////////////
// Purpose:   Initial fill of all buffer locations with 0.0
//
// Input:     None
//
// Returns:   Nothing
//
// Calls:     Nothing
//
// Notes:     None
///////////////////////////////////////////////////////////////////////
{
    int i;

    for(i=0; i < BUFFER_LENGTH; i++)  {
        buffer[LEFT][i] = 0.0;
        buffer[RIGHT][i] = 0.0;  
        }
}


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

            //set buffer size
            bufferEnd = ((oneBeat*8)-1);

     }

     sampleCounter = 0;
}

//SET BEAT REPEAT
//***************
void setBeatRepeat(int serialNumber){

        setActiveButton(1);

        int beatDivision = oneBeat/serialNumber;

    //set playback index to current record spot
    playbackIndex = recIndex;

        //set loop start and end points
        loopStart = playbackIndex;
        loopEnd = ((playbackIndex+beatDivision)%bufferEnd);

}

float serialToPlaybackSpeed(int serialNumber)
{
     //change playback speed
        if (serialNumber == 20){

        //-2
                return -2;

        }else if (serialNumber == 21){


            //-1
                return -1;


        }else if (serialNumber == 22){

        //-0.5
            return -0.5;

        }else if (serialNumber == 23){

                //tape stoppppppppp (0)
                return 0;

        }else if (serialNumber == 24){

            //0.5
                return 0.5;

        }else if (serialNumber == 25){

                //1
            return 1;

        }else if (serialNumber == 26){

                //2
                return 2;

        }else if (serialNumber == 27){

                //4
                return 4;

        }else{

                return 0;

        }
}

//TIME TRAVEL
//***********
void setTimeTravel(int serialNumber){

    float newPlaybackSpeed = serialToPlaybackSpeed(serialNumber);

    if (activeButton == 0)//pressing to jump to a speed
    {
        setActiveButton(2);
        playbackSpeed = newPlaybackSpeed;
        endSpeed = newPlaybackSpeed;
    }
    else if (activeButton == 2)//when dragging finger to different speeds
    {
        if(playbackSpeed == endSpeed)//initiate glide when starting from a stationary speed
        {
            glideCounter = 0;
            startSpeed = playbackSpeed;

        }
        endSpeed = newPlaybackSpeed;
    }

    //set playback to current recording spot
    playbackIndex = recIndex;

}




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
void timeTravel()
{
    if (playbackIndex > bufferEnd) // Forward and Backward circular playback
    {
        playbackIndex = (playbackIndex - bufferEnd - 1);
    }
    else if (playbackIndex < 0)
    {
        playbackIndex = (bufferEnd + playbackIndex + 1);
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





interrupt void Codec_ISR()
///////////////////////////////////////////////////////////////////////
// Purpose:   Codec interface interrupt service routine
//
// Input:     None
//
// Returns:   Nothing
//
// Calls:     CheckForOverrun, ReadCodecData, WriteCodecData
//
// Notes:     None
///////////////////////////////////////////////////////////////////////
{
    /* add any local variables here */
    //Uint32 newest;  // only used for infinite echo

    if(CheckForOverrun())                   // overrun error occurred (i.e. halted DSP)
        return;                             // so serial port is reset to recover

    CodecDataIn.UINT = ReadCodecData();     // get input data samples


    recordCurrentSample();
    incTapTempoCounter();

    if(activeButton == 0)//No Buttons Pressed
    {
        dryPlayback();
    }
    else if(activeButton == 1)//BeatRepeat Button Pressed
    {
        beatRepeat();
    }
    else if(activeButton == 2)//PlaybackSlider Pressed
    {
        timeTravel();

    }


    WriteCodecData(CodecDataOut.UINT);      // send output data to  port

}//end ISR


