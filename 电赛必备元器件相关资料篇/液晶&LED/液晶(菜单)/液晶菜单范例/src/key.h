#ifndef __KEY_H
#define __KEY_H

#include "typedef.h"

#define KEY_UP		0x01
#define KEY_DOWN 	0x08
#define KEY_LEFT	0x10
#define KEY_RIGHT	0x04
#define KEY_ENTER	0x02

extern void (*onKeyUp)(uchar key);
extern void (*onKeyDown)(uchar key);

void KeyInit();
uchar ReadKey(void) ;
void KeyEvent();


#endif