#include <string.h>
#include <stdlib.h>
#include "TFT_eSPI.h"
#include "Free_Fonts.h" //include the header file
TFT_eSPI tft;

#define HEIGHT 15
#define WIDTH 29
#define BUFSIZE 128
#define BUFLIMIT (BUFSIZE-10)
#define STRING_OFFSETX 5

char buffer[BUFSIZE];
char screen[HEIGHT][WIDTH];
int cursor_raw = 0;
int bgcolor = TFT_BLACK;
int fgcolor = TFT_WHITE;

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
    recvCommand();
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

void recvCommand(){
    int ret = 0;
    if (Serial.available() == 0) {
        return;
    }
    char cmd = Serial.read();
    if(cmd == 'C'){
        cursor_raw = 0;
        clearScreen();
        skipRecv();
    }else if(cmd == 'S'){
        char c = Serial.read();
        readInput();
        char *endptr;
        if(c == 'B'){
            bgcolor = (int)strtol(buffer, &endptr, 16);
        }else{
            fgcolor = (int)strtol(buffer, &endptr, 16);
        }
    }else if(cmd == 'D'){
        char c = Serial.read();
        readInput();
        if(c == 'L'){
            drawLine(buffer);
        }else if(c == 'R'){
            drawRect(buffer);
        }else if(c == 'r'){
            drawRoundRect(buffer);
        }else if(c == 'C'){
            drawCircle(buffer);
        }else if(c == 'T'){
            drawTriangle(buffer);
        }else if(c == 'E'){
            drawEllipse(buffer);
        }else if(c == 'S'){
            drawString(buffer);
        }
    }else if(cmd == 'F'){
        char c = Serial.read();
        readInput();
        if(c == 'R'){
            fillRect(buffer);
        }else if(c == 'r'){
            fillRoundRect(buffer);
        }else if(c == 'C'){
            fillCircle(buffer);
        }else if(c == 'T'){
            fillTriangle(buffer);
        }else if(c == 'E'){
            fillEllipse(buffer);
        }
    }else{ // 'W'
        readInput();
        putStringLn(buffer);
    }
}

void skipRecv(){
    while(1){ // wait for '\n'
        char c = Serial.read();
        if(c == '\n') return;
    }
}

void readInput(){
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
    if(terminated == 0){
        skipRecv();
    }
}

void updateScreen(){
    tft.setTextColor(fgcolor);
    for(int i = 0; i < HEIGHT; i++){
        screen[i][WIDTH-1] = ' '; // the last one should be ' '
        tft.drawString(screen[i], 0 + STRING_OFFSETX, i * 16);
    }
}

void newLine(){
    for(int i = 0; i < HEIGHT-1; i++){
        memcpy(screen[i], screen[i+1], WIDTH);
    }
    memset(screen[HEIGHT-1], 0, WIDTH);
    tft.fillScreen(bgcolor);
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

char * parseParameter(char *mesg, int num, int *val){
    char *ptr;
    if(num == 0) return mesg;
    for(int i = 0; i < num; i++){
        val[i] = 0;
    }
    ptr = strtok(mesg, ",");
    if(ptr == NULL){
        return NULL;
    }
    val[0] = atoi(ptr);
    for(int i = 1; i < num; i++){
        ptr = strtok(NULL, ",");
        if(ptr == NULL){
            return NULL;
        }
        val[i] = atoi(ptr);
    }
    return ptr;
}

void drawLine(char *mesg){
    int pt[4];
    parseParameter(mesg, 4, pt);
    tft.drawLine(pt[0], pt[1], pt[2], pt[3], fgcolor);
}

void drawRect(char *mesg){
    int pt[4];
    parseParameter(mesg, 4, pt);
    tft.drawRect(pt[0], pt[1], pt[2], pt[3], fgcolor);
}

void drawRoundRect(char *mesg){
    int pt[5];
    parseParameter(mesg, 5, pt);
    tft.drawRoundRect(pt[0], pt[1], pt[2], pt[3], pt[4], fgcolor);
}

void drawCircle(char *mesg){
    int pt[3];
    parseParameter(mesg, 3, pt);
    tft.drawCircle(pt[0], pt[1], pt[2], fgcolor);
}

void drawTriangle(char *mesg){
    int pt[6];
    parseParameter(mesg, 6, pt);
    tft.drawTriangle(pt[0], pt[1], pt[2], pt[3], pt[4], pt[5], fgcolor);
}

void drawEllipse(char *mesg){
    int pt[4];
    parseParameter(mesg, 4, pt);
    tft.drawEllipse(pt[0], pt[1], pt[2], pt[3], fgcolor);
}

void drawString(char *mesg){
    char *ptr;
    int pt[1];
    ptr = parseParameter(mesg, 1, pt);
    if(ptr != NULL){
        tft.setTextColor(fgcolor);
        tft.drawString(mesg + strlen(ptr) + 1, 0 + STRING_OFFSETX, pt[0] * 16);
    }
}

void fillRect(char *mesg){
    int pt[4];
    parseParameter(mesg, 4, pt);
    tft.fillRect(pt[0], pt[1], pt[2], pt[3], fgcolor);
}

void fillRoundRect(char *mesg){
    int pt[4];
    parseParameter(mesg, 5, pt);
    tft.fillRoundRect(pt[0], pt[1], pt[2], pt[3], pt[4], fgcolor);
}

void fillCircle(char *mesg){
    int pt[3];
    parseParameter(mesg, 3, pt);
    tft.fillCircle(pt[0], pt[1], pt[2], fgcolor);
}

void fillTriangle(char *mesg){
    int pt[6];
    parseParameter(mesg, 6, pt);
    tft.fillTriangle(pt[0], pt[1], pt[2], pt[3], pt[4], pt[5], fgcolor);
}

void fillEllipse(char *mesg){
    int pt[4];
    parseParameter(mesg, 4, pt);
    tft.fillEllipse(pt[0], pt[1], pt[2], pt[3], fgcolor);
}

void clearScreen(){
    memset(screen, 0, HEIGHT*WIDTH);
    tft.fillScreen(bgcolor);
}

void welcomeScreen() {
    tft.setFreeFont(FM9);
    tft.setTextColor(fgcolor);
    setString(1, "Wait for Connecting...");
    setString(HEIGHT-1, "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
    cursor_raw = HEIGHT;
    updateScreen();
}
