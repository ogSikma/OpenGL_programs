#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>
#include <time.h>
#include <string>

HDC			hDC = NULL;
HGLRC		hRC = NULL;
HWND		hWnd = NULL;
HINSTANCE	hInstance;
bool	keys[256];
bool	active = TRUE;
bool	fullscreen = false;


bool przyspieszenie = true;

GLfloat x[10] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat y[10] = { 30.0f, 30.0f, 30.0f, 30.0f, 30.0f, 30.0f, 30.0f, 30.0f, 30.0f, 30.0f };
GLfloat wlk_x[10] = { 12.0f, 12.0f, 12.0f, 12.0f, 12.0f, 12.0f, 12.0f, 12.0f, 12.0f, 12.0f };
GLfloat wlk_y[10] = { 6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f };
GLfloat angle[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int i = 0;

GLfloat predkoscLotu = 0.5f;

GLfloat kolor = 0.0f;
GLfloat predkoscKolor = 0.005f;
bool k = true;

GLfloat odlegloscKamera = -85.0f;
GLfloat katKameraX = 0.0f;
GLfloat katKameraY = 0.0f;

GLfloat predkoscObrotu = 1.5f;

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
	gluPerspective(60.0f, (GLfloat)width / (GLfloat)height, 0.1f, 140.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int Init(GLvoid)										
{
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
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

void nadajKolor()
{
	if (k)
	{
		kolor = kolor + predkoscKolor;
		if (kolor > 1.0f)
		{
			kolor = 1.0f;
			k = !k;
		}
	}
	else
	{
		kolor = kolor - predkoscKolor;
		if (kolor < 0.0f)
		{
			kolor = 0.0f;
			k = !k;
		}
	}
}

void rysujObiekty(GLint n)
{
	glPushMatrix();
	glTranslatef(x[n], y[n], 0.0f);
	glColor3f(0.0f, 0.8f, 0.0f);
	glRotatef(angle[n], 0.0f, 0.0f, 1.0f);
	auxSolidBox(wlk_x[n], wlk_y[n], 6.0f);
	glPopMatrix();
}

int Display(GLvoid)									
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, odlegloscKamera);
	glRotatef(katKameraX, 1.0f, 0.0f, 0.0f);
	glRotatef(katKameraY, 0.0f, 1.0f, 0.0f);

	if (i == 10)
	{
		i = 0;
	}

	nadajKolor();

	glPushMatrix();
	glTranslatef(0.0, -43.0, 0.0f);
	glColor3f(0.8f, 0.8f, 0.8f);
	auxSolidBox(40.0, 1.0, 6.0f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, -3.0, -4.0f);
	glColor3f(0.8f, 0.8f, 0.8f);
	auxSolidBox(40.0, 80.0, 1.0f);
	glPopMatrix();

	GLfloat y_max = -40.0f;

	if (wlk_x[i] == 6.0f)
	{
		y_max = -37.0f;
	}
	else
	{
		y_max = -40.0f;
	}

	if (i>0)
	{
		for (int j = (i - 1); j > -1; j--) {
			if (wlk_x[i] == 6.0f && wlk_x[j] == 6.0f)
			{
				if (x[i] == x[j] || (x[i] + 3) == x[j] || (x[i] - 3) == x[j])
				{
					y_max = y_max + 12.0f;
				}
			}
			if (wlk_x[i] == 6.0f && wlk_x[j] == 12.0f)
			{
				if (x[i] == x[j] || (x[i] + 3) == x[j] || (x[i] + 6) == x[j] || (x[i] - 3) == x[j] || (x[i] - 6) == x[j])
				{
					y_max = y_max + 6.0f;
				}
			}
			if (wlk_x[i] == 12.0f && wlk_x[j] == 6.0f)
			{
				if (x[i] == x[j] || (x[i] + 3) == x[j] || (x[i] + 6) == x[j] || (x[i] - 3) == x[j] || (x[i] - 6) == x[j])
				{
					y_max = y_max + 12.0f;
				}
			}
			if (wlk_x[i] == 12.0f && wlk_x[j] == 12.0f)
			{
				if (x[i] == x[j] || (x[i] + 3) == x[j] || (x[i] + 6) == x[j] || (x[i] + 9) == x[j] || (x[i] - 3) == x[j] || (x[i] - 6) == x[j] || (x[i] - 9) == x[j])
				{
					y_max = y_max + 6.0f;
				}
			}
		}
	}

	srand(time(0));
	int los1 = rand() % 10;
	if (y[i] > y_max)
	{
		if (i > 0)
		{
			for (int j = 0; j < i; j++)
			{
				rysujObiekty(j);
			}
		}
		glPushMatrix();
		glTranslatef(x[i], y[i], 0.0f);
		glColor3f(kolor, !kolor, 0.5f);
		glRotatef(angle[i], 0.0f, 0.0f, 1.0f);
		auxSolidBox(wlk_x[i], wlk_y[i], 6.0f);
		y[i] = y[i] - predkoscLotu;
		glPopMatrix();
	}
	else
	{
		i = i + 1;
	}
	return TRUE;
}

void Rules(GLvoid) {
	if (wlk_x[i] == 6.0)
	{
		if (x[i] < -15.0) x[i] = -15;
		if (x[i] > 15.0) x[i] = 15.0;
	}
	else if (wlk_x[i] == 12.0)
	{
		if (x[i] < -12.0) x[i] = -12;
		if (x[i] > 12.0) x[i] = 12.0;
	}
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

	MessageBox(NULL, "Zadanie 9 obracajace się obiekty, jak tetris. \n przycisk 'W' obróć klocek, przyciski 'A' 'D' poruszaj się w boki", "Info", MB_OK | MB_ICONEXCLAMATION);

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
	if (!CreateGLWindow("OpenGL zadanie 9 - niby tetris", 800, 600, 16, fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}
	int ostatniCzas = 0;
	while (!done)									// Loop That Runs While done=FALSE
	{
		bool rendered = false;
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
					int czas = GetTickCount();
					if (czas - ostatniCzas >= 20)
					{
						Display();
						SwapBuffers(hDC);
						ostatniCzas = czas;
						rendered = true;
					}
				}
			}
			if (rendered)
			{
				if (keys[VK_F1])						// Is F1 Being Pressed?
				{
					keys[VK_F1] = FALSE;					// If So Make Key FALSE
					KillGLWindow();						// Kill Our Current Window
					fullscreen = !fullscreen;				// Toggle Fullscreen / Windowed Mode
					// Recreate Our OpenGL Window
					if (!CreateGLWindow("OpenGL zadanie 9 - niby tetris", 800, 600, 16, fullscreen))
					{
						return 0;						// Quit If Window Was Not Created
					}
				}
				if (keys[VK_UP])
				{
					katKameraX = katKameraX + predkoscObrotu;
					if (katKameraX > 40) {
						katKameraX = 40;
					}
				}
				if (keys[VK_DOWN])
				{
					katKameraX = katKameraX - predkoscObrotu;
					if (katKameraX < 0) {
						katKameraX = 0;
					}
				}
				if (keys[VK_RIGHT])	
				{
					katKameraY = katKameraY + predkoscObrotu;
					if (katKameraY > 360) {
						katKameraY = katKameraY - 360;
					}
					if (katKameraY < 0) {
						katKameraY = katKameraY + 360;
					}
				}
				if (keys[VK_LEFT])
				{
					katKameraY = katKameraY - predkoscObrotu;
					if (katKameraY > 360) {
						katKameraY = katKameraY - 360;
					}
					if (katKameraY < 0) {
						katKameraY = katKameraY + 360;
					}
				}
				if (keys['A'])
				{
					x[i] = x[i] - 3.0;
				}
				if (keys['D'])
				{
					x[i] = x[i] + 3.0;
				}
				if (keys['W'])
				{
					GLfloat temp = wlk_x[i];
					wlk_x[i] = wlk_y[i];
					wlk_y[i] = temp;
				}
				Rules();
			}
		}
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}




