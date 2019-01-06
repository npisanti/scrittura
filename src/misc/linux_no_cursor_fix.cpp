
// Nicola Pisanti, GPLv3 License, 2016

#include "linux_no_cursor_fix.h"

void np::linux_no_cursor_fix(){

#ifdef __ARM_NEON__
        ofHideCursor();
#else
        ofHideCursor();
        /*
        Display* display = ofGetX11Display();
        Window window = ofGetX11Window();

        Cursor invisibleCursor;
        Pixmap bitmapNoData;
        XColor black;
        static char noData[] = { 0,0,0,0,0,0,0,0 };
        black.red = black.green = black.blue = 0;

        bitmapNoData = XCreateBitmapFromData(display, window, noData, 8, 8);
        invisibleCursor = XCreatePixmapCursor(display, bitmapNoData, bitmapNoData, 
                                             &black, &black, 0, 0);
        XDefineCursor(display,window, invisibleCursor);
        XFreeCursor(display, invisibleCursor);
        */ 
#endif    
    
}
