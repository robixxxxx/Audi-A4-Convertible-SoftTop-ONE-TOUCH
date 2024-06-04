#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include <can.h>

AsyncWebServer server(80);
DNSServer dnsServer;
IPAddress apIP(192, 168, 4, 1);

struct NetworkParameters
{
  char ssid[32];
  char password[32];
} paramNetwork;




void wifiInit(char * ssid, char * password){

    
        // WiFi.begin(ssid, password);
        // while (WiFi.status() != WL_CONNECTED) {
        //     delay(1000);
        //     Serial.println("Connecting to WiFi...");
        // }
        // Serial.println("Connected to WiFi");
    
    

    // Konfiguracja serwera WWW
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        String html = "<html><body><h1>ESP8266 Configuration</h1>";
        html += "<ul>";
        html += "<li><a href=\"/config\">Configure WiFi</a></li>";
        html += "<li><a href=\"/errors\">View CAN Errors</a></li>";
        html += "</ul></body></html>";
        request->send(200, "text/html", html);
    });
    server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request) {
        String html = "<html><body><h1>Configure WiFi</h1>";
        html += "<form action=\"/set\" method=\"post\">";
        html += "SSID: <input type=\"text\" name=\"ssid\"><br>";
        html += "Password: <input type=\"text\" name=\"password\"><br>";
        html += "<input type=\"submit\" value=\"Save\">";
        html += "</form><br><a href=\"/\">Back to Home</a></body></html>";
        request->send(200, "text/html", html);
    });

    server.on("/set", HTTP_POST, [](AsyncWebServerRequest *request) {
        String new_ssid;
        String new_password;
        if (request->hasParam("ssid", true)) {
            new_ssid = request->getParam("ssid", true)->value();
        }
        if (request->hasParam("password", true)) {
            new_password = request->getParam("password", true)->value();
        }
        // new_ssid.toCharArray(paramNetwork.ssid, 32);
        // new_password.toCharArray(password, 32);
        // saveCredentials(ssid, password);
        String html = "<html><body><h1>Settings Saved</h1>";
        html += "<p>SSID: " + new_ssid + "</p>";
        html += "<p>Password: " + new_password + "</p>";
        html += "<p>Please restart your device to apply the new settings.</p>";
        html += "<br><a href=\"/\">Back to Home</a></body></html>";
        request->send(200, "text/html", html);
    });

    server.on("/errors", HTTP_GET, [](AsyncWebServerRequest *request) {
        String html = "<html><body><h1>MCP2515 Error Log</h1>";
        html += "<pre>CAN0 Initialized: " + String(CAN0_INIT_FLAG) + "</pre>";
        html += "<pre>" + readCANErrors(&CAN0) + "</pre>";
        html += "<pre>CAN1 Initialized: " + String(CAN1_INIT_FLAG) + "</pre>";
        html += "<pre>" + readCANErrors(&CAN1) + "</pre>";
        html += "</body></html>";
        request->send(200, "text/html", html);
    });

    // Uruchomienie serwera
    server.begin();
}