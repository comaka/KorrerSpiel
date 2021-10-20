/*
 * main.c
 *
 * Created: 9/22/2021 10:41:36 AM
 *  Author: Colin
 */ 

#include <xc.h>
#include <avr/io.h>

#define F_CPU 1000000UL

#include <util/delay.h>

// MACROS FOR EASY PIN HANDLING FOR ATMEL GCC-AVR
//these macros are used indirectly by other macros , mainly for string concatenation
#define _SET(type,name,bit)          type ## name  |= _BV(bit)
#define _CLEAR(type,name,bit)        type ## name  &= ~ _BV(bit)
#define _TOGGLE(type,name,bit)       type ## name  ^= _BV(bit)
#define _GET(type,name,bit)          ((type ## name >> bit) &  1)
#define _PUT(type,name,bit,value)    type ## name = ( type ## name & ( ~ _BV(bit)) ) | ( ( 1 & (unsigned char)value ) << bit )

//these macros are used by end user
#define OUTPUT(pin)         _SET(DDR,pin)
#define INPUT(pin)          _CLEAR(DDR,pin)
#define HIGH(pin)           _SET(PORT,pin)
#define LOW(pin)            _CLEAR(PORT,pin)
#define TOGGLE(pin)         _TOGGLE(PORT,pin)
#define READ(pin)           _GET(PIN,pin)

#define BTNIN PC0

#define LH1 D,0
#define	LL1 D,1

#define LH2 D,2
#define	LL2 D,3

#define LH3 D,4
#define	LL3 B,6

#define	LH4 B,7
#define	LL4 D,5

#define	LH5 D,6
#define	LL5 D,7

#define	LH6 B,0
#define	LL6 B,1

#define	LH7 B,2
#define	LL7 C,1

#define	LH8 C,2
#define	LL8 C,3

#define	LH9 C,4
#define	LL9 C,5


const unsigned char linepattern[8][3] ={
	{
		0,1,2
	},
	{
		3,4,5
	},
	{
		6,7,8
	},
	{
		0,3,6
	},
	{
		1,4,7
	},
	{
		2,5,8
	},
	{
		0,4,8
	},
	{
		2,4,6
	},
};

uint8_t convertAnalogToNumber(uint16_t voltage){
	if(voltage>=540 && voltage<=580) return 0;
	else if(voltage>=650 && voltage<=680) return 1;
	else if(voltage>=820 && voltage<=860) return 2;
	else if(voltage>=370 && voltage<=410) return 3;
	else if(voltage>=420 && voltage<=450) return 4;
	else if(voltage>=490 && voltage<=520) return 5;
	else if(voltage>=620 && voltage<=645) return 6;
	else if(voltage>=750 && voltage<=770) return 7;
	else if(voltage>=950 && voltage<=1024) return 8;
	return 10;
}

uint8_t readButtons(void)
{
	//ADMUX = (ADMUX & 0xF8)|BTNIN;
	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC));
	return (convertAnalogToNumber(ADCW));
}

unsigned int readADC(void)
{
	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC));
	return (ADCW);
}

void initADC(void){
	ADCSRA |= (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1);
	ADMUX |= (1<<REFS0);
}


void setLedColor(uint8_t led, uint8_t color){
	switch(led){
		case 0:
		if(color == 2){
			HIGH(LH1);
			LOW(LL1);
		}
		else if(color == 1){
			LOW(LH1);
			HIGH(LL1);
		}
		else{
			LOW(LH1);
			LOW(LL1);
		}
		break;
		
		case 1:
		if(color == 2){
			HIGH(LH2);
			LOW(LL2);
		}
		else if(color == 1){
			LOW(LH2);
			HIGH(LL2);
		}
		else{
			LOW(LH2);
			LOW(LL2);
		}
		break;
		
		case 2:
		if(color == 2){
			HIGH(LH3);
			LOW(LL3);
		}
		else if(color == 1){
			LOW(LH3);
			HIGH(LL3);
		}
		else{
			LOW(LH3);
			LOW(LL3);
		}
		break;
		
		case 3:
		if(color == 2){
			HIGH(LH4);
			LOW(LL4);
		}
		else if(color == 1){
			LOW(LH4);
			HIGH(LL4);
		}
		else{
			LOW(LH4);
			LOW(LL4);
		}
		break;
		
		case 4:
		if(color == 2){
			HIGH(LH5);
			LOW(LL5);
		}
		else if(color == 1){
			LOW(LH5);
			HIGH(LL5);
		}
		else{
			LOW(LH5);
			LOW(LL5);
		}
		break;
		
		case 5:
		if(color == 2){
			HIGH(LH6);
			LOW(LL6);
		}
		else if(color == 1){
			LOW(LH6);
			HIGH(LL6);
		}
		else{
			LOW(LH6);
			LOW(LL6);
		}
		break;
		
		case 6:
		if(color == 2){
			HIGH(LH7);
			LOW(LL7);
		}
		else if(color == 1){
			LOW(LH7);
			HIGH(LL7);
		}
		else{
			LOW(LH7);
			LOW(LL7);
		}
		break;
		
		case 7:
		if(color == 2){
			HIGH(LH8);
			LOW(LL8);
		}
		else if(color == 1){
			LOW(LH8);
			HIGH(LL8);
		}
		else{
			LOW(LH8);
			LOW(LL8);
		}
		break;
		
		case 8:
		if(color == 2){
			HIGH(LH9);
			LOW(LL9);
		}
		else if(color == 1){
			LOW(LH9);
			HIGH(LL9);
		}
		else{
			LOW(LH9);
			LOW(LL9);
		}
		break;
	}
}

void setDisplay(uint8_t schema[]){
	uint8_t i;
	for(i = 0; i<9;i++){
		setLedColor(i, schema[i]);
	}
}

void winAnimation(uint8_t schema[], uint8_t winningPattern, uint8_t winningPlayer){
	for(unsigned char i = 0; i<9;i++){
		schema[i] = 0;
	}
	schema[linepattern[winningPattern][0]] = winningPlayer;
	schema[linepattern[winningPattern][1]] = winningPlayer;
	schema[linepattern[winningPattern][2]] = winningPlayer;
	setDisplay(schema);
	_delay_ms(500);
	for(unsigned char i = 0;i<2;i++){
		for(unsigned char i = 0; i<9;i++){
			schema[i] = 0;
		}
		setDisplay(schema);
		_delay_ms(500);
		schema[linepattern[winningPattern][0]] = winningPlayer;
		schema[linepattern[winningPattern][1]] = winningPlayer;
		schema[linepattern[winningPattern][2]] = winningPlayer;
		setDisplay(schema);
		_delay_ms(500);
	}
	for(unsigned char i = 0; i<9;i++){
		schema[i] = 0;
	}
	setDisplay(schema);
	_delay_ms(500);
}

uint8_t findWin(uint8_t schema[]){
	uint8_t i;
	for(i=0;i<9;i++){
		if(((schema[linepattern[i][0]]&&schema[linepattern[i][1]])&&(schema[linepattern[i][1]]&&schema[linepattern[i][2]])){
			winAnimation(schema, i, schema[linepattern[i][0]]);
			return 1;
		}
	}
	return 0;
}


void drawAnimation(uint8_t schema[]){
	uint8_t i;
	for(i = 1; i <= 9; i++){
		schema[i-1] = 0;
		if(i%3 == 0){
			setDisplay(schema);
			_delay_ms(500);
		}
	}
}


int main(void)
{
	uint8_t ledschema[9];
	for(unsigned char i = 0;i<9;i++){
		ledschema[i] = 0;
	}
	initADC();
	uint8_t lastButton = 10;
	uint8_t turnCounter = 1;
	uint8_t prevButton = 10;
	DDRB = 0xFF;
	DDRC = 0xFE;
	DDRD = 0xFF;
	while(1)
	{
		while(readADC()<200)_delay_ms(5);
		_delay_ms(1);
		prevButton = lastButton;
		lastButton = readButtons();
		if(prevButton == lastButton) continue;
		
		if(ledschema[lastButton] == 0){
			if((turnCounter%2) == 1){
				ledschema[lastButton] = 1;
			}
			else{
				ledschema[lastButton] = 2;
			}
			
			setDisplay(ledschema);
			/*
			if(findWin(ledschema) == 1){
				turnCounter = 0;
				lastButton = 0;
				for(unsigned char i = 0;i<9;i++){
					ledschema[i] = 0;
				}
			}*/
			
			if(turnCounter >= 9){
				drawAnimation(ledschema);
				turnCounter = 0;
				lastButton = 0;
			}
		}
		turnCounter++;
		_delay_ms(500);
	}
}



/*
void EEPROM_write(unsigned int uiAddress, unsigned char ucData)
{
	while(EECR & (1<<EEWE));
	EEAR = uiAddress;
	EEDR = ucData;
	EECR |= (1<<EEMWE);
	EECR |= (1<<EEWE);
}

union u_type
{
	unsigned int IntVar;
	unsigned char Bytes[2];
}
temp;

int main(void){
	initADC();
	DDRD = 0xFF;
	DDRC = 0x00;
	PORTC = 0x00;
	for(unsigned char i = 0; i<27;i++){
		HIGH(LL1);
		_delay_ms(2000);
		//while(~(PINC&(1<<PC5)));
		temp.IntVar = readButtons();
		EEPROM_write(i,temp.Bytes[1]);
		i++;
		EEPROM_write(i,temp.Bytes[0]);
		i++;
		EEPROM_write(i,0);
		LOW(LL1);
		_delay_ms(2000);
	}
}

*/