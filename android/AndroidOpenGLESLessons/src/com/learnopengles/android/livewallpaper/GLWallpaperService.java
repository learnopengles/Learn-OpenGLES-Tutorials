package com.learnopengles.android.livewallpaper;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.Renderer;
import android.os.Build;
import android.service.wallpaper.WallpaperService;
import android.util.Log;
import android.view.SurfaceHolder;

import com.learnopengles.android.util.LoggerConfig;

public abstract class GLWallpaperService extends WallpaperService {

	public class GLEngine extends Engine {
		class WallpaperGLSurfaceView extends GLSurfaceView {
			private static final String TAG = "WallpaperGLSurfaceView";

			WallpaperGLSurfaceView(Context context) {
				super(context);

				if (LoggerConfig.ON) {
					Log.d(TAG, "WallpaperGLSurfaceView(" + context + ")");
				}
			}

			@Override
			public SurfaceHolder getHolder() {
				if (LoggerConfig.ON) {
					Log.d(TAG, "getHolder(): returning " + getSurfaceHolder());
				}

				return getSurfaceHolder();
			}

			public void onDestroy() {
				if (LoggerConfig.ON) {
					Log.d(TAG, "onDestroy()");
				}

				super.onDetachedFromWindow();
			}
		}

		private static final String TAG = "GLEngine";

		private WallpaperGLSurfaceView glSurfaceView;
		private boolean rendererHasBeenSet;		

		@Override
		public void onCreate(SurfaceHolder surfaceHolder) {
			if (LoggerConfig.ON) {
				Log.d(TAG, "onCreate(" + surfaceHolder + ")");
			}

			super.onCreate(surfaceHolder);

			glSurfaceView = new WallpaperGLSurfaceView(GLWallpaperService.this);
		}

		@Override
		public void onVisibilityChanged(boolean visible) {
			if (LoggerConfig.ON) {
				Log.d(TAG, "onVisibilityChanged(" + visible + ")");
			}

			super.onVisibilityChanged(visible);

			if (rendererHasBeenSet) {
				if (visible) {
					glSurfaceView.onResume();
				} else {					
					glSurfaceView.onPause();														
				}
			}
		}		

		@Override
		public void onDestroy() {
			if (LoggerConfig.ON) {
				Log.d(TAG, "onDestroy()");
			}

			super.onDestroy();
			glSurfaceView.onDestroy();
		}
		
		protected void setRenderer(Renderer renderer) {
			if (LoggerConfig.ON) {
				Log.d(TAG, "setRenderer(" + renderer + ")");
			}

			glSurfaceView.setRenderer(renderer);
			rendererHasBeenSet = true;
		}
		
		protected void setPreserveEGLContextOnPause(boolean preserve) {
			if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
				if (LoggerConfig.ON) {
					Log.d(TAG, "setPreserveEGLContextOnPause(" + preserve + ")");
				}
	
				glSurfaceView.setPreserveEGLContextOnPause(preserve);
			}
		}		

		protected void setEGLContextClientVersion(int version) {
			if (LoggerConfig.ON) {
				Log.d(TAG, "setEGLContextClientVersion(" + version + ")");
			}

			glSurfaceView.setEGLContextClientVersion(version);
		}
	}
}
