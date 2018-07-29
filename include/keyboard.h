#ifndef FRIENDMII_KEYBOARD_H
#define FRIENDMII_KEYBOARD_H

#include <3ds.h>

const char *Keyboard_GetString(const char *initial_text, const char *hint_text);
void Keyboard_GetFriendCode(void);

#endif
