SDL STB Font Renderer
=====================
A header-only library for rendering text in [SDL2](https://www.libsdl.org/) with [STB_Truetype](https://github.com/nothings/stb). This caches glyphs as they are drawn allowing for fast text rendering. It also provides a couple of easy ways to render a string to texture for even faster text rendering.

## Example Image:
![Example text test](example.png)

Sample text from http://www.columbia.edu/~fdc/utf8/index.html

## Contact:
Liam Twigger - @SnapperTheTwig

## Features:
* Runs in pure SDL - no OpenGL required (though it does work with OpenGL) 
* Simple text rendering - `fc.drawText(x, y, string)`
* Rendering to a texture - `fc.renderTextToTexture(string, &widthOut, &heightOut)`
* Rendering to a texture object - see examples
* UTF-8 support
* Handles newlines
* Fallback fonts support - can support many languages at once!
* Only ~300 lines of code
* No dependencies apart from STB_Truetype, SDL and standard libraries
* Public Domain

## Performance:
On a Intel i7-8750H:

Example image takes 0.7ms to render (~1400 FPS) if using texture (`renderTextToTexture`/`renderTextToObject`)

Example image takes ~5ms to render (~200 FPS) if rendering directly (`drawText`)

For text that lasts more than one frame you should cache it with either `renderTextToTexture` or `renderTextToObject`.


How Do I?
=========

## Load Fonts and Draw Text?
```c++
#define SDL_STB_FONT_IMPL
#include "sdlStbFont.h"

...

// Load font
char * ttfFontFromMemory = loadFontFileSomehow("path/to/file.ttf");

sdl_stb_font_cache fc;
fc.faceSize = 24; // Must be set before loadFont()!
fc.loadFont(ttfFontFromMemory);

...

// Setup renderer
SDL_Renderer * mSdlRenderer = SDL_CreateRenderer(mWindow, SDL_RENDERER_SOFTWARE, 0);
fc.bindRenderer(mSdlRenderer); // Must bind a renderer before generating any glyphs

...

// Main loop
SDL_SetRenderDrawColor(mSdlRenderer, 125, 125, 125, 255);
SDL_RenderClear(mSdlRenderer);
fc.drawText(5, 5, "Hello world!");
SDL_RenderPresent(mSdlRenderer);

// fc will clean itself up when it falls out of scope

```

## Get Font Metrics
 
```c++
class sdl_stb_font_cache {
...
public:
// All of these are calculated when loadFont() is called
// You can safely read these
  int ascent;
  int descent;
  int lineGap;
  int baseline;
  int rowSize;
  float scale;
// Must be set before loadFont() is called. Cannot be changed after
  int faceSize;
...
  }
  
```

## Use Fallback Fonts (For Multilinugal Support)

```c++

fc.loadFont(ttfFontFromMemory);
fc.addFont(someSecondFontFromMemory);
fc.addFont(someThirdFontFromMemory);
...
etc

```

Note that all fonts have to be loaded before any drawing functions are called. If a glyph is not found in the first font then the second font will be searched, etc.

## Get the Size of Text

```c++

int w, h;
fc.getTextSize(w, h, "Text");

h = fc.getTextHeight("Text"); // Faster, if only height is needed
int nRows = fc.getTextRows("Text \n More text"); // Returns the number of rows of text - here it's 2
```

## Caching Results in a Texture
First way:
```c++
// creating
int RTw, RTh;
SDL_Texture * RT;
RT = fc.renderTextToTexture ("Text ", &RTw, &RTh);

// Rendering
SDL_Rect r;
r.x = 5;
r.y = 5;
r.w = RTw;
r.h = RTh;
SDL_RenderCopy(mSdlRenderer , RT , NULL, &r); 
```

Second way (same, but cleaner)
```c++
// creating
sdl_stb_prerendered_text prt;
fc.renderTextToObject(&prt, "Text"); 
		
// Rendering
prt.draw(mSdlRenderer, x, y);

// Cleanup
prt.freeTexture();
```

## Print in Colours Other Than White
Use `SDL_SetTextureColorMod` with a cached texture

Contributing
============
Be sure to create/edit the `.lzz` files, not the generated `.h` files. The tools + scripts to create the header files from the `.lzz` sources are provided.

Thanks
======
Thanks to the contribitors to both the SDL and STB projects!

License
=======
Public Domain

stb_truetype is Public Domain

Noto Fonts are (C) Google and are released under the SIL Open Font License, Version 1.1. See https://www.google.com/get/noto/

