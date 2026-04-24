#include <SDL.h>
#include <vector>
#include <iostream>
#include <string>
#include <cmath>

#include "gfx/ShadowGFX.h"
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

    // --- INICIALIZACIÓN DE TECNOLOGÍA AVANZADA DE TEXTURAS ---
    ShadowGFX gfx(renderer);
    InputManager input;
    UIManager ui;
    Player player;
    Camera camera(800, 600);

    // Equipamos elementos por defecto para testing
    player.SetElements(EARTH, DARKNESS);

    std::vector<Enemy> enemies;
    std::vector<Projectile> bullets;
    std::vector<Platform> level = LoadLevel("assets/maps/test_level.txt", enemies);

    // Cargamos assets de UI a través de la librería
    if (!ui.LoadAssets(gfx)) {
        std::cerr << "Advertencia: Fallo al registrar texturas de UI en ShadowGFX." << std::endl;
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
            Platform tempP = EarthSkill::CreateTempPlatform(player.GetPos(), player.GetFaceDir());
            tempP.textureID = "temp_platform_earth"; // Asignamos ID para la librería
            level.push_back(tempP);
            player.pendingPlatform = false;
        }

        player.Update(dt);
        ProcessWorld(player, level, enemies, bullets, dt);
        camera.Follow(player.GetPos(), dt);

        // --- RENDERIZADO CON SHADOW_GFX ---
        SDL_SetRenderDrawColor(renderer, 15, 15, 25, 255);
        SDL_RenderClear(renderer);

        // 1. Dibujar Plataformas (Ahora usan texturas o Fallback aleatorio)
        for (const auto& plat : level) {
            SDL_Rect r = {
                (int)(plat.bounds.x - camera.pos.x),
                (int)(plat.bounds.y - camera.pos.y),
                (int)plat.bounds.w,
                (int)plat.bounds.h
            };
            gfx.DrawStatic(plat.textureID, r);
        }

        // 2. Dibujar Enemigos
        for (const auto& e : enemies) {
            SDL_Rect r = {
                (int)(e.pos.x - camera.pos.x),
                (int)(e.pos.y - camera.pos.y),
                (int)e.hitbox.w,
                (int)e.hitbox.h
            };
            // Usamos un ID genérico para enemigos mientras definimos sus sheets
            gfx.DrawStatic("enemy_generic", r);
        }

        // 3. Dibujar Balas
        for (const auto& b : bullets) {
            SDL_Rect bRect = { (int)(b.pos.x - camera.pos.x), (int)(b.pos.y - camera.pos.y), (int)b.hitbox.w, (int)b.hitbox.h };
            gfx.DrawStatic("projectile_yellow", bRect);
        }

        // 4. Marca de Oscuridad
        if (player.GetHasMark()) {
            SDL_Rect mRect = {
                (int)(player.GetShadowMark().x - camera.pos.x),
                (int)(player.GetShadowMark().y - camera.pos.y),
                32, 48
            };
            gfx.DrawStatic("shadow_mark", mRect);
        }

        // 5. Dibujar Jugador (con sistema de animaciones 64x64)
        bool shouldDraw = true;
        if (player.GetInvulTimer() > 0) {
            if ((SDL_GetTicks() / 100) % 2 == 0) shouldDraw = false;
        }

        if (shouldDraw) {
            SDL_Rect pRect = {
                (int)(player.GetPos().x - camera.pos.x),
                (int)(player.GetPos().y - camera.pos.y),
                64, 64 // Forzamos el tamaño del frame de la librería
            };

            // Aquí llamamos a DrawAnimated. El frame se controlará en el Player más adelante.
            // Por ahora usamos frame 0 y detectamos dirección para el flip.
            gfx.DrawAnimated("player_main", pRect, 0, player.GetFaceDir() < 0);
        }

        // 6. Dibujar Interfaz (Pasamos la referencia de gfx)
        ui.Render(renderer, gfx, input, player);

        SDL_RenderPresent(renderer);
    }

    ui.Clean();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

