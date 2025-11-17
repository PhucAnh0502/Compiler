# Assignment 1 - Text Indexer

This small C project builds an index of words from a text file while ignoring stop words. The program is provided as `main.c` and the Makefile builds an executable named `nau`.

## What it does
- Loads stop words from `test/stop_words.txt` (or `stopw.txt` if you have that file)
- Processes the text file `test/vanban.txt` (the code currently uses fixed paths)
- Builds an index mapping each (non-stop) word to the list of line numbers where it appears
- Prints the sorted index to stdout

## Prerequisites
- A C compiler (GCC or clang)
- `make` (optional but recommended)

On Windows you can use MinGW or WSL. On Linux/macOS use the system toolchain.

## Build
From the project directory (`d:/OneDrive/Documents/Chương trình dịch/Assignment/Assignment1`) run:

```powershell
make
```

or directly with `gcc`:

```powershell
gcc -std=c11 -O2 -Wall -Wextra -o nau main.c
```

## Run

The program expects the stop words file at `test/stop_words.txt`. If you only have `stopw.txt` (included with the assignment), the Makefile or the `run` target will copy it to the expected filename.

To run the program after building:

```powershell
make run
# or
./nau
```

On Windows PowerShell you may need to prefix with `.
`:

```powershell
.\nau
```

## Test files
- `test/vanban.txt` — sample input text used by `main.c`.
- `test/stopw.txt` — alternative stop-words file (Makefile will copy it to `test/stop_words.txt` if needed).
- Other files provided in the folder can be used as additional inputs.

## Output
The program prints a sorted index to stdout. Each line contains a word (left aligned) and a comma-separated list of line numbers where the word appears.

Example output format:

```
word                 1, 3, 10
another              2, 7
```

## Changing input files
Currently `main.c` reads fixed paths (`test/stop_words.txt` and `test/vanban.txt`). If you want to pass filenames on the command line I can update `main.c` to accept arguments (recommended) and update the Makefile accordingly. Ask me to "Make the program accept input filenames" and I'll implement it.

## Clean

```powershell
make clean
```

## Notes
- The code includes logic to ignore capitalized words at the start of sentences and to avoid adding the same line number twice for repeated words on the same line.
- If you plan to run on Windows without WSL, make sure `make` and `gcc` are available (e.g., via MinGW/MSYS2 or install WSL and run inside Linux environment).

If you want, I can also add a `run` target that accepts a filename variable (e.g. `make run FILE=test/alice30.txt`). Would you like that? 

