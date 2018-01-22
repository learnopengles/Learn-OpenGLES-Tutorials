package com.learnopengles.android.lesson5;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.MotionEvent;

public class LessonFiveActivity extends Activity {

    /**
     * Hold a reference to our GLSurfaceView
     */
    private LessonFiveGLSurfaceView mGLSurfaceView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mGLSurfaceView = new LessonFiveGLSurfaceView(this);

        // Check if the system support OpenGL ES 2.0
        final ActivityManager activityManager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        final ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo();
        final boolean supportsEs2 = configurationInfo.reqGlEsVersion >= 0x20000;

        if (supportsEs2) {
            // Request an OpenGL ES 2.0 compatible context.
            mGLSurfaceView.setEGLContextClientVersion(2);

            // Set the renderer to out demo renderer, define below
            mGLSurfaceView.setRenderer(new LessonFiveNativeRenderer(this));
        } else {
            // This is where you could create an OpenGL ES 1.x compatible
            // renderer if you wanted to support both ES 1 and ES 2
            return;
        }
        setContentView(mGLSurfaceView);
    }

    @Override
    protected void onResume() {
        super.onResume();
        mGLSurfaceView.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
        mGLSurfaceView.onPause();
    }

    class LessonFiveGLSurfaceView extends GLSurfaceView {
        private BlendingMode mRenderer;

        public LessonFiveGLSurfaceView(Context context) {
            super(context);
        }

        @Override
        public boolean onTouchEvent(MotionEvent event) {
            if (event != null) {
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    if (mRenderer != null) {
                        // Ensure we call switchMode() on the OpenGL thread.
                        // queueEvent() is a method of GLSurfaceView that will do this for us.
                        queueEvent(new Runnable() {
                            @Override
                            public void run() {
                                mRenderer.switchMode();
                            }
                        });

                        return true;
                    }
                }
            }

            return super.onTouchEvent(event);
        }

        // Hides superclass method.
        public void setRenderer(Renderer renderer) {
            if (renderer instanceof BlendingMode) {
                mRenderer = (BlendingMode) renderer;
            }
            super.setRenderer(renderer);
        }
    }
}
