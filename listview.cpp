#include <windows.h>
#include "resource\resource.h"
#include <commctrl.h>
#include <stdio.h>

//==============Global Vatriabls===================
static HWND hList=NULL;  // List View identifier
LVCOLUMN LvCol; // Make Coluom struct for ListView
LVITEM LvItem;  // ListView Item struct
LV_DISPINFO lvd;
int iSelect=0;
int index=0;
int flag=0;
HWND hEdit;
bool escKey=0;
char tempstr[100]="";
TCHAR tchar;
MSG msg;
//===================================================

//======================Handles================================================//
HINSTANCE hInst; // main function handler
#define WIN32_LEAN_AND_MEAN // this will assume smaller exe

LRESULT ProcessCustomDraw (LPARAM lParam)
{
    return CDRF_DODEFAULT;
}

//================================About dialog window=============================//
BOOL CALLBACK DialogProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
  switch(Message)
  {
        
         // This Window Message will close the dialog  //
		//============================================//

        case WM_CLOSE:
        {
          PostQuitMessage(0);
          EndDialog(hWnd,0); // kill dialog
        }
        break;

		case WM_NOTIFY:
		{
			switch(LOWORD(wParam))
			{
			    case IDC_LIST: 
				LPNMLISTVIEW pnm = (LPNMLISTVIEW)lParam;

                if(pnm->hdr.hwndFrom == hList &&pnm->hdr.code == NM_CUSTOMDRAW)
                {
                    SetWindowLong(hWnd, DWLP_MSGRESULT, (LONG)ProcessCustomDraw(lParam));
                    return TRUE;
                }
				
                if(((LPNMHDR)lParam)->code == NM_DBLCLK)
				{
				  char Text[255]={0};  
				  char Temp[255]={0};
				  char Temp1[255]={0};
				  int iSlected=0;
				  int j=0;

				  iSlected=(int)SendMessage(hList,LVM_GETNEXTITEM,-1,LVNI_FOCUSED);
				  
				  if(iSlected==-1)
				  {
                    MessageBox(hWnd,"No Items in ListView","Error",MB_OK|MB_ICONINFORMATION);
					break;
				  }

				  memset(&LvItem,0,sizeof(LvItem));
                  LvItem.mask=LVIF_TEXT;
				  LvItem.iSubItem=0;
				  LvItem.pszText=Text;
				  LvItem.cchTextMax=256;
				  LvItem.iItem=iSlected;
                  
				  SendMessage(hList,LVM_GETITEMTEXT, iSlected, (LPARAM)&LvItem);
				  
				  sprintf_s(Temp1,Text);
				  
				  for(j=1;j<=5;j++)
				  {
					LvItem.iSubItem=j;
				    SendMessage(hList,LVM_GETITEMTEXT, iSlected, (LPARAM)&LvItem);
				    sprintf_s(Temp," %s",Text);
					lstrcat(Temp1,Temp);
				  }

				  MessageBox(hWnd,Temp1,"test",MB_OK);

				}
				if(((LPNMHDR)lParam)->code == NM_CLICK)
				{
					iSelect=(int)SendMessage(hList,LVM_GETNEXTITEM,-1,LVNI_FOCUSED);
				    
					if(iSelect==-1)
					{                      
					  break;
					}
					index=iSelect;
					flag=1;
				}

                if(((LPNMHDR)lParam)->code == LVN_BEGINLABELEDIT)
                {
                  //Editing=1;
                  hEdit=ListView_GetEditControl(hList);
                  GetWindowText(hEdit, tempstr, sizeof(tempstr));
                }
				
                if(((LPNMHDR)lParam)->code == LVN_ENDLABELEDIT)
                {
                    int iIndex;
                    char text[255]="";

                    tchar = (TCHAR)msg.wParam;
                    if(tchar == 0x1b)
                          escKey=1;

                    iIndex= (int)SendMessage(hList,LVM_GETNEXTITEM,-1,LVNI_FOCUSED);
                    if(iIndex==-1)
					   break;
					
					LvItem.iSubItem=0;
                                       
                    if(escKey==0)
					{
						LvItem.pszText=text; 
						GetWindowText(hEdit, text, sizeof(text));
                        SendMessage(hList,LVM_SETITEMTEXT,(WPARAM)iIndex,(LPARAM)&LvItem);
					}
                    else{
                        LvItem.pszText=tempstr;
                        SendMessage(hList,LVM_SETITEMTEXT,(WPARAM)iIndex,(LPARAM)&LvItem);
                        escKey=0;
                    }
                    //Editing=0;
                }
                break;
			}
		}

		case WM_PAINT:
			{
				return 0;
			}
			break;

		// This Window Message is the heart of the dialog  //
		//================================================//
		case WM_INITDIALOG:
			{
                int i;
				char Temp[255];
			
				InitCommonControls();
				hList=GetDlgItem(hWnd,IDC_LIST); // get the ID of the ListView				 
				
	
				OleInitialize(NULL);
				
				
                SendMessage(hList,LVM_SETEXTENDEDLISTVIEWSTYLE,0,LVS_EX_FULLROWSELECT); // Set style
				
				SendMessageA(hWnd,WM_SETICON,(WPARAM) 1,(LPARAM) LoadIconA(hInst,MAKEINTRESOURCE(IDI_ICON1)));
			
				// Here we put the info on the Coulom headers
				// this is not data, only name of each header we like
                memset(&LvCol,0,sizeof(LvCol)); // Reset Coluom
				LvCol.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM; // Type of mask
				LvCol.cx=0x28;                                // width between each coloum
				LvCol.pszText="Itemä";                     // First Header
 				LvCol.cx=0x42;

				// Inserting Couloms as much as we want
				SendMessage(hList,LVM_INSERTCOLUMN,0,(LPARAM)&LvCol); // Insert/Show the coloum
				LvCol.pszText="Sub Item1";                          // Next coloum
                SendMessage(hList,LVM_INSERTCOLUMN,1,(LPARAM)&LvCol); // ...
				LvCol.pszText="Sub Item2";                       //
                SendMessage(hList,LVM_INSERTCOLUMN,2,(LPARAM)&LvCol); //
				LvCol.pszText="Sub Item3";                              //
                SendMessage(hList,LVM_INSERTCOLUMN,3,(LPARAM)&LvCol); //
				LvCol.pszText="Sub Item4";                            //
                SendMessage(hList,LVM_INSERTCOLUMN,4,(LPARAM)&LvCol); //
				LvCol.pszText="Sub Item5";                      //
                SendMessage(hList,LVM_INSERTCOLUMN,5,(LPARAM)&LvCol); // ...same as above

                memset(&LvItem,0,sizeof(LvItem)); // Reset Item Struct
				
				//  Setting properties Of Items:

				LvItem.mask=LVIF_TEXT;   // Text Style
				LvItem.cchTextMax = 256; // Max size of test
                
				LvItem.iItem=0;          // choose item  
				LvItem.iSubItem=0;       // Put in first coluom
				LvItem.pszText="Itemä 0"; // Text to display (can be from a char variable) (Items)
                
				SendMessage(hList,LVM_INSERTITEM,0,(LPARAM)&LvItem); // Send to the Listview
				
				for(i=1;i<=5;i++) // Add SubItems in a loop
				{

					LvItem.iSubItem=i;
					sprintf_s(Temp,"SubItem %d",i);
					LvItem.pszText=Temp;
					SendMessage(hList,LVM_SETITEM,0,(LPARAM)&LvItem); // Enter text to SubItems
				}

				auto edit_ctrl = GetDlgItem(hWnd, IDC_ADD);
				if (edit_ctrl)	
				{ 
					
					NONCLIENTMETRICS info;
					info.cbSize = sizeof(info);
					::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, info.cbSize, &info, 0);
					info.lfMenuFont.lfCharSet = ANSI_CHARSET;
					HFONT f = ::CreateFontIndirect(&info.lfMenuFont);
					SendMessage(edit_ctrl, WM_SETFONT, (WPARAM)f, 1);

					SetWindowText(edit_ctrl, "ääàâßäòöõî");
				}

				auto button = GetDlgItem(hWnd, IDC_ADDITEM);
				if (button)
				{
					SetWindowText(button, "ädd item");
				}

				auto hdc = ::GetWindowDC(NULL);
				BYTE charset_desktop = 0;
				if (hdc)
				{
					charset_desktop = (BYTE)::GetTextCharsetInfo(hdc, NULL, 0);
					::ReleaseDC(NULL, hdc);
				}
				auto hdc_wnd = ::GetWindowDC(hWnd);
				BYTE charset_hwnd = 0;
				if (hdc_wnd)
				{
					charset_hwnd = (BYTE)::GetTextCharsetInfo(hdc_wnd, NULL, 0);
					::ReleaseDC(NULL, hdc_wnd);
				}

				HANDLE font = (HANDLE)SendMessage(hList, WM_GETFONT, 0, 0);
				LOGFONT lf;
				GetObject(font, sizeof(LOGFONT), &lf);
				HANDLE font_edit = (HANDLE)SendMessage(edit_ctrl, WM_GETFONT, 0, 0);
				LOGFONT lf_edit;
				GetObject(font_edit, sizeof(LOGFONT), &lf_edit);

				
				char buffer[4096];
				sprintf_s(buffer, "Win32 ListView Control UTF-8/Legacy Tëst ACP: %d, GetTextCharsetInfo(0): %d, (hWnd): %d, (list font charset): %d (edit): %d (unicode: %d)",
					GetACP(), charset_desktop, charset_hwnd, lf.lfCharSet, lf_edit.lfCharSet, IsWindowUnicode(edit_ctrl));

				SetWindowText(hWnd, buffer);

                //ListView_SetItemState(hList,0,LVIS_SELECTED	,LVIF_STATE);
                ShowWindow(hWnd,SW_NORMAL); 
                UpdateWindow(hWnd); 

                while(TRUE)
                {
                    if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
                    {   
                        /*
                        if(msg.message==WM_CHAR)
                        {
                            tchar = (TCHAR)msg.wParam;
                            if(tchar == 0x1b)
                                escKey=1;

                        }
                        */
                        if(msg.message==WM_QUIT)// killing while looking for a message
                        {
                                break;
                        }
                        
                        
                        TranslateMessage(&msg);
                        DispatchMessage(&msg);
                    }                   
                }
			}
			break;

     // This Window Message will control the dialog  //
	//==============================================//
        case WM_COMMAND:
		{
                 switch(LOWORD(wParam)) // what we press on?
				 {

					   case IDC_ADDITEM:
						 {
                           int iItem;
						   char ItemText[100];

						   iItem= (int)SendMessage(hList,LVM_GETITEMCOUNT,0,0);
                           
						   GetDlgItemText(hWnd,IDC_ADD,ItemText,100);

						   if((lstrlen(ItemText))==0)
						   {
							   MessageBox(hWnd,"Please Write Some Text","Error",MB_OK|MB_ICONINFORMATION);
						       break;
						   }

                           LvItem.iItem=iItem;            // choose item  
				           LvItem.iSubItem=0;         // Put in first coluom
				           LvItem.pszText=ItemText;   // Text to display (can be from a char variable) (Items)
                           SendMessage(hList,LVM_INSERTITEM,0,(LPARAM)&LvItem); // Send to the Listview
						 }
						 break;

					case IDC_ADDSUBITEM:
						{
                           int Item,i;
						   char SubItemText[100];

						   Item= (int)SendMessage(hList,LVM_GETITEMCOUNT,0,0);
                           
						   GetDlgItemText(hWnd,IDC_ADDSUB,SubItemText,100);
						   
						   if((lstrlen(SubItemText))==0)
						   {
							   MessageBox(hWnd,"Please Write Some Text","Error",MB_OK|MB_ICONINFORMATION);
						       break;
						   }

						   LvItem.iItem=Item-1;            // choose item  
				           
						   for(i=1;i<=5;i++)
						   {
				              LvItem.pszText=SubItemText;   // Text to display (can be from a char variable) (Items)
                              LvItem.iSubItem=i;         // Put in first coluom
						      SendMessage(hList,LVM_SETITEM,0,(LPARAM)&LvItem);
						   }
						}
						break;

					case IDC_BOTH:
						{
                           int itemIndex,j;
						   char iSubItemText[100]="";
						   char iItemText[100]="";

						   itemIndex= (int)SendMessage(hList,LVM_GETITEMCOUNT,0,0);
                           
						   GetDlgItemText(hWnd,IDC_ADD,iItemText,100);
						   GetDlgItemText(hWnd,IDC_ADDSUB,iSubItemText,100);
                           
						   if((lstrlen(iSubItemText) && lstrlen(iItemText))==0)
						   {
							   MessageBox(hWnd,"Please Write Some Text","Error",MB_OK|MB_ICONINFORMATION);
						       break;
						   }

						   LvItem.iItem=itemIndex;            // choose item  
				           LvItem.iSubItem=0;
						   LvItem.pszText=iItemText;
						   SendMessage(hList,LVM_INSERTITEM,0,(LPARAM)&LvItem);
						   
						   for(j=1;j<=5;j++)
						   {
				              LvItem.pszText=iSubItemText;   // Text to display (can be from a char variable) (Items)
                              LvItem.iSubItem=j;         // Put in first coluom
						      SendMessage(hList,LVM_SETITEM,0,(LPARAM)&LvItem);
						   }
						}
						break;

					case IDC_DELALL:
						SendMessage(hList,LVM_DELETEALLITEMS,0,0);
						break;

					case IDC_DELSELITEM:
						if(flag)
                           SendMessage(hList,LVM_DELETEITEM,iSelect,0);
						flag=0;
						break;

					case IDC_RENAME:
					{
						if(index==-1)
						{
						   MessageBox(hWnd,"Nothing to rename","error",MB_OK);
						}
						else{
							//Editing=1;
							SendMessage(hList,LVM_EDITLABEL ,(WPARAM)index,(LPARAM)0);
						}
					}
					break;

                    case IDC_SELECT_ALL:
                    {
                        ListView_SetItemState(hList, -1, 0, LVIS_SELECTED); // deselect all
                        SendMessage(hList,LVM_ENSUREVISIBLE ,(WPARAM)-1,FALSE); // Send to the Listview                        
                        ListView_SetItemState(hList,-1,LVIS_SELECTED ,LVIS_SELECTED);                
                    }
                    break;

                    case IDC_LAST_ITEM:
                    {            
                        int items;
                        items = (int)SendMessage(hList,LVM_GETITEMCOUNT ,(WPARAM)0,(LPARAM)0);
                        items--;
                        ListView_SetItemState(hList, -1, 0, LVIS_SELECTED); // deselect all
                        SendMessage(hList,LVM_ENSUREVISIBLE ,(WPARAM)items,FALSE); // Send to the Listview
                        ListView_SetItemState(hList,items,LVIS_SELECTED ,LVIS_SELECTED);
                        ListView_SetItemState(hList,items,LVIS_FOCUSED ,LVIS_FOCUSED);
                        
                    }
                    break;

				 }
		}
        break;
    
	    default:
		{
             return FALSE;
		}
    }

	return TRUE;
}

//===========================MAIN FUNCTION-WIN32 STARTING POINT========================================//
int __stdcall WINAPI WinMain(_In_  HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ PSTR szCmdLine, _In_ int iCmdShow)
{
	
	INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwICC = ICC_LISTVIEW_CLASSES;
    InitCtrls.dwSize = sizeof(INITCOMMONCONTROLSEX);
    BOOL bRet = InitCommonControlsEx(&InitCtrls);

	hInst=hInstance;
				
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDC_DIALOG), NULL, (DLGPROC)DialogProc,0);
	return 0;
}
//======================================================================================================//