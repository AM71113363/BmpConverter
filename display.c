#include <windows.h>
#include "resource.h"
#include "main.h"
#include "header.h"

//------------------ colors tab -----------------------------
UCHAR BmpColor[BmpColorSize];
HBITMAP Cbmp = NULL;

void ShowColorTab(HWND hW,UCHAR red,UCHAR green,UCHAR blue)
{
    HDC     hdc;
    DWORD i;
    for(i=0;i<BmpColorSize;i+=3)
    {
        BmpColor[i]=blue;                        
        BmpColor[i+1]=green;                        
        BmpColor[i+2]=red;                        
                       
    }
    
    if(Cbmp!=NULL)  DeleteObject(Cbmp);
    hdc = GetDC(NULL);
    HBITMAP hbm = CreateCompatibleBitmap(hdc, 32,32);
    SetDIBits(hdc, hbm, 0, 32, BmpColor,(BITMAPINFO*)&DeafultHeader, DIB_RGB_COLORS);
    ReleaseDC(NULL,hdc);
    SendMessage(hW, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP,(LPARAM)(HANDLE)hbm);
 
}

void SetDrawColor(HWND hw,UCHAR red,UCHAR green,UCHAR blue)
{  
     ShowColorTab(hw,red,green,blue);
}

UINT SetTransparentColor(HWND hw,UCHAR red,UCHAR green,UCHAR blue)
{ 
     UINT ColorToRemove=red+(green<<8)+(blue<<16);   
     ShowColorTab(hw,red,green,blue);
     return ColorToRemove;
     
}

void ShowImageToBitmap(UCHAR *buff)
{
    HDC     hMemDCsrc;
    HDC     hMemDCdst;
    HDC     hdc;
    IsOK=NO;
    if(hNewBm!=NULL)  DeleteObject(hNewBm);
    hdc = GetDC(NULL);
    HBITMAP hbm = CreateCompatibleBitmap(hdc, 32,32);
    SetDIBits(hdc, hbm, 0, 32, buff,(BITMAPINFO*)&DeafultHeader, DIB_RGB_COLORS);
   
    hMemDCsrc = CreateCompatibleDC(hdc);     SelectObject(hMemDCsrc,hbm);
    hMemDCdst = CreateCompatibleDC(hdc);

    hNewBm = CreateBitmap(IMAGEXY,IMAGEXY,1,32,NULL);
    
    SelectObject(hMemDCdst,hNewBm);

    SetStretchBltMode(hMemDCdst, COLORONCOLOR);

    StretchBlt(hMemDCdst,0,0,IMAGEXY,IMAGEXY, hMemDCsrc,0,0,32,32,SRCCOPY);

    DeleteObject(hbm);    
    DeleteDC(hMemDCdst);
    DeleteDC(hMemDCsrc);
    ReleaseDC(NULL,hdc);
    SendMessage(hBitmap, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP,(LPARAM)(HANDLE)hNewBm);
    IsOK=YES;
}


