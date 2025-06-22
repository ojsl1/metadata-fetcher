#include "character.h"


Character::Character(const std::string &spriteName, int x, int y, int w, int h,
                     const char *spritesheetPath, SDL_Rect spriteRect, int totalFrames)
  : Sprite(spriteName, x, y, w, h, spritesheetPath, spriteRect), // calling constructs from base class
    currentState(AnimationState::IDLE), currentFrame(0), lastUpdate(0), speed(2) {

  /*
   * struct AnimationData
  int startX;             // x pos in spritesheet
  int startY;             // y pos in spritesheet
  int frameWidth;
  int frameHeight;
  int frameCount;
  Uint32 animationSpeed;  // ms per frame
  int framePadding;       // padding between frames(1px)
  */
  animations = {
    {AnimationState::INTRO,             {1,0,0,0,0,400,1}},
    {AnimationState::INTRO_A,           {1,0,0,0,0,400,1}},
    {AnimationState::INTRO_B,           {1,0,0,0,0,400,1}},
    {AnimationState::TURN,              {1,0,0,0,0,400,1}},
    {AnimationState::INTRO_KOISHI,      {1,0,0,0,0,400,1}},
    {AnimationState::IDLE,              {1,648,192,192,15,400,1}},
    {AnimationState::MOVE_FORWARD,      {1,882,224,192,5,400,1}},
    {AnimationState::MOVE_BACK,         {1,0,0,0,0,400,1}},
    {AnimationState::DASH_FORWARD,      {1,0,0,0,0,400,1}},
    {AnimationState::DASH_BACKWARD,     {1,0,0,0,0,400,1}},
    {AnimationState::MOVE_DOWN_UP,      {1,0,0,0,0,400,1}},
    {AnimationState::ATTACK_A,          {1,0,0,0,0,400,1}},
    {AnimationState::DASH_ATTACK_A,     {1,0,0,0,0,400,1}},
    {AnimationState::ATTACK_B,          {1,0,0,0,0,400,1}},
    {AnimationState::DASH_ATTACK_B,     {1,0,0,0,0,400,1}},
    {AnimationState::ATTACK_UP_B,       {1,4344,288,320,11,400,1}},
    {AnimationState::ATTACK_FORWARD_B,  {1,0,0,0,0,400,1}},
    {AnimationState::ATTACK_DOWN_B,     {1,0,0,0,0,400,1}},
    {AnimationState::MAGIC_ATTACK_A,    {1,0,0,0,0,400,1}},
    {AnimationState::MAGIC_ATTACK_B,    {1,0,0,0,0,400,1}},
    {AnimationState::SPECIAL_ATTACK_A,  {1,0,0,0,0,400,1}},
    {AnimationState::SPECIAL_ATTACK_B,  {1,0,0,0,0,400,1}},
    {AnimationState::SPECIAL_ATTACK_C,  {1,0,0,0,0,400,1}},
    {AnimationState::SPECIAL_ATTACK_D,  {1,0,0,0,0,400,1}},
    {AnimationState::SPECIAL_ATTACK_E,  {1,0,0,0,0,400,1}},
    {AnimationState::SPECIAL_ATTACK_F,  {1,0,0,0,0,400,1}},
    {AnimationState::GRAB,							{1,0,0,0,0,400,1}},
    {AnimationState::MISS,							{1,0,0,0,0,400,1}},
    {AnimationState::HIT,								{1,0,0,0,0,400,1}},
    {AnimationState::SPELL_CALL,				{1,0,0,0,0,400,1}},
    {AnimationState::SPELL_A,						{1,0,0,0,0,400,1}},
    {AnimationState::SPELL_B,						{1,0,0,0,0,400,1}},
    {AnimationState::SPELL_B2,					{1,0,0,0,0,400,1}},
    {AnimationState::SPELL_B3,					{1,0,0,0,0,400,1}},
    {AnimationState::LAST_WORD,					{1,0,0,0,0,400,1}},
    {AnimationState::LAST_WORD_B,				{1,0,0,0,0,400,1}},
    {AnimationState::GUARD,							{1,0,0,0,0,400,1}},
    {AnimationState::MAGIC_GUARD,				{1,0,0,0,0,400,1}},
    {AnimationState::GUARD_BREAK,				{1,0,0,0,0,400,1}},
    {AnimationState::DIZZY,							{1,0,0,0,0,400,1}},
    {AnimationState::HIT_LOW,						{1,0,0,0,0,400,1}},
    {AnimationState::HIT_HIGH,					{1,0,0,0,0,400,1}},
    {AnimationState::WALL_BOUNCE,				{1,0,0,0,0,400,1}},
    {AnimationState::SPINNING,					{1,0,0,0,0,400,1}},
    {AnimationState::DOWNED,						{1,0,0,0,0,400,1}},
    {AnimationState::WIN_POSE_A,				{1,0,0,0,0,400,1}},
    {AnimationState::WIN_POSE_B,				{1,0,0,0,0,400,1}},
    {AnimationState::TIME_OVER,					{1,0,0,0,0,400,1}},
  };

  //Convert spritesheet to 32bpp
  
  if (spritesheet) {
    SDL_Surface* optimizedSurface = SDL_ConvertSurfaceFormat(spritesheet, SDL_PIXELFORMAT_RGBA32, 0);
    if (!optimizedSurface) {
      //24bpp images dont have an Amask
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

  // DEBUG: iterate over the unordered_map animation states and print keys of each state
  /*
  for (const auto &pair : animations) {
      std::cout << "State: " << static_cast<int>(pair.first)
                << ", Speed: " << pair.second.animationSpeed
                << ", FrameCount: " << pair.second.frameCount
                << std::endl;
  }
  */
}

void Character::update(double deltaTime){
  AnimationData &anim = animations[currentState];

  // Check if the animation state has changed from previous frame
  if (currentState != lastState) {
    srcRect.y = anim.startY;
    srcRect.w = anim.frameWidth;
    srcRect.h = anim.frameHeight;
    currentFrame = 0;          // Reset the frame to 0 when animation changes
    lastState = currentState;  // Update lastState for future comparisons
  }
  
  // Update the animation timer
  lastUpdate += deltaTime;

  //Animation playback logic
  //TODO couldnt this be in Character::playAnimation or do we have to check for it every frame(?)
  if (animationPlaying){
      // TODO the example has `static_cast<int>(deltaTime);` instead, why
      animationTimer -= deltaTime; //countdown timer for playing a temporary animation
      if (animationTimer <= 0){
          currentState = oldState; //revert to previous state
          animationPlaying = false; //end temporary animation
      }
  }

  //Animation frame logic, ensure no division by zero
  if (lastUpdate >= anim.animationSpeed && anim.frameCount > 0){
      //loop through frames
      currentFrame = (currentFrame + 1) % anim.frameCount;
      lastUpdate = 0; //reset the timer
  }

  // TODO WHAT: if this is inside above "Animation Frame Logic" loop and before lastUpdate=0; the frames become "unsynchronized", why(?)
  // TODO WHY: why do so many docs have the below inside above Animation Frame Logic loop(?)
  //
  //Compute srcRect.x based on the current frame, taking padding(s) into account so animation doesnt become unsynchronized.
  srcRect.x = (anim.frameWidth * currentFrame) + (anim.framePadding * (currentFrame + 1));
}

void Character::move(int dx, int dy){
  dRectSprite.x += dx * speed;
  dRectSprite.y += dy * speed;
}

void Character::playAnimation(AnimationState newState, int durationMs){
  //if below is true Character::Update starts animation playback logic
  if (currentState != newState) {
      oldState = currentState;
      currentState = newState;
      animationTimer = durationMs;
      animationPlaying = true;
  }
}

void Character::Draw(SDL_Surface *gScreen){
  if (!spritesheet || !gScreen) return;
  SDL_BlitSurface(spritesheet, &srcRect, gScreen, &dRectSprite);
}
