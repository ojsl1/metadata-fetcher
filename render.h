#ifndef RENDER_H
#define RENDER_H
#include "main.h" // globals

#if ALLEYS
#include <iostream>
#include <vector>
#endif

#include "input.h" // forward declare class for Draw();
#include "sprite.h" // forward declare class for Draw();
class Mouse;
class Sprite;

class RendererBase{
public:
    void cap_framerate(Uint32 starting_tick);
    void initVideo(int window_width, int window_height);
    void initColors(SDL_Surface *gScreen);
    void Clear();
    void Draw(Mouse &mouse, Sprite &spriteExit, Sprite &spriteTests,
         Sprite &spriteDrop, Sprite &spriteMute, Sprite &spritePause,
         Sprite &spriteBorder, Sprite &spriteFrame, Sprite &spriteBg);
    void DrawTests();
    void Present(); //for swapping buffers with opengl
    void Shutdown(SDL_Window *gWindow, WindowDimensions dims);
};


#if ALLEYS
// Refactor below Alley classes
class SpriteTest{
protected:
    // private members wont be inhereted, anything that's protected will be accesible to children
    SDL_Surface *image;
    SDL_Rect rect;
    // declaring origin variables pre-emptively here, could they be declared in Block instead? TODO
    int origin_x, origin_y;
public:
    SpriteTest( Uint32 color, int x, int y, int w = 48, int h = 64 ){
        image = SDL_CreateRGBSurface( 0, w, h, 32, 0, 0, 0, 0 );

        SDL_FillRect( image, NULL, color );

        rect = image->clip_rect;

        origin_x = rect.w / 2;
        origin_y = rect.h / 2;

        rect.x = x;
        rect.y = y;
    // applying origin this early is not good, instead apply it later after inheritance at Block
    //  rect.x = x - origin_x;
    //  rect.y = y - origin_y;

    }

  void update(){
      // Can be overridden!
  }

  void draw( SDL_Surface *destination ){
      SDL_BlitSurface( image, NULL, destination, &rect );
  }
  
  SDL_Surface* get_image(  ) const {
  // for `bool operator`
      return image;
  }

  bool operator==( const SpriteTest &other ) const {
  //arguments have to be constant because neither of them are going to change
  //the other has to be a pointer, it has to be a legitimate object we're looking in memory
  //below has to be a const function itself
  // because image is a private variable, we will not be able to access it in e `other` object
  // with just `other.image`, so create get_image() to return its private internals.
      return ( image == other.get_image() );
  }
};

class SpriteTestGroup{
private:
    std::vector <SpriteTest*> spritetestList;
    //keep track of spritetest size with a separate variable, so you dont have to repeatedly look for
    //the spritetest size by accessing the function of the vector,
    // as you know, the function std::vector has the size value and it's accessible with the .size function
    int spritetestList_size;
public:
    SpriteTestGroup copy(  ){
    // pygame.sprite.Group.copy  - duplicate the spritetest group
        SpriteTestGroup new_group;

        //fill the duplicated new_group with all the spritetests
        // note: no need to use spritetests.size here
        // spritetestList[i] is read as index at i
        for ( int i = 0; i < spritetestList_size; i++ ){
          new_group.add( spritetestList[i] );
        }

        return new_group;
    }

    void add( SpriteTest *spritetest ){
    //pygame.sprite.Group.add
    // std::vector::push_back 
        spritetestList.push_back( spritetest );

        spritetestList_size = spritetestList.size();
    }

    void remove( SpriteTest spritetest_object ){
    //pygame.sprite.Group.remove - removes spritetest_object - 
    // We're not looking at another pointer ie. *spritetest_object, because we're already using the reference we've already seen
    // in class SpriteTest: `bool operator==( const Spritee &other )` to look at the actual reference value with the ampersand
        for ( int i = 0; i < spritetestList_size; i++ ){
            if ( *spritetestList[i] == spritetest_object ){
            // std::vector::erase takes an iterator - easily access by adding desired index to std::vector::begin
                spritetestList.erase( spritetestList.begin() + i );
            }
        }
        //reset list size after removal
        spritetestList_size = spritetestList.size();
    }
    
    bool has( SpriteTest spritetest_object ){
    //pygame.sprite.Group.has - test if a group contains spritetests
        for ( int i = 0; i < spritetestList_size; i++ ){
            if ( *spritetestList[i] == spritetest_object ){
            return true;
            }
        }

        return false;
    }

    void update( ){
    //pygame.sprite.Group.update - call the update method on contained SpriteTests
        if ( !spritetestList.empty() ){
          // std::vector::empty
          for ( int i = 0; i < spritetestList_size; i++ ){
              spritetestList[i]->update();
          }
        }
    }

    void draw( SDL_Surface *destination ){
    //pygame.sprite.Group.draw - blit the spritetest images
        if ( !spritetestList.empty() ){
            for ( int i = 0; i < spritetestList_size; i++ ){
              spritetestList[i]->draw( destination );
          }
        }
    }

    void empty( ){
    //pygame.sprite.Group.clear - remove all spritetests
        // std::vector:clear
        spritetestList.clear();

        spritetestList_size = spritetestList.size();
    }

    int size(){
        return spritetestList_size;
    }

    std::vector <SpriteTest*> get_spritetests(){
    // pygame.sprite.Group.sprites - list of the spritetests this group contains
        return spritetestList;
    }
};

class Block : public SpriteTest{
public:
    //funnily this constructor will take the same arguments as original SpriteTest class
    // call the constructor for the parent object/SpriteTest class itself,
    // effectively mapping the parent object to Block
    Block( Uint32 color, int x, int y, int w = 48, int h = 64 )
    : SpriteTest( color, x, y, w, h ){
        update_properties();
    }

    void update_properties(){
    // reset the origin_x and origin_y for them them to be specific to this block
    // this is the benefit of implementing this with classes
        origin_x = 0;
        origin_y = 0;

        set_position( rect.x, rect.y );
    }

    void set_position( int x, int y ){
        rect.x = x - origin_x;
        rect.y = y - origin_y;
    }

    void set_image( const char filename[] = NULL ){
        if ( filename != NULL ){
            SDL_Surface *loaded_image = NULL;

            //loaded_image = SDL_LoadBMP( filename );
            loaded_image = IMG_Load( filename );

            if ( loaded_image != NULL ){
              // have to  reset the image variable, because it is the selfimage of the object
              // ie. the SpriteTest object itself needs to know the image we already loaded
                image = loaded_image;

              // resetting the image/changing the image for this object/SpriteTest will change the rectangular properties
              // ie. SDL_rect, rect.x rect.w etc.
              // thus for keeping track of the position create variables for pre-reset rect values
              
              int old_x = rect.x;
              int old_y = rect.y;

              //because image is a pointer we have to use the arrow selector
              //now we will be able to access it within the block object, and eventually our spritetest object
              rect = image->clip_rect;

              //small variable swap to keep track of the old values
              rect.x = old_x;
              rect.y = old_y;

              //apply origin
              update_properties();
            }
        }
    }
};
#endif // ALLEYS

#endif // RENDER_H
