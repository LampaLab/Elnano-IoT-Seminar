#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Update these with values suitable for your network.

const char* ssid = "YOUR_WIFI_NETWORK_ID";
const char* password = "YOUR_WIFI_NETWORK_PASS";
const char* mqtt_server = "cloud.iot-playground.com";

#define ONE_WIRE_BUS D7

WiFiClient espClient;
PubSubClient client(espClient);

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

char msg[10];

int temp;
int voltage;

char inTopicLedCtrl[] = "/Db/5713b0e0c943a022c0d2c4f1/2/Sensor.Parameter1";
char outTopicTemp[] = "/Db/5713b0e0c943a022c0d2c4f1/1/Sensor.Parameter1";
char outTopicVoltage[] = "/Db/5713b0e0c943a022c0d2c4f1/1/Sensor.Parameter2";

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, HIGH);  
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else if ((char)payload[0] == '0'){
    digitalWrite(BUILTIN_LED, LOW);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client", "lampa", "a^2+b^2=c^2")) {
      Serial.println("connected");
      // ... and resubscribe
      client.subscribe(inTopicLedCtrl);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

    sensors.requestTemperatures();
    
    temp = sensors.getTempCByIndex(0);
  
    snprintf (msg, 5, "%ld", temp);

    Serial.print("Temp: ");
    Serial.println(temp);
   
    client.publish(outTopicTemp, msg);

    voltage = analogRead(A0);

    snprintf (msg, 5, "%ld", voltage);

    Serial.print("Volt: ");
    Serial.println(voltage);

    client.publish(outTopicVoltage, msg);
    
}
