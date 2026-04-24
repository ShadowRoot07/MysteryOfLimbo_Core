#include "gfx/ShadowAudio.h"
#include <iostream>

ShadowAudio::ShadowAudio() : fallbackSound(nullptr) {}

ShadowAudio::~ShadowAudio() {
    Clean();
}

bool ShadowAudio::Init() {
    // Inicializamos con 44.1kHz (calidad CD) y 2 canales (Stereo)
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "[ShadowAudio] Error al inicializar SDL_mixer: " << Mix_GetError() << std::endl;
        return false;
    }

    // Opcional: Cargar un sonido de sistema por defecto si lo tienes
    fallbackSound = Mix_LoadWAV("assets/audio/system/fallback.wav");
    
    std::cout << "[ShadowAudio] Motor de audio iniciado correctamente." << std::endl;
    return true;
}

void ShadowAudio::LoadSound(const std::string& id, const std::string& path) {
    // Si ya existe en la caché, no lo cargamos de nuevo
    if (soundCache.count(id)) return;

    Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
    if (!chunk) {
        std::cerr << "[ShadowAudio] Advertencia: No se pudo cargar el archivo en " << path << std::endl;
        return;
    }

    soundCache[id] = chunk;
}

void ShadowAudio::Play(const std::string& id, int loops) {
    Mix_Chunk* chunkToPlay = nullptr;

    if (soundCache.count(id)) {
        chunkToPlay = soundCache[id];
    } else {
        chunkToPlay = fallbackSound;
    }

    if (chunkToPlay) {
        // -1 en el primer parámetro indica que SDL use el primer canal libre disponible
        Mix_PlayChannel(-1, chunkToPlay, loops);
    }
}

void ShadowAudio::Clean() {
    // Liberar cada pedazo de audio de la caché
    for (auto const& [id, chunk] : soundCache) {
        Mix_FreeChunk(chunk);
    }
    soundCache.clear();

    if (fallbackSound) {
        Mix_FreeChunk(fallbackSound);
        fallbackSound = nullptr;
    }

    Mix_CloseAudio();
    std::cout << "[ShadowAudio] Memoria de audio liberada." << std::endl;
}

