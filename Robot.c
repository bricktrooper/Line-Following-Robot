/*
============================ 
Authors:

Kyle Pinto
Naman Gulati
Matthew Fernandes

last update: 

Description:

Drives around octagon, reads
barcode, and picks up ball.

Hardware setup:
============================
*/

#include <pic.h>
#include <htc.h>

__CONFIG( FOSC_INTRCIO & WDTE_OFF & PWRTE_OFF & MCLRE_OFF & CP_OFF & CPD_OFF & BOREN_OFF & 
IESO_OFF & FCMEN_OFF );

//defined constants- the compiler does a substitution 


//Global Variables 

int leftSensor = 0;
int rightSensor = 0;
int counter = 0;
int LEFT_THRESHOLD = 140;
int RIGHT_THRESHOLD = 156;

// PREDEFINE PROCEDURES: procedure must be defined before you can use them

void init_hardware(void);
void forward(void);
void backward(void);
void left(void);
void right(void);
void stop(void);
void spinLeft(void);
void spinRight(void);
void runADC(void);
void sharpTurnLeft(void);
void sharpTurnRight(void);
void flashRSL(int);
void readSensors(void);
void armDown(void);
void armUp(void);

// MAIN PROGRAM //

void main(void)
{
	init_hardware();
	
	while(RA0 == 1){
		readSensors();	
	}
	
	LEFT_THRESHOLD=leftSensor + 10;
	RIGHT_THRESHOLD=rightSensor + 15;
	armDown();
	armUp();
	
	while(1)
	{
		RA5 = 1;
		
		readSensors();
		
		if((leftSensor >= LEFT_THRESHOLD) && (rightSensor >= RIGHT_THRESHOLD))  // touching line on both sides
		{	
			RB7 = 1;
			RC0 = 1;
			counter++;
			
				while ((leftSensor >= LEFT_THRESHOLD) && (rightSensor >= RIGHT_THRESHOLD))
				{
					forward();
					_delay(10000);
					right();
					_delay(9000);
					readSensors();
				}
				
				stop();
				flashRSL(counter);
				RB7 = 0;
				RC0 = 0;
				
				sharpTurnRight();
				int code;
				
				if(counter>=4&&counter<=7){
					stop();
					_delay(500000);
					code++;
				}
				

				if (counter==6 || counter == 5)
				{
					//flashRSL(code);
					armDown();
					_delay(300000);
					armUp();
				}
			
		}
		if ((rightSensor >= RIGHT_THRESHOLD) && (leftSensor < LEFT_THRESHOLD)) // touching line on right
		{
			RC0 = 1;
			right();
		}
		else if ((leftSensor >= LEFT_THRESHOLD) && (rightSensor < RIGHT_THRESHOLD)) // touching line on left
		{
			RB7 = 1;
			left();
		}
		else if ((rightSensor < RIGHT_THRESHOLD) && (leftSensor < LEFT_THRESHOLD))
		{
			forward();
			RB7 = 0;
			RC0 = 0;
		}
		
	} // end of infinite loop
} // end of main method
		
// INITIALIZE HARDWARE //

void init_hardware(void)
{
	// set analogue pins as analogue(1) or digital(0) 
	
	ANSEL  = 0b00000000;         // AN7 to AN0
	ANSELH = 0b00001010;	     //  n/c , n/c, n/c, n/c, AN11, AN10,AN9, AN8
	
	// ADC //
	
	ADCON0 = 0b00101101;
	
  	// set pins as Input(1) or Output(0)
       
	TRISA = 0b00000001;	
	TRISB = 0b00100000;
	TRISC = 0b10000000;
	
  	//initalize the ports
  	 
	PORTA = 0b00000000;
	PORTB = 0b00000000;
	PORTC = 0b00000000;	
	
	// DOWNLOAD INDICATION //
	
	flashRSL(3);
	
}

// FORWARD //

void forward(void)
{
	RC2 = 1;
	RC1 = 0;
	RC4 = 1;
	RC5 = 0;
}

// BACKWARD //

void backward(void)
{
	RC2 = 0;
	RC1 = 1;
	RC4 = 0;
	RC5 = 1;
}

// LEFT //

void left(void)
{
	RC2 = 1;
	RC1 = 0;
	RC4 = 1;
	RC5 = 1;
}

// RIGHT //

void right(void)
{
	RC2 = 1;
	RC1 = 1;
	RC4 = 1;
	RC5 = 0;
}

// LEFT SPIN //

void spinLeft(void)
{
	RC2 = 1;
	RC1 = 0;
	RC4 = 0;
	RC5 = 1;
}

// RIGHT SPIN //

void spinRight(void)
{
	RC2 = 0;
	RC1 = 1;
	RC4 = 1;
	RC5 = 0;
}
// STOP //

void stop(void)
{
	RC2 = 1;
	RC1 = 1;
	RC4 = 1;
	RC5 = 1;
}	

// RUN ADC //

void runADC(void)
{
	GO_DONE = 1;
	while (GO_DONE == 1) {}
}	

//Sharp right turn//

void sharpTurnRight(void)
{
	forward();
	_delay(300000);
	stop();
	ADCON0bits.CHS = 0b1001;
	spinRight();

/*	do
	{
		readSensors();
	
	} while(rightSensor <=RIGHT_THRESHOLD);*/
	
	do
	{
		readSensors();
	
	} while(leftSensor <= LEFT_THRESHOLD);
	
	spinLeft();
	_delay(100000);
	stop();
}

// FLASH ROBOT SIGNAL LIGHT //

void flashRSL(int count)
{
	for (int i = 0; i < count; i++)
	{
		RA5 = 1;
		_delay(500000);
		RA5 = 0;
		_delay(500000);
	}
}

// READ LEFT AND RIGHT SENSORS //

 void readSensors(void)
 {
	ADCON0bits.CHS = 0b1001;
	runADC();
	leftSensor = ADRESH;
	
	ADCON0bits.CHS = 0b1011;
	runADC();
	rightSensor = ADRESH;
}

// ARM DOWN //

void armDown(void)
{
	RA4 = 1;
	RC3 = 0;
	
	_delay(400000);
	
	RA4 = 1;
	RC3 = 1;
}

// ARM UP //

void armUp(void)
{
	RA4 = 0;
	RC3 = 1;
	
	_delay(400000);
	
	RA4 = 1;
	RC3 = 1;
}
void sharpTurnLeft(void)
{
	forward();
	_delay(500000);
	stop();
	spinLeft();
	int num=0;
	do
	{
		readSensors();
		if(rightSensor<=RIGHT_THRESHOLD)
			num++;
	} while(rightSensor <=RIGHT_THRESHOLD/*&&num==2&&counter!=2*/);
	
	
	spinRight();
	_delay(100000);
	stop();
}