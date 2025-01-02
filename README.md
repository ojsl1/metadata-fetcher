# Metadata Fetcher
This is a GUI tool for quickly fetching text metadata from PNG images, just dragndrop the image into the GUI and the text will get printed into the console (WIP eventually into the gui). It was mainly made for work with Stable Diffusion.

## Building and running
This hasn't been and wont be tested on non-Linux systems.

You need at least: `sdl2`, `sdl2_image`, `sdl2_mixer`.

```
$ git clone https://github.com/ojsl1/metadata-fetcher.git
$ cd metadata-fetcher
$ make
```

## Todo
- [ ] Rewrite using SDL_Renderer and SDL_Texture.
- [ ] Render with OpenGL instead of SDL
- [ ] Abstract pure virtual class methods for runtime polymorphism.
- [ ] SDL2 -> SDL3
- [ ] Rendering multiple images into a scrollable mosaic, compare:
    - imagemagick montage
    - yfszzx/stable-diffusion-webui-images-browser
    - bit9labs/sd-masonry
    - dolphin file manager icon view with all ui panels hidden, small label width, and max line number as 1

## Fix memos
#### RendererBase overscoping (26.11.2024)
Instead of attempting to abstract all the different renderers into subclasses call by call at the same time, reduce scope by first abstracting just the currently used one.

#### Namespace Usage
Encapsulate related variables within namespaces to avoid global scope and reduce the need for g prefixes.

```c++
namespace Engine {
    namespace Core {
        int framecount = 0;
        bool running = false;
    }

    namespace Graphics {
        SDL_Window* gWindow = nullptr;
        SDL_Renderer* gRenderer = nullptr;
        SDL_Texture* gTexture = nullptr;
        float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
        int screen_width = 480;
        int screen_height = 320;
    }
}
```
#### Class Member Variables:
If variables are logically tied to a class, use member variables (mWindow, mSurface) instead of global scope, adhering to encapsulation principles.
