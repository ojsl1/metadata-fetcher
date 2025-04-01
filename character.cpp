#include "character.h"

Character::Character(const std::string &spriteName, int x, int y, int w, int h,
                     const char *spritesheetPath, SDL_Rect spriteRect, int totalFrames)
  : Sprite(spriteName, x, y, w, h, spritesheetPath, spriteRect), // calling constructs from base class
    frameWidth(w), frameHeight(h), frameCount(totalFrames), framePadding(1),
    currentFrame(0), animationSpeed(400), lastUpdate(0), speed(2) {

  srcRect = { spriteRect.x, spriteRect.y, frameWidth, frameHeight };

  //Convert spritesheet to 32bpp
  if (spritesheet) {
    SDL_Surface* optimizedSurface = SDL_ConvertSurfaceFormat(spritesheet, SDL_PIXELFORMAT_RGBA32, 0);
    if (!optimizedSurface) {
      SDL_Log("Failed to convert spritesheet to 32-bit format: %s", SDL_GetError());
    } else {
      SDL_FreeSurface(spritesheet);
      spritesheet = optimizedSurface;
      SDL_Log("Successfully converted spritesheet to 32-bit format.");
    }
  }

  //DEBUG: Verify the conversion worked
  SDL_Log("Spritesheet BPP: %d", spritesheet->format->BitsPerPixel);
  SDL_Log("Spritesheet Pixel Format: Rmask: 0x%X, Gmask: 0x%X, Bmask: 0x%X, Amask: 0x%X",
          spritesheet->format->Rmask,
          spritesheet->format->Gmask,
          spritesheet->format->Bmask,
          spritesheet->format->Amask);

  //Set colorkey (before any blitting happens!)
  if (SDL_SetColorKey(spritesheet, SDL_TRUE, SDL_MapRGB(spritesheet->format, 147, 187, 236)) != 0) {
      SDL_Log("Failed to set colorkey: %s", SDL_GetError());
  } else {
      SDL_Log("Colorkey applied successfully.");
  }


  //DEBUG: Verify colorkey was set
  Uint32 key;
  if (SDL_GetColorKey(spritesheet, &key) == 0) {
      SDL_Log("Verified Colorkey: 0x%X", key);
  } else {
      SDL_Log("Colorkey verification failed: %s", SDL_GetError());
  }

  /*
  //DEBUG: Analyze sprite pixels
  SDL_LockSurface(spritesheet);
  Uint32* pixels = (Uint32*)spritesheet->pixels;
  int pitch = spritesheet->pitch / 4; // 4 bytes per pixel for 32-bit
  
  std::unordered_map<Uint32, int> colorCount;
  bool foundColorkey = false;
  
  for (int y = 0; y < spritesheet->h; ++y) {
      for (int x = 0; x < spritesheet->w; ++x) {
          Uint32 pixel = pixels[y * pitch + x];
          
          // Count occurrences of each color
          colorCount[pixel]++;
          
          // Check if pixel matches colorkey
          if (pixel == key && !foundColorkey) {
              SDL_Log("Colorkey found at (%d, %d)", x, y);
              foundColorkey = true;
          }
      }
  }
  
  if (!foundColorkey) {
      SDL_Log("No pixels match the colorkey!");
  }
  
  //Inspect first 10 pixels for format information
  for (int i = 0; i < std::min(10, spritesheet->w * spritesheet->h); ++i) {
      Uint32 pixel = pixels[i];
      Uint8 r, g, b;
      SDL_GetRGB(pixel, spritesheet->format, &r, &g, &b);
      SDL_Log("Pixel %d: R=%d, G=%d, B=%d, Hex: 0x%X", i, r, g, b, pixel);
  }
  
  SDL_UnlockSurface(spritesheet);
  
  //Find the most common color ie. background
  Uint32 mostCommonColor = 0;
  int maxCount = 0;
  for (const auto& pair : colorCount) {
      if (pair.second > maxCount) {
          maxCount = pair.second;
          mostCommonColor = pair.first;
      }
  }
  
  Uint8 r, g, b;
  SDL_GetRGB(mostCommonColor, spritesheet->format, &r, &g, &b);
  SDL_Log("Most common color (likely background): R=%d, G=%d, B=%d, Hex: 0x%X", r, g, b, mostCommonColor);
  */

}

void Character::update(double deltaTime){
  lastUpdate += deltaTime;
  if (lastUpdate >= animationSpeed){
      //loop animation frames
      currentFrame = (currentFrame + 1) % frameCount;
      //shift to next frame, taking padding into account
      srcRect.x = (frameWidth * currentFrame) + (framePadding * (currentFrame + 1));
      lastUpdate = 0;
  }
}

void Character::move(int dx, int dy){
  dRectSprite.x += dx * speed;
  dRectSprite.y += dy * speed;
}

void Character::slash(){
  std::cout << "start" << std::endl;
  
  int originalY = dRectSprite.y; // Save the original position

  // Assuming attack animation is on the next row in the sprite sheet
  int attackRowHeight = srcRect.frameHeight; 
  dRectSprite.y += attackRowHeight;

  // Display the attack animation (you can modify this for frame-based updates)
  SDL_BlitSurface(spritesheet, &srcRect, gScreen, &dRectSprite);

  // Reset srcRect.y after a short delay (can be handled with a timer in game loop)
  SDL_Delay(100);  // Adjust delay as needed
  dRectSprite.y = originalY; // Restore original position
  std::cout << "end" << std::endl;
}

void Character::Draw(SDL_Surface *gScreen){
  if (!spritesheet || !gScreen) return;
  SDL_BlitSurface(spritesheet, &srcRect, gScreen, &dRectSprite);
}
