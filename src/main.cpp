#include <SDL.h>
#include <vector>
#include <iostream>
#include <string>
#include <cmath>

#include "ui/UIManager.h"
#include "world/Camera.h"
#include "world/Enemy.h"
#include "world/Platform.h"
#include "player/Player.h"
#include "input/InputManager.h"

// Inclusión directa para que el compilador vea la clase estática EarthSkill
#include "elements/EarthSkill.cpp"

extern void ProcessWorld(Player& p, std::vector<Platform>& level, std::vector<Enemy>& enemies, std::vector<Projectile>& bullets, float dt);
extern std::vector<Platform> LoadLevel(const std::string& path, std::vector<Enemy>& enemies);

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "No se pudo inicializar SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Mystery of Limbo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) return 1;

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) return 1;

    InputManager input;
    UIManager ui;
    Player player;
    Camera camera(800, 600);

    // Equipamos elementos por defecto para testing
    player.SetElements(EARTH, DARKNESS); 

    std::vector<Enemy> enemies;
    std::vector<Projectile> bullets;
    std::vector<Platform> level = LoadLevel("assets/maps/test_level.txt", enemies);

    if (!ui.LoadAssets(renderer)) {
        std::cerr << "Advertencia: Algunos assets de UI no se cargaron." << std::endl;
    }

    bool running = true;
    SDL_Event ev;
    Uint32 lastTime = SDL_GetTicks();

    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        float dt = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        if (dt > 0.05f) dt = 0.05f;

        input.Update();
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) running = false;
            input.HandleRawEvent(ev);
        }

        player.HandleInput(input);

        // Lógica de señales de habilidades de Tierra
        if (player.pendingPlatform) {
            level.push_back(EarthSkill::CreateTempPlatform(player.GetPos(), player.GetFaceDir()));
            player.pendingPlatform = false;
        }

        player.Update(dt);
        ProcessWorld(player, level, enemies, bullets, dt);
        camera.Follow(player.GetPos(), dt);

        // --- RENDERIZADO ---
        SDL_SetRenderDrawColor(renderer, 15, 15, 25, 255);
        SDL_RenderClear(renderer);

        // 1. Dibujar Plataformas
        for (const auto& plat : level) {
            SDL_Rect r = {
                (int)(plat.bounds.x - camera.pos.x),
                (int)(plat.bounds.y - camera.pos.y),
                (int)plat.bounds.w,
                (int)plat.bounds.h
            };
            if (plat.type == TEMPORARY) SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
            else if (plat.type == SPIKE) SDL_SetRenderDrawColor(renderer, 200, 50, 50, 255);
            else SDL_SetRenderDrawColor(renderer, 80, 80, 90, 255);
            SDL_RenderFillRect(renderer, &r);
        }

        // 2. Dibujar Enemigos
        for (const auto& e : enemies) {
            SDL_Rect r = {
                (int)(e.pos.x - camera.pos.x),
                (int)(e.pos.y - camera.pos.y),
                (int)e.hitbox.w,
                (int)e.hitbox.h
            };
            SDL_SetRenderDrawColor(renderer, 180, 0, 200, 255);
            SDL_RenderFillRect(renderer, &r);
        }

        // 3. Dibujar Balas
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        for (const auto& b : bullets) {
            SDL_Rect bRect = { (int)(b.pos.x - camera.pos.x), (int)(b.pos.y - camera.pos.y), (int)b.hitbox.w, (int)b.hitbox.h };
            SDL_RenderFillRect(renderer, &bRect);
        }

        // 4. Marca de Oscuridad
        if (player.GetHasMark()) {
            SDL_SetRenderDrawColor(renderer, 150, 0, 255, 180);
            SDL_Rect mRect = {
                (int)(player.GetShadowMark().x - camera.pos.x),
                (int)(player.GetShadowMark().y - camera.pos.y),
                32, 48
            };
            SDL_RenderDrawRect(renderer, &mRect);
        }

        // 5. Dibujar Jugador (con parpadeo y estados)
        bool shouldDraw = true;
        if (player.GetInvulTimer() > 0) {
            if ((SDL_GetTicks() / 100) % 2 == 0) shouldDraw = false;
        }

        if (shouldDraw) {
            SDL_Rect pRect = {
                (int)(player.GetPos().x - camera.pos.x),
                (int)(player.GetPos().y - camera.pos.y),
                (int)player.hitbox.w,
                (int)player.hitbox.h
            };

            if (player.IsLiquid()) SDL_SetRenderDrawColor(renderer, 100, 100, 255, 200);
            else if (player.IsAttacking()) SDL_SetRenderDrawColor(renderer, 255, 200, 0, 255);
            else SDL_SetRenderDrawColor(renderer, 0, 255, 100, 255);

            SDL_RenderFillRect(renderer, &pRect);
        }

        // 6. Dibujar Interfaz
        ui.Render(renderer, input, player);

        SDL_RenderPresent(renderer);
    }

    ui.Clean();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

