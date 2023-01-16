/* getmem.c
   implements getmem (malloc) for memory system
   CSE 374 HW6
   Tatsuhiko Araki, Shin Komori
 *
 * getmem.c - Returns a pointer to a new block of storage with at least
 * the given size of bytes of memory. Block will be aligned on a 16
 * byte boundary. The given size should be greater than 0. The function
 * will return NULL if the given value is negative or getmem cannot
 * satisfy the request.
 **/

#include <assert.h>
#include "mem_impl.h"
#include "mem.h"

#define INITIAL_CAPACITY 10000
#define THRESHOLD 96
#define EXTRA_SIZE 100000

/* This code will be filled in by the students.  Your primary job is to
   implement getmem */

/* initialize global variables ?*/
uintptr_t total_free;
/* Are there helper functions that only getmeme will need?  Declare here. */

/* Define your functions below: */

void * getmem(uintptr_t size) {
  check_heap();

  if (size <= 0) {
    return NULL;
  }

  uintptr_t headerSize = 2 * sizeof(uintptr_t);
  // Only used for the first call the getmem
  if (!fro) {
    // initialize free list with a large block of memory
    uintptr_t requestSize = align(INITIAL_CAPACITY);
    fro = (freeNode *) malloc(requestSize);
    fro->size = requestSize - headerSize;
    fro->next = NULL;
    totalmalloc += requestSize;
    total_free += requestSize;
    nFreeBlocks++;
  }

  // fro will never be NULL at this point
  assert(fro != NULL);

  // Returning too small block will violate
  // heap condition 2 when it's freed later.
  // Make size to be multiple of 16 bytes
  if (size < MINCHUNK) {
    size = MINCHUNK;
  }
  size = align(size);
  assert(size % (2 * sizeof(void*)) == 0);

  // fro case: spliting the first block
  if (fro->size >= size + THRESHOLD + headerSize) {
    freeNode * remainderfro = split(fro, size);
    void * userBlock = (void *) (((uintptr_t) fro) + headerSize);
    fro = remainderfro;
    total_free -= (size + sizeof(freeNode));
    return userBlock;
  }

  // middle, end case (here, free list has at least one block):
  // spliting the non-first block
  freeNode * prev = fro;
  freeNode * cur = fro->next;
  while (cur) {
    if (cur->size >= size + THRESHOLD + headerSize) {
      freeNode * remainderfro = split(cur, size);
      void * userBlock = (void *) (((uintptr_t) cur) + headerSize);
      prev->next = remainderfro;
      total_free -= (size + sizeof(freeNode));
      return userBlock;
    } else {
      prev = cur;
      cur = cur->next;
    }
  }

  // block with enough size not found in the free list.
  // request a new block with at least the 'size'
  uintptr_t requestSize = align(size + EXTRA_SIZE);
  freeNode * newNode = (freeNode *) malloc(requestSize);
  newNode->size = requestSize - headerSize;
  newNode->next = NULL;
  totalmalloc += requestSize;
  total_free += (requestSize - (size + sizeof(freeNode)));
  nFreeBlocks++;

  // end case: malloc
  // 1. newfro = split(newNode, size);
  // 2. insert(newNode);
  // 3. return (void *) ((uintptr_t) newNode + headerSize);
  freeNode * remainderfro = split(newNode, size);
  insert(remainderfro);
  void * userBlock = (void *) (((uintptr_t) newNode) + headerSize);

  check_heap();
  return userBlock;
}

