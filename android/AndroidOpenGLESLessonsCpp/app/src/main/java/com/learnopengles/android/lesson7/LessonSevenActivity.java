package com.learnopengles.android.lesson7;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.util.DisplayMetrics;
import android.view.View;
import android.widget.Button;

import com.learnopengles.android.cpp.R;


public class LessonSevenActivity extends Activity {

    private LessonSevenGLSurfaceView mGlSurfaceView;
    private Action mRender;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.lesson_seven);

        mGlSurfaceView = (LessonSevenGLSurfaceView) findViewById(R.id.gl_surface_view);

        // Check if the system support OpenGL ES 2.0
        final ActivityManager activityManager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        final ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo();
        final boolean supportsEs2 = configurationInfo.reqGlEsVersion >= 0x20000;

        if (supportsEs2) {
            // Request an OpenGL ES 2.0 compatible context
            mGlSurfaceView.setEGLContextClientVersion(2);

            final DisplayMetrics displayMetrics = new DisplayMetrics();
            getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);

            // Set the renderer to out demo renderer, define now
            mRender = new LessonSevenNativeRenderer(this, mGlSurfaceView);
            mRender.init();
            mGlSurfaceView.setRenderer((GLSurfaceView.Renderer) mRender, displayMetrics.density);
        } else {
            return;
        }

        findViewById(R.id.button_decrease_num_cubes).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                decreaseCubeCount();
            }
        });

        findViewById(R.id.button_increase_num_cubes).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                increaseCubeCount();
            }
        });

        findViewById(R.id.button_switch_VBOs).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                toggleVBOs();
            }
        });

        findViewById(R.id.button_switch_stride).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                toggleStride();
            }
        });
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

    private void decreaseCubeCount() {
        mGlSurfaceView.queueEvent(new Runnable() {
            @Override
            public void run() {
                mRender.decreaseCubeCount();
            }
        });
    }

    private void increaseCubeCount() {
        mGlSurfaceView.queueEvent(new Runnable() {
            @Override
            public void run() {
                mRender.increaseCubeCount();
            }
        });
    }

    private void toggleVBOs() {
        mGlSurfaceView.queueEvent(new Runnable() {
            @Override
            public void run() {
                mRender.toggleVBOs();
            }
        });
    }

    protected void toggleStride() {
        mGlSurfaceView.queueEvent(new Runnable() {
            @Override
            public void run() {
                mRender.toggleStride();
            }
        });
    }

    public void updateVboStatus(final boolean usingVbos) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (usingVbos) {
                    ((Button) findViewById(R.id.button_switch_VBOs)).setText(R.string.lesson_seven_using_VBOs);
                } else {
                    ((Button) findViewById(R.id.button_switch_VBOs)).setText(R.string.lesson_seven_not_using_VBOs);
                }
            }
        });
    }

    public void updateStrideStatus(final boolean useStride) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (useStride) {
                    ((Button) findViewById(R.id.button_switch_stride)).setText(R.string.lesson_seven_using_stride);
                } else {
                    ((Button) findViewById(R.id.button_switch_stride)).setText(R.string.lesson_seven_not_using_stride);
                }
            }
        });
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mRender.destroy();
    }
}
