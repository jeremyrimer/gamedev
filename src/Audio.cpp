#include "Audio.h"
#include <iostream>
#include <cstring>
#include <vector>

Audio::Audio(const std::string& filePath)  {
    loadWAV(filePath);
    initAudioDevice();

    if (!buffer) return;

    stream = SDL_CreateAudioStream(&spec, &spec);
    if (!stream) {
        std::cerr << "Failed to create audio stream: " << SDL_GetError() << "\n";
        return;
    }

    SDL_BindAudioStream(device, stream);
    SDL_ResumeAudioDevice(device);
}

Audio::~Audio() {
    if (stream) {
        SDL_UnbindAudioStream(stream);
        SDL_DestroyAudioStream(stream);
    }
    if (device) {
        SDL_CloseAudioDevice(device);
    }
    if (buffer) {
        SDL_free(buffer);
    }
}

void Audio::loadWAV(const std::string& filePath) {
    if (!SDL_LoadWAV(filePath.c_str(), &spec, &buffer, &bufferLen)) {
        std::cerr << "Failed to load WAV: " << SDL_GetError() << "\n";
    }
}

void Audio::initAudioDevice() {
    device = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);
    if (!device) {
        std::cerr << "Failed to open audio device: " << SDL_GetError() << "\n";
    }
}

void Audio::play() {
    if (playing || !stream || !buffer) return;

    SDL_ClearAudioStream(stream);

    Uint8* tempBuf = nullptr;

    // Only create a temp buffer if volume needs adjusting
    if (volume < 0.99f) {
        tempBuf = (Uint8*)SDL_malloc(bufferLen);
        std::memcpy(tempBuf, buffer, bufferLen);

        Sint16* samples = (Sint16*)tempBuf;
        size_t sampleCount = bufferLen / sizeof(Sint16);

        for (size_t i = 0; i < sampleCount; ++i) {
            float scaled = static_cast<float>(samples[i]) * volume;
            samples[i] = static_cast<Sint16>(SDL_clamp(scaled, -32768.0f, 32767.0f));
        }
    }

    if (volume < 0.99f) {
        SDL_PutAudioStreamData(stream, tempBuf, bufferLen);
    } else {
        SDL_PutAudioStreamData(stream, buffer, bufferLen);
    }

    if (tempBuf) SDL_free(tempBuf);

    SDL_ResumeAudioDevice(device);
    playing = true;
}

void Audio::stop() {
    if (!playing) return;

    if (stream) {
        SDL_ClearAudioStream(stream);
        SDL_FlushAudioStream(stream);

        // Push silence to instantly kill sound
        std::vector<Uint8> silence(bufferLen, 0);
        SDL_PutAudioStreamData(stream, silence.data(), bufferLen);
    }

    SDL_PauseAudioDevice(device);
    playing = false;
}

void Audio::setVolume(float vol) {
    volume = SDL_clamp(vol, 0.0f, 1.0f);
}

bool Audio::isPlaying() const {
    return playing;
}

void Audio::updateForLooping() {
    if (playing && SDL_GetAudioStreamAvailable(stream) < (int)bufferLen) {
        if (SDL_PutAudioStreamData(stream, buffer, bufferLen) < 0) {
            SDL_Log("Failed to queue audio: %s", SDL_GetError());
        }
    }
}
