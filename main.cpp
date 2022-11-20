#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <ezTime.h>
#include <HTTPClient.h>

const int trigPin = 32;
const int echoPin = 33;

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

const char* ssid = "homeserver";
const char* password = "homeserver";


long duration;
float distanceCm;
float distanceInch;
int teller = 0;

String HOST_NAME = "http://192.168.0.158";
String PATH_NAME = "/insert_depth.php";
String meetingString;
String diepteString;



void connectWiFi(){
  delay(10);
  //connecting to a WiFi network
  Serial.print(F("Connecting to "));
  Serial.print(ssid);
  WiFi.mode(WIFI_STA); //Station mode
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(250);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print(F("WiFi connected on IP address"));
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(9600); // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  connectWiFi();
}

HTTPClient http;

void loop() {
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
  distanceCm = duration * SOUND_SPEED/2;
  
  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  teller += 1;
  meetingString = "?meeting=" + String(teller);
  diepteString = "&diepte=" + String(distanceCm);

  http.begin(HOST_NAME + PATH_NAME + meetingString + diepteString);
  int httpCode = http.GET();

  if(httpCode > 0){
    if(httpCode == HTTP_CODE_OK){
      String payload = http.getString();
      Serial.println(payload);
    }
    else{
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    }
  }
  else{
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();
  
  //delay(21600000);
  delay(1000);
  meetingString = "";
  diepteString = "";
}