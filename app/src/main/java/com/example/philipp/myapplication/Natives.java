package com.example.philipp.myapplication;

import android.content.res.AssetManager;




import android.content.res.AssetManager;

public class Natives {

    static
    {
        System.loadLibrary("GraphicsEngine");

    }

    private static EventListener listener;

    public static interface EventListener
    {
        void OnMessage(String text);
        void GLSwapBuffers();
    }

    public static void setListener (EventListener l) {
        listener = l;
    }

    /**
     * Native Render test
     * @return
     */
    public static native int NativeRender();

    public static native void NativeInit(AssetManager assetManager);
    /**
     * Receive text from native lib
     * @param text
     * @deprecated "Use the Android logging";
     */
    private static void OnMessage(String text) {
        if ( listener != null)
            listener.OnMessage(text);
    }

    private static void GLSwapBuffers() {
        if ( listener != null)
            listener.GLSwapBuffers();
    }

}
