#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <Adafruit_NeoPixel.h>

// Pin to use to send signals to WS2812B
#define LED_PIN 4

// Number of WS2812B LEDs attached to the Arduino
#define LED_COUNT 90

// Setting up the NeoPixel library
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

#define LED2  2 
#define LED12  12    // LED12
#define LED14  14    // LED14


int SOLVED_LED[54] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 5, 5, 5, 4, 4, 4, 0, 0, 0, 3, 3, 3, 5, 5, 5, 4, 4, 4, 0, 0, 0, 3, 3, 3,5, 5, 5, 4, 4, 4, 0, 0, 0, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1};

int Current_LED[54] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 5, 5, 5, 4, 4, 4, 0, 0, 0, 3, 3, 3, 5, 5, 5, 4, 4, 4, 0, 0, 0, 3, 3, 3,5, 5, 5, 4, 4, 4, 0, 0, 0, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1};
int Set_LED[54] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 5, 5, 5, 4, 4, 4, 0, 0, 0, 3, 3, 3, 5, 5, 5, 4, 4, 4, 0, 0, 0, 3, 3, 3,5, 5, 5, 4, 4, 4, 0, 0, 0, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1};

int LED_Map[54] = {0, 2, 4,  5, 7, 9,  10, 12, 14, 
15, 17, 19,  20, 22, 24,  25, 27, 29,  30, 32, 34,
35, 37, 39,  40, 42, 44,  45, 47, 49,  50, 52, 54,
55, 57, 59,  60, 62, 64,  65, 67, 69,  70, 72, 74,
75, 77, 79,  80, 82, 84,  85, 87, 89};

const char* ssid     = "Rubiks_Cube";
const char* password = "123456789";

WiFiServer server(80); // Port 80


String blank = "";
int wait30 = 30000; // time to reconnect when connection is lost.

bool standby = 1;


int tempLED;
int mapped_LED;
int tempArraySwap = 0;
int buttonPressed = 0;
int button_Pressed = 0;
bool sent = 0;
int temp = 0;
int randNumber = 0;

bool Button_1;
bool Button_2;
bool Button_3;
bool Button_4;
bool Button_5;
bool Button_6;
bool Button_7;
bool Button_8;
bool Button_9;
bool Button_10;
bool Button_11;
bool Button_12;

void setup() {
  Serial.begin(115200);
  strip.begin();           // Initialize NeoPixel object
  strip.setBrightness(10);

  randomSeed(analogRead(5));
  // Conecta a la red wifi.
  Serial.println();
  Serial.print("Setting Access Point:  ");
  Serial.println(ssid);
 
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();

  // Esta es la IP
  Serial.print("This is IP to connect to the WebServer: ");
  Serial.print("http://");
  Serial.println(myIP);

  // Start Web Server.
  server.begin();
  Serial.println("Web Server started.");
  
}

void copyC(int* src, int* dst, int len) {
    memcpy(dst, src, sizeof(src[0])*len);
}

int waitForButton(){
  buttonPressed = 0;
  sent = 0;

  while(sent == 0){
    WiFiClient client = server.available();
    if (!client) {
      return 99;
    }
   
    // Serial.print("New client: ");
    // Serial.println(client.remoteIP());

    /////////////////////////////////////////////////////
    // Read the information sent by the client.
    String req = client.readStringUntil('\r');
    //Serial.print(req);
    if (req.indexOf("B1") != -1) {
      buttonPressed = 1;
    }
    else if (req.indexOf("B2") != -1) {
      buttonPressed = 2;
    }
    else if (req.indexOf("B3") != -1) {
      buttonPressed = 3;
    }
    else if (req.indexOf("B4") != -1) {
      buttonPressed = 4;
    }
    else if (req.indexOf("B5") != -1) {
      buttonPressed = 5;
    }
    else if (req.indexOf("B6") != -1) {
      buttonPressed = 6;
    }
    else if (req.indexOf("B7") != -1) {
      buttonPressed = 7;
    }
    else if (req.indexOf("B8") != -1) {
      buttonPressed = 8;
    }
    else if (req.indexOf("B9") != -1) {
      buttonPressed = 9;
    }
    else if (req.indexOf("Z10") != -1) {
      buttonPressed = 10;
    }
    else if (req.indexOf("Z11") != -1) {
      buttonPressed = 11;
    }
    else if (req.indexOf("Z12") != -1) {
      buttonPressed = 12;
    }
    else if (req.indexOf("Shuffle") != -1) {
      buttonPressed = 77;
    }
    else if (req.indexOf("Solve") != -1) {
      buttonPressed = 88;
    }

    sent = 1;

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println(""); 
    client.println(""); 

    client.flush();
    //delay(500);
    client.stop();
    //Serial.println("Client disconnected.");
    return buttonPressed;
  }


}

void setLEDS(){
  // 0 - Red - 255 0 0
  // 1 - Blue - 0 0 255
  // 2 - Green - 0 255 0
  // 3 - White - 150 150 150 
  // 4 - Yellow - 255 255 0
  // 5 - Purple - 50 20 50

  for (int i = 0; i < 54; i++) {
    tempLED = Current_LED[i];
    mapped_LED = LED_Map[i];    

    switch(tempLED){
      case 0:
        strip.setPixelColor(mapped_LED, 255, 0, 0);
        strip.show();
        break;

      case 1:
        // Serial.println(tempLED);
        // Serial.println(temp);
        //leds[2*i] = CRGB(0, 0, 100);
        strip.setPixelColor(mapped_LED, 0, 0, 255);
        strip.show();
        break;

      case 2:
        //leds[2*i] = CRGB(0, 100, 0);
        strip.setPixelColor(mapped_LED, 0, 255, 0);
        strip.show();
        break;

      case 3:
        //leds[2*i] = CRGB(75, 75, 75);
        strip.setPixelColor(mapped_LED, 255, 255, 255);
        strip.show();
        break;

      case 4:
        //leds[2*i] = CRGB(100, 100, 0);
        strip.setPixelColor(mapped_LED, 255, 255, 0);
        strip.show();
        break;

      case 5:
        //leds[2*i] = CRGB(50, 20, 50);
        strip.setPixelColor(mapped_LED, 200, 50, 200);
        strip.show();
        break;

      default:
        break;
    }
  }
  return;
}

void shuffleCube(){
  // 0 - Red - 255 0 0
  // 1 - Blue - 0 0 255
  // 2 - Green - 0 255 0
  // 3 - White - 150 150 150 
  // 4 - Yellow - 255 255 0
  // 5 - Purple - 50 20 50
  for(int i = 0; i < 1000; i++)
  {
    randNumber = random(0, 13);
    rotateFace(randNumber);
  }
}

void rotateFace(int buttonNumber){
  switch(buttonNumber){
      case 1:
        rotate_White_CW();
         break;

      case 2:
        rotate_White_CCW();
         break;

      case 3:
        rotate_Yellow_CW();
         break;

      case 4:
        rotate_Yellow_CCW();
         break;

      case 5:
        rotate_Purple_CW();
         break;

      case 6:
        rotate_Purple_CCW();
         break;
      
      case 7:
        rotate_Red_CW();
         break;

      case 8:
        rotate_Red_CCW();
         break;

      case 9:
        rotate_Green_CW();
         break;
      
      case 10:
        rotate_Green_CCW();
         break;

      case 11:
        rotate_Blue_CW();
         break;

      case 12:
        rotate_Blue_CCW();
         break;

      default:
         
         break;
  }
  return;
}

// Button 1 - SHOULD BE CORRECT
void rotate_Blue_CW(){ 

  copyC(Current_LED, Set_LED, 54);

  //Front Face
  

  Set_LED[51] = Current_LED[53];
  Set_LED[50] = Current_LED[52];
  Set_LED[45] = Current_LED[51];
  Set_LED[46] = Current_LED[50];
  Set_LED[47] = Current_LED[45];
  Set_LED[48] = Current_LED[46];
  Set_LED[53] = Current_LED[47];
  Set_LED[52] = Current_LED[48];

  // Side LEDs
  Set_LED[35] = Current_LED[44];
  Set_LED[36] = Current_LED[33];
  Set_LED[37] = Current_LED[34];
  Set_LED[38] = Current_LED[35];
  Set_LED[39] = Current_LED[36];
  Set_LED[40] = Current_LED[37];
  Set_LED[41] = Current_LED[38];
  Set_LED[42] = Current_LED[39];
  Set_LED[43] = Current_LED[40];
  Set_LED[44] = Current_LED[41];
  Set_LED[33] = Current_LED[42];
  Set_LED[34] = Current_LED[43];

  copyC(Set_LED, Current_LED, 54);
  return;  

}

// Button 2 - SHOULD BE CORRECT
void rotate_Blue_CCW(){
  copyC(Current_LED, Set_LED, 54);

  // Serial.println("Button 2");

  Set_LED[51] = Current_LED[45];
  Set_LED[52] = Current_LED[50];
  Set_LED[53] = Current_LED[51];
  Set_LED[48] = Current_LED[52];
  Set_LED[47] = Current_LED[53];
  Set_LED[46] = Current_LED[48];
  Set_LED[45] = Current_LED[47];
  Set_LED[50] = Current_LED[46];
  

  // Side LEDs
  Set_LED[41] = Current_LED[44];
  Set_LED[40] = Current_LED[43];
  Set_LED[39] = Current_LED[42];
  Set_LED[38] = Current_LED[41];
  Set_LED[37] = Current_LED[40];
  Set_LED[36] = Current_LED[39];
  Set_LED[35] = Current_LED[38];
  Set_LED[34] = Current_LED[37];
  Set_LED[33] = Current_LED[36];
  Set_LED[44] = Current_LED[35];
  Set_LED[43] = Current_LED[34];
  Set_LED[42] = Current_LED[33];
 

  copyC(Set_LED, Current_LED, 54);
  return; 
}

// Button 3 - SHOULD BE CORRECT
void rotate_White_CW(){
  // Serial.println("Button 3");

  copyC(Current_LED, Set_LED, 54);


  Set_LED[42] = Current_LED[44];
  Set_LED[30] = Current_LED[43];
  Set_LED[18] = Current_LED[42];
  Set_LED[19] = Current_LED[30];
  Set_LED[20] = Current_LED[18];
  Set_LED[32] = Current_LED[19];
  Set_LED[44] = Current_LED[20];
  Set_LED[43] = Current_LED[32];
  

  // Side LEDs

  //New Swaps

  Set_LED[45] = Current_LED[9];
  Set_LED[50] = Current_LED[21];
  Set_LED[51] = Current_LED[33];
  Set_LED[41] = Current_LED[45];
  Set_LED[29] = Current_LED[50];
  Set_LED[17] = Current_LED[51];
  Set_LED[6] = Current_LED[41];
  Set_LED[7] = Current_LED[29];
  Set_LED[8] = Current_LED[17];
  Set_LED[9] = Current_LED[6];
  Set_LED[21] = Current_LED[7];
  Set_LED[33] = Current_LED[8];

 

  copyC(Set_LED, Current_LED, 54);
  return;
}

// Button 4 - SHOULD BE WORKING
void rotate_White_CCW(){
  // Serial.println("Button 4");

  copyC(Current_LED, Set_LED, 54);


  Set_LED[44] = Current_LED[42];
  Set_LED[32] = Current_LED[43];
  Set_LED[20] = Current_LED[44];
  Set_LED[19] = Current_LED[32];
  Set_LED[18] = Current_LED[20];
  Set_LED[30] = Current_LED[19];
  Set_LED[42] = Current_LED[18];
  Set_LED[43] = Current_LED[30];
  

  // Side LEDs
  Set_LED[45] = Current_LED[41];
  Set_LED[50] = Current_LED[29];
  Set_LED[51] = Current_LED[17];
  Set_LED[41] = Current_LED[6];
  Set_LED[29] = Current_LED[7];
  Set_LED[17] = Current_LED[8];
  Set_LED[6] = Current_LED[9];
  Set_LED[7] = Current_LED[21];
  Set_LED[8] = Current_LED[33];
  Set_LED[9] = Current_LED[45];
  Set_LED[21] = Current_LED[50];
  Set_LED[33] = Current_LED[51];
 

  copyC(Set_LED, Current_LED, 54);
  return;
}

// Button 5 - SHOULD BE WORKING
void rotate_Purple_CW(){
  // Serial.println("Button 5");

  copyC(Current_LED, Set_LED, 54);


  Set_LED[33] = Current_LED[35];
  Set_LED[21] = Current_LED[34];
  Set_LED[9] = Current_LED[33];
  Set_LED[10] = Current_LED[21];
  Set_LED[11] = Current_LED[9];
  Set_LED[23] = Current_LED[10];
  Set_LED[35] = Current_LED[11];
  Set_LED[34] = Current_LED[23];
  

  // Side LEDs
  Set_LED[36] = Current_LED[2];
  Set_LED[47] = Current_LED[12];
  Set_LED[46] = Current_LED[24];
  Set_LED[45] = Current_LED[36];
  Set_LED[44] = Current_LED[47];
  Set_LED[32] = Current_LED[46];
  Set_LED[20] = Current_LED[45];
  Set_LED[8] = Current_LED[44];
  Set_LED[3] = Current_LED[32];
  Set_LED[2] = Current_LED[20];
  Set_LED[12] = Current_LED[8];
  Set_LED[24] = Current_LED[3];
 
  copyC(Set_LED, Current_LED, 54);
  return;
}

// Button 6 - SHOULD BE WORKING
void rotate_Purple_CCW(){
  // Serial.println("Button 6");

  copyC(Current_LED, Set_LED, 54);


  Set_LED[35] = Current_LED[33];
  Set_LED[23] = Current_LED[34];
  Set_LED[11] = Current_LED[35];
  Set_LED[10] = Current_LED[23];
  Set_LED[9] = Current_LED[11];
  Set_LED[21] = Current_LED[10];
  Set_LED[33] = Current_LED[9];
  Set_LED[34] = Current_LED[21];
  

  // Side LEDs
  Set_LED[36] = Current_LED[45];
  Set_LED[24] = Current_LED[46];
  Set_LED[12] = Current_LED[47];
  Set_LED[2] = Current_LED[36];
  Set_LED[3] = Current_LED[24];
  Set_LED[8] = Current_LED[12];
  Set_LED[20] = Current_LED[2];
  Set_LED[32] = Current_LED[3];
  Set_LED[44] = Current_LED[8];
  Set_LED[45] = Current_LED[20];
  Set_LED[46] = Current_LED[32];
  Set_LED[47] = Current_LED[44];
 

  copyC(Set_LED, Current_LED, 54);
  return;
}

// Button 7 - SHOULD BE WORKING
void rotate_Yellow_CW(){
  // Serial.println("Button 7");

  copyC(Current_LED, Set_LED, 54);


  Set_LED[38] = Current_LED[14];
  Set_LED[37] = Current_LED[26];
  Set_LED[36] = Current_LED[38];
  Set_LED[24] = Current_LED[37];
  Set_LED[12] = Current_LED[36];
  Set_LED[13] = Current_LED[24];
  Set_LED[14] = Current_LED[12];
  Set_LED[26] = Current_LED[13];
  

  // Side LEDs
  //NEW

  Set_LED[39] = Current_LED[0];
  Set_LED[53] = Current_LED[15];
  Set_LED[48] = Current_LED[27];
  Set_LED[47] = Current_LED[39];
  Set_LED[35] = Current_LED[53];
  Set_LED[23] = Current_LED[48];
  Set_LED[11] = Current_LED[47];
  Set_LED[2] = Current_LED[35];
  Set_LED[1] = Current_LED[23];
  Set_LED[0] = Current_LED[11];
  Set_LED[15] = Current_LED[2];
  Set_LED[27] = Current_LED[1];
 

  copyC(Set_LED, Current_LED, 54);
  return;
}

// Button 8 - SHOULD BE WORKING
void rotate_Yellow_CCW(){
  // Serial.println("Button 8");

  copyC(Current_LED, Set_LED, 54);


  Set_LED[38] = Current_LED[36];
  Set_LED[26] = Current_LED[37];
  Set_LED[14] = Current_LED[38];
  Set_LED[13] = Current_LED[26];
  Set_LED[12] = Current_LED[14];
  Set_LED[24] = Current_LED[13];
  Set_LED[36] = Current_LED[12];
  Set_LED[37] = Current_LED[24];
  

  // Side LEDs
  Set_LED[39] = Current_LED[47];
  Set_LED[27] = Current_LED[48];
  Set_LED[15] = Current_LED[53];
  Set_LED[0] = Current_LED[39];
  Set_LED[1] = Current_LED[27];
  Set_LED[2] = Current_LED[15];
  Set_LED[11] = Current_LED[0];
  Set_LED[23] = Current_LED[1];
  Set_LED[35] = Current_LED[2];
  Set_LED[47] = Current_LED[11];
  Set_LED[48] = Current_LED[23];
  Set_LED[53] = Current_LED[35];
 

  copyC(Set_LED, Current_LED, 54);
  return;
}

// Button 9 - SHOULD BE WORKING
void rotate_Red_CW(){
  // Serial.println("Button 9");   

  copyC(Current_LED, Set_LED, 54);


  Set_LED[39] = Current_LED[41];
  Set_LED[27] = Current_LED[40];
  Set_LED[15] = Current_LED[39];
  Set_LED[16] = Current_LED[27];
  Set_LED[17] = Current_LED[15];
  Set_LED[29] = Current_LED[16];
  Set_LED[41] = Current_LED[17];
  Set_LED[40] = Current_LED[29];
  

  // Side LEDs
  //NEW
  Set_LED[53] = Current_LED[42];
  Set_LED[38] = Current_LED[51];
  Set_LED[26] = Current_LED[52];
  Set_LED[14] = Current_LED[53];
  Set_LED[0] = Current_LED[38];
  Set_LED[5] = Current_LED[26];
  Set_LED[6] = Current_LED[14];
  Set_LED[18] = Current_LED[0];
  Set_LED[30] = Current_LED[5];
  Set_LED[42] = Current_LED[6];
  Set_LED[51] = Current_LED[18];
  Set_LED[52] = Current_LED[30];
 

  copyC(Set_LED, Current_LED, 54);
  return;
}

// Button 10 - SHOULD BE WORKING
void rotate_Red_CCW(){
  // Serial.println("Button 10");  

  copyC(Current_LED, Set_LED, 54);


  Set_LED[41] = Current_LED[39];
  Set_LED[29] = Current_LED[40];
  Set_LED[17] = Current_LED[41];
  Set_LED[16] = Current_LED[29];
  Set_LED[15] = Current_LED[17];
  Set_LED[27] = Current_LED[16];
  Set_LED[39] = Current_LED[15];
  Set_LED[40] = Current_LED[27];
  

  // Side LEDs
  Set_LED[42] = Current_LED[53];
  Set_LED[30] = Current_LED[52];
  Set_LED[18] = Current_LED[51];
  Set_LED[6] = Current_LED[42];
  Set_LED[5] = Current_LED[30];
  Set_LED[0] = Current_LED[18];
  Set_LED[14] = Current_LED[6];
  Set_LED[26] = Current_LED[5];
  Set_LED[38] = Current_LED[0];
  Set_LED[53] = Current_LED[14];
  Set_LED[52] = Current_LED[26];
  Set_LED[51] = Current_LED[38];
 

  copyC(Set_LED, Current_LED, 54);
  return;
}

// Button 11 - SHOULD BE CORRECT
void rotate_Green_CW(){
  // Serial.println("Button 11");

  copyC(Current_LED, Set_LED, 54);

  Set_LED[2] = Current_LED[0];
  Set_LED[3] = Current_LED[1];
  Set_LED[8] = Current_LED[2];
  Set_LED[7] = Current_LED[3];
  Set_LED[6] = Current_LED[8];
  Set_LED[5] = Current_LED[7];
  Set_LED[0] = Current_LED[6];
  Set_LED[1] = Current_LED[5];
  

  // Side LEDs
  Set_LED[17] = Current_LED[20];
  Set_LED[16] = Current_LED[19];
  Set_LED[15] = Current_LED[18];
  Set_LED[14] = Current_LED[17];
  Set_LED[13] = Current_LED[16];
  Set_LED[12] = Current_LED[15];
  Set_LED[11] = Current_LED[14];
  Set_LED[10] = Current_LED[13];
  Set_LED[9] = Current_LED[12];
  Set_LED[20] = Current_LED[11];
  Set_LED[19] = Current_LED[10];
  Set_LED[18] = Current_LED[9];
 

  copyC(Set_LED, Current_LED, 54);
  return;
}

// Button 12 - SHOULD BE CORRECT
void rotate_Green_CCW(){
  // Serial.println("Button 12");

  copyC(Current_LED, Set_LED, 54);


  Set_LED[0] = Current_LED[2];
  Set_LED[5] = Current_LED[1];
  Set_LED[6] = Current_LED[0];
  Set_LED[7] = Current_LED[5];
  Set_LED[8] = Current_LED[6];
  Set_LED[3] = Current_LED[7];
  Set_LED[2] = Current_LED[8];
  Set_LED[1] = Current_LED[3];
  

  // Side LEDs
  Set_LED[12] = Current_LED[9];
  Set_LED[13] = Current_LED[10];
  Set_LED[14] = Current_LED[11];
  Set_LED[15] = Current_LED[12];
  Set_LED[16] = Current_LED[13];
  Set_LED[17] = Current_LED[14];
  Set_LED[18] = Current_LED[15];
  Set_LED[19] = Current_LED[16];
  Set_LED[20] = Current_LED[17];
  Set_LED[9] = Current_LED[18];
  Set_LED[10] = Current_LED[19];
  Set_LED[11] = Current_LED[20];
 

  copyC(Set_LED, Current_LED, 54);
  return;
}


void loop() {
  if(standby == 1){

    WiFiClient client = server.available();
    if (!client) {
      //return 99;
    }
   
    String req = client.readStringUntil('\r');
    //Serial.print(req);
    if (req.indexOf("SB") != -1) {
      standby = 0;
    }
    else if (req.indexOf("Shuffle") != -1) {
      standby = 0;
      buttonPressed = 77;
      shuffleCube();
    }

    delay(3000);
    randNumber = random(0, 13);
    rotateFace(randNumber);
    setLEDS();
  }
  else if(standby == 0){
    setLEDS();
    button_Pressed = waitForButton();
    if(button_Pressed != 99){
      if(button_Pressed == 77){
        shuffleCube();
        Serial.println("Shuffle");
      }
      else if(button_Pressed == 88){
        copyC(SOLVED_LED, Current_LED, 54);
        Serial.println("Solve");
      }
      rotateFace(button_Pressed);
      Serial.println(button_Pressed);
    }
  }
  
  
  //Serial.println(button_Pressed);
 
  
}

