#include <SPI.h>
#include <Ethernet.h>

byte motionSensors[3]  = {5,6,7};
byte motionSensorsState[3] = {LOW,LOW,LOW};
char* motionSensorLocation[] = {"Basement Hallway", "Basement", "Living Room"};
byte numberOfMotionSensors = 3;
  
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress serverIP(192,168,0,214);
int serverPort=1213;

EthernetServer server(serverPort);
EthernetClient client;

void setup() {
  int i;
   
   Serial.begin(115200);
  
   Serial.println("Serial started");
   
   Ethernet.begin(mac,serverIP);
   Serial.print(F("My IP address: "));
   Ethernet.localIP().printTo(Serial);
   Serial.println();
   
   //Initialize all of the motion sensors as inputs
   for(i=0 ; i<numberOfMotionSensors ; i++) {
     pinMode(motionSensors[i],INPUT);
   }
   
  server.begin();
  
  notify("{'status':'System started'");
}

void loop () {  
  // listen for incoming clients  
  while (!client) {
    // wait for incoming connection
    client = server.available();
  }
  
  if (!client.connected()) {
    Serial.println("stopping client");
    client.stop();
  }
  
  checkMotionSensors();
}

void checkMotionSensors(){
  int i;
  char c[50];
  for (i=0 ; i < numberOfMotionSensors; i++) {
    if (checkMotionSensor(motionSensors[i]) != motionSensorsState[i]) {
      //Motion sensor state has changed
      motionSensorsState[i] = !motionSensorsState[i];
      strcpy(c,"{'");
      strcat(c, motionSensorLocation[i]); 
      //Serial.print(motionSensorsLocation[i]);
      if (motionSensorsState[i] == HIGH) {
            Serial.println("motion!");
            strcat(c,"':'1'}");
                 //Avviso.push("Alarm!", "Motion detected!", 0);  
      } else {
        //Serial.println(motionSensorsState[i]);  
         Serial.println("NoMotion!");
         strcat(c,"':'0'}");
      }
      notify(c);
    }
  }
  
}

void notify(char *msg) {
  Serial.println("notify called");
  if (client) {
    Serial.println("client detected");
    if (client.available()) {
      client.println(msg);
    }
  }   
}

int checkMotionSensor(byte sensorPin) {
   int value;
   value = digitalRead(sensorPin);
   
   return value;
}


