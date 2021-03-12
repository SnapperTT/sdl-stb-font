// HOW TO COMPILE:
// - Include the bgfx headers
// - Indlude sdl headers
// - Link to both bgfx and sdl
// 
// I use:
//g++ -O3 -g -I . -I extlib/include/bgfx/bgfx bgfxExample.cpp -o bgfxExample -ldl -lGL -lX11 libbxDebug.a libbgfxDebug.a libbimgDebug.a `sdl2-config --libs`
//

#include <iostream>
#include <fstream>

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>
#include <bx/math.h>
#include <bx/mutex.h>
#include <bx/thread.h>
#include <bx/error.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#define SDL_STB_FONT_IMPL
#include <sdlStbFont.h>
#include <bgfxFrontend.h>

/// Opens a file and stores it in a stb_memory for automatic management
void readFileRaw_toMemory (const std::string & fullPath, sttfont_memory & mem) {
	std::ifstream fs(fullPath.c_str(), std::ios::in | std::ios::binary);
	
	if (!fs.is_open()) {
		std::cout << "readFileRaw: " << fullPath << " -- " << "WARNING: Could not open file." << std::endl;
		return;
		}
	else {
		std::cout << "Opened! " << fullPath << std::endl;
		}
				
	fs.seekg (0, std::ios::end);
	const size_t LEN = fs.tellg();
	fs.seekg (0, std::ios::beg);
	
	mem.alloc(LEN);
	fs.read(mem.data, LEN);
	fs.close();
	}
	
void initView2D (const bgfx::ViewId viewId, const char * tag, const uint vw, const uint vh, const bgfx::FrameBufferHandle fh, const bool doClear = true, const bool unitOrthoMatrix = true, const bool forwardZ = true) {
	float orthoProj[16];	// Ortho matrix for rendering screenspace quads
	if (unitOrthoMatrix)
		bx::mtxOrtho(orthoProj, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 100.0f, 0.0f, bgfx::getCaps()->homogeneousDepth); // flip bottom 0.0f and viewHeight if you don't like bottom left coordinates
	else
		bx::mtxOrtho(orthoProj, 0.0f, vw, vh, 0.0f, 0.0f, 100.0f, 0.0f, bgfx::getCaps()->homogeneousDepth);
	
	bgfx::setViewName(viewId, tag);
	bgfx::setViewRect(viewId, 0, 0, vw, vh);
	bgfx::setViewTransform(viewId, NULL, orthoProj);
	bgfx::setViewFrameBuffer(viewId, fh);
	
	const uint16_t clearFlags = doClear ? (BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH) : 0;
	const float clearDepth = (forwardZ ? 1.0f : 0.0f);
	const uint8_t clearStencil = 0;
	const uint32_t clearColour = 0x444444ff;
	bgfx::setViewClear(viewId, clearFlags, clearColour, clearDepth, clearStencil);
	}
	
bool initSdlWindow (SDL_Window * _window) {
	SDL_SysWMinfo wmi;
	SDL_VERSION(&wmi.version);
	if (!SDL_GetWindowWMInfo(_window, &wmi) ) {
		return false;
		}

	bgfx::PlatformData pd;
	#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
		pd.ndt          = wmi.info.x11.display;
		pd.nwh          = (void*)(uintptr_t)wmi.info.x11.window;
	#elif BX_PLATFORM_OSX
		pd.ndt          = NULL;
		pd.nwh          = wmi.info.cocoa.window;
	#elif BX_PLATFORM_WINDOWS
		pd.ndt          = NULL;
		pd.nwh          = wmi.info.win.window;
	#elif BX_PLATFORM_STEAMLINK
		pd.ndt          = wmi.info.vivante.display;
		pd.nwh          = wmi.info.vivante.window;
	#endif // BX_PLATFORM_
	pd.context      = NULL;
	pd.backBuffer   = NULL;
	pd.backBufferDS = NULL;
	bgfx::setPlatformData(pd);
	return true;
	}
	
bool initSdlWindowAndBgfx (SDL_Window * _window, bgfx::Init * _init) {
	if (!initSdlWindow (_window))
		return false;
		
	int ww;
	int wh;
	SDL_GetWindowSize(_window, &ww, &wh);
	
	const uint32_t m_resetFlags = BGFX_RESET_VSYNC | BGFX_RESET_MAXANISOTROPY | BGFX_RESET_FLIP_AFTER_RENDER ;// | BGFX_RESET_MSAA_X4;
	
	bgfx::renderFrame();
	
	bgfx::init(*_init);
	bgfx::reset(ww,wh, m_resetFlags);

	// Enable debug text.
	//if (_init->debug)
	//	bgfx::setDebug(BGFX_DEBUG_TEXT | BGFX_DEBUG_WIREFRAME);

	// Set view 0 clear state.
	bgfx::setViewClear(0
		, BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
		, 0x303030ff
		, 1.0f
		, 0
		);
		
	return true;
	}
	
void viewAtlas(bgfx_stb_font_cache & fc) {
	bgfx::setUniform(bgfxsfh::u_colour, bgfxsfh::toVec4(255,0,0,255).v);
	bgfxsfh::rect r;
	r.x = 650;
	r.y = 50;
	r.w = 450;
	r.h = 450;
	bgfxsfh::pushUntexturedQuad(r);
	bgfx::setState(bgfxsfh::RENDER_STATE);
	bgfx::submit(0, bgfxsfh::untexturedProgram);
	
	
	bgfx::setUniform(bgfxsfh::u_colour, bgfxsfh::toVec4(255,255,255,255).v);
	r.x = 650;
	r.y = 50;
	r.w = 450;
	r.h = 450;

	static int i = 0;
	i++;
	int pageIndex = (i/40) % fc.mAtlases.size();

	bgfxsfh::rect rt;
	rt.x = 0;
	rt.y = 0;
	rt.w = 1;
	rt.h = 1;
	bgfx::setTexture(0, bgfxsfh::s_texture, fc.mAtlases[pageIndex].mAtlasTexture);
	bgfxsfh::pushTexturedQuad(r, rt, true);
	bgfx::setState(bgfxsfh::RENDER_STATE);
	bgfx::submit(0, bgfxsfh::texturedProgram);
	}

int main(int argc, char**argv) {
	// Init SDL & BGFX
	int width = 1200;
	int height = 600;
	
	SDL_Init(0);
	
	SDL_Window * mWindow = SDL_CreateWindow("BGFXH Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	
	bgfx::Init init;
	//init.type     = bgfx::RendererType::Count;
	init.type     = bgfx::RendererType::OpenGL;
	//init.type     = bgfx::RendererType::Vulkan;
	init.vendorId = BGFX_PCI_ID_NONE;
	init.debug  = true;
	init.profile = true;
	init.resolution.width  = width;
	init.resolution.height = height;
	init.resolution.reset  = 0;//m_resetFlags;
	
	initSdlWindowAndBgfx (mWindow, &init); // Creates a bgfx context in an sdl window
	
	// Load the font
	bgfx_stb_font_cache fc;
	
	sttfont_memory notoSans;
	readFileRaw_toMemory("build/fonts/NotoSans-Regular.ttf", notoSans);
	fc.faceSize = 40;
	//fc.mAtlasSize = 1337; // Can be used to change the atlas texture size
	fc.loadFontManaged(notoSans);
	
	fc.bindRenderer(0);	// Attach fc to bgfx, using view #0
	
	
	sttfont_formatted_text formattedText;
	formattedText << "The quick " << sttfont_format::color(155,75,5) << "brown "
				<< sttfont_format::FORMAT_UNDERLINE << "fox " << sttfont_format::FORMAT_STRIKETHROUGH << "jumped " << "over the lazy log";
	
	
	// Caching characters!
	// As the bgfx frontend uses an atlas this is benificial
	std::vector<sttfont_uint32_t_range> r;
	sttfont_uint32_t_range::populateRangesLatin(r);
	sttfont_uint32_t_range::populateRangesCyrillic(r);
	fc.pregenGlyphs(r, 0);
	
	// Prerendering!
	// Warning: renderTextToObject() will mess with views + call bgfx::frame
	// 			if you are using this in your main loop then
	//			you should call it after bgfx::frame()
	bgfx_stb_prerendered_text prt;
	fc.renderTextToObject(&prt, formattedText);
	bgfx_stb_prerendered_text prt2;
	fc.renderTextToObject(&prt2, "prerendered row 2");
	bgfx_stb_prerendered_text prt3;
	fc.renderTextToObject(&prt3, "(prerendered with\nnewline)");
	
	// You now have a window + a bgfx context!
	while (true) {
		SDL_Event ev;
		while (SDL_PollEvent(&ev)) {
			switch (ev.type) {
				case SDL_QUIT:
					return 1;
					break;
				}
			}
			
		initView2D(0, "text test", width, height, BGFX_INVALID_HANDLE, true, false);
		//bgfx::setViewMode(0, bgfx::ViewMode::Sequential);
		
		// Uncomment to view atlas
		viewAtlas(fc);
		
		// Direct rendering
		fc.drawText( 5, 5, "Hello world!\t\tПривет!");
		fc.drawText( 5,45, formattedText);
		
		
		// Rendering prerendered
		prt.draw(0, 5, 85); // view #0, coords (5,85)
		prt2.draw(0, 5, 125); // view #0, coords (5,85)
		prt3.draw(0, 5, 165); // view #0, coords (5,85)
		
		
		bgfx::frame();
		SDL_Delay(15);
		}
		
	return 1;
	}
