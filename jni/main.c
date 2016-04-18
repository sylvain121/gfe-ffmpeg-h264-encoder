#include <iostream>
#include <stdlib.h>
#include <stdbool.h>

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

#include "utils.h"
#include "x11grab.h"
#include "encoder_ffmpeg.h"




int main(int argc, char *argv[]) {

    int frameWidth = 800;
    int frameHeight = 600;
    int videoFps = 30;
    XImage *image = NULL;

    int desktopWidth, desktopHeight, desktopDepths;
    bool running = true;

    x11_init(image, &desktopWidth, &desktopHeight, &desktopDepths);
    encoder_init(&desktopWidth, &desktopHeight,&frameWidth, &frameHeight);

    while(running) {
            x11_getNextFrame(image, &desktopWidth, &desktopHeight);
            encoder_encodeFrame(image, &frameHeight, &frameHeight);
    }
}

