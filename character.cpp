#include "character.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

static AnimationState stringToAnimationState(const std::string &key)
{
  static const std::unordered_map<std::string, AnimationState> lookuptable = {
    {"none", AnimationState::NONE},
    {"idle", AnimationState::IDLE},
    {"intro_koishi", AnimationState::INTRO_KOISHI},
    {"move_back", AnimationState::MOVE_BACK},
    {"move_forward", AnimationState::MOVE_FORWARD},
    {"attack_b", AnimationState::ATTACK_B},
    {"attack_up_b", AnimationState::ATTACK_UP_B},
    {"time_over", AnimationState::TIME_OVER},
    {"downed", AnimationState::DOWNED},
    // TODO .. add rest of the keys from json
  };
  auto iterator = lookuptable.find(key);
  return (iterator != lookuptable.end()) ? iterator->second : AnimationState::NONE; // fallback
}

Character::AnimMap Character::loadAnimationConfig(const std::string &filename, const std::string &characterName)
{
    nlohmann::json cfg;
    std::ifstream f(filename);
    if (!f) {
      SDL_Log("Cannot open %s", filename.c_str());
      return {};
    }
    f >> cfg;

    AnimMap result;

    if (!cfg.contains(characterName) || !cfg[characterName].is_object()) {
        SDL_Log("Animation config: character '%s' not found or invalid", characterName.c_str());

        SDL_Log("Available character keys:");
        for (auto it = cfg.begin(); it != cfg.end(); ++it)
            SDL_Log("  '%s'", it.key().c_str());
        return result;
    }else{
        SDL_Log("Animation config: character '%s' found", characterName.c_str());
    }

    const auto &charJson = cfg[characterName];
    if (!charJson.is_object()) {
      SDL_Log("Character '%s' is not a valid object", characterName.c_str());
      return result;
    }

    if (!charJson.contains("spritesheetPath") || charJson["spritesheetPath"].is_null()) {
      SDL_Log("Character '%s' is missing 'spritesheetPath'", characterName.c_str());
      SDL_Log("charJson dump: %s", charJson.dump(2).c_str());
      return result;
    }

    // common field
    const std::string sheet = charJson.at("spritesheetPath").get<std::string>();

    for (const auto &[key, val] : charJson.items()) {
      if (key == "spritesheetPath") continue; // skip metedata
      AnimationState st = stringToAnimationState(key);
      AnimationData d{};
      d.spritesheetPath = sheet;
      d.frameWidth     = val.at("frameWidth").get<int>();
      d.frameHeight    = val.at("frameHeight").get<int>();
      d.frameCount     = val.at("frameCount").get<int>();
      d.framePadding   = val.value("framePadding",      0);   // use 0 as default if missing
      d.animationSpeed = val.value("animationSpeed",    400); // 400 default if missing
      d.transient      = val.value("transient",         0);   // 0 default if missing
      d.speed          = val.value("speed",             2);   // 1 default if missing
      d.startX         = val.value("startX",            0);
      d.startY         = val.value("startY",            0);
      result[st] = d;   // map enum -> data
    }
    return result;

}

Character::Character(const std::string &spriteName, int x, int y, const AnimMap &anims)
  : Sprite(spriteName, x, y,
            anims.at(AnimationState::IDLE).frameWidth,
            anims.at(AnimationState::IDLE).frameHeight,
            anims.at(AnimationState::IDLE).spritesheetPath.c_str(),
            SDL_Rect{ anims.at(AnimationState::IDLE).startX,
                      anims.at(AnimationState::IDLE).startY,
                      anims.at(AnimationState::IDLE).frameWidth,
                      anims.at(AnimationState::IDLE).frameHeight })
    , animations(anims)
    , currentState(AnimationState::IDLE)
    , idleState(AnimationState::IDLE)
    , lastState(AnimationState::NONE)
    , currentFrame(0)
    , lastUpdate(0)
    , animationPlaying(false)
    , animationTimer(0)
{
  //Convert spritesheet to 32bpp (for consistency)
  if (spritesheet) {
    SDL_Surface* optimizedSurface = SDL_ConvertSurfaceFormat(
                                      spritesheet, SDL_PIXELFORMAT_RGBA32, 0);
    if (optimizedSurface) {
      SDL_FreeSurface(spritesheet);
      spritesheet = optimizedSurface;
    } else {
      //24bpp images dont have an Amask
      SDL_Log("Failed to convert spritesheet to 32-bit format.");
    }

    //Set colorkey (before any blitting happens!)
    if (SDL_SetColorKey(spritesheet, SDL_TRUE, SDL_MapRGB(spritesheet->format, 147, 187, 236)) != 0) {
        SDL_Log("Failed to set colorkey: %s", SDL_GetError());
    } else {
        SDL_Log("Colorkey applied successfully.");
    }
  }
}

void Character::update(double deltaTime)
{
  AnimationData &anim = animations[currentState];

  //SDL_Log("Player dstRect: w=%d h=%d x=%d y=%d", dRectSprite.w, dRectSprite.h, dRectSprite.x, dRectSprite.y);
  //SDL_Log("Animation: %d, frame: %d, src.x: %d, src.y: %d", currentState, currentFrame, srcRect.x, srcRect.y);

  // Check if the animation state has changed from previous frame
  if (currentState != lastState){

    // Calculate possible width/height change
    int deltaW = anim.frameWidth - dRectSprite.w;
    int deltaH = anim.frameHeight - dRectSprite.h;
    // shift half the difference (Assuming spritesheet uses center anchor)
    // BUG 1. before updating dRectSprite.w/h = anim.frameWidth/Height, the w/h values get clipped when playAnimation() is called out of bounds
    // BUG 2. the values getting clipped when out of bounds also affect below shifting leading to below
    // BUG 3. every sequential playAnimation() moves the sprite towards the origo untill the sprite is completely within bounds, ie. when the w/h values are what they originally were
    dRectSprite.x -= deltaW / 2;
    dRectSprite.y -= deltaH / 2;

    srcRect.x = anim.startX;
    srcRect.y = anim.startY;
    srcRect.w = anim.frameWidth;
    srcRect.h = anim.frameHeight;

    dRectSprite.w = anim.frameWidth;
    dRectSprite.h = anim.frameHeight;

    // TODO why is below resetting needed, it still works without it no? is this causing the random block frames?
    //currentFrame = 0;          // Reset the frame to 0 when animation changes

    lastState = currentState;  // change lastState back for future comparisons
  }
  
  // Update the animation timer
  lastUpdate += deltaTime;

  //Animation playback logic
  //TODO couldnt this be in Character::playAnimation or do we have to check for it every frame(?)
  if (animationPlaying){
      // TODO the example has `static_cast<int>(deltaTime);` instead, why
      animationTimer -= deltaTime; //countdown timer for playing a temporary animation
      if (animationTimer <= 0){
          currentState = idleState; //revert to idle state
          animationPlaying = false; //end temporary animation
      }
  }

  //Animation Frame Logic, ensure no division by zero
  if (lastUpdate >= anim.animationSpeed && anim.frameCount > 0){
      //loop through frames
      currentFrame = (currentFrame + 1) % anim.frameCount;
      lastUpdate = 0; //reset the timer
  }

  // TODO Test AFTER implementing anchoring if below method (old) still creates desync
  // TODO frames desync if this is inside above "Animation Frame Logic" loop and before lastUpdate=0; the frames become "unsynchronized"
  // TODO why do so many docs have the below inside above Animation Frame Logic loop(?)

  // OLD METHOD
  //Compute srcRect.x based on the current frame, taking padding(s) into account so animation doesnt desync
  //srcRect.x = (anim.frameWidth * currentFrame) + (anim.framePadding * (currentFrame + 1));

  // NEW METHOD
  //Compute srcRect.x based on the current frame, taking between-frame padding(s) into account so animation doesnt desync
  /* @param anim.startX, add to keep the horizontal offset
   * @param anim.frameWidth, multiply by elapsed frame amount
   * @param anim.framePadding, add multiples of frames
   */
  srcRect.x = anim.startX // TODO this term fixes racer but leads to marisa padding desync
                          // because racer doesnt have leftPadding?
            + anim.frameWidth  *  currentFrame
            + (anim.framePadding * (currentFrame + 1));
}

void Character::move(int dx, int dy)
{
  AnimationData &anim = animations[currentState];
  dRectSprite.x += dx * anim.speed;
  dRectSprite.y += dy * anim.speed;
}

void Character::playAnimation(AnimationState newState, int durationMs)
{
  //if below is true Character::Update starts animation playback logic
  if (currentState != newState) {
      currentState = newState;
      animationTimer = durationMs;
      animationPlaying = true;
  }
}

void Character::Draw(AppContext gApp)
{
  if (!spritesheet || !gApp.screen) return;
  SDL_BlitSurface(spritesheet, &srcRect, gApp.screen, &dRectSprite);
}
