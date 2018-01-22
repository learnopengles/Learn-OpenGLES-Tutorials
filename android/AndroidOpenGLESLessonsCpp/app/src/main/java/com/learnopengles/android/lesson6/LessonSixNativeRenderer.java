package com.learnopengles.android.lesson6;

import android.app.Activity;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.support.annotation.RequiresApi;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

@RequiresApi(api = Build.VERSION_CODES.CUPCAKE)
public class LessonSixNativeRenderer implements GLSurfaceView.Renderer, Action {

    private Activity mActivity;

    public LessonSixNativeRenderer(Activity activity) {
        mActivity = activity;
    }

    static {
        System.loadLibrary("lesson-lib");
    }

    public static native void nativeSurfaceCreate(AssetManager assetManager);

    public static native void nativeSurfaceChange(int width, int height);

    public static native void nativeDrawFrame();

    public static native void nativeSetDelta(float x, float y);

    public static native void nativeSetMinFilter(int filter);

    public static native void nativeSetMagFilter(int filter);

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
    public void setMinFilter(int filter) {
        nativeSetMinFilter(filter);
    }

    @Override
    public void setMagFilter(int filter) {
        nativeSetMagFilter(filter);
    }

    @Override
    public void setDelta(float deltaX, float deltaY) {
        nativeSetDelta(deltaX, deltaY);
    }
}
