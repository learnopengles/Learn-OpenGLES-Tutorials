package com.learnopengles.android.lesson6;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.MotionEvent;

public class LessonSixGLSurfaceView extends GLSurfaceView {

    private Action mRenderer;

    private float mPreviousX;
    private float mPreviousY;
    private float mDensity;

    public LessonSixGLSurfaceView(Context context) {
        super(context);
    }

    public LessonSixGLSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if (event != null) {
            float x = event.getX();
            float y = event.getY();
            if (event.getAction() == MotionEvent.ACTION_MOVE) {
                if (mRenderer != null) {
                    float deltaX = (x - mPreviousX) / mDensity / 2;
                    float deltaY = (y - mPreviousY) / mDensity / 2;
                    mRenderer.setDelta(deltaX, deltaY);
                }
            }
            mPreviousX = x;
            mPreviousY = y;
            return true;
        }

        return super.onTouchEvent(event);
    }

    // Hides superclass method.
    public void setRenderer(GLSurfaceView.Renderer renderer, float density) {
        mRenderer = (Action) renderer;
        mDensity = density;
        super.setRenderer(renderer);
    }
}