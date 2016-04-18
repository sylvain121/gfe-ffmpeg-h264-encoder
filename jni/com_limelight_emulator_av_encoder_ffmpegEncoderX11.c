#include <iostream>
#include <stdlib.h>
#include <stdbool.h>

#include "jni.h"
#include "utils.h"
#include "x11grab.h"
#include "encoder_ffmpeg.h"
#include "com_limelight_emulator_av_encoder_ffmpegEncoderX11.h"


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


JavaVM * g_vm;
jobject g_obj;
jmethodID g_mid;

JNIEXPORT void JNICALL com_limelight_emulator_av_encoder_ffmpegEncoderX11(JNIEnv *env, jobject obj)
{
    env->GetJavaVM(&g_vm);
    std::cout << "Starting native stream" << std::endl;
    /* get stream callback  method*/

    g_obj = env->NewGlobalRef(obj);
    jclass g_clazz = env->GetObjectClass(g_obj);
    if(g_clazz == NULL) {
     std::cout << "Failed to find class" << std::endl;
    }
    g_mid = env->GetMethodID(g_clazz, "streamByte", "([B)V");
    if(g_mid == NULL) {
        std::cout << "Unable to get method ref" << std::endl;
    }

}

JNIEXPORT void JNICALL Java_com_limelight_emulator_av_encoder_ffmpegEncoderX11_startStream(JNIEnv *env, jobject obj, jint frameWidth, jint frameHeight, jint videoFps)
{
    int desktopWidth, desktopHeight, desktopDepths;
    bool running = true;
    XImage *image = NULL;

    x11_init(image, &desktopWidth, &desktopHeight, &desktopDepths);
    encoder_init(&desktopWidth, &desktopHeight,&frameWidth, &frameHeight);

    while(running) {
            x11_getNextFrame(image, &desktopWidth, &desktopHeight);
            encoder_encodeFrame(image, &frameHeight, &frameHeight);
    }
}

JNIEXPORT void JNICALL Java_com_limelight_emulator_av_encoder_ffmpegEncoderX11_requestIdrFrame(JNIEnv *env, jobject obj)

{


}




void streamBytes(uint8_t *data, int size)
{

	JNIEnv * g_env;
    	// double check it's all ok
    	int getEnvStat = g_vm->GetEnv((void **)&g_env, JNI_VERSION_1_6);
    	if (getEnvStat == JNI_EDETACHED) {
    		std::cout << "GetEnv: not attached" << std::endl;
    		if (g_vm->AttachCurrentThread((void **) &g_env, NULL) != 0) {
    			std::cout << "Failed to attach" << std::endl;
    		}
    	} else if (getEnvStat == JNI_OK) {
    		//
    	} else if (getEnvStat == JNI_EVERSION) {
    		std::cout << "GetEnv: version not supported" << std::endl;
    	}

    	    jbyteArray packetBuffer = g_env->NewByteArray(size);
    	    g_env->SetByteArrayRegion(packetBuffer, 0, size, (jbyte*) data);
        	g_env->CallVoidMethod(g_obj, g_mid, packetBuffer);

    	if (g_env->ExceptionCheck()) {
    		g_env->ExceptionDescribe();
    	}

    	g_vm->DetachCurrentThread();

}




