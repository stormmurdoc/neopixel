/***************************************************************************
  ESP Skeleton
  Descriptionc: A template for ESP8266 devices
 ***************************************************************************/

/* include stuff */
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <main_macros.h>

#include <Adafruit_NeoPixel.h>
#define PIN D7
#define NUM_LEDS 100

bool effectSet=false;
char* CurrentEffect="xmas";

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
int i=0;

void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   strip.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}

// set one LED
void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}



WiFiClient espClient;
PubSubClient client(espClient);
WiFiUDP ntpUDP;

// pio lib install "arduino-libraries/NTPClient"
// europe.pool.ntp.org
NTPClient timeClient(ntpUDP,NTP_SERVER, 3600, 60000);

long lastMsg = 0;
int lastLEDState = 0;
int value = 0;
char msg[50];
char tmp[50];
char time_value[20];

// LED Pin
const int ledPin = D7;

#include <effects.h>
#include <main_functions.h>


/*
 * Setup procedure
*/
void setup()
{
    // init serial interface
    Serial.begin(115200);

    // MQTT define topics
    snprintf(cMQTT_TOPIC,sizeof cMQTT_TOPIC,"%s%s",MQTT_TOPIC, "/#");
    snprintf(MQTT_TOPIC_OUT,sizeof MQTT_TOPIC_OUT,"%s%s",MQTT_TOPIC, "/output");
    snprintf(MQTT_TOPIC_RSSI,sizeof MQTT_TOPIC_RSSI,"%s%s",MQTT_TOPIC, "/rssi");
    snprintf(MQTT_TOPIC_CMND,sizeof MQTT_TOPIC_CMND,"%s%s",MQTT_TOPIC, "/cmnd");
    snprintf(MQTT_TOPIC_EFFECT,sizeof MQTT_TOPIC_EFFECT,"%s%s",MQTT_TOPIC, "/effect");
    client.setServer(MQTT_SERVER, MQTT_PORT);
    client.setCallback(callback);

    // init stuff
    setup_wifi();
    setup_ota();
    setup_mqtt();

    // NTP time sync
    timeClient.begin();

    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(ledPin, OUTPUT);

    // print some debug infos
    printDevInfos();

    // set birghtness
    effectSet = false;
    strip.setBrightness(80);
}

/*
 *  Main loop
 */
void loop()
{
    long now = millis();
    if (!client.connected()) {
        Serial.println("MQTT connection down!");
        reconnect();
    }
    client.loop();
    timeClient.update();
    ArduinoOTA.handle();
    sendTelemetrie(now,CurrentEffect);
}
