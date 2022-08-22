#include <windows.h>
#include "resource.h"
#include "main.h"
#include "header.h"


UCHAR buffer[3126];
UINT bufferLen=0;

UCHAR BGRA[4096];
UCHAR *pBGRA[32*4];

//-------------------- load Icon/cursor ----------------------------------//

UCHAR *DecodeT(UCHAR *MapT,UCHAR And,UCHAR cn) //used to detect transparent colors
{
      if(And==0)
         return MapT;
      if(And & 0x01) MapT[cn]=1;
      And>>=1;
      if(cn==0 || And==0)
         return MapT; 
      cn--;
  return DecodeT(MapT,And,cn);
}

void LoadIconCursor()
{
     UCHAR palette[256][4]; //it holds the palette RGB values
     DWORD paletteSize=256*4;
     DWORD AllPixels=32*32;
     //Get hotSpot coords
     HotSpotX=*(USHORT*)&buffer[10];
     HotSpotY=*(USHORT*)&buffer[12];
     
     //first get Palette entry; 0x3E = 62
     memcpy(palette,&buffer[0x3E],paletteSize);
     //using BGRA as a temporary buffer;
     UCHAR *t=&BGRA[0];
     UCHAR *p=&buffer[62 + paletteSize];//skip palette
     DWORD x; UCHAR index;
     for(x=0;x<AllPixels;x++)
     {
         index=p[x];
         *t++=palette[index][0];                
         *t++=palette[index][1];                
         *t++=palette[index][2];                
     }
     //copy transparent map before overwrite
     UCHAR transMap[128];
     DWORD skip=62 + paletteSize + AllPixels;
     //copy the transparent data
     memcpy(&transMap[0],&buffer[skip],128);
    
     //overwrite
     memcpy(&buffer[54],&BGRA[0],AllPixels*3);//a single byte is converted to RGB value
     
     //handle transparent colors
     //use BGRA as transparent bits
     memset(BGRA,0,4096);
     p=&BGRA[0]; //reset the pointer
     //read each byte an decode it
     for(x=0;x<128;x++) //for a singe byte in transMap its going to be 8 bytes in BGRA buffer=1024 bytes=32*32
     {
       index=7;//just a counter
       DecodeT(p,transMap[x],index);
       p+=8;
     }
     //now we have the transparent values,loop to all main buffer bytes and replace the color
     index=0; //used to skip 3 RGB values
     t=&buffer[54]; //reset pointer
     for(x=0;x<AllPixels;x++)
     {
         if(BGRA[x]==1) //transparent color,usually all transparent colors are set to palette 0,but Other Editors can set to a diff palette index and signed as trasnparent the result is diff
         {                   
           t[(x*3)]=(ColorToRemove>>16)&0xFF;
           t[(x*3)+1]=(ColorToRemove>>8)&0xFF;
           t[(x*3)+2]=(ColorToRemove&0xFF);
         }                 
     }
}

///////////////////////////////////// RGB TO BGRA ////////////////////////////

void RGBtoBGRA(UCHAR *buff) //&buff[bFile->bfOffBits]=&buff[54];
{
    memset(BGRA,0,4096); 
    DWORD x; 
    DWORD bgraRowBytes=32*4; 
    UCHAR **rp; 
    DWORD imageBytes = 32*3*32;//32pixels * 3rgbBytes * 32Height
    
    UCHAR *t=&BGRA[0];
    UCHAR *p=buff;
    //convert RGB to BGRA
    for(x=0;x<imageBytes;x+=3) //for each RGB value
    {
         *t++=p[x+2];
         *t++=p[x+1];
         *t++=p[x]; 
         *t++=0;             
    
    }
    rp = &pBGRA[0];
    p = &BGRA[4096];

	for(x=0;x<32;x++)
    {
	    p -= bgraRowBytes;
	    *rp++ = p;
	}
}   

//////////////////////////////////////////////////////////////////////

UCHAR *GetPixelEntry()
{
     return (&buffer[54]); 
}


void SaveTO(USHORT xx,USHORT yy,UCHAR *name,USHORT type)
{
    RGBtoBGRA(&buffer[54]);
    SaveImage(pBGRA,xx,yy,name,type);
}

UCHAR ReadDataFile(UCHAR *name)
{
    HANDLE fd;
    DWORD result=0;
    memset(buffer,0,3126);
    fd = CreateFile(name, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
    if(fd == INVALID_HANDLE_VALUE)
    {
       MessageBox(hWnd,name,"#Error: Can't Open File",MB_OK |MB_ICONERROR);
      return NO;
    }
    bufferLen = GetFileSize(fd,NULL);
    if(bufferLen == 0xFFFFFFFF)
    {
        MessageBox(hWnd,name,"#Error: FileSize",MB_OK |MB_ICONERROR);        
        CloseHandle(fd);
        return NO;
    }
    if((bufferLen != 3126) && (bufferLen != 2238) )
    {
        MessageBox(hWnd,"Drop Only Bmp/Icon/Cursor with\n format 32x32 24Bit and not Compessed.\n\n(Bmp FileSize is 3126 bytes)\n(Icon/Cursors FileSize is 2238 bytes)","#Info",MB_OK |MB_ICONINFORMATION);        
        CloseHandle(fd);
        return NO;
    }
    
    if(ReadFile(fd, buffer, bufferLen, &result, NULL) == FALSE)
    {
        MessageBox(hWnd,name,"#Error: Can't Read File",MB_OK |MB_ICONERROR);        
        CloseHandle(fd);
        return NO;
    }
    if(result != bufferLen)
    {
       MessageBox(hWnd,name,"#Error: Reading File",MB_OK |MB_ICONERROR);        
        CloseHandle(fd);
        return NO;
    }
    CloseHandle(fd);  
    if(bufferLen == 2238) LoadIconCursor(); 
  return YES;
}

