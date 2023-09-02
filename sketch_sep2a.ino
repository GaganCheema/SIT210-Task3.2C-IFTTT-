#include "arduino_secrets.h"
#include <WiFiNINA.h>
#include <BH1750.h>
#include <Wire.h>

BH1750 Light;

// Please enter your sensitive data in the Secret tab
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASSWORD;

WiFiClient client;

char HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME_ON = "/trigger/Sunlight_ON/with/key/YOUR_KEY_HERE";   // change your EVENT-NAME and YOUR-KEY
String PATH_NAME_OFF = "/trigger/Sunlight_OFF/with/key/YOUR_KEY_HERE";  // change your EVENT-NAME and YOUR-KEY
String queryString = "?value1=57&value2=25";

void setup() {
  // Initialize WiFi connection
  WiFi.begin(ssid, pass);
  Serial.begin(9600);

  while (!Serial) {
    // Wait for Serial to connect
  }

  Wire.begin();
  Light.begin();

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void loop() {
  float lux = Light.readLightLevel();

  // Connect to the web server on port 80:
  if (client.connect(HOST_NAME, 80)) {
    Serial.println("Connected to server");
  } else {
    Serial.println("Connection failed");
  }

  if (lux > 300) {
    // Make an HTTP request to turn on something
    client.println("GET " + PATH_NAME_ON + queryString + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println();  // End HTTP header

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.print(c);
      }
    }

    client.stop();
    Serial.println();
    Serial.println("Disconnected");

    // Wait until the light level drops below 300 lux
    while (lux > 300) {
      lux = Light.readLightLevel();
      Serial.print("Light: ");
      Serial.print(lux);
      Serial.println(" lx");
      delay(1000);
    }
  }

  if (lux < 25) {
    // Make an HTTP request to turn off something
    if (client.connect(HOST_NAME, 80)) {
      Serial.println("Connected to server");
    } else {
      Serial.println("Connection failed");
    }

    client.println("GET " + PATH_NAME_OFF + queryString + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println();  // End HTTP header

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.print(c);
      }
    }

    client.stop();
    Serial.println();
    Serial.println("Disconnected");

    // Wait until the light level goes above 25 lux
    while (lux < 25) {
      lux = Light.readLightLevel();
      Serial.print("Light: ");
      Serial.print(lux);
      Serial.println(" lx");
      delay(1000);
    }
  }
}
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
