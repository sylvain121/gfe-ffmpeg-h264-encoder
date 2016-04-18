package com.limelight.emulator.av.encoder;

import java.io.IOException;
import java.io.OutputStream;

/**
 * Created by sylvain on 18/03/16.
 */
public class ffmpegEncoderX11 {


    public ffmpegEncoderX11() {

    }

    public native void startStream(int width, int height, int fps);
    public native void requestIdrFrame();


    public void streamByte(byte[] videoStream) {
        System.out.println("getting frame");
    }
}
