#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SPIDER_CIPHER_CARDS   40

  //
  // Spider Cipher Core is just the cipher.
  //
  // It does not translate and it does not place a
  // translated message in a packet.
  //
  // Showing the security of the core cipher is relatively
  // expensive, so leaving these as isolated parts facilitates
  // the tests of the core cipher.
  // 
  // SpiderCipherCard key(unit8_t at, void *misc)  { return key cards 0..39 }
  // SpiderCipherCard packet[]  { current packet }
  // 
  // SpiderCipherDeck deck,spare;
  // SpiderCipherDeckInitBy(&deck,key,NULL);
  // SpiderCipherDeckInit(&spare);
  //
  // for (int i=0; i<packetSize; ++i) {
  //    SpiderCipherCard clear,scrambled;
  //    if (scrambling) {
  //       clear = packet[i];
  //       scrambled = SpiderCipherScramble(&deck,clear);
  //       packet[i] = scrambled;
  //    } else if (unscrambling) {
  //       scrambled = packet[i];
  //       clear = SpiderCipherUnscramble(&deck,scrambled);
  //       packet[i] = clear;
  //    }
  //    SpiderCipherAdvance(&deck,clear,&spare);
  // }
  //
  // SpiderCiperDeckInit(&deck);
  // SpiderCiperDeckInit(&spare);  
  //

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
  //  0 - f(at,misc) is out of 0..39 or repeats a value.
  //
  int SpiderCipherDeckInitBy(SpiderCipherDeck *deck,
			     SpiderCipherCard (*f)(uint8_t at, void *misc),
			     void *misc);

  // scrambled = (clear + noise) mod 40
  SpiderCipherCard SpiderCipherScramble(SpiderCipherDeck *deck,
					SpiderCipherCard  clear);

  // clear = (scrambled - noise) mod 40
  SpiderCipherCard SpiderCipherUnscramble(SpiderCipherDeck *deck,
					 SpiderCipherCard scrambled);

  // Adjust deck for next scramble/unscramble of packet.
  // Spare should be set back to 0,..,39 after final use.
  void SpiderCipherAdvance(SpiderCipherDeck *deck,
			   SpiderCipherCard clear,
			   SpiderCipherDeck *spare);

#ifdef __cplusplus
}
#endif
