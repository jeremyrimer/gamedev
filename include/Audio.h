#pragma once
#include <SDL3/SDL.h>
#include <string>

class Audio {
public:
    /**
     * Constructor takes relative file path to a WAV file
     * e.g. assets/sound/thrusters.wav
     */
    Audio(const std::string& filePath);
    ~Audio();

    void play();
    void stop();
    void setLoop(bool loop);
    void setVolume(float volume); // 0.0 to 1.0
    bool isPlaying() const;
    void updateForLooping(); // Call in game loop update phase if looping

private:
    SDL_AudioDeviceID device = 0;
    SDL_AudioStream* stream = nullptr;
    Uint8* buffer = nullptr;
    Uint32 bufferLen = 0;
    SDL_AudioSpec spec{};

    bool playing = false;
    float volume = 1.0f;

    void loadWAV(const std::string& filePath);
    void initAudioDevice();
};
