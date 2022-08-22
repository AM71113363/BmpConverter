<p align="center">
<img align="center" width="360" height="247" src="https://raw.githubusercontent.com/AM71113363/BmpConverter/master/info.png">
</p>

# Bmp Converter -> BMP ICO CURSOR with Transparent Color
-----

## How To Use It?<br>
* DragDrop bmp/ico/cursor file(32x32 24Bit not Compressed) or Click MENU->File->New<br>
* Press "Ctrl" key to select a color in the current window or Desktop,,AnyWhere.<br>
* Click "Set" to set the color in ColorTab as Transparent Color.<br>
* MENU->File->Settings->(Set Transparent White/Black) by default is BLACK,you can use WHITE and see the results.<br>
* MENU->Mode->Draw it enables drawing in the image,click to draw 1 pixel or drag for more.<br>
* MENU->Mode->Set_Cursor_HotSpot when clicked it will show the Cursor Hotspot,Click anyWhere in the image to change it.<br>
* MENU->Display->Show_Mouse_Coords it will show always the Mouse Coords(like when using MENU->Mode->Set_Cursor_HotSpot).<br>
* When loading an ICO/CURSOR The color in TransparentColorTab will be used.(TIP: set always a transparent color that is not in the image) .<br>
* if the colors used in the image are more than 256 the Cursor/Icon will not be saved, reduce them and try again.<br>
** Use Mouse Right Click to turn back the last draw pixel.<br>

## Build.
_for beginners_ <br>
1.Download [DevCpp_v4.9.9.*](http://www.bloodshed.net/) and install it.<br>
2.Run build.bat (it works only with Dev-C++ 4.9.9.*).<br>
_others_ <br>
You already know how to rebuilt it. ^_^<br>
<br>

# NOTE
* By Default the ICON/CURSOR transparent color is black,and before dragdrop the Color in TransparentColorTab need to be changed,otherwise 
if the image contains black pixels after save those pixels will be transparent unless you set another Transparent color.<br>
* Set Transparent White/Black has nothing to do with the Transparent color int TransparentColorTab,since the palette entry 0 is black and 1 is white,
the "Set Transparent White/Black" it changes the transparent color palette index.<br>
* Bmp filesize must be 3126 bytes.<br>
* Ico/Cursor filesize must be 2238 bytes<br>
**LOL** you can dragdrop any file with filesize 3126/2238 bytes,and the App will load it,ofcourse the image its going to be based on file bytes.<br>

