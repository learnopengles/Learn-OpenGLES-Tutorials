package com.learnopengles.android.switchinglivewallpaper;

import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.opengl.GLSurfaceView.Renderer;
import android.preference.PreferenceManager;
import android.view.SurfaceHolder;

public abstract class OpenGLES2WallpaperService extends GLWallpaperService {
	private static final String SETTINGS_KEY = "use_gl_surface_view";

	@Override
	public Engine onCreateEngine() {
		if (PreferenceManager.getDefaultSharedPreferences(
				OpenGLES2WallpaperService.this).getBoolean(SETTINGS_KEY, true)) {
			return new ES2GLSurfaceViewEngine();
		} else {
			return new ES2RgbrnGLEngine();
		}
	}

	class ES2GLSurfaceViewEngine extends GLWallpaperService.GLSurfaceViewEngine {

		@Override
		public void onCreate(SurfaceHolder surfaceHolder) {
			super.onCreate(surfaceHolder);
			init(this);
		}
	}

	class ES2RgbrnGLEngine extends GLWallpaperService.RgbrnGLEngine {

		@Override
		public void onCreate(SurfaceHolder surfaceHolder) {
			super.onCreate(surfaceHolder);
			init(this);
		}
	}

	void init(OpenGLEngine engine) {
		// Check if the system supports OpenGL ES 2.0.
		final ActivityManager activityManager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
		final ConfigurationInfo configurationInfo = activityManager
				.getDeviceConfigurationInfo();
		final boolean supportsEs2 = configurationInfo.reqGlEsVersion >= 0x20000;

		if (supportsEs2) {
			// Request an OpenGL ES 2.0 compatible context.
			engine.setEGLContextClientVersion(2);

			// Set the renderer to our user-defined renderer.
			engine.setRenderer(getNewRenderer());
		} else {
			// This is where you could create an OpenGL ES 1.x compatible
			// renderer if you wanted to support both ES 1 and ES 2.
			return;
		}
	}

	abstract Renderer getNewRenderer();
}
