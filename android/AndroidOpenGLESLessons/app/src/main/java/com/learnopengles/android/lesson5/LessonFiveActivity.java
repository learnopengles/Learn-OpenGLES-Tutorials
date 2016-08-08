package com.learnopengles.android.lesson5;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.os.Bundle;
import android.widget.Toast;

import com.learnopengles.android.R;

public class LessonFiveActivity extends Activity 
{
	/** Hold a reference to our GLSurfaceView */
	private LessonFiveGLSurfaceView mGLSurfaceView;
	
	private static final String SHOWED_TOAST = "showed_toast";

	@Override
	public void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
		
		mGLSurfaceView = new LessonFiveGLSurfaceView(this);

		// Check if the system supports OpenGL ES 2.0.
		final ActivityManager activityManager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
		final ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo();
		final boolean supportsEs2 = configurationInfo.reqGlEsVersion >= 0x20000;

		if (supportsEs2) 
		{
			// Request an OpenGL ES 2.0 compatible context.
			mGLSurfaceView.setEGLContextClientVersion(2);

			// Set the renderer to our demo renderer, defined below.
			mGLSurfaceView.setRenderer(new LessonFiveRenderer(this));
		} 
		else 
		{
			// This is where you could create an OpenGL ES 1.x compatible
			// renderer if you wanted to support both ES 1 and ES 2.
			return;
		}

		setContentView(mGLSurfaceView);
		
		// Show a short help message to the user.
		if (savedInstanceState == null || !savedInstanceState.getBoolean(SHOWED_TOAST, false))
		{
			Toast.makeText(this, R.string.lesson_five_startup_toast, Toast.LENGTH_SHORT).show();
		}
	}

	@Override
	protected void onResume() 
	{
		// The activity must call the GL surface view's onResume() on activity onResume().
		super.onResume();
		mGLSurfaceView.onResume();
	}

	@Override
	protected void onPause() 
	{
		// The activity must call the GL surface view's onPause() on activity onPause().
		super.onPause();
		mGLSurfaceView.onPause();
	}	
	
	@Override
	protected void onSaveInstanceState (Bundle outState)
	{
		outState.putBoolean(SHOWED_TOAST, true);
	}
}