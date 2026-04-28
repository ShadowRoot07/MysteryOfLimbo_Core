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

    // Sobrescribimos para añadir un log que nos ayude a debuggear en Termux con logcat
    @Override
    protected void onCreate(android.os.Bundle savedInstanceState) {
        Log.d("ShadowRoot", "Iniciando Mystery of Limbo...");
        super.onCreate(savedInstanceState);
    }
}

