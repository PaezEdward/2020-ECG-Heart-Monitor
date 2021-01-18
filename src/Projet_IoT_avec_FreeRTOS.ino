/*
   Projet
   ECG_Heart_Monitor_IoT
   Auteur : PAEZ Edward
   Ecole : Sup Galilée
   www.sup-galilee.univ-paris13.fr
   Instrumentation ING2 @2021
*/

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneWire.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#define BLYNK_PRINT Serial

// ---------- Initialisation des variables ---------------------

int BPM_G, Ir_G;
float temp_G;

// Variables propres au Server Blynk

char auth[] = "bKi6bWBeta-YApVQ0IjBkfyFyOqDUg5s"; // Token in the Blynk App
char ssid[] = "ZTE_238FA1"; // WiFi credentials.
char pass[] = "97901829"; // Set password to "" for open networks.


// Variables propres au DS18B20
const int DS18B20_PIN = 5;
const int DS18B20_ID = 0x28;
// Déclaration de l'objet ds
OneWire ds(DS18B20_PIN); // on pin DS18B20_PIN (a 4.7K resistor is necessary)

// Variables propres au Sensor Pulse
const int ledPin = 2;
const int PulseSensor = 34;
const int delayMsec = 60; // 100msec per sample

int cont = 0;
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
int Signal = 0;
int Threshold = 2700;           //

// Variables propres au OLED 128x64
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// 'Sticker heart-128-128', 128x64px
static const uint8_t  PROGMEM Heart_PAEZ[] = {
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xe0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff,
  0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff,
  0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
  0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff,
  0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff,
  0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff,
  0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff,
  0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff,
  0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
  0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f,
  0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f,
  0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f,
  0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f,
  0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f,
  0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f,
  0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f,
  0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f,
  0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f,
  0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f,
  0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f,
  0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f,
  0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f,
  0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f,
  0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f,
  0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
  0xff, 0x80, 0x00, 0x00, 0x07, 0x00, 0x38, 0x1c, 0x70, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff,
  0xff, 0xc0, 0x00, 0x00, 0x1f, 0x00, 0x78, 0x1e, 0x78, 0x77, 0x80, 0x00, 0x00, 0x00, 0x03, 0xff,
  0xff, 0xe0, 0x00, 0x00, 0x77, 0x80, 0x70, 0x1e, 0xfc, 0xe1, 0xc0, 0x00, 0x00, 0x00, 0x07, 0xff,
  0xff, 0xf0, 0x00, 0x00, 0xe3, 0x87, 0x70, 0x0e, 0xff, 0x80, 0xf0, 0x1f, 0xc0, 0x00, 0x0f, 0xff,
  0xff, 0xfc, 0x00, 0x03, 0x83, 0x9f, 0xf0, 0x0e, 0xef, 0x00, 0x39, 0xc0, 0x78, 0x00, 0x3f, 0xff,
  0xff, 0xfe, 0x00, 0x00, 0x03, 0xff, 0xe0, 0x0e, 0xe0, 0x00, 0x1c, 0x00, 0x00, 0x00, 0xff, 0xff,
  0xff, 0xff, 0x80, 0x00, 0x01, 0xe3, 0xe0, 0x0e, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff,
  0xff, 0xff, 0xc0, 0x00, 0x01, 0xc3, 0xe0, 0x0f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff,
  0xff, 0xff, 0xf0, 0x00, 0x00, 0x03, 0xe0, 0x07, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff,
  0xff, 0xff, 0xfc, 0x00, 0x00, 0x01, 0xc0, 0x07, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff,
  0xff, 0xff, 0xfe, 0x00, 0x00, 0x01, 0x80, 0x07, 0xe0, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x03, 0xc0, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xec, 0xf8, 0x1e, 0x18, 0x10, 0x1f, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0x81, 0x78, 0x8d, 0x18, 0x3c, 0x3f, 0xe0, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0x8a, 0xf0, 0x38, 0x31, 0x70, 0xff, 0xfc, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xc1, 0xf1, 0xe6, 0x20, 0x20, 0x3f, 0xff, 0xc0, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

// define 3 tasks
void TaskOLED(void *pvParameters);
void TaskgetTemperatureDS18b20(void *pvParameters);
void TaskgetBPM(void *pvParameters);

// the setup function runs once when you press reset or power the board
void setup()
{
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  Serial.println("Initialisation du programme");
  Blynk.begin(auth, ssid, pass);
  pinMode (ledPin, OUTPUT);
  digitalWrite(ledPin, 0);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  vTaskDelay(2000); // Pause for aprox 2 seconds

  // Clear the buffer.
  display.clearDisplay();

  // Draw bitmap on the screen
  display.drawBitmap(0, 0, Heart_PAEZ, 128, 64, 1);
  display.display();
  vTaskDelay(2000); // Pause for aprox 2 seconds
  display.clearDisplay();

  // Now set up 3 tasks to run independently.
  xTaskCreatePinnedToCore(
    TaskOLED, "TaskOLED", 1000 // Stack size
    ,
    NULL, 1 // Priority
    ,
    NULL, ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
    TaskgetTemperatureDS18b20, "TaskgetTemperatureDS18b20" // A name just for humans
    ,
    65536 // This stack size can be checked & adjusted by reading the Stack Highwater
    ,
    NULL, 2 // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,
    NULL, ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
    TaskgetBPM, "TaskgetBPM", 65536 // Stack size
    ,
    NULL, 3 // Priority
    ,
    NULL, ARDUINO_RUNNING_CORE);

  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop()
{
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/
// Pour afficher la température du capteur et les BPM dans l’OLED
/*--------------------------------------------------*/
void TaskOLED(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  for (;;)
  {
    display.clearDisplay();       // Pour effacer l’écran
    display.setTextSize(1);       // The fontsize
    display.setTextColor(WHITE);  // Draw white text
    display.setCursor(0, 0);      // Start at top-left corner
    display.print("Temp:");       //the text
    display.print(temp_G);        // Pour afficher la valeur
    display.print(" 'C");         //the text
    display.setCursor(0, 50);     // Start at top-left corner
    display.print("BPM:");        //the text
    display.print(BPM_G);         // Pour afficher la valeur
    display.display();            //call to display

    vTaskDelay(1000);  // one tick delay aprox (1s) in between reads for stability
  }
}

/*--------------------------------------------------*/
// Pour lire la température du capteur
/*--------------------------------------------------*/
void TaskgetTemperatureDS18b20(void *pvParameters) // This is a task.
{
  (void)pvParameters;

  for (;;) // A Task shall never return or exit.
  {
    byte i;
    byte data[12];
    byte addr[8];
    float temp = 0.0;

    //Il n'y a qu'un seul capteur, donc on charge l'unique adresse.
    ds.search(addr);
    ds.reset_search();
    vTaskDelay(1);

    // Cette fonction sert à surveiller si la transmission s'est bien passée
    if (OneWire::crc8(addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
    }

    // On vérifie que l'élément trouvé est bien un DS18B20
    if (addr[0] != DS18B20_ID) {
      Serial.println("L'équipement trouvé n'est pas un DS18B20");
    }

    // Demander au capteur de mémoriser la température et lui laisser 850ms pour le faire (voir datasheet)
    ds.reset();
    ds.select(addr);
    ds.write(0x44, 1);
    vTaskDelay(850);
    // Demander au capteur de nous envoyer la température mémorisé
    ds.reset();
    ds.select(addr);
    ds.write(0xBE);
    // Le MOT reçu du capteur fait 9 octets, on les charge donc un par un dans le tableau data[]
    for ( i = 0; i < 9; i++) {
      data[i] = ds.read();
      //Serial.print(data[i], HEX);
      //Serial.print(" ");
    }
    // Puis on converti la température *0.0625 car la température est stockée sur 12 bits)
    temp = ( (data[1] << 8) + data[0] ) * 0.0625;

    // on affiche la T°
    Serial.print("Temperature: ");
    Serial.println(temp);
    temp_G = temp;
    Blynk.run(); // Pour lancer le serveur
    Blynk.virtualWrite(V5, temp); //Pour écrier la variable temp dans la variable V5 du serveur
    vTaskDelay(1000);
  }
}
/*--------------------------------------------------*/
// Pour lire le BPM du capteur - le rythme cardiaque
/*--------------------------------------------------*/
void TaskgetBPM(void *pvParameters) // This is a task.
{
  (void)pvParameters;

  for (;;)
  {
    Signal = analogRead(PulseSensor);  //
    Serial.println(Signal);
    Blynk.run();
    Blynk.virtualWrite(V0, Signal);  //Pour écrier la variable Signal dans la variable V0 du serveur

    if (Signal > Threshold) { // Si la valeur Signal est plus grand que 2800 on a un pulse
      currentMillis = millis();
      digitalWrite(ledPin, HIGH);
      cont++;
      //Serial.println(cont);
      vTaskDelay(150);
    }
    if (Signal <= Threshold)
    {
      digitalWrite(ledPin, LOW);
    }

    if (currentMillis - previousMillis >= 30000) {
      // save the last time
      BPM_G = cont * 2;
      Serial.print("Heartrate: ");
      Serial.println(BPM_G);
      Blynk.run();
      Blynk.virtualWrite(V1, BPM_G);
      cont = 0;
      currentMillis = millis();
      previousMillis = currentMillis;
    }
  }
}
