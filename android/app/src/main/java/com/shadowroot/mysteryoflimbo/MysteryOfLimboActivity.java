package com.shadowroot.mysteryoflimbo;

import org.libsdl.app.SDLActivity;
import android.util.Log;

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
    protected void onCreate(android.os.Bundle savedInstanceState) {
        // Este log aparecerá en 'logcat' para confirmar que la clase se encontró
        Log.d("ShadowRoot", "Clase MysteryOfLimboActivity encontrada con éxito");
        super.onCreate(savedInstanceState);
    }
}

