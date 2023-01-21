
#include "SSD1306.h"
/*
 * SSD1306.cpp
 *
 *  Created on: 6 janv. 2023
 *      Author: guedesite
 */

	SSD1306::SSD1306(TwoWire *wire, uint8_t i2c_address) : wire(wire), i2c_address(i2c_address), buffer(NULL) { }
	SSD1306::SSD1306(TwoWire *wire, uint8_t i2c_address, uint32_t clock, uint8_t width, uint8_t height)
	: wire(wire), i2c_address(i2c_address), buffer(NULL), width(width), height(height) { wire->setClock(clock); }

	SSD1306::~SSD1306(void) {
		if(buffer) {
			free(buffer);
			buffer = NULL;
		}
	}

  // Initialiser l'écran
  bool SSD1306::init() {
	  if ((!buffer) && !(buffer = (uint8_t *)malloc(width * ((height + 7) / 8)))) {
	      return false;
	  }
	  clear();
	  wire->begin();
	  static const uint8_t PROGMEM init1[] = {0xAE,0xD5,0x80,0xA8};
	  _sendCommandList(init1, sizeof(init1));
	  _sendCommand(height - 1);

	  static const uint8_t PROGMEM init2[] = {0xD3,0x0, 0x40 | 0x0,0x8D};
	  _sendCommandList(init2, sizeof(init2));

	  _sendCommand(0x10 /* 0x14 */);

	  static const uint8_t PROGMEM init3[] = {0x20, 0x00, 0xA0 | 0x1,0xC8};
	  _sendCommandList(init3, sizeof(init3));

	  uint8_t comPins = 0x02;
	  uint8_t contrast = 0x8F;


	  _sendCommand(0xDA);
	  _sendCommand(comPins);
	  _sendCommand(0x81);
	  _sendCommand(contrast);

	  _sendCommand( 0xD9 ); // 0xd9
	  _sendCommand(0x22 /* 0xF1 */);

	  static const uint8_t PROGMEM init5[] = {0xDB, 0x40,0xA4, 0xA6,0x2E,0xAF}; // Main screen turn on
	  _sendCommandList(init5, sizeof(init5));

	   return true;
  }

  void SSD1306::_sendCommand(uint8_t command) {
  	  wire->beginTransmission(i2c_address);
  	  wire->write(0x00);
  	  wire->write(command);
  	  wire->endTransmission();
    }

  void SSD1306::_sendCommandList(const uint8_t *c, uint8_t n) {
	  wire->beginTransmission(i2c_address);
	  wire->write(0x00);
	  uint16_t bytesOut = 1;
	  while(n--) {
		  if(bytesOut >= 256) {
			  wire->endTransmission();
			  wire->beginTransmission(i2c_address);
			  wire->write(0x00);
			  bytesOut = 1;
		  }
		  wire->write(pgm_read_byte(c++));
		  bytesOut++;
	  }
	  wire->endTransmission();
  }

  // Effacer l'écran
  void SSD1306::clear() {
	  memset(buffer, 0, width * ((height + 7) / 8));
  }

  // Dessiner un pixel
  void SSD1306::drawPixel(int x, int y) {
	  y = height - y - 1;
	  buffer[(width - x - 1) + (y / 8) * width] |= (1 << (y & 7));

  }

  // Dessiner un caractère
  //void drawChar(int x, int y, char c, uint8_t size);

  // Dessiner un string
  //void drawString(int x, int y, const char* str, uint8_t size, bool color = true);

  // Dessiner un rectangle
  void SSD1306::drawRect(int x, int y, int w, int h) {
	  for (int i = x; i < x + w; i++) {
	      drawPixel(i, y);
	      drawPixel(i, y + h - 1);
	    }

	    for (int i = y; i < y + h; i++) {
	      drawPixel(x, i);
	      drawPixel(x + w - 1, i);
	    }
  }

  // Dessiner un rectangle rempli
  //void drawRectFill(int x, int y, int w, int h, bool color = true);

  // Dessiner une ligne
  //void drawLine(int x0, int y0, int x1, int y1, bool color = true);

  // Mettre à jour l'affichage
  void SSD1306::update() {
	  static const uint8_t PROGMEM dlist1[] = {0x22,0,0xFF,0x21, 0};
	  _sendCommandList(dlist1, sizeof(dlist1));
	  _sendCommand(width - 1);

	  uint16_t count = width * ((height + 7) / 8);
	  uint8_t *ptr = buffer;
	  wire->beginTransmission(i2c_address);
	  wire->write((uint8_t)0x40);
	  uint16_t bytesOut = 1;
	  while (count--) {
		  if (bytesOut >= 256) {
			  wire->endTransmission();
	          wire->beginTransmission(i2c_address);
	          wire->write((uint8_t)0x40);
	          bytesOut = 1;
	      }
	      wire->write(*ptr++);
	      bytesOut++;
	  }
	  wire->endTransmission();
  }


