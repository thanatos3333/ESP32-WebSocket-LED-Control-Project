#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

const char* ssid = "Orange_Swiatlowod_2780"; // WiFi SSID
const char* password = "yuievrRXWaf5W6gkTQ"; // WiFi Password
const char* websocketServer = "frosted-simple-pheasant.glitch.me"; // Glitch server domain

WebSocketsClient webSocket; // WebSocket client
WiFiClientSecure client; // Secure WiFi client

const char* rootCACertificate = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIEdTCCA12gAwIBAgIJAKcOSkw0grd/MA0GCSqGSIb3DQEBCwUAMGgxCzAJBgNV\n" \
"BAYTAlVTMSUwIwYDVQQKExxTdGFyZmllbGQgVGVjaG5vbG9naWVzLCBJbmMuMTIw\n" \
"MAYDVQQLEylTdGFyZmllbGQgQ2xhc3MgMiBDZXJ0aWZpY2F0aW9uIEF1dGhvcml0\n" \
"eTAeFw0wOTA5MDIwMDAwMDBaFw0zNDA2MjgxNzM5MTZaMIGYMQswCQYDVQQGEwJV\n" \
"UzEQMA4GA1UECBMHQXJpem9uYTETMBEGA1UEBxMKU2NvdHRzZGFsZTElMCMGA1UE\n" \
"ChMcU3RhcmZpZWxkIFRlY2hub2xvZ2llcywgSW5jLjE7MDkGA1UEAxMyU3RhcmZp\n" \
"ZWxkIFNlcnZpY2VzIFJvb3QgQ2VydGlmaWNhdGUgQXV0aG9yaXR5IC0gRzIwggEi\n" \
"MA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDVDDrEKvlO4vW+GZdfjohTsR8/\n" \
"y8+fIBNtKTrID30892t2OGPZNmCom15cAICyL1l/9of5JUOG52kbUpqQ4XHj2C0N\n" \
"Tm/2yEnZtvMaVq4rtnQU68/7JuMauh2WLmo7WJSJR1b/JaCTcFOD2oR0FMNnngRo\n" \
"Ot+OQFodSk7PQ5E751bWAHDLUu57fa4657wx+UX2wmDPE1kCK4DMNEffud6QZW0C\n" \
"zyyRpqbn3oUYSXxmTqM6bam17jQuug0DuDPfR+uxa40l2ZvOgdFFRjKWcIfeAg5J\n" \
"Q4W2bHO7ZOphQazJ1FTfhy/HIrImzJ9ZVGif/L4qL8RVHHVAYBeFAlU5i38FAgMB\n" \
"AAGjgfAwge0wDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMCAYYwHQYDVR0O\n" \
"BBYEFJxfAN+qAdcwKziIorhtSpzyEZGDMB8GA1UdIwQYMBaAFL9ft9HO3R+G9FtV\n" \
"rNzXEMIOqYjnME8GCCsGAQUFBwEBBEMwQTAcBggrBgEFBQcwAYYQaHR0cDovL28u\n" \
"c3MyLnVzLzAhBggrBgEFBQcwAoYVaHR0cDovL3guc3MyLnVzL3guY2VyMCYGA1Ud\n" \
"HwQfMB0wG6AZoBeGFWh0dHA6Ly9zLnNzMi51cy9yLmNybDARBgNVHSAECjAIMAYG\n" \
"BFUdIAAwDQYJKoZIhvcNAQELBQADggEBACMd44pXyn3pF3lM8R5V/cxTbj5HD9/G\n" \
"VfKyBDbtgB9TxF00KGu+x1X8Z+rLP3+QsjPNG1gQggL4+C/1E2DUBc7xgQjB3ad1\n" \
"l08YuW3e95ORCLp+QCztweq7dp4zBncdDQh/U90bZKuCJ/Fp1U1ervShw3WnWEQt\n" \
"8jxwmKy6abaVd38PMV4s/KCHOkdp8Hlf9BRUpJVeEXgSYCfOn8J3/yNTd126/+pZ\n" \
"59vPr5KW7ySaNRB6nJHGDn2Z9j8Z3/VyVOEVqQdZe4O/Ui5GjLIAZHYcSNPYeehu\n" \
"VsyuLAOQ1xk4meTKCRlb/weWsKh/NEnfVqn3sF/tM+2MR7cwA130A4w=\n" \
"-----END CERTIFICATE-----\n"; // Certificate here

const int ledPin = 18; // Pin for the LED
bool lastLedState = false; // Previous LED state

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length);

void setup() {
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT); // Set LED pin as output

    // Connect to WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
    }

    // Initialize WebSocket connection over SSL
    client.setCACert(rootCACertificate);
    webSocket.beginSSL(websocketServer, 443, "/");
    webSocket.onEvent(webSocketEvent);
}

void loop() {
    webSocket.loop(); // Keep WebSocket connection alive
}

// WebSocket event handler
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
    switch (type) {
        case WStype_CONNECTED:
            webSocket.sendTXT("ESP32 connected"); // Send connection confirmation
            break;

        case WStype_TEXT: {
            DynamicJsonDocument doc(256); // Reduced size
            if (deserializeJson(doc, (char*)payload) == DeserializationError::Ok) {
                bool activate = doc["activate"];
                bool ledState = doc["led"];
                
                // Only change state if activation is true and state is different
                if (activate && ledState != lastLedState) {
                    digitalWrite(ledPin, ledState ? HIGH : LOW); // Set LED state
                    lastLedState = ledState; // Update last state
                }
            }
            break;
        }
        
        case WStype_DISCONNECTED:
            break;

        default:
            break;
    }
}
