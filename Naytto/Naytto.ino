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
const char keys[4][4] =
{ { 'A', '3', '2', '1' },
{ 'B', '6', '5', '4' },
{ 'C', '9', '8', '7' },
{ 'D', '#', '0', '*' }
};
*/

const uint8_t keys[4][4] =
{ { KA, K3, K2, K1 },
{ KB, K6, K5, K4 },
{ KC, K9, K8, K7 },
{ KD, Khastag, K0, Kstar }
};

const char chars[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', '*', '#' };

//nkn = nestekiden�ytt�
LiquidCrystal nkn(RSlcd, EnableLcd, data0Lcd, data1Lcd, data2Lcd, data3Lcd); 

int pressedKey = -1;
int pressedKeyLast = -2;

void setup()
{
	nkn.begin(columnsLcd, rowsLcd);

	//C-portti input yl�sveto
	//Sarakkeet
	DDRC = 0b000000000;
	PORTC = 0b00001111;

	//B-portti output
	//Rivit
	DDRB = 0b00001111;
	PORTB = 0b00000000;
	
}

void loop()
{
	aPlusB();

	//setValue();
	//printTo50();
}


int readKeyHit()
{
	//T�m� funktio tutkii mik� n�pp�in on painettuna.
	//Jos haluaa lukea n�pp�imist��, t�m�n funkiton on py�ritt�v� jatkuvasti.
	//Mikrokontrolleri testaa n�pp�in n�pp�imelt� onko se painettuna.

	//Alustetaan maskit
	uint8_t mask1 = 0b1110; //Rivit
	uint8_t mask2 = 0b1110; //Sarakkeet

	//Ulompi for-looppi k�y l�pi n�pp�imist�n rivit testateekseen onko jotain sen saraketta painettu.
	for (uint8_t i = 0; i < 4; i++)
	{
		//Asetetaan B-portti rivi-maskin mukaiseksi.
		//1. kierros 00001110, 2. 00001101, 3. 00001011, 4. 00000111
		//0 = maa (GND)
		PORTB = mask1 & 0b1111; 
		//Kun joku rivi on astuettu maahan, voidaan testata mit� sen rivin saraketta on painettu.
		//Kaikki rivit k�yd��n erikseen l�pi.

		mask2 = 0b1110; //Asetetaan sarakkeiden maski alku asentoon.

		//Sisempi for-looppi tarkistaa mit� saraketta on painettu.
		for (uint8_t j = 0; j < 4; j++)
		{
			//Luetaan sarakkeiden input (C-porttia, nelj� v�hiten merkitsev�� bitti�).
			//Jos jotain t�ll� hetkell� testattavan rivin saraketta on painettu.
			if (0b1111 != (PINC & 0b1111))
			{
				delay(keyDelay); //Odotetaan v�h�n aikaa, ett� n�pp�in asettuu.

				// Luetaan n�pp�imist� uudelleen ja tarkistetaan sarake-maskilla, mit� rivin saraketta on painettu.
				if ((mask2 & 0b1111) == (PINC & 0b1111))
				{
					//Palautetaan kaksiulotteisestataulukosta painetun n�pp�imen arvo.
					//Indeksoidaan taulukkukko looppien laskureiden avulla. i on rivi ja j on sarake.
					//Esim.: Painetaan nappia 5. Se on toisella rivill� (ylh��lt� alas) ja kolmannessa sarakkeessa (vasmmalta oikealle).
					//Joten ulompi for-looppi pit�� olla toisella kierroksella (i = 1) ja sisempi kolmannella (j = 2).
					//Palautetaan keys[1][2], mink� arvo on 5. Loopit j�� kesken, koska l�ydettiin osuma.
					return keys[i][j];
				}
			}
			//Muokataan sarake-maskia.
			mask2 = mask2 << 1; //1110 -> 1100
			mask2++; //1100 -> 1101
			//Loopin alkuu -> Testataan seuraava sarake.
		}
		//Mukataan rivi-maskia.
		mask1 = mask1 << 1; //1110 -> 1100
		mask1++; //1100 -> 1101
		//Loopin alkuu -> Testataan seuraava rivi.
	}
	
	//Ei osumia, n�pp�imist�� ei ole painettu. Palatetaan ep�onnistuminen.
	return KEYPADFAILURE;
}

//T�m� funktio palauttaa n�pp�imen arvon, kun n�pp�in p��stet��n yl�s.
int readKeyPress()
{
	int temp = readKeyHit(); //Luetaan mik� n�pp�in on painettuna v�liaikais muuttujaan.
	//Jos joku n�ppain on painettuna.
	if (temp >= 0)
	{
		//Py�ritet��n looppia niin kauvan kuin sama nappi on painettuna.
		while (temp == readKeyHit()) {}
		//Nappi p��stettiin yl�s.
		delay(keyDelay); //Odotetaan, ett� nappi asettuu.

		//Tarkistetaan uudelleen, ett� nappi on ylh��ll�.
		if (readKeyHit() == KEYPADFAILURE)
		{
			//Nappi p��stetty yl�s onnistuneesti. Palautetaan pohjassa olleen napin arvo.
			return temp;
		}
	}
	//N�pp�imist�� ei ole painettu. Palautetaan ep�onnistuminen.
	return KEYPADFAILURE;
}

void printTo50()
{
	const uint8_t iMax = 50;
	for (uint8_t i = 0; i < iMax; i++)
	{
		nkn.clear();

		nkn.setCursor(0, 0);
		nkn.print(i);
		nkn.setCursor(0, 1);
		nkn.print(sqrt(i), 3);

		nkn.setCursor(columnsLcd - 2, 0);
		nkn.print(iMax - i);
		nkn.setCursor(columnsLcd - 3, 1);
		nkn.print(-(iMax - i));
		
		delay(400);
	}
}

void displayKeyLcd()
{
	waitInput();

	if (pressedKey != pressedKeyLast )
	{
		nkn.clear();
		nkn.setCursor(columnsLcd / 2 - 1, 0);
		nkn.print(chars[pressedKey]);
	}
}

void aPlusB()
{
	#define firstChar 4
	#define row 0
	nkn.clear();
	nkn.setCursor(firstChar, row);
	const char line[] = { " + =" };
	nkn.print(line);
	int temp0, temp1;

	waitInput();

	temp0 = pressedKey;
	nkn.setCursor(firstChar, row);
	nkn.print(chars[temp0]);

	waitInput();

	temp1 = pressedKey;

	nkn.setCursor(firstChar + 2, row);
	nkn.print(chars[temp1]);
	nkn.setCursor(firstChar + sizeof(line) - 1, row);

	nkn.print(temp0 + temp1);

	waitInput();
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

		waitInput();

		switch (chars[pressedKey])
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

//Odotetaan, ett� n�pp�imist� painetaan.
void waitInput()
{
	//Sijotetaan globaliin "pressedKey"-muuttajaan sy�te n�pp�imit�lt�.
	//Looppi py�rii, kun n�p�imist�n luentafunktio palauttaa ep�onnistumisen (n�pp�imist�� ei ole painettu).
	while ((pressedKey = readKeyPress()) == KEYPADFAILURE) {}
	//Kun funktio palauttaa jotain muuta kuin ep�onnistumisen, looppi loppuu ja p��st��n eteen p�in.
	//"pressedKey"-muuttaja sis�lt�� painetun napin.
}