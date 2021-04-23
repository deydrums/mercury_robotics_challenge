#include <ESP8266WiFi.h>

#define ENA   14          // Enable/speed motors Right        GPIO14(D5)
#define ENB   12          // Enable/speed motors Left         GPIO12(D6)
#define IN_1  15          // L298N in1 motors Right           GPIO15(D8)
#define IN_2  13          // L298N in2 motors Right           GPIO13(D7)
#define IN_3  5           // L298N in3 motors Left            GPIO2(D4)
#define IN_4  4           // L298N in4 motors Left            GPIO0(D3)
#define IN_5  10           // L298N in4 motors Left            GPIO0(D3)

String command;             //String to store app command state.
int speedCar = 800;         // 400 - 1023.
int speed_Coeff = 3;
 
#define SendKey 0  //Button to send data Flash BTN on NodeMCU
 
int port = 8888;  //Port number
WiFiServer server(port);
 
//Server connect to WiFi Network
const char *ssid = "WLAN";  //Enter your wifi SSID
const char *password = "ABCDE7485";  //Enter your wifi Password
 
int count=0;
//=======================================================================
//                    Power on setup
//=======================================================================
void setup() 
{
 pinMode(ENA, OUTPUT);
 pinMode(ENB, OUTPUT);  
 pinMode(IN_1, OUTPUT);
 pinMode(IN_2, OUTPUT);
 pinMode(IN_3, OUTPUT);
 pinMode(IN_4, OUTPUT); 
 pinMode(IN_5, OUTPUT); 
  Serial.begin(115200);
  pinMode(SendKey,INPUT_PULLUP);  //Btn to send data
  Serial.println();
 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); //Connect to wifi
 
  // Wait for connection  
  Serial.println("Connecting to Wifi");
  while (WiFi.status() != WL_CONNECTED) {   
    delay(500);
    Serial.print(".");
    delay(500);
  }
 
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
 
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  
  server.begin();
  Serial.print("Open Telnet and connect to IP:");
  Serial.print(WiFi.localIP());
  Serial.print(" on port ");
  Serial.println(port);
}
//=======================================================================
//                    Movimientos
//=======================================================================
void goAhead(){ 

      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(ENB, speedCar);
  }

void goBack(){ 

      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
  }

void goRight(){ 

      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(ENB, speedCar);
  }

void goLeft(){

      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
  }

void goAheadRight(){
      
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(ENA, speedCar/speed_Coeff);
 
      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(ENB, speedCar);
   }

void goAheadLeft(){
      
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(ENB, speedCar/speed_Coeff);
  }

void goBackRight(){ 

      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar/speed_Coeff);

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
  }

void goBackLeft(){ 

      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar/speed_Coeff);
  }

void stopRobot(){  

      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
 }



//=======================================================================
//                    Loop
//=======================================================================
 
//=======================================================================
//                    Loop
//=======================================================================
 
void loop() 
{
  WiFiClient client = server.available();
  
  if (client) {
    if(client.connected())
    {
      Serial.println("Client Connected");
    }
    
    while(client.connected()){      
      while(client.available()>0){
        // read data from the connected client
       // Serial.write(client.read()); 
       String command = client.readStringUntil('\r');
       Serial.println(command);
       client.flush();

  if (command.indexOf("A") != -1)
  {
  digitalWrite(IN_5, LOW);
  goLeft();
  }
  else if (command.indexOf("W") != -1)
  {
  digitalWrite(IN_5, LOW);
   goAhead();
  }
  else if (command.indexOf("S") != -1)
  {
  digitalWrite(IN_5, LOW);
   goBack();
  }
  else if (command.indexOf("D") != -1)
  {
  digitalWrite(IN_5, LOW);
   goRight();
  }
    else if (command.indexOf("0") != -1)
  {
   speedCar = 550;
  }
    else if (command.indexOf("1") != -1)
  {
   speedCar = 750;
  }
    else if (command.indexOf("2") != -1)
  {
   speedCar = 890;
  }
    else if (command.indexOf("3") != -1)
  {
   speedCar = 1023;
  }
  else if (command.indexOf("E") != -1)
  {
   stopRobot();
   digitalWrite(IN_5, HIGH);

  }
  else 
  {
    Serial.println("invalid request");
  }








       
      }
      //Send Data to connected client

    }
    client.stop();
    Serial.println("Client disconnected");    
  }
}
//=======================================================================
