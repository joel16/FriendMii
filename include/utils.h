#ifndef FRIENDMII_UTILS_H
#define FRIENDMII_UTILS_H

bool Utils_IsN3DS(void);
void Utils_U16_To_U8(u8 *buf, const u16 *input, size_t bufsize);
void Utils_U8_To_U16(u16 *buf, const u8 *input, size_t bufsize);
void Utils_SetMax(int *set, int value, int max);
void Utils_SetMin(int *set, int value, int min);

#endif
