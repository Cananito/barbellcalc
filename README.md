# barbellcalc

CLI, webapp, and Android app for barbell weight and plate calculation.

## Dev Set Up

## CLI

Requirements:

1. A C compiler
1. Make

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

Requirements:

1. A C compiler that supports WASM output (Android's NDK supports it)
1. Make
1. Something to setup a local server to serve the webapp

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

Requirements:

1. Android's [cmdline-tools](https://developer.android.com/studio#command-line-tools-only)
1. JDK
1. Make

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
