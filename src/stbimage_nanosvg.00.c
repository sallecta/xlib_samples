/* 
http://murga-linux.com/puppy/viewtopic.php?p=959534

http://murga-linux.com/puppy/profile.php?mode=viewprofile&u=14229
goingnuts
Xlib version of technosaurus xcb based image-viewer two post back:

file set:
    stbimage_nanosvg.00.c            - main file
    stbimage_nanosvg.stb_image.h      - stb image single header library
    stbimage_nanosvg.nanosvg.h			- memononen/nanosvg Simple stupid SVG parser 
    stbimage_nanosvg.nanosvgrast.h	- memononen/nanosvg rasterizer

compile:
    gcc stbimage_nanosvg.00.c -o stbimage_nanosvg -L/usr/lib -L/usr/X11/lib -lX11 -lm
 */

/* Original notes*/

/*
Inspired by
xputjpeg by Gleicon S. Moraes - gleicon@terra.com.br
    from: https://searchcode.com/codesearch/raw/107558322/
unamed example by technosaurus
    from http://murga-linux.com/puppy/viewtopic.php?t=89272&start=390&sid=1f696000b4032a89e59e66135ce527b8

fun with stb_image...
goingnuts July 2017
compile: gcc xputimage.c -o xputimage -L/usr/lib -L/usr/X11/lib -lX11 -lm
*/

/* local versions of libraries file names */
#define stb_image_h "stbimage_nanosvg.stb_image.h"
#define nanosvg_h "stbimage_nanosvg.nanosvg.h"
#define nanosvgrast_h "stbimage_nanosvg.nanosvgrast.h"
/* end local versions of libraries file names */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/xpm.h>
#include <X11/extensions/shape.h>
#include <math.h>

#ifndef sinf
float sinf (float x) { return (float) sin( (double)x ); }
#endif
#ifndef cosf
float cosf (float x) { return (float) cos( (double)x ); }
#endif
#ifndef tanf
float tanf  (float x){return (float) tan ( (double)x );}
#endif
#ifndef atan2f
float atan2f(float x, float y){return (float) atan2( (double)x, (double)y );}
#endif
#ifndef fmodf
float fmodf (float x, float y){return (float) fmod ( (double)x, (double)y );}
#endif
#ifndef acosf
float acosf (float x){return (float) acos( (double)x );}
#endif

#define STBI_NO_HDR
#define STBI_NO_LINEAR
#define STB_IMAGE_IMPLEMENTATION
#include stb_image_h
#define NANOSVG_IMPLEMENTATION
#include nanosvg_h
#define NANOSVGRAST_IMPLEMENTATION
#include nanosvgrast_h

int main (int argc, char **argv) {

   int            i, len, w, h, n, bpl, depth, screen;
   Pixel         white, black;
   Window         Root, iconwin, win;
   XEvent          x_event;
   XImage         *ximage;
   Display         *dpy;
   GC            gc;
   XGCValues      gcv;
   char         *data;
   unsigned       *dp;
   NSVGimage *shapes = NULL;
   NSVGrasterizer *rast = NULL;
   
   if (argc < 2) {
	printf ("Use: %s path.to.image\n", argv[0]);
argv[1]="resources/nkscp1.0-graphicdesignstudio.svg";
      //exit(1);
   }
   
   if ((data = stbi_load(argv[1], &w, &h, &n, 4)))
      ;
   else if ((shapes = nsvgParseFromFile(argv[1], "px", 96.0f))) {
      w = (int)shapes->width;
      h = (int)shapes->height;
      rast = nsvgCreateRasterizer();
      data = malloc(w*h*4);
      nsvgRasterize(rast, shapes, 0,0,1, data, w, h, w*4);
   } else {
         printf("Unable to load image: %s\n", argv[0]);
      return -1;
   }
      
    for(i=0, len=w*h, dp=(unsigned *)data; i<len; i++) //rgba to bgra
      dp[i]=dp[i]&0xff00ff00|((dp[i]>>16)&0xFF)|((dp[i]<<16)&0xFF0000);
     
      if (!(dpy = XOpenDisplay(NULL)))
      printf("can't open X display\n");
   
   screen = DefaultScreen(dpy);
   Root = RootWindow (dpy, screen);
   white = WhitePixel (dpy, screen);
   black = BlackPixel (dpy, screen);

   win = XCreateSimpleWindow (dpy, Root, 0, 0, w, h, 0, black, white);

   iconwin = XCreateSimpleWindow (dpy,   win, 0, 0, w, h, 0, black, white);

   XSelectInput(dpy, win, ExposureMask | KeyPressMask);
   XMapWindow (dpy, win);
   gc = XCreateGC (dpy, win, 0, &gcv);

   depth = DefaultDepth(dpy, screen);

   switch(depth) {
   
      case 24:    
         bpl=4; break;
      case 16:
      case 15:    
         bpl=2; break;
      default:
         bpl=1; break;
   }

   ximage = XCreateImage (dpy,
           CopyFromParent, depth,
           ZPixmap, 0,
           data,
           w, h,
           bpl*8, bpl * w);

   XFlush(dpy);

   while (1) {
      XNextEvent(dpy, &x_event);
         switch (x_event.type) {
            case Expose:
               XPutImage (dpy, win, gc, ximage, 0,0,0,0, w, h);
                XFlush (dpy);     
               break;
            case KeyPress:   goto end;
            default: break;   
         }
   }

end:
   free (data);
   close(ConnectionNumber(dpy));
   return 0;
} 
