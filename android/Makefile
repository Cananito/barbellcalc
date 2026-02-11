APK_BASENAME=barbellcalc

CFLAGS=-Wall -Wextra -Werror

BUILD_TOOLS_VERSION:=$(shell ls $(ANDROID_HOME)/build-tools | sort -n |tail -1)
PLATFORM_VERSION:=$(shell ls $(ANDROID_HOME)/platforms | sort -nr |tail -1)
NDK_VERSION:=$(shell ls $(ANDROID_HOME)/ndk | sort -nr |tail -1)

ANDROID_CLASS_PATH=$(ANDROID_HOME)/platforms/$(PLATFORM_VERSION)/android.jar
AAPT=$(ANDROID_HOME)/build-tools/$(BUILD_TOOLS_VERSION)/aapt2
D8=$(ANDROID_HOME)/build-tools/$(BUILD_TOOLS_VERSION)/d8
ZIPALIGN=$(ANDROID_HOME)/build-tools/$(BUILD_TOOLS_VERSION)/zipalign
APKSIGNER=$(ANDROID_HOME)/build-tools/$(BUILD_TOOLS_VERSION)/apksigner
ADB=$(ANDROID_HOME)/platform-tools/adb
CC=$(ANDROID_HOME)/ndk/$(NDK_VERSION)/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=aarch64-linux-android21
LD=$(ANDROID_HOME)/ndk/$(NDK_VERSION)/toolchains/llvm/prebuilt/darwin-x86_64/bin/ld
AR=$(ANDROID_HOME)/ndk/$(NDK_VERSION)/toolchains/llvm/prebuilt/darwin-x86_64/bin/llvm-ar
RANLIB=$(ANDROID_HOME)/ndk/$(NDK_VERSION)/toolchains/llvm/prebuilt/darwin-x86_64/bin/llvm-ranlib
STRIP=$(ANDROID_HOME)/ndk/$(NDK_VERSION)/toolchains/llvm/prebuilt/darwin-x86_64/bin/llvm-strip

build: gen/$(APK_BASENAME).apk

lib/arm64-v8a/libcalc.so: calc.o jni_calc.o
	mkdir -p lib/arm64-v8a
	$(CC) -shared calc.o jni_calc.o -o lib/arm64-v8a/libcalc.so

jni_calc.o: ../calc.h src/com/cananito/barbellcalc/jni_calc.c
	$(CC) $(CFLAGS) -I ../ -c src/com/cananito/barbellcalc/jni_calc.c -o jni_calc.o

calc.o: ../calc.c
	$(CC) $(CFLAGS) -c ../calc.c -o calc.o

# TODO: List inputs to avoid aggressive caching.
gen/$(APK_BASENAME).apk: lib/arm64-v8a/libcalc.so
ifndef KEYSTORE_PASSWORD
	$(error Set $KEYSTORE_PASSWORD as an env variable with the KeyStore password!)
endif
	rm -rf gen classes.dex
	mkdir -p gen/res
	mkdir -p gen/dex
	$(AAPT) compile --dir res -o gen/res/
	$(AAPT) link -I $(ANDROID_CLASS_PATH) --manifest AndroidManifest.xml --target-sdk-version 34 --java gen gen/res/*.flat -o gen/$(APK_BASENAME)-unsigned.apk
	javac -classpath $(ANDROID_CLASS_PATH) -d gen/classes src/com/cananito/barbellcalc/MainActivity.java gen/com/cananito/barbellcalc/R.java
	$(D8) --lib $(ANDROID_CLASS_PATH) --output gen/dex/classes.zip gen/classes/com/cananito/barbellcalc/*.class
	unzip gen/dex/classes.zip -d gen/dex
	zip -uj gen/$(APK_BASENAME)-unsigned.apk gen/dex/classes.dex
	zip -u gen/$(APK_BASENAME)-unsigned.apk lib/arm64-v8a/libcalc.so
	$(ZIPALIGN) -p 4 gen/$(APK_BASENAME)-unsigned.apk gen/$(APK_BASENAME)-aligned.apk
	$(APKSIGNER) sign --ks $(ANDROID_HOME)/debug.keystore --ks-pass env:KEYSTORE_PASSWORD --ks-key-alias androiddebugkey --out gen/$(APK_BASENAME).apk gen/$(APK_BASENAME)-aligned.apk

install: gen/$(APK_BASENAME).apk
	$(ADB) install -r gen/$(APK_BASENAME).apk

clean:
	rm -rf gen
	rm -rf classes.dex
	rm -rf calc.o
	rm -rf lib
