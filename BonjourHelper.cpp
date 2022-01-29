#include "BonjourHelper.h"

#define ELGATO_SERVICE_NAME "_elg"

void serviceFound(const char*, MDNSServiceProtocol,const char*, const byte[4],unsigned short,const char*);

BonjourHelper::BonjourHelper()
{
    lightFound = false;

}

void BonjourHelper::init(){
  // Initialize the Bonjour/MDNS library. You can now reach or ping this
  // Arduino via the host name "arduino.local", provided that your operating
  // system is Bonjour-enabled (such as MacOS X).
  // Always call this before any other method!
  EthernetBonjour.begin("arduino");

  // We specify the function that the Bonjour library will call when it
  // discovers a service instance. In this case, we will call the function
  // named "serviceFound".
  EthernetBonjour.setServiceFoundCallback(serviceFound);

}
void BonjourHelper::loop() {
    if (!lightFound)
    {
        if(!EthernetBonjour.isDiscoveringService()) {

            // Now we tell the Bonjour library to discover the service. Below, I have
            // hardcoded the TCP protocol, but you can also specify to discover UDP
            // services.
            // The last argument is a duration (in milliseconds) for which we will
            // search (specify 0 to run the discovery indefinitely).
            // Note that the library will resend the discovery message every 10
            // seconds, so if you search for longer than that, you will receive
            // duplicate instances.

            EthernetBonjour.startDiscoveringService(ELGATO_SERVICE_NAME,
                                                    MDNSServiceTCP,
                                                    1000);
                
        }
    }
    EthernetBonjour.run();
}

IPAddress BonjourHelper::getLightIP(){
    return this->lightIp;
}
unsigned short BonjourHelper::getPort(){
    return this->port;
}

// This function is called when a name is resolved via MDNS/Bonjour. We set
// this up in the setup() function above. The name you give to this callback
// function does not matter at all, but it must take exactly these arguments
// as below.
// If a service is discovered, name, ipAddr, port and (if available) txtContent
// will be set.
// If your specified discovery timeout is reached, the function will be called
// with name (and all successive arguments) being set to NULL.
void serviceFound(const char* type, MDNSServiceProtocol proto,
                  const char* name, const byte ipAddr[4],
                  unsigned short port,
                  const char* txtContent)
{
  if (NULL == name) {
	Serial.print("Finished discovering services of type ");
	Serial.println(type);
  } 
  else {
    Serial.print("Found: '");
    Serial.print(name);
    Serial.print("' at ");
    Serial.print(ip_to_str(ipAddr));
    Serial.print(", port ");
    Serial.print(port);
    Serial.println(" (TCP)");

    bonjourHelper.lightFound = true;
    bonjourHelper.lightIp = ipAddr;
    bonjourHelper.port = port;
    
    // Check out http://www.zeroconf.org/Rendezvous/txtrecords.html for a
    // primer on the structure of TXT records. Note that the Bonjour
    // library will always return the txt content as a zero-terminated
    // string, even if the specification does not require this.
    if (txtContent) {
      Serial.print("\ttxt record: ");
      
      char buf[256];
      char len = *txtContent++, i=0;;
      while (len) {
        i = 0;
        while (len--)
          buf[i++] = *txtContent++;
        buf[i] = '\0';
        Serial.print(buf);
        len = *txtContent++;
        
        if (len)
          Serial.print(", ");
        else
          Serial.println();
      }
    }
  }
}

// This is just a little utility function to format an IP address as a string.
const char* ip_to_str(const uint8_t* ipAddr)
{
  static char buf[16];
  sprintf(buf, "%d.%d.%d.%d\0", ipAddr[0], ipAddr[1], ipAddr[2], ipAddr[3]);
  return buf;
}

BonjourHelper bonjourHelper;
