package com.shadowroot.mysteryoflimbo;

import org.libsdl.app.SDLActivity;

public class MysteryOfLimboActivity extends SDLActivity {

    @Override
    protected String[] getLibraries() {
        return new String[] {
            "SDL2",       // Cargamos el núcleo de SDL primero
            "SDL2_mixer", // Cargamos el mezclador para el audio
            "main"        // Finalmente tu juego, que depende de las anteriores
        };
    }
}

