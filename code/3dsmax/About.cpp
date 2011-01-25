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