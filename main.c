#include <windows.h>
#include "resource.h"
#include "main.h"
#include "header.h"

//turn back on error
DWORD TurnBackpoint=0xFFFF;
UCHAR TurnBackColor[]={0,0,0};

WNDPROC OldEditProc;

UCHAR DropFile[MAX_PATH];

void Init()
{
     RECT rC, rD;
     pDrawColor[0]=pDrawColor[1]=pDrawColor[2]=0;
     hNewBm = NULL;
     TransparentBit=0;   //transparent white
     ColorToRemove=0;    //set transparent the first palette entry which is white
     IsOK = NO;          //didn't load nothing yet
     HotSpotX=0;         //default 0,need to be 
     HotSpotY=0;         //chenged by the user
     ShowCoords=NO;      //Do not Show Coords
     SetDrawColor(hDrawColor,0,0,0);
     ColorToRemove=SetTransparentColor(hTransparent,pDrawColor[0],pDrawColor[1],pDrawColor[2]); 
     CHANGECURSOR(ColorCur);
     
     SystemParametersInfo(SPI_GETWORKAREA, 0, &rD, 0);
     GetWindowRect(hWnd, &rC);
     int nX=((rD.right - rD.left) / 2) -((rC.right - rC.left) / 2);
     int nY=((rD.bottom - rD.top) / 2) -((rC.bottom - rC.top) / 2);
     SetWindowPos(hWnd, NULL, nX, nY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void Reset()
{
    CHANGEIT(ID_MODE_DRAW,MF_CHECKED);
    CHANGEIT(ID_MODE_HOTSPOT,MF_UNCHECKED);
    CHANGECURSOR(ColorCur);
    if(ShowCoords!=YES) SetWindowText(hWnd,"Bmp Converter");
}

void ReadDataFromFile()
{
   Reset(); 
   HotSpotX=0;
   HotSpotY=0;   
   IsOK=ReadDataFile(DropFile); //at the begin it calls memset(buffer,0,3126);
   if(IsOK == NO)
   {
       //delete the draw image,since the buffer is empty    
       if(hNewBm!=NULL){  DeleteObject(hNewBm); hNewBm=NULL; }
       SendMessage(hBitmap, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP,(LPARAM)hNewBm);
       InvalidateRect(hWnd,NULL,TRUE);
       return;
   }

   ShowImageToBitmap(GetPixelEntry());
}


void SaveAsBmp()
{
    sprintf(DropFile,"%X.bmp\0",GetTickCount());
    ImageToBmp(DropFile);
    Reset();
}
     

void SaveAsIcon()
{
    sprintf(DropFile,"%X.ico\0",GetTickCount());
    SaveTO(0,0,DropFile,1);
    Reset();
}

void SaveAsCursor()
{
    sprintf(DropFile,"%X.cur\0",GetTickCount());
    SaveTO(HotSpotX,HotSpotY,DropFile,2);
    Reset();
}


LRESULT CALLBACK EditProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  POINT Pos; 
  UCHAR tr[32]; 
  if((message==WM_MOUSEMOVE) && (ShowCoords==YES) )
  {
     GetCursorPos(&Pos);
     ScreenToClient(hBitmap, &Pos);       
     sprintf(tr,"X=%d Y=%d\0",Pos.x>>3,Pos.y>>3);
     SetWindowText(hWnd,tr);                      
  }
  if(message==AM_CHANGECURSOR)
  {                   
      SetClassLong(hBitmap,GCL_HCURSOR,(LONG)lParam); 
      GetCursorPos(&Pos); // just a simple way to update the cursor,better than trying to handle WM_SETCURSOR
      SetCursorPos(Pos.x,Pos.y);
  }
  if(wParam == VK_RBUTTON && hNewBm != NULL && IsOK==YES)//turn back
  {
      if(TurnBackpoint != 0xFFFF)
      {
         UCHAR *p=GetPixelEntry();
         p[TurnBackpoint]=TurnBackColor[0];
         p[TurnBackpoint+1]=TurnBackColor[1]; 
         p[TurnBackpoint+2]=TurnBackColor[2];
         ShowImageToBitmap(p); 
         TurnBackpoint=0xFFF;
      }      
            
  }
  
  if(wParam == VK_LBUTTON && hNewBm != NULL && IsOK!=NO)
  { 
 
      GetCursorPos(&Pos);
      ScreenToClient(hBitmap, &Pos);
     
     if(IsOK==HOTSPOT)
     { 
          HotSpotX=Pos.x>>3;
          HotSpotY=Pos.y>>3;
     }      
     else
     {
           DWORD X=(Pos.x>>3);
           DWORD Y=31-(Pos.y>>3);
           UCHAR *p=GetPixelEntry();
           DWORD point=(Y*32*3)+X*3;
           if((p[point]!=pDrawColor[2]) || (p[point+1]!=pDrawColor[1]) || (p[point+2]!=pDrawColor[0]) )
           {
             TurnBackpoint=point;
             TurnBackColor[0]=p[point];
             TurnBackColor[1]=p[point+1];
             TurnBackColor[2]=p[point+2];
             
             p[point]=pDrawColor[2];
             p[point+1]=pDrawColor[1]; 
             p[point+2]= pDrawColor[0];
             ShowImageToBitmap(p); 
           }
           
      }   
   }               
	return CallWindowProc(OldEditProc, hwnd, message, wParam, lParam);
}


LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   
    switch (message)             
    {
        case WM_CREATE:
        {
             HFONT hFont; HWND s1;
             hWnd = hwnd;
             //draw image
             hBitmap= CreateWindowEx(WS_EX_CLIENTEDGE,"STATIC", "", WS_CHILD | WS_VISIBLE | SS_BITMAP| SS_NOTIFY,
				2, 2, 260,260, hwnd, (HMENU)0, ins, NULL );
             OldEditProc= (WNDPROC) SetWindowLong(hBitmap, GWL_WNDPROC, (LPARAM)EditProc);  
			 hFont = CreateFont(15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Comic Sans MS");//"ARIAL");
			 //transparent color group		
             s1=CreateWindow("BUTTON","Transparent Color",WS_VISIBLE|WS_CHILD|BS_GROUPBOX|BS_CENTER,265,2,107,58,hwnd,(HMENU) 0,ins,NULL);
             SendMessage(s1, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(1, 0));
             CreateWindow("BUTTON","Set",WS_VISIBLE|WS_CHILD|BS_CENTER,315,22,43,33,hwnd,(HMENU)ID_SET_TRANSPARENT_COLOR,ins,NULL);
             hTransparent= CreateWindowEx(WS_EX_CLIENTEDGE,"STATIC", "", WS_CHILD | WS_VISIBLE | SS_BITMAP,
				273, 19, 36,36, hwnd, 0, ins, NULL );
             //draw color group
             s1=CreateWindow("BUTTON","Color",WS_VISIBLE|WS_CHILD|BS_GROUPBOX|BS_CENTER,380,2,57,58,hwnd,(HMENU) 0,ins,NULL);
             SendMessage(s1, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(1, 0));
             hDrawColor= CreateWindowEx(WS_EX_CLIENTEDGE,"STATIC", "", WS_CHILD | WS_VISIBLE | SS_BITMAP,
				390, 19, 36,36, hwnd, 0, ins, NULL );
             //color bitmap 
            s1= CreateWindowEx(WS_EX_CLIENTEDGE,"STATIC", "", WS_CHILD | WS_VISIBLE | SS_BITMAP,
				265, 71, 179,191, hwnd, 0, ins, NULL );
            HBITMAP hb=LoadBitmap(ins,"AAAA");
            SendMessage(s1, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP,(LPARAM)(HANDLE)hb); 
            //some default settings
            Init(); 
            SetTimer(hwnd, 1, 100, NULL); 
            
        }break; 
     /*   case WM_CTLCOLORSTATIC: //on BS_GROUPBOX it shows a transparent background which it was what I wanted, 
                                 //but the TEXT was in the middle of GROUPBOX LINE,I didn't like it.
        {
             HDC hh=(HDC)wParam;
             SetTextColor(hh,RGB(0xFF,0,0));
             SetBkMode(hh,TRANSPARENT);
             return (int)GetStockObject(HOLLOW_BRUSH);
             
        }break;*/
        case WM_TIMER:
        {
           if(GetAsyncKeyState(VK_CONTROL)& 0x8000) 
           {                       
               POINT cursor;
               COLORREF nColor;
               HDC hScreenDC = GetDC(NULL);
               GetCursorPos(&cursor);
               nColor = GetPixel(hScreenDC, cursor.x, cursor.y);
               ReleaseDC(hWnd,hScreenDC);
               pDrawColor[0]=GetRValue(nColor);
               pDrawColor[1]=GetGValue(nColor);
               pDrawColor[2]=GetBValue(nColor);
               SetDrawColor(hDrawColor,pDrawColor[0],pDrawColor[1],pDrawColor[2]);
               if(IsOK==HOTSPOT)
               {
                   IsOK=YES;
                   Reset();
               }                            
           }  
        }
        break;
        case WM_COMMAND:
        {
           switch(LOWORD(wParam))
           {
               case ID_NEW_IMAGE:
               {
                    UCHAR *p=GetPixelEntry();
                    memset(p,0xff,BmpColorSize); //just make it all white
                    ShowImageToBitmap(p); 
                    IsOK=YES;
                    HotSpotX=0;
                    HotSpotY=0;   
                    TurnBackpoint=0xFFFF;
                    Reset();  
               }break;
               case ID_SAVE_IMAGE_BITMAP:
               {
                  CreateThread(0,0,(LPTHREAD_START_ROUTINE)SaveAsBmp,0,0,0);    
               }break;
               case ID_SAVE_IMAGE_ICON:
               {
                  CreateThread(0,0,(LPTHREAD_START_ROUTINE)SaveAsIcon,0,0,0);    
               }break;
               case ID_SAVE_IMAGE_CURSOR:
               {
                  CreateThread(0,0,(LPTHREAD_START_ROUTINE)SaveAsCursor,0,0,0);    
               }break;
               
               case ID_SET_TRANSPARENT_COLOR:
               {    
                 ColorToRemove=SetTransparentColor(hTransparent,pDrawColor[0],pDrawColor[1],pDrawColor[2]); 
               }break;
               //settings
               case ID_SET_TRASPARENT_WHITE:
               {
                    TransparentBit=0xFF;
                    CHANGEIT(ID_SET_TRASPARENT_WHITE,MF_CHECKED);
                    CHANGEIT(ID_SET_TRASPARENT_BLACK,MF_UNCHECKED);
                    
               }break;
               case ID_SET_TRASPARENT_BLACK:
               {
                    TransparentBit=0;
                    CHANGEIT(ID_SET_TRASPARENT_BLACK,MF_CHECKED);
                    CHANGEIT(ID_SET_TRASPARENT_WHITE,MF_UNCHECKED);
                    
               }break;
               //draw or set hotspot
               case ID_MODE_DRAW:
               {
                    if(IsOK<=YES) 
                      break;
                    IsOK=YES;
                    if(GETIT(ID_MODE_SHOWCOORDS)==MF_UNCHECKED) ShowCoords=NO; 
                    Reset();
                    
               }break;
               case ID_MODE_HOTSPOT:
               {
                    if(IsOK==NO)
                       break; 
                    IsOK=HOTSPOT;
                    ShowCoords=YES;
                    CHANGECURSOR(TargetCur);  
                    CHANGEIT(ID_MODE_HOTSPOT,MF_CHECKED);
                    CHANGEIT(ID_MODE_DRAW,MF_UNCHECKED);
                    POINT Pos; 
                    Pos.x=4+(HotSpotX<<3);
                    Pos.y=4+(HotSpotY<<3);
                    ClientToScreen(hBitmap, &Pos);  
                    SetCursorPos(Pos.x,Pos.y);
                      
               }break;
               //show Hide Coords
               case ID_MODE_SHOWCOORDS:
               {
                   if(GETIT(ID_MODE_SHOWCOORDS)==MF_UNCHECKED)
                   {
                      ShowCoords=YES; CHANGEIT(ID_MODE_SHOWCOORDS,MF_CHECKED);
                   }
                   else
                   {
                      ShowCoords=NO; CHANGEIT(ID_MODE_SHOWCOORDS,MF_UNCHECKED);
                      SetWindowText(hWnd,"Bmp Converter");
                   }                    
               }break;     
           }//switch
        }//WM_COMMAND
      break;   
      case WM_DROPFILES: 
      {
		     IsOK=NO;
		     TurnBackpoint=0xFFFF;
		     memset(DropFile,0,MAX_PATH);; 
		     DragQueryFile((HDROP)wParam, 0, DropFile, MAX_PATH);
		     DragFinish((HDROP) wParam);
		     HWND Old=GetForegroundWindow();
		     SNDMSG(Old,WM_KILLFOCUS,(WPARAM)hwnd,0);
		     SetForegroundWindow(hwnd); 
		     CreateThread(0,0,(LPTHREAD_START_ROUTINE)ReadDataFromFile,0,0,0);
      }
      break; 
        case WM_DESTROY:
        {
            KillTimer(hwnd, 1);
            PostQuitMessage (0);
         }   break;
        default: 
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}



int WINAPI WinMain (HINSTANCE _A_, HINSTANCE _B_, LPSTR _C_,  int _D_)
{
    HWND hwnd;
    MSG messages;
    WNDCLASSEX wincl;
    ins = _A_;

    wincl.hInstance = _A_;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS;//|CS_HREDRAW|CS_VREDRAW; //I did handle WM_SETCURSOR,but not 100%
    wincl.cbSize = sizeof (WNDCLASSEX);
    wincl.hIcon = LoadIcon(ins, "A");
    wincl.hIconSm = LoadIcon(ins, "A");
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = "MAINMENU"; 
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;
    
    if (!RegisterClassEx (&wincl))
        return 0;
    ColorCur=LoadCursor(ins, "AA"); //yeah,no need to use MAKEINTRESOURCE if I do not define them
    TargetCur=LoadCursor(ins, "AAA");
    hwnd = CreateWindowEx(WS_EX_ACCEPTFILES|WS_EX_TOPMOST|WS_EX_PALETTEWINDOW,szClassName,"Bmp Converter",WS_SYSMENU,
    0, 0, 450, 308,
    HWND_DESKTOP, NULL, ins, NULL);

    ShowWindow (hwnd, _D_);

    while (GetMessage (&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }
    return messages.wParam;
}

