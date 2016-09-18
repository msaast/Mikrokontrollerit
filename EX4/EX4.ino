#define DIFFERENTCHAR 15
#define D 500 //Char display time
//													0			1			2			3			4			5			6			7			8			9			.			blank
const uint8_t sevenSegBits[DIFFERENTCHAR] = { 0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110, 0b01101101, 0b01111101, 0b01000111, 0b01111111, 0b01101111, 0b10000000, 0b00000000,
												0b01110111, 0b01111001, 0b01110001 };
//													A			E			F
const char sevenSegChars[DIFFERENTCHAR] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', ' ', 'A', 'E', 'F' };

char wordChar[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ' ', '8', '6', '4', '2', '0', ' ' };
unsigned int wordDelay[] = { D,   D,   D,   D,   D,   D,   D,   D,   D,   D,   D,   D,   D,   D,   D,   D,   D * 2 };
unsigned long wordTimeOld = 0;
unsigned int wordIndex = 0;
bool wordStart = true;

#define d 300
char toinen[] =			   { 'A', 'E', 'F', '2', '6', '.' };
unsigned int toinenAika[] = { d,   d,   d,   d,   d,   d*2};
unsigned long toinenVanhaAika = 0;
unsigned int toinenIndeksi = 0;
bool toinenAloitus = true;

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
	displayCharacters(wordChar, wordDelay, sizeof(wordChar), wordTimeOld, wordIndex, wordStart, true);
	displayCharacters(toinen, toinenAika, sizeof(toinen), toinenVanhaAika, toinenIndeksi, toinenAloitus, false);
}
//bool dislay: true = PORTD display, false = PORTB + PORTC display
int sevenSeg(char character, bool decimal, bool display)
{
	for (uint8_t i = 0; i < DIFFERENTCHAR; i++)
	{
		if (sevenSegChars[i] == character)
		{
			if (decimal == true)
			{
				if (character == '.')
				{
					if (display == true)
					{
						PORTD = sevenSegBits[i];
					}
					else
					{
						PORTB = sevenSegBits[i] & 0b00001111;
						PORTC = (sevenSegBits[i] & 0b11110000) >> 4;
					}
				}
				else
				{
					if (display == true)
					{
						PORTD = sevenSegBits[i] + 0b10000000;
					}
					else
					{
						PORTB = sevenSegBits[i] & 0b00001111;
						PORTC = ((sevenSegBits[i] & 0b11110000) >> 4) + 0b1000;
					}
				}
				return EXIT_SUCCESS;
			}
			else
			{
				if (display == true)
				{
					PORTD = sevenSegBits[i];
				}
				else
				{
					PORTB = sevenSegBits[i] & 0b00001111;
					PORTC = (sevenSegBits[i] & 0b11110000) >> 4;
				}
			}
		}
	}
	return EXIT_FAILURE;
}

void displayCharacters(char word[], unsigned int time[], unsigned int lenght, unsigned long &timeOld, unsigned int &patternIndex, bool &start, bool display)
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