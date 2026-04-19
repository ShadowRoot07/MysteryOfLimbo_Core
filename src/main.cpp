#include <SDL.h>
#include <vector>
#include <iostream>
#include "Common.h"
#include "input/InputManager.h"
#include "player/Player.h"
#include "physics/Collision.h"
#include "world/Platform.h"

// Prototipo externo
extern void ProcessWorldInteractions(Player& p, const std::vector<Platform>& level);

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return -1;

    // Usamos Flags de compatibilidad para Termux-X11
    SDL_Window* window = SDL_CreateWindow("Shadow Core",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    
    // Cambiado a -1 y 0 para que SDL elija el driver más estable automáticamente
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

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

    std::cout << "[CORE] Iniciando motor..." << std::endl;

    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        float dt = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        // Cap de DeltaTime para evitar teletransporte por lag
        if (dt > 0.05f) dt = 0.05f;

        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) running = false;
            
            // --- NUEVA LÍNEA: Procesar el toque ---
            input.HandleRawEvent(ev);
        }

        SDL_PumpEvents(); 
        input.Update();
        
        player.HandleInput(input);
        player.Update(dt);
        ProcessWorldInteractions(player, level);

        // Renderizado
        SDL_SetRenderDrawColor(renderer, 10, 10, 15, 255); 
        SDL_RenderClear(renderer);

        for (const auto& plat : level) {
            SDL_Rect r = {(int)plat.bounds.x, (int)plat.bounds.y, (int)plat.bounds.w, (int)plat.bounds.h};
            if (plat.type == SPIKE) SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
            else SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
            SDL_RenderFillRect(renderer, &r);
        }

        SDL_Rect pRect = {(int)player.pos.x, (int)player.pos.y, (int)player.hitbox.w, (int)player.hitbox.h};
        if (player.IsInvulnerable() && (SDL_GetTicks() / 100) % 2 == 0) 
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100);
        else 
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        
        SDL_RenderFillRect(renderer, &pRect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

