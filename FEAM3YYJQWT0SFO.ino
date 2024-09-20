#include "LedControl.h"

#define ULTRASONIC_TRIGGER_PIN 9
#define ULTRASONIC_ECHO_PIN 6

#define MIC_PIN A1 // Microphone is attached to this analog pin

LedControl lc = LedControl(11, 13, 10, 4); // Pins: DIN,CLK,CS, # of Display connected

#define INPUT_FLOOR 10    // Lower range of analogRead input
#define INPUT_CEILING 300 // Max range of analogRead input, the lower the value the more sensitive (1023 = max)
#define SAMPLE_WINDOW 10  // Sample window for average level

unsigned long delayTime = 200; // Delay between Frames
unsigned int sample;
long duration;
int distance;
byte peak = 16; // Peak level of column; used for falling dots

byte lowestValueMatrixState[] = {
    B00000000,
    B00000000,
    B00000000,
    B00011000,
    B00011000,
    B00000000,
    B00000000,
    B00000000,
};

byte lowValueMatrixState[] = {
    B00000000,
    B00000000,
    B00111100,
    B00111100,
    B00111100,
    B00111100,
    B00000000,
    B00000000,
};

byte mediumValueMatrixState[] = {
    B00000000,
    B01111110,
    B01111110,
    B01111110,
    B01111110,
    B01111110,
    B01111110,
    B00000000,
};

byte maxValueMatrixState[] = {
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
};

void setup()
{
  Serial.begin(9600);                      // Start serial communication
  pinMode(ULTRASONIC_TRIGGER_PIN, OUTPUT); // Sets the trigPin as an Output
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);     // Sets the echoPin as an Input
  lc.shutdown(0, false);                   // Wake up displays
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

void sLowestState3()
{
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0, i, lowestValueMatrixState[i]);
    lc.setRow(1, i, lowestValueMatrixState[i]);
    lc.setRow(2, i, lowestValueMatrixState[i]);
    lc.setRow(3, i, lowestValueMatrixState[i]);
  }
}

void sLowState3()
{
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0, i, lowValueMatrixState[i]);
    lc.setRow(1, i, lowValueMatrixState[i]);
    lc.setRow(2, i, lowValueMatrixState[i]);
    lc.setRow(3, i, lowValueMatrixState[i]);
  }
}

void sMediumState3()
{
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0, i, mediumValueMatrixState[i]);
    lc.setRow(1, i, mediumValueMatrixState[i]);
    lc.setRow(2, i, mediumValueMatrixState[i]);
    lc.setRow(3, i, mediumValueMatrixState[i]);
  }
}

void sMaxState3()
{
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0, i, maxValueMatrixState[i]);
    lc.setRow(1, i, maxValueMatrixState[i]);
    lc.setRow(2, i, maxValueMatrixState[i]);
    lc.setRow(3, i, maxValueMatrixState[i]);
  }
}

void useUltrasonicSensor()
{
  // Clears the trigPin
  digitalWrite(ULTRASONIC_TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  // Sets the ULTRASONIC_TRIGGER_PIN on HIGH state for 10 micro seconds
  digitalWrite(ULTRASONIC_TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIGGER_PIN, LOW);
  // Reads the ULTRASONIC_ECHO_PIN, returns the sound wave travel time in microseconds
  duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
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

  if (peakToPeak >= 250)
  {
    sMaxState3();
  }
  else if (peakToPeak >= 200)
  {
    sMediumState3();
  }
  else if (peakToPeak >= 150)
  {
    sLowState3();
  }
  else if (peakToPeak <= 100)
  {
    sLowestState3();
  }

  useUltrasonicSensor();
}
