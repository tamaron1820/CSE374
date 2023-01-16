 /*
   Copyright [2022] <Tatsuhiko Araki> 
   trienode.h contains structure and function declarations for 
   a trie implementation 
   CSE374, HW5, 22au 
*/
#ifndef TRIENODE_H
#define TRIENODE_H

#include <stdio.h>

#define MAXLEN 50
#define BRANCHES 9

// trieNode is one node in an 9-tree, where branches correspond to
// # and the T9 alphabet.
typedef struct trieNode {
  struct trieNode *branches[BRANCHES];
  char *word;
} trieNode;

// create new Node
trieNode* makeNode();
// given a character,return the number of each character
int find_number(char c);
// given a trieNode, build a trie that holds
// each word.
void build_tree(trieNode * root, char * s);
// given a pattern, return the edge of trieNode
trieNode* get_word(trieNode * root, char * pattern);
// deletes the entire tree branching from root
void free_tree(trieNode *root);

#endif  // TRIENODE_H
