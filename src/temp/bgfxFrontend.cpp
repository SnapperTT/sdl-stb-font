// bgfxFrontend.cpp
//

#include "bgfxFrontend.hh"
#define LZZ_INLINE inline
bgfx_stb_prerendered_text::bgfx_stb_prerendered_text ()
  : sttfont_prerendered_text (), mBgfxTexture (BGFX_INVALID_HANDLE)
                                                                                                     {}
void bgfx_stb_prerendered_text::freeTexture ()
                            {
		if (bgfx::isValid(mBgfxTexture))
			bgfx::destroy(mBgfxTexture);
		mBgfxTexture = BGFX_INVALID_HANDLE;
		}
int bgfx_stb_prerendered_text::draw (bgfx::ViewId mViewId, int const x, int const y)
                                                                  {
		bgfxsfh::rect r;
		r.x = x;
		r.y = y;
		r.w = width;
		r.h = height;
		
		bgfx::setUniform(bgfxsfh::s_texture, &mBgfxTexture);
		bgfxsfh::pushTexturedQuad(x,y,width,height);
		bgfx::submit(mViewId, bgfxsfh::texturedProgram);
		return r.x + r.w;
		}
int bgfx_stb_prerendered_text::drawWithColorMod (bgfx::ViewId mViewId, int const x, int const y, uint8_t const r, uint8_t const g, uint8_t const b, uint8_t const a)
                                                                                                                                                       {
		bgfx::setUniform(bgfxsfh::u_colour, bgfxsfh::toVec4(r/255.0,g/255.0,b/255.0,a/255.0));
		return draw (mViewId, x, y);
		}
bgfx::ShaderHandle bgfxsfh::vert_passthrough = BGFX_INVALID_HANDLE;
bgfx::ShaderHandle bgfxsfh::frag_passthrough = BGFX_INVALID_HANDLE;
bgfx::ShaderHandle bgfxsfh::textured_vert_passthrough = BGFX_INVALID_HANDLE;
bgfx::ShaderHandle bgfxsfh::textured_frag_passthrough = BGFX_INVALID_HANDLE;
bgfx::ProgramHandle bgfxsfh::untexturedProgram = BGFX_INVALID_HANDLE;
bgfx::ProgramHandle bgfxsfh::texturedProgram = BGFX_INVALID_HANDLE;
bgfx::UniformHandle bgfxsfh::u_colour = BGFX_INVALID_HANDLE;
bgfx::UniformHandle bgfxsfh::s_texture = BGFX_INVALID_HANDLE;
int bgfxsfh::refCount = 0;
float * bgfxsfh::toVec4 (uint8_t const a, uint8_t const b, uint8_t const c, uint8_t const d)
                                                                                                 {
		static float r[4];
		r[0] = a;
		r[1] = b;
		r[2] = c;
		r[3] = d;
		return r;
		}
void bgfxsfh::initialise ()
                                 {
		if (refCount == 0) {
			#include "bgfxFrontendShaders.h"
			
			static const bgfx::EmbeddedShader s_embeddedShaders[] = {
				BGFXH_EMBEDDED_SHADER(fs_textured_passthrough_bin),
				BGFXH_EMBEDDED_SHADER(vs_textured_passthrough_bin),
				BGFXH_EMBEDDED_SHADER(fs_untextured_passthrough_bin),
				BGFXH_EMBEDDED_SHADER(vs_untextured_passthrough_bin),
				
				BGFX_EMBEDDED_SHADER_END()
				};
				
			
			bgfx::RendererType::Enum type = bgfx::getRendererType();
			vert_passthrough = bgfx::createEmbeddedShader(s_embeddedShaders, type, "vs_textured_passthrough_bin");
			frag_passthrough = bgfx::createEmbeddedShader(s_embeddedShaders, type, "fs_textured_passthrough_bin");
			textured_vert_passthrough = bgfx::createEmbeddedShader(s_embeddedShaders, type, "vs_untextured_passthrough_bin");
			textured_frag_passthrough = bgfx::createEmbeddedShader(s_embeddedShaders, type, "fs_untextured_passthrough_bin");
			
			untexturedProgram = bgfx::createProgram(vert_passthrough, frag_passthrough, false);
			texturedProgram = bgfx::createProgram(vert_passthrough, textured_frag_passthrough, false);
			
			u_colour = bgfx::createUniform("u_colour", bgfx::UniformType::Vec4);
			s_texture = bgfx::createUniform("s_texture", bgfx::UniformType::Sampler);
			}
		refCount++;
		}
void bgfxsfh::deinitialise ()
                                   {
		refCount--;
		if (refCount == 0) {
			bgfx::destroy(untexturedProgram); 	untexturedProgram = BGFX_INVALID_HANDLE;
			bgfx::destroy(texturedProgram); 	texturedProgram = BGFX_INVALID_HANDLE;
			
			bgfx::destroy(vert_passthrough); 	vert_passthrough = BGFX_INVALID_HANDLE;
			bgfx::destroy(frag_passthrough); 	frag_passthrough = BGFX_INVALID_HANDLE;
			bgfx::destroy(textured_frag_passthrough); textured_frag_passthrough = BGFX_INVALID_HANDLE;
			
			bgfx::destroy(u_colour); 			u_colour = BGFX_INVALID_HANDLE;
			bgfx::destroy(s_texture); 			s_texture = BGFX_INVALID_HANDLE;
			}
		}
void bgfxsfh::PosTexCoord0Vertex::init ()
                                   {
			ms_decl.begin()
				.add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
				.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.end();
			}
bgfx::VertexLayout bgfxsfh::PosTexCoord0Vertex::ms_decl;
void bgfxsfh::PosVertex::init ()
                                   {
			ms_decl.begin()
				.add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
			.end();
			}
bgfx::VertexLayout bgfxsfh::PosVertex::ms_decl;
void bgfxsfh::pushTexturedQuad (rect const & r)
                                                     { pushTexturedQuad(r.x, r.y, r.w, r.h); }
void bgfxsfh::pushTexturedQuad (float const xOffset, float const yOffset, float const xSize, float const ySize, float const _framebufferWidth, float const _framebufferHeight)
                                                                                                                                                                                                {
		/*
		* From the BGFX Examples, the following license applies to only this function:
		* Copyright 2011-2018 Branimir Karadzic. All rights reserved.
		* License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
		*/
		const bgfx::RendererType::Enum renderer = bgfx::getRendererType();
		const float _texelHalf = bgfx::RendererType::Direct3D9 == renderer ? 0.5f : 0.0f;
		const bool _originBottomLeft = bgfx::getCaps()->originBottomLeft;
		
		if (6 == bgfx::getAvailTransientVertexBuffer(6, bgfxsfh::PosTexCoord0Vertex::ms_decl) ) {
			bgfx::TransientVertexBuffer vb;
			bgfx::allocTransientVertexBuffer(&vb, 6, bgfxsfh::PosTexCoord0Vertex::ms_decl);
			bgfxsfh::PosTexCoord0Vertex* vertex = (bgfxsfh::PosTexCoord0Vertex*)vb.data;

			const float minx = xOffset;
			const float maxx = (xOffset + xSize);
			const float miny = yOffset;
			const float maxy = (yOffset + ySize);

			float texelHalfW = 0.f;
			float texelHalfH = 0.f;
			if (_framebufferWidth > 0.0f && _framebufferHeight > 0.0f) {
				texelHalfW = _texelHalf/_framebufferWidth;
				texelHalfH = _texelHalf/_framebufferHeight;
				}
			const float minu = 0.0f + texelHalfW;
			const float maxu = 1.0f + texelHalfH;

			const float zz = 0.0f;

			float minv = texelHalfH;
			float maxv = 1.0f + texelHalfH;

			if (_originBottomLeft) {
				float temp = minv;
				minv = maxv;
				maxv = temp;
				}

			vertex[0].m_x = minx;
			vertex[0].m_y = miny;
			vertex[0].m_z = zz;
			vertex[0].m_u = minu;
			vertex[0].m_v = minv;

			vertex[1].m_x = maxx;
			vertex[1].m_y = miny;
			vertex[1].m_z = zz;
			vertex[1].m_u = maxu;
			vertex[1].m_v = minv;

			vertex[2].m_x = maxx;
			vertex[2].m_y = maxy;
			vertex[2].m_z = zz;
			vertex[2].m_u = maxu;
			vertex[2].m_v = maxv;
			
			vertex[3].m_x = minx;
			vertex[3].m_y = miny;
			vertex[3].m_z = zz;
			vertex[3].m_u = minu;
			vertex[3].m_v = minv;

			vertex[4].m_x = maxx;
			vertex[4].m_y = maxy;
			vertex[4].m_z = zz;
			vertex[4].m_u = maxu;
			vertex[4].m_v = maxv;
			
			vertex[5].m_x = minx;
			vertex[5].m_y = maxy;
			vertex[5].m_z = zz;
			vertex[5].m_u = minu;
			vertex[5].m_v = maxv;


			bgfx::setVertexBuffer(0, &vb);
			}
		}
void bgfxsfh::pushUntexturedQuad (rect const & r)
                                                       { pushUntexturedQuad(r.x, r.y, r.w, r.h); }
void bgfxsfh::pushUntexturedQuad (float const xOffset, float const yOffset, float const xSize, float const ySize, float const _framebufferWidth, float const _framebufferHeight)
                                                                                                                                                                                                  {
		/*
		* From the BGFX Examples, the following license applies to only this function:
		* Copyright 2011-2018 Branimir Karadzic. All rights reserved.
		* License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
		*/
		const bgfx::RendererType::Enum renderer = bgfx::getRendererType();
		const float _texelHalf = bgfx::RendererType::Direct3D9 == renderer ? 0.5f : 0.0f;
		const bool _originBottomLeft = bgfx::getCaps()->originBottomLeft;
		
		if (6 == bgfx::getAvailTransientVertexBuffer(6, bgfxsfh::PosVertex::ms_decl) ) {
			bgfx::TransientVertexBuffer vb;
			bgfx::allocTransientVertexBuffer(&vb, 6, bgfxsfh::PosVertex::ms_decl);
			bgfxsfh::PosVertex* vertex = (bgfxsfh::PosVertex*)vb.data;

			const float minx = xOffset;
			const float maxx = (xOffset + xSize);
			const float miny = yOffset;
			const float maxy = (yOffset + ySize);

			float texelHalfW = 0.f;
			float texelHalfH = 0.f;
			if (_framebufferWidth > 0.0f && _framebufferHeight > 0.0f) {
				texelHalfW = _texelHalf/_framebufferWidth;
				texelHalfH = _texelHalf/_framebufferHeight;
				}
			const float minu = 0.0f + texelHalfW;
			const float maxu = 1.0f + texelHalfH;

			const float zz = 0.0f;

			float minv = texelHalfH;
			float maxv = 1.0f + texelHalfH;

			if (_originBottomLeft) {
				float temp = minv;
				minv = maxv;
				maxv = temp;
				}

			vertex[0].m_x = minx;
			vertex[0].m_y = miny;
			vertex[0].m_z = zz;

			vertex[1].m_x = maxx;
			vertex[1].m_y = miny;
			vertex[1].m_z = zz;

			vertex[2].m_x = maxx;
			vertex[2].m_y = maxy;
			vertex[2].m_z = zz;
			
			vertex[3].m_x = minx;
			vertex[3].m_y = miny;
			vertex[3].m_z = zz;

			vertex[4].m_x = maxx;
			vertex[4].m_y = maxy;
			vertex[4].m_z = zz;
			
			vertex[5].m_x = minx;
			vertex[5].m_y = maxy;
			vertex[5].m_z = zz;


			bgfx::setVertexBuffer(0, &vb);
			}
		}
bgfx_stb_glyph::bgfx_stb_glyph ()
  : sttfont_glyph (), mAtlasTexture (BGFX_INVALID_HANDLE), x (0), y (0), w (0), h (0)
                                                                                                        {}
bgfx_stb_glyph_atlas::bgfx_stb_glyph_atlas ()
  : mAtlasTexture (BGFX_INVALID_HANDLE), isFull (false), isNew (true)
                                                                                                {}
bgfx_stb_font_cache::bgfx_stb_font_cache ()
  : sttfont_font_cache (), mViewId (0), mAtlasSize (1024)
                                                                                    {}
bgfx_stb_font_cache::~ bgfx_stb_font_cache ()
                                {		
		clearGlyphs();
		bgfxsfh::deinitialise();
		}
void bgfx_stb_font_cache::clearGlyphs ()
                            {
		for (auto & g : mAtlases)
			bgfx::destroy(g.mAtlasTexture);
			
		mAtlases.clear();
		mGlyphs.clear();
		}
void bgfx_stb_font_cache::bindRenderer ()
                            {
		bgfxsfh::initialise();
		}
bgfx_stb_glyph_atlas * bgfx_stb_font_cache::getGenAtlasPage ()
                                                {
		if (mAtlases.empty()) {
			return createAtlasPage();
			}
		bgfx_stb_glyph_atlas * r = &mAtlases.back();
		if (r->isFull)
			return createAtlasPage();
		return r;
		}
bgfx_stb_glyph_atlas * bgfx_stb_font_cache::createAtlasPage ()
                                                {
		bgfx_stb_glyph_atlas a;
		a.mAtlasTexture = bgfx::createTexture2D(mAtlasSize, mAtlasSize, false, 1, bgfx::TextureFormat::BGRA8, BGFX_TEXTURE_NONE, NULL);
		mAtlases.push_back(a);
		return &mAtlases.back();
		}
void bgfx_stb_font_cache::genGlyph_writeData (uint32_t const codepoint, sttfont_glyph * gOut, unsigned char * bitmap2, int w, int h)
                                                                                                                        {
		genGlyph_writeData2(codepoint, gOut, bitmap2, w, h, true);
		}
void bgfx_stb_font_cache::genGlyph_writeData2 (uint32_t const codepoint, sttfont_glyph * gOut, unsigned char * bitmap2, int w, int h, bool firstCall)
                                                                                                                                         {
		// fetch or create atlas
		bgfx_stb_glyph * bOut = (bgfx_stb_glyph*) gOut;
		bgfx_stb_glyph_atlas * activeAtlas = getGenAtlasPage();
		
		// try to pack
		stbrp_rect r;
		r.id = *((int*) &codepoint);
		r.w = w;
		r.h = h;
		
		stbrp_pack_rects(&activeAtlas->mStbRectPackCtx, &r, 1);
		if (r.was_packed) {
			const bgfx::Memory* mem = bgfx::copy(bitmap2, w*h*4);
			bgfx::updateTexture2D(activeAtlas->mAtlasTexture, 0, 0, r.x, r.y, r.w, r.h, mem, w);
			bOut->x = r.x;
			bOut->y = r.y;
			bOut->w = r.w;
			bOut->h = r.h; 
			return;
			}
		else {
			activeAtlas->isFull = true;
			}
		
		// failed to pack, generate a new atlas
		if (firstCall)
			genGlyph_writeData2(codepoint, gOut, bitmap2, w, h, false);
		}
sttfont_glyph * bgfx_stb_font_cache::getGlyph (uint64_t const target)
                                                        {
		auto it = mGlyphs.find(target);
		if (it == mGlyphs.end())
			return NULL;
		return &((*it).second);
		}
sttfont_glyph * bgfx_stb_font_cache::genGlyph_createAndInsert (uint64_t const target, uint32_t const codepoint, uint8_t const format)
                                                                                                                        {
		bgfx_stb_glyph g;
		genGlyph(codepoint, format, &g);
		mGlyphs[target] = g;
		return getGlyph(target);
		}
void bgfx_stb_font_cache::processCodepoint (int & x, int & y, uint32_t const codepoint, sttfont_format const * const format, bool isDrawing, int kerningAdv, int & overdraw)
                                                                                                                                                                {
		// Draws the character, advances x & y to the next position
		// NOTE: KErning
		uint8_t formatCode = 0;
		if (format)
			formatCode = format->format;
			
		bgfx_stb_glyph * G = (bgfx_stb_glyph*) getGlyphOrTofu(codepoint, formatCode);
		if (!G) {
			x += faceSize/2;
			return;
			}
		if (isDrawing) {
			bgfxsfh::rect r;
			r.x = x + G->xOffset;
			r.y = y + G->yOffset + baseline;
			r.w = G->width;
			r.h = G->height;
			
			if (true) {//bgfx::isValid(G->mAtlasTexture)) {
				if (format) {
					int charAdv = kerningAdv + G->xOffset;
					bool isColoured = (format->r < 255) || (format->g < 255) || (format->b < 255);
					uint8_t cr,cg,cb,ca;
					
					bgfx::setUniform(bgfxsfh::u_colour, bgfxsfh::toVec4(format->r, format->g, format->b, format->a));
					
					if (isColoured || formatCode) {
						// Remove bleeding pixels
						bgfxsfh::rect r2;
						r2.x = r.x; r2.y = r.y;
						r2.w = r.w; r2.h = r.h;
						if (r2.x < overdraw) {
							int dx = overdraw - r2.x;
							r2.x += dx;
							r2.w -= dx;
							}
						overdraw = r.x + r.w;
						
						bgfxsfh::pushUntexturedQuad(r2); //TODO: prevent overlapping!
						bgfx::submit(mViewId, bgfxsfh::untexturedProgram);
						}
					bgfxsfh::pushTexturedQuad(r);
					bgfx::submit(mViewId, bgfxsfh::texturedProgram);
					
					if (formatCode & sttfont_format::FORMAT_STRIKETHROUGH) {
						bgfxsfh::rect r2;
						r2.w = G->width+strikethroughThickness + charAdv; r2.h = strikethroughThickness;
						if (r2.h < 1) r2.h = 1;
						r2.x = r.x-strikethroughThickness/2 - charAdv; r2.y = y + strikethroughPosition;
						
						bgfxsfh::pushUntexturedQuad(r2);
						bgfx::submit(mViewId, bgfxsfh::untexturedProgram);
						}
					if (formatCode & sttfont_format::FORMAT_UNDERLINE) {
						bgfxsfh::rect r2;
						r2.w = G->width+underlineThickness + charAdv; r2.h = underlineThickness;
						if (r2.h < 1) r2.h = 1;
						r2.x = r.x-underlineThickness/2 - charAdv; r2.y = y + underlinePosition;
						
						bgfxsfh::pushUntexturedQuad(r2);
						bgfx::submit(mViewId, bgfxsfh::untexturedProgram);
						}
					}
				else {
					overdraw = SSF_INT_MIN;
					bgfxsfh::pushTexturedQuad(r);
					bgfx::submit(mViewId, bgfxsfh::texturedProgram);
					}
				}
			}
		x += scale*G->advance;
		}
bgfx::TextureHandle bgfx_stb_font_cache::renderTextToTexture (char const * c, uint32_t const maxLen, int * widthOut, int * heightOut)
                                                                                                                                            {
		return renderTextToTexture_worker(NULL, c, maxLen, widthOut, heightOut);
		}
bgfx::TextureHandle bgfx_stb_font_cache::renderTextToTexture (sttfont_formatted_text const & formatted, int * widthOut, int * heightOut)
                                                                                                                                          {
		return renderTextToTexture_worker(&formatted, NULL, -1, widthOut, heightOut);
		}
bgfx::TextureHandle bgfx_stb_font_cache::renderTextToTexture_worker (sttfont_formatted_text const * formatted, char const * c, uint32_t const maxLen, int * widthOut, int * heightOut)
                                                                                                                                                                                             {
		int width, height;
		if (formatted)
			getTextSize(width, height, *formatted);
		else
			getTextSize(width, height, c, maxLen);
		
		/*
		
bgfx::FrameBufferHandle
			bgfx::setViewName(viewId + i, "ShadowPass");
			bgfx::setViewRect(viewId + i, 0, 0, shadowMapSz, shadowMapSz);
			bgfx::setViewFrameBuffer(viewId + i, m_shadowMapFB[i]);
			bgfx::setViewClear(viewId + i, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, shadowClearColour, shadowUseForwardZ ? 1.0f : 0.0f, 0);
			bgfx::setViewTransform(viewId + i, shadowLightView[i], shadowLightProj[i]);
 bgfx::createFrameBuffer(2, shadowProxTex, true);
					uint64_t state = 0
						| BGFX_STATE_WRITE_RGB
						| BGFX_STATE_WRITE_A
						| BGFX_STATE_MSAA
						;
		*/
		
		bgfx::ViewId viewId;
		bgfx::setViewMode(viewId, bgfx::ViewMode::Sequential);
		
		const bgfx::Memory* mem = bgfx::alloc(width*height*4);
		bgfx::TextureHandle RT = bgfx::createTexture2D(width, height, false, 1, bgfx::TextureFormat::BGRA8, BGFX_SAMPLER_NONE, mem);
		bgfx::FrameBufferHandle FB = bgfx::createFrameBuffer(1, &RT, false);
		
		bgfx::setViewFrameBuffer(viewId, FB);
		
		if (formatted)
			drawText(0, 0, *formatted);
		else
			drawText(0, 0, c, maxLen);
			
		bgfx::destroy(FB);
		
		*widthOut = width;
		*heightOut = height;
		return RT;
		}
bgfx::TextureHandle bgfx_stb_font_cache::renderTextToTexture (SSF_STRING const & str, int * widthOut, int * heightOut)
                                                                                                                        {
		return renderTextToTexture(str.data(), str.length(), widthOut, heightOut);
		}
void bgfx_stb_font_cache::renderTextToObject (sttfont_prerendered_text * textOut, char const * c, uint32_t const maxLen)
                                                                                                                 {
		bgfx_stb_prerendered_text * textOut2 = (bgfx_stb_prerendered_text*) textOut;
		textOut2->mBgfxTexture = renderTextToTexture(c, maxLen, &(textOut->width), &(textOut->height));
		}
void bgfx_stb_font_cache::renderTextToObject (sttfont_prerendered_text * textOut, SSF_STRING const & str)
                                                                                             {
		bgfx_stb_prerendered_text * textOut2 = (bgfx_stb_prerendered_text*) textOut;
		textOut2->mBgfxTexture = renderTextToTexture(str, &(textOut->width), &(textOut->height));
		}
void bgfx_stb_font_cache::renderTextToObject (sttfont_prerendered_text * textOut, sttfont_formatted_text const & str)
                                                                                                         {
		bgfx_stb_prerendered_text * textOut2 = (bgfx_stb_prerendered_text*) textOut;
		textOut2->mBgfxTexture = renderTextToTexture(str, &(textOut->width), &(textOut->height));
		}
#undef LZZ_INLINE
