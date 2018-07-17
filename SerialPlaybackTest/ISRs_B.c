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

/* add any global variables here */
float xLeft, xRight, yLeft, yRight;
Uint32 recIndex = 0; // index for buffer value
float playbackIndex = 0; 

//Collin edits



//Original
float playbackSpeed = 1.0;
//variable for what current serial info is being sent
int serialValue = 0;
#define BUFFER_LENGTH   96000 // buffer length in samples
#pragma DATA_SECTION (buffer, "CE0"); // put "buffer" in SDRAM
volatile float buffer[2][BUFFER_LENGTH]; // space for left + right
volatile float gain = 0.75; /* set gain value for echoed sample */

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








/************RANDOM FUNCTION THAT'S STUPID*********/////////

void helloThere (int intSerialTest) {
	
	//set ISR serial value to whatever number is sent in from main
	serialValue = intSerialTest;
	
	/*puts("\n");
	printf("This is from the ISR file: %d", intSerialTest);
	puts("\n");*/
		
}

/****************************************************///////








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

 	if(CheckForOverrun())					// overrun error occurred (i.e. halted DSP)
		return;								// so serial port is reset to recover

  	CodecDataIn.UINT = ReadCodecData();		// get input data samples
	
	/* add your code starting here */

	/****************************
	ECHO ROUTINE BEGINS HERE
	****************************/
	/*xLeft = CodecDataIn.Channel[LEFT];   // current LEFT input value to float
	xRight = CodecDataIn.Channel[RIGHT];   // current RIGHT input value to float

	buffer[LEFT][recIndex] = xLeft;
	buffer[RIGHT][recIndex] = xRight;
	newest = oldest;
	if (++oldest >= BUFFER_LENGTH) // implement circular buffer
		oldest = 0;

	// use either FIR or IIR lines below

	// for FIR comb filter effect, uncomment next two lines
	yLeft = xLeft + (gain * buffer[LEFT][oldest]);
	yRight = xRight + (gain * buffer[RIGHT][oldest]);*/
    
	// for IIR comb filter effect, uncomment four lines below
	//buffer[LEFT][newest] = xLeft + (gain * buffer[LEFT][oldest]);
	//buffer[RIGHT][newest] = xRight + (gain * buffer[RIGHT][oldest]);
	//yLeft = buffer[LEFT][oldest];  // or use newest 
	//yRight = buffer[RIGHT][oldest];  // or use newest
	
	//RECORD TO BUFFER
	//~~~~~~~~~~~~~~~~~~~~~	
	buffer[LEFT][recIndex] = CodecDataIn.Channel[LEFT];
	buffer[RIGHT][recIndex] = CodecDataIn.Channel[RIGHT];
	
	if (++recIndex >= BUFFER_LENGTH) // implement circular buffer recording
		recIndex = 0;
	//~~~~~~~~~~~~~~~~~~~~~
	//end RECORD TO BUFFER	
	
	//PLAYBACK SPEED TEST
	//~~~~~~~~~~~~~~~~~~~~~	
	//int roundedPlaybackIndex = (int) round(playbackIndex);
	
	if (playbackIndex >= BUFFER_LENGTH) // Forward and Backward circular playback
	{
		playbackIndex = 0;
	}
	else if (playbackIndex < 0)
	{
		playbackIndex = (BUFFER_LENGTH - 1);
	}
	
	int intPlaybackIndex = (int)(playbackIndex);
	
	//PLAYBACK SPEED TEST!!!!!!!!!
	
	//if serialTest is 24, playbackspeed is .5, otherwise, it's 1
	if (serialValue == 24){
		
		playbackSpeed = 0.5;
	else
		playbackSpeed = 1;
		
		
	
	yLeft = buffer[LEFT][intPlaybackIndex];
	yRight = buffer[RIGHT][intPlaybackIndex];
	
	playbackIndex = (playbackIndex + playbackSpeed);
	
	/*int testIndex = 1231;
	
	yLeft = buffer[LEFT][roundedPlaybackIndex];	
	yRight = buffer[RIGHT][roundedPlaybackIndex];
	
	//printf("The left sample: %f\n The right sample: %f\n", yLeft, yRight);
	
	//exit(0);
	
	int newIndex = recIndex - 1;*/

	CodecDataOut.Channel[LEFT] = yLeft;   // output the LEFT value
	CodecDataOut.Channel[RIGHT] = yRight; // output the RIGHT value
	/*****************************/
	/* end your code here */

	WriteCodecData(CodecDataOut.UINT);		// send output data to  port
}

