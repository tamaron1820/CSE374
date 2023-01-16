/* bench.c is the benchmarking /test program for mem memory management
   bench.c is implemented as part of HW6, CSE374 22WI
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "mem.h"
#include "mem_impl.h"


/* print_stats is a helper function that prints performance
   information about the memory system.  Requires a clock_t start
   time, and prints elapsed time plus results from get_mem_stats()
*/
void print_stats(clock_t start);

/* fill_mem is a helper function that takes in an address and the
   size of the memory allocation at that address.  It should fill
   the first 16 bytes (or up to size) with hexedecimal values.*/
void fill_mem(void* ptr, uintptr_t size);


/* Synopsis:   bench (main)
   [ntrials] (10000) getmem + freemem calls
   [pctget] (50) % of calls that are get mem
   [pctlarge] (10) % of calls requesting more memory than lower limit
   [small_limit] (200) largest size in bytes of small block
   [large_limit] (20000) largest size in byes of large block
   [random_seed] (time) initial seed for randn
*/
double time_elapse;
int main(int argc, char** argv) {
  // Initialize the parameters
  int NTRIALS;
  int PCTGET;
  int PCTLARGE;
  int SMALL_L;
  int LARGE_L;

  (argc > 1) ? (NTRIALS = atoi(argv[1])) : (NTRIALS = 10000);
  (argc > 2) ? (PCTGET = atoi(argv[2])) : (PCTGET = 50);
  (argc > 3) ? (PCTLARGE = atoi(argv[3])) : (PCTLARGE = 10);
  (argc > 4) ? (SMALL_L = atoi(argv[4])) : (SMALL_L = 200);
  (argc > 5) ? (LARGE_L = atoi(argv[5])) : (LARGE_L = 20000);

  // initialize random number gen.
  (argc > 6) ? srand(atoi(argv[6])) : srand(time(NULL));

  printf("Running bench for %d trials, %d%% getmem calls.\n", NTRIALS, PCTGET);
  
  void* blocks[NTRIALS];  // upperbound block storage
  clock_t start = clock();
  int oneTenth = NTRIALS / 10;
  uintptr_t stat0 = 0;
  uintptr_t stat1 = 0;
  uintptr_t stat2 = 0;
  uintptr_t *totalmalloc = &stat0;
  uintptr_t *total_free = &stat1;
  uintptr_t *nFreeBlocks = &stat2;
  uintptr_t Size;
  int prob,total_pointer;

  // perform NTRIALS mem operations
  for (int i = 0; i < NTRIALS; i++) {
    if (i % oneTenth == 0 && i != 0) {
      printf("    %d percent done\n", i / oneTenth * 10);
      get_mem_stats(totalmalloc, total_free, nFreeBlocks);
      time_elapse = ((double) (clock() - start)) / CLOCKS_PER_SEC;
      printf("    CPU TIME: %f seconds\n", time_elapse);
      printf("    %lu bytes used by underlying system\n", stat0);
      printf("    %lu blocks on the free list\n", stat2);
      if (stat2 != 0) {
        printf("    Average of %lu bytes per free block\n\n", stat1 / stat2);
      } else {
        printf("    Average of 0 bytes per free block\n\n");
      }
    }
    prob = rand() % 100;
    if (prob < PCTGET) {
      prob = rand() % 100;
      Size=0;
      if (prob < PCTLARGE) {
        Size = (uintptr_t) (rand() % (LARGE_L - SMALL_L) + SMALL_L);
      } else {
        Size = (uintptr_t) (rand() % SMALL_L);
      }
      blocks[total_pointer] = getmem(Size);
      total_pointer++;
    } else {  // freemem
      if (total_pointer > 0) {
        int index = rand() % total_pointer;
        freemem(blocks[index]);
        blocks[index] = blocks[total_pointer - 1];
        total_pointer--;
      }
    }
  }
  /* Students will fill in this section according to the assignment
     specs.*/
  printf("    100 percent done\n");
  get_mem_stats(totalmalloc, total_free, nFreeBlocks);
  time_elapse = ((double) (clock() - start)) / CLOCKS_PER_SEC;
  printf("    CPU TIME: %f seconds\n", time_elapse);
  printf("    %lu bytes used by underlying system\n", stat0);
  printf("    %lu blocks on the free list\n", stat2);
  if (stat2 != 0) {
    printf("    Average of %lu bytes per free block\n\n", stat1 / stat2);
  } else {
    printf("    Average of 0 bytes per free block\n\n");
  }
  printf("All tests passed\n");
  return EXIT_SUCCESS;
}
