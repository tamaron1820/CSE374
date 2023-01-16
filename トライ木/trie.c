/*
   Copyright [2022] <Tatsuhiko Araki>
   trie implements a trie, made of trieNodes. This includes
   code to build, search, and delete a trie
   CSE374, HW5, 22au 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "trienode.h"

// making new node which is empty
trieNode * makeNode() {
  trieNode * node = (trieNode *) malloc(sizeof(trieNode));
  node -> word = NULL;
  int i;
  for (i = 0; i < 9; i++) {
    node -> branches[i] = NULL;
  }
  return node;
}

// return numbers of each character
int find_number(char c) {
  int i;
  if (c=='a' || c=='b' || c=='c' || c=='A' || c=='B' || c=='C') {
    i=2;
  } else if (c=='d' || c=='e' || c=='f' || c=='D' || c=='E' || c=='F') {
    i=3;
  } else if (c=='g' || c=='h' || c=='i' || c=='G' || c=='H' || c=='I') {
    i=4;
  } else if (c=='j' || c=='k' || c=='l' || c=='J' || c=='K' || c=='L') {
    i=5;
  } else if (c=='m' || c=='n' || c=='o' || c=='M' || c=='N' || c=='O') {
    i=6;
  } else if (c=='p' || c=='q' || c=='r' || c=='s' || c=='P' || c=='Q'
  || c=='R' || c=='S') {
    i=7;
  } else if (c=='t' || c== 'u' || c=='v' || c=='T' || c=='U' || c=='V') {
    i=8;
  } else if (c=='w' || c=='x' || c=='y' || c=='z' || c=='W' || c=='X'
  || c=='Y' || c=='Z') {
    i=9;
  }
  return i;
}

// build a trie node
void build_tree(trieNode * root, char * word) {
  trieNode * curr = root;
  int length = strlen(word);
  char *str = (char *) malloc(length);
  if (str != NULL) {
    strncpy(str, word, length - 1);
    str[length - 1] = '\0';
  }
  int i = 0;
  while (word[i] != '\n') {
    int index = find_number(word[i])-2;
    if (curr -> branches[index] == NULL) {
      curr -> branches[index] = makeNode();
    }
    curr = curr -> branches[index];
    i++;
  }
  while (curr -> branches[8] != NULL) {
    curr = curr -> branches[8];
  }
  if (curr -> word == NULL) {
    curr -> word = str;
  } else {
    curr -> branches[8] = makeNode();
    curr = curr -> branches[8];
    curr -> word = str;
  }
}

// return edge of trie node
trieNode * get_word(trieNode * root, char * pattern) {
  trieNode * curr = root;
  int i = 0;
  while (i < strlen(pattern)) {
    if ((pattern[i] - '2') != -15) {
      if ((pattern[i] - '2') > 8 || (pattern[i] - '2') < 0) {
        return NULL;
      }
    }
    if (pattern[i] != '#') {
      if (curr -> branches[(pattern[i] - '2')] == NULL) {
        return NULL;
      }
      curr = curr -> branches[(pattern[i] - '2')];
    } else {
      if (curr -> branches[8] == NULL) {
        return NULL;
      }
      curr = curr -> branches[8];
    }
    i++;
  }
  return curr;
}

// free all trie
void free_tree(trieNode * root) {
  int i;
  for (i = 0; i < 9; i++) {
    if (root -> branches[i] != NULL) {
      free_tree(root -> branches[i]);
      free(root -> branches[i]);
    }
  }
  if (root -> word != NULL) {
    free(root -> word);
  }
}
