# Custom C Preprocessor (`mypreprocessor`)

A multi-pass C preprocessor written in modular C that transforms a `.c` source file into a preprocessed `.i` file.

## Features

1. **Comment Removal (`comment_removal.c`)**
   - Strips single-line (`// ...`) and multi-line (`/* ... */`) comments while preserving string literals (`"..."`).
2. **File Inclusion (`file_inclusions.c`)**
   - Expands local header files (`#include "filename.h"`) and standard header files (`#include <filename.h>`).
3. **Macro Replacement (`macro_handler.c`)**
   - Expands object-like macros (`#define NAME VALUE`) and function-like macros (`#define NAME(x) (...)`) without modifying text inside string literals.

## Project Structure

```text
.
├── main.c               # Entry point and 3-stage pipeline controller
├── header.h             # Shared function declarations
├── comment_removal.c    # Stage 1: Comment stripping
├── file_inclusions.c    # Stage 2: #include expansion
├── macro_handler.c      # Stage 3: #define and macro substitution
├── makefile             # Build automation rules
├── test.c               # Sample C source file for testing
└── sample.h             # Sample header file included by test.c for testing

```
