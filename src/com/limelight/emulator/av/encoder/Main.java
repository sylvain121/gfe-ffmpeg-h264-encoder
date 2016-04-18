package com.limelight.emulator.av.encoder;

public class Main {

    public static void main(String[] args) {
        ffmpegEncoderX11 encoder = new ffmpegEncoderX11();
        encoder.startStream(800,600,30);
    }
}
