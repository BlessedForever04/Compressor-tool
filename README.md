# Compressor-tool

Compressor-tool is a C-based command-line project focused on learning and implementing file compression internals.

At the moment, the fully working path is Huffman compression and Huffman decompression. The high-compression mode is reserved for future work.

## Core Idea of Compression

At its core, compression is about reducing redundancy.
Instead of storing every symbol with equal cost (8 bits), we:
Give shorter codes to frequent data, give longer codes to rare data, this minimizes total storage.

## Compression flow
[INPUT FILE]
     ↓
[READ BYTES]
     ↓
[FREQUENCY COUNT]
     ↓
[BUILD TREE]
     ↓
[GENERATE CODES]
     ↓
[ENCODE → BITSTREAM]
     ↓
[PACK → BYTES]
     ↓
[WRITE COMPRESSED FILE]

## Decompression flow
[READ HEADER]
     ↓
[REBUILD TREE]
     ↓
[UNPACK BITS]
     ↓
[TRAVERSE TREE]
     ↓
[RECONSTRUCT BYTES]
     ↓
[WRITE ORIGINAL FILE]

## Why This Project Is Useful

Many compression tools feel like a black box. This codebase is useful for newcomers because it shows the full pipeline clearly:

1. Read bytes from a file
2. Build a frequency table
3. Build a Huffman tree
4. Convert bytes to variable-length bit codes
5. Pack bits into bytes for storage
6. Rebuild and decode the original bytes

If you want to understand real compression logic (not only use a library), this is a good implementation to study.

## Current Status

- Working: Huffman compression via -c -n
- Working: Huffman decompression via -d
- Not implemented yet: high compression mode via -c -h (placeholder)

## Show help
compressor help
compressor -help

## Huffman compression (normal mode)
compressor -c -n input_file_name.extension

## High compression placeholder (future)
compressor -c -h input_file_name.extension

## Decompress a .compressed file
compressor -d some_file.compressed

## Open project GitHub URL
compressor -github

## Print README info from GitHub using curl
compressor -about

Notes:

- Compression output name is generated from the part before the first dot in the input name, then .compressed is appended.
- Decompression restores the original file name stored in the compressed header.

## Huffman Encoding Theory (Knowledge Section)

### What Problem Huffman Solves

A fixed-width byte encoding uses 8 bits for every symbol, even if some symbols are very common.

Huffman coding creates shorter bit codes for frequent symbols and longer codes for rare symbols, reducing total size on average.

### Core Idea

Huffman builds an optimal prefix-free binary code from symbol frequencies.

- Prefix-free means no code is the prefix of another code.
- Because of prefix-free property, decoding is unambiguous while scanning bits from left to right.

### Theory Steps

1. Count each symbol frequency.
2. Create one leaf node per symbol.
3. Repeatedly merge the two lowest-frequency nodes into a parent node.
4. Continue until one root remains (the Huffman tree).
5. Assign bits by traversal: left = 0, right = 1.
6. Replace each symbol by its bit code and concatenate.

## How This Project Implements Huffman

The code maps theory to specific components:

| Pipeline Stage | Where It Happens |
| --- | --- |
| CLI argument routing | src/main.c |
| Frequency counting | src/helper.h -> calculateFrequencies() |
| Store unique symbol frequencies | src/DataStructures/frequency_array.h |
| Store all original bytes | src/DataStructures/data_array.h |
| Build Huffman tree | src/helper.h -> createHuffmanTree(), src/DataStructures/tree_array.h -> buildTree() |
| Generate per-symbol code paths | src/helper.h -> createEncoding() |
| Serialize tree to file | src/helper.h -> storeTree() |
| Pack bits into bytes | src/helper.h -> bitPacking() |
| Write encoded payload | src/helper.h -> storeDataBufferInEncoding() |
| Rebuild tree at decode time | src/helper.h -> rebuildTree(), deserializeTree() |
| Decode bitstream | src/helper.h -> decode(), generateOutput() |

## Compressed File Format (Important)

The .compressed output is a custom binary container with this order:

1. nameLength (size_t)
2. originalFileName bytes (nameLength bytes)
3. compressionFlag (uint8_t)
4. treeLength (uint32_t)
5. serializedTree bytes (treeLength bytes)
6. validBitCount (uint32_t)
7. packedEncoding bytes (ceil(validBitCount / 8) bytes)

### Flag Meaning

- 0 -> Huffman
- 1 -> high-compression mode (reserved)

### Tree Serialization Rule

Preorder format:

- Internal node: write 0
- Leaf node: write 1, then write the symbol byte

Example pattern:

0 0 1 75 1 89 ...

## Decompression Flow

During decompression, the program:

1. Reads the original file name from header
2. Reads compression flag
3. If flag is 0, reads and rebuilds the Huffman tree
4. Reads validBitCount and packed encoding bytes
5. Unpacks bits and traverses tree to recover original bytes
6. Writes bytes to restored output file name

## Complexity Notes

High-level complexity for Huffman pipeline:

- Frequency pass: O(n), where n is input bytes
- Tree stage: depends on current merge/sort strategy
- Encode/decode passes: linear in produced bitstream length

Memory usage grows with:

- input size (stored bytes)
- number of unique symbols
- encoded bit buffer

## Project Layout

```text
README.md
src/
	main.c
	helper.h
	input.txt
	DataStructures/
		data_array.h
		frequency_array.h
		my_string.h
		node.h
		pair.h
		tree_array.h
```

## Known Limitations

1. High-compression mode is currently a placeholder and not production-ready.
2. Header uses native size_t and binary writes, so cross-architecture portability needs standardization (endianness and fixed-width metadata).
3. Current approach is educational and clear, but not yet tuned for large-file performance.

## Future Improvements

1. Implement true LZ77/LZMA mode for -c -h.
2. Replace repeated tree traversal per symbol with a direct code table.
3. Stream encode/decode with less intermediate buffering.
4. Standardize portable on-disk format (fixed-width fields and defined endianness).

## Contributing

Contributions are welcome. Issues and pull requests are appreciated.