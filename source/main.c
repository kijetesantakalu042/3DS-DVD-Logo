#include <citro2d.h>

#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240

int main(int argc, char* argv[]) {
    // Init various things
	romfsInit();
    gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	consoleInit(GFX_BOTTOM, NULL);
    
    // Create a C3D render target
    C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    
    // Store DVD logo size
    int logoSizeW = 80;
    int logoSizeH = 40;
    
    // DVD logo pos variables
    float logoPosX = SCREEN_WIDTH/2;
    float logoPosY = SCREEN_HEIGHT/2;
    
    float relLogoPosX;
    float relLogoPosY;
    
    // Make value for DVD logo velocity
    float logoVelX = 1;
    float logoVelY = 1;
    
    // Make variables to keep track of double bounces
    bool bounceY = false;
    bool bounceX = false;
    int bounceNum = 0;
    
    // Load the DVD Logo
	C2D_SpriteSheet sheet = C2D_SpriteSheetLoad("romfs:/gfx/logo.t3x");	
	C2D_Image logo = C2D_SpriteSheetGetImage(sheet, 0);
    
    while (aptMainLoop()) {
        hidScanInput();

		// Respond to user input
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu
        
		if (kDown & KEY_SELECT) {
            logoPosX = SCREEN_WIDTH/2;
            logoPosY = SCREEN_HEIGHT/2;
        }
        
        // Calculate bounce
        if (logoPosX + logoVelX == SCREEN_WIDTH - logoSizeW/2 || logoPosX + logoVelX == logoSizeW/2) {
            logoVelX = logoVelX * -1;
            bounceX = true;
        }
        if (logoPosY + logoVelY == SCREEN_HEIGHT - logoSizeH/2 || logoPosY + logoVelY == logoSizeH/2) {
            logoVelY = logoVelY * -1;
            bounceY = true;
        }
        
        if (bounceX & bounceY) {
            bounceNum++;
        }
        bounceX = bounceY = false;
        
        // Move DVD logo
        logoPosX = logoPosX + logoVelX;
        logoPosY = logoPosY + logoVelY;
        
        // Print debug Info
        printf("\x1b[0;0HlogoPosX:%f\nlogoPosY:%f\nlogoVelX:%f\nlogoVelY:%f\nBounce Times:%d", logoPosX, logoPosY, logoVelX, logoVelY, bounceNum);
        
        // Calculate real dvd logo pos
        relLogoPosX = logoPosX - logoSizeW/2;
        relLogoPosY = logoPosY - logoSizeH/2;
        
        // Render the scene
        C2D_TargetClear(top, C2D_Color32(0x00, 0x00, 0x00, 0xff));
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_SceneBegin(top);
        
        // Draw the Bird
        C2D_DrawImageAt(logo, relLogoPosX, relLogoPosY, 0, NULL, 1, 1);
        
        C3D_FrameEnd(0);
    };
    
    // Deinit libs
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	return 0;
}