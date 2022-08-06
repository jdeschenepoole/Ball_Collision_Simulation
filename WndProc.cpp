#include "global.h"

// Called by the window when soemthing happens (a message is sent)
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch( message )
    {
        case WM_PAINT:
            hdc = BeginPaint( hWnd, &ps );
            EndPaint( hWnd, &ps );
            break;

        case WM_DESTROY:
            PostQuitMessage( 0 );
            break;

		// Handle keyboard input...
		case WM_KEYDOWN: 
            switch (wParam) 
            { 
                case VK_LEFT: 
					// Process the LEFT ARROW key. 
					g_vecObjects[0].PosX += 0.5f;
                    break; 
				case VK_RIGHT:
					g_vecObjects[0].PosX -= 0.5f;
					break;
				// Added this, too
				case VK_UP:
					g_vecObjects[0].PosZ += 0.5f;
					break;
				case VK_DOWN:
					g_vecObjects[0].PosZ -= 0.5f;
					break;
			}

        default:
            return DefWindowProc( hWnd, message, wParam, lParam );
    }

    return 0;
}

