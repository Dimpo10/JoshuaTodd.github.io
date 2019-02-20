//my game
using namespace std;

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#define MB 10
#define MC 1000

#endif
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include <windows.h>
#include <ddraw.h>
#include <stdio.h>
#include <stdarg.h>
#include "resource.h"
#include "ddutil.h"
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <string.h>
#include <iostream>

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------

// Standard Windows program functions
static HRESULT InitApp(HINSTANCE, int); //start
HRESULT InitFail(HWND, HRESULT, LPCTSTR, ...); //if start fails
static void ReleaseAllObjects(void); //end
long FAR PASCAL WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); //if operating system gets in touch

// Our game functions
void Init(void);
void UpdateFrame(void);
void SpriteAnimation(void);
void Collision(void);
//int CollisionResolver(int x1, int x2, int y1, int y2);
void PlanetRender(int, int, RECT);
void CharacterRender(int, int, RECT);
void BulletRender(double, double, RECT);
void CharacterController();
void BulletLocation();
void BackgroundRender(int, int, RECT);
void MeteorRender(double, double, RECT);
void Menu1Render(int, int, RECT);
void Menu2Render(int, int, RECT);
void LivesRender(int, int, RECT);
void MeteorLocation();
void MenuSwitcher();

//-----------------------------------------------------------------------------
// Local definitions
//-----------------------------------------------------------------------------
#define NAME                "DDExample4"
#define TITLE               "Direct Draw Example 4"

#define MX	32
#define MY  24

#define KEY_DOWN(vk_code) GetAsyncKeyState(vk_code)
#define KEY_UP(vk_code) GetAsyncKeyState(vk_code)

//-----------------------------------------------------------------------------
// Global data
//-----------------------------------------------------------------------------
LPDIRECTDRAW4               g_pDD = NULL;        // DirectDraw object
LPDIRECTDRAWSURFACE4        g_pDDSPrimary = NULL;// DirectDraw primary surface
LPDIRECTDRAWSURFACE4        g_pDDSBack = NULL;   // DirectDraw back surface
LPDIRECTDRAWSURFACE4        g_pDDSOne = NULL;    // Offscreen image 1
LPDIRECTDRAWSURFACE4        g_pDDStwo = NULL;    // Offscreen image 2
LPDIRECTDRAWSURFACE4        g_pDDSthree = NULL;    // Offscreen image 3
LPDIRECTDRAWSURFACE4        g_pDDSfour = NULL;    // Offscreen image 4
LPDIRECTDRAWSURFACE4		FontSurface = NULL;

LPDIRECTDRAWPALETTE         g_pDDPal = NULL;     // The primary surface palette
BOOL                        g_bActive = FALSE;   // Is application active?

int screenWidth = 1024;
int screenHeight = 768;

//-----------------------------------------------------------------------------
// Local data
//-----------------------------------------------------------------------------
// Name of our bitmap resource.

static char                 szBitmap[] = "ALL"; // main bitmap
static char                 szBitmap2[] = "METEORS1"; // menu 1 bitmap
static char                 szBitmap3[] = "METEORS2"; // menu 2 bitmap
static char                 szBitmap1[] = "TILE"; // bitmap not used

bool collision = false;
int chspritex = screenWidth/2 - 13;
int chspritey = screenHeight/2 - 13;
int plspritex = screenWidth / 2 - 150 / 2;
int plspritey = screenHeight / 2 - 150 / 2;
double bspritex = screenWidth / 2 - 12;
double bspritey = screenHeight / 2 - 12;
double mespritex = screenWidth / 2 - 13;
double mespritey = screenHeight / 2 - 13;
int baspritex = 0;
int baspritey = 0;
int xSprite = 209;
int ySprite = 0;
int x2Sprite = 241;
int y2Sprite = 32;
float angle = 0;
float bulletAngle = 0;
int radius = 120;
double bulletRadius = 140;
double meteorRadius = 700;
DWORD thisTickCount;
double acceleration = 0.005;
int count = 0;
double br[MB], ba[MB], bA[MB];
double mr[MC], ma[MC], mA[MC];
static int currentFrame = 0;
int start[2];
int end[2];
int z = 0;
int k = 0;
int counter1 = 0;
int counter2 = 0;
int planethealth = 3;
int score = 0;

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Entry point to DX/CPP program.
//-----------------------------------------------------------------------------
int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) //Initialization, message loop
{
	MSG msg;

	//Run InitApp
	if (InitApp(hInstance, nCmdShow) != DD_OK) return FALSE;

	//GameLoop - remains in here once program is setup
	while (TRUE)
	{
		//Handle system message if appropriate
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, 0, 0)) return msg.wParam;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}else if (g_bActive) { 
			//Otherwise, if this program is active, update our game loop
			UpdateFrame(); 
		}
		else { 
			//Otherwise hold fire
			WaitMessage();/* Make sure we go to sleep if we have nothing else to do*/ 
		}
	}
}

//-----------------------------------------------------------------------------
// Name: InitApp()
// Desc: Do work required for every instance of the application:
//          Create the window, initialize data
//-----------------------------------------------------------------------------
static HRESULT InitApp(HINSTANCE hInstance, int nCmdShow)
{
	HWND                        hWnd;
	WNDCLASS                    wc;
	DDSURFACEDESC2              ddsd;
	DDSCAPS2                    ddscaps;
	HRESULT                     hRet;
	LPDIRECTDRAW                pDD;

	// Set up and register window class
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NAME;
	wc.lpszClassName = NAME;
	RegisterClass(&wc);

	// Create a window
	hWnd = CreateWindowEx(WS_EX_TOPMOST,
		NAME,
		TITLE,
		WS_POPUP,
		0,
		0,
		1024,
		768,
		NULL,
		NULL,
		hInstance,
		NULL);
	if (!hWnd)
		return FALSE;
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	SetFocus(hWnd);

	///////////////////////////////////////////////////////////////////////////
	// Create the main DirectDraw object
	///////////////////////////////////////////////////////////////////////////
	hRet = DirectDrawCreate(NULL, &pDD, NULL);
	if (hRet != DD_OK)
		return InitFail(hWnd, hRet, "DirectDrawCreate FAILED");

	// Fetch DirectDraw4 interface
	hRet = pDD->QueryInterface(IID_IDirectDraw4, (LPVOID *)& g_pDD);
	if (hRet != DD_OK)
		return InitFail(hWnd, hRet, "QueryInterface FAILED");

	// Get exclusive mode
	hRet = g_pDD->SetCooperativeLevel(hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
	if (hRet != DD_OK)
		return InitFail(hWnd, hRet, "SetCooperativeLevel FAILED");

	// Set the video mode 
	hRet = g_pDD->SetDisplayMode(1024, 768, 32, 0, 0);
	if (hRet != DD_OK)
		return InitFail(hWnd, hRet, "SetDisplayMode FAILED");

	// Create the primary surface with 1 back buffer
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |
		DDSCAPS_FLIP |
		DDSCAPS_COMPLEX;
	ddsd.dwBackBufferCount = 1;
	hRet = g_pDD->CreateSurface(&ddsd, &g_pDDSPrimary, NULL);
	if (hRet != DD_OK) return InitFail(hWnd, hRet, "CreateSurface FAILED");

	// Get a pointer to the back buffer
	ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
	hRet = g_pDDSPrimary->GetAttachedSurface(&ddscaps, &g_pDDSBack);
	if (hRet != DD_OK) return InitFail(hWnd, hRet, "GetAttachedSurface FAILED");

	// Create the offscreen surface, by loading our bitmap.
	g_pDDSOne = DDLoadBitmap(g_pDD, szBitmap, 0, 0);
	if (g_pDDSOne == NULL) return InitFail(hWnd, hRet, "load background FAILED");

	// Set the color key for this bitmap (green)
	DDSetColorKey(g_pDDSOne, RGB(0, 255, 0));

	//load tile bitmap
	g_pDDStwo = DDLoadBitmap(g_pDD, szBitmap1, 0, 0);
	if (g_pDDStwo == NULL) return InitFail(hWnd, hRet, "load tiles FAILED");

	// Set the color key for this bitmap (green)
	DDSetColorKey(g_pDDStwo, RGB(0, 255, 0));

	g_pDDSthree = DDLoadBitmap(g_pDD, szBitmap2, 0, 0);
	if (g_pDDStwo == NULL) return InitFail(hWnd, hRet, "load tiles FAILED");

	DDSetColorKey(g_pDDSthree, RGB(0, 255, 0));

	g_pDDSfour = DDLoadBitmap(g_pDD, szBitmap3, 0, 0);
	if (g_pDDStwo == NULL) return InitFail(hWnd, hRet, "load tiles FAILED");

	DDSetColorKey(g_pDDSfour, RGB(0, 255, 0));

	//Run our game initialisation function
	Init();

	return DD_OK;
}


void Init(void)
{
	
}


//-----------------------------------------------------------------------------
// Name: InitFail()
// Desc: This function is called if an initialization function fails
//-----------------------------------------------------------------------------
HRESULT InitFail(HWND hWnd, HRESULT hRet, LPCTSTR szError, ...)
{
	char                        szBuff[128];
	va_list                     vl;

	va_start(vl, szError);
	vsprintf(szBuff, szError, vl);
	ReleaseAllObjects();
	MessageBox(hWnd, szBuff, TITLE, MB_OK);
	DestroyWindow(hWnd);
	va_end(vl);
	return hRet;
}


//-----------------------------------------------------------------------------
// Name: RestoreAll()
// Desc: Restore all lost objects
//-----------------------------------------------------------------------------
HRESULT RestoreAll(void)
{
	HRESULT                     hRet;

	hRet = g_pDDSPrimary->Restore();
	if (hRet == DD_OK)
	{
		hRet = g_pDDSOne->Restore();
		if (hRet == DD_OK)
		{
			DDReLoadBitmap(g_pDDSOne, szBitmap);
		}
		hRet = g_pDDSthree->Restore();
		if (hRet == DD_OK)
		{
			DDReLoadBitmap(g_pDDSthree, szBitmap2);
		}
		hRet = g_pDDSfour->Restore();
		if (hRet == DD_OK)
		{
			DDReLoadBitmap(g_pDDSfour, szBitmap3);
		}

	}
	return hRet;
}

//-----------------------------------------------------------------------------
// Name: UpdateFrame()
// Desc: Decide what needs to be blitted next, wait for flip to complete,
//       then flip the buffers.
//-----------------------------------------------------------------------------
void UpdateFrame(void)
{
	static DWORD    lastTickCount = 0;

	DWORD           delay = 13;
									//x  y  x2   y2
	RECT            planetSprRect = { 0, 0, 149, 150 }; //Rectangle for grabbing image from sprite sheet
	RECT            characterSprRect = { 149, 0, 189, 49 };
	RECT			backSprRect = { 300, 300, 1324, 1065 };


	int				maxFrame = 59; //60 frames of animation -1 as we start from 0
	 //used to store timing var for animation

	HRESULT         hRet;

	// Milliseconds since program started
	thisTickCount = GetTickCount();

	//Animation step
	if ((thisTickCount - lastTickCount) > delay)
	{
		//Move to next frame;
		lastTickCount = thisTickCount;
		currentFrame++;
		if (currentFrame > maxFrame) currentFrame = 0;
	}

	MenuSwitcher();

	if (KEY_DOWN(VK_RETURN))
	{
		start[z] = 1; // pass a value into the array
	}

	if (start[z] == 1) // runs if the button is pressed once, keeps running
	{
		CharacterController();

		BackgroundRender(baspritex, baspritey, backSprRect);

		CharacterRender(chspritex + 4, chspritey + 4, characterSprRect);

		PlanetRender(plspritex, plspritey, planetSprRect);

		BulletLocation();

		MeteorLocation();	
		
		Collision();

		SpriteAnimation();
	}


	// Flip the buffers
	while (TRUE)
	{
		hRet = g_pDDSPrimary->Flip(NULL, 0);
		if (hRet == DD_OK) break;
		if (hRet == DDERR_SURFACELOST) {
			hRet = RestoreAll();
			if (hRet != DD_OK) break;
		}
		if (hRet != DDERR_WASSTILLDRAWING) break;
	}
}

void SpriteAnimation(void)
{
	if (currentFrame % 20 == 0) // runs every 20 frames
	{
		
		xSprite += 32; // increment sprite grabbing value by 32
		x2Sprite += 32;

		if (x2Sprite > 528) // if the renderer goes paast the last sprite on the bitmap
		{
			xSprite = 209; // send it back to the first sprite
			x2Sprite = 241;
		}
	}

	
}

void Collision(void)
{
	for (int i = 0; i < MB; i++) // check for every bullet
	{ 
		if (ba[i] == 1) 
		{
			for (int a = 0; a < MC; a++) // check for every meteor
			{ 
				if (ma[a] == 1) // if both bullet and meteor are active do this
				{
					bspritex = screenWidth / 2 - 12 + br[i] * cos(bA[i]); // check the location of the meteors and bullets
					bspritey = screenHeight / 2 - 12 + br[i] * sin(bA[i]);
					mespritex = screenWidth / 2 - 25 + mr[a] * cos(mA[a]);
					mespritey = screenHeight / 2 - 25 + mr[a] * sin(mA[a]);
					if ((mespritex - bspritex)*(mespritex - bspritex) + (mespritey - bspritey)*(mespritey - bspritey) < (25 * 25)) // if they collide deactivate them
					{
						ba[i] = 0;
						ma[a] = 0;

						score++; // increment score
					}
				}
			}
		}
	}
}

void PlanetRender(int x, int y, RECT cropRect) // code that renders the sprite, sprite determined by the variable passed into cropRect
{

	HRESULT         hRet;

	while (TRUE)
	{
		hRet = g_pDDSBack->BltFast(x, y, g_pDDSOne, &cropRect, DDBLTFAST_SRCCOLORKEY);
		if (hRet == DD_OK) break;
		if (hRet == DDERR_SURFACELOST) {
			hRet = RestoreAll();
			if (hRet != DD_OK)
				return;
		}
		if (hRet != DDERR_WASSTILLDRAWING)
			return;
	}
} 

void CharacterRender(int x, int y, RECT cropRect)
{
	HRESULT         hRet;

	while (TRUE)
	{
		hRet = g_pDDSBack->BltFast(x, y, g_pDDSOne, &cropRect, DDBLTFAST_SRCCOLORKEY);
		if (hRet == DD_OK) break;
		if (hRet == DDERR_SURFACELOST) {
			hRet = RestoreAll();
			if (hRet != DD_OK)
				return;
		}
		if (hRet != DDERR_WASSTILLDRAWING)
			return;
	}
}

void BackgroundRender(int x, int y, RECT cropRect)
{
	HRESULT         hRet;

	while (TRUE)
	{
		hRet = g_pDDSBack->BltFast(x, y, g_pDDSOne, &cropRect, DDBLTFAST_SRCCOLORKEY);
		if (hRet == DD_OK) break;
		if (hRet == DDERR_SURFACELOST) {
			hRet = RestoreAll();
			if (hRet != DD_OK)
				return;
		}
		if (hRet != DDERR_WASSTILLDRAWING)
			return;
	}
}

void Menu1Render(int x, int y, RECT cropRect)
{
	HRESULT         hRet;

	while (TRUE)
	{
		hRet = g_pDDSBack->BltFast(x, y, g_pDDSthree, &cropRect, DDBLTFAST_SRCCOLORKEY);
		if (hRet == DD_OK) break;
		if (hRet == DDERR_SURFACELOST) {
			hRet = RestoreAll();
			if (hRet != DD_OK)
				return;
		}
		if (hRet != DDERR_WASSTILLDRAWING)
			return;
	}
}

void Menu2Render(int x, int y, RECT cropRect)
{
	HRESULT         hRet;

	while (TRUE)
	{
		hRet = g_pDDSBack->BltFast(x, y, g_pDDSfour, &cropRect, DDBLTFAST_SRCCOLORKEY);
		if (hRet == DD_OK) break;
		if (hRet == DDERR_SURFACELOST) {
			hRet = RestoreAll();
			if (hRet != DD_OK)
				return;
		}
		if (hRet != DDERR_WASSTILLDRAWING)
			return;
	}
}

void MeteorRender(double x, double y, RECT cropRect)
{
	HRESULT         hRet;

	while (TRUE)
	{
		hRet = g_pDDSBack->BltFast(x, y, g_pDDSOne, &cropRect, DDBLTFAST_SRCCOLORKEY);
		if (hRet == DD_OK) break;
		if (hRet == DDERR_SURFACELOST) {
			hRet = RestoreAll();
			if (hRet != DD_OK)
				return;
		}
		if (hRet != DDERR_WASSTILLDRAWING)
			return;
	}
}

void BulletRender(double x, double y, RECT cropRect)
{
	HRESULT         hRet;

	while (TRUE)
	{
		hRet = g_pDDSBack->BltFast(x, y, g_pDDSOne, &cropRect, DDBLTFAST_SRCCOLORKEY);
		if (hRet == DD_OK) break;
		if (hRet == DDERR_SURFACELOST) {
			hRet = RestoreAll();
			if (hRet != DD_OK)
				return;
		}
		if (hRet != DDERR_WASSTILLDRAWING)
			return;
	}
}

void LivesRender(int x, int y, RECT cropRect)
{
	HRESULT         hRet;

	while (TRUE)
	{
		hRet = g_pDDSBack->BltFast(x, y, g_pDDSOne, &cropRect, DDBLTFAST_SRCCOLORKEY);
		if (hRet == DD_OK) break;
		if (hRet == DDERR_SURFACELOST) {
			hRet = RestoreAll();
			if (hRet != DD_OK)
				return;
		}
		if (hRet != DDERR_WASSTILLDRAWING)
			return;
	}
}

void CharacterController()
{

	chspritex = screenWidth / 2 - 25 + radius * cos(angle); // put the character in the center of the screen
	chspritey = screenHeight / 2 - 25 + radius * sin(angle);

	if(KEY_DOWN(VK_RIGHT)) // increment angle when pressing right to go clockwise
	{
		angle += 0.035;
	}

	else if(KEY_DOWN(VK_LEFT)) // opposite
	{
		angle -= 0.035;
	}
}

void BulletLocation()
{
	RECT	bulletSprRect = { 189, 0, 209, 20 }; // render sprite

	if (KEY_DOWN(VK_SPACE) && currentFrame % 10 == 0) // is spacebar is pressed every 10 frames
	{
		for (int counter1 = 0; counter1 < MB; counter1++) // increment counter until 10
		{
			if (ba[counter1] == 0 && currentFrame % 10 == 0) // if bullet is incative on a 10th frame
			{
				br[counter1] = bulletRadius; // pass values into arrays
				ba[counter1] = 1; // make bullet active
				bA[counter1] = angle;

				counter1 = MB;
			}
		}
	}
	for (int counter1 = 0; counter1 < MB; counter1++)//first check if bullet is active
	{
		if (ba[counter1] == 1) // if bullet is active
		{
			br[counter1] += 10; //move bullet 10 pixels each frame
			bspritex = screenWidth / 2 - 12 + br[counter1] * cos(bA[counter1]); // dput bullet at this location
			bspritey = screenHeight / 2 - 12 + br[counter1] * sin(bA[counter1]);

			BulletRender(bspritex, bspritey, bulletSprRect); // render the bullet

			if (br[counter1] > 650)//if bullet at edge of screen
			{
				ba[counter1] = 0;//make the bullet inactive	
			}
		}
	}
}

void MeteorLocation()
{
	srand(time(NULL)*GetTickCount()); // seed random number generator

	double random = rand() % (360 - 1 + 1) + 1; // pass random a random value each time
	double meteorAngle = random; // make meteor angle random
	
	RECT meteorSprRect = { xSprite, ySprite, x2Sprite, y2Sprite };

	if (currentFrame % 20 == 0) // run every 20 frames
	{
		for (int counter2 = 0; counter2 < MC; counter2++) // start a counter to run until 1000
		{
			if (ma[counter2] == 0 && currentFrame % 20 == 0)  // if meteor is inactive and its a 20th frame
			{
				mr[counter2] = meteorRadius; // pass values to arrays
				ma[counter2] = 1;
				mA[counter2] = meteorAngle;

				counter2 = MC;
			}
		}
	}
	for (int counter2 = 0; counter2 < MC; counter2++)//first check if meteor is active
	{
		if (ma[counter2] == 1) // is meteor is active
		{
			mr[counter2] -= 0.35; //move meteor 0.35 pixels each frame
			mespritex = screenWidth / 2 - 25 + mr[counter2] * cos(mA[counter2]); //set location of x and y
			mespritey = screenHeight / 2 - 25 + mr[counter2] * sin(mA[counter2]);

			MeteorRender(mespritex, mespritey, meteorSprRect); // render meteors

			if (planethealth == 3) // is the health of the planet is 3, display 3 lives
			{
				RECT lifeSprRect = { 528, 0, 626, 32 };
				LivesRender(20, 20, lifeSprRect);
			}
			if (planethealth == 2)// is the health of the planet is 2, display 2 lives
			{
				RECT lifeSprRect = { 528, 0, 594, 32 };
				LivesRender(20, 20, lifeSprRect);
			}

			if (planethealth == 1)// is the health of the planet is 1, display 1 life
			{
				RECT lifeSprRect = { 528, 0, 562, 32 };
				LivesRender(20, 20, lifeSprRect);
			}

			if (mr[counter2] < 100)//if meteor is at planet radius
			{
				ma[counter2] = 0;//make the meteor inactive	

				planethealth -= 1; // decrement planet health by 1

				if (planethealth < 1) // if plate health is less than 1
				{
					start[z] = 2; // show menu screen
					planethealth = 3; // reset the planets health
				}
			}
			if (start[z] == 2)
			{
				ma[counter2] = 0;
			}

		}
	}
}

void MenuSwitcher()
{
	RECT			menu1SprRect = { 0, 0, 1024, 768 };
	RECT			menu2SprRect = { 0, 0, 1024, 768 };

	k++; // increment

	if (k < 80) // if i is less than 80 display menu 1
	{
		Menu1Render(0, 0, menu1SprRect);
	}
	if (k > 80 && k < 160) // if i is more than 80 and less than 160 display menu 2
	{
		Menu2Render(0, 0, menu2SprRect);
	}
	if (k > 160) // set i back to 0 if it is greater than 160
	{
		k = 0;
	}
}

//-----------------------------------------------------------------------------
// Name: WindowProc()
// Desc: Called by the operating system when a system event occurs.
//-----------------------------------------------------------------------------

long FAR PASCAL WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) //The Main Window Procedure
{
	switch (message)
	{
	case WM_ACTIVATEAPP:
		// Pause if minimized or not the top window
		g_bActive = (wParam == WA_ACTIVE) || (wParam == WA_CLICKACTIVE);
		return 0L;

	case WM_DESTROY:
		// Clean up and close the app
		ReleaseAllObjects();
		PostQuitMessage(0);
		return 0L;

	case WM_KEYDOWN:
		// Handle any non-accelerated key commands
		switch (wParam)
		{
		case VK_ESCAPE:
		case VK_F12:
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			return 0L;
		}
		break;

	case WM_SETCURSOR:
		// Turn off the cursor since this is a full-screen app
		SetCursor(NULL);
		return TRUE;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

//-----------------------------------------------------------------------------
// Name: ReleaseAllObjects()
// Desc: Finished with all objects we use; release them. Called when closing program.
//-----------------------------------------------------------------------------
static void ReleaseAllObjects(void)
{
	if (g_pDD != NULL)
	{
		if (g_pDDSPrimary != NULL)
		{
			g_pDDSPrimary->Release();
			g_pDDSPrimary = NULL;
		}
		if (g_pDDSOne != NULL)
		{
			g_pDDSOne->Release();
			g_pDDSOne = NULL;
		}
		if (g_pDDSthree != NULL)
		{
			g_pDDSthree->Release();
			g_pDDSthree = NULL;
		}
		if (g_pDDSfour != NULL)
		{
			g_pDDSfour->Release();
			g_pDDSfour= NULL;
		}

		if (g_pDDPal != NULL)
		{
			g_pDDPal->Release();
			g_pDDPal = NULL;
		}
		g_pDD->Release();
		g_pDD = NULL;
	}
}