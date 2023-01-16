t9: tnine.o trie.o
	gcc -Wall -g -std=c11 -o t9 tnine.o trie.o

tnine.o: tnine.c trienode.h
	gcc -Wall -g -std=c11 -c tnine.c

trie.o: trie.c trienode.h
	gcc -Wall -g -std=c11 -c trie.c

clean:
	rm -f *.o t9 *~