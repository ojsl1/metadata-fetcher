#ifndef RENDER_H
#define RENDER_H
#include "main.h"

#include <iostream>
#include <vector>

#define window_width 480
#define window_height 320
#define fps 1

void initVideo();

class Game{
  public:
    //public members
  private:
    //private members
};

class Sound{
  public:
    //public members
  private:
    //private members
    Sound(  ){

    }

    ~Sound(){
    Mix_FreeChunk(bell);
    bell = NULL;
    Mix_FreeMusic(bgm);
    bgm = NULL;
    }
};

class BlankRect{
  public:
      //public members
};

class Sprite{
  protected:
      // private members wont be inhereted, anything that's protected will be accesible to children
      SDL_Surface *image;
      SDL_Rect rect;
      // declaring origin variables pre-emptively here, could they be declared in Block instead? TODO
      int origin_x, origin_y;

  public:
      Sprite( Uint32 color, int x, int y, int w = 48, int h = 64 ){
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

    bool operator==( const Sprite &other ) const {
    //arguments have to be constant because neither of them are going to change
    //the other has to be a pointer, it has to be a legitimate object we're looking in memory
    //below has to be a const function itself
    // because image is a private variable, we will not be able to access it in e `other` object
    // with just `other.image`, so create get_image() to return its private internals.
        return ( image == other.get_image() );
    }
};

class SpriteGroup{
  private:
      std::vector <Sprite*> spriteList;
      //keep track of sprite size with a separate variable, so you dont have to repeatedly look for
      //the sprite size by accessing the function of the vector,
      // as you know, the function std::vector has the size value and it's accessible with the .size function
      int spriteList_size;

  public:
      SpriteGroup copy(  ){
      // pygame.sprite.Group.copy  - duplicate the sprite group
          SpriteGroup new_group;

          //fill the duplicated new_group with all the sprites
          // note: no need to use sprites.size here
          // spriteList[i] is read as index at i
          for ( int i = 0; i < spriteList_size; i++ ){
            new_group.add( spriteList[i] );
          }

          return new_group;
      }

      void add( Sprite *sprite ){
      //pygame.sprite.Group.add
      // std::vector::push_back 
          spriteList.push_back( sprite );

          spriteList_size = spriteList.size();
      }

      void remove( Sprite sprite_object ){
      //pygame.sprite.Group.remove - removes sprite_object - 
      // We're not looking at another pointer ie. *sprite_object, because we're already using the reference we've already seen
      // in class Sprite: `bool operator==( const Sprite &other )` to look at the actual reference value with the ampersand
          for ( int i = 0; i < spriteList_size; i++ ){
              if ( *spriteList[i] == sprite_object ){
              // std::vector::erase takes an iterator - easily access by adding desired index to std::vector::begin
                  spriteList.erase( spriteList.begin() + i );
              }
          }
          //reset list size after removal
          spriteList_size = spriteList.size();
      }
      
      bool has( Sprite sprite_object ){
      //pygame.sprite.Group.has - test if a group contains sprites
          for ( int i = 0; i < spriteList_size; i++ ){
              if ( *spriteList[i] == sprite_object ){
              return true;
              }
          }

          return false;
      }

      void update( ){
      //pygame.sprite.Group.update - call the update method on contained Sprites
          if ( !spriteList.empty() ){
            // std::vector::empty
            for ( int i = 0; i < spriteList_size; i++ ){
                spriteList[i]->update();
            }
          }
      }

      void draw( SDL_Surface *destination ){
      //pygame.sprite.Group.draw - blit the sprite images
          if ( !spriteList.empty() ){
              for ( int i = 0; i < spriteList_size; i++ ){
                spriteList[i]->draw( destination );
            }
          }
      }

      void empty( ){
      //pygame.sprite.Group.clear - remove all sprites
          // std::vector:clear
          spriteList.clear();

          spriteList_size = spriteList.size();
      }

      int size(){
          return spriteList_size;
      }

      std::vector <Sprite*> get_sprites(){
      // pygame.sprite.Group.sprites - list of the sprites this group contains
          return spriteList;
      }
};

class Block : public Sprite{
  // inherit everything from Sprite to class Block,
  // no need for private as everything we care about is already inherited
  public:
    //funnily this constructor will take the same arguments as original Sprite class
        // call the constructor for the parent object/Sprite class itself,
          // effectively mapping the parent object to Block
      Block( Uint32 color, int x, int y, int w = 48, int h = 64 )
      : Sprite( color, x, y, w, h ){
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
                // ie. the Sprite object itself needs to know the image we already loaded
                  image = loaded_image;

                // resetting the image/changing the image for this object/Sprite will change the rectangular properties
                // ie. SDL_rect, rect.x rect.w etc.
                // thus for keeping track of the position create variables for pre-reset rect values
                
                int old_x = rect.x;
                int old_y = rect.y;

                //because image is a pointer we have to use the arrow selector
                //now we will be able to access it within the block object, and eventually our sprite object
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

class EventReceiver {
  public:
      virtual bool HandleEvent(const SDL_Event* Event){
          return false;
      }
};

class Button{
  public:
      //public stuff
  private:
      //private stuff
};
#endif // RENDER_H
