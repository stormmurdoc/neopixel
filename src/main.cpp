/***************************************************************************
  ESP8266 Sketch for Neopixel USB SK6812 LED String
  vim:filetype=arduino
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

/*
 * define required objects
 */
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
WiFiClient espClient;
PubSubClient client(espClient);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,NTP_SERVER, 3600, 60000);

/*
 * include some additional files
 */
String CurrentEffect="";
int i=0;
int j=0;
int k=0;
int l=0;
int m=0;
int n=0;
long lastMsg = 0;
long lastnow = 0;
int lastLEDState = 0;
int value = 0;
char msg[50];
char tmp[50];
char time_value[20];

// LED Pin which control the LED string
const int ledPin = D7;

/*
 * include some additional files
 */
#include <effects.h>
#include <main_functions.h>


/*
 * Setup procedure
*/
void setup()
{
    // init serial interface for debugging purpose
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

    // set brightness to pull not so much power
    strip.setBrightness(80);

    // check strip
    setAll(0xff,0,0);
    delay(1000);
    setAll(0,0xff,0);
    delay(1000);
    setAll(0,0,0xff);
    delay(1000);
    setAll(0,0,0);
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
    //sendTelemetrie(now,CurrentEffect);

    // run the effects

  if (now - lastnow > 1000) {
   if (i<NUM_LEDS){
    strip.setPixelColor(i,125,0,0);
    strip.show();
   }
   if ( i > 10 ) {
    strip.setPixelColor(j,80,45,0);
    strip.show();
    j++;
   }
   if ( i > 20 ) {
    strip.setPixelColor(k,64,64,0);
    strip.show();
    k++;
   }
   if ( i > 30 ) {
    strip.setPixelColor(l,0,64,64);
    strip.show();
    l++;
   }
   if ( i > 40 ) {
    strip.setPixelColor(m,0,0,125);
    strip.show();
    m++;
   }
   if ( i > 50 ) {
    strip.setPixelColor(n,0,125,0);
    strip.show();
    n++;
   }


   i++;
   if (i>NUM_LEDS) {
    i=0;
   }
   if (j>NUM_LEDS) {
    j=0;
   }
   if (k>NUM_LEDS) {
    k=0;
   }
   if (l>NUM_LEDS) {
    l=0;
   }
   if (m>NUM_LEDS) {
    m=0;
   }
   if (n>NUM_LEDS) {
    n=0;
   }
   lastnow=now;
  }


  // if (CurrentEffect.length() == 0) {
  //      xmas();
  //      CurrentEffect="xmas";
  //  }

  //if (now - lastnow > 2000) {
  //    uint32_t oldColor;
  //    int Pixel = random(NUM_LEDS);
  //    oldColor = strip.getPixelColor(Pixel);
  //    uint8_t r = oldColor >> 16;
  //    uint8_t g = oldColor >> 8;
  //    uint8_t b = oldColor;
  //    setPixel(Pixel,0xff,0xff,0xff);
  //    showStrip();
  //    delay(20);
  //    setPixel(Pixel,r,g,b);
  //    showStrip();
  //    delay(200);
  //    lastnow=now;
  //}
  //  //int Pixel=0;
    //int sleep=30;
    //for (Pixel=0;Pixel<NUM_LEDS;Pixel++){
    //    oldColor = strip.getPixelColor(Pixel);
    //    uint8_t r = oldColor >> 16;
    //    uint8_t g = oldColor >> 8;
    //    uint8_t b = oldColor;

    //    setPixel(Pixel,0xff,0xff,0xff);
    //    showStrip();
    //    delay(sleep);
    //    setPixel(Pixel,r,g,b);
    //    showStrip();
    //    delay(sleep);
    //}
    //showStrip();
    //for (Pixel=NUM_LEDS;Pixel<1;Pixel--){
    //    oldColor = strip.getPixelColor(Pixel);
    //    uint8_t r = oldColor >> 16;
    //    uint8_t g = oldColor >> 8;
    //    uint8_t b = oldColor;

    //    setPixel(Pixel,0xff,0xff,0xff);
    //    showStrip();
    //    delay(sleep);
    //    setPixel(Pixel,r,g,b);
    //    showStrip();
    //    delay(sleep);
    //}

}
