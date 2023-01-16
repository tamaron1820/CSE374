
/* freemem.c
   implements freemem (free) for memory system
   CSE 374 HW6
   Tatsuhiko Araki, Shin Komori
 *
 * freemem.c - Puts the assign_block storage block back to free storage.
 * freemem will not do anything if the pointer p equals to NULL
 *
 * preconditions:
 * - p must be one of the values acquried from getmem
 * - there's no expected behavior if p has already been freed
 *   or if p is not a storage block from getmem (might crash)
 **/

#include <assert.h>
#include "mem_impl.h"
#include "mem.h"

void freemem(void* p) {
  check_heap();
  if (p) {
    freeNode* prev = NULL;
    freeNode* cur = fro;
    freeNode* assign_block = (freeNode*) ((uintptr_t) p - sizeof(freeNode));
    freeNode* end = (freeNode*) ((uintptr_t) assign_block + assign_block->size + sizeof(freeNode));

    // Updates total number of bytes inside the free list
    total_free += assign_block->size + sizeof(freeNode);

    // Traverses through the FreeList
    while (cur && (end > cur)) {
      prev = cur;
      cur = cur->next;
    }

    // curly looking at the beginning of free storage
    if (prev == NULL) {
      if (end == fro) {  // Merge w/ first block in storage
        assign_block->next = fro->next;
        assign_block->size = assign_block->size + fro->size + sizeof(freeNode);
      } else {  // Insert block to the beginning of storage
        assign_block->next = fro;
        nFreeBlocks++;
      }
      fro = assign_block;
      return;
    }

    // Acquires the address of the next adjacent block
    freeNode* end_last_block = (freeNode*) ((uintptr_t) prev
                                    + prev->size
                                    + sizeof(freeNode));

    // curly looking at the end of free storage
    if (cur == NULL) {  // Insert to the end of free storage
      prev->next = assign_block;
      assign_block->next = NULL;
      nFreeBlocks++;
      return;
    }

    // curly looking inside of free storage
    if (end == cur && assign_block == end_last_block) {
      // Connect to the two adjacent blocks
      prev->size = prev->size + assign_block->size + cur->size + 32;
      prev->next = cur->next;
      nFreeBlocks--;
    } else if (assign_block == end_last_block) {
      // Connect to bottom of the left block
      prev->size = prev->size + assign_block->size + 16;
    } else if (end == cur) {
      // Connect to top of the right block
      assign_block->next = cur->next;
      prev->next = assign_block;
      assign_block->size = assign_block->size + cur->size + 16;
    } else {  // Insert block in the middle of the two adjacent blocks
      assign_block->next = cur;
      prev->next = assign_block;
      nFreeBlocks++;
    }
  }
  check_heap();
}
