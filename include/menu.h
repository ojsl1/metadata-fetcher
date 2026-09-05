#ifndef MENU_H
#define MENU_H

#include "render.h"
#include <optional>

struct MainMenuAssets {
  SurfaceSprite *spriteExit;
  SurfaceSprite *spriteTests;
  SurfaceSprite *spriteDrop;
  SurfaceSprite *spriteMute;
  SurfaceSprite *spritePause;
  SurfaceSprite *spriteBorder;
  SurfaceSprite *spriteFrame;
  SurfaceSprite *spriteBg;
  Character *player;
};

struct MinigameAssets {
  SurfaceSprite *spritePause;
  SurfaceSprite *spritePlaceholder;
  Character *player2;
};

class Menu {
public:
  enum class Scene { MAIN_MENU, MINIGAME, PAUSE, EXIT };

  struct Item {
    std::string id;                   // play, tests, pause, mute, exit, etc.
    SurfaceSprite *sprite = nullptr;         // TODO owned elsewhere or by this class  (see below note)
    // same as sprite: "void Sprite::SetToggleCallback(std::function<void(bool)> callback)"
    std::function<void()> onActivate; // action when selected
    bool selectable = true;           // mon-selectable labels etc.
  };

  Menu(IRenderer &ren, Mouse &mouse, bool v);
  
  void setVisible(bool v) { visible_ = v; }
  bool isVisible() const { return visible_; }

  void setFrame(SurfaceSprite &frame);
  SurfaceSprite *frame_ = nullptr;
  void setBackground(SurfaceSprite &background);
  SurfaceSprite *background_ = nullptr;

  // Static menu setup (sprite pointers not owned).
  void addItem(const std::string &id, SurfaceSprite* sprite, std::function<void()> onActivate, bool selectable = true);

  // Store Sprite by value (-> menu owns a copy), returns a pointer for references
  SurfaceSprite* addItemByValue(const std::string &id, const SurfaceSprite& sprite, std::function<void()> onActivate, bool selectable = true);

  // Core loop hooks
  void handleEvent(const SDL_Event &e);
  void Render();
  void update(float deltatime) const; // TODO parity

  bool setSelectedById(const std::string &id);
  std::optional<std::string> selectedId() const;
private:
  struct OwnedSprite {
    // Storage for value-owned sprites.
    std::string id;
    SurfaceSprite *ptr = nullptr; // points into owned_.storage after emplace_back
  };

  IRenderer *renderer_;
  Mouse *mouse_;
  bool visible_;


  std::vector<Item> items_;
  std::vector<SurfaceSprite> ownedStorage_; // when addItemByValue is used
  int selected_ = -1;
  bool wrapNav_ = true;

  void selectNext(int dir); // dir = +1 or -1
  void activateSelected();
  void updateMouseSelection(int mouseX, int mouseY);
  bool isInside(const SurfaceSprite &s, int x, int y) const; // fallback when not using Mouse::DetectCollisions
  SDL_Rect spriteRect(const SurfaceSprite &s) const;         // TODO adapt for old sprite class
};

class SceneComposer {
public:
  //void composeScene( Mouse &mouse, struct &assets, IRenderer &ren );
  static void composeMainMenu( Mouse &mouse, const MainMenuAssets &assets, IRenderer &ren );
  static void composeMinigame( Mouse &mouse, const MinigameAssets &assets, IRenderer &ren );

  SceneComposer() = default;
  ~SceneComposer() = default;

};

#endif // MENU_H
