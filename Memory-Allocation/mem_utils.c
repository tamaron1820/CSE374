/* mem_utils.c utilities to support memory management system.
   HW6, CSE 374
*/

#include "mem.h"
#include "mem_impl.h"

// global variable needs to be initialized to a constant value
// node * fro = (node *) malloc(size); does not work
freeNode *fro = NULL;
uintptr_t totalmalloc = 0;
uintptr_t totalFree = 0;
uintptr_t nFreeBlocks = 0;
void check_heap() {

  if (!fro) return;
  freeNode* cur_Node = fro;
  uintptr_t minimum_size = cur_Node->size;

  while (cur_Node != NULL) {
    if (cur_Node->size < minimum_size) {
      minimum_size = cur_Node->size;
    }
    if (cur_Node->next != NULL) {
      assert((uintptr_t)cur_Node <(uintptr_t)(cur_Node->next));
      assert((uintptr_t)cur_Node + cur_Node->size + NODESIZE
              <(uintptr_t)(cur_Node->next));
    }
    cur_Node = cur_Node->next;
  }
  // go through free list and check for all the things
  if (minimum_size == 0) print_heap( stdout);
  assert(minimum_size >= MINCHUNK);
}
void get_mem_stats(uintptr_t* total_size, uintptr_t* total_free,
                   uintptr_t* n_free_blocks) {
  *total_size = totalmalloc;
  *total_free = 0;
  *n_free_blocks = 0;

  freeNode* cur_Node = fro;
  while (cur_Node) {
    *n_free_blocks = *n_free_blocks + 1;
    *total_free = *total_free + (cur_Node->size + NODESIZE);
    cur_Node = cur_Node->next;
  }
}

void print_heap(FILE *f) {
  printf("Printing the heap\n");
  freeNode* cur_Node = fro;
  while (cur_Node !=NULL) {
    fprintf(f, "%" PRIuPTR, (uintptr_t)cur_Node);
    fprintf(f, ", size: %" PRIuPTR, cur_Node->size);
    fprintf(f, ", next: %" PRIuPTR, (uintptr_t)cur_Node->next);
    fprintf(f, "\n");
    cur_Node = cur_Node->next;
  }
}

void insert(freeNode * next_Node) {
  // Sanity check to make sure insert function
  // is not called before the first call to getmem.
  // Also, inserting NULL won't do anything.
  if (!fro || !next_Node) {
    return;
  }

  uintptr_t next_NodeAddress = (uintptr_t) next_Node;

  // Given block goes to fro of free storage
  if (next_NodeAddress < (uintptr_t) fro) {
    next_Node->next = fro;
    fro = next_Node;
    return;
  }

  // Given block goes to middle of free storage
  freeNode* cur = fro;
  freeNode* nextNode = fro->next;
  while (cur && nextNode) {
    if (next_Node > cur && next_Node < nextNode) {
      next_Node->next = nextNode;
      cur->next = next_Node;
      return;
    } else {
      cur = nextNode;
      nextNode = nextNode->next;
    }
  }

  // Given block goes to end of the fro
  cur->next = next_Node;
  return;
}

freeNode * split(freeNode * cur, uintptr_t size) {
    uintptr_t headerSize = 2 * sizeof(uintptr_t);
    uintptr_t next_size = cur->size - size - headerSize;
    uintptr_t next_address = ((uintptr_t) cur) + headerSize + size;

    freeNode * new_cur = (freeNode *) next_address;
    new_cur->size = next_size;
    new_cur->next = cur->next;

    cur->size = size;
    cur->next = NULL;

    return new_cur;
}

uintptr_t align(uintptr_t address) {
  uintptr_t bound = 2 * sizeof(uintptr_t);
  if (address % bound == 0) {
    return address;
  } else {
    return address + (bound - address % bound);
  }
}