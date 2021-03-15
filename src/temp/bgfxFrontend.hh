// bgfxFrontend.hh
//

#ifndef LZZ_bgfxFrontend_hh
#define LZZ_bgfxFrontend_hh
// SdlStbFont bgfxFrontend
// By Liam Twigger - 2021
// Public Domain
#include "bgfxh_embedded_shader.h"

//#define BGFXSFH_IS_VALID(X) std::cout << "is valid " << #X << ": " << bgfx::isValid(X) << std::endl 
#define BGFXSFH_IS_VALID(X) BX_NOOP(X)

#define LZZ_INLINE inline
struct bgfx_stb_prerendered_text : public sttfont_prerendered_text
{
  bgfx::TextureHandle mBgfxTexture;
  bgfx_stb_prerendered_text ();
  void freeTexture ();
  int draw (bgfx::ViewId mViewId, int const x, int const y);
  int draw_worker (bgfx::ViewId mViewId, int const x, int const y, bool const resetColour);
  int drawWithColorMod (bgfx::ViewId mViewId, int const x, int const y, uint8_t const r, uint8_t const g, uint8_t const b, uint8_t const a = 255);
};
struct bgfxsfh
{
public:
  static bgfx::ShaderHandle vert_passthrough;
  static bgfx::ShaderHandle frag_passthrough;
  static bgfx::ShaderHandle textured_vert_passthrough;
  static bgfx::ShaderHandle textured_frag_passthrough;
  static bgfx::ProgramHandle untexturedProgram;
  static bgfx::ProgramHandle texturedProgram;
  static bgfx::UniformHandle u_colour;
  static bgfx::UniformHandle s_texture;
  static uint32_t const RENDER_STATE;
  static uint32_t const RENDER_STATE_PRERENDER;
  static int refCount;
  struct rect
  {
    float x;
    float y;
    float w;
    float h;
  };
  struct Vec4
  {
    float (v) [4];
    Vec4 ();
    static bgfxsfh::Vec4 fromUint8 (uint8_t const r, uint8_t const g, uint8_t const b, uint8_t const a);
  };
  static Vec4 toVec4 (uint8_t const a, uint8_t const b, uint8_t const c, uint8_t const d);
  static void initialise ();
  static void deinitialise ();
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
  static void pushTexturedQuad (rect const & r, rect const & r2, bool dontFilpY = false);
  static void pushTexturedQuad (float const xOffset, float const yOffset, float const xSize, float const ySize, float const texU, float const texV, float const uWidth, float const vHeight, bool const dontFilpY, float const _framebufferWidth = 0.f, float const _framebufferHeight = 0.f);
  static void pushUntexturedQuad (rect const & r);
  static void pushUntexturedQuad (float const xOffset, float const yOffset, float const xSize, float const ySize, float const _framebufferWidth = 0.f, float const _framebufferHeight = 0.f);
};
struct bgfx_stb_glyph : public sttfont_glyph
{
  bgfx::TextureHandle mAtlasTexture;
  float x;
  float y;
  float w;
  float h;
  bgfx_stb_glyph ();
};
struct bgfx_stb_glyph_atlas
{
  bgfx::TextureHandle mAtlasTexture;
  stbrp_context * mStbRectPackCtx;
  stbrp_node * mNodes;
  bool isFull;
  bgfx_stb_glyph_atlas ();
};
class bgfx_stb_font_cache : public sttfont_font_cache
{
public:
  bgfx::ViewId mViewId;
  int mAtlasSize;
  SSF_VECTOR <bgfx_stb_glyph_atlas> mAtlases;
  bool isRenderingToTarget;
  bgfx_stb_font_cache ();
  ~ bgfx_stb_font_cache ();
  SSF_MAP <uint64_t, bgfx_stb_glyph> mGlyphs;
  void clearGlyphs ();
  void bindRenderer (bgfx::ViewId const _viewId);
  bgfx_stb_glyph_atlas * getGenAtlasPage ();
  bgfx_stb_glyph_atlas * createAtlasPage ();
  struct tempGlyph
  {
    unsigned char * bitmapData;
    uint64_t target;
  };
  void pregenGlyphs (SSF_VECTOR <sttfont_uint32_t_range> & mRanges, uint8_t const format);
  void pregenGlyphs_pack (SSF_VECTOR <tempGlyph> & tempGlyphs, SSF_VECTOR <stbrp_rect> & rects, bool force);
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
LZZ_INLINE bgfxsfh::Vec4::Vec4 ()
                             {}
LZZ_INLINE bgfxsfh::Vec4 bgfxsfh::Vec4::fromUint8 (uint8_t const r, uint8_t const g, uint8_t const b, uint8_t const a)
                                                                                                                          {
			Vec4 rv;
			rv.v[0] = r/255.0;
			rv.v[1] = g/255.0;
			rv.v[2] = b/255.0;
			rv.v[3] = a/255.0;
			return rv;
			}
#undef LZZ_INLINE
#endif
