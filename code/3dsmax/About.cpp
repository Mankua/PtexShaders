/*-------------------------------------------------------------------------*\
-----------------------------------------------------------------------------
Ptex Shaders
http://www.mankua.com/Ptex/Ptex.php

Author : Diego A. Castaño
Copyright : (c) 2004-2011 Mankua Software Inc.

Licence : ZLib licence :

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
-----------------------------------------------------------------------------
\*-------------------------------------------------------------------------*/

#include "Max.h"
#include "resource.h"

INT_PTR CALLBACK PtexLicenseDlgProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch (msg) 
	{
		case WM_INITDIALOG:
		{
			CenterWindow(hWnd,GetParent(hWnd));
			break;
		}

		case WM_CLOSE:
		{
			EndDialog(hWnd,1);
			break;
		}

		default:
		{
			return FALSE;
		}
	}
	return TRUE;
}

INT_PTR CALLBACK AboutDlgProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch (msg) 
	{
		case WM_INITDIALOG:
		{
			CenterWindow(hWnd,GetParent(hWnd));
			break;
		}

		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case IDC_ABOUT_MANKUA:
				{
					ShellExecute( NULL, "open", "http://www.mankua.com", NULL, NULL, SW_SHOWNORMAL );
				}
				break;

				case IDC_ABOUT_PTEX:
				{
					ShellExecute( NULL, "open", "http://www.ptex.us", NULL, NULL, SW_SHOWNORMAL );
				}
				break;

				case IDC_ABOUT_PTEX_LICENSE:
				{
					DialogBoxParam(	hInstance, MAKEINTRESOURCE(IDD_PTEX_LICENSE), hWnd, PtexLicenseDlgProc, 0);
				}
				break;
			}

			break;
		}

		case WM_CLOSE:
		{
			EndDialog(hWnd,1);
			break;
		}

		default:
		{
			return FALSE;
		}
	}
	return TRUE;
}