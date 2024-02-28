# Searches byte sequence from specified directory

## Overview
This is a command-line program written in C to search for a byte sequence in files within a specified directory. The program can search for a byte sequence represented in hexadecimal format in files and display the files containing that byte sequence.

## Usage
### Requirements
- Linux operating system
- C compiler (gcc)
- System libraries: `sys/types.h`, `sys/stat.h`, `fts.h`, `stdio.h`, `string.h`, `stdlib.h`, `errno.h`, `stdbool.h`, `getopt.h`

### Installation
1. Copy the source code from GitHub to your computer.
2. Compile the program by running the following command in the directory containing the source code: ```make```

### Running
Use the following command to run the program:

```./hex_finder [directory] [target]```

Where:
- `[directory]` is the path to the directory from which the search begins.
- `[target]` is the byte sequence (in hexadecimal representation) to be searched for in the files.

### Options
The program supports the following options:
- `-h, --help`: Display usage instructions for the program.
- `-v, --version`: Display information about the program version.



