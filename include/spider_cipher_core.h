#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SPIDER_CIPHER_CARDS   40

  //
  // A SpiderCipherCard is in the range 0...39
  //
  typedef uint8_t SpiderCipherCard;

  //
  // The Deck structure tracks cards and thier locations:
  //
  //   cards[ats[card]]=card
  //   ats[cards[at]]=at
  //
  // This facilitates constant time cipher steps.
  //
  
  typedef struct {
    SpiderCipherCard cards[SPIDER_CIPHER_CARDS];
    uint8_t ats[SPIDER_CIPHER_CARDS];
  } SpiderCipherDeck;

  // Initialize deck to 0,...,39
  void SpiderCipherDeckInit(SpiderCipherDeck *deck);

  // Initialize deck to f(0,misc),...,f(39,misc)
  //
  // RETURN VALUE
  //  1 - Deck was properly initialized.
  //  0 - f(at,misc) is out of 00..39 or repeats a value.
  //
  int SpiderCipherDeckInitBy(SpiderCipherDeck *deck,
			 SpiderCipherCard (*f)(uint8_t at, void *misc),
			 void *misc);

  uint8_t SpiderCipherFindCard(SpiderCipherDeck *deck, SpiderCipherCard card);
  
  // tag card = ((3rd card of deck) - 1) mod 40
  // noise card = card after (wrapping to front) tag card in deck
  SpiderCipherCard SpiderCipherNoiseCard(SpiderCipherDeck *deck);

  // cut card = (1st card of deck) + (clear/unscrambled card of packet) mod 40
  SpiderCipherCard SpiderCipherCutCard(SpiderCipherDeck *deck,
				       SpiderCipherCard clear);
  
  // scrambled = (clear + noise) mod 40
  SpiderCipherCard SpiderCipherScramble(SpiderCipherDeck *deck,
					SpiderCipherCard  clear);

  // clear = (scrambled - noise) mod 40
  SpiderCipherCard SpiderCipherUnscramble(SpiderCipherDeck *deck,
					 SpiderCipherCard scrambled);

  void SpiderCipherCutDeck(SpiderCipherDeck *inputDeck,
			   SpiderCipherCard cutCard,
			   SpiderCipherDeck *outputDeck);

  void SpiderCipherBackFrontShuffleDeck(SpiderCipherDeck *inputDeck,
					SpiderCipherDeck *outputDeck);
  
#ifdef __cplusplus
}
#endif
