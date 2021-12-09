/*
 * some device infos
*/
void printDevInfos()
{
	Serial.println(">> Booting");
	Serial.print(">> git rev: ");
    Serial.println(GIT_REV);
	Serial.print(">> ESP_HOSTNAME: ");
    Serial.println(ESP_HOSTNAME);
    Serial.print(">> MQTT_SERVER: ");
    Serial.println(MQTT_SERVER);
    Serial.print(">> MQTT_PORT: ");
    Serial.println(MQTT_PORT);
    Serial.print(">> WIFI SSID: ");
    Serial.println(WIFI_SSID);
    Serial.print(">> NTP SERVER: ");
    Serial.println(NTP_SERVER);
    Serial.print(">> UPDATE INTERVAL (ms): ");
    Serial.println(UPDATE_INTERVAL);
}

/*
 * init WiFi module
 *
*/

void setup_wifi()
{
	WiFi.mode(WIFI_STA);
	WiFi.begin(WIFI_SSID,WIFI_PASSWORD);

	while (WiFi.waitForConnectResult() != WL_CONNECTED) {
		Serial.print("Connection to ");
        Serial.print(WIFI_SSID);
        Serial.println(" failed! Rebooting...");
		delay(5000);
		ESP.restart();
	}
}

/*
 * Over the air updates
 *
*/
void setup_ota()
{
    ArduinoOTA.setPort(8266);
	ArduinoOTA.setHostname(ESP_HOSTNAME);
	ArduinoOTA.setPassword(OTA_PASSWORD);
	ArduinoOTA.onStart([]()
	{
		String type;
		if (ArduinoOTA.getCommand() == U_FLASH) {
			type = "sketch";
		}
		else {
		    type = "filesystem"; // U_FS
		}
		// NOTE: if updating FS this would be the place to unmount FS using FS.end()
		Serial.println("Start updating " + type);
	});

	ArduinoOTA.onEnd([]() {
		Serial.println("\nEnd");
	});

	ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
		Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
	});

	ArduinoOTA.onError([](ota_error_t error)
	{
		Serial.printf("Error[%u]: ", error);
		if (error == OTA_AUTH_ERROR) {
			Serial.println("Auth Failed");
		}
		else if (error == OTA_BEGIN_ERROR) {
			Serial.println("Begin Failed");
		}
		else if (error == OTA_CONNECT_ERROR) {
			Serial.println("Connect Failed");
		}
		else if (error == OTA_RECEIVE_ERROR)
		{
			Serial.println("Receive Failed");
		}
		else if (error == OTA_END_ERROR) {
			Serial.println("End Failed");
		} });

	ArduinoOTA.begin();
	Serial.print(">> Ready");
	Serial.print(" - My IP address: ");
	Serial.println(WiFi.localIP());
}

/*
 * Setup MQTT
*/
void setup_mqtt(){
    while (!client.connected()) {
        Serial.println(">> Connecting to MQTT...");
        if (client.connect(ESP_HOSTNAME)) {
            Serial.print(">> connected to ");
            Serial.println(MQTT_SERVER);
            client.subscribe(cMQTT_TOPIC);
        } else {
            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }
}

/*
 * MQTT (re)connect function
*/
void reconnect() {
 // Loop until we're reconnected
  while (!client.connected()) {
      Serial.println(">> Attempting MQTT connection...");
      Serial.println(client.state());
      /* setup mqtt_server */
      if (client.connect(ESP_HOSTNAME)) {
          Serial.print(">> MQTT connected to ");
          Serial.print(MQTT_SERVER);
          Serial.print(":");
          Serial.println(MQTT_PORT);
          Serial.print(">> subcribing ");
          Serial.println(cMQTT_TOPIC);
          client.subscribe(cMQTT_TOPIC);
      } else {
          // Attempt to connect
          Serial.print("failed, rc=");
          Serial.print(client.state());
          Serial.println(" try again in 5 seconds");
          // Wait 5 seconds before retrying
          delay(5000);
      }
  }
}

/*
 * Flash LED
*/
void FlashStatusLED(int led){

    int i = 0;
    while(i<3){
        if(digitalRead(led) == HIGH){
            digitalWrite(led, LOW);
        } else {
            digitalWrite(led, HIGH);
        }
        delay(20);
        i = i + 1;
    }
    digitalWrite(led, LOW);
}

/*
 * Publish mqtt data
*/
void sendTelemetrie(long now, String currenteffect){

    // send telemetrie data
    if (now - lastMsg > UPDATE_INTERVAL) {
        lastMsg = now;
        long rssi = WiFi.RSSI();
        itoa(rssi,tmp,10);
        FlashStatusLED(LED_BUILTIN);
        client.publish(MQTT_TOPIC_RSSI, tmp);
        //client.publish(MQTT_TOPIC_EFFECT, currenteffect);
    }
}

/*
 * Publish mqtt data
 void callback(char* topic, byte* message, unsigned int length) {
*/
void callback(char* topic, byte* message, int length) {

    Serial.print("[");
    Serial.print(timeClient.getFormattedTime());
    Serial.print("]");
    Serial.print(" >> Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    FlashStatusLED(LED_BUILTIN);
    String messageTemp;
    const char *LED_STATE;
    const char *EFFECT="";
    const char *CMND="";

    for (int i = 0; i < length; i++) {
      Serial.print((char)message[i]);
      messageTemp += (char)message[i];
    }

    // cmnd topic
    if (String(topic) == String(MQTT_TOPIC_CMND)) {
      if(messageTemp == "set_on"){
        digitalWrite(ledPin, HIGH);
        lastLEDState=HIGH;
        CMND="on";
      }
      else if(messageTemp == "set_off"){
        digitalWrite(ledPin, LOW);
        lastLEDState=LOW;
        CMND="off";
      }
      else if(messageTemp == "set_schalke"){
        EFFECT="schalke";
        CMND="schalke";
      }
      else if(messageTemp == "set_xmas"){
        Serial.println(">> xmas");
        EFFECT="xmas";
        CMND="xmas";
      }
      else if(messageTemp == "set_pct_1"){
        strip.setBrightness(1);
        strip.show();
      }
      else if(messageTemp == "set_pct_5"){
        strip.setBrightness(5);
        strip.show();
      }
      else if(messageTemp == "set_pct_10"){
        strip.setBrightness(10);
        strip.show();
      }
      else if(messageTemp == "set_pct_20"){
        strip.setBrightness(20);
        strip.show();
      }
      else if(messageTemp == "set_pct_30"){
        strip.setBrightness(30);
        strip.show();
      }
      else if(messageTemp == "set_pct_40"){
        strip.setBrightness(40);
        strip.show();
      }
      else if(messageTemp == "set_pct_50"){
        strip.setBrightness(50);
        strip.show();
      }
      else if(messageTemp == "set_pct_60"){
        strip.setBrightness(60);
        strip.show();
      }
      else if(messageTemp == "set_pct_70"){
        strip.setBrightness(70);
        strip.show();
      }
      else if(messageTemp == "set_pct_80"){
        strip.setBrightness(80);
        strip.show();
      }
      else if(messageTemp == "set_pct_90"){
        strip.setBrightness(90);
        strip.show();
      }
      else if(messageTemp == "set_pct_100"){
        strip.setBrightness(100);
        strip.show();
      }
      else if(messageTemp == "set_pct_150"){
        strip.setBrightness(150);
        strip.show();
      }
      else if(messageTemp == "set_pct_200"){
        strip.setBrightness(200);
        strip.show();
      }

      else if(messageTemp == "toggle"){
        if(lastLEDState == HIGH){
            digitalWrite(ledPin, LOW);
            lastLEDState=LOW;
        }
        else {
            digitalWrite(ledPin, HIGH);
            lastLEDState=HIGH;
        }
      }
      else if(messageTemp == "reboot"){
        Serial.print("reboot");
        ESP.restart();
      }
      if(lastLEDState == 1 ) {
          LED_STATE="on";
      }
      else {
          LED_STATE="off";
      }
      Serial.print(">> ");
      Serial.println(messageTemp);
      Serial.println("<< ");
      client.publish(MQTT_TOPIC_OUT, LED_STATE);
      //client.publish(MQTT_TOPIC_CMND, CMND);
      client.publish(MQTT_TOPIC_EFFECT, EFFECT);
    }
    Serial.println("");
}
