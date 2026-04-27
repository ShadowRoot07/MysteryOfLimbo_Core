#ifndef SHADOW_GFX_H
#define SHADOW_GFX_H

#include <SDL2/SDL.h>
#include <string>
#include <map>
#include <random>

class ShadowGFX {
public:
    ShadowGFX(SDL_Renderer* renderer);
    ~ShadowGFX();

    // useColorKey ahora es true por defecto y detecta el color automáticamente
    SDL_Texture* GetTexture(const std::string& id, const std::string& path = "", bool useColorKey = true);

    void DrawStatic(const std::string& id, SDL_Rect dest);
    void DrawAnimated(const std::string& id, SDL_Rect dest, int frame, bool flip = false);

private:
    SDL_Renderer* renderer;
    std::map<std::string, SDL_Texture*> textureCache;
    SDL_Texture* CreateFallbackTexture();
};

#endif

