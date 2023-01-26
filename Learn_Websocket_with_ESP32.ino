#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include <Arduino_JSON.h>

const char* ssid = "Wokwi-GUEST"; //Enter SSID
const char* password = ""; //Enter Password
const char* websockets_server_host = "192.168.3.175"; //Enter server adress
const uint16_t websockets_server_port = 5000; // Enter server port

using namespace websockets;

const char* app_id = "";
const char* app_key = "";
const char* app_secret = "";
const char* cluster = "";

String url = "ws://ws-ap1.pusher.com:80/app/APP_KEY?client=esp32&version=7.0.3&protocol=5"

String channel_name = "private-controlling";
// String signature = createHmacSHA256(channel_name, app_secret);
String signature = "";

WebsocketsClient client;
void setup() {
    Serial.begin(115200);
    // Connect to wifi
    WiFi.begin(ssid, password);

    // Wait some time to connect to wifi
    for(int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
        Serial.print(".");
        delay(1000);
    }

    // Check if connected to wifi
    if(WiFi.status() != WL_CONNECTED) {
        Serial.println("No Wifi!");
        return;
    }
    String value = "Hello Koalawan";
    Serial.println("Connected to Wifi, Connecting to server.");
    // try to connect to Websockets server
    bool connected = client.connect(url);
    if(connected) {
        Serial.println("Connected!");
        client.send("{\"event\":\"pusher:subscribe\",\"data\":{\"channel\":\"my-controlling\"}}");
    } else {
        Serial.println("Not Connected!");
    }
    
    // run callback when messages are received
    client.onMessage([&](WebsocketsMessage message){
        Serial.print("Got Message: ");
        Serial.println(message.data());
    });
}

void loop() {
    // let the websockets client check for incoming messages
    if(client.available()) {
        bool control = client.poll();
        if(control) {
            Serial.println("Connected!");
            // Serial.println(client.poll());
            client.onMessage([&](WebsocketsMessage message){
                Serial.print("Got Message: ");
                String value = message.data();
                Serial.println(value);

                JSONVar controlObject = JSON.parse(value);
                Serial.print("Got Data: ");
                String controlData = controlObject["data"];
                JSONVar controlMessage = JSON.parse(controlData);
                String controlValMessage = controlMessage["message"];
                JSONVar controlVal = JSON.parse(controlValMessage);
                JSONVar valueCompare = 1;
                Serial.println(controlValMessage);
                JSONVar control1 = controlVal["control_1"];
                JSONVar control2 = controlVal["control_2"];
                JSONVar control3 = controlVal["control_3"];
                JSONVar control4 = controlVal["control_4"];
                if(control1 == valueCompare){
                    pinMode(2, OUTPUT);
                    digitalWrite(2, HIGH);
                }else{
                    pinMode(2, OUTPUT);
                    digitalWrite(2, LOW);
                }
                if(control2 == valueCompare){
                    pinMode(4, OUTPUT);
                    digitalWrite(4, HIGH);
                }else{
                    pinMode(4, OUTPUT);
                    digitalWrite(4, LOW);
                }
                if(control3 == valueCompare){
                    pinMode(5, OUTPUT);
                    digitalWrite(5, HIGH);
                }else{
                    pinMode(5, OUTPUT);
                    digitalWrite(5, LOW);
                }
                if(control4 == valueCompare){
                    pinMode(18, OUTPUT);
                    digitalWrite(18, HIGH);
                }else{
                    pinMode(18, OUTPUT);
                    digitalWrite(18, LOW);
                }
            });
            
        } else {
            // Serial.println("Not Connected!");
        }
    }

    delay(500);
}
