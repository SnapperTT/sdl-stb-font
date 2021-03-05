// bgfxFrontend.hh
//

#ifndef LZZ_bgfxFrontend_hh
#define LZZ_bgfxFrontend_hh
// SdlStbFont bgfxFrontend
// By Liam Twigger - 2021
// Public Domain
#include "bgfxFrontendShaders.h"
#define LZZ_INLINE inline
struct bgfx_stb_prerendered_text : public sttfont_prerendered_text
{
  bgfx::TextureHandle mBgfxTexture;
  bgfx_stb_prerendered_text ();
  void freeTexture ();
  int draw (int const x, int const y);
  int drawWithColorMod (int const x, int const y, uint8_t const r, uint8_t const g, uint8_t const b, uint8_t const a = 255);
};
struct bgfxsfh
{
public:
  static bgfx::ShaderHandle vert_passthrough;
  static bgfx::ProgramHandle texturedProgram;
  static bgfx::UniformHandle u_colour;
  static bgfx::UniformHandle u_texture;
  static int refCount;
  static void initialise ();
  static void deinitialise ();
  struct rect
  {
    float x;
    float y;
    float w;
    float h;
  };
  struct PosTexCoord0Vertex
  {
    float m_x;
    float m_y;
    float m_z;
    float m_u;
    float m_v;
    static void init ();
    static bgfx::VertexLayout ms_decl;
  };
  struct PosVertex
  {
    float m_x;
    float m_y;
    float m_z;
    static void init ();
    static bgfx::VertexLayout ms_decl;
  };
  static void pushTexturedQuad (float const x, float const y, float const xSize, float const ySize, float const _framebufferWidth = 0.f, float const _framebufferHeight = 0.f);
  static void pushUntexturedQuad (float const x, float const y, float const xSize, float const ySize, float const _framebufferWidth = 0.f, float const _framebufferHeight = 0.f);
};
struct bgfx_stb_glyph : public sttfont_glyph
{
  bgfx::TextureHandle mAtlas;
  float x;
  float y;
  float w;
  float h;
  bgfx_stb_glyph ();
};
struct bgfx_stb_glyph_atlas
{
  bgfx::TextureHandle mAtlas;
  stbrp_context mStbRectPackCtx;
  bool isFull;
  bool isNew;
  bgfx_stb_glyph_atlas ();
};
class bgfx_stb_font_cache : public sttfont_font_cache
{
public:
  bgfx::ViewId mViewId;
  int mAtlasSize;
  SSF_VECTOR <bgfx_stb_glyph_atlas> mAtlases;
  bgfx_stb_font_cache ();
  ~ bgfx_stb_font_cache ();
  SSF_MAP <uint64_t, bgfx_stb_glyph> mGlyphs;
  void clearGlyphs ();
  void bindRenderer ();
  bgfx_stb_glyph_atlas * getGenAtlasPage ();
  bgfx_stb_glyph_atlas * createAtlasPage ();
  void genGlyph_writeData (uint32_t const codepoint, sttfont_glyph * gOut, unsigned char * bitmap2, int w, int h);
  void genGlyph_writeData2 (uint32_t const codepoint, sttfont_glyph * gOut, unsigned char * bitmap2, int w, int h, bool firstCall);
  sttfont_glyph * getGlyph (uint64_t const target);
  sttfont_glyph * genGlyph_createAndInsert (uint64_t const target, uint32_t const codepoint, uint8_t const format);
  void processCodepoint (int & x, int & y, uint32_t const codepoint, sttfont_format const * const format, bool isDrawing, int kerningAdv, int & overdraw);
  bgfx::TextureHandle renderTextToTexture (char const * c, uint32_t const maxLen = -1, int * widthOut = NULL, int * heightOut = NULL);
  bgfx::TextureHandle renderTextToTexture (sttfont_formatted_text const & formatted, int * widthOut = NULL, int * heightOut = NULL);
protected:
  bgfx::TextureHandle renderTextToTexture_worker (sttfont_formatted_text const * formatted, char const * c, uint32_t const maxLen = -1, int * widthOut = NULL, int * heightOut = NULL);
public:
  bgfx::TextureHandle renderTextToTexture (SSF_STRING const & str, int * widthOut = NULL, int * heightOut = NULL);
  void renderTextToObject (sttfont_prerendered_text * textOut, char const * c, uint32_t const maxLen = -1);
  void renderTextToObject (sttfont_prerendered_text * textOut, SSF_STRING const & str);
  void renderTextToObject (sttfont_prerendered_text * textOut, sttfont_formatted_text const & str);
};
#undef LZZ_INLINE
#endif
