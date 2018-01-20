package com.learnopengles.android.lesson4;

import android.app.Activity;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class LessonFourNativeRenderer implements GLSurfaceView.Renderer {

    static {
        System.loadLibrary("lesson-lib");
    }

    private Activity mActivity;

    public LessonFourNativeRenderer(Activity activity) {
        mActivity = activity;
    }


    public static native void nativeSurfaceCreate(AssetManager assetManager);

    public static native void nativeSurfaceChange(int width, int height);

    public static native void nativeDrawFrame();

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        AssetManager assetManager = mActivity.getAssets();
        nativeSurfaceCreate(assetManager);
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        nativeSurfaceChange(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        nativeDrawFrame();
    }
}
