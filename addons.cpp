#include "addons.h"

void cap_framerate ( Uint32 starting_tick ){
    if ( ( 1000 / FPS ) > SDL_GetTicks() - starting_tick ){
      SDL_Delay( 1000 / FPS - ( SDL_GetTicks() - starting_tick ) );
    }
};

void initColors(SDL_Surface* gScreen){
    gPink = SDL_MapRGB(gScreen->format, 232, 111, 148);
    gRed = SDL_MapRGB(gScreen->format, 250, 0, 0);
    gBeige = SDL_MapRGB(gScreen->format, 255, 255, 115);
    gBlue = SDL_MapRGB(gScreen->format, 0, 0, 255);
    gDarkblue = SDL_MapRGB(gScreen->format, 111, 114, 120);
    gDarkgreen = SDL_MapRGB(gScreen->format, 100, 120, 100);
}

void printVectorTodos(){
  //Store all to-dos in a vector of pairs (priority, description)
  // 1: current, 2: high, 3: medium, 4: low, 5: future
  std::vector<std::pair<int, std::string>> todos = {
    {1, "REFACTOR \n input.h: refactor point as a private member with getters"},
    {1, "CLEANUP \n to avoid confusion make sure all variables are declared only either GLOBALLY or LOCALLY"},

    {2, "FEATURE \n implement a state-agnostic viewport render function and clean handleMenuState()"},
    {2, "FEATURE \n Make class members private where appropriate"},

    {3, "button.cpp: change playButton sprite on interact without spritesheets"},
    {6, "Rewrite the eventloop as a separate EventListener (mouse events) -> clickable buttons -> main menu -> submenus"},

    {6, "Separate classes from `render.h` to their own header files"},
    {6, "Is it possible to draw mouse using Sprite, SpriteGroup or Block classes"},
    {6, "was adding window_width and window_height return types to initVideo() really necessary when you made it a global/extern function"},

    {4, "BUG \n Fix clicking X not closing the game"},
    {4, "Add listeners for ctrl+q and ctrl+w and q to exit the game"},

    {6, "FEATURE \n test if `double delta` timer from main.cpp can be combined with cap_framerate()"},
    {6, "Currently all header files that require SDL libs source them through `render.h` -> `main.h`. \n Delete this superfluous step and directly source the SDL libs through main.h.\n For the affected header files this will require adding includes for the standard libs that are also in render.h (as of now just iostream and vector)."},
    {6, "FEATURE \n implement gl_renderer.cpp (and vk_renderer.cpp) for the SDL_Render stuff and comments"},
    {5, "physics.h: finish the empty classes"},
    {5, "FEATURE \n physics.h: vect2 to move sprites"},
    {6, "Refactor everything into OOP, with base classes and subclasses"},
    {5, "FEATURE \n button.cpp: implement spritesheet capabilities"},
  };

  std::cout << "### Printing The Autosorted TODO List... ###" << std::endl;

  //print todos by priority
  //for (int priority = 1; priority <= 6; ++priority) {
  for (int priority = 6; priority >= 1; --priority) {
    for (const auto& todo : todos) {
      if (todo.first == priority) {
        std::cout << priority << ". " << todo.second << std::endl;
      }
    }
  }
}

#if 0
void Addons::skeletonVoid(){
  std::cout << "Makefile Example\n";
}

void SCALER( ){

    Scaler funkIn;
    int funkOut;

    //std::cin >> "" >> funkIn; // TODO

    funkOut = funkIn.scale();
    

    printf("scale(%d)=%d\n",funkIn,funkOut); //TODO muuta c++ std::cout
}

#endif
