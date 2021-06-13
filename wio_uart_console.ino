#include <string.h>
#include "TFT_eSPI.h"
#include "Free_Fonts.h" //include the header file
TFT_eSPI tft;

#define HEIGHT 15
#define WIDTH 29
#define BUFSIZE 128
#define BUFLIMIT (BUFSIZE-10)

char buffer[BUFSIZE];
char screen[HEIGHT][WIDTH];
int cursor_raw = 0;

void setup() {
    Serial.begin(115200);
    Serial.flush();
    
    pinMode(WIO_KEY_C, INPUT_PULLUP);
    pinMode(WIO_KEY_A, INPUT_PULLUP);
    
    pinMode(WIO_5S_UP, INPUT_PULLUP);
    pinMode(WIO_5S_DOWN, INPUT_PULLUP);
    pinMode(WIO_5S_LEFT, INPUT_PULLUP);
    pinMode(WIO_5S_RIGHT, INPUT_PULLUP);
    pinMode(WIO_5S_PRESS, INPUT_PULLUP);
    
    tft.begin();
    tft.setRotation(3);
    
    cursor_raw = 0;
    clearScreen();
    welcomeScreen();
}

void loop() {
    readInput();
    if(digitalRead(WIO_KEY_C) == LOW){
        newLine();
        delay(200);
    }
    if(digitalRead(WIO_KEY_A) == LOW){
        cursor_raw = 0;
        clearScreen();
        delay(200);
    }
    if (digitalRead(WIO_5S_UP) == LOW) {
        Serial.println("5U");
        delay(200);
    }
    if (digitalRead(WIO_5S_DOWN) == LOW) {
        Serial.println("5D");
        delay(200);
    }
    if (digitalRead(WIO_5S_LEFT) == LOW) {
        Serial.println("5L");
        delay(200);
    }
    if (digitalRead(WIO_5S_RIGHT) == LOW) {
        Serial.println("5R");
        delay(200);
    }
    if (digitalRead(WIO_5S_PRESS) == LOW) {
        Serial.println("5P");
        delay(200);
    }
}

void readInput(){
    int ret = 0;
    if (Serial.available() == 0) {
        return;
    }
    char cmd = Serial.read();
    if(cmd == 'C'){
        cursor_raw = 0;
        clearScreen();
        while(1){ // wait for '\n'
            char c = Serial.read();
            if(c == '\n') return;
        }
    }else{ // 'L'
        int terminated = 0;
        memset(buffer, 0, sizeof(buffer));
        for(int i = 0; i < BUFLIMIT; i++){
            char ch = Serial.read();
            if(ch == '\n'){
                terminated = 1;
                break;
            }
            buffer[i] = ch;
        }
        while(terminated == 0){
            char ch = Serial.read();
            if(ch == '\n'){
                terminated = 1;
            }
        }
        putStringLn(buffer);
    }
}

#define OFFSET 5
void updateScreen(){
    for(int i = 0; i < HEIGHT; i++){
        screen[i][WIDTH-1] = ' '; // the last one should be ' '
        tft.drawString(screen[i], 0 + OFFSET, i * 16);
    }
}

void newLine(){
    for(int i = 0; i < HEIGHT-1; i++){
        memcpy(screen[i], screen[i+1], WIDTH);
    }
    memset(screen[HEIGHT-1], 0, WIDTH);
    tft.fillScreen(TFT_BLACK);
    updateScreen();
}

void setString(int raw, char *mesg){
    memset(screen[raw], 0, WIDTH);
    int len = strlen(mesg);
    len = len > WIDTH ? WIDTH : len;
    memcpy(screen[raw], mesg, len);  
}

void putStringLn(char *mesg){
    if(cursor_raw == HEIGHT){
        newLine();
        cursor_raw = HEIGHT-1;
    }
    setString(cursor_raw, mesg);
    cursor_raw++;
    updateScreen();
}

void clearScreen(){
    memset(screen, 0, HEIGHT*WIDTH);
    tft.fillScreen(TFT_BLACK);
}

void welcomeScreen() {
    tft.setFreeFont(FM9);
    tft.setTextColor(TFT_WHITE);
    setString(1, "Wait for Connecting...");
    setString(HEIGHT-1, "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
    cursor_raw = HEIGHT;
    updateScreen();
}
