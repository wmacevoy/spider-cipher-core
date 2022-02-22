#include <stdint.h>
#include "spider_cipher_core.h"

#define SPIDER_CIPHER_CUT_ZTH  0
#define SPIDER_CIPHER_TAG_ZTH  2
#define SPIDER_CIPHER_TAG_ADD 39

#ifdef __cplusplus
extern "C" {
#endif

  void SpiderCipherDeckInit(SpiderCipherDeck *deck) {
    for (uint8_t i=0; i<SPIDER_CIPHER_CARDS; ++i) {
      deck->cards[i]=i;
      deck->ats[i]=i;
    }
  }
  
  void SpiderCipherDeckInitFunction(SpiderCipherDeck *deck,
				    SpiderCipherCard (*f)(uint8_t at, void *misc), void *misc) {
    for (uint8_t i=0; i<SPIDER_CIPHER_CARDS; ++i) {
      deck->ats[i]=SPIDER_CIPHER_CARDS;
    }
    
    for (uint8_t i=0; i<SPIDER_CIPHER_CARDS; ++i) {
      SpiderCipherCard card = (f != NULL) ? f(i,misc) : i;
      assert(card < SPIDER_CIPHER_CARDS);
      assert(deck->ats[card] == SPIDER_CIPHER_CARDS);
      deck->cards[i]=card;
      deck->ats[card]=i;
    }
  }
  
  static uint8_t SpiderCipherCutCard(SpiderCipherDeck *deck, uint8_t clearCard) {
    return (deck->cards[SPIDER_CIPHER_CUT_ZTH] + clearCard) % SPIDER_CIPHER_CARDS;
  }

  uint8_t SpiderCipherNoiseCard(SpiderCipherDeck *deck) {
    uint8_t tagCard = (deck->cards[SPIDER_CIPHER_TAG_ZTH] + SPIDER_CIPHER_TAG_ADD) % SPIDER_CIPHER_CARDS;
    uint8_t tagAt = deck->ats[tagCard];
    uint8_t noiseAt = (tagAt + 1) % SPIDER_CIPHER_CARDS;
    uint8_t noiseCard = deck->cards[noiseAt];
    return noiseCard;
  }

  uint8_t SpiderCipherScrambleClearCard(SpiderCipherDeck *deck,uint8_t clearCard) {
    uint8_t noiseCard = SpiderCipherNoiseCard(deck);
    uint8_t scrambledCard = (clearCard + noiseCard) % SPIDER_CIPHER_CARDS;
    return scrambledCard;
  }

  uint8_t SpiderCipherClearScrambledCard(SpiderCipherDeck *deck,uint8_t scrambledCard) {
    uint8_t noiseCard = SpiderCipherNoiseCard(deck);
    uint8_t denoiseCard = SPIDER_CIPHER_CARDS-noiseCard;
    uint8_t clearCard = (scrambledCard + denoiseCard) % SPIDER_CIPHER_CARDS;
    return clearCard;
  }
  
  inline void SpiderCipherDeckCutAt(SpiderCipherDeck *input,
				    uint8_t cutAt,
				    SpiderCipherDeck *output) {

    uint8_t uncutAt = SPIDER_CIPHER_CARDS-cutAt;
    
    for (uint8_t i=0; i<SPIDER_CIPHER_CARDS; ++i) {
      output->cards[i]=input->cards[(i + cutAt) % SPIDER_CIPHER_CARDS];
      output->ats[i]=(input->ats[i]+uncutAt) % SPIDER_CIPHER_CARDS;
    }
    
    uncutAt = 0;
  }
  
  inline void SpiderCipherBackFrontShuffle(SpiderCipherDeck *input,
					   SpiderCipherDeck *output) {
    
    for (uint8_t i=0; i<SPIDER_CIPHER_CARDS/2; ++i) {
      output->cards[SPIDER_CIPHER_CARDS/2+i]=input[2*i];
      output->cards[SPIDER_CIPHER_CARDS/2-1-i]=input[2*i+1];
    }

    int8_t was,eo,at;
    for (int8_t i=0; i<SPIDER_CIPHER_CARDS; ++i) {    
      was = input->ats[i];
      eo=was&1;
      at = SPIDER_CIPHER_CARDS/2+(1-2*eo)*(was/2+eo);
      output->ats[at]=was;
    }
    was=eo=at=0;
  }

  uint8_t SpiderCipherFindCard(SpiderCipherDeck *deck,uint8_t card) {
    return deck->ats[card];
  }
  
  void SpiderCipherShuffle(uint8_t deck[SPIDER_CIPHER_CARDS],
			   uint8_t cutLoc);
  

#ifdef __cplusplus
}
#endif
