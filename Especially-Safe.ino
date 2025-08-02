#include <WiFiClientSecure.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DNSServer.h>


const char* dnsServerIP = "1.1.1.1"; // Google DNS Server
const int dnsServerPort = 443; // DNS-over-HTTP (DoH) port

const char* dohServer = "dns.cloudflare";
const char* dohPath = "/resolve";
DNSServer dnsServer;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 oled(SCREEN_WIDTH,SCREEN_HEIGHT, &Wire, -1);

#define PIN_RED    25   // Led red pin is connected to 23
#define PIN_GREEN  33   // Led green pin is connected to 22
#define PIN_BLUE   32 

const char* ssid     = "HIMU";     // your network SSID (name of wifi network)
const char* password = "12345678910"; // your network password

const char*  server = "www.howsmyssl.com";  // Server URL

// www.howsmyssl.com root certificate authority, to verify the server
// change it to your server root CA
// SHA1 fingerprint is broken now!

const char* test_root_ca= \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDuzCCAqOgAwIBAgIQF5lWz9/TKoQTvShHZ04+OjANBgkqhkiG9w0BAQsFADBI\n" \
"MRswGQYDVQQDDBJFU0VUIFNTTCBGaWx0ZXIgQ0ExHDAaBgNVBAoME0VTRVQsIHNw\n" \
"b2wuIHMgci4gby4xCzAJBgNVBAYTAlNLMB4XDTI0MDEzMTIzMTkxM1oXDTI0MDQz\n" \
"MDIzMTkxMlowHDEaMBgGA1UEAxMRd3d3Lmhvd3NteXNzbC5jb20wggEiMA0GCSqG\n" \
"SIb3DQEBAQUAA4IBDwAwggEKAoIBAQDd7PCfxMHKaWLVIphw3uYEWVPfA5OKdn9V\n" \
"og2y6NiP0ds3mDuwg3Frf1adLd0f7mLUCl27VP092W0pGjLZ3mkOSBOU85VzNZMB\n" \
"fjCUFVRkEdlpXEoUZIyfGur4udBbka7sO2wj/pZ4+VNRMgGqB5TM1bbcy4vCzW+Z\n" \
"IFRN2zi/gHfhAjY6nB1KacyMlhm/cYn86strHmII5H/onKvwqob/2O2x9IxJ58zV\n" \
"OwBHQvfiLFobM7iDCFDmB2baIR5j5MdCYdYij11cfNYOT1jwZgUAtpp7i/rOKZIh\n" \
"sFMKiw6vsvlyFx2pA/A5PAVc7maEMEpCanri7ZValkRm2pdiaMm7AgMBAAGjgcww\n" \
"gckwCwYDVR0PBAQDAgWgMB0GA1UdDgQWBBQIdkMHRf9ATvXOQrdufGTF0j4kADAf\n" \
"BgNVHSMEGDAWgBQjvVH72pPmppaVIk9jJDLbtkMl3TAdBgNVHSUEFjAUBggrBgEF\n" \
"BQcDAQYIKwYBBQUHAwIwDAYDVR0TAQH/BAIwADBNBgNVHREERjBEgg1ob3dzbXlz\n" \
"c2wuY29tgg1ob3dzbXl0bHMuY29tghF3d3cuaG93c215c3NsLmNvbYIRd3d3Lmhv\n" \
"d3NteXRscy5jb20wDQYJKoZIhvcNAQELBQADggEBAJDBok1o/QklI/BaXSH0rtmv\n" \
"qWnBVGjTA9mOpu71LC4Fg2ujY/V7eeqdCq4esGQUbeoGiSmajs5Oh6/iFlfYNSAg\n" \
"VU//y9B7I3GHLtWOvieJLR++myT9aYb54sQU61JCsm8UWR5PRb1m9wWsYYbZOCnX\n" \
"FbvHbYo/sbfH0R8ssRo4dOcsqkPQWSy7Mqoo9UaZCcJFiq4aUFwSC1YGUvqNs53f\n" \
"raHBA5gQVjxENf4ehTohcA86a5JAIgWve6GW6LzMCWCyrslnGkTEC/Uigi8CncEf\n" \
"67zrnfmWFPYQfhM0blAXNN0zQ6vvGKXpL+9CHpB+1Azn9rr7w2sTmq7Yb8UtHTo=\n" \
"-----END CERTIFICATE-----\n";
 
// You can use x.509 client certificates if you want
//const char* test_client_key = "";   //to verify the client
//const char* test_client_cert = "";  //to verify the client
WiFiClientSecure client;
void setup() {
  // Initialize serial and wait for the port to open:
  Serial.begin(115200);
  delay(100);

  pinMode(PIN_RED,   OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE,  OUTPUT);
  
  if(!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("Failed to start SSD1306 OLED"));
    while(1);
  }
  delay(2000);

  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(WHITE);
  oled.setCursor(0,10);
  oled.println("Connection Secure");
  oled.display();


  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // Attempt to connect to WiFi network:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    // Wait 1 second before retrying
    delay(1000);
  }

  Serial.print("Connected to ");
  Serial.println(ssid);

  client.setCACert(test_root_ca);

  Serial.println("\nStarting connection to server...");
  if (!client.connect(server, 443)) {
    Serial.println("Connection failed!");

  } else {
    Serial.println("Connected to server!");
    // Make an HTTP request:
    client.println("GET /a/check HTTP/1.1");
    client.println("Host: www.howsmyssl.com");
    client.println("Connection: close");
    client.println();

    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        Serial.println("Headers received");
        break;
      }
    }
    // If there are incoming bytes available from the server, read and print them:
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }

    client.stop();
  }
}

void loop() {

    // Red color code
  analogWrite(PIN_RED,   255);
  analogWrite(PIN_GREEN, 0);
  analogWrite(PIN_BLUE,  0);
  delay(1000); // Keep the color for 1 second

  // Green color code
  analogWrite(PIN_RED,   0);
  analogWrite(PIN_GREEN, 0);
  analogWrite(PIN_BLUE,  255);
  delay(1000); // Keep the color for 1 second

  // Blue color code
  analogWrite(PIN_RED,   0);    
  analogWrite(PIN_GREEN, 255);
  analogWrite(PIN_BLUE,  0);
  delay(1000); // Keep the color for 1 second
}
