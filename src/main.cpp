#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

// const char *ssid = "Nom du réseau";
// const char *password = "Mot de passe";

const char *ssid = "BTS_CIEL";
const char *password = "ERIR1234";

// Set LED GPIO
const int ledPin = 2;
// Stores LED state
String ledState;
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
// Replaces placeholder with LED state value
String processor(const String &var)
{
    Serial.println(var);
    if (var == "STATE")
    {
        if (digitalRead(2))
        {
            ledState = "MARCHE";
        }
        else
        {
            ledState = "ARRET";
        }
        Serial.print(ledState);
        return ledState;
    }
    return String();
}
void setup()
{
    // Serial port for debugging purposes
    Serial.begin(9600);
    pinMode(2, OUTPUT);
    // Initialize SPIFFS
    if (!SPIFFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }
    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }
    // Print ESP32 Local IP Address
    Serial.println(WiFi.localIP());
    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/index.html", String(), false, processor); });
    // Route to load style.css file
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/style.css", "text/css"); });
    // Route to set GPIO to HIGH
    server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request)
              {
digitalWrite(ledPin, HIGH);
request->send(SPIFFS, "/index.html", String(), false, processor); });
    // Route to set GPIO to LOW
    server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request)
              {
digitalWrite(ledPin, LOW);
request->send(SPIFFS, "/index.html", String(), false, processor); });
    // Start server
    server.begin();
}
void loop()
{
}