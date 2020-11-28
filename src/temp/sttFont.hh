// sttFont.hh
//

#ifndef LZZ_sttFont_hh
#define LZZ_sttFont_hh
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
#define sttfont_formatted_text_item_MS sttfont_formatted_text_item&&
#define sttfont_formatted_text_MS sttfont_formatted_text&&

// misc
#ifdef INT32_MIN
	#define SSF_INT_MIN  INT32_MIN
#else
	#define SSF_INT_MIN 0x8000000;
#endif
struct sttfont_formatted_text;
#define LZZ_INLINE inline
struct sttfont_format_callback
{
  virtual void callbackOnDraw (sttfont_formatted_text const & text, int index, int x, int y, int xOffsetInitial, int xOffsetAfter, int segmentWidth, int segmentHeight);
};
struct sttfont_format_reset
{
};
struct sttfont_format
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
  sttfont_format ();
  sttfont_format (uint8_t const _format, uint8_t const _r = 255, uint8_t const _g = 255, uint8_t const _b = 255, uint8_t const _a = 255);
  void combine (sttfont_format const & other);
  static sttfont_format color (uint8_t const _r, uint8_t const _g, uint8_t const _b, uint8_t const _a = 255);
  static sttfont_format colour (uint8_t const _r, uint8_t const _g, uint8_t const _b, uint8_t const _a = 255);
  static sttfont_format const bold;
  static sttfont_format const italic;
  static sttfont_format const underline;
  static sttfont_format const strikethrough;
  static sttfont_format const red;
  static sttfont_format const green;
  static sttfont_format const blue;
  static sttfont_format const yellow;
  static sttfont_format const black;
  static sttfont_format const white;
  static sttfont_format const magenta;
  static sttfont_format const cyan;
  static sttfont_format const grey;
  static sttfont_format_reset const reset;
};
struct sttfont_formatted_text_item
{
  SSF_STRING text;
  sttfont_format format;
  sttfont_format_callback * callback;
  sttfont_formatted_text_item ();
  sttfont_formatted_text_item (SSF_STRING const & _text, sttfont_format const & _format);
  sttfont_formatted_text_item (SSF_STRING_MS _text, sttfont_format const & _format);
  sttfont_formatted_text_item & setCallback (sttfont_format_callback * _cb);
};
struct sttfont_formatted_text
{
  SSF_VECTOR <sttfont_formatted_text_item> mItems;
  sttfont_format activeFormat;
  sttfont_formatted_text ();
  sttfont_formatted_text (SSF_STRING const & text);
  sttfont_formatted_text (SSF_STRING_MS text);
  sttfont_formatted_text (char const * text);
  sttfont_formatted_text (sttfont_formatted_text_item_MS text);
  sttfont_formatted_text (sttfont_formatted_text_item const & text);
  sttfont_formatted_text (sttfont_formatted_text_MS obj);
  sttfont_formatted_text & operator = (sttfont_formatted_text_MS obj);
  sttfont_formatted_text & operator = (sttfont_formatted_text const & obj);
  void resetFormat ();
  sttfont_formatted_text & operator << (SSF_STRING const & text);
  sttfont_formatted_text & operator << (SSF_STRING_MS text);
  sttfont_formatted_text & operator << (char const * text);
  sttfont_formatted_text & operator += (SSF_STRING const & text);
  sttfont_formatted_text & operator += (SSF_STRING_MS text);
  sttfont_formatted_text & operator += (char const * text);
  sttfont_formatted_text & operator << (sttfont_format const & format);
  sttfont_formatted_text & operator << (sttfont_format_reset const & reset);
  sttfont_formatted_text & operator << (sttfont_formatted_text_item const & obj);
  sttfont_formatted_text & operator << (sttfont_formatted_text_item_MS obj);
  bool back (unsigned int const num);
  void insert (unsigned int const position, SSF_STRING const & str);
  size_t size () const;
  size_t length () const;
  bool isEmpty () const;
  SSF_STRING getString () const;
private:
  static void remove_substr_worker2 (sttfont_formatted_text & tt, unsigned int & index, int const offset, unsigned int const num, int * const writeOut);
  static void remove_substr_worker2 (sttfont_formatted_text const & tt, unsigned int & index, int const offset, unsigned int const num, SSF_STRING * const writeOut);
  template <typename TT, typename RR>
  static void remove_substr_worker1 (TT & tt, unsigned int const position, unsigned int const num, RR * const writeOut = NULL);
public:
  void remove (unsigned int const position, unsigned int const num);
  SSF_STRING substr (unsigned int const position, unsigned int const num) const;
};
struct sttfont_prerendered_text
{
  int width;
  int height;
  sttfont_prerendered_text ();
  virtual void freeTexture ();
  virtual int draw (int const x, int const y);
  virtual int drawWithColorMod (int const x, int const y, uint8_t const r, uint8_t const g, uint8_t const b, uint8_t const a = 255);
};
struct sttfont_glyph
{
  int advance;
  int leftSideBearing;
  int width;
  int height;
  int xOffset;
  int yOffset;
  sttfont_glyph ();
};
struct sttfont_memory
{
  char * data;
  bool ownsData;
  void alloc (size_t const size);
  void transferTo (sttfont_memory & destination);
  sttfont_memory ();
  ~ sttfont_memory ();
};
struct sttfont_font_list
{
  stbtt_fontinfo mFont;
  sttfont_memory mMemory;
  uint8_t format;
  SSF_VECTOR <sttfont_font_list*> mFormatedVariants;
  sttfont_font_list * next;
  sttfont_font_list ();
  ~ sttfont_font_list ();
  void fetchFontForCodepoint (uint32_t const codepoint, uint8_t const format, stbtt_fontinfo * * mFontOut, int * indexOut);
};
class sttfont_font_cache
{
public:
  sttfont_font_list mFont;
  int ascent;
  int descent;
  int lineGap;
  int baseline;
  int rowSize;
  int tabWidth;
  float scale;
  float underlineThickness;
  float strikethroughThickness;
  float underlinePosition;
  float strikethroughPosition;
  int faceSize;
  int tabWidthInSpaces;
  sttfont_font_cache ();
  virtual ~ sttfont_font_cache ();
  void setFaceSize (int const _faceSize);
  int getScaledRowSize () const;
  void loadFont (char const * ttf_buffer, int index = 0);
  void loadFontManaged (sttfont_memory & memory, int index = 0);
  void addFont (char const * ttf_buffer, int index = 0);
  void addFontManaged (sttfont_memory & memory, int index = 0);
  void addFormatFont (uint8_t formatMask, char const * ttf_buffer, int index = 0);
  void addFormatFontManaged (uint8_t formatMask, sttfont_memory & memory, int index = 0);
protected:
  struct addFontWrap
  {
    char const * ttf_buffer;
    sttfont_memory * memory;
    int index;
    addFontWrap (char const * c);
  };
  void addFont_worker (addFontWrap & fwm, bool isFormatVariant, uint8_t formatMask = 0);
public:
  void genGlyph (uint32_t const codepoint, uint8_t const format, sttfont_glyph * gOut);
  virtual void genGlyph_writeData (sttfont_glyph * gOut, unsigned char * bitmap2, int w, int h);
  virtual sttfont_glyph * getGlyph (uint64_t const target);
  sttfont_glyph * getGenGlyph (uint32_t const codepoint, uint8_t const format);
  virtual sttfont_glyph * genGlyph_createAndInsert (uint64_t const target, uint32_t const codepoint, uint8_t const format);
  int getKerningAdvance (uint32_t const cp1, uint32_t const cp2);
  int utf8_charsize (char const * c);
  uint32_t utf8_read (char const * c, uint32_t & seek, uint32_t const maxLen);
  int drawText (int const x, int const y, char const * c, uint32_t const maxLen = -1);
  int drawText (int const x, int const y, SSF_STRING const & str);
  int drawText (int const x, int const y, int & widthOut, int & heightOut, char const * c, uint32_t const maxLen = -1);
  int drawText (int const x, int const y, SSF_STRING const & str, int & widthOut, int & heightOut);
  int drawText (int const x, int const y, sttfont_formatted_text const & text);
  int drawText (int const x, int const y, sttfont_formatted_text const & text, int & widthOut, int & heightOut);
  int getTextSize (int & w, int & h, char const * c, uint32_t const maxLen = -1);
  int getTextSize (int & w, int & h, SSF_STRING const & str);
  int getTextSize (int & w, int & h, sttfont_formatted_text const & str);
  int getNumberOfRows (SSF_STRING const & str);
  int getNumberOfRows (sttfont_formatted_text const & str);
  int getTextHeight (SSF_STRING const & str);
  int getTextHeight (sttfont_formatted_text const & str);
  int processString (int const x, int const y, char const * c, uint32_t const maxLen, sttfont_format const * const format, bool const isDrawing, int * const widthOut = NULL, int * const heightOut = NULL, int const * const threshX = NULL, int const * const threshY = NULL, int * const caretPosition = NULL, int initialXOffset = 0);
  int processFormatted (sttfont_formatted_text const & text, int x, int y, bool const isDrawing, int * const widthOut = NULL, int * const heightOut = NULL, int const * const threshX = NULL, int const * const threshY = NULL, int * const caretPosition = NULL, int initialXOffset = 0);
  int getCaretPos (SSF_STRING const & str, int const relMouseX, int const relMouseY);
  int getCaretPos (sttfont_formatted_text const & str, int const relMouseX, int const relMouseY);
  bool isTofu (sttfont_glyph * G);
  sttfont_glyph * getGlyphOrTofu (uint32_t const codepoint, uint8_t const format);
  virtual void processCodepoint (int & x, int & y, uint32_t const codepoint, sttfont_format const * const format, bool isDrawing, int kerningAdv, int & overdraw);
  virtual void renderTextToObject (sttfont_prerendered_text * textOut, char const * c, uint32_t const maxLen = -1);
  virtual void renderTextToObject (sttfont_prerendered_text * textOut, SSF_STRING const & str);
  virtual void renderTextToObject (sttfont_prerendered_text * textOut, sttfont_formatted_text const & str);
};
template <typename TT, typename RR>
void sttfont_formatted_text::remove_substr_worker1 (TT & tt, unsigned int const position, unsigned int const num, RR * const writeOut)
                                                                                                                                    {
		unsigned int numToRemove = num;
		unsigned int removeIndex = -1;
		unsigned int workingLen = 0;
		for (unsigned int  i = 0; i < tt.mItems.size(); ++i ) {
			if (tt.mItems[i].text.size() + workingLen > position && workingLen <= position) {
				removeIndex = i;
				break;
				}
			workingLen += tt.mItems[i].text.size();
			}
		if (removeIndex >= tt.mItems.size()) return; // not found
		
		int offset = position - workingLen;	
		
		for (unsigned int i = removeIndex; i < tt.mItems.size(); ++i) {
			unsigned int nToRemove = tt.mItems[i].text.size() - offset;
			if (nToRemove > numToRemove) nToRemove = numToRemove;
			
			remove_substr_worker2(tt, i, offset, nToRemove, writeOut);
			
			numToRemove -= nToRemove;
			offset = 0;
			}
			
		}
#undef LZZ_INLINE
#endif
