package com.learnopengles.android.lesson6;

import android.app.Activity;
import android.app.ActivityManager;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.pm.ConfigurationInfo;
import android.opengl.GLES20;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.view.View;
import android.view.View.OnClickListener;

import com.learnopengles.android.R;

public class LessonSixActivity extends Activity 
{
	/** Hold a reference to our GLSurfaceView */
	private LessonSixGLSurfaceView mGLSurfaceView;
	private LessonSixRenderer mRenderer;
	
	private static final int MIN_DIALOG = 1;
	private static final int MAG_DIALOG = 2;
	
	private int mMinSetting = -1;
	private int mMagSetting = -1;
	
	private static final String MIN_SETTING = "min_setting";
	private static final String MAG_SETTING = "mag_setting";
	
	@Override
	public void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.lesson_six);

		mGLSurfaceView = (LessonSixGLSurfaceView)findViewById(R.id.gl_surface_view);

		// Check if the system supports OpenGL ES 2.0.
		final ActivityManager activityManager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
		final ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo();
		final boolean supportsEs2 = configurationInfo.reqGlEsVersion >= 0x20000;

		if (supportsEs2) 
		{
			// Request an OpenGL ES 2.0 compatible context.
			mGLSurfaceView.setEGLContextClientVersion(2);
			
			final DisplayMetrics displayMetrics = new DisplayMetrics();
			getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);

			// Set the renderer to our demo renderer, defined below.
			mRenderer = new LessonSixRenderer(this);
			mGLSurfaceView.setRenderer(mRenderer, displayMetrics.density);					
		} 
		else 
		{
			// This is where you could create an OpenGL ES 1.x compatible
			// renderer if you wanted to support both ES 1 and ES 2.
			return;
		}
		
		findViewById(R.id.button_set_min_filter).setOnClickListener(new OnClickListener() 
		{		
			@Override
			public void onClick(View v) 
			{
				showDialog(MIN_DIALOG);				
			}
		});

		findViewById(R.id.button_set_mag_filter).setOnClickListener(new OnClickListener() 
		{		
			@Override
			public void onClick(View v) 
			{
				showDialog(MAG_DIALOG);				
			}
		});
		
		// Restore previous settings
		if (savedInstanceState != null)
		{
			mMinSetting = savedInstanceState.getInt(MIN_SETTING, -1);
			mMagSetting = savedInstanceState.getInt(MAG_SETTING, -1);
			
			if (mMinSetting != -1) { setMinSetting(mMinSetting); }
			if (mMagSetting != -1) { setMagSetting(mMagSetting); }
		}
	}

	@Override
	protected void onResume() 
	{
		// The activity must call the GL surface view's onResume() on activity
		// onResume().
		super.onResume();
		mGLSurfaceView.onResume();
	}

	@Override
	protected void onPause() 
	{
		// The activity must call the GL surface view's onPause() on activity
		// onPause().
		super.onPause();
		mGLSurfaceView.onPause();
	}
	
	@Override
	protected void onSaveInstanceState (Bundle outState)
	{
		outState.putInt(MIN_SETTING, mMinSetting);
		outState.putInt(MAG_SETTING, mMagSetting);
	}
	
	private void setMinSetting(final int item)
	{
		mMinSetting = item;
		
		mGLSurfaceView.queueEvent(new Runnable()
		{
			@Override
			public void run()
			{
				final int filter;
				
				if (item == 0)
				{
					filter = GLES20.GL_NEAREST;
				}
				else if (item == 1)
				{
					filter = GLES20.GL_LINEAR;
				}
				else if (item == 2)
				{
					filter = GLES20.GL_NEAREST_MIPMAP_NEAREST;
				}
				else if (item == 3)
				{
					filter = GLES20.GL_NEAREST_MIPMAP_LINEAR;
				}
				else if (item == 4)
				{
					filter = GLES20.GL_LINEAR_MIPMAP_NEAREST;
				}
				else // if (item == 5)
				{
					filter = GLES20.GL_LINEAR_MIPMAP_LINEAR;
				}
				
				mRenderer.setMinFilter(filter);
			}
		});
	}
	
	private void setMagSetting(final int item)
	{
		mMagSetting = item;
		
		mGLSurfaceView.queueEvent(new Runnable()
		{
			@Override
			public void run()
			{
				final int filter;
				
				if (item == 0)
				{
					filter = GLES20.GL_NEAREST;
				}
				else // if (item == 1)
				{
					filter = GLES20.GL_LINEAR;
				}	    						
				
				mRenderer.setMagFilter(filter);
			}
		});
	}
	
	@Override
	protected Dialog onCreateDialog(int id) 
	{
	    Dialog dialog = null;
	    
	    switch(id) 
	    {
	    	case MIN_DIALOG:
	    	{
	    		AlertDialog.Builder builder = new AlertDialog.Builder(this);
	    		builder.setTitle(getText(R.string.lesson_six_set_min_filter_message));
	    		builder.setItems(getResources().getStringArray(R.array.lesson_six_min_filter_types), new DialogInterface.OnClickListener() 	    		
	    		{
	    			@Override
	    		    public void onClick(final DialogInterface dialog, final int item) 
	    			{
	    				setMinSetting(item);
	    		    }
	    		});
	    		
	    		dialog = builder.create();
	    	}
	        break;
	    	case MAG_DIALOG:
	    	{
	    		AlertDialog.Builder builder = new AlertDialog.Builder(this);
	    		builder.setTitle(getText(R.string.lesson_six_set_mag_filter_message));
	    		builder.setItems(getResources().getStringArray(R.array.lesson_six_mag_filter_types), new DialogInterface.OnClickListener() 	    		
	    		{
	    			@Override
	    		    public void onClick(final DialogInterface dialog, final int item) 
	    			{
	    				setMagSetting(item);
	    		    }
	    		});
	    		
	    		dialog = builder.create();
	    	}
	        break;
	    default:
	        dialog = null;
	    }
	    
	    return dialog;
	}
}