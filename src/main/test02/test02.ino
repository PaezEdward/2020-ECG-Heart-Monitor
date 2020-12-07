#include <Adafruit_SSD1306.h>
#define OLED_Address 0x3C // 0x3C device address of I2C OLED. Few other OLED has 0x3D
Adafruit_SSD1306 oled(128, 64); // create our screen object setting resolution to 128x64
 
int a=0;
int lasta=0;
int lastb=0;
int LastTime=0;
int ThisTime;
bool BPMTiming=false;
bool BeatComplete=false;
int BPM=0;
#define UpperThreshold 560
#define LowerThreshold 530

int rawValue;


bool heartbeatDetected(int IRSensorPin, int delay)
{
 static int maxValue = 0;
 static bool isPeak = false;


 bool result = false;

 rawValue = analogRead(4);
 // Hier wird der aktuelle Spannungswert am Fototransistor ausgelesen und in der rawValueVariable zwischengespeichert
 rawValue *= (1000/delay);

 // Sollte der aktuelle Wert vom letzten maximalen Wert zu weit abweichen
 // (z.B. da der Finger neu aufgesetzt oder weggenommen wurde)
 // So wird der MaxValue resetiert, um eine neue Basis zu erhalten.
 if (rawValue * 4L < maxValue) { maxValue = rawValue * 0.8; } // Detect new peak
 if (rawValue > maxValue - (1000/delay)) {
 // Hier wird der eigentliche Peak detektiert. Sollte ein neuer RawValue groeßer sein
 // als der letzte maximale Wert, so wird das als Spitze der aufgezeichnten Daten erkannt.
 if (rawValue > maxValue) {
 maxValue = rawValue;
 }
 // Zum erkannten Peak soll nur ein Herzschlag zugewiesen werden
 if (isPeak == false) {
 result = true;
 }
 isPeak = true;
 } else if (rawValue < maxValue - (3000/delay)) {
 isPeak = false;
 // Hierbei wird der maximale Wert bei jeden Durchlauf
 // etwas wieder herabgesetzt. Dies hat den Grund, dass
 // nicht nur der Wert sonst immer stabil bei jedem Schlag
 // gleich oder kleiner werden wuerde, sondern auch,
 // falls der Finger sich minimal bewegen sollte und somit
 // das Signal generell schwaecher werden wuerde.
 maxValue-=(1000/delay);
}
 return result;
}



int analogPin=4;

void setup()
{

  oled.begin(SSD1306_SWITCHCAPVCC, OLED_Address);
oled.clearDisplay();
oled.setTextSize(2);

}

const int delayMsec = 60; // 100msec per sample

// Das Hauptprogramm hat zwei Aufgaben:
// - Wird ein Herzschlag erkannt, so blinkt die LED kurz aufgesetzt
// - Der Puls wird errechnet und auf der serriellen Ausgabe ausgegeben.

void loop()
{
 static int beatMsec = 0;
 int heartRateBPM = 0;
 if (heartbeatDetected(analogPin, delayMsec)) {
 heartRateBPM = 60000 / beatMsec;
 // LED-Ausgabe bei Herzschlag

 // Serielle Datenausgabe
 oled.setCursor(0,50);
oled.print("BPM:");
oled.print(heartRateBPM);
 
oled.display();

 beatMsec = 0;
 } else {

 }
 delay(delayMsec);
 beatMsec += delayMsec;
}
