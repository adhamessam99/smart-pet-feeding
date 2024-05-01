#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <time.h>
#include <TZ.h>
#include <FS.h>
#include <LittleFS.h>
#include <CertStoreBearSSL.h>
const int trigPin = 12;
const int echoPin = 14;
int button = 16; // push button is connected
int buttonPushed =0;

//define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
//const char* conf="container is empty need to refill";

#include <Servo.h> // servo library  
int angle =90;    // initial angle  for servo (beteen 1 and 179)
int angleStep =10;
const int minAngle = 0;
const int maxAngle = 180;
const int type =2;
 
 Servo s1; 
const char* message="container is  not empty";
const char* ssid = "Abdo iphone";
const char* password = "abdo55555";
const char* mqtt_server = "ae22b2dd89f2466cac9dda2003ef03af.s2.eu.hivemq.cloud";

BearSSL::CertStore certStore;
WiFiClientSecure espClient;
PubSubClient * client;
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (500)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setDateTime() {
  configTime(TZ_Europe_Berlin, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(100);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println();

  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.printf("%s %s", tzname[0], asctime(&timeinfo));
}

void reconnect() {
  while (!client->connected()) {
    Serial.print("Attempting MQTT connection…");
    String clientId = "ESP8266Client - MyClient";
    if (client->connect(clientId.c_str(), "Device001", "Device001")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc = ");
      Serial.print(client->state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  pinMode(button, INPUT); // declare push button as input
  Serial.begin(115200); // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  delay(500);
  Serial.begin(9600);
  delay(500);
  s1.attach(0);  // servo attach D3 pin of arduino  
  LittleFS.begin();
  setup_wifi();
  setDateTime();

  pinMode(LED_BUILTIN, OUTPUT);

  int numCerts = certStore.initCertStore(LittleFS, PSTR("/certs.idx"), PSTR("/certs.ar"));
  Serial.printf("Number of CA certs read: %d\n", numCerts);
  if (numCerts == 0) {
    Serial.printf("No certs found. Did you run certs-from-mozilla.py and upload the LittleFS directory before running?\n");
    return;
  }

  BearSSL::WiFiClientSecure *bear = new BearSSL::WiFiClientSecure();
  bear->setCertStore(&certStore);

  client = new PubSubClient(*bear);
  client->setServer(mqtt_server, 8883);
}

void loop() {
  if(digitalRead(button) == LOW){
    buttonPushed = 1;
  }
   if( buttonPushed ){
  // change the angle for next time through the loop:
  angle = angle + angleStep;}

    // reverse the direction of the moving at the ends of the angle:
    if (angle >= maxAngle) {
      angleStep = -angleStep;
        if(type ==1)
        {
            buttonPushed =0;                   
        }
    }
    
    if (angle <= minAngle) {
      angleStep = -angleStep;
       if(type ==2)
        {
            buttonPushed =0;       
        }
    }
    
    s1.write(angle); // move the servo to desired angle
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_VELOCITY/2;
  
  if(distanceCm>=18)
  {
    message="container is empty";
  }
  else
  {message="container Is Not empty";}
    
    
    
    
  // Prints the distance on the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  
  
  
  delay(1000);
  
  if (!client->connected()) {
    reconnect();
  }
  client->loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, MSG_BUFFER_SIZE, message,value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client->publish("testTopic", msg);
  }
}
