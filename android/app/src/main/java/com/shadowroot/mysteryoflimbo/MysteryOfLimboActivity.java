package com.shadowroot.mysteryoflimbo;

import org.libsdl.app.SDLActivity;
import android.util.Log;
import android.os.Bundle;
import android.view.View; // Necesario para los flags de visibilidad

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
        Log.i("ShadowRoot", "=== [ Mystery of Limbo: ENGINE START ] ===");
        super.onCreate(savedInstanceState);
        
        // Aplicamos el modo inmersivo al iniciar
        hideSystemUI();
    }

    @Override
    public void onWindowFocusChanged(bool hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus) {
            // Si el usuario sale de la app y vuelve, nos aseguramos de ocultar todo de nuevo
            hideSystemUI();
        }
    }

    private void hideSystemUI() {
        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                | View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_FULLSCREEN);
    }
}

