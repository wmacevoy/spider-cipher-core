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

  int SpiderCipherDeckInitBy(SpiderCipherDeck *deck,
			      SpiderCipherCard (*f)(uint8_t at, void *misc),
			      void *misc) {
    for (uint8_t card=0; card<SPIDER_CIPHER_CARDS; ++card) {
      deck->ats[card]=SPIDER_CIPHER_CARDS;      
    }
    
    for (uint8_t i=0; i<SPIDER_CIPHER_CARDS; ++i) {
      SpiderCipherCard card = f ? f(i,misc) : i;
      if (card >= SPIDER_CIPHER_CARDS) return 0;
      if (deck->ats[card] != SPIDER_CIPHER_CARDS) return 0;
      deck->cards[i]=card;
      deck->ats[card]=i;
    }

    return 1;
  }

  uint8_t SpiderCipherFindCard(SpiderCipherDeck *deck, SpiderCipherCard card) {
    return deck->ats[card];
  }
  
  SpiderCipherCard SpiderCipherCutCard(SpiderCipherDeck *deck,
				       SpiderCipherCard clear) {
    return (clear+deck->cards[SPIDER_CIPHER_CUT_ZTH])%SPIDER_CIPHER_CARDS;
  }

  SpiderCipherCard SpiderCipherNoiseCard(SpiderCipherDeck *deck) {
    return deck->cards[(deck->ats[(deck->cards[SPIDER_CIPHER_TAG_ZTH]+SPIDER_CIPHER_TAG_ADD)%SPIDER_CIPHER_CARDS]+1)%SPIDER_CIPHER_CARDS];
  }

  SpiderCipherCard SpiderCipherScramble(SpiderCipherDeck *deck,
					SpiderCipherCard clear) {
    return (clear + SpiderCipherNoiseCard(deck))
      % SPIDER_CIPHER_CARDS;
  }

  uint8_t SpiderCipherUnscramble(SpiderCipherDeck *deck,
				 SpiderCipherCard scrambled) {
    return (scrambled+(SPIDER_CIPHER_CARDS-SpiderCipherNoiseCard(deck)))
      % SPIDER_CIPHER_CARDS;
  }

  void SpiderCipherCutDeck(SpiderCipherDeck *inputDeck,
			   SpiderCipherCard cutCard,
			   SpiderCipherDeck *outputDeck) {
    if (cutCard >= SPIDER_CIPHER_CARDS) return;
    
    uint8_t cutAt = inputDeck->ats[cutCard];
    uint8_t uncutAt = SPIDER_CIPHER_CARDS-cutAt;
    
    for (uint8_t i=0; i<SPIDER_CIPHER_CARDS; ++i) {
      outputDeck->cards[i]=
	inputDeck->cards[(i + cutAt) % SPIDER_CIPHER_CARDS];
      outputDeck->ats[i]=
	(inputDeck->ats[i]+uncutAt) % SPIDER_CIPHER_CARDS;
    }
    cutAt = 0;
    uncutAt = 0;
  }
  
  void SpiderCipherBackFrontShuffleDeck(SpiderCipherDeck *inputDeck,
					SpiderCipherDeck *outputDeck) {
    for (uint8_t i=0; i<SPIDER_CIPHER_CARDS/2; ++i) {
      outputDeck->cards[SPIDER_CIPHER_CARDS/2+i]=
	inputDeck->cards[2*i];
      outputDeck->cards[SPIDER_CIPHER_CARDS/2-1-i]=
	inputDeck->cards[2*i+1];
    }

    int8_t was,eo,at;
    for (int8_t i=0; i<SPIDER_CIPHER_CARDS; ++i) {    
      was = inputDeck->ats[i];
      eo=was&1;
      at = SPIDER_CIPHER_CARDS/2+(1-2*eo)*(was/2+eo);
      outputDeck->ats[i]=at;
    }
    was=eo=at=0;
  }

#ifdef __cplusplus
}
#endif
