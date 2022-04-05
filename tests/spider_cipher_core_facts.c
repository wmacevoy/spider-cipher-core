#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <assert.h>
#include <arpa/inet.h>

#include "facts.h"

//
// Unusual, but this tests the "private" static components
// of the cipher.  The exposed API limits potential incorrect
// use of the cipher.
//

#include "../src/spider_cipher_core.c"

#define CARDS SPIDER_CIPHER_CARDS
#define CUT_ZTH 0
#define TAG_ZTH 2
#define TAG_ADD 39

typedef SpiderCipherDeck Deck;
typedef SpiderCipherCard Card;

void deckOk(Deck *deck) {
  for (uint8_t i=0; i<CARDS; ++i) {
    assert(deck->ats[i] < CARDS);
    assert(deck->cards[i] < CARDS);
  }
  for (uint8_t i=0; i<CARDS; ++i) {
    assert(deck->cards[deck->ats[i]]==i);
    assert(deck->ats[deck->cards[i]]==i);    
  }
}

int cardsCmp(int n,Card *a, Card *b) {
  for (int i=0; i<n; ++i) {
    if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
  }
  return 0;
}

int deckCmp(Deck *a, Deck *b) {
  deckOk(a);
  deckOk(b);
  return cardsCmp(CARDS,a->cards,b->cards);
}

void setAts(Deck *deck) {
  for (uint8_t i=0; i<CARDS; ++i) {
    deck->ats[i]=CARDS;
  }
  for (uint8_t i=0; i<CARDS; ++i) {
    Card card = deck->cards[i];
    assert(card < CARDS);
    assert(deck->ats[card] == CARDS);
    deck->ats[card]=i;
  }
}

typedef Card Permutation [CARDS];

const Permutation ID =
  {
   0,1,2,3,4,5,6,7,8,9,
   10,11,12,13,14,15,16,17,18,19,
   20,21,22,23,24,25,26,27,28,29,
   30,31,32,33,34,35,36,37,38,39
  };

const Permutation CUT_00 =
  {
   0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
   10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
   20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
   30, 31, 32, 33, 34, 35, 36, 37, 38, 39
  };

const Permutation CUT_01 =
  {
   1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
   11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
   21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
   31, 32, 33, 34, 35, 36, 37, 38, 39, 0
  };

const Permutation CUT_02 =
  {
   2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
   12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
   22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
   32, 33, 34, 35, 36, 37, 38, 39, 0, 1
  };

const Permutation CUT_03 =
  {
   3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
   13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
   23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
   33, 34, 35, 36, 37, 38, 39, 0, 1, 2
  };

const Permutation CUT_04 =
  {
   4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
   14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
   24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
   34, 35, 36, 37, 38, 39, 0, 1, 2, 3
  };


const Permutation CUT_05 =
  {
   5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
   15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
   25, 26, 27, 28, 29, 30, 31, 32, 33, 34,
   35, 36, 37, 38, 39, 0, 1, 2, 3, 4
  };

const Permutation CUT_06 =
  {
   6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
   16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
   26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
   36, 37, 38, 39, 0, 1, 2, 3, 4, 5
  };

const Permutation CUT_07 =
  {
   7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
   17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
   27, 28, 29, 30, 31, 32, 33, 34, 35, 36,
   37, 38, 39, 0, 1, 2, 3, 4, 5, 6
  };
const Permutation CUT_08 =
  {
   8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
   18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
   28, 29, 30, 31, 32, 33, 34, 35, 36, 37,
   38, 39, 0, 1, 2, 3, 4, 5, 6, 7
  };

const Permutation CUT_09 =
  {
   9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
   19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
   29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
   39, 0, 1, 2, 3, 4, 5, 6, 7, 8
  };

const Permutation CUT_10 =
  {
   10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
   20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
   30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
   0, 1, 2, 3, 4, 5, 6, 7, 8, 9
  };

const Permutation CUT_11 =
  {
   11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
   21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
   31, 32, 33, 34, 35, 36, 37, 38, 39, 0,
   1, 2, 3, 4, 5, 6, 7, 8, 9, 10
  };

const Permutation CUT_12 =
  {
   12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
   22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
   32, 33, 34, 35, 36, 37, 38, 39, 0, 1,
   2, 3, 4, 5, 6, 7, 8, 9, 10, 11
  };

const Permutation CUT_13 =
  {
   13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
   23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
   33, 34, 35, 36, 37, 38, 39, 0, 1, 2,
   3, 4, 5, 6, 7, 8, 9, 10, 11, 12
  };

const Permutation CUT_14 =
  {
   14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
   24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
   34, 35, 36, 37, 38, 39, 0, 1, 2, 3,
   4, 5, 6, 7, 8, 9, 10, 11, 12, 13
  };

const Permutation CUT_15 =
  {
   15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
   25, 26, 27, 28, 29, 30, 31, 32, 33, 34,
   35, 36, 37, 38, 39, 0, 1, 2, 3, 4,
   5, 6, 7, 8, 9, 10, 11, 12, 13, 14
  };

const Permutation CUT_16 =
  {
   16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
   26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
   36, 37, 38, 39, 0, 1, 2, 3, 4, 5,
   6, 7, 8, 9, 10, 11, 12, 13, 14, 15
  };

const Permutation CUT_17 =
  {
   17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
   27, 28, 29, 30, 31, 32, 33, 34, 35, 36,
   37, 38, 39, 0, 1, 2, 3, 4, 5, 6,
   7, 8, 9, 10, 11, 12, 13, 14, 15, 16
  };

const Permutation CUT_18 =
  {
   18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
   28, 29, 30, 31, 32, 33, 34, 35, 36, 37,
   38, 39, 0, 1, 2, 3, 4, 5, 6, 7,
   8, 9, 10, 11, 12, 13, 14, 15, 16, 17
  };

const Permutation CUT_19 = {
 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
 39, 0, 1, 2, 3, 4, 5, 6, 7, 8,
 9, 10, 11, 12, 13, 14, 15, 16, 17, 18
};
const Permutation CUT_20 = {
 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
 10, 11, 12, 13, 14, 15, 16, 17, 18, 19
};
const Permutation CUT_21 = {
 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
 31, 32, 33, 34, 35, 36, 37, 38, 39, 0,
 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
 11, 12, 13, 14, 15, 16, 17, 18, 19, 20
};
const Permutation CUT_22 = {
 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
 32, 33, 34, 35, 36, 37, 38, 39, 0, 1,
 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
 12, 13, 14, 15, 16, 17, 18, 19, 20, 21
};
const Permutation CUT_23 = {
 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
 33, 34, 35, 36, 37, 38, 39, 0, 1, 2,
 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
 13, 14, 15, 16, 17, 18, 19, 20, 21, 22
};
const Permutation CUT_24 = {
 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
 34, 35, 36, 37, 38, 39, 0, 1, 2, 3,
 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
 14, 15, 16, 17, 18, 19, 20, 21, 22, 23
};
const Permutation CUT_25 = {
 25, 26, 27, 28, 29, 30, 31, 32, 33, 34,
 35, 36, 37, 38, 39, 0, 1, 2, 3, 4,
 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
 15, 16, 17, 18, 19, 20, 21, 22, 23, 24
};
const Permutation CUT_26 = {
 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
 36, 37, 38, 39, 0, 1, 2, 3, 4, 5,
 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
 16, 17, 18, 19, 20, 21, 22, 23, 24, 25
};
const Permutation CUT_27 = {
 27, 28, 29, 30, 31, 32, 33, 34, 35, 36,
 37, 38, 39, 0, 1, 2, 3, 4, 5, 6,
 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, 21, 22, 23, 24, 25, 26
};
const Permutation CUT_28 = {
 28, 29, 30, 31, 32, 33, 34, 35, 36, 37,
 38, 39, 0, 1, 2, 3, 4, 5, 6, 7,
 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
 18, 19, 20, 21, 22, 23, 24, 25, 26, 27
};
const Permutation CUT_29 = {
 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
 39, 0, 1, 2, 3, 4, 5, 6, 7, 8,
 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
 19, 20, 21, 22, 23, 24, 25, 26, 27, 28
};
const Permutation CUT_30 = {
 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
 20, 21, 22, 23, 24, 25, 26, 27, 28, 29
};
const Permutation CUT_31 = {
 31, 32, 33, 34, 35, 36, 37, 38, 39, 0,
 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
 21, 22, 23, 24, 25, 26, 27, 28, 29, 30
};
const Permutation CUT_32 = {
 32, 33, 34, 35, 36, 37, 38, 39, 0, 1,
 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
};
const Permutation CUT_33 = {
 33, 34, 35, 36, 37, 38, 39, 0, 1, 2,
 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
 23, 24, 25, 26, 27, 28, 29, 30, 31, 32
};
const Permutation CUT_34 = {
 34, 35, 36, 37, 38, 39, 0, 1, 2, 3,
 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
 24, 25, 26, 27, 28, 29, 30, 31, 32, 33
};
const Permutation CUT_35 = {
 35, 36, 37, 38, 39, 0, 1, 2, 3, 4,
 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
 25, 26, 27, 28, 29, 30, 31, 32, 33, 34
};
const Permutation CUT_36 = {
 36, 37, 38, 39, 0, 1, 2, 3, 4, 5,
 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
 26, 27, 28, 29, 30, 31, 32, 33, 34, 35
};
const Permutation CUT_37 = {
 37, 38, 39, 0, 1, 2, 3, 4, 5, 6,
 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
 27, 28, 29, 30, 31, 32, 33, 34, 35, 36
};
const Permutation CUT_38 = {
 38, 39, 0, 1, 2, 3, 4, 5, 6, 7,
 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
 28, 29, 30, 31, 32, 33, 34, 35, 36, 37
};

const Permutation CUT_39 =
  {
   39, 0, 1, 2, 3, 4, 5, 6, 7, 8,
   9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
   19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
   29, 30, 31, 32, 33, 34, 35, 36, 37, 38
};

const Permutation* CUTS[] =
  {
   &CUT_00, &CUT_01, &CUT_02, &CUT_03, &CUT_04, &CUT_05, &CUT_06, &CUT_07, &CUT_08, &CUT_09,
   &CUT_10, &CUT_11, &CUT_12, &CUT_13, &CUT_14, &CUT_15, &CUT_16, &CUT_17, &CUT_18, &CUT_19,
   &CUT_20, &CUT_21, &CUT_22, &CUT_23, &CUT_24, &CUT_25, &CUT_26, &CUT_27, &CUT_28, &CUT_29,
   &CUT_30, &CUT_31, &CUT_32, &CUT_33, &CUT_34, &CUT_35, &CUT_36, &CUT_37, &CUT_38, &CUT_39
  };

const Permutation BACK_FRONT = {39,37,35,33,31,29,27,25,23,21,
				19,17,15,13,11, 9, 7, 5, 3, 1,
				0, 2, 4, 6, 8,10,12,14,16,18,
				20,22,24,26,28,30,32,34,36,38};

//
// a = 1..40 x b = 0..40  test permutations
//
void samplePermutation(Permutation p, int a,int b) {
  assert(a > 0 && a < 41);
  assert(b >= 0 && b < 41);
  int skip=0;
  
  // <= is ok here: 1 case is always skipped
  for (int at=0; at<=CARDS; ++at) {
    //
    // Because 41 is prime x is guaranteed to be a permutation
    // of 0..40 as at=0..40.
    // This skips the x=40 case to make a permutation of 0..39
    //
    int x=(a*at+b) % 41;
    if (x == 40) {
      skip = 1;
    } else {
      p[at-skip]=x;
    }
    x = (x+a) % 41;
  }
}

void sampleBadPermutation(Permutation p, int a,int b) {
  samplePermutation(p,a,b);
  if (p[0] % 2 == 0) {
    p[p[1]]=p[p[2]];
  } else {
    p[p[1]]=p[2]+CARDS;
  }
}

void deckSet(Deck *deck, const Permutation permutation) {
  for (int i=0; i<CARDS; ++i) {
    deck->cards[i]=permutation[i];
    deck->ats[deck->cards[i]]=i;
  }
  deckOk(deck);  
}

void permute(const Permutation permutation, Deck *in, Deck *out) {
  deckOk(in);
  for (int i=0; i<CARDS; ++i) {
    out->cards[i]=in->cards[permutation[i]];
    out->ats[out->cards[i]]=i;
  }
  deckOk(out);  
}

void deckMix(Deck *deck, const Permutation permutation) {
  Deck spare;
  permute(permutation,deck,&spare);
  permute(ID,&spare,deck);
}

void testDeckInit(Deck *deck) {
  SpiderCipherDeckInit(deck);
  for (int i=0; i<CARDS; ++i) {
    assert(deck->cards[i]==i);
    assert(deck->ats[i]==i);    
  }
}

FACTS(DeckInit) {
  Deck deck;
  testDeckInit(&deck);
}

uint8_t PermutationFunction(uint8_t at, void *misc) {
  return (*(const Permutation*) misc)[at];
}

int testInitBy(Deck *deck, Card (*f)(uint8_t at, void *misc),void *misc) {
  Permutation permutation;
  int expectStatus = 1;

  uint8_t ats[CARDS];
  for (uint8_t card=0; card<CARDS; ++card) {
    ats[card]=CARDS;
  }
  
  for (uint8_t at=0; at<CARDS; ++at) {
    Card card = f ? f(at,misc) : at;
    if (card >= CARDS) {
      expectStatus = 0;
    }
    permutation[at]=card;
    if (ats[card] == CARDS) {
      ats[card]=at;
    } else {
      expectStatus = 0;
    }
  }
  
  int status = SpiderCipherDeckInitBy(deck,PermutationFunction,&permutation);
  assert(status == expectStatus);
  if (status) {
    Deck expect;
    deckSet(&expect,permutation);
    assert(deckCmp(&expect,deck)==0);
  }
  return status;
}


FACTS(InitBy) {
  for (int a=1; a <= CARDS; ++a) {
    for (int b=0; b <= CARDS; ++b) {
      Deck deck;
      Permutation permutation;
      samplePermutation(permutation,a,b);
      FACT(testInitBy(&deck,PermutationFunction,&permutation),==,1);
      sampleBadPermutation(permutation,a,b);
      FACT(testInitBy(&deck,PermutationFunction,&permutation),==,0);
    }
  }
}

void sampleDeck(Deck *deck, int a, int b) {
  Permutation permutation;
  samplePermutation(permutation,a,b);
  deckSet(deck,permutation);
}


Card testCutCard(Deck *deck, Card clear) {
  Card card = (deck->cards[CUT_ZTH] + clear) % CARDS;
  assert(card == SpiderCipherCutCard(deck,clear));
  return card;
}

FACTS(CutCard) {
  for (int a=1; a <= CARDS; ++a) {
    for (int b=0; b <= CARDS; ++b) {    
      Deck deck;
      sampleDeck(&deck,a,b);
      for (Card clear = 0; clear < CARDS; ++clear) {
	testCutCard(&deck,clear);
      }
    }
  }
}

int testFindCard(Deck *deck, Card card) {
  assert(card < CARDS);
  uint8_t at = deck->ats[card];
  assert(at < CARDS);
  assert(deck->cards[at] == card);
  return at;
}

FACTS(FindCard) {
  for (int a=1; a <= CARDS; ++a) {
    for (int b=0; b <= CARDS; ++b) {    
      Deck deck;
      sampleDeck(&deck,a,b);
      for (Card card = 0; card < CARDS; ++card) {
	uint8_t at = testFindCard(&deck,card);
	FACT(deck.cards[at],==,card);
      }
    }
  }
}

Card testNoiseCard(Deck *deck) {
  Card tagCard = (deck->cards[TAG_ZTH]+TAG_ADD) % CARDS;
  uint8_t tagAt = testFindCard(deck, tagCard);
  uint8_t noiseAt = (tagAt + 1) % CARDS;
  Card noiseCard = deck->cards[noiseAt];
  assert(noiseCard == SpiderCipherNoiseCard(deck));
  return noiseCard;
}

FACTS(NoiseCard) {
  for (int a=1; a <= CARDS; ++a) {
    for (int b=0; b <= CARDS; ++b) {    
      Deck deck;
      sampleDeck(&deck,a,b);
      testNoiseCard(&deck);
    }
  }
}

void testCutDeck(Deck *in, Card cutCard, Deck *out) {
  int cutAt = testFindCard(in,cutCard);
  Deck expect;
  permute(*CUTS[cutAt],in,&expect);
  SpiderCipherCutDeck(in,cutCard,out);
  assert(deckCmp(&expect,out)==0);
  assert(out->cards[0] == cutCard);
}

FACTS(CutDeck) {
  for (int a=1; a <= CARDS; ++a) {
    for (int b=0; b <= CARDS; ++b) {
      Deck deck,spare;
      sampleDeck(&deck,a,b);
      for (int cutCard=0; cutCard < CARDS; ++cutCard) {      
	testCutDeck(&deck,cutCard,&spare);
      }
    }
  }
}

void CutDeckAt(Deck *in, int8_t cutAt, Deck *out) {
  cutAt = ((cutAt % CARDS)+CARDS)%CARDS;
  Card cutCard=in->cards[cutAt];
  SpiderCipherCutDeck(in,cutCard,out);
}

FACTS(CutDeckAt) {
  for (int a=1; a <= CARDS; ++a) {
    for (int b=0; b <= CARDS; ++b) {
      for (int cutAt=-CARDS; cutAt <= 2*CARDS; ++cutAt) {      
	Deck deck,spare,expect;
	sampleDeck(&deck,a,b);
	sampleDeck(&expect,a,b);
	CutDeckAt(&deck,cutAt,&spare);
  int at = cutAt;
  while (at < 0) at += CARDS;
  while (at >= CARDS) at -= CARDS;
  deckMix(&expect,*CUTS[at]);
	FACT(deckCmp(&spare,&expect),==,0);
      }
    }
  }
}

void InverseCutDeckAt(Deck *in, int8_t cutAt, Deck *out) {
  CutDeckAt(in,-cutAt,out);
}

FACTS(InverseCutDeckAt) {
  for (int a=1; a <= CARDS; ++a) {
    for (int b=0; b <= CARDS; ++b) {
      for (int cutAt=-CARDS; cutAt <= 2*CARDS; ++cutAt) {      
	      Deck deck,spare,expect;
      	sampleDeck(&deck,a,b);
      	sampleDeck(&expect,a,b);
	      CutDeckAt(&deck,cutAt,&spare);
        InverseCutDeckAt(&spare,cutAt,&deck);
       	FACT(deckCmp(&deck,&expect),==,0);
      }
    }
  }
}

void testBackFrontShuffleDeck(Deck *in, Deck *out)
{
  Deck expect;
  permute(BACK_FRONT,in,&expect);
  SpiderCipherBackFrontShuffleDeck(in,out);
  assert(deckCmp(&expect,out) == 0);
}

FACTS(BackFrontShuffleDeck) {
  for (int a=1; a <= CARDS; ++a) {
    for (int b=0; b <= CARDS; ++b) {    
      Deck deck,spare;
      sampleDeck(&deck,a,b);
      testBackFrontShuffleDeck(&deck,&spare);
    }
  }
}



void BackFrontUnshuffleDeck(Deck *in, Deck *out) {
  for (int i=0; i<CARDS; ++i) {
    out->cards[BACK_FRONT[i]]=in->cards[i];
  }
  setAts(out);
}

FACTS(BackFrontUnshuffle) {
  for (int a=1; a <= CARDS; ++a) {
    for (int b=0; b <= CARDS; ++b) {
      Deck deck,shuffled,unshuffled;
      sampleDeck(&deck,a,b);
      SpiderCipherBackFrontShuffleDeck(&deck,&shuffled);
      FACT(deckCmp(&deck,&shuffled),!=,0);      
      BackFrontUnshuffleDeck(&shuffled,&unshuffled);
      FACT(deckCmp(&deck,&unshuffled),==,0);
    }
  }
}

void CutShuffleCut(Deck *deck,
                   int8_t cutAtBefore,int8_t cutAtAfter) {
  Deck spare;
  CutDeckAt(deck,cutAtBefore,&spare);
  SpiderCipherBackFrontShuffleDeck(&spare,deck);
  CutDeckAt(deck,cutAtAfter,&spare);
  SpiderCipherCopyDeck(&spare,deck);
}

void InverseCutShuffleCut(Deck *deck,
                   uint8_t cutAtBefore,uint8_t cutAtAfter) {
  Deck spare;
  CutDeckAt(deck,-cutAtAfter,&spare);
  BackFrontUnshuffleDeck(&spare,deck);
  CutDeckAt(deck,-cutAtBefore,&spare);
  SpiderCipherCopyDeck(&spare,deck);
}

void CutShuffleCutTestDeck(struct FactsStruct *facts, Deck *original) {
    Deck deck,expect,spare;
    for (int cutAtBefore=0; cutAtBefore<CARDS; ++cutAtBefore) {
        for (int cutAtAfter=0; cutAtAfter<CARDS; ++cutAtAfter) {
    SpiderCipherCopyDeck(original,&deck);
    SpiderCipherCopyDeck(original,&expect);
    CutShuffleCut(&deck,cutAtBefore,cutAtAfter);
    deckMix(&expect,*CUTS[cutAtBefore]);
    deckMix(&expect,BACK_FRONT);
    deckMix(&expect,*CUTS[cutAtAfter]);
    FACT(deckCmp(&deck,&expect),==,0);
    InverseCutShuffleCut(&deck,cutAtBefore,cutAtAfter);
    FACT(deckCmp(&deck,original),==,0);
        }
    }
}

FACTS_EXCLUDE(CutShuffleCutSlower) {
  for (int a=1; a <= CARDS; ++a) {
    for (int b=0; b <= CARDS; ++b) {
      Deck original;
      sampleDeck(&original,a,b);
      CutShuffleCutTestDeck(facts,&original);
    }
  }
}

FACTS(CutShuffleCut) {
  for (int a=1; a <= CARDS; ++a) {
    for (int b=0; b <= CARDS; ++b) {
      // skip about 90% of the tests...
      if ((a*1299827+9973*b) % 11 != 0) continue;
      Deck original;
      sampleDeck(&original,a,b);
      CutShuffleCutTestDeck(facts,&original);
    }
  }
}

void PseudoShuffleCutAt(Deck *deck, uint8_t cutAt) {
  CutShuffleCut(deck,cutAt,0);
}

void InversePseudoShuffleCutAt(Deck *deck, uint8_t cutAt) {
  InverseCutShuffleCut(deck,cutAt,0);
}

FACTS(CutCardUniform) {
  for (int a=1; a <= CARDS; ++a) {
    for (int b=0; b <= CARDS; ++b) {    
      Deck deck,spare;
      int counts[CARDS] = {0};
      for (Card clear = 0; clear < CARDS; ++clear) {
	sampleDeck(&deck,a,b);
	SpiderCipherAdvanceDeck(&deck,clear,&spare);
	++counts[SpiderCipherCutCard(&deck,0)];
      }
      for (Card card = 0; card < CARDS; ++card) {
	FACT(counts[card],==,1);
      }
    }
  }
}

FACTS(NoiseCardUniform) {
  for (int a=1; a <= CARDS; ++a) {
    for (int b=0; b <= CARDS; ++b) {    
      Deck deck,spare;
      int counts[CARDS] = {0};
      for (Card clear = 0; clear < CARDS; ++clear) {
	sampleDeck(&deck,a,b);
	SpiderCipherAdvanceDeck(&deck,clear,&spare);
	++counts[SpiderCipherScramble(&deck,0)];
      }
      for (Card card = 0; card < CARDS; ++card) {
	FACT(counts[card],==,1);
      }
    }
  }
}

// pseudo-shuffle on cut at location cutAt
void P(Deck *deck,int cutAt) {
  PseudoShuffleCutAt(deck,cutAt);
}

void pd(Deck *deck) {
  for (int i=0; i < CARDS; ++i) {
    printf(" %02d",deck->cards[i]);
    if (i % 10 == 9) printf("\n");
  }
}


FACTS(P) {
  for (int a=1; a <= CARDS; ++a) {
    for (int b=0; b <= CARDS; ++b) {
      for (int cutAt=0; cutAt<CARDS; ++cutAt) {
	Deck deck,spare,expect;
	sampleDeck(&deck,a,b);
	CutDeckAt(&deck,cutAt,&spare);
	permute(BACK_FRONT,&spare,&expect);
	P(&deck,cutAt);
	FACT(deckCmp(&deck,&expect),==,0);
      }
    }
  }
}

void I(Deck *deck) {
  //id;
}

FACTS(IReachable) {
  Deck id;
  SpiderCipherDeckInit(&id);
  I(&id);

  Deck reach;
  SpiderCipherDeckInit(&reach);
  for (int k=0; k<9; ++k) {
    P(&reach,2);
  }

  FACT(deckCmp(&reach,&id),==,0);
}

void Q(Deck *deck,int cutAt) {
  InversePseudoShuffleCutAt(deck,cutAt);  
}

FACTS(Q) {
  for (int cutAt=0; cutAt<CARDS; ++cutAt) {
    Deck deck;
    Deck id;
    SpiderCipherDeckInit(&deck);
    SpiderCipherDeckInit(&id);
    P(&deck,cutAt);
    FACT(deckCmp(&deck,&id),!=,0);
    Q(&deck,cutAt);
    FACT(deckCmp(&deck,&id),==,0);
  }
}

FACTS(QReachable) {
  for (int cutAt=0; cutAt<CARDS; ++cutAt) {
    Deck q;
    SpiderCipherDeckInit(&q);
    Q(&q,cutAt);
	  
    Deck reach;
    SpiderCipherDeckInit(&reach);

    for (int i=0; i<17; ++i) {
      int pCutAt=2;
      if (i == 8) {
	pCutAt = (4+(CARDS-cutAt))%CARDS;
      }
      P(&reach,pCutAt);
    }
    FACT(deckCmp(&reach,&q),==,0);
  }
}

void T(Deck *deck, int cutAt) {
  Deck spare;
  CutDeckAt(deck,cutAt,&spare);
  permute(ID,&spare,deck);
}

FACTS(T) {
  for (int cutAt=0; cutAt<CARDS; ++cutAt) {
    Deck t;
    SpiderCipherDeckInit(&t);
    T(&t,cutAt);

    for (int i=0; i<CARDS; ++i) {
      FACT(t.cards[i],==,(i+cutAt) % CARDS);
    }
  }
}

FACTS(TReachable) {
  for (int cutAt=0; cutAt<CARDS; ++cutAt) {
    Deck t;
    SpiderCipherDeckInit(&t);
    T(&t,cutAt);

    Deck reach;
    SpiderCipherDeckInit(&reach);
    for (int i=0; i<9; ++i) {
      PseudoShuffleCutAt(&reach,(i == 0) ? (2+cutAt)%CARDS : 2);
    }
    
    FACT(deckCmp(&reach,&t),==,0);
  }
}

void R(Deck *deck) { // reverse deck
  Deck spare;
  for (int i=0; i<CARDS; ++i) {
    spare.cards[i]=deck->cards[(CARDS-1)-i];
  }
  setAts(&spare);
  
  permute(ID,&spare,deck);
}

FACTS(R) {
  Deck r;
  SpiderCipherDeckInit(&r);
  R(&r);
  for (int i=0; i<CARDS; ++i) {
    FACT(r.cards[i],==,CARDS-1 - i);
  }
}

FACTS(RReachable) {
  Deck r;
  SpiderCipherDeckInit(&r);
  R(&r);

  Deck reach;
  SpiderCipherDeckInit(&reach);
  P(&reach,0);
  T(&reach,20);
  Q(&reach,0);

  FACT(deckCmp(&reach,&r),==,0);
}

void X(Deck *deck) { // exchange pairs
  Deck spare;
  for (int i=0; i<CARDS; i += 2) {
    int j=i+1;
    spare.cards[i]=deck->cards[j];
    spare.cards[j]=deck->cards[i];
  }
  setAts(&spare);
  permute(ID,&spare,deck);
}

FACTS(X) {
  Deck x;
  SpiderCipherDeckInit(&x);
  X(&x);
  for (int i=0; i<CARDS; ++i) {
    FACT(x.cards[i],==,(i % 2 == 0) ? i+1 : i-1);
  }
}

FACTS(XReachable) {
  Deck x;
  SpiderCipherDeckInit(&x);
  X(&x);

  Deck reach;
  SpiderCipherDeckInit(&reach);

  P(&reach,0);
  R(&reach);
  Q(&reach,0);

  FACT(deckCmp(&reach,&x),==,0);
}

void B(Deck *deck, int i) { // exchange i with i+1
  int j = (i+1)%CARDS;
  Card a = deck->cards[i];
  Card b = deck->cards[j];
  deck->cards[i]=b;
  deck->cards[j]=a;
  deck->ats[a]=j;
  deck->ats[b]=i;
}

FACTS(B) {
  for (int i=0; i<CARDS; ++i) {
    Deck b;
    SpiderCipherDeckInit(&b);
    B(&b,i);
    for (int k=0; k<CARDS; ++k) {
      int j=(i+1) % CARDS;
      if (k == i) FACT(b.cards[k],==,j);
      if (k == j) FACT(b.cards[k],==,i);
      if (k != i && k != j) FACT(b.cards[k],==,k);
    }
  }
}

FACTS(BReachable) {
  for (int i=0; i<CARDS; ++i) {
    Deck b;
    SpiderCipherDeckInit(&b);
    B(&b,i);

    Deck reach;
    SpiderCipherDeckInit(&reach);

    
    T(&reach,(i+1)%CARDS);
    P(&reach,0);
    T(&reach,21);
    Q(&reach,0);
    R(&reach);
    X(&reach);
    T(&reach,1);
    X(&reach);
    T(&reach,CARDS-1);
    T(&reach,(2*CARDS-(i+1)) % CARDS);
    
    FACT(deckCmp(&reach,&b),==,0);
  }
}

void S(Deck *deck, int i, int j) { // swap i and j
  Card a = deck->cards[i];
  Card b = deck->cards[j];
  deck->cards[i]=b;
  deck->cards[j]=a;
  deck->ats[a]=j;
  deck->ats[b]=i;
}

FACTS(S) {
  for (int i=0; i<CARDS; ++i) {
    for (int j=0; j<CARDS; ++j) {
      Deck s;
      SpiderCipherDeckInit(&s);
      S(&s,i,j);
      
      for (int k=0; k<CARDS; ++k) {
	if (k == i) FACT(s.cards[k],==,j);
	if (k == j) FACT(s.cards[k],==,i);
	if (k != i && k != j) FACT(s.cards[k],==,k);
      }
    }
  }
}

FACTS(SReachable) {
  for (int i=0; i<CARDS; ++i) {
    for (int j=0; j<CARDS; ++j) {
      Deck s;
      SpiderCipherDeckInit(&s);
      S(&s,i,j);
      
      Deck reach;
      SpiderCipherDeckInit(&reach);

      if (i != j) {
	int a = (i < j) ? i : j;
	int b = (i > j) ? i : j;

	T(&reach,a);	
	for (int k=0; k<b-a; ++k) {
	  B(&reach,k);
	}
	for (int k=b-a-2; k>=0; --k) {
	  B(&reach,k);
	}
	T(&reach,(CARDS-a));
      }
      FACT(deckCmp(&reach,&s),==,0);
    }
  }
}

const int CYCLE_LENGTHS [] =
  {
   27,  30,   9, 110,  12,  90,  99, 234, 105,  12,
   60, 126, 115,  56,  20, 174,  12,  66, 105,  20,
   39,  40,  39,  60,  72, 150,  60,  40,  39, 264,
   36, 380,  75,  20,  60,  40, 182, 190, 440,  24
  };

const int PERFECT_CYCLE_LENGTHS [] =
  {
  36, 465,  36, 176,  30,  39, 180, 105, 390,  48,
 330, 175, 140,  88,  30,  96,  60,  39,  36, 220,
  38, 279, 380,  84,1848, 420, 780,  84, 380, 264,
 132, 240,1400,  30,  60,  35, 308, 145,1848, 168
  };

FACTS(Cycles) {
  int ok = 1;
  
  Deck t[CARDS];
  for (int c = 0; c<CARDS; ++c) {
    SpiderCipherDeckInit(&t[c]);
    T(&t[c],c);
  }
  
  for (int inverse = 0; inverse < 2; ++inverse) {
    for (int perfect = 0; perfect < 1; ++perfect) { // !! skip perfect
      for (int c = 0; c < CARDS; ++c) {
	Deck deck;
	SpiderCipherDeckInit(&deck);
	int i = 0,eq=-1;
	while (eq == -1) {
	  if (inverse) {
	    Q(&deck,c);
	    if (perfect) {
	      S(&deck,CARDS/2-1,CARDS-1);
	    }
	  } else { 
	    if (perfect) {
	      S(&deck,CARDS/2-1,CARDS-1);
	    }
	    P(&deck,c);
	  }
	  
	  ++i;
	  for (int k=0; k<CARDS; ++k) {
	    if (deckCmp(&deck,&t[k])==0) {
	      eq = k;
	    }
	  }
	}

	int length = perfect ? PERFECT_CYCLE_LENGTHS[c] : CYCLE_LENGTHS[c];

	if (perfect) {
	  FACT(i,>=,30);
	}
	
	if (i != length || eq != 0) {
	  printf("cycles %d = %d\n",c,i);
	  ok = 0;
	}
      }
    }
  }
  
  FACT(ok,==,1);
}

FACTS_FAST