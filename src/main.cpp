#include <SDL.h>
#include <vector>
#include <string>
#include <cmath>
#include "Common.h"
#include "input/InputManager.h"
#include "player/Player.h"
#include "world/Platform.h"
#include "world/Camera.h"
#include "world/Enemy.h"

extern void ProcessWorld(Player& p, std::vector<Platform>& level, std::vector<Enemy>& enemies, std::vector<Projectile>& bullets, float dt);
extern std::vector<Platform> LoadLevel(const std::string& path);

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    SDL_Window* window = SDL_CreateWindow("Shadow Core", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, dm.w, dm.h, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_RenderSetLogicalSize(renderer, 800, 600);

    // Assets
    SDL_Texture* texBase = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP(Assets::JOY_BASE));
    SDL_Texture* texKnob = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP(Assets::JOY_KNOB));
    SDL_Texture* texBtnZ = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP(Assets::BTN_Z));
    SDL_Texture* texBtnX = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP(Assets::BTN_X));
    SDL_Texture* texBtnF = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP(Assets::BTN_F));

    InputManager input;
    Player player;
    Camera camera(800, 600);
    
    std::vector<Platform> level = LoadLevel("assets/maps/test_level.txt");
    std::vector<Enemy> enemies;
    std::vector<Projectile> bullets;

    // Inicialización manual de enemigos (corregida para evitar errores de push_back)
    Enemy e1; e1.pos = {400, 300}; e1.hitbox = {400, 300, 32, 48}; e1.type = WALKER; enemies.push_back(e1);
    Enemy e2; e2.pos = {600, 200}; e2.hitbox = {600, 200, 32, 48}; e2.type = FLYER; enemies.push_back(e2);
    Enemy e3; e3.pos = {200, 450}; e3.hitbox = {200, 450, 32, 48}; e3.type = TURRET; enemies.push_back(e3);

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
        ProcessWorld(player, level, enemies, bullets, dt);
        camera.Follow(player.pos, dt);

        SDL_SetRenderDrawColor(renderer, 10, 10, 15, 255);
        SDL_RenderClear(renderer);

        for (const auto& plat : level) {
            SDL_Rect r = { (int)(plat.bounds.x - camera.pos.x), (int)(plat.bounds.y - camera.pos.y), (int)plat.bounds.w, (int)plat.bounds.h };
            if (plat.type == SPIKE) SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
            else SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
            SDL_RenderFillRect(renderer, &r);
        }

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        for (const auto& b : bullets) {
            SDL_Rect bRect = { (int)(b.pos.x - camera.pos.x), (int)(b.pos.y - camera.pos.y), (int)b.hitbox.w, (int)b.hitbox.h };
            SDL_RenderFillRect(renderer, &bRect);
        }

        SDL_SetRenderDrawColor(renderer, 200, 0, 255, 255);
        for (const auto& e : enemies) {
            SDL_Rect eRect = { (int)(e.pos.x - camera.pos.x), (int)(e.pos.y - camera.pos.y), (int)e.hitbox.w, (int)e.hitbox.h };
            SDL_RenderFillRect(renderer, &eRect);
        }

        SDL_Rect pRect = { (int)(player.pos.x - camera.pos.x), (int)(player.pos.y - camera.pos.y), (int)player.hitbox.w, (int)player.hitbox.h };
        if (player.IsDashing()) SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
        else if (player.IsAttacking()) SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255);
        else SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &pRect);

        // UI
        SDL_Rect areaJoy = {30, 370, 180, 180};
        SDL_RenderCopy(renderer, texBase, NULL, &areaJoy);
        SDL_Rect knobRect = { (int)(areaJoy.x + 65 + (input.GetJoystick().x * 50)), (int)(areaJoy.y + 65 + (input.GetJoystick().y * 50)), 50, 50 };
        SDL_RenderCopy(renderer, texKnob, NULL, &knobRect);
        SDL_Rect rZ = {680, 460, 80, 80}, rX = {590, 480, 80, 80}, rF = {680, 370, 80, 80};
        SDL_RenderCopy(renderer, texBtnZ, NULL, &rZ);
        SDL_RenderCopy(renderer, texBtnX, NULL, &rX);
        SDL_RenderCopy(renderer, texBtnF, NULL, &rF);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texBase); SDL_DestroyTexture(texKnob);
    SDL_DestroyTexture(texBtnZ); SDL_DestroyTexture(texBtnX); SDL_DestroyTexture(texBtnF);
    SDL_DestroyRenderer(renderer); SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

