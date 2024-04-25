/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID           "TMPLvUmztoOi"
#define BLYNK_TEMPLATE_NAME         "Quickstart Template"
#define BLYNK_AUTH_TOKEN            "1antnWzjJlT33QjfZtS8Jq94gKNveiQ2"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define WATER_PUMP_PIN D7
#define SOIL_MOISTURE_THRESHOLD 400

#define VIRTUAL_PIN_WATER_PUMP 0
#define VIRTUAL_PIN_WATER_PUMP_STATE 1
#define VIRTUAL_PIN_UPTIME 2
#define VIRTUAL_PIN_SOIL_MOISTURE_THRESHOLD 3
#define VIRTUAL_PIN_SOIL_MOISTURE 4

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "CHANGE-ME";
char pass[] = "CHANGE-ME";

BlynkTimer timer;
int nSoilMoistureTheshold = SOIL_MOISTURE_THRESHOLD;

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(VIRTUAL_PIN_WATER_PUMP)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();
  // Update state
  Blynk.virtualWrite(VIRTUAL_PIN_WATER_PUMP_STATE, value);  
  // set water pump state
  digitalWrite(WATER_PUMP_PIN, value);
}

// This function is called every time the Virtual Pin 3 state changes
BLYNK_WRITE(VIRTUAL_PIN_SOIL_MOISTURE_THRESHOLD)
{
  nSoilMoistureTheshold = param.asInt();
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  Blynk.syncVirtual(VIRTUAL_PIN_WATER_PUMP);
  Blynk.virtualWrite(VIRTUAL_PIN_SOIL_MOISTURE, nSoilMoistureTheshold);
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  Blynk.virtualWrite(VIRTUAL_PIN_UPTIME, millis() / 1000);

  // get soil moisture value
  int soilMoisture = analogRead(A0);
  Blynk.virtualWrite(V4, soilMoisture);
  int nState = (soilMoisture > nSoilMoistureTheshold);
  digitalWrite(WATER_PUMP_PIN, nState);
  Blynk.virtualWrite(VIRTUAL_PIN_WATER_PUMP_STATE, nState);
}

void setup()
{
  // Debug console
  pinMode(WATER_PUMP_PIN, OUTPUT);
  digitalWrite(WATER_PUMP_PIN, LOW);
  Serial.begin(115200);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run();
}
