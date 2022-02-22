#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

  // Deck key,work,spare;
  //
  // DeckInitFunction(&key,KeyFunction);
  // DeckInit(&work);
  // DeckInit(&spare);
  //
  // DeckSet(&key,{....})
  // SetDeckFromDeck(&work,&key);
  // SetDeckFrom
  // for clearCard in packet:
  //   Card scrambledCard = ScrambleCard(deck,clearCard);
  //   Card cutCard = CutCard(deck,clearCard);
  //   CutDeck(deck,cutCard,spare);
  //   BackFrontShuffleDeck(spare,deck);
  
  // for current in 0 .. packetLength-1:
  //   
  //   if (scrambling)   scramble[current]=(clear[current]+noiseCard) mod 40
  //   if (unscrambling) clear[current]=(scramble[current]-noiseCard) mod 40
  //
  //   cutCard = (deck[0]+clear[current]) mod 40
  //   cutAt = find(deck,cutCard)
  //   cutDeck(deck,cutAt,cutDeck)
  //   backFrontShuffle(cutDeck,deck)

  // Spider Cipher is based on a 40 card deck numbered 00 .. 39
#define SPIDER_CIPHER_CARDS   40

  //
  // Card is a unsigned byte (0..255),
  // the name helps you remember it is a Card (0..39)
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
  
  // Initialize deck to 00 .. 39
  void SpiderCipherDeckInit(SpiderCipherDeck *deck);

  // scramble = clear + noise(deck) mod 40
  SpiderCipherCard SpiderCipherScrambleCard(SpiderCipherDeck *deck,
					    SpiderCipherCard  clearCard);

  // clear = scramble - noise(deck) mod 40  
  SpiderCipherCard SpiderCipherClearCard(SpiderCipherDeck *deck,
					 SpiderCipherCard scrambledCard);
  
  void SpiderCipherCutDeck(SpiderCipherDeck *inputDeck,
			   SpiderCipherCard cutCard,
			   SpiderCipherDeck *outputDeck);

  void SpiderCipherBackFrontShuffleDeck(SpiderCipherDeck *inputDeck,
					SpiderCipherDeck *outputDeck);
  
  

  
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
    j=0;
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
      at = SPIDER_CIPHER_CARDS/2+(2*eo-1)*(was/2+eo);
      output->ats[at]=was;
    }
    was=eo=at=0;
  }

  void SpiderCipherShuffle(uint8_t deck[SPIDER_CIPHER_CARDS],
			   uint8_t cutLoc);
  

#ifdef __cplusplus
}
#endif
