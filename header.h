//------------------- SAVE -> ICON CURSOR HEADER ------------------------------
typedef struct PACKED _ICONHEADER
{
        USHORT idReserved;
        USHORT idType;
        USHORT idCount;
        UCHAR  idWidth;
        UCHAR  idHeight;
        UCHAR  idColors;
        UCHAR  idUnused;
        USHORT idhotSpotX;
        USHORT idhotSpotY;
        DWORD  idBytes;
        DWORD  idOffset;
    //BITMAPINFOHEADER
        DWORD  biSize;
        DWORD  biWidth;
        DWORD  biHeight;
        USHORT biPlanes;
        USHORT biBitCount;
        DWORD  biCompression;
        DWORD  biSizeImage;
        DWORD  biXPelsPerMeter;
        DWORD  biYPelsPerMeter;
        DWORD  biClrUsed;
        DWORD  biClrImportant;
}ICONHEADER;

static ICONHEADER icoHeader={
  0,     //idReserved
  1,     //idType 1 = ICON 2=Cursor
  1,     //idCount
  32,    //idWidth
  32,    //idHeight
  0,     //idColors
  0,     //idUnused
  0,     //idhotSpotX
  0,     //idhotSpotY
  0x8A8, //idBytes
  0x16,   //idOffset
//BITMAPINFOHEADER
  40,    //biSize; 
  32,    //biWidth; 
  64,    //biHeight; 
  1,     //biPlanes; 
  8,     //biBitCount 
  0,     //biCompression; 
  0x480, //biSizeImage; 
  0,     //biXPelsPerMeter; 
  0,     //biYPelsPerMeter; 
  0,     //biClrUsed; 
  0      //biClrImportant; 
}; 
//----------------------- BMP INFO HEADER ------------------------------------
typedef struct PACKED _BMPiHEADER
{
        DWORD  biSize;
        DWORD  biWidth;
        DWORD  biHeight;
        USHORT biPlanes;
        USHORT biBitCount;
        DWORD  biCompression;
        DWORD  biSizeImage;
        DWORD  biXPelsPerMeter;
        DWORD  biYPelsPerMeter;
        DWORD  biClrUsed;
        DWORD  biClrImportant;
        UCHAR bmiColors[1];
}BMPiHEADER;

static BMPiHEADER DeafultHeader={
  40,    //biSize; 
  32,    //biWidth; 
  32,    //biHeight; 
  1,     //biPlanes; 
  24,     //biBitCount 
  0,     //biCompression; 
  0xC00,  //biSizeImage; 
  0,     //biXPelsPerMeter; 
  0,     //biYPelsPerMeter; 
  0,     //biClrUsed; 
  0,      //biClrImportant; 
  0 //bmiColors
};

//--------------------- SAVE -> BMP HEADER ------------------------
typedef struct PACKED _BMPHEADER {
	USHORT	idType;
	DWORD	idSize;
	USHORT	idReserved1;
	USHORT	idReserved2;
	DWORD	idOffBits;
    //BITMAPINFOHEADER
    DWORD  biSize;
    DWORD  biWidth;
    DWORD  biHeight;
    USHORT biPlanes;
    USHORT biBitCount;
    DWORD  biCompression;
    DWORD  biSizeImage;
    DWORD  biXPelsPerMeter;
    DWORD  biYPelsPerMeter;
    DWORD  biClrUsed;
    DWORD  biClrImportant;
}BMPHEADER;

static BMPHEADER bmpheader={
    0x4D42,  //idType; 'BM'
	0xC36,   //idSize;
	0,       //idReserved1;
	0,	     //idReserved2;
	54,      //idOffBits;
    //BITMAPINFOHEADER
    40,      //biSize; 
    32,      //biWidth; 
    32,      //biHeight; 
    1,       //biPlanes; 
    24,      //biBitCount 
    0,       //biCompression; 
    0xC00,   //biSizeImage; 
    0,       //biXPelsPerMeter; 
    0,       //biYPelsPerMeter; 
    0,       //biClrUsed; 
    0,       //biClrImportant;
};
