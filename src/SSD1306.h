/*
 * SSD1306.h
 *
 *  Created on: 6 janv. 2023
 *      Author: guedesite
 */

#ifndef SSD1306_H
#define SSD1306_H

#include <Wire.h>
#include "font.h"

class SSD1306 {
 public:
  // Constructeur
  SSD1306(TwoWire *wire, uint8_t i2c_address);
  SSD1306(TwoWire *wire, uint8_t i2c_address, uint32_t clock, uint8_t width, uint8_t height);
  ~SSD1306(void);

  // Initialiser l'écran
  bool init();

  // Effacer l'écran
  void clear();

  // Dessiner un pixel
  void drawPixel(int x, int y);

  // Dessiner un caractère
  //void drawChar(int x, int y, char c, uint8_t size);

  // Dessiner un string
  //void drawString(int x, int y, const char* str, uint8_t size, bool color = true);

  // Dessiner un rectangle
  void drawRect(int x, int y, int w, int h);

  // Dessiner un rectangle rempli
 // void drawRectFill(int x, int y, int w, int h, bool color = true);

  // Dessiner une ligne
  //void drawLine(int x0, int y0, int x1, int y1, bool color = true);

  // Mettre à jour l'affichage
  void update();


 private:
  TwoWire *wire;
  uint8_t i2c_address;
  uint8_t width = 128;
  uint8_t height = 64;
  uint8_t *buffer;

  void _sendCommandList(const uint8_t *c, uint8_t n);

  void _sendCommand(uint8_t command);


};

#endif
