# Metadata Fetcher
This is a GUI tool for quickly fetching text metadata from PNG images, just dragndrop the image into the GUI and the text will get printed into the console (WIP eventually into the gui). It was mainly made for work with Stable Diffusion, which is why it doesnt use hardware acceleration and runs on the CPU.

## Building and running
This hasn't been and wont be tested on non-Linux systems.

You need at least: `sdl2`, `sdl2_image`, `sdl2_mixer`, `sdl2_ttf`.

```
$ git clone https://github.com/ojsl1/metadata-fetcher.git
$ cd metadata-fetcher
$ make
```

## Todo
- Rewrite using SDL_Renderer and SDL_Texture.
- Render with OpenGL instead of SDL
- Abstract pure virtual class methods for runtime polymorphism.
- SDL2 -> SDL3
- Rendering multiple images into a scrollable mosaic, compare:
    - imagemagick montage
    - yfszzx/stable-diffusion-webui-images-browser
    - bit9labs/sd-masonry
    - dolphin file manager icon view with all ui panels hidden, small label width, and max line number as 1
- Integrate into KDE Plasma (dolphin) context menus: "Open With-\>Metadata Fetcher", "Actions-\>Copy metadata", "Actions-\>Open metadata in default text-editor", compare:
    - irfanhakim-as/kde-service-menu-reimage
    - https://wiki.archlinux.org/title/Dolphin#Creating_custom_service_menus

