#include "main.h"
#include "menu.h"
#include "input.h"
#include "sprite.h"

#include <cassert>
#include <optional>
#include <algorithm>

Menu::Menu(IRenderer &ren, Mouse &mouse, bool visible)
  : renderer_(&ren), mouse_(&mouse), visible_(false)
{
  assert(renderer_);
  assert(mouse_);
}

void Menu::setBackground(Sprite &background)
{
  background_ = &background;
}
void Menu::setFrame(Sprite &frame)
{
  frame_ = &frame;
}

void Menu::addItem(const std::string &id,
                            Sprite* sprite,
                            std::function<void()> onActivate,
                            bool selectable)
{
  Item it;
  it.id = id;
  it.sprite = sprite;
  it.onActivate = std::move(onActivate);
  it.selectable = selectable;
  items_.push_back(std::move(it));

  if (selected_ < 0 && selectable) {
      selected_ = static_cast<int>(items_.size()) - 1;
  }
}

// TODO read std::move and push_back
Sprite* Menu::addItemByValue(const std::string& id,
                                      const Sprite& sprite,
                                      std::function<void()> onActivate,
                                      bool selectable)
{
  ownedStorage_.push_back(sprite);
  Sprite* p = &ownedStorage_.back();
  addItem(id, p, std::move(onActivate), selectable);
  return p;
}

// TODO
bool Menu::setSelectedById(const std::string& id)
{
  for (int i = 0; i < static_cast<int>(items_.size()); ++i) {
    if (items_[i].selectable && items_[i].id == id){
      selected_ = i;
      return true;
    }
  }
  return false;
}

// TODO std::nullopt and std::optional
std::optional<std::string> Menu::selectedId() const
{
  if (selected_ >= 0 && selected_ < static_cast<int>(items_.size()))
    return items_[selected_].id;
  return std::nullopt;
}

void Menu::handleEvent(const SDL_Event& e)
{
    if (!visible_) return;

    switch (e.type) {
        case SDL_KEYDOWN: {
            const SDL_Keysym ks = e.key.keysym;
            switch (ks.scancode) {
                case SDL_SCANCODE_UP:
                case SDL_SCANCODE_W:
                    selectNext(-1);
                    break;
                case SDL_SCANCODE_DOWN:
                case SDL_SCANCODE_S:
                    selectNext(+1);
                    break;
                case SDL_SCANCODE_RETURN:
                case SDL_SCANCODE_SPACE:
                    activateSelected();
                    break;
                case SDL_SCANCODE_ESCAPE:
                    // pseu: if (!setSelectedById("back") && !setSelectedById("exit")){
                    // no-op if neither exists}
                    activateSelected();
                    break;
                default:
                    break;
            }
            break;
        }
        case SDL_MOUSEMOTION: {
            // TODO adapt to class Mouse as it already tracks x/y.
            int mx = e.motion.x;
            int my = e.motion.y;
            updateMouseSelection(mx, my);
            break;
        }
        case SDL_MOUSEBUTTONDOWN: {
            if (e.button.button == SDL_BUTTON_LEFT) {
                // ensure hovered item is selected before activation
                int mx = e.button.x;
                int my = e.button.y;
                updateMouseSelection(mx, my);
                activateSelected();
            }
            break;
        }
        default:
            break;
    }
}

void Menu::update(float /*deltatime*/)
{
  if (!visible_) return;

  // TODO adapt to using pos from class Mouse position
  // int mx = mouse_->x; int my = mouse_->y; updateMouseSelection(mx, my);
  // This allows hover-follow even without events (e.g., when events are throttled).
}

void Menu::Render()
{
  if (!visible_){
    //SDL_Log("Not drawing menu, visible set to null");
    return;
  };

  if (background_) {
      //std::cout << background_ << " background_ from Menu::render()" << '\n';
      background_->DrawScaled(gApp);
  }else{ SDL_Log("background_ was null"); };

  if (frame_) {
      //std::cout << frame_ << " frame_ from Menu::render()" << '\n';
      frame_->DrawScaled(gApp);
  }else{ SDL_Log("frame_ was null"); };

  // TODO read continue
  // Draw by iterating
  for (int i = 0; i < static_cast<int>(items_.size()); ++i) {
      Item& it = items_[i];
      if (!it.sprite) continue;
      it.sprite->Draw(gApp);
  }
}

void Menu::selectNext(int dir)
{
  if (items_.empty()) {
      selected_ = -1;
      return;
  }
  if (selected_ < 0) {
      // Picks first selectable
      for (int i = 0; i < (int)items_.size(); ++i) {
          if (items_[i].selectable) { selected_ = i; return; }
      }
      return; // if no selectable items
  }

  int i = selected_;
  const int n = static_cast<int>(items_.size());
  for (int step = 0; step < n; ++step) {
      i += dir;
      if (wrapNav_) {
          if (i < 0) i = n - 1;
          if (i >= n) i = 0;
      } else {
          i = std::clamp(i, 0, n - 1);
      }
      if (items_[i].selectable) {
          selected_ = i;
          return;
      }
      if (!wrapNav_ && (i == 0 || i == n - 1)) {
        // hit bound without finding selectable
        break;
      }
  }
}

void Menu::activateSelected()
{
  if (selected_ < 0 || selected_ >= static_cast<int>(items_.size())) return;
  Item& it = items_[selected_];
  if (!it.selectable) return;
  if (it.onActivate) it.onActivate();
}

void Menu::updateMouseSelection(int mouseX, int mouseY)
{
  // TODO Adapt to class Sprite/Mouse DetectCollisions
  // pseu: if (DetectCollisions(mouse_, *it.sprite)) { selected_ = i; }

  for (int i = 0; i < static_cast<int>(items_.size()); ++i) {
      Item& it = items_[i];
      if (!it.selectable || !it.sprite) continue;
      if (isInside(*it.sprite, mouseX, mouseY)) {
          selected_ = i;
          return;
      }
  }
}

bool Menu::isInside(const Sprite& s, int x, int y) const
{
  SDL_Rect r = spriteRect(s);
  return x >= r.x && x < (r.x + r.w) && y >= r.y && y < (r.y + r.h);
}

SDL_Rect Menu::spriteRect(const Sprite& s) const
{
  // TODO Adapt to class Sprite
  // use dstRect from class Sprite or TODO the x/y/w/h accessors
  // pseu:
  SDL_Rect r{};
  // r.x = s.GetX();
  // r.y = s.GetY();
  // r.w = s.GetW();
  // r.h = s.GetH();
  return r;
}
