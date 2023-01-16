/* 
   Copyright [2022] <Tatsuhiko Araki>
   MakeFile
   CSE374, HW5, 22au 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trienode.h"


int main(int argc, char* argv[]) {
  FILE * dictionary = NULL;
  trieNode *wordTrie = NULL;
  trieNode *root = makeNode();
  char line[100];

  if (argc < 2) {
    fprintf(stderr, "Usage: %s [DICTIONARY]\n", argv[0]);
    return EXIT_FAILURE;
  } else {
    dictionary = fopen(argv[1], "r");
    if (!dictionary) {
      fprintf(stderr, "Error: Cannot open %s\n", argv[1]);
      return EXIT_FAILURE;
    }
  }
  while (fgets(line, 100, dictionary) != NULL) {
    build_tree(root, line);
  }
  fclose(dictionary);
  printf("Enter \"exit\" to quit.\n");
  char str[100];
  wordTrie = root;
  while (1) {
    printf("Enter Key Sequence (or \"#\" for next word): \n");
    scanf("%s", str);
    if (strcmp("exit", str) != 0) {
      if (str[0] != '#') {
      wordTrie = get_word(root, str);
      } else if (wordTrie != NULL && str[0] == '#') {
      wordTrie = get_word(wordTrie, str);
      }
      if (wordTrie != NULL) {
        if (wordTrie-> word) {
          printf("\'%s\'\n", wordTrie -> word);
        } else {
          printf("Not found in current dictionary.\n");
        }
      } else {
        if (str[strlen(str) - 1] == '#') {
          printf("There are no more T9onyms\n");
        } else {
          printf("Not found in current dictionary.\n");
        }
      }
    } else {
      break;
    }
  }
  free_tree(root);
  free(root);
  return(EXIT_SUCCESS);
}
