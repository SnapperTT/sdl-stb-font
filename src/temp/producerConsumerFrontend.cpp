// producerConsumerFrontend.cpp
//

#include "producerConsumerFrontend.hh"
#define LZZ_INLINE inline
producer_consumer_font_cache::pcfc_consumer_prerendered_text::pcfc_consumer_prerendered_text ()
                                                 {}
pcfc_handle producer_consumer_font_cache::getNextPrerenderToken ()
                                            {
		return nextPrerenderTokenId++;
		}
producer_consumer_font_cache::producer_consumer_font_cache ()
  : consumer_font_cache (NULL), nextPrerenderTokenId (0)
                                                                                            {}
void producer_consumer_font_cache::freeStoredPrerenderedText (bool const freeTextures)
                                                                {
		for (auto ppair : prerenderMap) {
			sttfont_prerendered_text* prt = ppair.second;
			if (!prt) continue;
			if (freeTextures) {
				prt->freeTexture();
				}
			SSF_DEL(prt);
			prerenderMap.clear();
			}
		}
void producer_consumer_font_cache::loadFontManagedBoth (sttfont_memory & memory, int index)
                                                                          {
		sttfont_memory memory2;
		memory.cloneTo(memory2);
		
		consumer_font_cache->syncFrom(*this);
		consumer_font_cache->loadFontManaged(memory2, index);
		this->syncFrom(*consumer_font_cache);
		this->loadFontManaged(memory, index);
		}
void producer_consumer_font_cache::addFontManagedBoth (sttfont_memory & memory, int index)
                                                                         {
		sttfont_memory memory2;
		memory.cloneTo(memory2);
		
		consumer_font_cache->syncFrom(*this);
		consumer_font_cache->addFontManaged(memory2, index);
		this->syncFrom(*consumer_font_cache);
		this->addFontManaged(memory, index);
		}
pcfc_handle producer_consumer_font_cache::pushText (int const x, int const y, char const * c, uint32_t const maxLen, int * xOut, int * widthOut, int * heightOut)
                                                                                                                                                                 {
		sttfont_formatted_text tmp(c, maxLen);
		return pushText(x, y, tmp, xOut, widthOut, heightOut);
		}
pcfc_handle producer_consumer_font_cache::pushText (int const x, int const y, SSF_STRING const & str, int * xOut, int * widthOut, int * heightOut)
                                                                                                                                              {
		sttfont_formatted_text tmp(str);
		return pushText(x, y, tmp, xOut, widthOut, heightOut);
		}
pcfc_handle producer_consumer_font_cache::pushText (int const x, int const y, sttfont_formatted_text const & str, int * xOut, int * widthOut, int * heightOut)
                                                                                                                                                          {
		if (xOut || widthOut || heightOut) {
			int xx = processFormatted(str, x, y, false, widthOut, heightOut);
			if (xOut) *xOut = xx;
			}
		pcfc_formatted_text p;
		p.text = str;
		p.x = x;
		p.y = y;
		textProducer.push_back(std::move(p));
		return textProducer.size() - 1; 
		}
void producer_consumer_font_cache::drawCodepoint (sttfont_glyph const * const GS, int const x, int const y, uint32_t const codepoint, sttfont_format const * const format, uint8_t const formatCode, int const kerningAdv, int & overdraw)
                                                                                                                                                                                                                     {
		// This function should never be called for this frontend. Use pushText instead of drawText
		abort();
		}
void producer_consumer_font_cache::renderTextToObject (sttfont_prerendered_text * textOut, char const * c, uint32_t const maxLen)
                                                                                                                 {
		sttfont_formatted_text tmp(c, maxLen);
		renderTextToObject (textOut, tmp);
		}
void producer_consumer_font_cache::renderTextToObject (sttfont_prerendered_text * textOut, SSF_STRING const & str)
                                                                                             {
		sttfont_formatted_text tmp(str);
		renderTextToObject (textOut, tmp);
		}
void producer_consumer_font_cache::renderTextToObject (sttfont_prerendered_text * textOut, sttfont_formatted_text const & str)
                                                                                                         {
		pcfc_prerendered_text* p = (pcfc_prerendered_text*) textOut;
		getTextSize(textOut->width, textOut->height, str);
		p->handle = getNextPrerenderToken();
		p->owner = this;
		
		pcfc_consumer_prerendered_text p2;
		p2.width = p->width;
		p2.height = p->height;
		p2.handle = p->handle;
		p2.text = str;
		
		prerenderQueueProducer.push_back(p2);
		}
void producer_consumer_font_cache::destroyPrerender (pcfc_handle const handle)
                                                        {
		destroyPrerenderQueueProducer.push_back(handle);
		}
void producer_consumer_font_cache::submitToConsumer ()
                                {
		if (!(prerenderQueueProducer.size() || destroyPrerenderQueueProducer.size())) return; // nothing to submit
		mMutex.lock();
		submitWorker(prerenderQueueProducer, prerenderQueueTx);
		submitWorker(destroyPrerenderQueueProducer, destroyPrerenderQueueTx);
		submitWorker(textProducer, textTx);
		mMutex.unlock();
		}
void producer_consumer_font_cache::receiveFromProducer ()
                                   {
		textConsumer.clear();
		mMutex.lock();
		prerenderQueueConsumer.swap(prerenderQueueTx);
		destroyPrerenderQueueConsumer.swap(destroyPrerenderQueueTx);
		textConsumer.swap(textTx);
		mMutex.unlock();
		}
void producer_consumer_font_cache::dispatchSinglePrerendered (pcfc_handle const prtId, int x, int y)
                                                                              {
		auto itt = prerenderMap.find(prtId);
		if (itt != prerenderMap.end()) {
			itt->second->draw(x, y);
			}
		}
void producer_consumer_font_cache::dispatchSingleText (pcfc_handle const texId)
                                                         {
		if (texId < textConsumer.size() && texId >= 0) {
			pcfc_formatted_text & pcf = textConsumer[texId];
			consumer_font_cache->drawText(pcf.x, pcf.y, pcf.text);
			}
		}
#undef LZZ_INLINE
