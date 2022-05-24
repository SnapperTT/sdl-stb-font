// producerConsumerFrontend.hh
//

#ifndef LZZ_producerConsumerFrontend_hh
#define LZZ_producerConsumerFrontend_hh
/// Used for when you want to have, eg, an Update thread and a Render thread
/// 
#ifndef SSF_MUTEX
	#define SSF_MUTEX std::mutex
	#include <mutex>
#endif

typedef uint32_t pcfc_handle;
struct producer_consumer_font_cache;
#define LZZ_INLINE inline
struct pcfc_prerendered_text : public sttfont_prerendered_text
{
  pcfc_handle handle;
  producer_consumer_font_cache * owner;
};
class producer_consumer_font_cache : public sttfont_font_cache
{
protected:
  struct pcfc_consumer_prerendered_text : public sttfont_prerendered_text
  {
    pcfc_handle handle;
    sttfont_formatted_text text;
    pcfc_consumer_prerendered_text ();
  };
  struct pcfc_formatted_text
  {
    sttfont_formatted_text text;
    int x;
    int y;
  };
public:
  sttfont_font_cache * consumer_font_cache;
protected:
  pcfc_handle nextPrerenderTokenId;
  SSF_MUTEX mMutex;
  SSF_MAP <pcfc_handle, sttfont_prerendered_text*> prerenderMap;
  SSF_VECTOR <producer_consumer_font_cache::pcfc_consumer_prerendered_text> prerenderQueueProducer;
  SSF_VECTOR <producer_consumer_font_cache::pcfc_consumer_prerendered_text> prerenderQueueTx;
  SSF_VECTOR <producer_consumer_font_cache::pcfc_consumer_prerendered_text> prerenderQueueConsumer;
  SSF_VECTOR <pcfc_handle> destroyPrerenderQueueProducer;
  SSF_VECTOR <pcfc_handle> destroyPrerenderQueueTx;
  SSF_VECTOR <pcfc_handle> destroyPrerenderQueueConsumer;
  SSF_VECTOR <producer_consumer_font_cache::pcfc_formatted_text> textProducer;
  SSF_VECTOR <producer_consumer_font_cache::pcfc_formatted_text> textTx;
  SSF_VECTOR <producer_consumer_font_cache::pcfc_formatted_text> textConsumer;
  pcfc_handle getNextPrerenderToken ();
public:
  producer_consumer_font_cache ();
  void freeStoredPrerenderedText (bool const freeTextures);
  void loadFontManagedBoth (sttfont_memory & memory, int index = 0);
  void addFontManagedBoth (sttfont_memory & memory, int index = 0);
  pcfc_handle pushText (int const x, int const y, char const * c, uint32_t const maxLen = -1, int * xOut = NULL, int * widthOut = NULL, int * heightOut = NULL);
  pcfc_handle pushText (int const x, int const y, SSF_STRING const & str, int * xOut = NULL, int * widthOut = NULL, int * heightOut = NULL);
  pcfc_handle pushText (int const x, int const y, sttfont_formatted_text const & str, int * xOut = NULL, int * widthOut = NULL, int * heightOut = NULL);
  void drawCodepoint (sttfont_glyph const * const GS, int const x, int const y, uint32_t const codepoint, sttfont_format const * const format, uint8_t const formatCode, int const kerningAdv, int & overdraw);
  void renderTextToObject (sttfont_prerendered_text * textOut, char const * c, uint32_t const maxLen = -1);
  void renderTextToObject (sttfont_prerendered_text * textOut, SSF_STRING const & str);
  void renderTextToObject (sttfont_prerendered_text * textOut, sttfont_formatted_text const & str);
  void destroyPrerender (pcfc_handle const handle);
protected:
  template <typename T>
  void submitWorker (SSF_VECTOR <T> & vSrc, SSF_VECTOR <T> & vDst);
public:
  void submitToConsumer ();
  void receiveFromProducer ();
  template <typename T>
  void dispatchPrerenders ();
  template <typename T>
  void dispatchDestroy ();
  void dispatchSinglePrerendered (pcfc_handle const prtId, int x, int y);
  void dispatchSingleText (pcfc_handle const texId);
};
template <typename T>
void producer_consumer_font_cache::submitWorker (SSF_VECTOR <T> & vSrc, SSF_VECTOR <T> & vDst)
                                                                    {
		if (!vDst.size())
			vDst.swap(vSrc);
		else {
			vDst.insert(vDst.end(), std::make_move_iterator(vSrc.begin()), std::make_move_iterator(vSrc.end()));
			vSrc.clear();
			}
		}
template <typename T>
void producer_consumer_font_cache::dispatchPrerenders ()
                                  {
		// T is the sttfont_prerendered_text subclass that is used by consumer_font_cache
		for (pcfc_consumer_prerendered_text & p2 : prerenderQueueConsumer) {
			auto itt = prerenderMap.find(p2.handle);
			if (itt != prerenderMap.end())
				abort(); // Idx reusued, this will leak memory
			T* t = SSF_NEW(T);
			consumer_font_cache->renderTextToObject(t, p2.text);
			prerenderMap[p2.handle] = t;
			}
		prerenderQueueConsumer.clear();
		}
template <typename T>
void producer_consumer_font_cache::dispatchDestroy ()
                               {
		// T is the sttfont_prerendered_text subclass that is used by consumer_font_cache
		for (const pcfc_handle idx : destroyPrerenderQueueConsumer) {
			auto itt = prerenderMap.find(idx);
			if (itt != prerenderMap.end()) {
				itt->second->freeTexture();
				SSF_DEL(itt->second);
				prerenderMap.erase(itt);
				}
			}
		}
#undef LZZ_INLINE
#endif
