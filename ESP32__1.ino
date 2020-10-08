#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
 int time_US;
int distance_US;
const int trigger=2;
const int echo=4;

// Update these with values suitable for your network.

const char* ssid = "SKP-Home";                                   // WIFI SSID of network
const char* password = "#SKPanutap1";                               // WIFI password of the newtowk

const char* mqtt_server = "mqtt.thingsup.io";            // Thingsup Broker URL
const char* mqtt_password = "esp32";                          // Device Password set in the device addition stage in thingsup
const char* mqtt_username = "3p309x1kf3yxwdw:esp32";                          // Device Key set in the device addition stage in thingsup
String clientId = "me_client";                                    // Client ID set in the device addition stage in thingsup
unsigned int mqtt_port = 1883;                           // MQTT port for SSL connection in thingsup
String accountID = "3p309x1kf3yxwdw";

long current_millis = 0;

WiFiClientSecure espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

/*
 * This function sets ESP in STA mode and connects to the WIFI
 * Args: Null
 * Returns: Null
 */

void setup_wifi() {

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

/*
 * This function is callback function which gets triggered with data is recived on the MQTT subscribed topic
 * Args: Null
 * Returns: Topic name, Payload, Length
 */

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

/*
 * This function Reconnects to MQTT when there is disconnection
 * Args: Null
 * Returns: Null
 */

void reconnect() {
  // Loop until we're reconnected
  if (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    Serial.print("clientId : ");
    Serial.println(clientId);

    Serial.print("mqtt_user=");
    Serial.println(mqtt_username);
    Serial.print("mqtt_pass=");
    Serial.println(mqtt_password);

    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password))
    {
      Serial.println("Connection Established Firmly");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
    }
  }
}

void setup() {
  pinMode(echo,INPUT);
  pinMode(trigger,OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setClient(espClient);
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  if (millis() - current_millis > 10000)
  {
    current_millis = millis();

    if (client.connected())
    {
      digitalWrite (trigger,0);
      delayMicroseconds(3) ;
      digitalWrite(trigger,1);
      delayMicroseconds(10);
      digitalWrite(trigger,0);
      time_US=pulseIn(echo,1);
      distance_US=time_US*.034/2;
      Serial.print("Distance==");
      Serial.print(distance_US);
      Serial.println();
      Serial.print("Publish message: ");
      String publish_topic = "/"  + accountID + "/" + "ESP";   // The topic name should be always start with /accountID
      String datatosend= "{\"Distance\":"+ String(distance_US)+"}";
      if (client.publish((char*)publish_topic.c_str(),datatosend.c_str()) == true)
      {
        Serial.println("Publish sucess: ");
      }
    }
  }
}
