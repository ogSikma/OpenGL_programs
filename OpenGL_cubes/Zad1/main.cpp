#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>
#include <string>
#include <math.h>
#include <time.h>
#include <vector>

HDC			hDC = NULL;
HGLRC		hRC = NULL;
HWND		hWnd = NULL;
HINSTANCE	hInstance;
bool	keys[256];
bool	active = TRUE;
bool	fullscreen = false;

int tryb = 1;
const GLint liczbaSzescianow = 3;

GLfloat kostkaWymiary = 1.0f;

GLfloat kat_obrotu_wszystk = 0.0f;

GLfloat kat_pojedynczego = 0.0f;

GLfloat odstep = 1.5f;
GLfloat minimalny_odstep = 1.5f;
GLfloat maksymalny_odstep = 2.2f;

bool powiekszanie = true;
GLfloat kolory[3] = { 0.0f, 0.5f, 1.0f };
bool k[3] = { true, true, true };

GLfloat predkoscKat1 = -0.5f;
GLfloat predkoscKat2 = 1.0f;
GLfloat predkoscPrzerwa = 0.01f;
GLfloat predkoscKolor = 0.025f;

GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightPosition[] = { -5.0f, 0.0f, -5.0f, -1.0f };
GLfloat ambientMaterial[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat diffuseMaterial[] = { 0.4f, 0.8f, 0.5f, 1.0f };

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

void Rzutnia(GLsizei width, GLsizei height)
{
	if (height == 0)						
	{
		height = 1;
	}
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 50.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int Init(GLvoid)										
{
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);	
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);	
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	return TRUE;
}

void drawCube_GetSizeRotation(GLfloat rozmiar, GLfloat rotacja) 
{
	glPushMatrix();
	glRotatef(rotacja, 0.0f, 1.0f, 0.0f);
	auxSolidBox(rozmiar, rozmiar, rozmiar);
	glPopMatrix();
}

void UstalKolory(GLint x, GLint y, GLint z)
{
	if (x == 0 && z == 0 && y == liczbaSzescianow - 1)
	{
		glColor3f(kolory[0], kolory[1], kolory[2]);
	}
	else if (x == 1 && z == 1 && y == liczbaSzescianow - 1)
	{
		glColor3f(kolory[1], kolory[2], kolory[0]);
	}
	else if (x == liczbaSzescianow - 1 && z == liczbaSzescianow - 1 && y == liczbaSzescianow - 1)
	{
		glColor3f(kolory[2], kolory[0], kolory[1]);
	}
	else
	{
		glColor3f(0.4f, 0.1f, 0.1f);
	}
}


int Display(GLvoid)									
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -10);	// kamera
	glRotatef(25, 1.0f, 0.0f, 0.0f);

	glRotatef(kat_obrotu_wszystk, 0.0f, 1.0f, 0.0f);
	glTranslatef(-(2) * odstep / 2, -(2) * odstep / 2, -(2) * odstep / 2);

	for (int x = 0; x < liczbaSzescianow; x++)
	{
		glPushMatrix();
		glTranslatef(x * odstep, 0.0f, 0.0f);
		for (int y = 0; y < liczbaSzescianow; y++)
		{
			glPushMatrix();
			glTranslatef(0.0f, y * odstep, 0.0f);
			for (int z = 0; z < liczbaSzescianow; z++)
			{
				glPushMatrix();
				glTranslatef(0.0f, 0.0f, z * odstep);
				UstalKolory(x, y, z);
				drawCube_GetSizeRotation(kostkaWymiary, kat_pojedynczego);
				glPopMatrix();
			}
			glPopMatrix();
		}
		glPopMatrix();
	}

// TRYB 1 = ZWYK�Y
	if (tryb == 1)	
	{
		kat_obrotu_wszystk = kat_obrotu_wszystk + predkoscKat1;
		if (kat_obrotu_wszystk > 360)
			kat_obrotu_wszystk = kat_obrotu_wszystk - 360;
		if (kat_obrotu_wszystk < 0)
			kat_obrotu_wszystk = kat_obrotu_wszystk + 360;
	}

// TRYB 2 = OBR�T POJEDYNCZYCH OBIEKT�W
	else if (tryb == 2)	
	{
		kat_pojedynczego = kat_pojedynczego + predkoscKat2;
		if (kat_pojedynczego > 360)
			kat_pojedynczego = kat_pojedynczego - 360;
		if (kat_pojedynczego < 0)
			kat_pojedynczego = kat_pojedynczego + 360;
	}

// TRYB 3 = ROZSZERZANIE OBIEKT�W OD SIEBIE
	else if (tryb == 3)	
	{
		if (powiekszanie)											
			odstep = odstep + predkoscPrzerwa;
		else														
			odstep = odstep - predkoscPrzerwa;

		if (odstep >= maksymalny_odstep || odstep <= minimalny_odstep)
			powiekszanie = !powiekszanie;
	}

// TRYB 4 = ZMIANA KOLORU SZCZYTOWYCH OBIEKT�W
	else if (tryb == 4)
	{
		if (k[0])													
		{
			kolory[0] = kolory[0] + predkoscKolor;				
			if (kolory[0] > 1.0f)								
			{
				kolory[0] = 1.0f;
				k[0] = !k[0];
			}
		}
		else
		{
			kolory[0] = kolory[0] - predkoscKolor;			
			if (kolory[0] < 0.0f)									
			{
				kolory[0] = 0.0f;
				k[0] = !k[0];
			}
		}
		if (k[1])
		{
			kolory[1] = kolory[1] + predkoscKolor;
			if (kolory[1] > 1.0f)
			{
				kolory[1] = 1.0f;
				k[1] = !k[1];
			}
		}
		else
		{
			kolory[1] = kolory[1] - predkoscKolor;
			if (kolory[1] < 0.0f)
			{
				kolory[1] = 0.0f;
				k[1] = !k[1];
			}
		}
		if (k[2])
		{
			kolory[2] = kolory[2] + predkoscKolor;
			if (kolory[2] > 1.0f)
			{
				kolory[2] = 1.0f;
				k[2] = !k[2];
			}
		}
		else
		{
			kolory[2] = kolory[2] - predkoscKolor;
			if (kolory[2] < 0.0f)
			{
				kolory[2] = 0.0f;
				k[2] = !k[2];
			}
		}
	}
	return TRUE;
}

GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL, 0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL, NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		hRC = NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd, hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hDC = NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL", hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/

BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left = (long)0;			// Set Left Value To 0
	WindowRect.right = (long)width;		// Set Right Value To Requested Width
	WindowRect.top = (long)0;				// Set Top Value To 0
	WindowRect.bottom = (long)height;		// Set Bottom Value To Requested Height

	fullscreen = fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance = GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc = (WNDPROC)WndProc;					// WndProc Handles Messages
	wc.cbClsExtra = 0;									// No Extra Window Data
	wc.cbWndExtra = 0;									// No Extra Window Data
	wc.hInstance = hInstance;							// Set The Instance
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground = NULL;									// No Background Required For GL
	wc.lpszMenuName = NULL;									// We Don't Want A Menu
	wc.lpszClassName = "OpenGL";								// Set The Class Name

	MessageBox(NULL, "Zadanie 1 sze�ciany. Prze��czaj mi�dzy trybami:\n 1-zwyk�y; 2-obr�t wok� �rodka ci�ko�ci; 3-rozszerzanie; 4-zmiana kolor�w", "Info", MB_OK | MB_ICONEXCLAMATION);

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth = width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight = height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel = bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
			{
				fullscreen = FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle = WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle = WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle = WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd = CreateWindowEx(dwExStyle,							// Extended Style For The Window
		"OpenGL",							// Class Name
		title,								// Window Title
		dwStyle |							// Defined Window Style
		WS_CLIPSIBLINGS |					// Required Window Style
		WS_CLIPCHILDREN,					// Required Window Style
		0, 0,								// Window Position
		WindowRect.right - WindowRect.left,	// Calculate Window Width
		WindowRect.bottom - WindowRect.top,	// Calculate Window Height
		NULL,								// No Parent Window
		NULL,								// No Menu
		hInstance,							// Instance
		NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(hDC = GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC = wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!wglMakeCurrent(hDC, hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd, SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	Rzutnia(width, height);					// Set Up Our Perspective GL Screen

	if (!Init())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}

LRESULT CALLBACK WndProc(HWND	hWnd,			// Handle For This Window
	UINT	uMsg,			// Message For This Window
	WPARAM	wParam,			// Additional Message Information
	LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
	case WM_ACTIVATE:							// Watch For Window Activate Message
	{
		if (!HIWORD(wParam))					// Check Minimization State
		{
			active = TRUE;						// Program Is Active
		}
		else
		{
			active = FALSE;						// Program Is No Longer Active
		}

		return 0;								// Return To The Message Loop
	}

	case WM_SYSCOMMAND:							// Intercept System Commands
	{
		switch (wParam)							// Check System Calls
		{
		case SC_SCREENSAVE:					// Screensaver Trying To Start?
		case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
			return 0;							// Prevent From Happening
		}
		break;									// Exit
	}

	case WM_CLOSE:								// Did We Receive A Close Message?
	{
		PostQuitMessage(0);						// Send A Quit Message
		return 0;								// Jump Back
	}

	case WM_KEYDOWN:							// Is A Key Being Held Down?
	{
		keys[wParam] = TRUE;					// If So, Mark It As TRUE
		return 0;								// Jump Back
	}

	case WM_KEYUP:								// Has A Key Been Released?
	{
		keys[wParam] = FALSE;					// If So, Mark It As FALSE
		return 0;								// Jump Back
	}

	case WM_SIZE:								// Resize The OpenGL Window
	{
		Rzutnia(LOWORD(lParam), HIWORD(lParam));  // LoWord=Width, HiWord=Height
		return 0;								// Jump Back
	}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE	hInstance,			// Instance
	HINSTANCE	hPrevInstance,		// Previous Instance
	LPSTR		lpCmdLine,			// Command Line Parameters
	int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done = FALSE;								// Bool Variable To Exit Loop

	// Create Our OpenGL Window
	if (!CreateGLWindow("OpenGL zadanie 1 - sze�ciany 4 tryby", 800, 600, 16, fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}
	int last_time = 0;
	while (!done)									// Loop That Runs While done=FALSE
	{

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message == WM_QUIT)				// Have We Received A Quit Message?
			{
				done = TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if (active)								// Program Active?
			{
				if (keys[VK_ESCAPE])				// Was ESC Pressed?
				{
					done = TRUE;						// ESC Signalled A Quit
				}
				else
				{
					int time = GetTickCount();
					if (time - last_time >= 20)
					{
						Display();
						SwapBuffers(hDC);
						last_time = time;
					}

				}
			}
			if (keys[VK_F1])						// Is F1 Being Pressed?
			{
				keys[VK_F1] = FALSE;					// If So Make Key FALSE
				KillGLWindow();						// Kill Our Current Window
				fullscreen = !fullscreen;				// Toggle Fullscreen / Windowed Mode
				// Recreate Our OpenGL Window
				if (!CreateGLWindow("OpenGL zadanie 1 - sze�ciany 4 tryby", 800, 600, 16, fullscreen))
				{
					return 0;						// Quit If Window Was Not Created
				}
			}
			if (keys['1'])
			{
				keys['1'] = false;
				tryb = 1;
			}
			if (keys['2'])
			{
				keys['2'] = false;
				tryb = 2;
			}
			if (keys['3'])
			{
				keys['3'] = false;
				tryb = 3;
			}
			if (keys['4'])
			{
				keys['4'] = false;
				tryb = 4;
			}
		}
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}




