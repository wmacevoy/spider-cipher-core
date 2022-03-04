#include "spider_cipher_core.h"

#define SPIDER_CIPHER_CUT_ZTH  0
#define SPIDER_CIPHER_TAG_ZTH  2
#define SPIDER_CIPHER_TAG_ADD 39

#ifdef __cplusplus
extern "C" {
#endif

  static SpiderCipherCard SpiderCipherNoiseCard(SpiderCipherDeck *deck);

  static SpiderCipherCard SpiderCipherCutCard(SpiderCipherDeck *deck,
				       SpiderCipherCard clear);
  
  static void SpiderCipherCutDeck(SpiderCipherDeck *inputDeck,
			   SpiderCipherCard cutCard,
			   SpiderCipherDeck *outputDeck);
  
  static void SpiderCipherBackFrontShuffleDeck(SpiderCipherDeck *inputDeck,
					SpiderCipherDeck *outputDeck);

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

  void SpiderCipherAdvanceDeck(SpiderCipherDeck *deck,
			       SpiderCipherCard clear,
			       SpiderCipherDeck *spare) {
    SpiderCipherBackFrontShuffleDeck(deck,spare);
    SpiderCipherCutDeck(spare,SpiderCipherCutCard(spare,clear),deck);
  }
  
  static SpiderCipherCard SpiderCipherCutCard(SpiderCipherDeck *deck,
				       SpiderCipherCard clear) {
    return (clear+deck->cards[SPIDER_CIPHER_CUT_ZTH])%SPIDER_CIPHER_CARDS;
  }

  static SpiderCipherCard SpiderCipherNoiseCard(SpiderCipherDeck *deck) {
    return deck->cards[(deck->ats[(deck->cards[SPIDER_CIPHER_TAG_ZTH]+SPIDER_CIPHER_TAG_ADD)%SPIDER_CIPHER_CARDS]+1)%SPIDER_CIPHER_CARDS];
  }

  static void SpiderCipherCutDeck(SpiderCipherDeck *input,
				  SpiderCipherCard cut,
				  SpiderCipherDeck *output) {
    if (cut >= SPIDER_CIPHER_CARDS) return;
    
    uint8_t cutAt = input->ats[cut];
    uint8_t uncutAt = (SPIDER_CIPHER_CARDS-cutAt) % SPIDER_CIPHER_CARDS;
    
    for (uint8_t i=0; i<SPIDER_CIPHER_CARDS; ++i) {
      output->cards[i]=
	input->cards[(i + cutAt) % SPIDER_CIPHER_CARDS];
      output->ats[i]=
	(input->ats[i]+uncutAt) % SPIDER_CIPHER_CARDS;
    }
    cutAt = 0;
    uncutAt = 0;
  }
  
  static void SpiderCipherBackFrontShuffleDeck(SpiderCipherDeck *input,
					SpiderCipherDeck *output) {
    {
      SpiderCipherCard *in = input->cards;
      SpiderCipherCard *out = output->cards+SPIDER_CIPHER_CARDS/2;
      for (uint8_t i=0; i<SPIDER_CIPHER_CARDS/2; ++i) {
	out[i]=in[2*i];
	out[-(i+1)]=in[2*i+1];
      }
    }

    {
      uint8_t *in = input->ats;
      uint8_t *out = output->ats;
      
      int8_t was,eo,at;
      for (int8_t i=0; i<SPIDER_CIPHER_CARDS; ++i) {    
	was = in[i];
	eo=was&1;
	at = SPIDER_CIPHER_CARDS/2+(1-2*eo)*(was/2+eo);
	out[i]=at;
      }
      was=eo=at=0;
      in=out=NULL;
    }
  }
#ifdef __cplusplus
}
#endif
