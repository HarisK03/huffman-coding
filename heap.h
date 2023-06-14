void _swap(HuffmanTree *a, HuffmanTree *b);

void _heapify(HuffmanTreeDict *heap, int index);

void build_min_heap(HuffmanTreeDict *huffman_tree_dict);

void insert(HuffmanTreeDict *heap, HuffmanTree entry);

HuffmanTree *extract_min(HuffmanTreeDict *heap);
