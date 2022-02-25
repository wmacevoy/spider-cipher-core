  static void SpiderCipherSwap(Deck deck, uint8_t i, uint8_t j) {
    if (i >= SPIDER_CIPHER_CARDS || j >= SPIDER_CIPHER_CARDS) return;
    
    SpiderCipherCard a = deck->cards[i];
    SpiderCipherCard b = deck->cards[j];

    deck->cards[i]=b;
    deck->cards[j]=a;
    deck->ats[a]=j;
    deck->ats[b]=i;

    a=0;
    b=0;
  }

int SpiderCipherRandomize(SpiderCipher *deck,
			    int64_t (*rand)(void *misc),
			    void *misc,
			    int64_t randMax) {
    Deck pool;
    Deck spare;
    int64_t q,qMax,r,rMax,nr;
    SpiderCipherCard card;
    uint8_t i,j,n;
    
    SpiderCipherDeckInit(&pool);
    SpiderCipherDeckInit(&spare);

    nr=0;
    rMax=randMax+1;
    qMax=1;
    while (qMax < SPIDER_CIPHER_CARDS*SPIDER_CIPHER_CARDS) {
      qMax *= rMax;
      ++nr;
    }

    for (i=0; i<SPIDER_CIPHER_CARDS-1; ++i) {
      n=SPIDER_CIPHER_CARDS-i;
      do {
	q=0;
	for (j=0; j<nq; ++j) {
	  r = rand(misc);
	  if (r < 0 || r >= rMax) return 0;
	  q = rMax*q + r;
	}
      } while (q >= (qMax-qMax%n));
      j=(uint8_t)(i + (q%n));
      SpiderCipherSwap(pool,i,j);
    }


    qMax=qMax-qMax%SPIDER_CIPHER_CARDS;

    for (i=0; i<SPIDER_CIPHER_CARDS; ++i) {
      do {
	q=0;
	for (j=0; j<nq; ++j) {
	  r = rand(misc);
	  if (r < 0 || r >= rMax) return 0;
	  q = rMax*q + r;
	}
      } while (q >= qMax);
      card = (q % SPIDER_CIPHER_CARDS);
      card = SpiderCipherCutCard(pool,card);
      SpiderCipherCutDeck(pool,card,spare);
      SpiderCipherBackFrontShuffleDeck(spare,pool);
      
      card = SpiderCipherNoiseCard(pool);
      card = SpiderCipherCutCard(deck,card);
      SpiderCipherCutDeck(deck,card,spare);
      SpiderCipherBackFrontShuffleDeck(spare,deck);
    }

    SpiderCipherDeckInit(&spare);
    SpiderCipherDeckInit(&pool);
    
    q=qMax=r=rMax=nr=0;
    card=0;
    i=j=n=0;
    
    return 1;
  }
