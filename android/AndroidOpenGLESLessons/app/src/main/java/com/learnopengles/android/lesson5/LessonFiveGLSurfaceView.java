package com.learnopengles.android.lesson5;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;

public class LessonFiveGLSurfaceView extends GLSurfaceView 
{
	private LessonFiveRenderer mRenderer;
	
	public LessonFiveGLSurfaceView(Context context) 
	{
		super(context);	
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) 
	{
		if (event != null)
		{
			if (event.getAction() == MotionEvent.ACTION_DOWN)
			{
				if (mRenderer != null)
				{
					// Ensure we call switchMode() on the OpenGL thread.
					// queueEvent() is a method of GLSurfaceView that will do this for us.
					queueEvent(new Runnable()
					{
						@Override
						public void run()
						{
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
	public void setRenderer(LessonFiveRenderer renderer) 
	{
		mRenderer = renderer;
		super.setRenderer(renderer);
	}
}
