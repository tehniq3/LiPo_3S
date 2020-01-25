// original http://nicuflorica.blogspot.com/2015/01/indicator-stare-acumulator-auto-cu-led.html

/*
basesd sketch named "Analog Input" from Examples -> 03.Analog
created by David Cuartielles, modified 30 Aug 2011, By Tom Igoe
http://arduino.cc/en/Tutorial/AnalogInput
*/

/* original circuit made by niq_ro from http://www.tehnic.go.ro
http://nicuflorica.blogspot.ro/
http://arduinotehniq.blogspot.com/
1) uC: ATtiny85 (ATtiny45) at 8MHz (internal clock)
2) common chatode RGB LED code OSTA5131A-C, see http://nicuflorica.blogspot.ro/2012/12/arduino-uno-si-un-led-multicolor-rgb.html
- an 180ohms resistor at pin D0 (phisical pin 5) in series with RED LED at GND
- an 100ohms resistor at pin D1 (phisical pin 6) in series with GREN LED at GND
- an 100ohms resistor at pin D2 (phisical pin 7) in series with BLUE LED at GND
3) a voltage divisor 1:4 made like this: +BAT --|=10k=|-|=10k=|-|=10k=|-A2|=10k=|-GND, pin A2 is phisical pin 3
- for tests can use an 10-50kohms variabile resistor put at +5V and GND, midle at A2 (phisical pin 3)
4) Vcc (phisical pin 8) at 5V
5) GND (phisixcal pin 4) at GND

program written by Nicu FLORICA (niq_ro) for Lidiano Marcassa

*/

#define sensorPin A2   // select the input pin for the potentiometer / resistor divider
int ledRPin = 0;      // select the pin for the RED LED
int ledGPin = 1;      // select the pin for the GREEN LED
int ledBPin = 2;      // select the pin for the BLUE LED

int u = 0;  // variable to store the value coming from the sensor

float k=4.99/5.; // corection voltage ("real 5V" / 5V)
// define voltage steps (treapta = 1023 * voltage_threshold /20)
int treapta1 = 624/k;       // Ubat=12,2V (100%) 
int treapta2 = 593/k;       // 95%
int treapta3 = 562/k;       // 90%
int treapta4 = 499/k;       // 80%
int treapta5 = 468/k;       // 75%
int histerezis = 3;         // for eliminate flash changes
int pauza = 500;           // brake between measurements
int pauza2 = 300;           // time in ms for red flash (on/off)


// Coomon Anode
byte stins = 1;
byte aprins = 0;

/*
// Comon Cathode
byte stins = 0;
byte aprins = 1;
*/

void setup() {
  // declare the ledPins as an OUTPUT:
  pinMode(ledRPin, OUTPUT);  
  pinMode(ledGPin, OUTPUT);
  pinMode(ledBPin, OUTPUT);
  digitalWrite(ledRPin, stins);
  digitalWrite(ledGPin, stins);
  digitalWrite(ledBPin, stins);

  pinMode(sensorPin, INPUT);  
}

void loop() {
/*
  // all leds are off
    digitalWrite(ledRPin, stins;    // turn the red led off
    digitalWrite(ledGPin, stins);    // turn the green led off
    digitalWrite(ledBPin, stins);    // turn the blue led off
*/ 
  
  // read the value from the sensor:
  u = analogRead(sensorPin);    

// voltage below 75% -> flash RED
if (u < treapta5)
{
  digitalWrite(ledRPin, aprins);
  digitalWrite(ledGPin, stins);
  digitalWrite(ledBPin, stins);
  delay(pauza2);
  digitalWrite(ledRPin, stins);
  digitalWrite(ledGPin, stins);
  digitalWrite(ledBPin, stins);
  delay(pauza2);
}

// voltage over 75% & below 80% -> YeLLOW (RED + GREEN)
if (u >= treapta5 && u < treapta4-histerezis)
{ 
  digitalWrite(ledRPin, aprins);
  digitalWrite(ledGPin, aprins);
  digitalWrite(ledBPin, stins);
  delay(pauza);
}


// voltage over 80% & below 90% -> GREEN
if (u >= treapta4 && u < treapta3-histerezis)
{ 
  digitalWrite(ledRPin, stins);
  digitalWrite(ledGPin, aprins);
  digitalWrite(ledBPin, stins);
  delay(pauza);
}


// voltage over 90% & below 95% -> CYAN (BLUE + GREEN)
if (u >= treapta3 && u < treapta2-histerezis)
{ 
  digitalWrite(ledRPin, stins);
  digitalWrite(ledGPin, aprins);
  digitalWrite(ledBPin, aprins);
  delay(pauza);
}

// voltage over 95% & below 100% -> BLUE
if (u >= treapta2 && u <= treapta1-histerezis)
{ 
  digitalWrite(ledRPin, stins);
  digitalWrite(ledGPin, stins);
  digitalWrite(ledBPin, aprins);
  delay(pauza);
}

// voltage over 12,2V (> 100%) -> MAUVE (RED + BLUE)
if (u > treapta1)
{
  digitalWrite(ledRPin, aprins);
  digitalWrite(ledGPin, stins);
  digitalWrite(ledBPin, aprins);
  /*
  delay(300);
  digitalWrite(ledRPin, stins);
  digitalWrite(ledGPin, stins);
  digitalWrite(ledBPin, stins);
  */
  delay(pauza);
}
}
