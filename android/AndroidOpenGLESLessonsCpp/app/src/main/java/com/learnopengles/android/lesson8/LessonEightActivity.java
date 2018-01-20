package com.learnopengles.android.lesson8;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.util.DisplayMetrics;

public class LessonEightActivity extends Activity {

    private LessonEightGLSurfaceView mGlSurfaceView;
    private Action mRender;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mGlSurfaceView = new LessonEightGLSurfaceView(this);
        setContentView(mGlSurfaceView);
        // Check if the system support OpenGL ES 2.0
        final ActivityManager activityManager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        final ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo();
        final boolean supportsEs2 = configurationInfo.reqGlEsVersion >= 0x20000;

        if (supportsEs2) {
            // Request an OpenGL ES 2.0 compatible context.
            mGlSurfaceView.setEGLContextClientVersion(2);

            final DisplayMetrics displayMetrics = new DisplayMetrics();
            getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);

            // Set the renderer to our demo renderer, defined below.
            mRender = (Action) new LessonEightNativeRenderer(this);
            mGlSurfaceView.setRenderer((GLSurfaceView.Renderer) mRender, displayMetrics.density);
        } else {
            // This is where you could create an OpenGL ES 1.x compatible
            // renderer if you wanted to support both ES 1 and ES 2
            return;
        }

    }

    @Override
    protected void onResume() {
        // The activity must call the GL surface view's onResume() on activity
        // onResume().
        super.onResume();
        mGlSurfaceView.onResume();
    }

    @Override
    protected void onPause() {
        // The activity must call the GL surface view's onPause() on activity
        // onPause().
        super.onPause();
        mGlSurfaceView.onPause();
    }

}
