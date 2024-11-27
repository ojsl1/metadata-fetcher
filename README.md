# Foray-SDL2

## Purpose
Currently the main goals are:
- To create a pseudo-file manager that allows rendering images into a customizable (and scrollable) mosaic, compare:
    - imagemagick montage
    - yfszzx/stable-diffusion-webui-images-browser
    - bit9labs/sd-masonry
    - dolphin file manager icon view with all ui panels hidden, small label width, and max line number as 1
- To be able to easily fetch pnginfo from images by drag-n-dropping into the gui.

## Todo
- [X] OOP
    - [X] Baseclass
        - [X] RendererBase
            - [ ] renderer\_sdl\_sw
            - [ ] renderer\_sdl\_hw
            - [ ] renderer\_opengl
            - [ ] renderer\_vk
        - [X] AppObject
            - [X] Mouse
            - [X] Button
    - [ ] Polymorphism
- [ ] OpenGLRenderer subclass
    - [ ] Replace all SDL\_Rects with vertex buffers (VBO) and vertex array objects (VAO)
        - [ ] If some SDL\_Rects were texctured, load textures into opengl, bind them and adjust texture coords
        - [ ] If some SDL\_Rects were colored, pass color data into a separate buffer or through uniform variables within your shader
    - [ ] Simple vertex and fragment shaders
        - [ ] For positioning and transformations pass a matrix to the shader
        - [ ] Pass a matrix to the shader for position and transformation operations (scaling/translating/rotating)
    - [ ] Reduce number of draw calls when calling lots of rects by batching them into a single vbo
        - [ ] Dynamically update the buffer or use instancing for rendering multiple rects in a single call
- [ ] Use Vect2 for sprite movement
- [ ] RAII
- [ ] SFINAE
- [ ] ODR and inline keywords
- [ ] Optimize memory of classes with union keywords
- [ ] Currently all header files that require SDL libs source them through "render.h" -> "main.h". Delete this superfluous step and directly source SDL libs through main.h. For the affected header files this will require adding includes for the standard libs that are also in render.h (as of now just iostream and vector).
- [ ] Gallery: Iterate over filenames image000...image991 and draw them one by one with some delay between
- [ ] SDL2 -> SDL3

## Fix memos

#### mouse.drawMouse( screen ) (2024)
1. changed variable from mouse.mouseImage to screen
2. moved drawing logic from render.cpp to main loop
3. made *window and *screen into global variables

#### Mouse size (21.06.2024)
0. For now copied mouse draw logic to main.cpp
1. Created separate surface for the scaled image with SDL_CreateRGBSurface
2. Also created a destination rectangle for the scaled image so we can draw to it
3. Using SDL_BlitScaled() scaled the raw image to the scaled surface
4. Finally with SDL_BlitSurface() drew the scaled surface to the destination rectangle.

#### Prepended global variables with g (22.06.2024)
0. gWindow, gScreen, (gFont)
1. gPink, gBlue etc.

#### RendererBase overscoping (26.11.2024)
Instead of attempting to abstract all the different renderers into subclasses call by call at the same time, reduce scope by first abstracting just the currently used one.


- Namespace Usage: Encapsulate related variables within namespaces to avoid global scope and reduce the need for prefixes like g.

```c++
namespace Engine {
    namespace Core {
        int frameCount = 0;
        bool isRunning = false;
    }

    namespace Graphics {
        SDL_Window* gWindow = nullptr;
        SDL_Renderer* gRenderer = nullptr;
        SDL_Texture* gTexture = nullptr;
        float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
        int screenWidth = 800;
        int screenHeight = 600;
    }
}
```

```c++
int main() {
    using namespace Engine;

    Core::isRunning = true;

    while (Core::isRunning) {
        // Game loop

        // Update logic

        // Render graphics
        std::cout << "Rendering at " << Graphics::screenWidth << "x" << Graphics::screenHeight << std::endl;
    }

    return 0;
}
```

- Class Member Variables: If variables are logically tied to a class, use member variables (mWindow, mSurface) instead of global scope, adhering to encapsulation principles.
