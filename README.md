# Especially-Safe 🔐  
Secure Wi-Fi Node for ESP32 Microcontrollers

This project demonstrates a **secure Wi-Fi node** built on the **ESP32** microcontroller from Espressif Systems, prioritizing **encrypted communication** with up to three potential layers of security:

### 🔐 Layers of Encryption
1. **WPA2/WPA3 Wi-Fi Network Encryption** *(recommended for most deployments)*  
2. **TLS/SSL Encryption** for secure data transfer using **AES-GCM** or **ChaCha20-Poly1305** (depending on server/client support)  
3. **Client Certificate Authentication** *(optional, not implemented in this code but can be added)*  

---

## 🚀 Features
- Connects to a Wi-Fi network using WPA2/WPA3 encryption  
- Establishes a secure HTTPS connection using:
  - `WiFiClientSecure` for encrypted communication  
  - Trusted **Root CA** to verify server identity  
- Performs optional **HTTP GET** to a verification server (`https://www.howsmyssl.com/`)  
- Visual indication via RGB **LED blinking** (optional)  

---

## 🧰 Hardware Requirements
- ESP32 microcontroller (any compatible board)  
- 3 LEDs (Red, Green, Blue)  
- Resistors (values based on LED specs)  
- Breadboard + Jumper wires  

---

## 💻 Software Requirements
- [Arduino IDE](https://www.arduino.cc/en/software)  
- [ArduinoJson](https://arduinojson.org/) *(optional, for parsing JSON responses)*  
- `WiFiClientSecure` *(included in Arduino IDE)*  

---

## ⚙️ Configuration

### 1. Wi-Fi Credentials
Update the following lines in your code with your network credentials:
```cpp
const char* ssid = "<WIFI_SSID>";
const char* password = "<WIFI_PW>";
```

### 2. Cloudflare DNS *(Optional)*
For better performance and added DNS-level security, use Cloudflare’s DNS:
```cpp
const char* dnsServerIP = "1.1.1.1";
WiFi.config(dnsServerIP);  // Set DNS server after WiFi connection
```

### 3. Server and Root CA
By default, the code uses **https://www.howsmyssl.com/**.  
You can change the server and root certificate as needed:
```cpp
const char* server = "www.howsmyssl.com";
const char* test_root_ca = R"EOF(
-----BEGIN CERTIFICATE-----
...
-----END CERTIFICATE-----
)EOF";
```

---

## 🔐 Encryption Algorithms Overview

- **Wi-Fi Encryption:** WPA2/WPA3 (usually AES-CCM or similar)  
- **TLS/SSL Encryption:** Typically uses **TLS 1.2+**, with:
  - **AES-GCM**
  - **ChaCha20-Poly1305**  
  *(actual algorithm depends on client-server negotiation)*  

---

## 📖 Code Breakdown

### 🔧 Includes & Definitions
- Essential libraries:
  ```cpp
  #include <WiFiClientSecure.h>
  #include <Wire.h>
  ```
- Constants for:
  - LED pins  
  - Screen size (if using OLED)  
  - Wi-Fi credentials  
  - Server details  

### 🖥️ OLED Display Setup *(Optional)*
If using an OLED screen, the code initializes it for visual feedback.

### 📶 Wi-Fi Connection
- Connects to Wi-Fi with secure WPA2/WPA3 encryption  
- Prints connection status to the Serial Monitor  

### 🔒 Secure Server Connection
- `WiFiClientSecure` handles encrypted communication  
- Root CA is used to validate server certificate  
- HTTPS connection established over port `443`  

### 🌐 HTTP GET Request *(Optional)*
- Sends GET request to `https://www.howsmyssl.com/a/check`  
- Can use ArduinoJson to parse the response  

### 🔴🟢🔵 LED Indication *(Optional)*
- Red, Green, and Blue LEDs blink in sequence  
- Useful for debugging or visual status  

---

## 🛡️ Security Verification
This setup has been **tested and verified using Wireshark logs** to confirm secure encryption across all network activity.  
Use of [HowsMySSL](https://www.howsmyssl.com/) further ensures TLS connection integrity.

---

## 📎 License
MIT License (or your preferred open-source license)

