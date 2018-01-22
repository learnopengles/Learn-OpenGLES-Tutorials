package com.learnopengles.android.lesson5;

import android.app.Activity;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class LessonFiveNativeRenderer implements GLSurfaceView.Renderer, BlendingMode {

    private Activity mActivity;

    public LessonFiveNativeRenderer(Activity activity) {
        mActivity = activity;
    }

    static {
        System.loadLibrary("lesson-lib");
    }

    public static native void nativeSurfaceCreate(AssetManager assetManager);

    public static native void nativeSurfaceChange(int width, int height);

    public static native void nativeDrawFrame();

    public static native void nativeSwitchMode();


    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        nativeSurfaceCreate(mActivity.getAssets());
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        nativeSurfaceChange(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        nativeDrawFrame();
    }

    @Override
    public void switchMode() {
        nativeSwitchMode();
    }
}
