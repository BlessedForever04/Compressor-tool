# Compressor-tool

A command-line utility for compressing and decompressing files using Huffman and LZMA algorithms. This tool provides both normal (Huffman) and fast (LZMA) compression modes, as well as decompression support.

## Features

- **Normal Compression:** Uses the Huffman algorithm for efficient file size reduction.
- **Fast Compression:** Uses the LZMA algorithm for high-speed compression.
- **Decompression:** Supports decompressing files compressed by this tool.
- **Cross-platform:** Written in C for portability.
- **Simple CLI interface:** Easy-to-use command-line options.

## Usage

```sh
# Fast file compression (LZMA)
./compressor -c -f < input_file > output_file

# Normal file compression (Huffman)
./compressor -c -n < input_file > output_file

# Decompress file
./compressor -d < input_file > output_file

# Show help
./compressor help

# About
./compressor -about

# Open GitHub repo
./compressor -github
```

## Directory Structure

- `src/` — Source code (C files and headers)
- `compressed_example/` — Example compressed files
- `decompressed_example/` — Example decompressed files

## Building

To build the project, use a C compiler (e.g., gcc):

```sh
gcc src/main.c src/tree.c -o compressor
```

## Contributing

Contributions are welcome! Please open issues or pull requests on the [GitHub repository](https://github.com/BlessedForever04/Compressor-tool).

## License

MIT License
