#include "C2D_helper.h"
#include "sprites.h"
#include "textures.h"

static C2D_SpriteSheet spritesheet;

void Textures_Load(void)
{
	spritesheet = C2D_SpriteSheetLoad("romfs:/res/drawable/sprites.t3x");

	battery_1 = C2D_SpriteSheetGetImage(spritesheet, sprites_battery_1_idx);
	battery_2 = C2D_SpriteSheetGetImage(spritesheet, sprites_battery_2_idx);
	battery_3 = C2D_SpriteSheetGetImage(spritesheet, sprites_battery_3_idx);
	battery_4 = C2D_SpriteSheetGetImage(spritesheet, sprites_battery_4_idx);
	battery_charge = C2D_SpriteSheetGetImage(spritesheet, sprites_battery_charge_idx);
	icon_search = C2D_SpriteSheetGetImage(spritesheet, sprites_ic_material_light_search_idx);
	icon_add = C2D_SpriteSheetGetImage(spritesheet, sprites_ic_material_light_add_idx);
}

void Textures_Free(void)
{
	C2D_SpriteSheetFree(spritesheet);
}