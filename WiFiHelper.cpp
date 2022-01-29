#include "WifiHelper.h"
#include <SPI.h>
#include <WiFi101.h>

WiFiHelper::WiFiHelper() {

    lightDataRetrieved = false;
}
bool WiFiHelper::setupWifi() {
  //Configure pins for Adafruit ATWINC1500 Feather
  WiFi.setPins(8,7,4,2);
  // Initialize serial and wait for port to open:
  
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    return false;
  }

  // Start in provisioning mode:
  //  1) This will try to connect to a previously associated access point.
  //  2) If this fails, an access point named "wifi101-XXXX" will be created, where XXXX
  //     is the last 4 digits of the boards MAC address. Once you are connected to the access point,
  //     you can configure an SSID and password by visiting http://wifi101/
  
  WiFi.beginProvision();

  while (WiFi.status() != WL_CONNECTED) {
    // wait while not connected
    // blink the led to show an unconnected status
    ;
  }

  // you're connected now, so print out the data:
  Serial.println("You're connected to the network");
  printCurrentNet();
  printWiFiData();

  bonjourHelper.init();

  return true;

}



void WiFiHelper::updateElgatoLight(char* json){
  if (!bonjourHelper.isLightFound()) {
    return;
  }
     if (client.connect(bonjourHelper.getLightIP(), bonjourHelper.getPort())) {
        
        char response[500];
        sprintf(response, putRequestFormat, strlen(json), json);
#ifdef DEBUG        
        Serial.print(response);
#endif
        client.print(response);
        client.flush();
            // Wait for the data from the server
        while(client.connected() && !client.available()) {
            delay(1); //waits for data
        }
        Serial.println("Got update response");
        
    }
    client.stop();
    handleElgatoResponse();    
}

char* WiFiHelper::getElgatoLightData() {
  if (!bonjourHelper.isLightFound()) {
    return NULL;
  }

  if (client.connect(bonjourHelper.getLightIP(), bonjourHelper.getPort())) {
    // Make a HTTP request:
    client.println("GET /elgato/lights HTTP/1.1");
    client.println();

    if (!handleElgatoResponse()) {
      client.stop();
      return NULL;
    }
  }
  client.stop();
  return rawJson;
  
}

bool WiFiHelper::handleElgatoResponse() {

    // Wait for the data from the server
    while(client.connected() && !client.available()) {
        delay(1); //waits for data
    }
    Serial.println("Got Data!");

    // Create an array to hold the response
    int responseSize = client.available();
    if (responseSize == 0 || responseSize >MAX_JSON_SIZE)
    {
        // Something's not right, let's abort.
        return false;
    }

    if (!trimHttpHeaders()) {
        return false;
    }

    int availableData = client.available();
    Serial.print("AvailableData: ");
    Serial.println(availableData);
    if (availableData < MAX_JSON_SIZE)        
        client.readBytes(rawJson, availableData);
    Serial.println(rawJson);
    return true;
}

bool WiFiHelper::trimHttpHeaders() {
  // Check HTTP status
  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  Serial.print("Response: ");
  Serial.println(status);
  if (strcmp(status, "HTTP/1.1 200 OK") != 0) {
    Serial.println(F("Unexpected response!"));
    return false;
  }
  Serial.println(status);

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    Serial.println(F("Invalid response"));
    return false;
  }
  return true;
}



void WiFiHelper::printWiFiData() {
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP address : ");
  Serial.println(ip);

  Serial.print("Subnet mask: ");
  Serial.println((IPAddress)WiFi.subnetMask());

  Serial.print("Gateway IP : ");
  Serial.println((IPAddress)WiFi.gatewayIP());

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  printMacAddress(mac);
}

void WiFiHelper::printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  printMacAddress(bssid);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI): ");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type: ");
  Serial.println(encryption, HEX);
  Serial.println();
}

void WiFiHelper::printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}

