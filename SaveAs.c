#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <commctrl.h>
#include "resource.h"
#include "main.h"
#include "header.h"

UCHAR palette[256][3];
UINT paletteSize;
UCHAR transparentMap[32][4];


UCHAR MakePalette(UCHAR **ImageBytes)
{
    UINT key,i;
    int y;
    UCHAR *pixel; UCHAR *row; UINT col;
    
    DeleteHashMap();
    memset(palette,0,256*3);
    memset(transparentMap,0,32*4);
    paletteSize=2;
    palette[0][0]=palette[0][1]=palette[0][2]=0;      SetHashMap(0,0,YES); 
    palette[1][0]=palette[1][1]=palette[1][2]=0xFF;   SetHashMap(0xFFFFFF,1,YES);    
    
    for(y=31; y>=0; y--)
    {
        pixel=ImageBytes[y];
        for(i=0; i<32; i++)
        {
            col=*(UINT*)pixel;
            if(col!=ColorToRemove)
            {
                if(SetHashMap(col,0,NO)==YES)
                {
                    palette[paletteSize][0]=pixel[0];
                    palette[paletteSize][1]=pixel[1];
                    palette[paletteSize][2]=pixel[2];
                    SetHashMap(col,paletteSize,YES);
                    paletteSize++;
                    if(paletteSize==256) return NO;
                }                                
            }
            else
            {
                pixel[0]=TransparentBit;
                pixel[1]=TransparentBit;
                pixel[2]=TransparentBit; 
                pixel[3]=1;                                 
            } 
            pixel+=4;
        } 
    }
    
    for(y=31; y>=0; y--)
    {
        row=ImageBytes[y];
        pixel=row;
        col=0;
        for(i=0; i<32; i++)
        {
            key=pixel[i<<2]+(pixel[(i<<2)+1]<<8)+(pixel[(i<<2)+2]<<16);
            if(pixel[(i<<2)+3]==1) col++;              

            if((i%8)==7){ transparentMap[y][i>>3]=col; col=0; }else{ col<<=1; }
            row[i]=GetHashMap(key);
        }
    }
  return YES;
}


void SaveImage(UCHAR **ImageBytes,USHORT xx,USHORT yy,UCHAR *name,USHORT type)
{
    HANDLE hFile;  DWORD dwWritten; UCHAR *p; UCHAR color[4]; DWORD x; int y;
    
    if(MakePalette(ImageBytes) == NO)
    {
          MessageBox(hWnd,"The Image has more than 256 colors.\nTry to reduce them.","#INFO",MB_OK |MB_ICONINFORMATION);  
          return;
    }
    hFile = CreateFile(name, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if(hFile == INVALID_HANDLE_VALUE)
    {
        MessageBox(hWnd,"ERROR: Can't CreateFile!!!","#Error",MB_OK |MB_ICONERROR);        
        return;
    } 
  
    icoHeader.idType=type;
    icoHeader.idhotSpotX=xx;
    icoHeader.idhotSpotY=yy;
    //write header
    p=(UCHAR*)&icoHeader;
    if(WriteFile(hFile, p, sizeof(ICONHEADER), &dwWritten, NULL)==0)
    {
        MessageBox(hWnd,"ERROR: Can't WriteFile!!!","#Error",MB_OK |MB_ICONERROR);
        CloseHandle(hFile);      
        return;
    }
    
    for(x=0; x<256; x++)
    {
        color[0]=palette[x][2];
        color[1]=palette[x][1];
        color[2]=palette[x][0];
        color[3]=0;
        if(WriteFile(hFile,color, 4, &dwWritten, NULL)==0)
        {
            MessageBox(hWnd,"ERROR: Can't WriteFile!!!","#Error",MB_OK |MB_ICONERROR);
            CloseHandle(hFile);      
            return;
        }
    }

    for(y=31; y>=0; y--)
    {
        if(WriteFile(hFile,ImageBytes[y],32,&dwWritten, NULL)==0)
        {
            MessageBox(hWnd,"ERROR: Can't WriteFile!!!","#Error",MB_OK |MB_ICONERROR);
            CloseHandle(hFile);      
            return;
        }
    }

    for(y=31; y>=0; y--)
    {
        if(WriteFile(hFile,transparentMap[y],4,&dwWritten, NULL)==0)
        {
            MessageBox(hWnd,"ERROR: Can't WriteFile!!!","#Error",MB_OK |MB_ICONERROR);
            CloseHandle(hFile);      
            return;
        }
    }
    CloseHandle(hFile);
   MessageBox(hWnd,name,"#File Saved",MB_OK |MB_ICONINFORMATION);  
}

//-------------------------- save as BMP -------------------------------------------

void ImageToBmp(UCHAR *name)
{
    HANDLE hFile;  DWORD dwWritten; UCHAR *p;
    hFile = CreateFile(name, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if(hFile == INVALID_HANDLE_VALUE)
    {
        MessageBox(hWnd,"ERROR: Can't CreateFile!!!","#Error",MB_OK |MB_ICONERROR);        
        return;
    } 
    //write header
    p=(UCHAR*)&bmpheader;
    if(WriteFile(hFile, p, sizeof(BMPHEADER), &dwWritten, NULL)==0)
    {
        MessageBox(hWnd,"ERROR: Can't WriteFile!!!","#Error",MB_OK |MB_ICONERROR);
        CloseHandle(hFile);      
        return;
    }
    //write pixel bytes
    p=GetPixelEntry(); 
    if(WriteFile(hFile, p, 3072, &dwWritten, NULL)==0)
    {
        MessageBox(hWnd,"ERROR: Can't WriteFile!!!","#Error",MB_OK |MB_ICONERROR);
        CloseHandle(hFile);      
        return;
    }
    CloseHandle(hFile);
   MessageBox(hWnd,name,"#File Saved",MB_OK |MB_ICONINFORMATION);  
}



