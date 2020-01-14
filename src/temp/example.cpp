// example.cpp
//

#include "example.hh"
#include <string>
#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>

#define SDL_STB_FONT_IMPL
#include "sdlStbFont.h"

/// Opens a file and returns a string of the raw data
char * readFileRaw (const char * fullPath) {
	std::ifstream fs(fullPath, std::ios::in | std::ios::binary);
	
	if (!fs.is_open()) {
		std::cout << "readFileRaw: " << fullPath << " -- " << "WARNING: Could not open file." << std::endl;
		return NULL;
		}
	else {
		std::cout << "Opened! " << fullPath << std::endl;
		}
				
	fs.seekg (0, std::ios::end);
	const size_t LEN = fs.tellg();
	fs.seekg (0, std::ios::beg);
	
	char * r = SSF_NEW(char[LEN]);
	fs.read(r, LEN);
	fs.close();
	return r;
	}


int main(int argc, char**argv) {
// Sample text from http://www.columbia.edu/~fdc/utf8/index.html
// Notes on example text:
// Certian scripts such as Devanagari or Thai require HarfBuzz to render correctly
// and are therefore not supported
// This does not support HarfBuzz or FriBidi
// 
const std::string loremIpsum = "\"I can eat glass\" sample text:\n\nEuro Symbol: €.\nGreek: Μπορώ να φάω σπασμένα γυαλιά χωρίς να πάθω τίποτα.\nÍslenska / Icelandic: Ég get etið gler án þess að meiða mig.\nPolish: Mogę jeść szkło, i mi nie szkodzi.\nRomanian: Pot să mănânc sticlă și ea nu mă rănește.\nUkrainian: Я можу їсти шкло, й воно мені не пошкодить.\nArmenian: Կրնամ ապակի ուտել և ինծի անհանգիստ չըներ։\nGeorgian: მინას ვჭამ და არა მტკივა.\nHebrew: אני יכול לאכול זכוכית וזה לא מזיק לי.\nArabic: أنا قادر على أكل الزجاج و هذا لا يؤلمني.\nChinese: 我能吞下玻璃而不伤身体。\nChinese (Traditional): 我能吞下玻璃而不傷身體。 \nJapanese: 私はガラスを食べられます。それは私を傷つけません。\nKorean: 률로 정한다. 군사법원의 조직·권한 및 재판관의 자격은 법률로 정한다\nCJK Variants: 判 与 海 直 約 返 次 今 ";

	sdl_stb_font_cache fc;
	fc.faceSize = 24;
	
	{
		char * notoSans = readFileRaw("fonts/NotoSans-Regular.ttf");
		char * notoSansArmenian = readFileRaw("fonts/NotoSansArmenian-Regular.ttf");
		char * notoSansGeorgian = readFileRaw("fonts/NotoSansGeorgian-Regular.ttf");
		char * notoSansHebrew   = readFileRaw("fonts/NotoSansHebrew-Regular.ttf");
		//char * notoSansHindi = readFileRaw("fonts/NotoSansDevanagari-Regular.ttf", notoSansHindi);
		char * notoSansArabic = readFileRaw("fonts/NotoSansArabic-Regular.ttf");
		char * notoSansCJK    = readFileRaw("fonts/NotoSansCJKjp-Regular.otf");
		
		fc.loadFontManaged(notoSans);
		fc.addFontManaged(notoSansArmenian);
		fc.addFontManaged(notoSansGeorgian);
		fc.addFontManaged(notoSansHebrew);
		//fc.addFontManaged(notoSansHindi);
		fc.addFontManaged(notoSansArabic);
		fc.addFontManaged(notoSansCJK);
	}
		
	// Setup the SDL window & renderer
	int windowWidth = 800;
	int windowHeight = 1000;

	SDL_Init(0);
	SDL_Window * mWindow = SDL_CreateWindow("Example Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	
	SDL_Renderer * mSdlRenderer = SDL_CreateRenderer(mWindow, SDL_RENDERER_SOFTWARE, 0);
	fc.bindRenderer(mSdlRenderer); // Must bind a renderer before generating any glyphs
	
	// Profiling variables
	uint64_t NOW = SDL_GetPerformanceCounter();
	uint64_t LAST = 0;
	
	// Rendering test
	int test = 3;
	
	if (test == 1) {
		// Directly rendering text
		for (int i = 0;; ++i) {
			SDL_Event ev;
			while (SDL_PollEvent(&ev)) {
				switch (ev.type) {
					case SDL_QUIT:
						return 1;
						break;
					}
				}
			
			SDL_SetRenderDrawColor(mSdlRenderer, 125, 125, 125, 255);
			SDL_RenderClear(mSdlRenderer);
			
			fc.drawText(5, 5, "Direct Rendering Test - " + loremIpsum); // Renders the loremIpsum string and stores in the texture
   
			SDL_RenderPresent(mSdlRenderer);
			
			if (i % 100 == 0) {
				LAST = NOW;
				NOW = SDL_GetPerformanceCounter();
				const double deltaTime = (double)((NOW - LAST)*1000 / (double)SDL_GetPerformanceFrequency() ) / 100.0;
				std::cout << "Method 1 - 100 Frames Average - Frametime: " << deltaTime << "ms \t FPS: " << (1000.0/deltaTime) << std::endl;
				}
			}
		}
	if (test == 2) {
		// Rendering text to a render target & reusing
		SDL_Texture * RT = NULL;
		int RTw, RTh;
		RT = fc.renderTextToTexture("Render Target Test - " + loremIpsum, &RTw, &RTh); // Renders the loremIpsum string and stores in the texture
			
		for (int i = 0;; ++i) {
			SDL_Event ev;
			while (SDL_PollEvent(&ev)) {
				switch (ev.type) {
					case SDL_QUIT:
						return 1;
						break;
					}
				}
			
			SDL_SetRenderDrawColor(mSdlRenderer, 125, 125, 125, 255);
			SDL_RenderClear(mSdlRenderer);
			
			SDL_Rect r;
			r.x = 5;
			r.y = 5;
			r.w = RTw;
			r.h = RTh;
			SDL_RenderCopy(mSdlRenderer , RT , NULL, &r); 
			
			SDL_RenderPresent(mSdlRenderer);
			
			if (i % 100 == 0) {
				LAST = NOW;
				NOW = SDL_GetPerformanceCounter();
				const double deltaTime = (double)((NOW - LAST)*1000 / (double)SDL_GetPerformanceFrequency() ) / 100.0;
				std::cout << "Method 2 - 100 Frames Average - Frametime: " << deltaTime << "ms \t FPS: " << (1000.0/deltaTime) << std::endl;
				}
			}
			
		// Cleanup
		SDL_DestroyTexture(RT);
		}
	if (test == 3) {
		// Rendering text to a render target wrapped in an object & reusing
		sdl_stb_prerendered_text prt;
		fc.renderTextToObject(&prt, "Render Object Test - " + loremIpsum); // Renders the loremIpsum string and stores in the prt object
		
		for (int i = 0;; ++i) {
			SDL_Event ev;
			while (SDL_PollEvent(&ev)) {
				switch (ev.type) {
					case SDL_QUIT:
						return 1;
						break;
					}
				}
			
			SDL_SetRenderDrawColor(mSdlRenderer, 125, 125, 125, 255);
			SDL_RenderClear(mSdlRenderer);
			
			//prt.draw(mSdlRenderer, 5, 5); // Render without colour modification
			prt.drawWithColorMod(mSdlRenderer, 5, 5, 255, 185, 80, 255); // Render in orange
			
			
			SDL_RenderPresent(mSdlRenderer);
			
			if (i % 100 == 0) {
				LAST = NOW;
				NOW = SDL_GetPerformanceCounter();
				const double deltaTime = (double)((NOW - LAST)*1000 / (double)SDL_GetPerformanceFrequency() ) / 100.0;
				std::cout << "Method 3 - 100 Frames Average - Frametime: " << deltaTime << "ms \t FPS: " << (1000.0/deltaTime) << std::endl;
				}
			}
			
		// Cleanup. The object doesn't have a destructor to prevent accidental destruction of internal texture
		prt.freeTexture();	
		}
		
	// Cleanup - just let fc fall out of scope
	
	return 1;
	}

#define LZZ_INLINE inline
#undef LZZ_INLINE
