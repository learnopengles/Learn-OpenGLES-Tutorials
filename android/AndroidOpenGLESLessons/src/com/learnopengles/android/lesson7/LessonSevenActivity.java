package com.learnopengles.android.lesson7;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

import com.learnopengles.android.R;

public class LessonSevenActivity extends Activity {
	/** Hold a reference to our GLSurfaceView */
	private LessonSevenGLSurfaceView mGLSurfaceView;
	private LessonSevenRenderer mRenderer;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		setContentView(R.layout.lesson_seven);

		mGLSurfaceView = (LessonSevenGLSurfaceView) findViewById(R.id.gl_surface_view);

		// Check if the system supports OpenGL ES 2.0.
		final ActivityManager activityManager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
		final ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo();
		final boolean supportsEs2 = configurationInfo.reqGlEsVersion >= 0x20000;

		if (supportsEs2) {
			// Request an OpenGL ES 2.0 compatible context.
			mGLSurfaceView.setEGLContextClientVersion(2);

			final DisplayMetrics displayMetrics = new DisplayMetrics();
			getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);

			// Set the renderer to our demo renderer, defined below.
			mRenderer = new LessonSevenRenderer(this, mGLSurfaceView);
			mGLSurfaceView.setRenderer(mRenderer, displayMetrics.density);
		} else {
			// This is where you could create an OpenGL ES 1.x compatible
			// renderer if you wanted to support both ES 1 and ES 2.
			return;
		}

		findViewById(R.id.button_decrease_num_cubes).setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				decreaseCubeCount();
			}
		});

		findViewById(R.id.button_increase_num_cubes).setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				increaseCubeCount();
			}
		});

		findViewById(R.id.button_switch_VBOs).setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				toggleVBOs();
			}
		});
		
		findViewById(R.id.button_switch_stride).setOnClickListener(new OnClickListener() {
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
		mGLSurfaceView.onResume();
	}

	@Override
	protected void onPause() {
		// The activity must call the GL surface view's onPause() on activity
		// onPause().
		super.onPause();
		mGLSurfaceView.onPause();
	}

	private void decreaseCubeCount() {
		mGLSurfaceView.queueEvent(new Runnable() {
			@Override
			public void run() {
				mRenderer.decreaseCubeCount();
			}
		});
	}

	private void increaseCubeCount() {
		mGLSurfaceView.queueEvent(new Runnable() {
			@Override
			public void run() {
				mRenderer.increaseCubeCount();
			}
		});
	}

	private void toggleVBOs() {
		mGLSurfaceView.queueEvent(new Runnable() {
			@Override
			public void run() {
				mRenderer.toggleVBOs();
			}
		});
	}
	
	protected void toggleStride() {
		mGLSurfaceView.queueEvent(new Runnable() {
			@Override
			public void run() {
				mRenderer.toggleStride();
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
}