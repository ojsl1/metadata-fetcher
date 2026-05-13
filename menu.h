#ifndef MENU_H
#define MENU_H

#include "render.h"
#include <optional>
//#include <functional>
//#include <string>
//#include <vector>
//#include <optional>

class AppStateBase{
public:
  enum class Scene { MAIN_MENU, MINIGAME, PAUSE, EXIT };

  virtual ~AppStateBase() = default;               // inline dtor for anchoring vtable
  //virtual void render() = 0;          // pure virtual -> abstract
  virtual void handleEvent(const SDL_Event &e) = 0;
};

class Menu : public AppStateBase{
public:
  struct Item {
    std::string id;                   // play, tests, pause, mute, exit, etc.
    Sprite *sprite = nullptr;         // TODO owned elsewhere or by this class  (see below note)
    // same as sprite: "void Sprite::SetToggleCallback(std::function<void(bool)> callback)"
    std::function<void()> onActivate; // action when selected
    bool selectable = true;           // mon-selectable labels etc.
  };

  Menu(IRenderer &ren, Mouse &mouse, bool v);
  
  void setVisible(bool v) { visible_ = v; }
  bool isVisible() const { return visible_; }

  void setFrame(Sprite &frame);
  Sprite *frame_ = nullptr;
  void setBackground(Sprite &background);
  Sprite *background_ = nullptr;

  // Static menu setup (sprite pointers not owned).
  void addItem(const std::string &id, Sprite* sprite, std::function<void()> onActivate, bool selectable = true);

  // Store Sprite by value (-> menu owns a copy), returns a pointer for references
  Sprite* addItemByValue(const std::string &id, const Sprite& sprite, std::function<void()> onActivate, bool selectable = true);

  // Core loop hooks
  void handleEvent(const SDL_Event &e) override;
  void Render();
  void update(float deltatime); // TODO parity

  bool setSelectedById(const std::string &id);
  std::optional<std::string> selectedId() const;
private:
  struct OwnedSprite {
    // Storage for value-owned sprites.
    std::string id;
    Sprite *ptr = nullptr; // points into owned_.storage after emplace_back
  };

  IRenderer *renderer_;
  Mouse *mouse_;
  bool visible_;


  std::vector<Item> items_;
  std::vector<Sprite> ownedStorage_; // when addItemByValue is used
  int selected_ = -1;
  bool wrapNav_ = true;

  void selectNext(int dir); // dir = +1 or -1
  void activateSelected();
  void updateMouseSelection(int mouseX, int mouseY);
  bool isInside(const Sprite &s, int x, int y) const; // fallback when not using Mouse::DetectCollisions
  SDL_Rect spriteRect(const Sprite &s) const;         // TODO adapt for old sprite class

};

#endif // MENU_H
