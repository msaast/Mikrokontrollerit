#define DIFFERENTCHAR 18
#define D 500 //Char display time
//									0			1			2			3			4			5			6			7			8			9			.			blank
const uint8_t sevenSegBits[] = { 0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110, 0b01101101, 0b01111101, 0b01000111, 0b01111111, 0b01101111, 0b10000000, 0b00000000,
								0b01110111, 0x7C, 0x39, 0x5E, 0b01111001, 0b01110001 };
//									A		B		C		D	E			F
//const char sevenSegChars[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', ' ', 'A', 'B', 'C', 'D', 'E', 'F' };

#define SS0 0
#define SS1 1
#define SS2 2
#define SS3 3
#define SS4 4
#define SS5 5
#define SS6 6
#define SS7 7
#define SS8 8
#define SS9 9
#define SSdot 10
#define SSemt 11
#define SSA 12
#define SSB 13
#define SSC 14
#define SSD 15
#define SSE 16
#define SSF 17



//char wordChar1[] = {   '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ' ',   '8', '6', '4', '2', '0', ' ' };
uint8_t wordChar[] = {		 SS0, SS1, SS2, SS3, SS4, SS5, SS6, SS7, SS8, SS9, SSemt, SS8, SS6, SS4, SS2, SS0, SSemt };
unsigned int wordDelay[] = {  D,   D,   D,   D,   D,   D,   D,   D,   D,   D,   D,     D,   D,   D,   D,   D,   D * 2 };
unsigned long wordTimeOld = 0;
unsigned int wordIndex = 0;
bool wordStart = true;

#define d 800
//char toinen1[] =			   { '1', '3', '7', '9', 'B', 'D', 'F', 'C', 'A', '6', '4', '2', '0' };
uint8_t toinen[] =		   { SS1, SS3, SS7, SS9, SSB, SSD, SSF, SSC, SSA, SS6, SS4, SS2, SS0 };
unsigned int toinenAika[] = { d,   d,   d,   d,   d,   d,   d,   d,   d,   d,   d,   d,   d, };
unsigned long toinenVanhaAika = 0;
unsigned int toinenIndeksi = 0;
bool toinenAloitus = true;

int aika = 200;

void setup()
{
	//Display 1
	DDRD = 255;
	//Display 2
	DDRB = 0b1111;
	DDRC = 0b1111;
}

void loop()
{
	//displayCharacters(wordChar, wordDelay, sizeof(wordChar), wordTimeOld, wordIndex, wordStart, true);
	//displayCharacters(toinen, toinenAika, sizeof(toinen), toinenVanhaAika, toinenIndeksi, toinenAloitus, false);

	//sevenSeg('2', false, true);
	
	
	for (int i = 0; i < 6; i++)
	{
		if (i == 0)
		{
			PORTD = 0b0000001;
		}
		else
		{
			PORTD = PORTD << 1;
		}
		delay(aika);
	}
	for (int i = 0; i < 6; i++)
	{
		if (i == 0)
		{
			PORTD = 0b0010000;
		}
		else
		{
			PORTD = PORTD >> 1;
		}
		delay(aika);
	}


	aika = aika - 20;
	if (aika < 0)
	{
		aika = 200;
	}
	
}
//bool dislay: true = PORTD display, false = PORTB + PORTC display
int sevenSeg(uint8_t character, bool decimal, bool display)
{

	if (decimal == true)
	{
		if (character == '.')
		{
			if (display == true)
			{
				PORTD = sevenSegBits[character];
			}
			else
			{
				PORTB = sevenSegBits[character] & 0b00001111;
				PORTC = (sevenSegBits[character] & 0b11110000) >> 4;
			}
		}
		else
		{
			if (display == true)
			{
				PORTD = sevenSegBits[character] + 0b10000000;
			}
			else
			{
				PORTB = sevenSegBits[character] & 0b00001111;
				PORTC = ((sevenSegBits[character] & 0b11110000) >> 4) + 0b1000;
			}
		}
		return EXIT_SUCCESS;
	}
	else
	{
		if (display == true)
		{
			PORTD = sevenSegBits[character];
		}
		else
		{
			PORTB = sevenSegBits[character] & 0b00001111;
			PORTC = (sevenSegBits[character] & 0b11110000) >> 4;
		}

	}

	return EXIT_SUCCESS;
}

void displayCharacters(uint8_t word[], unsigned int time[], unsigned int lenght, unsigned long &timeOld, unsigned int &patternIndex, bool &start, bool display)
{
	if (patternIndex < lenght)
	{
		if (patternIndex == 0 && start == true)
		{
			sevenSeg(word[patternIndex], false, display);
			start = false;
			timeOld = millis();
		}
		else
		{
			if (millis() - timeOld > time[patternIndex])
			{
				patternIndex++;
				sevenSeg(word[patternIndex], false, display);
				timeOld = millis();
			}
		}
	}
	else
	{
		patternIndex = 0;
		start = true;
	}
}