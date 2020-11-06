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

// Defines
#ifndef SSF_MAP
	#include <map>
	#define SSF_MAP std::map
#endif
#ifndef SSF_VECTOR
	#include <vector>
	#define SSF_VECTOR std::vector
#endif
#ifndef SSF_STRING
	#include <string>
	#define SSF_STRING std::string
#endif


// new and delete macros
// all calls in this library are done with "foo * f = SSF_NEW(f)"
// implement your custom allocator by defining SSF_NEW and SSF_DEL
#ifndef SSF_NEW
	#define SSF_NEW(X) new X
#endif
#ifndef SSF_NEW_ARR
	#define SSF_NEW_ARR(X,S) new X[S]
#endif
#ifndef SSF_DEL
	#define SSF_DEL(X) delete X
#endif
#ifndef SSF_DEL_ARR
	#define SSF_DEL_ARR(X) delete[] X
#endif


#include <cstdint>

// move semantics - makes lzz happy
#define SSF_STRING_MS SSF_STRING&&
#define sdl_stb_formated_text_item_MS sdl_stb_formated_text_item&&

// misc
#ifdef INT32_MIN
	#define SSF_INT_MIN  INT32_MIN
#else
	#define SSF_INT_MIN 0x8000000;
#endif
struct sdl_stb_formatted_text;
#define LZZ_INLINE inline
struct sdl_stb_format_callback
{
  virtual void callbackOnDraw (sdl_stb_formatted_text const & text, int index, int x, int y, int xOffsetInitial, int xOffsetAfter, int segmentWidth, int segmentHeight);
};
struct sdl_stb_format_reset
{
};
struct sdl_stb_format
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
  uint8_t format;
  static uint8_t const FORMAT_NONE;
  static uint8_t const FORMAT_BOLD;
  static uint8_t const FORMAT_ITALIC;
  static uint8_t const FORMAT_UNDERLINE;
  static uint8_t const FORMAT_STRIKETHROUGH;
  static uint8_t const FORMAT_RENDER_EVEN_IF_CALLBACK_EXISTS;
  sdl_stb_format ();
  sdl_stb_format (uint8_t const _format, uint8_t const _r = 255, uint8_t const _g = 255, uint8_t const _b = 255, uint8_t const _a = 255);
  void combine (sdl_stb_format const & other);
  static sdl_stb_format color (uint8_t const _r, uint8_t const _g, uint8_t const _b, uint8_t const _a = 255);
  static sdl_stb_format colour (uint8_t const _r, uint8_t const _g, uint8_t const _b, uint8_t const _a = 255);
  static sdl_stb_format const bold;
  static sdl_stb_format const italic;
  static sdl_stb_format const underline;
  static sdl_stb_format const strikethrough;
  static sdl_stb_format const red;
  static sdl_stb_format const green;
  static sdl_stb_format const blue;
  static sdl_stb_format const yellow;
  static sdl_stb_format const black;
  static sdl_stb_format const white;
  static sdl_stb_format const magenta;
  static sdl_stb_format const cyan;
  static sdl_stb_format const grey;
  static sdl_stb_format_reset const reset;
};
struct sdl_stb_formated_text_item
{
  SSF_STRING text;
  sdl_stb_format format;
  sdl_stb_format_callback * callback;
  sdl_stb_formated_text_item ();
  sdl_stb_formated_text_item (SSF_STRING const & _text, sdl_stb_format const & _format);
  sdl_stb_formated_text_item (SSF_STRING_MS _text, sdl_stb_format const & _format);
  sdl_stb_formated_text_item & setCallback (sdl_stb_format_callback * _cb);
};
struct sdl_stb_formatted_text
{
  SSF_VECTOR <sdl_stb_formated_text_item> mItems;
  sdl_stb_format activeFormat;
  sdl_stb_formatted_text ();
  sdl_stb_formatted_text (SSF_STRING const & text);
  sdl_stb_formatted_text (SSF_STRING_MS text);
  sdl_stb_formatted_text (char const * text);
  void resetFormat ();
  sdl_stb_formatted_text & operator << (SSF_STRING const & text);
  sdl_stb_formatted_text & operator << (SSF_STRING_MS text);
  sdl_stb_formatted_text & operator << (char const * text);
  sdl_stb_formatted_text & operator << (sdl_stb_format const & format);
  sdl_stb_formatted_text & operator << (sdl_stb_format_reset const & reset);
  sdl_stb_formatted_text & operator << (sdl_stb_formated_text_item const & obj);
  sdl_stb_formatted_text & operator << (sdl_stb_formated_text_item_MS obj);
};
struct sdl_stb_prerendered_text
{
  SDL_Texture * mSdlTexture;
  int width;
  int height;
  sdl_stb_prerendered_text ();
  void freeTexture ();
  int draw (SDL_Renderer * mRenderer, int const x, int const y);
  int drawWithColorMod (SDL_Renderer * mRenderer, int const x, int const y, uint8_t const r, uint8_t const g, uint8_t const b, uint8_t const a = 255);
};
struct sdl_stb_memory
{
  char * data;
  bool ownsData;
  void alloc (size_t const size);
  void transferTo (sdl_stb_memory & destination);
  sdl_stb_memory ();
  ~ sdl_stb_memory ();
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
  sdl_stb_memory mMemory;
  uint8_t format;
  SSF_VECTOR <sdl_stb_font_list*> mFormatedVariants;
  sdl_stb_font_list * next;
  sdl_stb_font_list ();
  ~ sdl_stb_font_list ();
  void fetchFontForCodepoint (uint32_t const codepoint, uint8_t const format, stbtt_fontinfo * * mFontOut, int * indexOut);
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
  float underlineThickness;
  float strikethroughThickness;
  float underlinePosition;
  float strikethroughPosition;
  int faceSize;
  SSF_MAP <uint64_t, sdl_stb_glyph> mGlyphs;
  sdl_stb_font_cache ();
  ~ sdl_stb_font_cache ();
  void clearGlyphs ();
  void bindRenderer (SDL_Renderer * _mRenderer);
  void setFaceSize (int const _faceSize);
  int getScaledRowSize () const;
  void loadFont (char const * ttf_buffer, int index = 0);
  void loadFontManaged (sdl_stb_memory & memory, int index = 0);
  void addFont (char const * ttf_buffer, int index = 0);
  void addFontManaged (sdl_stb_memory & memory, int index = 0);
  void addFormatFont (uint8_t formatMask, char const * ttf_buffer, int index = 0);
  void addFormatFontManaged (uint8_t formatMask, sdl_stb_memory & memory, int index = 0);
protected:
  struct addFontWrap
  {
    char const * ttf_buffer;
    sdl_stb_memory * memory;
    int index;
    addFontWrap (char const * c);
  };
  void addFont_worker (addFontWrap & fwm, bool isFormatVariant, uint8_t formatMask = 0);
public:
  void genGlyph (uint32_t const codepoint, uint8_t const format, sdl_stb_glyph * gOut);
  sdl_stb_glyph * getGlyph (uint64_t const target);
  sdl_stb_glyph * getGenGlyph (uint32_t const codepoint, uint8_t const format);
  int getKerningAdvance (uint32_t const cp1, uint32_t const cp2);
  int utf8_charsize (char const * c);
  uint32_t utf8_read (char const * c, uint32_t & seek, uint32_t const maxLen);
  int drawText (int const x, int const y, char const * c, uint32_t const maxLen = -1);
  int drawText (int const x, int const y, SSF_STRING const & str);
  int drawText (int const x, int const y, int & widthOut, int & heightOut, char const * c, uint32_t const maxLen = -1);
  int drawText (int const x, int const y, SSF_STRING const & str, int & widthOut, int & heightOut);
  int drawText (int const x, int const y, sdl_stb_formatted_text const & text);
  int drawText (int const x, int const y, sdl_stb_formatted_text const & text, int & widthOut, int & heightOut);
  int getTextSize (int & w, int & h, char const * c, uint32_t const maxLen = -1);
  int getTextSize (int & w, int & h, SSF_STRING const & str);
  int getTextSize (int & w, int & h, sdl_stb_formatted_text const & str);
  int getNumberOfRows (SSF_STRING const & str);
  int getNumberOfRows (sdl_stb_formatted_text const & str);
  int getTextHeight (SSF_STRING const & str);
  int getTextHeight (sdl_stb_formatted_text const & str);
  int processString (int const x, int const y, char const * c, uint32_t const maxLen, sdl_stb_format const * const format, bool const isDrawing, int * const widthOut = NULL, int * const heightOut = NULL, int const * const threshX = NULL, int const * const threshY = NULL, int * const caretPosition = NULL, int initialXOffset = 0);
  int processFormatted (sdl_stb_formatted_text const & text, int x, int y, bool const isDrawing, int * const widthOut = NULL, int * const heightOut = NULL, int const * const threshX = NULL, int const * const threshY = NULL, int * const caretPosition = NULL, int initialXOffset = 0);
  int getCaretPos (SSF_STRING const & str, int const relMouseX, int const relMouseY);
  int getCaretPos (sdl_stb_formatted_text const & str, int const relMouseX, int const relMouseY);
  bool isTofu (sdl_stb_glyph * G);
  sdl_stb_glyph * getGlyphOrTofu (uint32_t const codepoint, uint8_t const format);
  void processCodepoint (int & x, int & y, uint32_t const codepoint, sdl_stb_format const * const format, bool isDrawing, int kerningAdv, int & overdraw);
  SDL_Texture * renderTextToTexture (char const * c, uint32_t const maxLen = -1, int * widthOut = NULL, int * heightOut = NULL);
  SDL_Texture * renderTextToTexture (sdl_stb_formatted_text const & formatted, int * widthOut = NULL, int * heightOut = NULL);
protected:
  SDL_Texture * renderTextToTexture_worker (sdl_stb_formatted_text const * formatted, char const * c, uint32_t const maxLen = -1, int * widthOut = NULL, int * heightOut = NULL);
public:
  SDL_Texture * renderTextToTexture (SSF_STRING const & str, int * widthOut = NULL, int * heightOut = NULL);
  void renderTextToObject (sdl_stb_prerendered_text * textOut, char const * c, uint32_t const maxLen = -1);
  void renderTextToObject (sdl_stb_prerendered_text * textOut, SSF_STRING const & str);
  void renderTextToObject (sdl_stb_prerendered_text * textOut, sdl_stb_formatted_text const & str);
};
#undef LZZ_INLINE
#endif
