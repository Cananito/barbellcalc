# barbellcalc

CLI, webapp, and Android app for barbell weight and plate calculation.

## Dev Set Up

Requirements:

1. A C compiler that supports WASM output
1. Make
1. Python3 (for local serving of the web app)

## CLI

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

## Webapp

To build the web app's WASM file:

```
$ make build-wasm
```

In my case, since my default Clang doesn't support WASM ouput, I do:

```
$ make CC=/Users/rogeliogudino/android/ndk/29.0.14206865/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang build-wasm
```

Start a local server to serve the webapp directory (`nginx -p .` or `python3 -m http.server 9999` e.g.).

## Android

Pre-requisite:

```
$ cd android/
```

To build the Android app:

```
$ make build
```

To deploy the Android app to a local emulator or device:

```
$ make install
```

To delete the Android app compilation artifacts:

```
$ make clean
```
