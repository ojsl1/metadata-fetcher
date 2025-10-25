#ifndef CHARACTER_H
#define CHARACTER_H

#include "sprite.h"
#include <string>
#include <unordered_map>

enum class AnimationState {
  NONE,
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

class Character : public Sprite
{
public:
  struct AnimationData {
    int frameWidth;
    int frameHeight;
    int frameCount;
    int framePadding;
    Uint32 animationSpeed;
    double speed; // pixels/second
    int startX;
    int startY;
    std::string spritesheetPath;
    bool transient; //for differentiating animations that use multiple different states
  };

  using AnimMap = std::unordered_map<AnimationState, AnimationData>;

  static AnimMap loadAnimationConfig(const std::string &fileName, const std::string &characterName);

  Character(const std::string &spriteName, int x, int y, const AnimMap &anims);

  void update(double deltaTime);
  void move(int dx, int dy);
  void playAnimation(AnimationState, int);
  void Draw(AppContext gApp) override;

private:
  AnimMap animations;
  AnimationData anim;// is this OLD pre-AnimMap CRUFT still needed?
  AnimationState currentState = AnimationState::IDLE;
  AnimationState idleState, newState, lastState;
  int currentFrame;             // Current animation frame
  Uint32 lastUpdate;            // time since last frame update
  bool animationPlaying = false;
  int animationTimer = 0;
};

#endif // CHARACTER_H
