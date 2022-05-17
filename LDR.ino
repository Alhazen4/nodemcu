#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Servo variables
Servo myservo;

const int pinLED = 7;

// PIR variables
const int pinPIR = 2;
int state = LOW;
int val = 0;

// NodeMCU variables
const char* ssid = "Alfi Agil";
const char* pass = "Alfi1Agiel2";
ESP8266WebServer server(80);
uint8_t ledMCU = D5;
bool statusLED = LOW;

void setup()
{
	lcd.begin();
  Serial.begin(9600);
  
  pinMode(pinLED, OUTPUT);
  pinMode(pinPIR, INPUT);

  myservo.attach(D2);

    Serial.begin(115200);
    delay(100);
    pinMode(ledMCU, OUTPUT);

    Serial.println("Connecting ");
    Serial.println(ssid);
    WiFi.begin("Alfi Agil", "Alfi1Agiel2");
    
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(1000);
      Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());

    server.on("/", tersambung);
    server.on("/ledmenyala", LED_menyala);
    server.on("/ledmati", LED_mati);
    server.onNotFound(tidak_ditemukan);
    server.begin();
    Serial.println("Starting HTTP server");
}

void loop()
{
  server.handleClient();
  if(statusLED)
  {
    digitalWrite(ledMCU, HIGH);
    myservo.write(90);
  } else {
    digitalWrite(ledMCU, LOW);
    myservo.write(-90);
  }

  
  int motion = digitalRead(2);
  if(motion)
  {
//    Serial.println("Motion Detected");
    digitalWrite(9, HIGH);
    lcd.setCursor(0,0);
    lcd.print("Motion Detect!");
//     myservo.write(90);
  }
  else
  {
//    Serial.println("=== Nothing moves");
    digitalWrite(9, LOW);
    lcd.setCursor(0,0);
    lcd.print("No Motion!");
//    myservo.write(-90);
  }
  delay(100);
}

void tersambung()
{
  statusLED = LOW;
  server.send(200, "text/html", kirimHTML(statusLED));
}

void LED_menyala()
{
  statusLED = HIGH;
  server.send(200, "text/html", kirimHTML(statusLED));
}

void LED_mati() {
  statusLED = LOW;
  server.send(200, "text/html", kirimHTML(statusLED)); 
}

void tidak_ditemukan(){
  server.send(404, "text/plain", "Halaman tidak ditemukan");
}

// Fungsi untuk mengemas kode HTML yang dikembalikan sebagai string
String kirimHTML(uint8_t statusLED){
  String konten = "<html>\n";
  konten +="<head>\n";
  konten +="<title>LED Control</title>\n";
  konten +="</head>\n";
  konten +="<body>\n";
  konten +="<h1 style=\"font-size:100px\">Mengendalikan LED</h1>\n";
  
  if(statusLED){
   konten +="<p style=\"font-size:80px\">Status LED: ON</p>\n";
   konten +="<a href=\"/ledmati\">\n";
   konten +="<button type=\"button\" style=\"font-size:150px\">OFF</button>\n";
   konten +="</a>\n";}
  else
  {konten +="<p style=\"font-size:80px\">Status LED: OFF</p>\n";
   konten +="<a href=\"/ledmenyala\">\n";
   konten +="<button type=\"button\" style=\"font-size:150px\">ON</button>\n";
   konten +="</a>\n";
  }

  konten +="</body>\n";
  konten +="</html>\n";
  return konten;
}
