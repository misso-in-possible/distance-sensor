
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "RouteInn-Free";
const char* password = "routeinn";

const int echoPin = 22;
const int trigPin = 23;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");

  pinMode(echoPin, INPUT);               // Set echo pin as input
  pinMode(trigPin, OUTPUT);              // Set trig pin as output
}

void loop() {
  int distance = (int) readDistance();  // Call the function to read the sensor data and get the distance
  Serial.print(distance);           // Print the distance value
  Serial.println(" cm");            // Print " cm" to indicate the unit of measurement
  Serial.print(String(distance));
  Serial.println("string cm");
 
  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;

    http.begin("https://misso-dev.h1rose.com/distance-sensor");  //curlコマンドと同じURi『http://192.168.0.100/datastore/ext/obank/2/ch/0/name 』
    //http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(String(distance));  //curlコマンドと同じ内容のJSON『{\"value\":\"My favorite channel\"} 』
    if (httpResponseCode > 0) {

      String response = http.getString();

      Serial.println(httpResponseCode);
      Serial.println(response);

    } else {

      Serial.print("Error on sending PUT Request: ");
      Serial.println(httpResponseCode);

    }

    http.end();

  } else {
    Serial.println("Error in WiFi connection");
  }

  delay(500);
}

float readDistance() {
  digitalWrite(trigPin, LOW);   // Set trig pin to low to ensure a clean pulse
  delayMicroseconds(2);         // Delay for 2 microseconds
  digitalWrite(trigPin, HIGH);  // Send a 10 microsecond pulse by setting trig pin to high
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);  // Set trig pin back to low

  // Measure the pulse width of the echo pin and calculate the distance value
  float distance = pulseIn(echoPin, HIGH) / 58.00;  // Formula: (340m/s * 1us) / 2
  return distance;
}
