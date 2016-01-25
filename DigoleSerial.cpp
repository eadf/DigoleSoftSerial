//Digole Digital Solutions: www.digole.com
#include "DigoleSerial.h"
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"

/*
// Communication set up command
 * "SB":Baud (ascII bytes end with 0x00/0x0A/0x0D) -- set UART Baud Rate
 * "SI2CA":Address(1 byte <127) -- Set I2C address, default address is:0x27
 * "DC":1/0(1byte) -- set config display on/off, if set to 1, displayer will display current commucation setting when power on
// Text Function command
 * "CL": -- Clear screen--OK
 * "CS":1/0 (1 byte)-- Cursor on/off
 * "TP":x(1 byte) y(1 byte) -- set text position
 * "TT":string(bytes) end with 0x00/0x0A/0x0D -- display string under regular mode
// Graphic function command
 * "GP":x(1byte) y(1byte) -- set current graphic position
 * "DM":"C/!/~/&/|/^"(ASCII 1byte) -- set drawing mode--C="Copy",! and ~ = "Not", & = "And", | = "Or", ^ = "Xor"
 * "SC":1/0 (1byte) -- set draw color--only 1 and 0
 * "LN":x0(1byte) y0(1byte) x1(1byte) y2(1byte)--draw line from x0,y0 to x1,y1,set new pot to x1,y1
 * "LT":x(1byte) y(1byte) -- draw line from current pos to x,y
 * "CC":x(1byte) y(1byte) ratio(byte) -- draw circle at x,y with ratio
 * "DP":x(1byte) y(1byte) Color(1byte) -- draw a pixel--OK
 * "DR":x0(1byte) y0(1byte) x1(1byte) y2(1byte)--draw rectangle, top-left:x0,y0; right-bottom:x1,y1
 * "FR":x0(1byte) y0(1byte) x1(1byte) y2(1byte)--draw filled rectangle, top-left:x0,y0; right-bottom:x1,y1
 */

// that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystal constructor is called).

//UART function

void DigoleSerialDisp::write2B(unsigned int v) {
    if (v < 255)
        write(v);
    else {
        write(255);
        write(v - 255);
    }
}

void DigoleSerialDisp::preprint(void) {
    //write((uint8_t)0);
    Print::print("TT");
}

/*----------Functions for Graphic LCD/OLED adapters only---------*/
void DigoleSerialDisp::drawBitmap(unsigned int x, unsigned int y, unsigned int w, unsigned int h, const uint8_t *bitmap) {
    uint8_t i = 0;
    if ((w & 7) != 0)
        i = 1;
    Print::print("DIM");
    write2B(x); //x;
    write2B(y);
    write2B(w);
    write2B(h);
    for (int j = 0; j < h * ((w >> 3) + i); j++) {
        write(pgm_read_byte_near(bitmap + j));
        //        delay(5);
    }
}

void DigoleSerialDisp::setRot90(void) {
    Print::print("SD1");
}

void DigoleSerialDisp::setRot180(void) {
    Print::print("SD2");
}

void DigoleSerialDisp::setRot270(void) {
    Print::print("SD3");
}

void DigoleSerialDisp::undoRotation(void) {
    Print::print("SD0");
}

void DigoleSerialDisp::setRotation(uint8_t d) {
    Print::print("SD");
    write2B(d);
}

void DigoleSerialDisp::setContrast(uint8_t c) {
    Print::print("CT");
    write2B(c);
}

void DigoleSerialDisp::drawBox(unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
    Print::print("FR");
    write2B(x);
    write2B(y);
    write2B(x + w);
    write2B(y + h);
}

void DigoleSerialDisp::drawCircle(unsigned int x, unsigned int y, unsigned int r, uint8_t f) {
    Print::print("CC");
    write2B(x);
    write2B(y);
    write2B(r);
    write(f);
}

void DigoleSerialDisp::drawDisc(unsigned int x, unsigned int y, unsigned int r) {
    drawCircle(x, y, r, 1);
}

void DigoleSerialDisp::drawFrame(unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
    Print::print("DR");
    write2B(x);
    write2B(y);
    write2B(x + w);
    write2B(y + h);
}

void DigoleSerialDisp::drawPixel(unsigned int x, unsigned int y, uint8_t color) {
    Print::print("DP");
    write2B(x);
    write2B(y);
    write(color);
}

void DigoleSerialDisp::drawLine(unsigned int x, unsigned int y, unsigned int x1, unsigned int y1) {
    Print::print("LN");
    write2B(x);
    write2B(y);
    write2B(x1);
    write2B(y1);
}

void DigoleSerialDisp::drawLineTo(unsigned int x, unsigned int y) {
    Print::print("LT");
    write2B(x);
    write2B(y);
}

void DigoleSerialDisp::drawHLine(unsigned int x, unsigned int y, unsigned int w) {
    drawLine(x, y, x + w, y);
}

void DigoleSerialDisp::drawVLine(unsigned int x, unsigned int y, unsigned int h) {
    drawLine(x, y, x, y + h);
}

void DigoleSerialDisp::nextTextLine(void) {
    write((uint8_t) 0);
    Print::print("TRT");
}

void DigoleSerialDisp::setFont(uint8_t font) {
    Print::print("SF");
    write(font);
}

void DigoleSerialDisp::setColor(uint8_t color) {
    Print::print("SC");
    write(color);
}

void DigoleSerialDisp::backLightOn(void) {
    Print::print("BL");
    write((uint8_t) 1);
}

void DigoleSerialDisp::backLightOff(void) {
    Print::print("BL");
    write((uint8_t) 0);
}

void DigoleSerialDisp::directCommand(uint8_t d) {
    Print::print("MCD");
    write(d);
}

void DigoleSerialDisp::directData(uint8_t d) {
    Print::print("MDT");
    write(d);
}

void DigoleSerialDisp::moveArea(unsigned int x0, unsigned int y0, unsigned int w, unsigned int h, char xoffset, char yoffset) {
    Print::print("MA");
    write2B(x0);
    write2B(y0);
    write2B(w);
    write2B(h);
    write(xoffset);
    write(yoffset);
}

void DigoleSerialDisp::uploadStartScreen(int lon, const unsigned char *data) {
    int j;
    uint8_t c;
    Print::print("SSS");
    write((uint8_t) (lon % 256));
    write((uint8_t) (lon / 256));
    delay(300);
    for (j = 0; j < lon; j++) {
        if ((j % 32) == 0)
            delay(50);
        delay(_Comdelay);
        c = pgm_read_byte_near(data + j);
        write(c);
    }
}

void DigoleSerialDisp::uploadUserFont(int lon, const unsigned char *data, uint8_t sect) {
    uint8_t c;
    Print::print("SUF");
    write(sect);
    write((uint8_t) (lon % 256));
    write((uint8_t) (lon / 256));
    for (int j = 0; j < lon; j++) {
        if ((j % 32) == 0)
            delay(50);
        delay(_Comdelay);
        c = pgm_read_byte_near(data + j);
        write(c);
    }
}

void DigoleSerialDisp::drawBitmap256(unsigned int x, unsigned int y, unsigned int w, unsigned int h, const uint8_t *bitmap) { //display 256 color image
    uint8_t i = 0;
    Print::print("EDIM1");
    write2B(x); //x;
    write2B(y);
    write2B(w);
    write2B(h);
    for (int j = 0; j < h * w; j++) {
        write(pgm_read_byte_near(bitmap + j));
        //        delay(5);
    }
}

void DigoleSerialDisp::drawBitmap262K(unsigned int x, unsigned int y, unsigned int w, unsigned int h, const uint8_t *bitmap) { //display 256 color image
    uint8_t i = 0;
    Print::print("EDIM3");
    write2B(x); //x;
    write2B(y);
    write2B(w);
    write2B(h);
    for (int j = 0; (j < h * w * 3); j++) {
        write(pgm_read_byte_near(bitmap + j));
        //        delay(5);
    }
}

void DigoleSerialDisp::setTrueColor(uint8_t r, uint8_t g, uint8_t b) { //Set true color
    uint8_t i = 0;
    Print::print("ESC");
    write(r);
    write(g);
    write(b);
}

void DigoleSerialDisp::drawStr(unsigned int x, unsigned int y, const char *s) {
    Print::print("TP");
    write2B(x);
    write2B(y);
    Print::print("TT");
    Print::print(s);
    write((uint8_t)0);
}

void DigoleSerialDisp::setPrintPos(unsigned int x, unsigned int y, uint8_t graph) {
    if (graph == 0) {
        Print::print("TP");
        write2B(x);
        write2B(y);
    } else {
        Print::print("GP");
        write2B(x);
        write2B(y);
    }
}

void DigoleSerialDisp::setLCDColRow(uint8_t col, uint8_t row) {
    Print::print("STCR");
    write(col);
    write(row);
    Print::print("\x80\xC0\x94\xD4");
}

void DigoleSerialDisp::setTextPosAbs(unsigned int x, unsigned int y) {
    Print::print("ETP");
    write2B(x);
    write2B(y);
}

//----Touch screen functions-----//

void DigoleSerialDisp::calibrateTouchScreen(void) {
    write('T');
    write('U');
    write('C');
    write('H');
    write('C');

}

void DigoleSerialDisp::readTouchScreen() {
    write('R');
    write('P');
    write('N');
    write('X');
    write('Y');
    write('W'); //change to 'I' if you don't want wait till pen touched
}

void DigoleSerialDisp::readClick() //read a click on touch screen
{
    write('R');
    write('P');
    write('N');
    write('X');
    write('Y');
    write('C');
}
int DigoleSerialDisp::readBattery(void)
{
  int c;
  write('R');
  write('D');
  write('B');
  write('A');
  write('T');
  c=read1();
  c<<=8;
  c|=read1();
  return c;
}
int DigoleSerialDisp::readAux(void)
{
  int c;
  write('R');
  write('D');
  write('A');
  write('U');
  write('X');
  c=read1();
  c<<=8;
  c+=read1();
  return c;
}
int DigoleSerialDisp::readTemperature(void)
{
  int c;
  write('R');
  write('D');
  write('T');
  write('M');
  write('P');
  c=read1();
  c<<=8;
  c|=read1();
  return c;
}
//-----Flash memory functions----//

void DigoleSerialDisp::flashErase(unsigned long int addr, unsigned long int length) {
    write('F');
    write('L');
    write('M');
    write('E');
    write('R');
    write(addr >> 16);
    write(addr >> 8);
    write(addr);
    write(length >> 16);
    write(length >> 8);
    write(length);
}

void DigoleSerialDisp::flashWriteL(unsigned long int addr, unsigned long int len, const unsigned char *data) {
    while (len > 1024) {
        flashWrite(addr, 1024, data);
        data += 1024;
        addr += 1024;
        len = len - 1024;
    }
    flashWrite(addr, len, data);
}

void DigoleSerialDisp::flashWrite(unsigned long int addr, unsigned long int len, const unsigned char *data) {
    unsigned char c;
    write('F');
    write('L');
    write('M');
    write('W');
    write('R');
    write(addr >> 16);
    write(addr >> 8);
    write(addr);
    write(len >> 16);
    write(len >> 8);
    write(len);
    for (long int i = 0; i < len; i++) {
        c = pgm_read_byte_near(data + i);
        write(c);
    }
    //check write memory done
    while (read1()!= 17);
}

void DigoleSerialDisp::flashReadStart(unsigned long int addr, unsigned long int len) {
    write('F');
    write('L');
    write('M');
    write('R');
    write('D');
    write(addr >> 16);
    write(addr >> 8);
    write(addr);
    write(len >> 16);
    write(len >> 8);
    write(len);
}

void DigoleSerialDisp::setFlashFont(unsigned long int addr) {
    write('S');
    write('F');
    write('F');
    write(addr >> 16);
    write(addr >> 8);
    write(addr);
}

void DigoleSerialDisp::runCommandSet(unsigned long int addr) {
    write('F');
    write('L');
    write('M');
    write('C');
    write('S');
    write(addr >> 16);
    write(addr >> 8);
    write(addr);
}
