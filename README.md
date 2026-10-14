# plus

A Cargo for C++. Or the start of one: today it finds your `.cpp` files,
compiles them and runs the result. Cargo had to start somewhere too.

## Why

I'd write a lot more C++ if C++ had a Cargo. It doesn't, so here we are.

## Usage

Put your code in `src/`:

```
hello/
└── src/
    └── main.cpp
```

From `hello/`:

```bash
plus run
```

Output:
```
Compiling project: hello
Build successful: target/debug/hello
hello from plus
```

The executable is named after the folder and lands in `target/debug/`, like Cargo.

| Command      | What it does                                         |
|--------------|------------------------------------------------------|
| `plus build` | compile every `.cpp` in `src/` into one executable   |
| `plus run`   | build, then run it                                   |
| `plus help`  | list the commands                                    |

## Build

You need `clang++` with C++20. plus builds itself, but someone has to build it
the first time:

```bash
clang++ -std=c++20 src/main.cpp -o plus
./plus build
```

That second line is plus compiling plus. From then on, use `target/debug/plus`.

## Notes

- Every build is a debug build with AddressSanitizer and UndefinedBehaviorSanitizer
  (`-std=c++20 -Wall -Wextra -fsanitize=address,undefined -g`). Your program runs
  slower and complains more. That's the point. Release builds come later.
- Only `src/` itself is scanned, not its subfolders.
- No incremental builds yet: every build recompiles everything. Fine for a hello
  world. For LLVM, bring a book.
- If your program exits with an error, plus says `Running error` and exits with 1
  instead of your program's code. For now.
- Tested on macOS (Apple Silicon) only.

## License

Apache-2.0. See [LICENSE](LICENSE).
