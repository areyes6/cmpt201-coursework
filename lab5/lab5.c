#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct header {
  uint64_t size;
  struct header *next;
  int id;
};

void initialize_block(struct header *block, uint64_t size, struct header *next, int id) {
  block->size = size;
  block->next = next;
  block->id = id;
}

int find_first_fit(struct header *free_list_ptr, uint64_t size) {
  struct header *curr = free_list_ptr;
  while (curr != NULL) {
    if (curr->size >= size) {
      return curr->id;
    }
    curr = curr->next;
  }
  return -1;
}

int find_best_fit(struct header *free_list_ptr, uint64_t size) {
  int best_fit_id = -1;
  struct header *best = NULL;
  struct header *curr = free_list_ptr;
  while (curr != NULL) {
    if (curr->size >= size && (best == NULL || curr->size < best->size)) {
      best = curr;
    }
    curr = curr->next;
  }
  if (best != NULL) {
    best_fit_id = best->id;
  }
  return best_fit_id;
}

int find_worst_fit(struct header *free_list_ptr, uint64_t size) {
  int worst_fit_id = -1;
  struct header *worst = NULL;
  struct header *curr = free_list_ptr;
  while (curr != NULL) {
    if (curr->size >= size && (worst == NULL || curr->size > worst->size)) {
      worst = curr;
    }
    curr = curr->next;
  }
  if (worst != NULL) {
    worst_fit_id = worst->id;
  }
  return worst_fit_id;
}

int main(void) {

  struct header *free_block1 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block2 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block3 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block4 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block5 = (struct header *)malloc(sizeof(struct header));

  initialize_block(free_block1, 6, free_block2, 1);
  initialize_block(free_block2, 12, free_block3, 2);
  initialize_block(free_block3, 24, free_block4, 3);
  initialize_block(free_block4, 8, free_block5, 4);
  initialize_block(free_block5, 4, NULL, 5);

  struct header *free_list_ptr = free_block1;

  int first_fit_id = find_first_fit(free_list_ptr, 7);
  int best_fit_id = find_best_fit(free_list_ptr, 7);
  int worst_fit_id = find_worst_fit(free_list_ptr, 7);

  printf("The ID for First-Fit algorithm is: %d\n", first_fit_id);
  printf("The ID for Best-Fit algorithm is: %d\n", best_fit_id);
  printf("The ID for Worst-Fit algorithm is: %d\n", worst_fit_id);

  free(free_block1);
  free(free_block2);
  free(free_block3);
  free(free_block4);
  free(free_block5);

  return 0;
}

/*
 * Pseudo-code: coalescing a newly freed block into the free list
 *
 * COALESCE(head, z):
 *   before = NULL        // free block that ends right where z starts
 *   after = NULL         // free block that starts right where z ends
 *   after_prev = NULL    // list node pointing to `after` (needed to unlink it)
 *
 *   prev = NULL
 *   curr = head
 *   while curr != NULL:
 *     if addr(curr) + HDR + curr.size == addr(z):
 *       before = curr
 *     if addr(z) + HDR + z.size == addr(curr):
 *       after = curr
 *       after_prev = prev
 *     prev = curr
 *     curr = curr.next
 *
 *   if before == NULL and after == NULL:
 *     // Case 1: no free neighbors -> just add z to the front of the list
 *     z.next = head
 *     head = z
 *
 *   else if before != NULL and after == NULL:
 *     // Case 2: free block before z -> grow `before` to swallow z.
 *     // `before` is already in the list, so no pointers change.
 *     before.size = before.size + HDR + z.size
 *
 *   else if before == NULL and after != NULL:
 *     // Case 3: free block after z -> z swallows `after`. The merged block
 *     // now starts at z, so z takes over `after`'s position in the list.
 *     z.size = z.size + HDR + after.size
 *     z.next = after.next
 *     if after_prev == NULL:
 *       head = z
 *     else:
 *       after_prev.next = z
 *
 *   else:
 *     // Case 4: free blocks on both sides -> `before` swallows z and `after`,
 *     // then `after` is unlinked from the list.
 *     before.size = before.size + HDR + z.size + HDR + after.size
 *     if after_prev == NULL:
 *       head = after.next
 *     else:
 *       after_prev.next = after.next
 *
 *   return head
 *
 *
 * Example: memory layout  a | b | c | d | m | z | n | f | g | h
 *   Free blocks: b, m, n.  Free list: head -> n -> b -> m -> NULL
 *   Block z (red) is freed.
 *
 *   Scan:
 *     curr = n: addr(z) + HDR + z.size == addr(n)  -> after = n, after_prev = NULL
 *     curr = b: not adjacent to z
 *     curr = m: addr(m) + HDR + m.size == addr(z)  -> before = m
 *
 *   before = m and after = n, so this is Case 4:
 *     m.size = m.size + HDR + z.size + HDR + n.size   (m now spans m, z, n)
 *     after_prev == NULL (n was the head), so head = n.next = b
 *
 *   Result: memory  a | b | c | d | [ m + z + n ] | f | g | h
 *           free list: head -> b -> m -> NULL
 */
