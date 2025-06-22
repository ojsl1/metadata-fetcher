#ifndef CHARACTER_H
#define CHARACTER_H

#include "main.h" // globals
#include "sprite.h"

enum class AnimationState {
  INTRO,
  INTRO_A,
  INTRO_B,
  TURN,
  INTRO_KOISHI,
  IDLE,
  MOVE_FORWARD,
  MOVE_BACK,
  DASH_FORWARD,
  DASH_BACKWARD,
  MOVE_DOWN_UP,
  ATTACK_A,
  DASH_ATTACK_A,
  ATTACK_B,
  DASH_ATTACK_B,
  ATTACK_UP_B,
  ATTACK_FORWARD_B,
  ATTACK_DOWN_B,
  MAGIC_ATTACK_A,
  MAGIC_ATTACK_B,
  SPECIAL_ATTACK_A,
  SPECIAL_ATTACK_B,
  SPECIAL_ATTACK_C,
  SPECIAL_ATTACK_D,
  SPECIAL_ATTACK_E,
  SPECIAL_ATTACK_F,
  GRAB,
  MISS,
  HIT,
  SPELL_CALL,
  SPELL_A,
  SPELL_B,
  SPELL_B2,
  SPELL_B3,
  LAST_WORD,
  LAST_WORD_B,
  GUARD,
  MAGIC_GUARD,
  GUARD_BREAK,
  DIZZY,
  HIT_LOW,
  HIT_HIGH,
  WALL_BOUNCE,
  SPINNING,
  DOWNED,
  WIN_POSE_A,
  WIN_POSE_B,
  TIME_OVER,
};

struct AnimationData {
  int startX;
  int startY;
  int frameWidth;
  int frameHeight;
  int frameCount;
  Uint32 animationSpeed;
  int framePadding;
};

class Character : public Sprite{
private:
  AnimationData anim;
  AnimationState currentState;
  AnimationState oldState;
  AnimationState newState;
  AnimationState lastState;
  int currentFrame;             // Current animation frame
  Uint32 lastUpdate;            // time since last frame update
  SDL_Rect srcRect;
  int speed;                    // movement speed
  bool animationPlaying = false;
  int animationTimer = 0;

public:
  std::unordered_map<AnimationState, AnimationData> animations;

  Character(const std::string &spriteName, int x, int y, int w, int h,
            const char *spritesheetPath, SDL_Rect spriteRect, int totalFrames);

  void update(double deltaTime);
  void move(int dx, int dy);
  void playAnimation(AnimationState, int);
  void Draw(SDL_Surface *gScreen) override;
};

#endif // CHARACTER_H
