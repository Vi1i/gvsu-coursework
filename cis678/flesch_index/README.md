# Flesch Index
A application calculating the Flesch Index in C++ using simple measures.

The Flesch Index (FI), a numerical measure of the readability of English text. Originally invented for evaluating the difficulty of reading U.S. Army training manuals, it has since become ubiquitous.

Earlier on I said this application used simple measures in the Flesch Index calculation, this was pointed out for several reasons:
1. Syllables
2. Sentences
3. Words

As an native English speaker, these reasons seem very base, except the more I personally looked into them, the more I realized the difficulty of a machine knowing phonemes, sentence break downs, and even words from a sentence.

## Requirements:
* CMake >= 3.1
* C++11

## How To Use:
Once the repo has been pulled down, follow these steps to compile and run:

```bash
cd <RepoLocation>
cd build
cmake ..
make
bin/flesch-index ../data/GettysburgAddress.txt
```
Line number:

1. Moves into the root of the repo so that other command can be relative.
2. This sets up and out of program build for the next lines.
  * This allows the build file generated to not funk up the repo.
3. Calls the `cmake` to on `../CMakeLists.txt` to generate build files.
4. This starts the compile on the generated build structure
5. This is an example use of the application with a sample data.


## [Results](results/RESULTS.md)
