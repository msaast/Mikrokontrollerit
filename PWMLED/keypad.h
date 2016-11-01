#include <Arduino.h>

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

extern int pressedKey;
extern int pressedKeyLast;

int readKeyHit();

//Tämä funktio palauttaa näppäimen arvon, kun näppäin päästetään ylös.
int readKeyPress();
//Odotetaan, että näppäimistö painetaan.
void waitInput();