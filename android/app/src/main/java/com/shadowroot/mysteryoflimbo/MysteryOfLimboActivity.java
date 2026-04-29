package com.shadowroot.mysteryoflimbo;

import org.libsdl.app.SDLActivity;
import android.util.Log;
import android.os.Bundle;

public class MysteryOfLimboActivity extends SDLActivity {

    @Override
    protected String[] getLibraries() {
        return new String[] {
            "SDL2",
            "SDL2_mixer",
            "main"
        };
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        // Este log es nuestra señal de vida en logcat
        Log.i("ShadowRoot", "=== [ Mystery of Limbo: ENGINE START ] ===");
        super.onCreate(savedInstanceState);
    }
}

