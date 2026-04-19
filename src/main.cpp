#include <SDL.h>
#include <vector>
#include "Common.h"
#include "input/InputManager.h"
#include "player/Player.h"
#include "world/Platform.h"

extern void ProcessWorldInteractions(Player& p, const std::vector<Platform>& level);

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    // 1. Obtener la resolución nativa del dispositivo (ZTE Blade A54)
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);

    // 2. Crear ventana ocupando toda la pantalla disponible en Termux-X11
    SDL_Window* window = SDL_CreateWindow("Shadow Core", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        dm.w, dm.h, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // 3. ESTABLECER RESOLUCIÓN LÓGICA
    // Esto escala automáticamente tus 800x600 a cualquier pantalla, centrándolo.
    SDL_RenderSetLogicalSize(renderer, 800, 600);

    // Carga de Assets
    SDL_Texture* texBase = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP(Assets::JOY_BASE));
    SDL_Texture* texKnob = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP(Assets::JOY_KNOB));
    SDL_Texture* texBtn  = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP(Assets::BTN_Z));

    InputManager input;
    Player player;
    std::vector<Platform> level = {
        {{0, 550, 800, 50}, NORMAL, 0},
        {{300, 400, 200, 20}, NORMAL, 0},
        {{550, 530, 100, 20}, SPIKE, 25.0f}
    };

    bool running = true;
    SDL_Event ev;
    Uint32 lastTime = SDL_GetTicks();

    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        float dt = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        if (dt > 0.05f) dt = 0.05f;

        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) running = false;
            input.HandleRawEvent(ev);
        }

        input.Update();
        player.HandleInput(input);
        player.Update(dt);
        ProcessWorldInteractions(player, level);

        // --- RENDERIZADO ---
        
        // 1. Limpiar pantalla (Fondo oscuro)
        SDL_SetRenderDrawColor(renderer, 10, 10, 15, 255);
        SDL_RenderClear(renderer);

        // 2. Dibujar Plataformas
        for (const auto& plat : level) {
            SDL_Rect r = {(int)plat.bounds.x, (int)plat.bounds.y, (int)plat.bounds.w, (int)plat.bounds.h};
            if (plat.type == SPIKE) SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
            else SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
            SDL_RenderFillRect(renderer, &r);
        }

        // 3. Dibujar Jugador
        SDL_Rect pRect = {(int)player.pos.x, (int)player.pos.y, (int)player.hitbox.w, (int)player.hitbox.h};
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &pRect);

        // 4. Dibujar UI (Siempre al frente)
        SDL_Rect areaJoy = {50, 400, 150, 150};
        SDL_RenderCopy(renderer, texBase, NULL, &areaJoy);

        SDL_Rect knobRect = {
            (int)(areaJoy.x + 50 + (input.GetJoystick().x * 40)),
            (int)(areaJoy.y + 50 + (input.GetJoystick().y * 40)),
            50, 50
        };
        SDL_RenderCopy(renderer, texKnob, NULL, &knobRect);

        SDL_Rect btnRect = {650, 450, 100, 100};
        SDL_RenderCopy(renderer, texBtn, NULL, &btnRect);

        SDL_RenderPresent(renderer);
    }

    // Limpieza
    SDL_DestroyTexture(texBase); SDL_DestroyTexture(texKnob); SDL_DestroyTexture(texBtn);
    SDL_DestroyRenderer(renderer); SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

