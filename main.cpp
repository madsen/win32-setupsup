#include <windows.h>
#include <winuser.h>
#include <winnls.h>
#include <stdio.h>

#include "list.h"

// max size for class names
#define CLASS_BUF_SIZE	128
// max size for for window caption
#define TEXT_BUF_SIZE		1024

// identifier strings
#define HWND_STR					"#HWND:"
#define CLASS_STR					"#CLASS:"
#define TEXT_STR					"#TEXT:"
#define ID_STR						"#ID:"

#define ISVALID_STR				"#ISVALID:"
#define ISVISIBLE_STR			"#ISVISIBLE:"
#define ISENABLED_STR			"#ISENABLED:"
#define ISCHECKED_STR			"#ISCHECKED:"
#define ISICONIC_STR			"#ISICONIC:"
#define ISZOOMED_STR			"#ISZOOMED:"
#define ISFOREGROUND_STR	"#ISFOREGROUND:"
#define HASFOCUS_STR			"#HASFOCUS:"

#define PARENT_STR				"#PARENT:"
#define PREV_STR					"#PREV:"
#define NEXT_STR					"#NEXT:"
#define RECT_SIZE_STR			"#RECT:"
#define CLIENT_SIZE_STR		"#CLIENT:"

#define PROCEDURE_STR			"#PROCEDURE:"
#define INSTANCE_STR			"#INSTANCE:"
#define STYLE_STR					"#STYLE:"
#define EXTSTYLE_STR			"#EXTSTYLE:"
#define MENU_STR					"#MENU:"
#define ICON_STR					"#ICON:"

#define PROCESS_STR				"#PROCESS:"
#define THREAD_STR				"#THREAD:"

#define HEXINT_STR				"00000000"
#define BIT_STR						"0"
#define SEPARATOR_STR			"\n"


typedef struct _WindowProperties
{
	BOOL storeWindowStr;
	PSTR windowStr;
	BOOL storeClassStr;
	PSTR classStr;
	BOOL storeTextStr;
	PSTR textStr;
	BOOL storeIdStr;
	PSTR idStr;

	BOOL storeIsValidStr;
	PSTR isValidStr;
	BOOL storeIsVisibleStr;
	PSTR isVisibleStr;
	BOOL storeIsEnabledStr;
	PSTR isEnabledStr;
	BOOL storeIsCheckedStr;
	PSTR isCheckedStr;
	BOOL storeIsIconicStr;
	PSTR isIconicStr;
	BOOL storeIsZoomedStr;
	PSTR isZoomedStr;
	BOOL storeIsForegroundStr;
	PSTR isForegroundStr;
	BOOL storeHasFocusStr;
	PSTR hasFocusStr;
	
	BOOL storeParentStr;
	PSTR parentStr;
	BOOL storePrevStr;
	PSTR prevStr;
	BOOL storeNextStr;
	PSTR nextStr;
	BOOL storeRectStr;
	PSTR rectStr;
	BOOL storeClientStr;
	PSTR clientStr;
	
	BOOL storeProcedureStr;
	PSTR procedureStr;
	BOOL storeInstanceStr;
	PSTR instanceStr;
	BOOL storeStyleStr;
	PSTR styleStr;
	BOOL storeExtStyleStr;
	PSTR extStyleStr;
	
	BOOL storeMenuStr;
	PSTR menuStr;
	BOOL storeIconStr;
	PSTR iconStr;

	BOOL storeProcessStr;
	PSTR processStr;
	BOOL storeThreadStr;
	PSTR threadStr;
} WindowProp, *PWindowProp;



// base for user defined error codes
#define USER_ERROR_BASE	0x20000000

// string has a bad format
#define BAD_STR_FORMAT	USER_ERROR_BASE + 0x1
#define NOT_ENOUGTH_MEM	BAD_STR_FORMAT + 0x1

// key codes
#define VK_ALT_STR			"ALT"
#define VK_CTRL_STR			"CTRL"
#define VK_SHIFT_STR		"SHIFT"

#define VK_TAB_STR			"TAB"
#define VK_ENTER_STR		"RET"
#define VK_ESC_STR			"ESC"

#define VK_BACK_STR			"BACK"
#define VK_DEL_STR			"DEL"
#define VK_INSERT_STR		"INS"
#define VK_HELP_STR			"HELP"

#define VK_LEFT_STR			"LEFT"
#define VK_RIGHT_STR		"RIGHT"
#define VK_UP_STR				"UP"
#define VK_DOWN_STR			"DN"
#define VK_PGUP_STR			"PGUP"
#define VK_PGDOWN_STR		"PGDN"
#define VK_BEG_STR			"BEG"
#define VK_END_STR			"END"

#define VK_F1_STR				"F1"
#define VK_F2_STR				"F2"
#define VK_F3_STR				"F3"
#define VK_F4_STR				"F4"
#define VK_F5_STR				"F5"
#define VK_F6_STR				"F6"
#define VK_F7_STR				"F7"
#define VK_F8_STR				"F8"
#define VK_F9_STR				"F9"
#define VK_F10_STR			"F10"
#define VK_F11_STR			"F11"
#define VK_F12_STR			"F12"

#define VK_NUMPAD0_STR	"NUM0"
#define VK_NUMPAD1_STR	"NUM1"
#define VK_NUMPAD2_STR	"NUM2"
#define VK_NUMPAD3_STR	"NUM3"
#define VK_NUMPAD4_STR	"NUM4"
#define VK_NUMPAD5_STR	"NUM5"
#define VK_NUMPAD6_STR	"NUM6"
#define VK_NUMPAD7_STR	"NUM7"
#define VK_NUMPAD8_STR	"NUM8"
#define VK_NUMPAD9_STR	"NUM9"

#define VK_MULTIPLY_STR	"NUM*"
#define VK_ADD_STR 			"NUM+"
#define VK_SUBTRACT_STR	"NUM-"
#define VK_DECIMAL_STR	"NUM,"
#define VK_DIVIDE_STR		"NUM/"

#define LeaveTrue() { result = TRUE; __leave; }
#define LeaveFalse() { result = FALSE; __leave; }
#define LeaveTrueError(error) { result = TRUE; SetLastError(error); __leave; }
#define LeaveFalseError(error) { result = FALSE; SetLastError(error); __leave; }


/*
#define keybd_event(a, b, c, d) \
	{	SetForegroundWindow(hWnd); ::keybd_event(a, b, c, d); }
*/


///////////////////////////////////////////////////////////////////////////////
//
// converts a hexadecimal string to hex value
//
// param  - str - hex string which contains the value
//        - hex - dword pointer to store the result
//
// return - success - true
//          failure - false
//
///////////////////////////////////////////////////////////////////////////////

BOOL ScanHex(PSTR str, DWORD *hex)
{
	// str must be valid, not empty and not longer than 8 chars
	// hex must not be valid
	if(!str || !*str || lstrlen(str) > 8)
		return FALSE;

	int count = lstrlen(str) - 1;
	
	// walk through string
	for(DWORD base = 1, hexVal = 0; count >= 0; count--, base <<= 4)
	{
		// check for invalid chars
		if(!isxdigit(str[count]))
			return FALSE;

		hexVal +=	
			(str[count] - (isdigit(str[count]) ? '0' : 
				islower(str[count]) ? 'a' - 10 : 'A' - 10)) * base;
	}

	if(hex)
		*hex = hexVal;

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
//
// converts a bit string to bit value
//
// param  - str - bit string which contains the value
//        - bit - dword pointer to store the result
//        - maxBits - max. number of bits to convert; must be between 1 and 32
//
// return - success - true
//          failure - false
//
///////////////////////////////////////////////////////////////////////////////

BOOL ScanBit(PSTR str, DWORD *bit, int maxBits = 1)
{
	// str must be valid, not empty and not longer than maxBits chars
	// bit must not be valid
	if(maxBits < 1 || maxBits > 32 || !str || !*str || lstrlen(str) > maxBits)
		return FALSE;

	int count = lstrlen(str) - 1;
	
	// walk through string
	for(DWORD base = 1, bitVal = 0; count >= 0; count--, base <<= 1)
	{
		// check for invalid chars
		if(str[count] != '0' && str[count] != '1')
			return FALSE;

		bitVal +=	(str[count] - '0') * base; 
	}

	if(bit)
		*bit = bitVal;

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
//
// calculate the needed size to copy the contents of a list to a string
//
// param  - list - list to calculate the size for
//
// return - needed size
//
///////////////////////////////////////////////////////////////////////////////

DWORD CalcListStrSizeNeeded(PList list)
{
	DWORD size = 0;

	// walk through the list ...
	for(PNode node = list ? list->HeadPos() : NULL; node; 
			node = list->NextPos(node))
	{
		char *data = (char*) list->This(node);
		
		// ... and add the size
		if(data)
			size += strlen(data);
	}

	return size;
}


///////////////////////////////////////////////////////////////////////////////
//
// initializes the char pointers in a WindowProp structure to null
//
// param  - prop - pointer to a properties structure
//
// return - success - TRUE
//          failure - FALSE
//
///////////////////////////////////////////////////////////////////////////////

BOOL ZeroWindowPropPointers(PWindowProp prop)
{
	// prop must be valid
	if(!prop)
		return FALSE;

	// init with null
	for(int count = 0; count < sizeof(*prop) / (sizeof(BOOL) + sizeof(PSTR)); count++)
		*((PSTR)prop + count * (sizeof(BOOL) + sizeof(PSTR)) + sizeof(BOOL)) = NULL;

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
//
// calculates the size needed to copy the strings in prop to a string
//
// param  - prop - pointer to a properties structure
//          size - pointer to get the size
//
// return - success - TRUE
//          failure - FALSE
//
///////////////////////////////////////////////////////////////////////////////

BOOL CalcSizeNeededToCopyWindowPropPointers(PWindowProp prop, int *size)
{
	// prop and size must be valid
	if(!prop && size)
		return FALSE;

	*size = 0;

	// calc the size
	for(int count = 0; count < sizeof(*prop) / (sizeof(BOOL) + sizeof(PSTR)); count++)
		if(*(PSTR*)((PSTR)prop + count * (sizeof(BOOL) + sizeof(PSTR)) + sizeof(BOOL)))
			*size += 
				lstrlen(*(PSTR*)((PSTR)prop + count * (sizeof(BOOL) + sizeof(PSTR)) + sizeof(BOOL)));

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
//
// copies the strings in prop to a string
//
// param  - prop - pointer to a properties structure
//          str  - pointer to get the string; str must be big enougth
//
// return - success - TRUE
//          failure - FALSE
//
///////////////////////////////////////////////////////////////////////////////

BOOL CopyWindowPropPointersToString(PWindowProp prop, PSTR str)
{
	// prop and str must be valid
	if(!prop && str)
		return FALSE;

	*str = 0;

	// copy the string
	for(int count = 0; count < sizeof(*prop) / (sizeof(BOOL) + sizeof(PSTR)); count++)
		if(*(PSTR*)((PSTR)prop + count * (sizeof(BOOL) + sizeof(PSTR)) + sizeof(BOOL)))
			lstrcat(str, *(PSTR*)((PSTR)prop + count * (sizeof(BOOL) + sizeof(PSTR)) + sizeof(BOOL)));

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
//
// frees allocated memory in a WindowProp structure
//
// param  - prop - pointer to a properties structure
//
// return - success - TRUE
//          failure - FALSE
//
///////////////////////////////////////////////////////////////////////////////

BOOL FreeWindowPropMem(PWindowProp prop)
{
	// prop must be valid
	if(!prop)
		return FALSE;

	// delete allocated memory
	for(int count = 0; count < sizeof(*prop) / (sizeof(BOOL) + sizeof(PSTR)); count++)
		if(*(PSTR*)((PSTR)prop + count * (sizeof(BOOL) + sizeof(PSTR)) + sizeof(BOOL)))
			delete *(PSTR*)((PSTR)prop + count * (sizeof(BOOL) + sizeof(PSTR)) + sizeof(BOOL));

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
//
// retrieves window properties and stores them in the properties structure
//
// param  - hWnd			 - handle to the app. window
//          properties - pointer to a properties structure to store the
//                       information; each property to get must have an
//                       app. storeXXXXStr value set to true
//
// return - success - TRUE
//          failure - FALSE
//
///////////////////////////////////////////////////////////////////////////////

BOOL GetWindowProperties(HWND hWnd, PWindowProp prop)
{
	// prop must be valid
	if(!prop)
		return FALSE;

	BOOL result = TRUE;

	// let's try it
	__try
	{
		// initialize all string pointer in the structure with null
		ZeroWindowPropPointers(prop);

		// store the window handle in text form
		if(prop->storeWindowStr)
		{
			if(!(prop->windowStr = new char[sizeof(HWND_STR) + sizeof(HEXINT_STR)]))
				LeaveFalse();

			lstrcpy(prop->windowStr, HWND_STR);
			wsprintf(prop->windowStr + lstrlen(prop->windowStr), "%08x", hWnd);
		}

		// gets the class name for the window
		if(prop->storeClassStr)
		{
			char ClassBuf[CLASS_BUF_SIZE];

			ZeroMemory(ClassBuf, CLASS_BUF_SIZE);
			GetClassName(hWnd, ClassBuf, CLASS_BUF_SIZE);

			if(!(prop->classStr = new char[sizeof(CLASS_STR) + lstrlen(ClassBuf)]))
				LeaveFalse();

			lstrcpy(prop->classStr, CLASS_STR);
			lstrcat(prop->classStr, ClassBuf);
		}

		// gets the caption for the window
		if(prop->storeTextStr)
		{
			char TextBuf[TEXT_BUF_SIZE];
	
			ZeroMemory(TextBuf, TEXT_BUF_SIZE);
			GetWindowText(hWnd, TextBuf, TEXT_BUF_SIZE);

			if(!(prop->textStr = new char[sizeof(TEXT_STR) + lstrlen(TextBuf)]))
				LeaveFalse();

			lstrcpy(prop->textStr, TEXT_STR);
			lstrcat(prop->textStr, TextBuf);
		}

		
		// gets the id for the window
		if(prop->storeIdStr)
		{
			if(!(prop->idStr = new char[sizeof(ID_STR) + sizeof(HEXINT_STR)]))
				LeaveFalse();

			lstrcpy(prop->idStr, ID_STR);
			wsprintf(prop->idStr + lstrlen(prop->idStr), "%08x", GetDlgCtrlID(hWnd));
		}

		// gets if the the window is valid
		if(prop->storeIsValidStr)
		{
			if(!(prop->isValidStr = new char[sizeof(ISVALID_STR) + sizeof(BIT_STR)]))
				LeaveFalse();

			lstrcpy(prop->isValidStr, ISVALID_STR);
			lstrcat(prop->isValidStr, IsWindow(hWnd) ? "1" : "0");
		}

		// gets if the the window is visible
		if(prop->storeIsVisibleStr)
		{
			if(!(prop->isVisibleStr = new char[sizeof(ISVISIBLE_STR) + sizeof(BIT_STR)]))
				LeaveFalse();

			lstrcpy(prop->isVisibleStr, ISVISIBLE_STR);
			lstrcat(prop->isVisibleStr, IsWindowVisible(hWnd) ? "1" : "0");
		}

		// gets if the the window is enabled
		if(prop->storeIsEnabledStr)
		{
			if(!(prop->isEnabledStr = new char[sizeof(ISENABLED_STR) + sizeof(BIT_STR)]))
				LeaveFalse();

			lstrcpy(prop->isEnabledStr, ISENABLED_STR);
			lstrcat(prop->isEnabledStr, IsWindowEnabled(hWnd) ? "1" : "0");
		}

		// gets if the the window is checked (only valid for buttons or check boxes)
		if(prop->storeIsCheckedStr)
		{
			if(!(prop->isCheckedStr = new char[sizeof(ISCHECKED_STR) + sizeof(BIT_STR)]))
				LeaveFalse();

			lstrcpy(prop->isCheckedStr, ISCHECKED_STR);
			wsprintf(prop->isCheckedStr + lstrlen(prop->isCheckedStr), "%1x", 
							 SendMessage(hWnd, BM_GETCHECK, 0, 0));
		}

		// gets if the the window is iconic
		if(prop->storeIsIconicStr)
		{
			if(!(prop->isIconicStr = new char[sizeof(ISICONIC_STR) + sizeof(BIT_STR)]))
				LeaveFalse();

			lstrcpy(prop->isIconicStr, ISICONIC_STR);
			lstrcat(prop->isIconicStr, IsIconic(hWnd) ? "1" : "0");
		}

		// gets if the the window is zoomed
		if(prop->storeIsZoomedStr)
		{
			if(!(prop->isZoomedStr = new char[sizeof(ISZOOMED_STR) + sizeof(BIT_STR)]))
				LeaveFalse();

			lstrcpy(prop->isZoomedStr, ISZOOMED_STR);
			lstrcat(prop->isZoomedStr, IsZoomed(hWnd) ? "1" : "0");
		}

		// gets if the window or the parent is the foreground window
		if(prop->storeIsForegroundStr)
		{
			if(!(prop->isForegroundStr = new char[sizeof(ISFOREGROUND_STR) + sizeof(BIT_STR)]))
				LeaveFalse();

			HWND hForegroundWnd = GetForegroundWindow();

			lstrcpy(prop->isForegroundStr, ISFOREGROUND_STR);
			lstrcat(prop->isForegroundStr, hForegroundWnd == hWnd || GetParent(hWnd) == hWnd ? "1" : "0");
		}

		// gets if the window has the focus
		if(prop->storeHasFocusStr)
		{
			if(!(prop->hasFocusStr = new char[sizeof(HASFOCUS_STR) + sizeof(BIT_STR)]))
				LeaveFalse();

			// must attach to the thread of the window
			if(!AttachThreadInput(GetCurrentThreadId(), GetWindowThreadProcessId(hWnd, NULL), TRUE))
				LeaveFalse();

			lstrcpy(prop->hasFocusStr, HASFOCUS_STR);
			lstrcat(prop->hasFocusStr, GetFocus() == hWnd ? "1" : "0");
			
			if(!AttachThreadInput(GetCurrentThreadId(), GetWindowThreadProcessId(hWnd, NULL), FALSE))
				LeaveFalse();
		}

		// gets the parent of the window
		if(prop->storeParentStr)
		{
			if(!(prop->parentStr = new char[sizeof(PARENT_STR) + sizeof(HEXINT_STR)]))
				LeaveFalse();

			lstrcpy(prop->parentStr, PARENT_STR);
			wsprintf(prop->parentStr + lstrlen(prop->parentStr), "%08x", GetParent(hWnd));
		}

		// gets the previous window
		if(prop->storePrevStr)
		{
			if(!(prop->prevStr = new char[sizeof(PREV_STR) + sizeof(HEXINT_STR)]))
				LeaveFalse();

			lstrcpy(prop->prevStr, PREV_STR);
			wsprintf(prop->prevStr + lstrlen(prop->prevStr), "%08x", GetNextWindow(hWnd, GW_HWNDPREV));
		}

		// gets the next window
		if(prop->storeNextStr)
		{
			if(!(prop->nextStr = new char[sizeof(NEXT_STR) + sizeof(HEXINT_STR)]))
				LeaveFalse();

			lstrcpy(prop->nextStr, NEXT_STR);
			wsprintf(prop->nextStr + lstrlen(prop->nextStr), "%08x", GetNextWindow(hWnd, GW_HWNDNEXT));
		}

		// gets the size of the window
		if(prop->storeRectStr)
		{
			if(!(prop->rectStr = new char[sizeof(RECT_SIZE_STR) + 4 * sizeof(HEXINT_STR)]))
				LeaveFalse();

			RECT rect = {0, 0, 0, 0};

			GetWindowRect(hWnd, &rect);
			lstrcpy(prop->rectStr, RECT_SIZE_STR);
			wsprintf(prop->rectStr + lstrlen(prop->rectStr), "%08x,%08x,%08x,%08x", 
							 rect.left, rect.top, rect.right, rect.bottom);
		}

		// gets the size of the window
		if(prop->storeClientStr)
		{
			if(!(prop->clientStr = new char[sizeof(CLIENT_SIZE_STR) + 4 * sizeof(HEXINT_STR)]))
				LeaveFalse();

			RECT rect = {0, 0, 0, 0};

			GetClientRect(hWnd, &rect);
			lstrcpy(prop->clientStr, CLIENT_SIZE_STR);
			wsprintf(prop->clientStr + lstrlen(prop->clientStr), "%08x,%08x,%08x,%08x", 
							 rect.left, rect.top, rect.right, rect.bottom);
		}

		// gets window procedure address of the window
		if(prop->storeProcedureStr)
		{
			if(!(prop->procedureStr = new char[sizeof(PROCEDURE_STR) + sizeof(HEXINT_STR)]))
				LeaveFalse();

			lstrcpy(prop->procedureStr, PROCEDURE_STR);
			wsprintf(prop->procedureStr + lstrlen(prop->procedureStr), "%08x", 
							 GetWindowLong(hWnd, GWL_WNDPROC));
		}

		// gets the instance of the appl which the window belongs
		if(prop->storeInstanceStr)
		{
			if(!(prop->instanceStr = new char[sizeof(INSTANCE_STR) + sizeof(HEXINT_STR)]))
				LeaveFalse();

			lstrcpy(prop->instanceStr, INSTANCE_STR);
			wsprintf(prop->instanceStr + lstrlen(prop->instanceStr), "%08x", 
							 GetWindowLong(hWnd, GWL_HINSTANCE));
		}

		// gets the style of the window
		if(prop->storeStyleStr)
		{
			if(!(prop->styleStr = new char[sizeof(STYLE_STR) + sizeof(HEXINT_STR)]))
				LeaveFalse();

			lstrcpy(prop->styleStr, STYLE_STR);
			wsprintf(prop->styleStr + lstrlen(prop->styleStr), "%08x", 
							 GetWindowLong(hWnd, GWL_STYLE));
		}

		// gets the extended style of the window
		if(prop->storeExtStyleStr)
		{
			if(!(prop->extStyleStr = new char[sizeof(EXTSTYLE_STR) + sizeof(HEXINT_STR)]))
				LeaveFalse();

			lstrcpy(prop->extStyleStr, EXTSTYLE_STR);
			wsprintf(prop->extStyleStr + lstrlen(prop->extStyleStr), "%08x", 
							 GetWindowLong(hWnd, GWL_EXSTYLE));
		}

		// gets menue handle of the window
		if(prop->storeMenuStr)
		{
			if(!(prop->menuStr = new char[sizeof(MENU_STR) + sizeof(HEXINT_STR)]))
				LeaveFalse();

			lstrcpy(prop->menuStr, MENU_STR);
			wsprintf(prop->menuStr + lstrlen(prop->menuStr), "%08x", GetMenu(hWnd));
		}

		// gets icon handle of the window
		if(prop->storeIconStr)
		{
			if(!(prop->iconStr = new char[sizeof(ICON_STR) + sizeof(HEXINT_STR)]))
				LeaveFalse();

			lstrcpy(prop->iconStr, ICON_STR);
			wsprintf(prop->iconStr + lstrlen(prop->iconStr), "%08x", GetClassLong(hWnd, GCL_HICON));
		}

		// gets the process id of the window
		if(prop->storeProcessStr)
		{
			if(!(prop->processStr = new char[sizeof(PROCESS_STR) + sizeof(HEXINT_STR)]))
				LeaveFalse();

			DWORD processId = 0;
			
			GetWindowThreadProcessId(hWnd, &processId);

			lstrcpy(prop->processStr, PROCESS_STR);
			wsprintf(prop->processStr + lstrlen(prop->processStr), "%08x", processId);
		}

		// gets the thread id of the window
		if(prop->storeThreadStr)
		{
			if(!(prop->threadStr = new char[sizeof(THREAD_STR) + sizeof(HEXINT_STR)]))
				LeaveFalse();

			lstrcpy(prop->threadStr, THREAD_STR);
			wsprintf(prop->threadStr + lstrlen(prop->threadStr), "%08x", 
							 GetWindowThreadProcessId(hWnd, NULL));
		}
	}
	__finally
	{
		// if we got an failure, clear all allocated memory 
		if(!result)
		{
			FreeWindowPropMem(prop);
			SetLastError(NOT_ENOUGTH_MEM);
		}
	}

	return result;
}


///////////////////////////////////////////////////////////////////////////////
//
// call back function called to enum each window 
//
// param  - hWnd   - handle to the app. window
//          lParam - pointer to the list
//
// return - success - TRUE
//          failure - FALSE
////////////////////////////////////////////////////////////////////////////////

BOOL CALLBACK PlEnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	PList list = (PList)lParam;

	WindowProp prop;

	// we need only the window handle
	ZeroMemory(&prop, sizeof(prop));
	prop.storeWindowStr = TRUE;

	// get the window handle
	if(!GetWindowProperties(hWnd, &prop))
		return FALSE;

	BOOL result = TRUE;

	__try
	{
		// calculate the size needed
		int sizeNeeded = sizeof(SEPARATOR_STR) + (prop.windowStr ? lstrlen(prop.windowStr) : 0);
		
		// alloc memory to store
		PSTR str = new char[sizeNeeded];
		
		if(!str)
			LeaveFalse();

		ZeroMemory(str, sizeNeeded);

		// copy strings
		if(prop.windowStr)
			lstrcpy(str, prop.windowStr);

		lstrcat(str, SEPARATOR_STR);

		// add string to the list
		if(!list->AddTail(str, TRUE))
			LeaveFalse();
	}
	__finally
	{
		// free memory
		FreeWindowPropMem(&prop);
		if(!result)
			SetLastError(NOT_ENOUGTH_MEM);
	}

	return result;
}


///////////////////////////////////////////////////////////////////////////////
//
// enum window function 
//
// param  - windows - pointer to a string pointer (must be deleted)
//
// return - success - TRUE
//          failure - FALSE
//
///////////////////////////////////////////////////////////////////////////////

BOOL WINAPI PlEnumWindows(PSTR *windows)
{
	List list;
	BOOL result = TRUE;

	if(result = EnumWindows((WNDENUMPROC)PlEnumWindowsProc, (LPARAM)&list))
	{
		DWORD sizeNeeded = CalcListStrSizeNeeded(&list);
		
		if(sizeNeeded && (*windows = new char[++sizeNeeded]))
		{
			ZeroMemory(*windows, sizeNeeded);

			for(PNode node = list.HeadPos(); node; node = list.NextPos(node))
			{
				char *data = (char*) list.This(node);		
				
				lstrcat(*windows, data);
			}
		}
		else
		{
			result = FALSE;
			if(sizeNeeded)
				SetLastError(NOT_ENOUGTH_MEM);
			*windows = NULL;

		}
	}
	
	list.RemoveAll();

	return result;
}


///////////////////////////////////////////////////////////////////////////////
//
// call back function called to enum each child window 
//
// param  - hWnd	 - handle to the app. window
//          lParam - pointer to the list
//
// return - success - TRUE
//          failure - FALSE
//
///////////////////////////////////////////////////////////////////////////////

BOOL CALLBACK PlEnumChildWindowsProc(HWND hWnd, LPARAM lParam)
{
	// pointer to list
	PList list = (PList)lParam;
	WindowProp prop;

	// we need only the window handle
	ZeroMemory(&prop, sizeof(prop));
	prop.storeWindowStr = TRUE;

	// get the window handle
	if(!GetWindowProperties(hWnd, &prop))
		return FALSE;

	BOOL result = TRUE;

	__try
	{
		// calculate the size needed
		int sizeNeeded = sizeof(SEPARATOR_STR) + (prop.windowStr ? lstrlen(prop.windowStr) : 0);
		
		// alloc memory to store
		PSTR str = new char[sizeNeeded];
		
		if(!str)
			LeaveFalse();

		ZeroMemory(str, sizeNeeded);

		// copy strings
		if(prop.windowStr)
			lstrcpy(str, prop.windowStr);

		lstrcat(str, SEPARATOR_STR);

		// add string to the list
		if(!list->AddTail(str, TRUE))
			LeaveFalse();
	}
	__finally
	{
		// free memory
		FreeWindowPropMem(&prop);
		if(!result)
			SetLastError(NOT_ENOUGTH_MEM);
	}

	return result;
}


///////////////////////////////////////////////////////////////////////////////
//
// enum child windows function 
//
// param  - parentWnd - window handle for the parent window (as hex string)
//          windows   - pointer to a string pointer (must be deleted)
//
// return - success - TRUE
//          failure - FALSE
//
///////////////////////////////////////////////////////////////////////////////

BOOL WINAPI PlEnumChildWindows(PSTR parentWnd, PSTR *windows)
{
	List list;
	BOOL result = TRUE;

	HWND hParentWnd = NULL;

	if(!ScanHex(parentWnd, (DWORD*)&hParentWnd))
		return FALSE;

	if(result = EnumChildWindows(hParentWnd, (WNDENUMPROC)PlEnumChildWindowsProc, 
															 (LPARAM)&list))
	{
		DWORD sizeNeeded = CalcListStrSizeNeeded(&list);
		
		if(sizeNeeded && (*windows = new char[++sizeNeeded]))
		{
			ZeroMemory(*windows, sizeNeeded);

			for(PNode node = list.HeadPos(); node; node = list.NextPos(node))
			{
				char *data = (char*) list.This(node);		
				
				if(data)
					lstrcat(*windows, data);
			}
		}
		else
		{
			result = FALSE;
			if(sizeNeeded)
				SetLastError(NOT_ENOUGTH_MEM);
			*windows = NULL;
		}
	}
	
	list.RemoveAll();

	return result;
}


///////////////////////////////////////////////////////////////////////////////
//
// retrieves window properties
//
// param  - window - handle of the window to get (in textual form)
//          outStr - pointer to output string (must be deallocated)
//
// return - success - true
//          failure - false
//
///////////////////////////////////////////////////////////////////////////////

BOOL WINAPI PlGetWindowProperties(PSTR window, PSTR *outStr)
{
	HWND hWnd = NULL;

	// strings must be valid
	if(!ScanHex(window, (DWORD*)&hWnd) || !outStr)
		return FALSE;

	WindowProp prop;

	ZeroMemory(&prop, sizeof(prop));

	prop.storeWindowStr = TRUE;
	prop.storeClassStr = TRUE;
	prop.storeTextStr = TRUE;
	prop.storeIdStr = TRUE;
	prop.storeIsValidStr = TRUE;
	prop.storeIsVisibleStr = TRUE;
	prop.storeIsEnabledStr = TRUE;
	prop.storeIsCheckedStr = TRUE;
	prop.storeIsIconicStr = TRUE;
	prop.storeIsZoomedStr = TRUE;
	prop.storeIsForegroundStr = TRUE;
	prop.storeHasFocusStr = TRUE;
	prop.storeParentStr = TRUE;
	prop.storePrevStr = TRUE;
	prop.storeNextStr = TRUE;
	prop.storeRectStr = TRUE;
	prop.storeClientStr = TRUE;
	prop.storeProcedureStr = TRUE;
	prop.storeInstanceStr = TRUE;
	prop.storeStyleStr = TRUE;
	prop.storeExtStyleStr = TRUE;
	prop.storeMenuStr = TRUE;
	prop.storeIconStr = TRUE;
	prop.storeProcessStr = TRUE;
	prop.storeThreadStr = TRUE;

	// get the window handle
	if(!GetWindowProperties(hWnd, &prop))
		return FALSE;

	BOOL result = TRUE;

	*outStr = NULL;

	__try
	{
		// calculate the size needed
		int sizeNeeded = 0;
		
		// get the size needed
		if(!CalcSizeNeededToCopyWindowPropPointers(&prop, &sizeNeeded))
			LeaveFalse();
		
		// alloc memory
		if(!(*outStr = new char[sizeNeeded += sizeof(SEPARATOR_STR)]))
			LeaveFalseError(NOT_ENOUGTH_MEM);

		// copy strings
		if(!CopyWindowPropPointersToString(&prop, *outStr))
			LeaveFalse();

		// and the terminator
		lstrcat(*outStr, SEPARATOR_STR);

	}
	__finally
	{
		// free memory
		FreeWindowPropMem(&prop);

		if(!result && *outStr)
		{
			delete *outStr;
			*outStr = NULL;
		}
	}

	return result;
}


///////////////////////////////////////////////////////////////////////////////
//
// waits for a window title and returns the app. window handle
//
// param  - window    - window title
//          handleWnd - returned window handle (must be deallocated)
//          timeout   - timeout value in ms
//          refresh   - refresh cycle to look for the window in ms
//
// return - success - true
//          failure - false
//
///////////////////////////////////////////////////////////////////////////////

BOOL WINAPI PlWaitForWindow(PSTR window, PSTR *handleWnd, DWORD timeout, DWORD refresh)
{
	// window handle must be valid
	if(!handleWnd)
		return FALSE;

	for(DWORD startTime = GetTickCount(), curTime = startTime; TRUE; curTime = GetTickCount())
	{
		// look for the window
		HWND hWnd = FindWindow(NULL, window);

		// did we found it
		if(hWnd)
		{
			// allocate memory
			if(!(*handleWnd = new char[sizeof(HWND_STR) + sizeof(HEXINT_STR)]))
			{
				SetLastError(NOT_ENOUGTH_MEM);
				return FALSE;
			}

			// and store the result
			lstrcpy(*handleWnd, HWND_STR);
			wsprintf(*handleWnd + lstrlen(*handleWnd), "%08x", hWnd);

			break;
		}
		
		// ckeck for overflow
		if(timeout && curTime < startTime)
			timeout -= 0xffffffff - startTime + curTime; 

		// check for timeout
		if(timeout && curTime - startTime >= timeout)
			return FALSE;

		// wait a while 
		Sleep(refresh);
	}

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
//
// determines if we have a special char in the string form beg to end
//
// param  - beg - begin of string to look in
//          end - end of string to look in
//          curKey - pointer to a char to store the key
//          altCtrlShift - true if a special key is pressed
//
// return - true if there is a special key
//
///////////////////////////////////////////////////////////////////////////////

BOOL GetSpecialKeyCode(PSTR beg, PSTR end, BYTE *curKey, BOOL *altCtrlShift)
{
	*altCtrlShift = FALSE;

	if(!strnicmp(VK_ALT_STR, beg, end - beg))
	{
		*curKey = VK_MENU;
		*altCtrlShift = TRUE;
		return TRUE;
	}

	if(!strnicmp(VK_CTRL_STR, beg, end - beg))
	{
		*curKey = VK_CONTROL;
		*altCtrlShift = TRUE;
		return TRUE;
	}

	if(!strnicmp(VK_SHIFT_STR, beg, end - beg))
	{
		*curKey = VK_SHIFT;
		*altCtrlShift = TRUE;
		return TRUE;
	}

	if(!strnicmp(VK_TAB_STR, beg, end - beg))
	{
		*curKey = VK_TAB;
		return TRUE;
	}

	if(!strnicmp(VK_ENTER_STR, beg, end - beg))
	{
		*curKey = VK_RETURN;
		return TRUE;
	}

	if(!strnicmp(VK_ESC_STR, beg, end - beg))
	{
		*curKey = VK_ESCAPE;
		return TRUE;
	}

	if(!strnicmp(VK_INSERT_STR, beg, end - beg))
	{
		*curKey = VK_INSERT;
		return TRUE;
	}

	if(!strnicmp(VK_BACK_STR, beg, end - beg))
	{
		*curKey = VK_BACK;
		return TRUE;
	}

	if(!strnicmp(VK_DEL_STR, beg, end - beg))
	{
		*curKey = VK_DELETE;
		return TRUE;
	}

	if(!strnicmp(VK_HELP_STR, beg, end - beg))
	{
		*curKey = VK_HELP;
		return TRUE;
	}

	if(!strnicmp(VK_LEFT_STR, beg, end - beg))
	{
		*curKey = VK_LEFT;
		return TRUE;
	}

	if(!strnicmp(VK_RIGHT_STR, beg, end - beg))
	{
		*curKey = VK_RIGHT;
		return TRUE;
	}

	if(!strnicmp(VK_UP_STR, beg, end - beg))
	{
		*curKey = VK_UP;
		return TRUE;
	}

	if(!strnicmp(VK_DOWN_STR, beg, end - beg))
	{
		*curKey = VK_DOWN;
		return TRUE;
	}

	if(!strnicmp(VK_PGUP_STR, beg, end - beg))
	{
		*curKey = VK_PRIOR;
		return TRUE;
	}

	if(!strnicmp(VK_PGDOWN_STR, beg, end - beg))
	{
		*curKey = VK_NEXT;
		return TRUE;
	}

	if(!strnicmp(VK_BEG_STR, beg, end - beg))
	{
		*curKey = VK_HOME;
		return TRUE;
	}

	if(!strnicmp(VK_END_STR, beg, end - beg))
	{
		*curKey = VK_END;
		return TRUE;
	}

	if(!strnicmp(VK_F1_STR, beg, end - beg))
	{
		*curKey = VK_F1;
		return TRUE;
	}

	if(!strnicmp(VK_F2_STR, beg, end - beg))
	{
		*curKey = VK_F2;
		return TRUE;
	}

	if(!strnicmp(VK_F3_STR, beg, end - beg))
	{
		*curKey = VK_F3;
		return TRUE;
	}

	if(!strnicmp(VK_F4_STR, beg, end - beg))
	{
		*curKey = VK_F4;
		return TRUE;
	}

	if(!strnicmp(VK_F5_STR, beg, end - beg))
	{
		*curKey = VK_F5;
		return TRUE;
	}

	if(!strnicmp(VK_F6_STR, beg, end - beg))
	{
		*curKey = VK_F6;
		return TRUE;
	}

	if(!strnicmp(VK_F7_STR, beg, end - beg))
	{
		*curKey = VK_F7;
		return TRUE;
	}

	if(!strnicmp(VK_F8_STR, beg, end - beg))
	{
		*curKey = VK_F8;
		return TRUE;
	}

	if(!strnicmp(VK_F9_STR, beg, end - beg))
	{
		*curKey = VK_F9;
		return TRUE;
	}

	if(!strnicmp(VK_F10_STR, beg, end - beg))
	{
		*curKey = VK_F10;
		return TRUE;
	}

	if(!strnicmp(VK_F11_STR, beg, end - beg))
	{
		*curKey = VK_F11;
		return TRUE;
	}

	if(!strnicmp(VK_F12_STR, beg, end - beg))
	{
		*curKey = VK_F12;
		return TRUE;
	}

	if(!strnicmp(VK_NUMPAD0_STR, beg, end - beg))
	{
		*curKey = VK_NUMPAD0;
		return TRUE;
	}

	if(!strnicmp(VK_NUMPAD1_STR, beg, end - beg))
	{
		*curKey = VK_NUMPAD1;
		return TRUE;
	}

	if(!strnicmp(VK_NUMPAD2_STR, beg, end - beg))
	{
		*curKey = VK_NUMPAD2;
		return TRUE;
	}

	if(!strnicmp(VK_NUMPAD3_STR, beg, end - beg))
	{
		*curKey = VK_NUMPAD3;
		return TRUE;
	}

	if(!strnicmp(VK_NUMPAD4_STR, beg, end - beg))
	{
		*curKey = VK_NUMPAD4;
		return TRUE;
	}

	if(!strnicmp(VK_NUMPAD5_STR, beg, end - beg))
	{
		*curKey = VK_NUMPAD5;
		return TRUE;
	}

	if(!strnicmp(VK_NUMPAD6_STR, beg, end - beg))
	{
		*curKey = VK_NUMPAD6;
		return TRUE;
	}

	if(!strnicmp(VK_NUMPAD7_STR, beg, end - beg))
	{
		*curKey = VK_NUMPAD7;
		return TRUE;
	}

	if(!strnicmp(VK_NUMPAD8_STR, beg, end - beg))
	{
		*curKey = VK_NUMPAD8;
		return TRUE;
	}

	if(!strnicmp(VK_NUMPAD9_STR, beg, end - beg))
	{
		*curKey = VK_NUMPAD9;
		return TRUE;
	}

	if(!strnicmp(VK_MULTIPLY_STR, beg, end - beg))
	{
		*curKey = VK_MULTIPLY;
		return TRUE;
	}

	if(!strnicmp(VK_ADD_STR, beg, end - beg))
	{
		*curKey = VK_ADD;
		return TRUE;
	}

	if(!strnicmp(VK_SUBTRACT_STR, beg, end - beg))
	{
		*curKey = VK_SUBTRACT;
		return TRUE;
	}

	if(!strnicmp(VK_DECIMAL_STR, beg, end - beg))
	{
		*curKey = VK_DECIMAL;
		return TRUE;
	}

	if(!strnicmp(VK_DIVIDE_STR, beg, end - beg))
	{
		*curKey = VK_DIVIDE;
		return TRUE;
	}


	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
//
// sends keys to a window; the key string is interpolated; special keys must
// be included in backslashes; if we need a backslash char we have to write
// two backslashes; the following special keys are defined:
// ALT, CTRL, SHIFT, TAB, RET, ESC, BACK, DEL, INS, HELP, LEFT, RIGHT, UP, DN,
// PGUP, PGDN, BEG, END, F1, ... , F12, NUM0, ... , NUM9, NUM*, NUM+, NUM-, 
// NUM and NUM/
//
// param  - hWnd							- Handle of the window to send the keys
//          str								- string wit keys
//          activateEverytime - activates hWnd everytime before sending a key
//															if true; activates only onces if false
//          timeout           - waits timeout milliseconds after sending each
//															key
//
// return - success - true
//          failure - false
//
///////////////////////////////////////////////////////////////////////////////

BOOL WINAPI PlSendKeys(PSTR window, PSTR str, BOOL activateEverytime, DWORD timeout)
{
	BOOL result = TRUE;
	BOOL altKeyPressed = FALSE, ctrlKeyPressed = FALSE, shiftKeyPressed = FALSE;

	HWND hWnd = NULL;

	if(!ScanHex(window, (DWORD*)&hWnd))
		return FALSE;

	// activate foreign windows
	if(!SetForegroundWindow(hWnd))
		return FALSE;

	// walk through the string
	for(BYTE curKey; str && *str; str++)
	{
		// activate foreign windows if wished
		if(activateEverytime && !SetForegroundWindow(hWnd))
			return FALSE;

		// is this a delimiter char and the next is no delimiter
		if(*str == '\\' && *++str != '\\')
		{
			PSTR nextDelim = strchr(str, '\\');
			BOOL altCtrlShift = FALSE;
				
			// we could not find the next delimiter, so there is an error
			if(!nextDelim || !GetSpecialKeyCode(str, nextDelim, &curKey, &altCtrlShift))
			{
				SetLastError(BAD_STR_FORMAT);
				return FALSE;
			}

			// set string to the next char
			str = nextDelim;

			// did we get a alt-, ctrl- or shift key, send it and continue
			if(altCtrlShift)
			{
				keybd_event(curKey, curKey, KEYEVENTF_EXTENDEDKEY, 0);
				
				// remember we send an alt-, crtl- or shift-keydown
				if(curKey == VK_MENU)
					altKeyPressed = TRUE;

				if(curKey == VK_CONTROL)
					ctrlKeyPressed = TRUE;

				if(curKey == VK_SHIFT)
					shiftKeyPressed = TRUE;

				continue;
			}
		}
		else
		{
			// we have a normal char or the next char is a delimiter (well then we treat 
			// it as a normal char); get the scancode now 
			short scanCode = VkKeyScan(*str);

			if(HIBYTE(scanCode) & 1)
			{
				shiftKeyPressed = TRUE;
				keybd_event(VK_SHIFT, VK_SHIFT, KEYEVENTF_EXTENDEDKEY, 0);
			}

			if(HIBYTE(scanCode) & 2)
			{
				ctrlKeyPressed = TRUE;
				keybd_event(VK_CONTROL, VK_CONTROL, KEYEVENTF_EXTENDEDKEY, 0);
			}

			if(HIBYTE(scanCode) & 4)
			{
				altKeyPressed = TRUE;
				keybd_event(VK_MENU, VK_MENU, KEYEVENTF_EXTENDEDKEY, 0);
			}

			curKey = LOBYTE(scanCode);
		}

		// now we send the keyboard events (down and up)
		keybd_event(curKey, curKey, 0, 0);
		keybd_event(curKey, curKey, KEYEVENTF_KEYUP, 0);

		// did we send a special char, we have to send a keyup now
		if(shiftKeyPressed)
		{
			shiftKeyPressed = FALSE;
			keybd_event(VK_SHIFT, VK_SHIFT, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		}

		if(ctrlKeyPressed)
		{
			ctrlKeyPressed = FALSE;
			keybd_event(VK_CONTROL, VK_CONTROL, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		}

		if(altKeyPressed)
		{
			altKeyPressed = FALSE;
			keybd_event(VK_MENU, VK_MENU, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		}
	
		// wait a while
		if(timeout)
			Sleep(timeout);

	} // for(BYTE curKey; str && *str; str++)

	return result;
}




//###################################
//###################################
//###################################

PSTR StrStr(PSTR str, PSTR substr, BOOL ignoreCase)
{
	if(!str || !substr)
		return NULL;

	for(int substrLen = lstrlen(substr); *str; str++)
		if(!(CompareString(LOCALE_USER_DEFAULT, ignoreCase ? NORM_IGNORECASE : 0, 
											str, substrLen, substr, substrLen) - 2))
			return str;

	return NULL;
}


#define CMP_STR_POS(str1, str2) (str2 ? str1 < str2 : TRUE)
  

BOOL CheckOptions(PSTR windowList)
{
	if(!windowList)
		return FALSE;

	for(PSTR recordBeg = windowList, recordEnd, recordPtr;	recordBeg; 
			recordBeg = recordEnd ? recordEnd + 1 : NULL)
	{
		recordEnd = strchr(recordBeg, ';');

		// look for identifiers until recordEnd
		char windowBuf[sizeof(HEXINT_STR)] = "";
		
		if((recordPtr = StrStr(recordBeg, HWND_STR, TRUE)) && CMP_STR_POS(recordPtr, recordEnd))
			strncpy(windowBuf, recordPtr + strlen(HWND_STR), strlen(HEXINT_STR));

		char classBuf[sizeof(HEXINT_STR)] = "";

		if((recordPtr = StrStr(recordBeg, CLASS_STR, TRUE)) && CMP_STR_POS(recordPtr, recordEnd))
			strncpy(classBuf, recordPtr + strlen(CLASS_STR), strlen(HEXINT_STR));

/*
		char textBuf[sizeof(HEXINT_STR)] = "";

		if(recordPtr = StrStr(recordBeg, CLASS_STR, TRUE) && CMP_STR_POS(recordPtr, recordEnd))
			strncpy(classBuf, recordPtr + strlen(CLASS_STR), strlen(HEXINT_STR));
*/

			
	}

	return FALSE;
}



PSTR GetNextTokenPtr(PSTR str)
{
	if(!str)
		return NULL;

	PSTR identifiers[] = 
		{ HWND_STR, CLASS_STR, TEXT_STR, ID_STR, ISVALID_STR, ISVISIBLE_STR, ISENABLED_STR,
			ISCHECKED_STR, ISICONIC_STR, ISZOOMED_STR, ISFOREGROUND_STR, HASFOCUS_STR,
			PARENT_STR, PREV_STR, NEXT_STR, RECT_SIZE_STR, CLIENT_SIZE_STR, PROCEDURE_STR,
			INSTANCE_STR, STYLE_STR, EXTSTYLE_STR, MENU_STR, ICON_STR, PROCESS_STR, THREAD_STR };			

	for(int count = 0; count < sizeof(identifiers) / sizeof(identifiers[0]); count++)
	{
		PSTR nextToken = StrStr(str, identifiers[count], TRUE);

		if(nextToken)
			return nextToken;
	}


	return NULL;
}


#define CopyTokenValue(ident, propBool, propStr, check)																		\
	if(curToken = StrStr(window, ident, TRUE))																							\
	{																																												\
		if(!(prop->##propStr##Str = 																													\
			new char[tokenLen = ((nextToken = GetNextTokenPtr(curToken += lstrlen(ident))) ?		\
														nextToken - curToken : strlen(curToken)) + 1]))								\
			LeaveFalseError(NOT_ENOUGTH_MEM);																										\
		ZeroMemory(prop->##propStr##Str, tokenLen);																						\
		strncpy(prop->##propStr##Str, curToken, tokenLen - 1);																\
		prop->store##propBool##Str = TRUE;																										\
		if(!check)																																						\
			LeaveFalseError(BAD_STR_FORMAT);																										\
	}




BOOL z(PSTR window, PWindowProp prop)
{
	if(!window || !prop)
		return FALSE;

	ZeroMemory(prop, sizeof(*prop));

	BOOL result = TRUE;
	PSTR curToken, nextToken;
	int tokenLen;

	__try
	{
		CopyTokenValue(HWND_STR, Window, window, ScanHex(prop->windowStr, NULL));
		CopyTokenValue(CLASS_STR, Class, class, TRUE);
		CopyTokenValue(TEXT_STR, Text, text, TRUE);

		CopyTokenValue(ID_STR, Id, id, ScanHex(prop->idStr, NULL));
		CopyTokenValue(ISVALID_STR, IsValid, isValid, ScanBit(prop->isValidStr, NULL));
		CopyTokenValue(ISVISIBLE_STR, IsVisible, isVisible, TRUE);
		CopyTokenValue(ISENABLED_STR, IsEnabled, isEnabled, TRUE);
		CopyTokenValue(ISCHECKED_STR, IsChecked, isChecked, TRUE);
		CopyTokenValue(ISICONIC_STR, IsIconic, isIconic, TRUE);
		CopyTokenValue(ISZOOMED_STR, IsZoomed, isZoomed, TRUE);
		CopyTokenValue(ISFOREGROUND_STR, IsForeground, isForeground, TRUE);
		CopyTokenValue(HASFOCUS_STR, HasFocus, hasFocus, TRUE);
		CopyTokenValue(PARENT_STR, Parent, parent, TRUE);
		CopyTokenValue(PREV_STR, Prev, prev, TRUE);
		CopyTokenValue(NEXT_STR, Next, next, TRUE);

		CopyTokenValue(CLASS_STR, Class, class, FALSE);
		CopyTokenValue(CLASS_STR, Class, class, FALSE);
		CopyTokenValue(CLASS_STR, Class, class, FALSE);
		CopyTokenValue(CLASS_STR, Class, class, FALSE);
		CopyTokenValue(CLASS_STR, Class, class, FALSE);
		CopyTokenValue(CLASS_STR, Class, class, FALSE);
		CopyTokenValue(CLASS_STR, Class, class, FALSE);
		CopyTokenValue(CLASS_STR, Class, class, FALSE);
		CopyTokenValue(CLASS_STR, Class, class, FALSE);
		CopyTokenValue(CLASS_STR, Class, class, FALSE);

/*
WindowStr;
ClassStr;
TextStr;
IdStr;
IsValidStr;
IsVisibleStr;
IsEnabledStr;
IsCheckedStr;
IsIconicStr;
IsZoomedStr;
IsForegroundStr;
HasFocusStr;
ParentStr;
PrevStr;
NextStr;

RectStr;
ClientStr;
ProcedureStr;
InstanceStr;
StyleStr;
ExtStyleStr;
MenuStr;
IconStr;
ProcessStr;
ThreadStr;
*/




	}
	__finally
	{
		if(!result)
			FreeWindowPropMem(prop);
	}

	return result;
}



BOOL BuildOptionsList(PSTR windowList, PList list)
{
	if(!windowList || !list)
		return FALSE;

	list->RemoveAll();

	BOOL result = TRUE;
	char *recordStr = NULL;
	PWindowProp prop = NULL;

	__try
	{
		for(PSTR recordBeg = windowList, recordEnd; recordBeg && *recordBeg; recordBeg = recordEnd)
		{
			int recordLen = 
				((recordEnd = StrStr(recordBeg, SEPARATOR_STR, TRUE)) ? 
					recordEnd - recordBeg : lstrlen(recordBeg)) + 1;
			
			if(!(recordStr = new char[recordLen]))
				LeaveFalseError(NOT_ENOUGTH_MEM);

			ZeroMemory(recordStr, recordLen);
			strncpy(recordStr, recordBeg, recordLen - 1);

			if(!(prop = new WindowProp))
				LeaveFalseError(NOT_ENOUGTH_MEM);

			if(!z(recordStr, prop))
				LeaveFalse();

			if(!list->AddTail(prop))
				LeaveFalseError(NOT_ENOUGTH_MEM);

			delete recordStr;
			recordStr = NULL;
			prop = NULL;

			if(recordEnd)
				recordEnd++;
		}
	}
	__finally
	{
		if(!result)
			list->RemoveAll();

		if(recordStr)
			delete recordStr;

		if(prop)
			delete prop;
	}

	return result;
}


///////////////////////////////////////////////////////////////////////////////
//
// waits for a window title and returns the app. window handle
//
// param  - window    - window title
//          handleWnd - returned window handle (must be deallocated)
//          timeout   - timeout value in ms
//          refresh   - refresh cycle to look for the window in ms
//
// return - success - true
//          failure - false
//
///////////////////////////////////////////////////////////////////////////////

BOOL WINAPI PlWaitForWindowOption(PSTR windowList, DWORD timeout, DWORD refresh)
{
	// window list must be valid
	if(!windowList)
		return FALSE;

	BOOL result = TRUE;

	List list;

	if(!BuildOptionsList(windowList, &list))
		return FALSE;

	for(DWORD startTime = GetTickCount(), curTime = startTime; TRUE; curTime = GetTickCount())
	{
		// check if all options are true
		if(CheckOptions(windowList))
			break;

		// ckeck for overflow
		if(timeout && curTime < startTime)
			timeout -= 0xffffffff - startTime + curTime; 

		// check for timeout
		if(timeout && curTime - startTime >= timeout)
		{
			result = FALSE;
			break;
		}

		// wait a while 
		Sleep(refresh);
	}

	list.RemoveAll();

	return result;
}



int main(int argc, char argv)
{
	PSTR hWnd = NULL;

	if(!PlWaitForWindow("test", &hWnd, 30000, 1000))
		return 0;
	
	char *windows = NULL;
	char testwnd[9] = "";

	PSTR p = hWnd + lstrlen("#HWND:");
	PlGetWindowProperties(p, &p);

	strncpy(testwnd, p + lstrlen("#HWND:"), 8);

	if(PlEnumChildWindows(testwnd, &windows))
	{
		char s[4096] = "";

		strcpy(s, p);
		for(PSTR pp = windows; pp && *pp; pp = strchr(++pp, '\n'))
		{
			char wnd[9] = "";

			if(*pp == '\n')
				pp++;

			strncpy(wnd,  pp + lstrlen("#HWND:"), 8);
			if(PlGetWindowProperties(wnd, &p))
			{
					strcat(s, p);
			}
		}

//		puts(s);
		PlWaitForWindowOption(s, 10000, 1000);
	}
	

	return 0;
}

