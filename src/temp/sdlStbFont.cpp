// sdlStbFont.cpp
//

#include "sdlStbFont.hh"
#include <stdio.h>

#ifndef STB_TRUETYPE_INCLUDE_HANDLED
	////////////////////////////////////////
	// STB TRUETYPE 
	#define STB_TRUETYPE_IMPLEMENTATION
	#include <math.h>
	#include <string.h>
	#include <assert.h>
	#include <stdlib.h>
	#include "stb_truetype.h"
	////////////////////////////////////////
#endif

#include <SDL2/SDL.h>

#define LZZ_INLINE inline
sdl_stb_prerendered_text::sdl_stb_prerendered_text ()
  : mSdlTexture (NULL), width (0), height (0)
                                                                             {}
void sdl_stb_prerendered_text::freeTexture ()
                            {
		if (mSdlTexture)
			SDL_DestroyTexture(mSdlTexture);
		mSdlTexture = NULL;
		}
void sdl_stb_prerendered_text::draw (SDL_Renderer * mRenderer, int const x, int const y)
                                                                       {
		SDL_Rect r;
		r.x = x;
		r.y = y;
		r.w = width;
		r.h = height;
		SDL_RenderCopy(mRenderer, mSdlTexture, NULL, &r);
		}
void sdl_stb_prerendered_text::drawWithColorMod (SDL_Renderer * mRenderer, int const x, int const y, uint8_t const r, uint8_t const g, uint8_t const b, uint8_t const a)
                                                                                                                                                            {
		SDL_SetTextureColorMod(mSdlTexture, r, g, b);
		if (a != 255)
			SDL_SetTextureAlphaMod(mSdlTexture, a);
		draw (mRenderer, x, y);
		}
void sdl_stb_memory::alloc (size_t const size)
                                      {
		data = SSF_NEW_ARR(char, size);
		ownsData = true;
		}
void sdl_stb_memory::transferTo (sdl_stb_memory & destination)
                                                      {
		destination.data = data;
		destination.ownsData = destination.data;
		ownsData = false;
		}
sdl_stb_memory::sdl_stb_memory ()
  : data (NULL), ownsData (false)
                                                        {}
sdl_stb_memory::~ sdl_stb_memory ()
                           {
		if (ownsData) {
			SSF_DEL_ARR(data);
			data = NULL;
			}
		}
sdl_stb_glyph::sdl_stb_glyph ()
  : mSdlTexture (0), mSdlSurface (0), advance (0), leftSideBearing (0), width (0), height (0), xOffset (0), yOffset (0)
                                                                                                                                       {}
sdl_stb_font_list::sdl_stb_font_list ()
  : next (NULL)
                                         {}
sdl_stb_font_list::~ sdl_stb_font_list ()
                             {
		if (next) delete next;
		}
void sdl_stb_font_list::fetchFontForCodepoint (uint32_t const codepoint, stbtt_fontinfo * * mFontOut, int * indexOut)
                                                                                                         {
		sdl_stb_font_list * working = this;
		
		*mFontOut = NULL;
		*indexOut = 0;
		
		while (working) {
			int index = stbtt_FindGlyphIndex(&(working->mFont), codepoint);
			if (index) {
				*mFontOut = &(working->mFont);
				*indexOut = index;
				return;
				}
			working = working->next;
			}
		}
sdl_stb_font_cache::sdl_stb_font_cache ()
  : mRenderer (NULL), ascent (0), descent (0), lineGap (0), baseline (0), rowSize (0), scale (1.f), faceSize (20)
                                                                                                                                      {}
sdl_stb_font_cache::~ sdl_stb_font_cache ()
                               {
		clearGlyphs();
		}
void sdl_stb_font_cache::clearGlyphs ()
                            {
		for (auto & g : mGlyphs) {
			if (g.second.mSdlTexture) {
				SDL_DestroyTexture(g.second.mSdlTexture);
				g.second.mSdlTexture = NULL;
				}
			if (g.second.mSdlSurface) {
				SDL_FreeSurface(g.second.mSdlSurface);
				g.second.mSdlSurface = NULL;
				}
			}
		mGlyphs.clear();
		}
void sdl_stb_font_cache::bindRenderer (SDL_Renderer * _mRenderer)
                                                     {
		mRenderer = _mRenderer;
		}
void sdl_stb_font_cache::setFaceSize (int const _faceSize)
                                              { faceSize = _faceSize; }
int sdl_stb_font_cache::getScaledRowSize () const
                                      { return scale * rowSize; }
void sdl_stb_font_cache::loadFont (char const * ttf_buffer, int index)
                                                               {
		stbtt_InitFont(&mFont.mFont, (const unsigned char *) ttf_buffer, stbtt_GetFontOffsetForIndex((const unsigned char *) ttf_buffer,index));
		stbtt_GetFontVMetrics(&mFont.mFont, &ascent, &descent, &lineGap);
		
		scale = stbtt_ScaleForPixelHeight(&mFont.mFont, faceSize);
		baseline = ascent*scale;
		rowSize = ascent - descent + lineGap;
		}
void sdl_stb_font_cache::loadFontManaged (sdl_stb_memory & memory, int index)
                                                                      {
		memory.transferTo(mFont.mMemory);
		loadFont(mFont.mMemory.data, index);
		}
void sdl_stb_font_cache::addFont (char const * ttf_buffer, int index)
                                                              {
		sdl_stb_font_list * n = SSF_NEW(sdl_stb_font_list);
		sdl_stb_font_list * w = &mFont;
		while (w->next)
			w = w->next;
		
		stbtt_InitFont(&n->mFont, (const unsigned char *) ttf_buffer, stbtt_GetFontOffsetForIndex((const unsigned char *) ttf_buffer,index));
		w->next = n;
		}
void sdl_stb_font_cache::addFontManaged (sdl_stb_memory & memory, int index)
                                                                     {
		sdl_stb_font_list * n = SSF_NEW(sdl_stb_font_list);
		sdl_stb_font_list * w = &mFont;
		while (w->next)
			w = w->next;
		
		memory.transferTo(n->mMemory);
		stbtt_InitFont(&n->mFont, (const unsigned char *) n->mMemory.data, stbtt_GetFontOffsetForIndex((const unsigned char *) n->mMemory.data,index));
		w->next = n;
		}
void sdl_stb_font_cache::genGlyph (uint32_t const codepoint, sdl_stb_glyph * gOut)
                                                                       {
		// Fetch font and index - existance check for glyph in font
		stbtt_fontinfo * mFontContaining;
		int mIndex;
		mFont.fetchFontForCodepoint(codepoint, &mFontContaining, &mIndex);
		
		if (!mIndex)
			return;
		
		// found the font! generate the glyph
	   	unsigned char *bitmap;
	   	int w,h,woff,hoff;
		bitmap = stbtt_GetCodepointBitmap(mFontContaining, 0, scale, codepoint, &w, &h, &woff, &hoff);
		
        // Convert bitmap to RGBA
        unsigned int sz = w*h;
        unsigned char bitmap2[sz*4];
        for (unsigned int i = 0; i < sz; ++i) {
        	bitmap2[i*4+0] = 255;
        	bitmap2[i*4+1] = 255;
        	bitmap2[i*4+2] = 255;
        	bitmap2[i*4+3] = bitmap[i];
        	}
        
        stbtt_FreeBitmap (bitmap, 0);
        
        if (w && h) {
			gOut->mSdlSurface = SDL_CreateRGBSurfaceFrom(bitmap2, w, h, 32, 4*w, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
			gOut->mSdlTexture = SDL_CreateTextureFromSurface(mRenderer, gOut->mSdlSurface);
			}
		
		gOut->width = w;
		gOut->height = h;
		stbtt_GetCodepointHMetrics(mFontContaining, codepoint, &gOut->advance, &gOut->leftSideBearing);
		
		gOut->xOffset = woff;
		gOut->yOffset = hoff;
		
		}
sdl_stb_glyph * sdl_stb_font_cache::getGlyph (uint32_t const codepoint)
                                                           {
		auto it = mGlyphs.find(codepoint);
		if (it == mGlyphs.end())
			return NULL;
		return &((*it).second);
		}
sdl_stb_glyph * sdl_stb_font_cache::getGenGlyph (uint32_t const codepoint)
                                                              {
		sdl_stb_glyph * r = getGlyph(codepoint);
		if (r) return r;
		sdl_stb_glyph g;
		genGlyph(codepoint, &g);
		mGlyphs[codepoint] = g;
		return getGlyph(codepoint);
		}
int sdl_stb_font_cache::getKerningAdvance (uint32_t const cp1, uint32_t const cp2)
                                                                      {
		return stbtt_GetCodepointKernAdvance(&mFont.mFont, cp1, cp2);
		}
int sdl_stb_font_cache::utf8_charsize (char const * c)
                                         {
		if (!c) return 0;
		if ((uint8_t)*c <= 0x7F) return 1;
		else if ((uint8_t)*c <= 0xE0) return 2;
		else if ((uint8_t)*c <= 0xF0) return 3;
		else
			return 4;
		}
uint32_t sdl_stb_font_cache::utf8_read (char const * c, uint32_t & seek, uint32_t const maxLen)
                                                                                  {
		if (!c) return 0;
		int chsz = utf8_charsize(c);
		seek += chsz;
	
		if (seek > maxLen) {
			return *c; //Buffer overflow - stop to be safe!
			}
			
		if (chsz == 1) return *c;		
		if (chsz == 2)
			return (((uint32_t((uint8_t) c[0] & 0b00111111) << 6)) | uint32_t((uint8_t) c[1] & 0b00111111));
		if (chsz == 3)
			return (uint32_t((uint8_t) c[0] & 0b00011111) << 12) | (uint32_t((uint8_t) c[1] & 0b00111111) << 6) | uint32_t((uint8_t) c[2] & 0b00111111);
		return (uint32_t((uint8_t) c[0] & 0b00001111) << 18) | (uint32_t((uint8_t) c[1] & 0b00111111) << 12) | (uint32_t((uint8_t) c[2] & 0b00111111) << 6) | uint32_t((uint8_t) c[3] & 0b00111111);
		}
void sdl_stb_font_cache::drawText (int const x, int const y, char const * c, uint32_t const maxLen)
                                                                                            {
		processString(x, y, c, maxLen, true);
		}
void sdl_stb_font_cache::drawText (int const x, int const y, SSF_STRING const & str)
                                                                         {
		drawText(x,y,str.data(),str.size());
		}
void sdl_stb_font_cache::getTextSize (int & w, int & h, char const * c, uint32_t const maxLen)
                                                                                        {
		processString(0, 0, c, maxLen, false, &w, &h);
		}
void sdl_stb_font_cache::getTextSize (int & w, int & h, SSF_STRING const & str)
                                                                    {
		processString(0, 0, str.data(), str.size(), false, &w, &h);
		}
int sdl_stb_font_cache::getNumberOfRows (SSF_STRING const & str)
                                                     {
		int n = 1;
		for (const char c : str)
			if (c == '\n') n++;
		return n;
		}
int sdl_stb_font_cache::getTextHeight (SSF_STRING const & str)
                                                   {
		return scale*rowSize*getNumberOfRows(str);
		}
void sdl_stb_font_cache::processString (int const x, int const y, char const * c, uint32_t const maxLen, bool const isDrawing, int * const widthOut, int * const heightOut, int const * const threshX, int const * const threshY, int * const caretPosition)
                                                                                                                                                                                                                                                                                   {
		// Scan through function and extract the glyphs
		uint32_t seek = 0;
		uint32_t uCharLast = 0;
		uint32_t uChar = utf8_read(c, seek, maxLen);
		int xx = x;
		int yy = y;
		if (widthOut) *widthOut = 0;
		if (heightOut) *heightOut = 0;
		
		const bool lookupCaret = caretPosition && threshX && threshY;
		if (lookupCaret) {
			*caretPosition = -1;
			}
		
		while (uChar && seek <= maxLen) {
			int xxl = xx;
			if (uChar == '\n') {
				if (widthOut)
					if (*widthOut < xx) *widthOut = xx;
				xx = x;
				yy += scale*rowSize;
				}
			else {
				xx += scale*getKerningAdvance(uCharLast, uChar);
				processCodepoint(xx, yy, uChar, isDrawing);
				}
			
			if (lookupCaret) {
				const int dx = xx - x;
				if (xx > *threshX && xxl <= *threshX ) {
					if (*threshX > xxl + (xx - xxl)/2)
						*caretPosition = seek; // right half of char
					else
						*caretPosition = seek-1; // left half of char
					return;
					}
				}
				
			uCharLast = uChar;
			uChar = utf8_read(c + seek, seek, maxLen);
			}
		if (widthOut) {
			if (*widthOut < xx) *widthOut = xx;
			*widthOut -= x;
			}
		if (heightOut) {
			if (*heightOut < yy) *heightOut = yy;
			*heightOut += scale*rowSize;
			*heightOut -= y;
			}
		}
int sdl_stb_font_cache::getCaretPos (SSF_STRING const & str, int const relMouseX, int const relMouseY)
                                                                                          {
		int caretPosition = -1;
		processString(0,0, str.data(), str.length(), false, NULL, NULL, &relMouseX, &relMouseY, &caretPosition);
		return caretPosition;
		}
bool sdl_stb_font_cache::isTofu (sdl_stb_glyph * G)
                                        {
		if (!G) return true;
		if (!G->advance) return true; //unprintable characters have no width
		return false;
		}
sdl_stb_glyph * sdl_stb_font_cache::getGlyphOrTofu (uint32_t const codepoint)
                                                                  {
		sdl_stb_glyph * G = getGenGlyph(codepoint);
		if (!isTofu(G)) return G;
		
		G = getGenGlyph((uint32_t) 0xFFFD); // https://en.wikipedia.org/wiki/Specials_(Unicode_block)#Replacement_character
		if (!isTofu(G)) return G;
			
		 G = getGenGlyph((uint32_t) '?');
		if (!isTofu(G)) return G;
		return NULL;
		}
void sdl_stb_font_cache::processCodepoint (int & x, int & y, uint32_t const codepoint, bool isDrawing)
                                                                                           {
		// Draws the character, advances x & y to the next position
		// NOTE: KErning
		sdl_stb_glyph * G = getGlyphOrTofu(codepoint);
		if (!G) {
			x += faceSize/2;
			return;
			}
		if (isDrawing) {
			SDL_Rect r;
			r.x = x + G->xOffset;
			r.y = y + G->yOffset + baseline;
			r.w = G->width;
			r.h = G->height;
			if (G->mSdlTexture)
				SDL_RenderCopy(mRenderer, G->mSdlTexture, NULL, &r);
			}
		x += scale*G->advance;
		}
SDL_Texture * sdl_stb_font_cache::renderTextToTexture (char const * c, uint32_t const maxLen, int * widthOut, int * heightOut)
                                                                                                                                      {
		int width, height;
		getTextSize(width, height, c, maxLen);
		
		SDL_Texture * RT = SDL_CreateTexture(mRenderer, SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET, width, height);
		SDL_SetRenderTarget(mRenderer, RT);
		SDL_SetTextureBlendMode(RT, SDL_BLENDMODE_NONE);
		SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 0); // Must be the same colour as the text
		// Fill RT with blank pixels. SDL_RenderClear will leave artefacts in SDL Software mode
		SDL_Rect r;
		r.x = 0; r.y = 0;
		r.w = width; r.h = height;
		SDL_RenderFillRect (mRenderer, &r); // Must be rendered with a fill rect
		
		SDL_SetTextureBlendMode(RT, SDL_BLENDMODE_BLEND);
		drawText(0, 0, c, maxLen);
		SDL_SetRenderTarget(mRenderer, NULL);
		
		*widthOut = width;
		*heightOut = height;
		return RT;
		}
SDL_Texture * sdl_stb_font_cache::renderTextToTexture (SSF_STRING const & str, int * widthOut, int * heightOut)
                                                                                                                  {
		return renderTextToTexture(str.data(), str.length(), widthOut, heightOut);
		}
void sdl_stb_font_cache::renderTextToObject (sdl_stb_prerendered_text * textOut, char const * c, uint32_t const maxLen)
                                                                                                                 {
		textOut->mSdlTexture = renderTextToTexture(c, maxLen, &(textOut->width), &(textOut->height));
		}
void sdl_stb_font_cache::renderTextToObject (sdl_stb_prerendered_text * textOut, SSF_STRING const & str)
                                                                                             {
		textOut->mSdlTexture = renderTextToTexture(str, &(textOut->width), &(textOut->height));
		}
#undef LZZ_INLINE
