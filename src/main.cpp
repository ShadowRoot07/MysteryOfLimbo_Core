#include <SDL.h>
#include <vector>
#include <iostream>
#include <string>
#include <cmath>

#include "gfx/ShadowGFX.h"
#include "gfx/ShadowAudio.h" // Incluimos la nueva librería
#include "ui/UIManager.h"
#include "world/Camera.h"
#include "world/Enemy.h"
#include "world/Platform.h"
#include "player/Player.h"
#include "input/InputManager.h"

#include "elements/EarthSkill.cpp"

extern void ProcessWorld(Player& p, std::vector<Platform>& level, std::vector<Enemy>& enemies, std::vector<Projectile>& bullets, float dt);
extern std::vector<Platform> LoadLevel(const std::string& path, std::vector<Enemy>& enemies);

int main(int argc, char* argv[]) {
    // Inicializamos Video y Audio
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "No se pudo inicializar SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Mystery of Limbo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) return 1;

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) return 1;

    // --- SISTEMAS SHADOW ---
    ShadowGFX gfx(renderer);
    ShadowAudio sfx;
    if (!sfx.Init()) {
        std::cerr << "Advertencia: El motor de audio no inició, el juego seguirá en silencio." << std::endl;
    }

    InputManager input;
    UIManager ui;
    Player player;
    Camera camera(800, 600);

    // Precarga de sonidos
    sfx.LoadSound("jump", "assets/audio/fx/jump.wav");
    sfx.LoadSound("double_jump", "assets/audio/fx/double_jump.wav");
    sfx.LoadSound("attack", "assets/audio/fx/attack.wav");
    sfx.LoadSound("dash", "assets/audio/fx/dash.wav");
    sfx.LoadSound("earth_skill", "assets/audio/fx/earth.wav");
    sfx.LoadSound("mark_set", "assets/audio/fx/mark.wav");
    sfx.LoadSound("teleport", "assets/audio/fx/teleport.wav");
    sfx.LoadSound("liquid_form", "assets/audio/fx/liquid.wav");

    player.SetElements(EARTH, DARKNESS);

    std::vector<Enemy> enemies;
    std::vector<Projectile> bullets;
    std::vector<Platform> level = LoadLevel("assets/maps/test_level.txt", enemies);

    if (!ui.LoadAssets(gfx)) {
        std::cerr << "Advertencia: Fallo al registrar texturas de UI." << std::endl;
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

        // Ahora pasamos input Y sfx
        player.HandleInput(input, sfx);

        if (player.pendingPlatform) {
            Platform tempP = EarthSkill::CreateTempPlatform(player.GetPos(), player.GetFaceDir());
            tempP.textureID = "temp_platform_earth";
            level.push_back(tempP);
            player.pendingPlatform = false;
        }

        player.Update(dt);
        ProcessWorld(player, level, enemies, bullets, dt);
        camera.Follow(player.GetPos(), dt);

        SDL_SetRenderDrawColor(renderer, 15, 15, 25, 255);
        SDL_RenderClear(renderer);

        for (const auto& plat : level) {
            SDL_Rect r = {
                (int)(plat.bounds.x - camera.pos.x),
                (int)(plat.bounds.y - camera.pos.y),
                (int)plat.bounds.w,
                (int)plat.bounds.h
            };
            gfx.DrawStatic(plat.textureID, r);
        }

        for (const auto& e : enemies) {
            SDL_Rect r = {(int)(e.pos.x - camera.pos.x), (int)(e.pos.y - camera.pos.y), (int)e.hitbox.w, (int)e.hitbox.h};
            gfx.DrawStatic("enemy_generic", r);
        }

        for (const auto& b : bullets) {
            SDL_Rect bRect = {(int)(b.pos.x - camera.pos.x), (int)(b.pos.y - camera.pos.y), (int)b.hitbox.w, (int)b.hitbox.h};
            gfx.DrawStatic("projectile_yellow", bRect);
        }

        if (player.GetHasMark()) {
            SDL_Rect mRect = {(int)(player.GetShadowMark().x - camera.pos.x), (int)(player.GetShadowMark().y - camera.pos.y), 32, 48};
            gfx.DrawStatic("shadow_mark", mRect);
        }

        bool shouldDraw = true;
        if (player.GetInvulTimer() > 0) {
            if ((SDL_GetTicks() / 100) % 2 == 0) shouldDraw = false;
        }

        if (shouldDraw) {
            SDL_Rect pRect = {(int)(player.GetPos().x - camera.pos.x), (int)(player.GetPos().y - camera.pos.y), 64, 64};
            gfx.DrawAnimated("player_main", pRect, 0, player.GetFaceDir() < 0);
        }

        ui.Render(renderer, gfx, input, player);
        SDL_RenderPresent(renderer);
    }

    sfx.Clean(); // Limpieza de audio obligatoria
    ui.Clean();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_Quit(); // SDL_mixer
    SDL_Quit();

    return 0;
}

