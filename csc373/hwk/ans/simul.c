#include <stdio.h>

#define SetCount   (8)
#define BlockCount (2)

#define Empty      (-1)
#define true       (1)
#define false      (0)

int sets[SetCount][BlockCount];
int lru[SetCount];

int already_in_p(int word, int set_index) {
  int w = 0;
  while (w < BlockCount) 
    if (word == sets[set_index][w]) return true;
    else w++;
  return false;
}

int main() {
  int reference_string[ ] = 
    {1, 4, 8, 5, 20, 17, 19, 56, 9, 11, 4, 43, 5, 6, 9, 17, 9, 56, 9, Empty};

  int i, j;
  /* Initialize the cache to empty. */
  for (i = 0; i < SetCount; i++) 
    for (j = 0; j < BlockCount; j++)
      sets[i][j] = Empty;

  /* Initialize the LRU pointers to -1 */
  for (i = 0; i < SetCount; i++)
    lru[i] = Empty;

  /* Iterate through the reference string. */
  i = 0;
  while (reference_string[i] != Empty) {
    int next_word = reference_string[i];
    int set_index = next_word % SetCount;

    /* hit */
    if (already_in_p(next_word, set_index))
      printf("Hit for word %i in Set %i\n\n", next_word, set_index);
    /* miss */
    else {
      printf("Miss for word %i in Set %i\n", next_word, set_index);
      lru[set_index] = (lru[set_index] + 1) % BlockCount;
      sets[set_index][lru[set_index]] = next_word;
      printf("Word %i inserted into Set %i at position %i\n\n",
	     next_word, set_index, lru[set_index]);
    }
    i++;
  }
  return 0;
}
