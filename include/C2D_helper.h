#ifndef FRIENDMII_C2D_HELPER_H
#define FRIENDMII_C2D_HELPER_H

#include <citro2d.h>

#define DEFAULT_BG            C2D_Color32(234, 234, 234, 255)
#define FRIEND_FILL           C2D_Color32(255, 196, 142, 255)
#define SELECTOR_COLOUR_LIGHT C2D_Color32(241, 241, 241, 255)
#define BLACK                 C2D_Color32(0, 0, 0, 255)
#define WHITE                 C2D_Color32(255, 255, 255, 255)
#define TEXT_COLOUR           C2D_Color32(82, 82, 82, 255)
#define TEXT_COLOUR2          C2D_Color32(82, 82, 82, 255)
#define TEXT_COLOUR_LIGHT     C2D_Color32(82, 60, 60, 255)
#define TEXT_COLOUR_LIGHT2    C2D_Color32(241, 164, 34, 255)

C3D_RenderTarget *RENDER_TOP, *RENDER_BOTTOM;
C2D_TextBuf staticBuf, dynamicBuf, sizeBuf;

typedef u32 Colour;

void Draw_EndFrame(void);
void Draw_Text(float x, float y, float size, Colour colour, const char *text);
void Draw_Textf(float x, float y, float size, Colour colour, const char* text, ...);
void Draw_GetTextSize(float size, float *width, float *height, const char *text);
float Draw_GetTextWidth(float size, const char *text);
float Draw_GetTextHeight(float size, const char *text);
bool Draw_Rect(float x, float y, float w, float h, Colour colour);
bool Draw_Image(C2D_Image image, float x, float y);
bool Draw_ImageScale(C2D_Image image, float x, float y, float scaleX, float scaleY);

#endif