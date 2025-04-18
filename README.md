# barbellcalc
Webapp and CLI for barbell weight and plate calculation.

## Dev Set Up

Requirements:

1. A C compiler that supports WASM output
1. Make
1. Python3

## Building and Running

To build the CLI:

```
$ make build
```

To run the CLI:

```
$ make run
```

To delete the CLI compilation artifacts:

```
$ make clean
```

To build the web app's WASM file:

```
$ make build-wasm
```

In my case, since my default Clang doesn't support WASM ouput, I do:

```
$ make CC=/Users/rogeliogudino/repos/llvm-project/build/bin/clang build-wasm
```

To start the web app server:

```
$ make server
```

To run the web app, open http://localhost:9999 in your web browser.
