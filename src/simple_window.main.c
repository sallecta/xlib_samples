/*
 * simple-window.c - demonstrate creation of a simple window.
 */

#include <X11/Xlib.h>

#include <stdio.h>
#include <stdlib.h>		/* getenv(), etc. */
#include <unistd.h>		/* sleep(), etc.  */

int main(int argc, char* argv[])
{
  Display* display;		/* pointer to X Display structure.           */
  int screen_num;		/* number of screen to place the window on.  */
  Window win;			/* pointer to the newly created window.      */
  unsigned int display_width,
               display_height;	/* height and width of the X display.        */
  unsigned int width, height;	/* height and width for the new window.      */
  unsigned int win_x, win_y;	/* location of the window's top-left corner. */
  unsigned int win_border_width; /* width of window's border.                */
  char *display_name = getenv("DISPLAY");  /* address of the X display.      */

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
  width = (display_width / 3);
  height = (display_height / 3);

  /* the window should be placed at the top-left corner of the screen. */
  win_x = 0;
  win_y = 0;

  /* the window's border shall be 2 pixels wide. */
  win_border_width = 2;

  /* create a simple window, as a direct child of the screen's   */
  /* root window. Use the screen's white color as the background */
  /* color of the window. Place the new window's top-left corner */
  /* at the given 'x,y' coordinates.                             */
  win = XCreateSimpleWindow(display, RootWindow(display, screen_num),
                            win_x, win_y, width, height, win_border_width,
                            BlackPixel(display, screen_num),
                            WhitePixel(display, screen_num));

  /* make the window actually appear on the screen. */
  XMapWindow(display, win);

  /* flush all pending requests to the X server, and wait until */
  /* they are processed by the X server.                        */
  XSync(display, False);

  /* make a delay for a short period. */
  sleep(4);

  /* close the connection to the X server. */
  XCloseDisplay(display);
  return(0);
}
