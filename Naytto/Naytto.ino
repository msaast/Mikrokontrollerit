#include <LiquidCrystal.h>

#define K0 0
#define K1 1
#define K2 2
#define K3 3
#define K4 4
#define K5 5
#define K6 6
#define K7 7
#define K8 8
#define K9 9
#define KA 10
#define KB 11
#define KC 12
#define KD 13
#define KE 14
#define KF 15
#define Kstar 16
#define Khastag 17

#define keyDelay 5 //ms

#define RSlcd 1
#define EnableLcd 0
#define data0Lcd 5
#define data1Lcd 4
#define data2Lcd 3
#define data3Lcd 2

#define rowsLcd 2
#define columnsLcd 16

#define KEYPADFAILURE -100

/*
const uint8_t keys[4][4] =
{ { KA, K3, K2, K1 },
{ KB, K6, K5, K4 },
{ KC, K9, K8, K7 },
{ KD, Khastag, K0, Kstar }
};
*/

const char keys[4][4] =
{ { 'A', '3', '2', '1' },
{ 'B', '6', '5', '4' },
{ 'C', '9', '8', '7' },
{ 'D', '#', '0', '*' }
};

//nkn = nestekidenäyttö
LiquidCrystal nkn(RSlcd, EnableLcd, data0Lcd, data1Lcd, data2Lcd, data3Lcd); 

uint8_t i50 = 0;
const uint8_t maxi50 = 50;

const char chars[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', '*', '#' };

int pressedKey = 17;
int pressedKeyLast = 0;

void setup()
{

	nkn.begin(columnsLcd, rowsLcd);

	//C-portti input ylös
	DDRC = 0b000000000;
	PORTC = 0b00001111;

	//B-portti output alas
	DDRB = 0b00001111;
	PORTB = 0b00000000;
	//Serial.begin(57600);
}

void loop()
{
	aPlusB();

	
	//printTo50();
}

int readKeyHit()
{
	uint8_t mask1 = 0b1110;
	uint8_t mask2 = 0b1110;

	for (uint8_t i = 0; i < 4; i++)
	{
		PORTB = mask1 & 0b1111;
		mask2 = 0b1110;
		for (uint8_t j = 0; j < 4; j++)
		{
			if (0b1111 != (PINC & 0b1111))
			{
				delay(keyDelay);
				if ((mask2 & 0b1111) == (PINC & 0b1111))
				{
					return keys[i][j];
				}
			}
			mask2 = mask2 << 1;
			mask2++;
		}
		mask1 = mask1 << 1;
		mask1++;
	}
	return -100;
}

int readKeyPress()
{
	int temp = readKeyHit();
	if (temp >= 0)
	{
		while (temp == readKeyHit())
		{
		}
		delay(keyDelay);
		return temp;
	}
	return -100;
}

void printTo50()
{
	nkn.clear();

	nkn.setCursor(0, 0);
	nkn.print(i50);
	nkn.setCursor(0, 1);
	nkn.print(sqrt(i50), 3);

	nkn.setCursor(columnsLcd - 2, 0);
	nkn.print(maxi50 - i50);
	nkn.setCursor(columnsLcd - 3, 1);
	nkn.print(-(maxi50 - i50));
	i50++;

	if (i50 > maxi50)
	{
		i50 = 0;
	}
	delay(400);

}

void displayKeyLcd()
{
	pressedKey = readKeyPress();

	if (pressedKey != pressedKeyLast && pressedKey != -100)
	{
		nkn.clear();
		nkn.setCursor(columnsLcd / 2 - 1, 0);
		nkn.print(char(pressedKey));
	}
}

void aPlusB()
{
	nkn.clear();
	nkn.setCursor(4, 0);
	nkn.print(" + =");
	int temp0, temp1;
	while ((pressedKey = readKeyPress()) == -100) {}

	temp0 = pressedKey;
	nkn.setCursor(4, 0);
	nkn.print(char(temp0));

	while ((pressedKey = readKeyPress()) == -100) {}

	temp1 = pressedKey;

	nkn.setCursor(6, 0);
	nkn.print(char(temp1));
	nkn.setCursor(8, 0);


	nkn.print((temp0 - '0') + (temp1 - '0'));

	while ((pressedKey = readKeyPress()) == -100) {}
}

int setValue()
{
	int value = 0;
	while (true)
	{
		nkn.clear();
		nkn.setCursor(0, 0);
		char line[] = { "Set value=" };
		nkn.print(line);
		nkn.setCursor(sizeof(line) + 1, 0);
		nkn.print(value);

		while ((pressedKey = readKeyPress()) == KEYPADFAILURE) {}

		switch (pressedKey)
		{
		case '1':
			value++;
			break;
		case '4':
			value--;
			break;
		case '3':
			value+= 10;
			break;
		case '6':
			value-= 10;
			break;
		case 'D':
			return value;
			break;
		default:
			break;
		}
	}	
}

