/*
 * window-operations.c - demonstrate usage of window operation functions -
 *                       moving windows, resizing them, etc.
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <stdio.h>
#include <stdlib.h>		/* getenv(), etc. */
#include <unistd.h>		/* sleep(), etc.  */

/*
 * function: create_simple_window. Creates a window with a white background
 *           in the given size.
 * input:    display, size of the window (in pixels), and location of the window
 *           (in pixels).
 * output:   the window's ID.
 * notes:    window is created with a black border, 2 pixels wide.
 *           the window is automatically mapped after its creation.
 */
Window
create_simple_window(Display* display, int width, int height, int x, int y)
{
  int screen_num = DefaultScreen(display);
  int win_border_width = 2;
  Window win;

  /* create a simple window, as a direct child of the screen's */
  /* root window. Use the screen's black and white colors as   */
  /* the foreground and background colors of the window,       */
  /* respectively. Place the new window's top-left corner at   */
  /* the given 'x,y' coordinates.                              */
  win = XCreateSimpleWindow(display, RootWindow(display, screen_num),
                            x, y, width, height, win_border_width,
                            BlackPixel(display, screen_num),
                            WhitePixel(display, screen_num));

  /* make the window actually appear on the screen. */
  XMapWindow(display, win);

  /* flush all pending requests to the X server. */
  XFlush(display);

  return win;
}

GC
create_gc(Display* display, Window win, int reverse_video)
{
  GC gc;				/* handle of newly created GC.  */
  unsigned long valuemask = 0;		/* which values in 'values' to  */
					/* check when creating the GC.  */
  XGCValues values;			/* initial values for the GC.   */
  unsigned int line_width = 2;		/* line width for the GC.       */
  int line_style = LineSolid;		/* style for lines drawing and  */
  int cap_style = CapButt;		/* style of the line's edje and */
  int join_style = JoinBevel;		/*  joined lines.		*/
  int screen_num = DefaultScreen(display);

  gc = XCreateGC(display, win, valuemask, &values);
  if (gc < 0) {
	fprintf(stderr, "XCreateGC: \n");
  }

  /* allocate foreground and background colors for this GC. */
  if (reverse_video) {
    XSetForeground(display, gc, WhitePixel(display, screen_num));
    XSetBackground(display, gc, BlackPixel(display, screen_num));
  }
  else {
    XSetForeground(display, gc, BlackPixel(display, screen_num));
    XSetBackground(display, gc, WhitePixel(display, screen_num));
  }

  /* define the style of lines that will be drawn using this GC. */
  XSetLineAttributes(display, gc,
                     line_width, line_style, cap_style, join_style);

  /* define the fill style for the GC. to be 'solid filling'. */
  XSetFillStyle(display, gc, FillSolid);

  return gc;
}

int main(int argc, char* argv[])
{
  Display* display;		/* pointer to X Display structure.           */
  int screen_num;		/* number of screen to place the window on.  */
  Window win;			/* pointer to the newly created window.      */
  unsigned int display_width,
               display_height;	/* height and width of the X display.        */
  unsigned int win_width,
	       win_height;	/* height and width for the new window.      */
  char *display_name = getenv("DISPLAY");  /* address of the X display.      */
  GC gc;			/* GC (graphics context) used for drawing    */
				/*  in our window.			     */

  /* open connection with the X server. */
  display = XOpenDisplay(display_name);
  if (display == NULL) {
    fprintf(stderr, "%s: cannot connect to X server '%s'\n",
            argv[0], display_name);
    exit(1);
  }

  /* get the geometry of the default screen for our display. */
  screen_num = DefaultScreen(display);
  display_width = DisplayWidth(display, screen_num);
  display_height = DisplayHeight(display, screen_num);

  /* make the new window occupy 1/9 of the screen's size. */
  win_width = (display_width / 3);
  win_height = (display_height / 3);
  printf("window width - '%d'; height - '%d'\n", win_width, win_height);

  /* create a simple window, as a direct child of the screen's   */
  /* root window. Use the screen's white color as the background */
  /* color of the window. Place the new window's top-left corner */
  /* at the given 'x,y' coordinates.                             */
  win = create_simple_window(display, win_width, win_height, 0, 0);

  /* allocate a new GC (graphics context) for drawing in the window. */
  gc = create_gc(display, win, 0);
  XFlush(display);

  sleep(3);

  /* example of resizing a window. */
  {
    int i;

    /* start shrinking our window in a loop. */
    for (i=0; i<40; i++) {
      win_width -= 3;
      win_height -= 3;
      XResizeWindow(display, win, win_width, win_height);
      XFlush(display);
      usleep(20000);
    }

    /* start shrinking our window in a loop. */
    for (i=0; i<40; i++) {
      win_width += 3;
      win_height += 3;
      XResizeWindow(display, win, win_width, win_height);
      XFlush(display);
      usleep(20000);
    }
  }

  sleep(1);

  /* example of moving a window. */
  {
    int i;
    XWindowAttributes win_attr;
    int x, y;
    int scr_x, scr_y;
    Window child_win;
    /* this variable will store the ID of the parent window of our window. */
    Window parent_win;

    /* first, get the current attributes of our window. */
    XGetWindowAttributes(display, win, &win_attr);

    x = win_attr.x;
    y = win_attr.y;

    /* next, find the parent window of our window.      */
    {
      /* this variable will store the ID of the root window of the screen    */
      /* our window is mapped on.                                            */
      Window root_win;
      /* this variable will store an array of IDs of the child windows of    */
      /* our window.                                                         */
      Window* child_windows;
      /* and this one will store the number of child windows our window has. */
      int num_child_windows;

      /* finally, make the query for the above values. */
      XQueryTree(display, win,
                 &root_win,
                 &parent_win,
                 &child_windows, &num_child_windows);

      /* we need to free the list of child IDs, as it was dynamically */
      /* allocated by the XQueryTree function.                        */
      XFree(child_windows);
    }

    /* next, translate the location coordinates to screen coordinates. */
    /* this is done using the root window as the destination window.   */
    /* this works since the root window always spans the entire screen */
    /* area, and thus has its top-left corner always at the top-left   */
    /* corner of the screen.                                           */
    XTranslateCoordinates(display,
			  parent_win, win_attr.root,
			  x, y,
			  &scr_x, &scr_y,
			  &child_win);

    /* start moving the window to the left. */
    for (i=0; i<40; i++) {
      scr_x -= 3;
      XMoveWindow(display, win, scr_x, scr_y);
      XFlush(display);
      usleep(20000);
    }

    /* start moving the window to down. */
    for (i=0; i<40; i++) {
      scr_y += 3;
      XMoveWindow(display, win, scr_x, scr_y);
      XFlush(display);
      usleep(20000);
    }

    /* start moving the window to the right. */
    for (i=0; i<40; i++) {
      scr_x += 3;
      XMoveWindow(display, win, scr_x, scr_y);
      XFlush(display);
      usleep(20000);
    }

    /* start moving the window up. */
    for (i=0; i<40; i++) {
      scr_y -= 3;
      XMoveWindow(display, win, scr_x, scr_y);
      XFlush(display);
      usleep(20000);
    }
  }

  sleep(1);

  /* example of iconifying and de-iconifying a window. */
  {
    /* iconify our window. */
    XIconifyWindow(display, win, DefaultScreen(display));
    XFlush(display);
    sleep(2);
    /* de-iconify our window. */
    XMapWindow(display, win);
    XFlush(display);
    sleep(2);
  }

  /* flush all pending requests to the X server. */
  XFlush(display);

  /* make a delay for a short period. */
  sleep(2);

  /* close the connection to the X server. */
  XCloseDisplay(display);
  return(0);
}
