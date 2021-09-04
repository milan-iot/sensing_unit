#ifndef _LED_H
#define _LED_H

#include <NeoPixelBus.h>

typedef enum {RED, GREEN, BLUE, YELLOW, PURPLE, CYAN, WHITE, BLACK} LED_COLOR;

void LED_init(void);
void LED_setSaturation(uint8_t saturation);
void LED_setColor(LED_COLOR color);

#endif
