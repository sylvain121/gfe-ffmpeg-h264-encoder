
#include <stdio.h>
#include "utils.h"
//X11GRAB IMPORT

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <sys/ipc.h>
#include <X11/X.h>
#include <X11/Xlibint.h>
#include <X11/Xproto.h>
#include <X11/Xutil.h>
#include <sys/shm.h>
#include <X11/extensions/shape.h>
#include <X11/extensions/XShm.h>
#include <X11/extensions/Xfixes.h>
#include "x11grab.h"



//X11GRAB vars
static int screenNumber;
static char *displayname = ":0";
static Display *display = NULL;
static Window rootWindow;
static Screen *screen = NULL;
XColor color;

static XShmSegmentInfo __xshminfo;


int x11_init(XImage *image, int *desktopWidth, int *desktopHeight, int *desktopDepth)
{
  // init
  int ignore = 0;
  bzero(&__xshminfo, sizeof(__xshminfo));

  // open display
  if((display = XOpenDisplay(NULL)) == NULL) {
    printf("cannot open display \"%s\"\n", displayname ? displayname : "DEFAULT");
    return -1;
  }

  // check MIT extension
  if(XQueryExtension(display, "MIT-SHM", &ignore, &ignore, &ignore) ) {
    int major, minor;
    Bool pixmaps;
    if(XShmQueryVersion(display, &major, &minor, &pixmaps) == True) {
      printf("XShm extention version %d.%d %s shared pixmaps\n",
          major, minor, (pixmaps==True) ? "with" : "without");
    } else {
      printf("XShm extension not supported.\n");
    }
  }
  // get default screen
  screenNumber = XDefaultScreen(display);
  if((screen = XScreenOfDisplay(display, screenNumber)) == NULL) {
    printf("cannot obtain screen #%d\n", screenNumber);
  }
  // get screen hight, width, depth
  *desktopWidth = XDisplayWidth(display, screenNumber);
  *desktopHeight = XDisplayHeight(display, screenNumber);
  *desktopDepth = XDisplayPlanes(display, screenNumber);
  printf("X-Window-init: dimension: %dx%dx%d @ %d/%d\n",
      *desktopWidth, *desktopHeight, *desktopDepth,
      screenNumber, XScreenCount(display));
  //create image context
  if((image = XShmCreateImage(display,
      XDefaultVisual(display, screenNumber),
      *desktopDepth, ZPixmap, NULL, &__xshminfo,
      *desktopWidth, *desktopHeight)) == NULL) {
    printf("XShmCreateImage failed.\n");
  }


  //get shm info
  if((__xshminfo.shmid = shmget(IPC_PRIVATE,
        image->bytes_per_line*image->height,
        IPC_CREAT | 0777)) < 0) {
    printf("shmget error");
  }

    //
  __xshminfo.shmaddr = image->data = (char*) shmat(__xshminfo.shmid, 0, 0);
  __xshminfo.readOnly = False;
  if(XShmAttach(display, &__xshminfo) == 0) {
    printf("XShmAttach failed.\n");
  }
  //
  rootWindow = XRootWindow(display, screenNumber);
}


void x11_getNextFrame(XImage *image, int *desktopWidth, int *desktopHeight) {

        // get image from X11

        if(XShmGetImage(display, rootWindow, image, 0, 0, XAllPlanes()) == 0) {
          // image->data RGBA
          printf("FATAL: XShmGetImage failed.\n");
          exit(-1);
        }

}
