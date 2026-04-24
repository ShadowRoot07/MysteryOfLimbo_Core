#include "gfx/ShadowGFX.h"
#include <iostream>

ShadowGFX::ShadowGFX(SDL_Renderer* r) : renderer(r) {}

ShadowGFX::~ShadowGFX() {
    for (auto const& [id, tex] : textureCache) {
        SDL_DestroyTexture(tex);
    }
}

SDL_Texture* ShadowGFX::CreateFallbackTexture() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(50, 255);

    SDL_Surface* surface = SDL_CreateRGBSurface(0, 32, 32, 32, 0, 0, 0, 0);
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, dis(gen), dis(gen), dis(gen)));
    
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return tex;
}

SDL_Texture* ShadowGFX::GetTexture(const std::string& id, const std::string& path, bool useColorKey) {
    if (textureCache.count(id)) return textureCache[id];

    if (path != "") {
        SDL_Surface* surface = SDL_LoadBMP(path.c_str());
        if (surface) {
            if (useColorKey) {
                // TÉCNICA MAESTRA: Usamos el primer píxel (0,0) como color transparente automático
                Uint32 colorkey;
                if (SDL_LockSurface(surface) == 0) {
                    Uint8* pixels = (Uint8*)surface->pixels;
                    // Obtenemos el valor del primer píxel según el formato de la imagen
                    if (surface->format->BytesPerPixel == 1) colorkey = *pixels;
                    else if (surface->format->BytesPerPixel == 2) colorkey = *(Uint16*)pixels;
                    else if (surface->format->BytesPerPixel == 3) {
                        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                            colorkey = pixels[0] << 16 | pixels[1] << 8 | pixels[2];
                        else
                            colorkey = pixels[0] | pixels[1] << 8 | pixels[2] << 16;
                    } 
                    else colorkey = *(Uint32*)pixels;
                    
                    SDL_UnlockSurface(surface);
                    SDL_SetColorKey(surface, SDL_TRUE, colorkey);
                }
            }
            
            // Creamos la textura
            SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
            
            // FORZAR MODO DE MEZCLA: Vital para que la transparencia se procese
            SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);

            SDL_FreeSurface(surface);
            
            if (tex) {
                textureCache[id] = tex;
                return tex;
            }
        }
    }

    std::cout << "[ShadowGFX] Advertencia: Usando fallback para " << id << std::endl;
    SDL_Texture* fallback = CreateFallbackTexture();
    textureCache[id] = fallback;
    return fallback;
}

void ShadowGFX::DrawStatic(const std::string& id, SDL_Rect dest) {
    SDL_RenderCopy(renderer, GetTexture(id), NULL, &dest);
}

void ShadowGFX::DrawAnimated(const std::string& id, SDL_Rect dest, int frame, bool flip) {
    SDL_Rect src = { frame * 64, 0, 64, 64 };
    SDL_RendererFlip sdlFlip = flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    SDL_RenderCopyEx(renderer, GetTexture(id), &src, &dest, 0, NULL, sdlFlip);
}

