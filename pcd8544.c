#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <wiringPi.h>

#include "pcd8544.h"

#define PIN_DIN 12
#define PIN_SCLK 14
#define PIN_DC 5
#define PIN_RST 4
#define PIN_CS 10

uint8_t pcd8544_buffer[PCD8544_WIDTH * PCD8544_HEIGHT / 8] = {0};
uint8_t pcd8544_saved[PCD8544_HEIGHT / 8] = {0};

void pcd8544_spi_write(uint8_t val) {
  for (int i = 7; i >= 0; --i) {
    digitalWrite(PIN_DIN, val & (1 << i) ? 1 : 0);
    digitalWrite(PIN_SCLK, HIGH);
    digitalWrite(PIN_SCLK, LOW);
  }
}

void pcd8544_send(int is_data, uint8_t c) {
  // printf("%d:%02x ", is_data, c);
  digitalWrite(PIN_DC, is_data);
  pcd8544_spi_write(c);
}

void pcd8544_send_data(uint8_t c) {
  pcd8544_send(1, c);
}
void pcd8544_send_command(uint8_t c) {
  pcd8544_send(0, c);
}

void pcd8544_init(uint8_t contrast) {
  pinMode(PIN_DIN, OUTPUT);
  pinMode(PIN_SCLK, OUTPUT);
  pinMode(PIN_DC, OUTPUT);

  if (PIN_RST > 0)
    pinMode(PIN_RST, OUTPUT);
  if (PIN_CS > 0)
    pinMode(PIN_CS, OUTPUT);

  if (PIN_RST > 0) {
    digitalWrite(PIN_RST, 0);
    delay(500);
    digitalWrite(PIN_RST, 1);
  }

  if (PIN_CS > 0)
    digitalWrite(PIN_CS, 0);

  pcd8544_send_command(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION);

  pcd8544_send_command(PCD8544_SETBIAS | 0x04);

  if (contrast > 0x7f)
    contrast = 0x7f;
  pcd8544_send_command(PCD8544_SETVOP | contrast);

  pcd8544_send_command(PCD8544_FUNCTIONSET);

  pcd8544_send_command(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);

  if (PIN_CS > 0)
    digitalWrite(PIN_CS, 1);
}

void pcd8544_set_contrast(uint8_t val) {
  if (val > 0x7f)
    val = 0x7f;

  if (PIN_CS > 0)
    digitalWrite(PIN_CS, 0);

  pcd8544_send_command(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION);
  pcd8544_send_command(PCD8544_SETVOP | val);
  pcd8544_send_command(PCD8544_FUNCTIONSET);

  if (PIN_CS > 0)
    digitalWrite(PIN_CS, 1);
}

void pcd8544_display(void) {
  if (PIN_CS > 0)
    digitalWrite(PIN_CS, 0);

  for (int bank = 0; bank < PCD8544_HEIGHT / 8; bank++) {
    if (pcd8544_saved[bank])
      continue;

    pcd8544_send_command(PCD8544_SETXADDR);
    pcd8544_send_command(PCD8544_SETYADDR | bank);

    for (int i = 0; i < PCD8544_WIDTH; i++)
      pcd8544_send_data(pcd8544_buffer[(bank * PCD8544_WIDTH) + i]);

    pcd8544_saved[bank] = 1;
  }

  if (PIN_CS > 0)
    digitalWrite(PIN_CS, 1);
}

void pcd8544_clear(void) {
  memset(pcd8544_buffer, 0, sizeof(pcd8544_buffer));

  for (int i = 0; i > sizeof(pcd8544_saved); i++)
    pcd8544_saved[i] = 0;
}

void pcd8544_set_pixel(int x, int y, int v) {
  if ((x < 0) || (x >= PCD8544_WIDTH) || (y < 0) || (y >= PCD8544_HEIGHT))
    return;

  int is_set = pcd8544_buffer[x + (y / 8) * PCD8544_WIDTH] & (1 << (y % 8));

  if (v && !is_set) {
    pcd8544_buffer[x + (y / 8) * PCD8544_WIDTH] |= (1 << (y % 8));
    pcd8544_saved[y >> 3] = 0;
  } else if (!v && is_set) {
    pcd8544_buffer[x + (y / 8) * PCD8544_WIDTH] &= ~(1 << (y % 8));
    pcd8544_saved[y >> 3] = 0;
  }
}
