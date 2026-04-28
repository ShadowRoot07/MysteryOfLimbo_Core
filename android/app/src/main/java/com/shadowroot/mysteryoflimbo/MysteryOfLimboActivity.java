package com.shadowroot.mysteryoflimbo;

import org.libsdl.app.SDLActivity;

// Extendemos SDLActivity para aprovechar la inicialización de SDL
public class MysteryOfLimboActivity extends SDLActivity {

    @Override
    protected String[] getLibraries() {
        return new String[] {
            "main" // Esto carga tu libmain.so
        };
    }
}

