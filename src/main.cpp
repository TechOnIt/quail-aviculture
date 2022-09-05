// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Heater/HeaterService.h>

// Input pins
#define SETTER_TEMPERATURE_PIN 2
#define HATCHER_TEMPERATURE_PIN 4
#define TEMPERATURE_Check_PIN 8

// Output pins
#define HEATER_PIN 7
#define FAN_PIN 12

// initialize variables.
const float minSetter = 37.5;
const float maxSetter = 37.7;

const float minHatcher = 37.15;
const float maxHatcher = 37.25;

bool isSetterMode = true;
bool isHeaterOn = false;

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire setterOneWire(SETTER_TEMPERATURE_PIN);
OneWire hatcherOneWire(HATCHER_TEMPERATURE_PIN);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature setterTemperatureSensor(&setterOneWire);
DallasTemperature hatcherTemperatureSensor(&hatcherOneWire);

HeaterService heaterService;

void setup(void)
{
  Serial.begin(9600);

  pinMode(HEATER_PIN, OUTPUT);
  pinMode(TEMPERATURE_Check_PIN, OUTPUT);

  // Start up the library
  setterTemperatureSensor.begin();
  hatcherTemperatureSensor.begin();
}

void loop(void)
{
  setterTemperatureSensor.requestTemperatures(); // Send the command to get temperatures
  float TempC = setterTemperatureSensor.getTempCByIndex(0);
  if (TempC == DEVICE_DISCONNECTED_C)
  {
    hatcherTemperatureSensor.requestTemperatures(); // Send the command to get temperatures
    TempC = hatcherTemperatureSensor.getTempCByIndex(0);
    // Check if reading was successful
    if (TempC != DEVICE_DISCONNECTED_C)
    {
      // HATCHER
      heaterService.ControlHeaterWithStandBy(HEATER_PIN, FAN_PIN, TempC, minHatcher, maxHatcher);
      digitalWrite(TEMPERATURE_Check_PIN, LOW);
    }
    else
    {
      // Temperature sensor is down!
      Serial.println("Error: Could not read temperature data!");
      digitalWrite(TEMPERATURE_Check_PIN, HIGH);
    }
  }
  else
  {
    // SETTER
    heaterService.ControlHeaterWithStandBy(HEATER_PIN, FAN_PIN, TempC, minHatcher, maxHatcher);
    digitalWrite(TEMPERATURE_Check_PIN, LOW);
  }
  if (TempC != DEVICE_DISCONNECTED_C)
  {
    Serial.print("Temperature is: ");
    Serial.println(TempC);
  }

  delay(200);
}