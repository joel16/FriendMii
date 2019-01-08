#include <3ds.h>

#include "C2D_helper.h"
#include "common.h"
#include "frd.h"
#include "fs.h"
#include "menu_main.h"
#include "textures.h"
#include "utils.h"

static u32 cpu_time_limit = 0;

static void Init_Services(void) {
	FS_OpenArchive(&archive, ARCHIVE_SDMC);
	frdInit();
	srvGetServiceHandle(&frdHandle, "frd:u");
	cfguInit();
	amInit();
	ptmuInit();
	
	romfsInit();
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	
	if (Utils_IsN3DS())
		osSetSpeedupEnable(true);

	APT_GetAppCpuTimeLimit(&cpu_time_limit);
	APT_SetAppCpuTimeLimit(30);

	staticBuf = C2D_TextBufNew(4096);
	dynamicBuf = C2D_TextBufNew(4096);
	sizeBuf = C2D_TextBufNew(4096);

	Textures_Load();

	RENDER_TOP = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	RENDER_BOTTOM = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
}

static void Term_Services(void) {
	C2D_TextBufDelete(sizeBuf);
	C2D_TextBufDelete(dynamicBuf);
	C2D_TextBufDelete(staticBuf);

	Textures_Free();

	if (cpu_time_limit != UINT32_MAX)
		APT_SetAppCpuTimeLimit(cpu_time_limit);

	if (Utils_IsN3DS())
		osSetSpeedupEnable(0);

	C2D_Fini();
	C3D_Fini();
	gfxExit();
	romfsExit();

	ptmuExit();
	amExit();
	cfguExit();
	svcCloseHandle(frdHandle);
	frdExit();
	FS_CloseArchive(archive);
}

int main(int argc, char **argv) {
	Init_Services();

	if (setjmp(exitJmp)) {
		Term_Services();
		return 0;
	}

	MENU_STATE = STATE_FRIENDCARD;
	Menu_Main();

	Term_Services();
	return 0;
}
