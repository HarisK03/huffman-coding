#include <stdlib.h>
#include "huffman.h"
#include "heap.h"

/**
 * Swap two Huffman trees in the heap.
 */
void _swap(HuffmanTree *a, HuffmanTree *b)
{
    HuffmanTree temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * Heapify the heap.
 */
void _heapify(HuffmanTreeDict *heap, int index)
{
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < heap->size && heap->entries[left].frequency < heap->entries[smallest].frequency)
        smallest = left;

    if (right < heap->size && heap->entries[right].frequency < heap->entries[smallest].frequency)
        smallest = right;

    if (smallest != index)
    {
        _swap(&heap->entries[index], &heap->entries[smallest]);
        _heapify(heap, smallest);
    }
}

/**
 * Build a min-heap from a Huffman tree dictionary.
 */
void build_min_heap(HuffmanTreeDict *huffman_tree_dict)
{
    for (int i = (huffman_tree_dict->size / 2) - 1; i >= 0; i--)
    {
        _heapify(huffman_tree_dict, i);
    }

    // remove 0 frequency entries
    while (huffman_tree_dict->entries[0].frequency == 0)
    {
        extract_min(huffman_tree_dict);
    }

    // handle the 1 entry tree
    if (huffman_tree_dict->size == 1)
    {
        HuffmanTree dummy_huffman_tree;
        dummy_huffman_tree.symbol = (huffman_tree_dict->entries[0].symbol + 1) % 256;
        dummy_huffman_tree.frequency = 0;
        dummy_huffman_tree.number = -1;
        dummy_huffman_tree.left = NULL;
        dummy_huffman_tree.right = NULL;

        insert(huffman_tree_dict, dummy_huffman_tree);
    }
}

/**
 * Inseart a Huffman tree entry into the heap.
 */
void insert(HuffmanTreeDict *heap, HuffmanTree entry)
{
    int i = heap->size;
    heap->entries[i] = entry;

    while (i != 0 && heap->entries[i].frequency < heap->entries[(i - 1) / 2].frequency)
    {
        _swap(&heap->entries[i], &heap->entries[(i - 1) / 2]);
        i = (i - 1) / 2;
    }

    heap->size++;
}

/**
 * Extract and return the minimum element from the min-heap (root).
 */
HuffmanTree *extract_min(HuffmanTreeDict *heap)
{
    HuffmanTree *min = (HuffmanTree *)malloc(sizeof(HuffmanTree));
    if (min == NULL)
    {
        perror("Malloc error.");
        return NULL;
    }
    *min = heap->entries[0];
    heap->entries[0] = heap->entries[heap->size - 1];
    heap->size--;

    _heapify(heap, 0);

    return min;
}
