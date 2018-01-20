package com.learnopengles.android.lesson7;

import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.support.annotation.RequiresApi;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class LessonSevenNativeRenderer implements GLSurfaceView.Renderer, Action {

    private LessonSevenActivity mActivity;
    private GLSurfaceView mGlSurfaceView;

    public LessonSevenNativeRenderer(LessonSevenActivity activity, GLSurfaceView glSurfaceView) {
        mActivity = activity;
        mGlSurfaceView = glSurfaceView;
    }

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
    public void init() {
        nativeInit();
    }

    @Override
    public void destroy() {
        nativeDestroy();
    }

    @Override
    public void setDelta(float deltaX, float deltaY) {
        nativeSetDelta(deltaX, deltaY);
    }

    @Override
    public void decreaseCubeCount() {
        nativeDecreaseCubeCount();
    }

    @Override
    public void increaseCubeCount() {
        nativeIncreaseCubeCount();
    }

    @Override
    public void toggleVBOs() {
        nativeToggleVBOs();
    }

    @Override
    public void toggleStride() {
        nativeToggleStride();
    }

    {
        System.loadLibrary("lesson-lib");
    }

    public void updateVboStatus(boolean useVbos) {
        mActivity.updateVboStatus(useVbos);
    }

    public void updateStrideStatus(boolean useStride) {
        mActivity.updateStrideStatus(useStride);
    }

    public native void nativeInit();

    public native void nativeDestroy();

    public native void nativeSurfaceCreate(AssetManager assetManager);

    public native void nativeSurfaceChange(int width, int height);

    public native void nativeDrawFrame();

    public native void nativeSetDelta(float x, float y);

    public native void nativeDecreaseCubeCount();

    public native void nativeIncreaseCubeCount();

    public native void nativeToggleVBOs();

    public native void nativeToggleStride();
}
