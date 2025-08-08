#include "DebugHUD.h"
#include "Player.h"
#include <SDL3_image/SDL_image.h>
#include <string>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <Constants.h>
#include <stdexcept>
#include "Text.h"


DebugHUD::DebugHUD(SDL_Renderer* renderer, Player* player) : 
  renderer(renderer), 
  player(player),
  font(Text(renderer, "assets/fonts/jb.ttf", 12)) {}

void DebugHUD::handleInput(const bool* keystates) {
    bool currentF3State = keystates[SDL_SCANCODE_F3];

    // Toggle on key down (not held)
    if (currentF3State && !lastF3State) {
        visible = !visible;
    }

    lastF3State = currentF3State;
}

void DebugHUD::update(float deltaTime) {
    if (visible) {
        playerBounds = player->getPosition();
        playerAngle = player->getAngle();
        playerVelocity = player->getVelocity();
    }
}

void DebugHUD::render() {
    if (!visible) return;

    std::string playerXText = "Position X: " + std::to_string(playerBounds.x); 
    std::string playerYText = "Position Y: " + std::to_string(playerBounds.y); 
    std::string playerAngleText = "Angle: " + std::to_string(playerAngle);
    std::string playerXVelocity = "Velocity X: " + std::to_string(playerVelocity.x); 
    std::string playerYVelocity = "Velocity Y: " + std::to_string(playerVelocity.y);

    font.display(playerXText, 10, SCREEN_HEIGHT - 20, 255, 255, 255, 255);
    font.display(playerXText, 10, SCREEN_HEIGHT - 20, 255, 255, 255, 255);
    font.display(playerYText, 10, SCREEN_HEIGHT - 35, 255, 255, 255, 255);
    font.display(playerAngleText, 10, SCREEN_HEIGHT - 50, 255, 255, 255, 255);
    font.display(playerXVelocity, 10, SCREEN_HEIGHT - 65, 255, 255, 255, 255);
    font.display(playerYVelocity, 10, SCREEN_HEIGHT - 80, 255, 255, 255, 255);
}