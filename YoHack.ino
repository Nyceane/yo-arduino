#include <M2XStreamClient.h>
#include <stdlib.h>
#include <jsonlite.h>
#include <WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

// create an object of the IoTkit class
//IoTkit iotkit;        


//M2X stuff
byte mac[] = { 0x91, 0xA2, 0xDA, 0x0E, 0xF9, 0xC9 };
IPAddress ip(192,168,1,182);

char m2xKey[] = "5a59666eddc8c86c3f1fdd39729c3050";
char feedId[] = "731eca7c84d4535387c3d7c2dd7004f4";

const char *streamNames[] = { "temperature", "infrared" };
const char *temp = "temperature";
const char *infrared = "infrared";

char locationname[] = "currentcar"; // Name of current location of datasource
double latitude = 37.7706259;
double longitude = -122.4306477; // You can also read those values from a GPS
double elevation = 15;

int counts[] = { 1, 1 };
const char *ats[] = { NULL, NULL, NULL };


WiFiClient client;
M2XStreamClient m2xClient(&client, m2xKey);
//end M2X stuff

//char ssid[] = "WickedlyFastWiFi";     //  your network SSID (name) 
char ssid[] = "YoHack";     //  your network SSID (name) 
char pass[] = "yoyohack";    // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

//WiFiServer server(80);

boolean hasIf = false;

int stage = 0;
int count = 0;
int demo = 0;

char callClientServer[] = "www.prototyper.co";
char smsClientServer[] = "test.spotvite.com";

//int ledPin = 13; // choose the pin for the LED
int inPin = 7;   // choose the input pin (for a pushbutton)
int val = 0;     // variable for reading the pin status

void setup() {
  
  //Initialize serial and wait for port to open:
  Serial.begin(9600); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    // don't continue:
    while(true);
  } 
  
  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid, pass);
//    status = WiFi.begin(ssid);

    // wait 10 seconds for connection:
    delay(15000);
  }   
  
  
//  pinMode(ledPin, OUTPUT);  // declare LED as output
  pinMode(inPin, INPUT);    // declare pushbutton as input

  
//  server.begin();
  // you're connected now, so print out the status:
  printWifiStatus();
//  iotkit.begin();
  
      // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    
    lcd.setRGB(0, colorG, colorB);
    
    // Print a message to the LCD.
    lcd.print("CarSense");

  //printing temperature
//  MsTimer2::set(5000, printTemperature); // 1 second period
//  MsTimer2::start();
}

void loop() {
  val = digitalRead(inPin);  // read input value
  if (val == HIGH) {         // check if the input is HIGH (button released)
    stage = 0;
    demo = 0;
    Serial.println("reset");
          lcd.setRGB(0, colorG, colorB);

  }

  int IF = analogRead(A0);
  if(demo == 0)
  {
    if(IF > 0)
    {
       hasIf = true;
  //    digitalWrite(ledPin, HIGH);  // turn LED ON
      lcd.print("IF Detected");
        Serial.println("IF Detected");
    }
    else
    {
      hasIf = false;
      stage = 0;
  //    digitalWrite(ledPin, LOW);  // turn LED OFF
      lcd.setCursor(0, 0);
      lcd.print("IF Not Detected");
        Serial.println("IF Not Detected");
  
    }
  }
  else
  {
    hasIf = true;
    lcd.setCursor(0, 0);
    lcd.print("IF Detected");
  }
  
  hasIf = true;
  
  Serial.print("pin:");
  Serial.println(val);
  
  Serial.print("IF:");
  Serial.println(analogRead(A0));

  Serial.print("stage:");
  Serial.println(stage);
  
  if((count % 5) == 0)
  {
    float Celcius = (5.0 * analogRead(A1) * 100.0) / 1024;
    float Fahrenheit = (Celcius * 9.0 / 5.0) + 32.0;
    char buffer[10];
    String tmp = dtostrf(Fahrenheit, 2, 2, buffer);
    String temperature = (tmp);
    Serial.println("temperature: " + temperature + " F");
  
    //Infrared Sensor
//    Serial.println(hasIf);
  
  
    int fresponse = m2xClient.post(feedId, temp, Fahrenheit);
    Serial.print("M2x temperature client response code for temp: ");

    Serial.println(fresponse);
    
    if(IF > 0 || stage > 0)
    {
      int fresponse1 = m2xClient.post(feedId, infrared, 1);
      Serial.print("M2x temperature client response code for infrared: ");
      Serial.println(fresponse1);    
    }
    else
    {
      int fresponse2 = m2xClient.post(feedId, infrared, 0);
      Serial.print("M2x temperature client response code for infrared: ");
      Serial.println(fresponse2);    
    }

    
    
        // set the cursor to column 0, line 1
    // (note: line 1 is the second row, since counting begins with 0):
    lcd.setCursor(0, 1);
    lcd.print("temp: " + temperature + " F");

        //sending to intel
//    iotkit.send("timevstemperature", Fahrenheit);  
//    iotkit.send("timevsif", hasIf);  
    
    //calculate stage here
    if(Fahrenheit > 100)
    {
      demo = 1;
      stage++;
    }
    else if(demo == 1)
    {
      stage++;
    }
  
    if(stage == 1)
    {
      if (client.connect(callClientServer, 80)) {
          Serial.println("connected");
          Serial.println("GET http://www.prototyper.co/wp-content/demos/yo/alert.php HTTP/1.0");
          client.println("GET http://www.prototyper.co/wp-content/demos/yo/alert.php HTTP/1.0");
          client.println("Host: www.prototyper.co"); // OK -> It solves the Arduino sending the query to abc.com instead of abc.com IP.

          client.println();
          stage++;
          
          while(client.connected() && !client.available()) delay(1); //waits for data
          while (client.connected() && client.available()) { //connected or data available
            char c = client.read(); //gets byte from ethernet buffer
            Serial.print(c); //prints byte to serial monitor 
          }
          Serial.println();
          Serial.println("disconnecting.");
          Serial.println("==================");
          Serial.println();
          client.stop(); //stop client
        } else {

          Serial.println("connection failed");
        } 
    }
    else if(stage == 3)
    {
      if (client.connect(callClientServer, 80)) {
          Serial.println("connected");
          Serial.println("GET http://www.prototyper.co/wp-content/demos/yo/alert.php HTTP/1.0");
          client.println("GET http://www.prototyper.co/wp-content/demos/yo/alert.php HTTP/1.0");
          client.println("Host: www.prototyper.co"); // OK -> It solves the Arduino sending the query to abc.com instead of abc.com IP.

          client.println();
          stage++;
          
          while(client.connected() && !client.available()) delay(1); //waits for data
          while (client.connected() && client.available()) { //connected or data available
            char c = client.read(); //gets byte from ethernet buffer
            Serial.print(c); //prints byte to serial monitor 
          }
          Serial.println();
          Serial.println("disconnecting.");
          Serial.println("==================");
          Serial.println();
          client.stop(); //stop client
        } else {

          Serial.println("connection failed");
        } 
      }
      else if(stage == 7)
    {
            if (client.connect(callClientServer, 80)) {
          Serial.println("connected");
          Serial.println("GET http://www.prototyper.co/wp-content/demos/yo/alertcallback.php HTTP/1.0");
          client.println("GET http://www.prototyper.co/wp-content/demos/yo/alertcallback.php HTTP/1.0");
          client.println("Host: www.prototyper.co"); // OK -> It solves the Arduino sending the query to abc.com instead of abc.com IP.

          client.println();
          stage++;
          
          while(client.connected() && !client.available()) delay(1); //waits for data
          while (client.connected() && client.available()) { //connected or data available
            char c = client.read(); //gets byte from ethernet buffer
            Serial.print(c); //prints byte to serial monitor 
          }
          Serial.println();
          Serial.println("disconnecting.");
          Serial.println("==================");
          Serial.println();
          client.stop(); //stop client
        } else {

          Serial.println("connection failed");
        } 
       /*
      if (client.connect(callClientServer, 80)) {
          Serial.println("connected");
          Serial.println("GET /wp-content/demos/outgoing/emergency.php?temperature=" + temperature + " HTTP/1.0");
          client.println("GET /wp-content/demos/outgoing/emergency.php?temperature=" + temperature + " HTTP/1.0");
          client.println("Host: www.prototyper.co"); // OK -> It solves the Arduino sending the query to abc.com instead of abc.com IP.

          client.println();
          stage++;
          
          while(client.connected() && !client.available()) delay(1); //waits for data
          while (client.connected() && client.available()) { //connected or data available
            char c = client.read(); //gets byte from ethernet buffer
            Serial.print(c); //prints byte to serial monitor 
          }
          Serial.println();
          Serial.println("disconnecting.");
          Serial.println("==================");
          Serial.println();
          client.stop(); //stop client
        } else {

          Serial.println("connection failed");
        } 
        */
      }
    }
    
    count++;
    delay(1000);
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  
    // check firmware version
  Serial.print("Firmware version: ");
  Serial.println(WiFi.firmwareVersion());
}

char *dtostrf (double val, signed char width, unsigned char prec, char *sout) {
  char fmt[20];
  sprintf(fmt, "%%%d.%df", width, prec);
  sprintf(sout, fmt, val);
  return sout;
}
