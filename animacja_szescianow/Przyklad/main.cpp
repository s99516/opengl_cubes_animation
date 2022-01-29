#define WIN32_LEAN_AND_MEAN				// "odchudza" aplikacjê Windows

// Pliki nag³ówkowe
#include <windows.h>					// standardowy plik nag³ówkowy Windows
#include <gl/gl.h>						// standardowy plik nag³ówkowy OpenGL
#include <gl/glu.h>						// plik nag³ówkowy biblioteki GLU
#include <conio.h>
#include <math.h>
#include <sysinfoapi.h> // biblioteka do funkcji GetTickCount()

#pragma comment(lib, "legacy_stdio_definitions.lib")

// Zmienne globalne
HDC g_HDC;								// globalny kontekst urz¹dzenia
bool fullScreen = false;				// true = tryb pe³noekranowy; false = tryb okienkowy

// Parametry oœwietlenia
float ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };	    // œwiat³o otoczenia
float diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };		// œwiat³o rozproszone
float lightPosition[] = { 1.0f, 0.0f, 1.0f, 0.0f };	    // po³o¿enie Ÿród³a œwiat³a

// Parametry materia³u
float ambientMaterial[] = { 0.5f, 0.0f, 0.0f, 1.0f };
float diffuseMaterial[] = { 0.3f, 0.5f, 0.7f, 1.0f };

//poczatkowy tryb
int mode = 0;

//kolory
float red = 1.0f;
float blue = 1.0f;
float green = 1.0f;
float additional_color_canal_to_change = 0.0f;
bool is_additional_color_growing = true;

char ch;
float scene_rotation = 30.0f;
float gap_y_change = 0.0f;
bool is_gap_y_growing = true;

int number_of_cubes = 27;
int number_of_cubes_in_one_axis = cbrt(number_of_cubes);
int middle = number_of_cubes_in_one_axis / 2;

float gap = 1.0f;

// tu som podstawowe parametry dla kostki
float cube_size = 0.33f;
float y_rot = 0.0f;
bool are_cubes_growing = true;


void Initialize()       // Inicjacja OpenGL
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		// czarny kolor t³a

	glShadeModel(GL_SMOOTH);					// cieniowanie g³adkie
	glEnable(GL_DEPTH_TEST);					// w³¹czenie bufora g³êbi
	glEnable(GL_CULL_FACE);						// ukrywanie tylnych stron wielok¹tów
	glFrontFace(GL_CCW);						// porz¹dek wierzcho³ków przeciwny do kierunku ruchu wskazówek zegara
	glEnable(GL_LIGHTING);						// w³¹czenie oœwietlenia

	// Ustawienie w³aœciwoœci materia³u dla pierwszego Ÿród³a œwiat³a LIGHT0
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);

	// Ustawienie Ÿród³a œwiat³a GL_LIGHT0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);		// sk³adowa otoczenia
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);		// sk³adowa rozproszona
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);	// po³o¿enie Ÿród³a œwiat³a

	// W³¹czenie Ÿród³a œwiat³a
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
}

void DrawCube(float xPos, float yPos, float zPos, float xRot, float yRot, float zRot, float cube_size, float col_red, float col_green, float col_blue) //Narysowanie szeœcianu w xPos,yPos,zPos obróconego o k¹ty xRot,yRot,zRot
{
	glPushMatrix();
	glTranslatef((xPos)*gap, (yPos)*gap, (zPos)*gap);
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glRotatef(zRot, 0.0f, 0.0f, 1.0f);
	glColor3f(col_red, col_green, col_blue);
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 1.0f, 0.0f);	// górna œciana (w p³aszczyŸnie XZ)
	glVertex3f(cube_size, cube_size, cube_size);
	glVertex3f(cube_size, cube_size, -cube_size);
	glVertex3f(-cube_size, cube_size, -cube_size);
	glVertex3f(-cube_size, cube_size, cube_size);
	glEnd();
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, 1.0f);	// przednia œciana (w p³aszczyŸnie XY)
	glVertex3f(cube_size, cube_size, cube_size);
	glVertex3f(-cube_size, cube_size, cube_size);
	glVertex3f(-cube_size, -cube_size, cube_size);
	glVertex3f(cube_size, -cube_size, cube_size);
	glEnd();
	glBegin(GL_POLYGON);
	glNormal3f(1.0f, 0.0f, 0.0f);	// prawa œciana (w p³aszczyŸnie YZ)
	glVertex3f(cube_size, cube_size, cube_size);
	glVertex3f(cube_size, -cube_size, cube_size);
	glVertex3f(cube_size, -cube_size, -cube_size);
	glVertex3f(cube_size, cube_size, -cube_size);
	glEnd();
	glBegin(GL_POLYGON);
	glNormal3f(-1.0f, 0.0f, 0.0f);	// lewa œciana (w p³aszczyŸnie YZ)
	glVertex3f(-cube_size, cube_size, cube_size);
	glVertex3f(-cube_size, cube_size, -cube_size);
	glVertex3f(-cube_size, -cube_size, -cube_size);
	glVertex3f(-cube_size, -cube_size, cube_size);
	glEnd();
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, -1.0f, 0.0f);	// dolna œciana (w p³aszczyŸnie XZ)
	glVertex3f(-cube_size, -cube_size, cube_size);
	glVertex3f(-cube_size, -cube_size, -cube_size);
	glVertex3f(cube_size, -cube_size, -cube_size);
	glVertex3f(cube_size, -cube_size, cube_size);
	glEnd();
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, -1.0f);	// tylna œciana (w p³aszczyŸnie XY)
	glVertex3f(cube_size, -cube_size, -cube_size);
	glVertex3f(-cube_size, -cube_size, -cube_size);
	glVertex3f(-cube_size, cube_size, -cube_size);
	glVertex3f(cube_size, cube_size, -cube_size);
	glEnd();
	glPopMatrix();
}

void Wait(DWORD interval)
{
	DWORD startTime = GetTickCount();

	while (GetTickCount() < (startTime + interval))
	{
		//nie rob nic
	}
}

void Render()   // Renderowanie sceny
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Opró¿nienie bufora ekranu i bufora g³êbi
	glLoadIdentity();										// Zresetowanie macierzy modelowania


	glTranslatef(0.0f, 0.0f, -7.0f);		// Wykonanie przekszta³ceñ geometrycznych
	glRotatef(30, 1.0f, 0.0f, 0.0f);
	glRotatef(scene_rotation, 0.f, 1.0f, 0.0f);
	glRotatef(0, 0.0f, 0.0f, 1.0f);


	for (int i = 0; i < number_of_cubes_in_one_axis; i = i + 1)
		for (int j = 0; j < number_of_cubes_in_one_axis; j = j + 1)
			for (int k = 0; k < number_of_cubes_in_one_axis; k = k + 1)
			{
				if (mode == 4)
				{
					if (is_gap_y_growing) gap += 0.0005f;
					else gap -= 0.0005f;
					if (gap >= 2.0f) is_gap_y_growing = false;
					else if (gap <= 1.0f) is_gap_y_growing = true;
				}
				if (j == (number_of_cubes_in_one_axis - 1) && i==0 && i==k) DrawCube(float(i) - middle, float(j) - middle, float(k) - middle, 0.0f, y_rot, 0.0f, cube_size, red, additional_color_canal_to_change, additional_color_canal_to_change);
				else if (j == (number_of_cubes_in_one_axis - 1) && i == middle && i == k) DrawCube(float(i) - middle, float(j) - middle, float(k) - middle, 0.0f, y_rot, 0.0f, cube_size, red, green, additional_color_canal_to_change);
				else if (j == (number_of_cubes_in_one_axis - 1) && i == (number_of_cubes_in_one_axis - 1) && i == k) DrawCube(float(i) - middle, float(j) - middle, float(k) - middle, 0.0f, y_rot, 0.0f, cube_size, additional_color_canal_to_change, green, additional_color_canal_to_change);
				else DrawCube(float(i) - middle, float(j) - middle, float(k) - middle, 0.0f, y_rot, 0.0f, cube_size, 0.0f, 0.0f, 1.0f);
			}

	if (GetAsyncKeyState('1')) mode = 1;
	else if (GetAsyncKeyState('2')) mode = 2;
	else if (GetAsyncKeyState('3')) mode = 3;
	else if (GetAsyncKeyState('4')) mode = 4;
	else if (GetAsyncKeyState('5')) mode = 5;
	if (mode == 1) y_rot += 0.5;
	if (mode == 2)
	{
		if (are_cubes_growing)	cube_size += 0.005f;
		else cube_size -= 0.005f;
		if (cube_size > 0.33f)	are_cubes_growing = false;
		else if (cube_size < 0.16f)	are_cubes_growing = true;
	}
	if (mode == 3) scene_rotation += 0.5f;
	if (mode == 5)
	{
		if (is_additional_color_growing == true) additional_color_canal_to_change += 0.01f;
		else additional_color_canal_to_change -= 0.1f;
		if (additional_color_canal_to_change >= 1.0f) is_additional_color_growing = false;
		else if (additional_color_canal_to_change <= 0.0f) is_additional_color_growing = true;
	}
	Wait(20);

	glFlush();								// Zrzucenie bufora graficznego na ekran

	SwapBuffers(g_HDC);						// Prze³¹czenie buforów
}


void SetupPixelFormat(HDC hDC)  // Funkcja okreœlaj¹ca format pikseli
{
	int nPixelFormat;					// indeks formatu pikseli

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),	// rozmiar struktury
		1,								// domyœlna wersja
		PFD_DRAW_TO_WINDOW |			// grafika w oknie
		PFD_SUPPORT_OPENGL |			// grafika OpenGL 
		PFD_DOUBLEBUFFER,				// podwójne buforowanie
		PFD_TYPE_RGBA,					// tryb kolorów RGBA 
		32,								// 32-bitowy opis kolorów
		0, 0, 0, 0, 0, 0,				// nie specyfikuje bitów kolorów
		0,								// bez bufora alfa
		0,								// nie specyfikuje bitu przesuniêcia
		0,								// bez bufora akumulacji
		0, 0, 0, 0,						// ignoruje bity akumulacji
		16,								// 16-bitowy bufor Z
		0,								// bez bufora powielania
		0,								// bez buforów pomocniczych
		PFD_MAIN_PLANE,					// g³ówna p³aszczyzna rysowania
		0,								// zarezerwowane
		0, 0, 0 };						// ignoruje maski warstw

	nPixelFormat = ChoosePixelFormat(hDC, &pfd);	// wybiera najbardziej zgodny format pikseli 
	SetPixelFormat(hDC, nPixelFormat, &pfd);		// okreœla format pikseli dla danego kontekstu urz¹dzenia
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)  // Procedura okienkowa
{
	static HGLRC hRC;					// kontekst tworzenia grafiki
	static HDC hDC;						// kontekst urz¹dzenia
	int width, height;					// szerokoœæ i wysokoœæ okna

	switch (message)						// obs³uga komunikatów
	{
	case WM_CREATE:					// Utworzenie okna
		hDC = GetDC(hwnd);			// pobiera kontekst urz¹dzenia dla okna
		g_HDC = hDC;
		SetupPixelFormat(hDC);		// wywo³uje funkcjê okreœlaj¹c¹ format pikseli
		// tworzy kontekst grafiki i czyni go bie¿¹cym
		hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);
		return 0;
		break;

	case WM_CLOSE:					// Zamkniêcie okna
		wglMakeCurrent(hDC, NULL);  // usuwa kontekst renderowania okna
		wglDeleteContext(hRC);
		PostQuitMessage(0);			// wysy³a WM_QUIT do kolejki komunikatów
		return 0;
		break;

	case WM_SIZE:					// Zmiana wymiarów okna
		height = HIWORD(lParam);
		width = LOWORD(lParam);
		if (height == 0)				// zabezpieczenie przed dzieleniem przez 0
			height = 1;
		glViewport(0, 0, width, height);		// nadanie nowych wymiarów okna OpenGL
		glMatrixMode(GL_PROJECTION);			// prze³¹czenie macierzy rzutowania
		glLoadIdentity();						// zresetowanie macierzy rzutowania
		gluPerspective(66.0f, ((GLfloat)width / (GLfloat)height), 1.0f, 1000.0f);  // wyznaczenie proporcji obrazu i ustawienie rzutowania perspektywicznego
		//bylo 66.0f
		glMatrixMode(GL_MODELVIEW);				// prze³¹czenie macierzy modelowania
		glLoadIdentity();						// zresetowanie macierzy modelowania
		return 0;
		break;

	default:
		break;
	}

	return (DefWindowProc(hwnd, message, wParam, lParam));
}

// Funkcja g³ówna, od której rozpoczyna siê wykonywanie aplikacji
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	MessageBox(NULL, "Prze³¹czenie sie miedzy trybami - 1, 2, 3, 4 i 5", "Ustawienia animacji", MB_OK);

	WNDCLASSEX windowClass;		// klasa okna
	HWND	   hwnd;			// uchwyt okna
	MSG		   msg;				// komunikat
	bool	   done;			// znacznik zakoñczenia aplikacji
	DWORD	   dwExStyle;		// rozszerzony styl okna
	DWORD	   dwStyle;			// styl okna
	RECT	   windowRect;		// rozmiar okna

	// parametry okna
	int width = 800;
	int height = 600;
	int bits = 32;

	// fullScreen = TRUE/FALSE;		// Prze³¹cznik aplikacja okienkowa/aplikacja pe³noekranowa

	windowRect.left = (long)0;						// struktura okreœlaj¹ca rozmiary okna
	windowRect.right = (long)width;
	windowRect.top = (long)0;
	windowRect.bottom = (long)height;

	// definicja klasy okna
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WndProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	// domyœlna ikona
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);		// domyœlny kursor
	windowClass.hbrBackground = NULL;								// bez t³a
	windowClass.lpszMenuName = NULL;								// bez menu
	windowClass.lpszClassName = "MojaKlasa";
	windowClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);		// logo Windows

	// zarejestrowanie klasy okna
	if (!RegisterClassEx(&windowClass))
		return 0;

	if (fullScreen)								// W³¹czenie trybu pe³noekranowego
	{
		DEVMODE dmScreenSettings;						// tryb urz¹dzenia
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = width;			// szerokoœæ ekranu
		dmScreenSettings.dmPelsHeight = height;			// wysokoœæ ekranu
		dmScreenSettings.dmBitsPerPel = bits;			// iloœæ bitów na piksel
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// je¿eli prze³¹czenie trybu na pe³noekranowy nie powiod³o siê, prze³¹czenie z powrotem na tryb okienkowy
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			MessageBox(NULL, "Prze³¹czenie trybu wyswietlania nie powiod³o siê", NULL, MB_OK);
			fullScreen = FALSE;
		}
	}

	if (fullScreen)								// Tryb pe³noekranowy?
	{
		dwExStyle = WS_EX_APPWINDOW;				// rozszerzony styl okna
		dwStyle = WS_POPUP;						// styl okna
		ShowCursor(FALSE);						// ukrycie kursora myszy
	}
	else										// Tryb okienkowy?
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	// definicja klasy okna
		dwStyle = WS_OVERLAPPEDWINDOW;					// styl okna
	}

	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);		// skorygowanie rozmiaru okna
	// utworzenie okna
	hwnd = CreateWindowEx(NULL,									// styl rozszerzony
		"MojaKlasa",							// nazwa klasy
		"Animacja szeœcianów",	// nazwa aplikacji
		dwStyle | WS_CLIPCHILDREN |
		WS_CLIPSIBLINGS,
		0, 0,								// wspó³rzêdne x,y
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,	// szerokoœæ, wysokoœæ
		NULL,									// uchwyt okna nadrzêdnego
		NULL,									// uchwyt menu
		hInstance,							// instancja aplikacji
		NULL);

	if (!hwnd)	// sprawdzenie, czy utworzenie okna nie powiod³o siê (wtedy wartoœæ hwnd równa NULL)
		return 0;

	ShowWindow(hwnd, SW_SHOW);			// wyœwietlenie okna
	UpdateWindow(hwnd);					// aktualizacja okna

	done = false;						// inicjacja zmiennej warunku pêtli przetwarzania komunikatów
	Initialize();						// inicjacja OpenGL

	while (!done)	// pêtla przetwarzania komunikatów
	{
		PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
		if (msg.message == WM_QUIT)		// aplikacja otrzyma³a komunikat WM_QUIT?
			done = true;				// jeœli tak, to koñczy dzia³anie
		else
		{
			Render();					// jeœli nie to renderuje scenê,
			TranslateMessage(&msg);		// t³umaczy komunikat i wysy³a do systemu
			DispatchMessage(&msg);
		}
	}

	if (fullScreen)						// je¿eli by³ tryb pe³noekranowy
	{
		ChangeDisplaySettings(NULL, 0);	// to przywrócenie pulpitu
		ShowCursor(TRUE);				// i wskaŸnika myszy
	}

	return msg.wParam;
}
