#include <Arduino.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <WiFi.h>
#include <SpotifyEsp32.h>
#include <SPI.h>

#define TFT_CS 1
#define TFT_RST 2
#define TFT_DC 3
#define TFT_SCLK 4
#define TFT_MOSI 5

// --- Button Pin Setup ---
#define BUTTON_PIN 14 
bool lastButtonState = HIGH;

char* SSID = "YOUR WIFI SSID";
const char* PASSWORD = "YOUR WIFI PASSWORD";
const char* CLIENT_ID = "YOUR CLIENT ID";
const char* CLIENT_SECRET = "YOUR CLIENT SECRET";

String lastArtist;
String lastTrackname;
Spotify sp(CLIENT_ID, CLIENT_SECRET);
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void setup() {
    Serial.begin(115200);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    tft.initR(INITR_BLACKTAB); 
    tft.setRotation(1); 
    tft.fillScreen(ST77XX_BLACK); 

    WiFi.begin(SSID, PASSWORD);
    tft.setCursor(0,0);
    tft.print("Connecting to WiFi...");
    
    while(WiFi.status() != WL_CONNECTED) {
        delay(1000);
    }
    
    tft.fillScreen(ST77XX_BLACK);
    tft.setCursor(0,0); 
    tft.println("WiFi Connected!");

    sp.begin();
    while(!sp.is_auth()){
        sp.handle_client();
    }
    tft.println("Spotify Auth OK!");
    delay(2000);
    tft.fillScreen(ST77XX_BLACK);
}

void loop(){
    // --- 1. Push-to-Talk Button Logic ---
    bool currentButtonState = digitalRead(BUTTON_PIN);
    
    // If button was JUST pressed down
    if (currentButtonState == LOW && lastButtonState == HIGH) {
        Serial.println("START_MIC");
        tft.fillScreen(ST77XX_RED); // Turn screen RED while recording
        tft.setCursor(5, 5);
        tft.setTextColor(ST77XX_WHITE);
        tft.print("RECORDING...");
        delay(50); // Debounce
    }
    // If button was JUST released
    else if (currentButtonState == HIGH && lastButtonState == LOW) {
        Serial.println("STOP_MIC");
        tft.fillScreen(ST77XX_BLUE); // Turn screen BLUE while thinking
        tft.setCursor(5, 5);
        tft.setTextColor(ST77XX_WHITE);
        tft.print("Thinking...");
        delay(50); // Debounce
    }
    lastButtonState = currentButtonState;

    // --- 2. Check for incoming Gemini answers ---
    if (Serial.available() > 0) {
        String incomingMessage = Serial.readStringUntil('\n');
        incomingMessage.trim(); 
        
        if (incomingMessage.length() > 0 && incomingMessage != "START_MIC" && incomingMessage != "STOP_MIC") {
            tft.fillScreen(ST77XX_BLUE);
            tft.setCursor(5, 5);
            tft.setTextColor(ST77XX_WHITE);
            tft.print("Gemini says:");
            
            tft.setCursor(5, 25);
            tft.print(incomingMessage);
            
            delay(5000); // Hold answer for 5 seconds
            tft.fillScreen(ST77XX_BLACK); 
            lastArtist = ""; // Force Spotify to redraw
        }
    }

    // --- 3. Handle Spotify (Only update if button isn't currently held down) ---
    if (currentButtonState == HIGH) { // Only draw Spotify if button is released
        String currentArtist = sp.current_artist_names();
        String currentTrackname = sp.current_track_name();

        if (lastArtist != currentArtist && currentArtist != "Something went wrong" && !currentArtist.isEmpty()) {
            tft.fillScreen(ST77XX_BLACK);
            lastArtist = currentArtist;
            tft.setCursor(10,10);
            tft.setTextColor(ST77XX_WHITE);
            tft.print(lastArtist);
        }

        if (lastTrackname != currentTrackname && currentTrackname != "Something went wrong" && currentTrackname != "null") {
            lastTrackname = currentTrackname;
            tft.setCursor(10,40);
            tft.print(lastTrackname);
        }
    }
    
    // Keep delay short so button is instantly responsive
    delay(20); 
}