// sdlStbFont.hh
//

#ifndef LZZ_sdlStbFont_hh
#define LZZ_sdlStbFont_hh
// SdlStbFont example
// By Liam Twigger - 2020
// Public Domain

struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Surface;

#ifndef STB_TRUETYPE_INCLUDE_HANDLED
	////////////////////////////////////////
	// STB TRUETYPE 
	// Handle double-including
	#ifdef STB_TRUETYPE_IMPLEMENTATION
		#undef STB_TRUETYPE_IMPLEMENTATION
	#endif
	#include "stb_truetype.h"
	////////////////////////////////////////
#endif
	
#include <cstdint>
#include <map>
#define LZZ_INLINE inline
struct sdl_stb_prerendered_text
{
  SDL_Texture * mSdlTexture;
  int width;
  int height;
  sdl_stb_prerendered_text ();
  void freeTexture ();
  void draw (SDL_Renderer * mRenderer, int const x, int const y);
  void drawWithColorMod (SDL_Renderer * mRenderer, int const x, int const y, uint8_t const r, uint8_t const g, uint8_t const b, uint8_t const a = 255);
};
struct sdl_stb_glyph
{
  SDL_Texture * mSdlTexture;
  SDL_Surface * mSdlSurface;
  int advance;
  int leftSideBearing;
  int width;
  int height;
  int xOffset;
  int yOffset;
  sdl_stb_glyph ();
};
struct sdl_stb_font_list
{
  stbtt_fontinfo mFont;
  sdl_stb_font_list * next;
  sdl_stb_font_list ();
  ~ sdl_stb_font_list ();
  void fetchFontForCodepoint (uint32_t const codepoint, stbtt_fontinfo * * mFontOut, int * indexOut);
};
class sdl_stb_font_cache
{
public:
  SDL_Renderer * mRenderer;
  sdl_stb_font_list mFont;
  int ascent;
  int descent;
  int lineGap;
  int baseline;
  int rowSize;
  float scale;
  int faceSize;
  std::map <uint32_t, sdl_stb_glyph> mGlyphs;
  sdl_stb_font_cache ();
  ~ sdl_stb_font_cache ();
  void clearGlyphs ();
  void bindRenderer (SDL_Renderer * _mRenderer);
  void setFaceSize (int const _faceSize);
  void loadFont (char const * ttf_buffer, int index = 0);
  void addFont (char const * ttf_buffer, int index = 0);
  void genGlyph (uint32_t const codepoint, sdl_stb_glyph * gOut);
  sdl_stb_glyph * getGlyph (uint32_t const codepoint);
  sdl_stb_glyph * getGenGlyph (uint32_t const codepoint);
  int getKerningAdvance (uint32_t const cp1, uint32_t const cp2);
  int utf8_charsize (char const * c);
  uint32_t utf8_read (char const * c, uint32_t & seek, uint32_t const maxLen);
  void drawText (int const x, int const y, char const * c, uint32_t const maxLen = -1);
  void drawText (int const x, int const y, std::string const & str);
  void getTextSize (int & w, int & h, char const * c, uint32_t const maxLen = -1);
  void getTextSize (int & w, int & h, std::string const & str);
  int getNumberOfRows (std::string const & str);
  int getTextHeight (std::string const & str);
  void processString (int const x, int const y, char const * c, uint32_t const maxLen, bool const isDrawing, int * const widthOut = NULL, int * const heightOut = NULL);
  bool isTofu (sdl_stb_glyph * G);
  sdl_stb_glyph * getGlyphOrTofu (uint32_t const codepoint);
  void processCodepoint (int & x, int & y, uint32_t const codepoint, bool isDrawing);
  SDL_Texture * renderTextToTexture (char const * c, uint32_t const maxLen = -1, int * widthOut = NULL, int * heightOut = NULL);
  SDL_Texture * renderTextToTexture (std::string const & str, int * widthOut = NULL, int * heightOut = NULL);
  void renderTextToObject (sdl_stb_prerendered_text * textOut, char const * c, uint32_t const maxLen = -1);
  void renderTextToObject (sdl_stb_prerendered_text * textOut, std::string const & str);
};
#undef LZZ_INLINE
#endif
