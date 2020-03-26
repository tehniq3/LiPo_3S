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
LiPo 2S version
*/

/*  // ATtiny85/45
#define sensorPin A2   // select the input pin for the potentiometer / resistor divider
int ledRPin = 0;      // select the pin for the RED LED
int ledGPin = 1;      // select the pin for the GREEN LED
int ledBPin = 2;      // select the pin for the BLUE LED
*/

  // Arduino
#define sensorPin A2   // select the input pin for the potentiometer / resistor divider
int ledRPin = 8;      // select the pin for the RED LED
int ledGPin = 9;      // select the pin for the GREEN LED
int ledBPin = 10;      // select the pin for the BLUE LED


int u = 0;  // variable to store the value coming from the sensor
int um = 0;  // sum
int numar = 0; // number

float u0, u1, ux;

float k0 = 1.016;  // correction for resistive divider
float k=(float)4.88/5.; // corection voltage ("real 5V" / 5V)
// define voltage steps (treapta = 1023 * voltage_threshold /20)
/*
int treapta1 = 624;       // Ubat=12,2V (100%) 
int treapta2 = 593;       // 95%
int treapta3 = 562;       // 90%
int treapta4 = 499;       // 80%
int treapta5 = 468;       // 75%
*/
//int treapta1 = 624;       // Ubat=12,2V (100%) 
int treapta1 = 416;       // Ubat=8,2V (100%) 
int treapta2 = 0.95*treapta1;       // 95%
int treapta3 = 0.9*treapta1;       // 90%
int treapta4 = 0.8*treapta1;      // 80%
int treapta5 = 0.75*treapta1;        // 75%

int histerezis = 2;         // for eliminate flash changes
int pauza = 500;           // brake between measurements
int pauza2 = 300;           // time in ms for red flash (on/off)
int citiri = 50;          // number of measurements
/*
// Coomon Anode
byte stins = 1;
byte aprins = 0;
*/

// Comon Cathode
byte stins = 0;
byte aprins = 1;


void setup() {
  Serial.begin(9600);
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
    digitalWrite(ledRPin, stins);    // turn the red led off
    digitalWrite(ledGPin, stins);    // turn the green led off
    digitalWrite(ledBPin, stins);    // turn the blue led off
*/
  
  // read the value from the sensor:
  um = 0;
  for (int i = 0; i < citiri; i++) 
  {
  u = (float)(k0 * k *analogRead(sensorPin));  
  delay(10);
  um = um + u;
  }
  ux = (float)(um/citiri);
  u0 = (float)(ux * 5. / 1024); // voltage on uC pin
  u1 = (float)(4. *  u0 );
  Serial.print("step = ");
  Serial.print(u);
  Serial.print("/1024 = ");
  Serial.print(u1);
  Serial.println("V");  

   
// voltage below 75% -> flash RED
if (u < treapta5)
{
  digitalWrite(ledRPin, aprins);
  digitalWrite(ledGPin, stins);
  digitalWrite(ledBPin, stins);
  Serial.println("<75% ===> R");
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
  Serial.println("75..80% ===> R+G");
  delay(pauza);
}


// voltage over 80% & below 90% -> GREEN
if (u >= treapta4 && u < treapta3-histerezis)
{ 
  digitalWrite(ledRPin, stins);
  digitalWrite(ledGPin, aprins);
  digitalWrite(ledBPin, stins);
  Serial.println("80..90%  ===> G");
  delay(pauza);
}


// voltage over 90% & below 95% -> CYAN (BLUE + GREEN)
if (u >= treapta3 && u < treapta2-histerezis)
{ 
  digitalWrite(ledRPin, stins);
  digitalWrite(ledGPin, aprins);
  digitalWrite(ledBPin, aprins);
  Serial.println("90..95%  ===> B+G");
  delay(pauza);
}

// voltage over 95% & below 100% -> BLUE
if (u >= treapta2 && u <= treapta1-histerezis)
{ 
  digitalWrite(ledRPin, stins);
  digitalWrite(ledGPin, stins);
  digitalWrite(ledBPin, aprins);
  Serial.println("95..100%  ===> B");
  delay(pauza);
}

// voltage over 12,2V (> 100%) -> MAUVE (RED + BLUE)
if (u > treapta1)
{
  digitalWrite(ledRPin, aprins);
  digitalWrite(ledGPin, stins);
  digitalWrite(ledBPin, aprins);
  Serial.println(">100%  ===> R+B");
  /*
  delay(300);
  digitalWrite(ledRPin, stins);
  digitalWrite(ledGPin, stins);
  digitalWrite(ledBPin, stins);
  */
  delay(pauza);
}
}
