#include <SPI.h>
#include <WiFi101.h>
#include <Arduino_JSON.h>
#include <Adafruit_NeoPixel.h>
#include "arduino_secrets.h" 

#define NUM_LEDS 90 //39 for Sarah's shoes //90 for Aasta's red
#define PIN 10
#define MIC_PIN 9 // microphone
#define FORCE_PIN 1
int fsrReading;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

#define BUTTON 13
int currentProgram = 0;

    // Microphone
    #define DC_OFFSET  0  // DC offset in mic signal - if unusure, leave 0
    #define NOISE     10  // Noise/hum/interference in mic signal
    #define SAMPLES   60  // Length of buffer for dynamic level adjustment
    #define TOP       (NUM_LEDS + 2) // Allow dot to go slightly off scale
    #define PEAK_FALL 4  // Rate of peak falling dot

    byte
      peak      = 0,      // Used for falling dot
      dotCount  = 0,      // Frame counter for delaying dot-falling speed
      volCount  = 0;      // Frame counter for storing past volume data
    int
      vol[SAMPLES],       // Collection of prior volume samples
      lvl       = 10,      // Current "dampened" audio level
      minLvlAvg = 0,      // For dynamic adjustment of graph low & high
      maxLvlAvg = 512;
      
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

bool offlineMode = true;

char programDefault[] = "{\"example\":[{\"measurementType\":\"pressure\", \"lightBehavior\":\"meteor\"}, {}],\"timestamp\":42}";
JSONVar programs = JSON.parse(programDefault);

int status = WL_IDLE_STATUS;

// Initialize the WiFi client library
//WiFiClient client;
WiFiSSLClient client;

// server address:
//char server[] = "example.org";
char server[] = "ue4g2j6lsd.execute-api.us-east-1.amazonaws.com";
//char server[] = "api.lumin8it.com";
//IPAddress server(64,131,82,241);

unsigned long lastConnectionTime = 0;            // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 10L * 1000L; // delay between updates, in milliseconds

//unsigned long lastConnectionTime = 0;
//const unsigned long postingInterval = 3L * 1000L;

void changeEffect() {
  //if (digitalRead (BUTTON) == HIGH) {
  if (millis() - lastConnectionTime > postingInterval) {   
    currentProgram++;
    Serial.print(currentProgram);
    lastConnectionTime = millis();
    //EEPROM.put(0, selectedEffect);
    //asm volatile ("  jmp 0");
  }
  //}
}

void setup() {
  memset(vol, 0, sizeof(vol));
  
  lastConnectionTime = millis();
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  pinMode(13,INPUT_PULLUP);  // internal pull-up resistor
  attachInterrupt (digitalPinToInterrupt (BUTTON), changeEffect, CHANGE); // pressed


  
  WiFi.setPins(8,7,4,2);
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    offlineMode = true;
    // don't continue:
    //while (true);
  }

  // attempt to connect to WiFi network:
  while ( offlineMode == false && status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  // you're connected now, so print out the status:
  printWiFiStatus();
}

int currentTimestamp = 2;
bool dataChanged = true;
void loop() {
  if(currentProgram>18) { 
    currentProgram=0; 
  }
  // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only:

  String fullMessage = "";
  bool printed = false;

  if (offlineMode == false){
  
  while (client.available()) {
    char c = client.read();
    fullMessage += c;

    //log the output being received in stream form
    Serial.write(c);

    //the stream is done
    if (client.available() == 0 && printed == false){

      //log the parsed json response
      Serial.println("PARSED RESPONSE");
      JSONVar jsonResponse = JSON.parse(fullMessage.substring(fullMessage.indexOf("{")));
      //Serial.println(jsonResponse["example"][0]);
      //Serial.println((int) jsonResponse["example"]);
      printed = true;
      
      //check if the json response is different than the last time polled
      
      if((int) jsonResponse["timestamp"] != currentTimestamp){
        currentTimestamp = (int) jsonResponse["example"];
        dataChanged = true;
        currentProgram = 0;

        // Update Programs
        programs = jsonResponse;
        Serial.println(programs);
      }
      
    }
  }

  // if ten seconds have passed since your last connection,
  // then connect again and send data:
  if (millis() - lastConnectionTime > postingInterval) {
    httpRequest();
  }

  }

  //restart the behavior if new data available
  if (dataChanged == true){
    Serial.println("RESTART PROGRAM");
    dataChanged = false;

    Serial.println("CURRENT PROGRAM");
    Serial.println(programs["example"][currentProgram]["measurementType"]);
    Serial.println(JSON.typeof(programs["example"][currentProgram]["measurementType"]));
    //Serial.println(programs[currentProgram]["measurementType"]);
    Serial.println(JSON.stringify(programs["example"][currentProgram]["measurementType"]));
  }

  if(JSON.stringify(programs["example"][currentProgram]["measurementType"]) == "\"accelerometer\""){
    Serial.println("accelerometer");
    //if value greater than blah
    Serial.println(programs["example"][currentProgram]["lightBehavior"]);
    //lightUp(programs["example"][currentProgram]["lightBehavior"]));
  }
  else if(JSON.stringify(programs["example"][currentProgram]["measurementType"]) == "\"sound\""){
    //Serial.println("sound");
    //if sound
    lightUp(JSON.stringify(programs["example"][currentProgram]["lightBehavior"]));
  }
  else if(JSON.stringify(programs["example"][currentProgram]["measurementType"]) == "\"pressure\""){
    fsrReading = analogRead(FORCE_PIN);
    if (fsrReading < 100)
    {
    lightUp(JSON.stringify(programs["example"][currentProgram]["lightBehavior"]));
    }
    }
}

void lightUp(String lightBehavior){
  //Serial.println(lightBehavior);
  if(lightBehavior == "\"blink\""){
    Serial.println("MEEEEEP");
  }
  else if(lightBehavior == "\"rainbow\""){
    RGBLoop();
  }
  else if(lightBehavior == "\"strobe\""){
    Strobe(0xff, 0xff, 0xff, 10, 50, 1000);
    }
  else if(lightBehavior =="\"fade\""){
    FadeInOut(0xff, 0x00, 0x00); // red
    FadeInOut(0xff, 0xff, 0xff); // white 
    FadeInOut(0x00, 0x00, 0xff);
    }
  else if(lightBehavior =="\"cylon\""){
    CylonBounce(0xff, 0x00, 0x00, 4, 10, 50);
    }
  else if(lightBehavior =="\"twinkle\""){
    Twinkle(0xff, 0x00, 0x00, 10, 100, false);
    }
  else if(lightBehavior =="\"meteor\""){
    meteorRain(0xff,0xff,0xff,10, 64, true, 30);
  }
  else if(lightBehavior == "\"sound\""){
    
    uint8_t  i;
  uint16_t minLvl, maxLvl;
  int      n, height;
 
 
 
  n   = analogRead(MIC_PIN);                        // Raw reading from mic 
  n   = abs(n - 512 - DC_OFFSET); // Center on zero
  n   = (n <= NOISE) ? 0 : (n - NOISE);             // Remove noise/hum
  lvl = ((lvl * 7) + n) >> 3;    // "Dampened" reading (else looks twitchy)
 
  // Calculate bar height based on dynamic min/max levels (fixed point):
  height = TOP * (lvl - minLvlAvg) / (long)(maxLvlAvg - minLvlAvg);
 
  if(height < 0L)       height = 0;      // Clip output
  else if(height > TOP) height = TOP;
  if(height > peak)     peak   = height; // Keep 'peak' dot at top
 
 
  // Color pixels based on rainbow gradient
  for(i=0; i<NUM_LEDS; i++) {
    if(i >= height)               strip.setPixelColor(i,   0,   0, 0);
    else strip.setPixelColor(i,Wheel(map(i,0,strip.numPixels()-1,30,150)));
    
  }
 
 
 
  // Draw peak dot  
  if(peak > 0 && peak <= NUM_LEDS-1) strip.setPixelColor(peak,Wheel(map(peak,0,strip.numPixels()-1,30,150)));
  
   strip.show(); // Update strip
 
// Every few frames, make the peak pixel drop by 1:
 
    if(++dotCount >= PEAK_FALL) { //fall rate 
      
      if(peak > 0) peak--;
      dotCount = 0;
    }
 
 
 
  vol[volCount] = n;                      // Save sample for dynamic leveling
  if(++volCount >= SAMPLES) volCount = 0; // Advance/rollover sample counter
 
  // Get volume range of prior frames
  minLvl = maxLvl = vol[0];
  for(i=1; i<SAMPLES; i++) {
    if(vol[i] < minLvl)      minLvl = vol[i];
    else if(vol[i] > maxLvl) maxLvl = vol[i];
  }
  // minLvl and maxLvl indicate the volume range over prior frames, used
  // for vertically scaling the output graph (so it looks interesting
  // regardless of volume level).  If they're too close together though
  // (e.g. at very low volume levels) the graph becomes super coarse
  // and 'jumpy'...so keep some minimum distance between them (this
  // also lets the graph go to zero when no sound is playing):
  if((maxLvl - minLvl) < TOP) maxLvl = minLvl + TOP;
  minLvlAvg = (minLvlAvg * 63 + minLvl) >> 6; // Dampen min/max levels
  maxLvlAvg = (maxLvlAvg * 63 + maxLvl) >> 6; // (fake rolling average)
  }
}

// this method makes a HTTP connection to the server:
void httpRequest() {
  // close any connection before send a new request.
  // This will free the socket on the WiFi shield
  client.stop();

  // if there's a successful connection:
  if (client.connectSSL(server, 443)) {
    Serial.println("connecting...");
    // send the HTTP PUT request:
    client.println("GET /PROD/programs HTTP/1.1");
    client.println("Host: ue4g2j6lsd.execute-api.us-east-1.amazonaws.com");
    //client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
    client.println();

    // note the time that the connection was made:
    lastConnectionTime = millis();
  }
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }
}


void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void RGBLoop(){
  for(int j = 0; j < 3; j++ ) { 
    // Fade IN
    for(int k = 0; k < 256; k++) { 
      switch(j) { 
        case 0: setAll(k,0,0); break;
        case 1: setAll(0,k,0); break;
        case 2: setAll(0,0,k); break;
      }
      showStrip();
      delay(3);
    }
    // Fade OUT
    for(int k = 255; k >= 0; k--) { 
      switch(j) { 
        case 0: setAll(k,0,0); break;
        case 1: setAll(0,k,0); break;
        case 2: setAll(0,0,k); break;
      }
      showStrip();
      delay(3);
    }
  }
}

uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue); 
  }
  showStrip();
}

void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}

// Set a LED color (not yet visible)
void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H 
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

void FadeInOut(byte red, byte green, byte blue){
  float r, g, b;
      
  for(int k = 0; k < 256; k=k+1) { 
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(r,g,b);
    showStrip();
  }
     
  for(int k = 255; k >= 0; k=k-2) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(r,g,b);
    showStrip();
  }
}

void Strobe(byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause){
  for(int j = 0; j < StrobeCount; j++) {
    setAll(red,green,blue);
    showStrip();
    delay(FlashDelay);
    setAll(0,0,0);
    showStrip();
    delay(FlashDelay);
  }
 
 delay(EndPause);
}

void CylonBounce(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay){

  for(int i = 0; i < NUM_LEDS-EyeSize-2; i++) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue); 
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }

  delay(ReturnDelay);

  for(int i = NUM_LEDS-EyeSize-2; i > 0; i--) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue); 
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }
  
  delay(ReturnDelay);
}

void Twinkle(byte red, byte green, byte blue, int Count, int SpeedDelay, boolean OnlyOne) {
  setAll(0,0,0);
  
  for (int i=0; i<Count; i++) {
     setPixel(random(NUM_LEDS),red,green,blue);
     showStrip();
     delay(SpeedDelay);
     if(OnlyOne) { 
       setAll(0,0,0); 
     }
   }
  
  delay(SpeedDelay);
}

void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {  
  setAll(0,0,0);
  
  for(int i = 0; i < NUM_LEDS+NUM_LEDS; i++) {
    
    
    // fade brightness all LEDs one step
    for(int j=0; j<NUM_LEDS; j++) {
      if( (!meteorRandomDecay) || (random(10)>5) ) {
        fadeToBlack(j, meteorTrailDecay );        
      }
    }
    
    // draw meteor
    for(int j = 0; j < meteorSize; j++) {
      if( ( i-j <NUM_LEDS) && (i-j>=0) ) {
        setPixel(i-j, red, green, blue);
      } 
    }
   
    showStrip();
    delay(SpeedDelay);
  }
}

// used by meteorrain
void fadeToBlack(int ledNo, byte fadeValue) {
 #ifdef ADAFRUIT_NEOPIXEL_H 
    // NeoPixel
    uint32_t oldColor;
    uint8_t r, g, b;
    int value;
    
    oldColor = strip.getPixelColor(ledNo);
    r = (oldColor & 0x00ff0000UL) >> 16;
    g = (oldColor & 0x0000ff00UL) >> 8;
    b = (oldColor & 0x000000ffUL);

    r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
    g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
    b=(b<=10)? 0 : (int) b-(b*fadeValue/256);
    
    strip.setPixelColor(ledNo, r,g,b);
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[ledNo].fadeToBlackBy( fadeValue );
 #endif  
}
