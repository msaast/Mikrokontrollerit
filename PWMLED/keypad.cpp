#include "keypad.h"
#include <Arduino.h>

int pressedKey = -1;
int pressedKeyLast = -2;


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

//Odotetaan, ett� n�pp�imist� painetaan.
void waitInput()
{
	//Sijotetaan globaliin "pressedKey"-muuttajaan sy�te n�pp�imit�lt�.
	//Looppi py�rii, kun n�p�imist�n luentafunktio palauttaa ep�onnistumisen (n�pp�imist�� ei ole painettu).
	while ((pressedKey = readKeyPress()) == KEYPADFAILURE) {}
	//Kun funktio palauttaa jotain muuta kuin ep�onnistumisen, looppi loppuu ja p��st��n eteen p�in.
	//"pressedKey"-muuttaja sis�lt�� painetun napin.
}