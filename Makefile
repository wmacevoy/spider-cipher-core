CSTD?=-std=c11  -D_POSIX_C_SOURCE=199309L
CDBG?=-g
COPT?=-O2
CINC?=-Iinclude

CFLAGS=$(CDBG) $(COPT) $(CSTD) $(CINC)

LDLIBS=-lm

.PHONY: all

all : tests/spider_cipher_core_facts

tests/spider_cipher_core_facts : src/spider_cipher_core.c include/spider_cipher_core.h tests/spider_cipher_core_facts.c tests/facts.h tests/facts.c
	$(CC) -o tests/spider_cipher_core_facts $(CFLAGS) $(LDFLAGS) src/spider_cipher_core.c tests/spider_cipher_core_facts.c tests/facts.c $(LDLIBS)

.PHONY: check
check : all
	bin/spider_cipher_facts | diff - expected/spider_cipher_facts.out

.PHONY: expected
expected : all
	mkdir -p expected
	bin/spider_cipher_facts >expected/spider_cipher_facts.out