#include "LedControl.h"
#include <DHT.h>

#define MIC_PIN A1 // Microphone is attached to this analog pin
#define DHTPIN 2   // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
// #define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22 // DHT 22  (AM2302), AM2321

// #define DHTTYPE DHT21   // DHT 21 (AM2301)
LedControl lc = LedControl(11, 13, 10, 4); // Pins: DIN,CLK,CS, # of Display connected
#define INPUT_FLOOR 10                     // Lower range of analogRead input
#define INPUT_CEILING 300                  // Max range of analogRead input, the lower the value the more sensitive (1023 = max)
#define SAMPLE_WINDOW 10                   // Sample window for average level

unsigned long delayTime = 200; // Delay between Frames
unsigned int sample;
byte peak = 16; // Peak level of column; used for falling dots

// Put values in arrays
byte invader1a[] = {
    B01100111,
    B01101110,
    B01111100,
    B01111000,
    B01111000,
    B01101100,
    B01100110,
    B01100011,
};

byte invader1b[] = {
    B00000000,
    B01100110,
    B10011001,
    B10000001,
    B10000001,
    B01000010,
    B00100100,
    B00011000,
};

byte invader2a[] = {
    B00111100,
    B01000010,
    B10000001,
    B10000001,
    B10000001,
    B10000001,
    B01000010,
    B00111100,
};

byte invader2b[] = {
    B00000000,
    B01100110,
    B10011001,
    B10000001,
    B10000001,
    B01000010,
    B00100100,
    B00011000,
};

byte invader3a[] = {
    B11111111,
    B11111111,
    B00011000,
    B00011000,
    B00011000,
    B00011000,
    B00011000,
    B00011000,
};

byte invader3b[] = {
    B00000000,
    B01100110,
    B10011001,
    B10000001,
    B10000001,
    B01000010,
    B00100100,
    B00011000,
};

byte invader4a[] = {
    B00011111,
    B00100011,
    B01000011,
    B01000011,
    B00111111,
    B00011111,
    B00110011,
    B01100011,
};

byte invader4b[] = {
    B00000000,
    B01100110,
    B10011001,
    B10000001,
    B10000001,
    B01000010,
    B00100100,
    B00011000,
};

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin(9600);    // Start serial communication
  lc.shutdown(0, false); // Wake up displays
  lc.shutdown(1, false);
  lc.shutdown(2, false);
  lc.shutdown(3, false);
  lc.setIntensity(0, 5); // Set intensity levels
  lc.setIntensity(1, 5);
  lc.setIntensity(2, 5);
  lc.setIntensity(3, 5);
  lc.clearDisplay(0); // Clear Displays
  lc.clearDisplay(1);
  lc.clearDisplay(2);
  lc.clearDisplay(3);
}

//  Take values in Arrays and Display them
void sinvader1a()
{
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(3, i, invader1a[i]);
  }
}

void sinvader1b()
{
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(3, i, invader1b[i]);
  }
}

void sinvader2a()
{
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(2, i, invader2a[i]);
  }
}

void sinvader2b()
{
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(2, i, invader2b[i]);
  }
}

void sinvader3a()
{
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(1, i, invader3a[i]);
  }
}

void sinvader3b()
{
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(1, i, invader3b[i]);
  }
}

void sinvader4a()
{
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0, i, invader4a[i]);
  }
}

void sinvader4b()
{
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0, i, invader4b[i]);
  }
}

void loop()
{
  unsigned long startMillis = millis(); // Start of sample window
  float peakToPeak = 0;                 // peak-to-peak level

  unsigned int signalMax = 0;
  unsigned int signalMin = 1023;
  unsigned int c, y;

  // collect data for length of sample window (in mS)
  while (millis() - startMillis < SAMPLE_WINDOW)
  {
    sample = analogRead(MIC_PIN);
    if (sample < 1024) // toss out spurious readings
    {
      if (sample > signalMax)
      {
        signalMax = sample; // save just the max levels
      }
      else if (sample < signalMin)
      {
        signalMin = sample; // save just the min levels
      }
    }
  }

  peakToPeak = signalMax - signalMin; // max - min = peak-peak amplitude

  Serial.println(peakToPeak);

  // Put #1 frame on both Display
  sinvader1a();
  delay(delayTime);
  sinvader2a();
  delay(delayTime);

  // Put #2 frame on both Display
  sinvader1b();
  delay(delayTime);
  sinvader2b();
  delay(delayTime);

  // Put #3 frame on both Display
  sinvader3a();
  delay(delayTime);
  sinvader4a();
  delay(delayTime);

  // Put #4 frame on both Display
  sinvader3b();
  delay(delayTime);
  sinvader4b();
  delay(delayTime);
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f))
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
}
