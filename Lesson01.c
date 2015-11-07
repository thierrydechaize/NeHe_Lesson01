/*
===========================================================
            OpenGL Lesson 01:  Creating An OpenGL Window
===========================================================

  Authors Name: Jeff Molofee ( NeHe )

  Disclaimer:

  This program may crash your system or run poorly depending on your
  hardware.  The program and code contained in this archive was scanned
  for virii and has passed all test before it was put online.  If you
  use this code in project of your own, send a shout out to the author!

===========================================================
                NeHe Productions 1997-2004
===========================================================
*/
/**         Comments manageable by Doxygen
*
*  Modified smoothly by Thierry DECHAIZE
*
*  Paradigm : obtain one source (only one !) compatible for multiple free C Compilers
*    and provide for all users an development environment on Windows (64 bits compatible),
*    the great Code::Blocks manager (version 12.11), and don't use glaux.lib or glaux.dll.
*
*	 a) Mingw 32 and 64 bits : downloadable http://sourceforge.net/projects/mingw/
*    b) Mingw 32 and 64 bits : included in packages Orwell in replacement of old Dev-Cpp http://sourceforge.net/projects/orwelldevcpp/ (two installations in different place)
*    c) Mingw 32 : included in package Code::Blocks (version 12.11) http://sourceforge.net/projects/codeblocks/files/Binaries/12.11/Windows/
*    d) Borland C : downloadable  http://edn.embarcadero.com/article/20633
*    e) Digital Mars Compiler C : version 8.56 downloadable http://www.digitalmars.com
*    f) OpenWatcom : version 1.9 downloadable http://openwatcom.mirror.fr/
*	 g) Lcc and Lcc64 : downloadable http://www.cs.virginia.edu/~lcc-win32/
*	 h) Cygwin : downloadable http://www.cygwin.com/install.html (setup.exe)
*	 i) and Visual Studio 2005 : express edition for free (my version 2005 is full not express) http://www.microsoft.com/france/visual-studio/essayez/express.aspx
*
*  Important remark : All install in C partition, "no space" and "no special characters" in the name of directories
*            (it's reason to don't install in C:\Program Files (x86) ... by default proposed on windows 64 bits)
*
*  Add resource file end resource header for restitute version + icon OpenGL.ico for fun
*  because versionning is important, also for freeware :-) !
*
*  Date : 2013/05/11
*
* \file            Lesson01.c
* \author          Jeff Molofee ( NeHe ) originely, adapted by Thierry Dechaize
* \version         1.0.1.2
* \date            12 May 2013
* \brief           Ouvre une simple fenêtre Windows et dessine un triangle muticolore en rotation avec OpenGL.
* \details         Ce programme ne gére que deux événements : le clic sur le bouton "Fermé" de la fenêtre ou la sortie par la touche ESC.
*
*
*/

#ifdef __CYGWIN__
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>   /* Bug with LCC64 : two define function 'GetProcessWorkingSetSize' and 'SetProcessWorkingSetSize'in win.h */
#if defined( __LCC__ )
#ifndef WINGDIAPI
#   define WINGDIAPI __stdcall
#endif
#endif
#include <gl/gl.h>


#include "resource.h"

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);


int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;
    float theta = 0.0f;

    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON), IMAGE_ICON, 16, 16, 0);


    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "OpenGL Sample",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          256,
                          256,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            /* OpenGL animation code goes here */

            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glPushMatrix();
            glRotatef(theta, 0.0f, 0.0f, 1.0f);

            glBegin(GL_TRIANGLES);

                glColor3f(1.0f, 0.0f, 0.0f);   glVertex2f(0.0f,   1.0f);
                glColor3f(0.0f, 1.0f, 0.0f);   glVertex2f(0.87f,  -0.5f);
                glColor3f(0.0f, 0.0f, 1.0f);   glVertex2f(-0.87f, -0.5f);

            glEnd();

            glPopMatrix();

            SwapBuffers(hDC);

            theta += 1.0f;
            Sleep (1);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

/**         Comments manageable by Doxygen
*
* \brief      Fonction CALLBACK de traitement des messages Windows
* \details    Traitement de la boucle infinie des messages Windows
* \param      hwnd         L'header de la fenêtre principale.
* \param      uint         Un entier non signé.
* \param      wParam       Les paramétres en entrée.
* \param      lParam       Autres paramétres en entrée.
* \return     Un \e LRESULT donnant le status du traitement du message.
*
*/

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                break;
            }
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

/**         Comments manageable by Doxygen
*
* \brief      Fonction EnableOpenGL permettant la configuration d'OpenGL pour la fenêtre principale.
* \details    Ce qui est important est le descripteur du format du pixel utilisé.
* \param      hwnd         L'header de la fenêtre principale.
* \param      hDC          L'header du Device Context.
* \param      hRC          L'header du Resource Context sous OpenGL.
* \return     Aucun        Void.
*
*/

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

/**         Comments manageable by Doxygen
*
* \brief      Fonction DisableOpenGL permettant la dé-configuration d'OpenGL pour la fenêtre principale.
* \details    On libère les différents contextes : Device Context et Resource Context.
* \param      hwnd         L'header de la fenêtre principale.
* \param      hDC          L'header du Device Context.
* \param      hRC          L'header du Resource Context sous OpenGL.
* \return     Aucun        Void.
*
*/

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}

