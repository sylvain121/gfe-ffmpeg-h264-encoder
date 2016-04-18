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

#ifdef __cplusplus
extern "C" {

#endif
void encoder_init(int *desktopWidth,int *desktopHeight,int *frameWidth,int *frameHeight);
void encoder_encodeFrame(XImage *image, int *w, int *h);
#ifdef __cplusplus
}
#endif
