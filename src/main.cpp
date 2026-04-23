#include <SDL.h>
#include <vector>
#include <iostream>
#include <string>
#include <cmath> // Necesario para fmod y lógica de parpadeo

// Organización de headers por módulos
#include "ui/UIManager.h"
#include "world/Camera.h"
#include "world/Enemy.h"
#include "world/Platform.h"
#include "player/Player.h"
#include "input/InputManager.h"

// Inclusión de la lógica de habilidades (Asegúrate que el archivo exista en src/elements/)
#include "elements/EarthSkill.cpp"

// Declaraciones de funciones externas definidas en otros .cpp
extern void ProcessWorld(Player& p, std::vector<Platform>& level, std::vector<Enemy>& enemies, std::vector<Projectile>& bullets, float dt);
extern std::vector<Platform> LoadLevel(const std::string& path, std::vector<Enemy>& enemies);

int main(int argc, char* argv[]) {
    // Inicialización de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "No se pudo inicializar SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Configuración de ventana y renderizado
    SDL_Window* window = SDL_CreateWindow("Mystery of Limbo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Error al crear ventana: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Error al crear renderer: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Instancia de Managers y Entidades
    InputManager input;
    UIManager ui;
    Player player;
    Camera camera(800, 600);

    // --- TEST DE ELEMENTOS ---
    // Puedes cambiar estos para probar las combinaciones en tu ZTE
    player.SetElements(EARTH, DARKNESS); 

    std::vector<Enemy> enemies;
    std::vector<Projectile> bullets;

    // Carga de Mapa y Assets
    std::vector<Platform> level = LoadLevel("assets/maps/test_level.txt", enemies);

    if (!ui.LoadAssets(renderer)) {
        std::cerr << "Error critico: No se pudieron cargar los assets de la UI en assets/sprites/ui/" << std::endl;
    }

    bool running = true;
    SDL_Event ev;
    Uint32 lastTime = SDL_GetTicks();

    // Loop Principal
    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        float dt = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        if (dt > 0.05f) dt = 0.05f;

        // 1. PRIMERO actualizamos el estado anterior
        input.Update();

        // 2. SEGUNDO procesamos nuevos eventos
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) running = false;
            input.HandleRawEvent(ev);
        }

        // 3. TERCERO procesamos la lógica
        player.HandleInput(input);

        // --- LÓGICA DE SEÑALES DE HABILIDADES (NUEVO) ---
        if (player.pendingPlatform) {
            // Si el jugador activó la combo de Tierra, creamos la plataforma física aquí
            level.push_back(EarthSkill::CreateTempPlatform(player.GetPos(), player.GetFaceDir()));
            player.pendingPlatform = false; // Reset de la señal
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
            if (plat.type == TEMPORARY) SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); // Café para Tierra
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
            SDL_Rect bRect = {
                (int)(b.pos.x - camera.pos.x),
                (int)(b.pos.y - camera.pos.y),
                (int)b.hitbox.w,
                (int)b.hitbox.h
            };
            SDL_RenderFillRect(renderer, &bRect);
        }

        // 4. Marca de Oscuridad (Feedback visual de TP)
        if (player.GetHasMark()) {
            SDL_SetRenderDrawColor(renderer, 150, 0, 255, 150);
            SDL_Rect mRect = {
                (int)(player.GetShadowMark().x - camera.pos.x),
                (int)(player.GetShadowMark().y - camera.pos.y),
                32, 48
            };
            SDL_RenderDrawRect(renderer, &mRect); // Dibujamos solo el borde
        }

        // 5. Dibujar Jugador
        bool shouldDraw = true;
        if (player.GetInvulTimer() > 0) {
            if ((SDL_GetTicks() / 100) % 2 == 0) {
                shouldDraw = false;
            }
        }

        if (shouldDraw) {
            SDL_Rect pRect = {
                (int)(player.GetPos().x - camera.pos.x),
                (int)(player.GetPos().y - camera.pos.y),
                (int)player.hitbox.w,
                (int)player.hitbox.h
            };

            // Colores según estado elemental
            if (player.IsLiquid()) {
                SDL_SetRenderDrawColor(renderer, 100, 100, 255, 200); // Azul translúcido (Agua)
            } else if (player.IsAttacking()) {
                SDL_SetRenderDrawColor(renderer, 255, 200, 0, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 255, 100, 255);
            }

            SDL_RenderFillRect(renderer, &pRect);
        }

        // 6. Dibujar Interfaz (Pasamos el player para mostrar estados en la UI si lo necesitas)
        ui.Render(renderer, input, player);

        SDL_RenderPresent(renderer);
    }

    // Limpieza
    ui.Clean();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

