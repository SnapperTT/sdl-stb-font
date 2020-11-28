// sdlFrontend.hh
//

#ifndef LZZ_sdlFrontend_hh
#define LZZ_sdlFrontend_hh
// SdlStbFont example
// By Liam Twigger - 2020
// Public Domain

struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Surface;

#define LZZ_INLINE inline
struct sdl_stb_prerendered_text : public sttfont_prerendered_text
{
  SDL_Texture * mSdlTexture;
  SDL_Renderer * mRenderer;
  sdl_stb_prerendered_text ();
  void freeTexture ();
  int draw (int const x, int const y);
  int drawWithColorMod (int const x, int const y, uint8_t const r, uint8_t const g, uint8_t const b, uint8_t const a = 255);
};
struct sdl_stb_glyph : public sttfont_glyph
{
  SDL_Texture * mSdlTexture;
  SDL_Surface * mSdlSurface;
  sdl_stb_glyph ();
};
class sdl_stb_font_cache : public sttfont_font_cache
{
public:
  SDL_Renderer * mRenderer;
  sdl_stb_font_cache ();
  ~ sdl_stb_font_cache ();
  SSF_MAP <uint64_t, sdl_stb_glyph> mGlyphs;
  void clearGlyphs ();
  void bindRenderer (SDL_Renderer * _mRenderer);
  void genGlyph_writeData (sttfont_glyph * gOut, unsigned char * bitmap2, int w, int h);
  sttfont_glyph * getGlyph (uint64_t const target);
  sttfont_glyph * genGlyph_createAndInsert (uint64_t const target, uint32_t const codepoint, uint8_t const format);
  void processCodepoint (int & x, int & y, uint32_t const codepoint, sttfont_format const * const format, bool isDrawing, int kerningAdv, int & overdraw);
  SDL_Texture * renderTextToTexture (char const * c, uint32_t const maxLen = -1, int * widthOut = NULL, int * heightOut = NULL);
  SDL_Texture * renderTextToTexture (sttfont_formatted_text const & formatted, int * widthOut = NULL, int * heightOut = NULL);
protected:
  SDL_Texture * renderTextToTexture_worker (sttfont_formatted_text const * formatted, char const * c, uint32_t const maxLen = -1, int * widthOut = NULL, int * heightOut = NULL);
public:
  SDL_Texture * renderTextToTexture (SSF_STRING const & str, int * widthOut = NULL, int * heightOut = NULL);
  void renderTextToObject (sttfont_prerendered_text * textOut, char const * c, uint32_t const maxLen = -1);
  void renderTextToObject (sttfont_prerendered_text * textOut, SSF_STRING const & str);
  void renderTextToObject (sttfont_prerendered_text * textOut, sttfont_formatted_text const & str);
};
#undef LZZ_INLINE
#endif
