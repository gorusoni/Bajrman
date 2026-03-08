#define BLYNK_TEMPLATE_ID "TMPL3ZyoS5Roy"
#define BLYNK_TEMPLATE_NAME "BAJRMAN"
#define BLYNK_AUTH_TOKEN "JBq2q2-w4piicOXDk-_A7QibJwIspfhp"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "ESP32TEST";
char pass[] = "12345678";

// Touch sensor pins (GPIO)
#define T1 13
#define T2 12
#define T3 14
#define T4 27

BlynkTimer timer;

bool lastMainState = false;
bool invalidTimerStarted = false;
unsigned long invalidStartTime = 0;

void checkLogic()
{
  // Read sensors (change to !digitalRead if sensors are active LOW)
  int s1 = digitalRead(T1);
  int s2 = digitalRead(T2);
  int s3 = digitalRead(T3);
  int s4 = digitalRead(T4);

  // Send raw sensor values to dashboard
  Blynk.virtualWrite(V0, s1);
  Blynk.virtualWrite(V1, s2);
  Blynk.virtualWrite(V2, s3);
  Blynk.virtualWrite(V3, s4);

  bool validState = false;

  // VALID conditions:
  // (1 & 3 only) OR (2 & 4 only)
  if (s1 && s3 && !s2 && !s4)
    validState = true;

  if (s2 && s4 && !s1 && !s3)
    validState = true;

  // Control main light (V10)
  if (validState != lastMainState)
  {
    Blynk.virtualWrite(V10, validState);
    lastMainState = validState;
  }

 static unsigned long lastInvalidDetected = 0;
static bool eventSent = false;

bool invalidState = !validState && (s1 || s2 || s3 || s4);

if (invalidState)
{
  lastInvalidDetected = millis();

  if (!eventSent && (millis() - invalidStartTime >= 10000))
  {
    Serial.println("INVALID TRIGGERED");
    Blynk.logEvent("invalid_state", "Please Wear Seat Belt Properly!");
    eventSent = true;
  }

  if (!invalidTimerStarted)
  {
    invalidTimerStarted = true;
    invalidStartTime = millis();
  }
}
else
{
  // Only reset if no invalid detected for 2 seconds
  if (millis() - lastInvalidDetected > 2000)
  {
    invalidTimerStarted = false;
    eventSent = false;
  }
}
  // Debug output
  Serial.printf("S1:%d S2:%d S3:%d S4:%d | Valid:%d\n", s1, s2, s3, s4, validState);
}

void setup()
{
  Serial.begin(115200);

  pinMode(T1, INPUT);
  pinMode(T2, INPUT);
  pinMode(T3, INPUT);
  pinMode(T4, INPUT);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(200L, checkLogic);
}

void loop()
{
  Blynk.run();
  timer.run();
}