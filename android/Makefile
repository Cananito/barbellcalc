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

install: gen/$(APK_BASENAME).apk
	$(ADB) install -r gen/$(APK_BASENAME).apk

gen/$(APK_BASENAME).apk: gen/$(APK_BASENAME)-aligned.apk
ifndef KEYSTORE_PASSWORD
	$(error Set $KEYSTORE_PASSWORD as an env variable with the KeyStore password!)
endif
	$(APKSIGNER) sign --ks $(ANDROID_HOME)/debug.keystore --ks-pass env:KEYSTORE_PASSWORD --ks-key-alias androiddebugkey --out gen/$(APK_BASENAME).apk gen/$(APK_BASENAME)-aligned.apk

gen/$(APK_BASENAME)-aligned.apk: gen/$(APK_BASENAME)-unsigned.apk
	$(ZIPALIGN) -p 4 gen/$(APK_BASENAME)-unsigned.apk gen/$(APK_BASENAME)-aligned.apk

# Depending on .zip for dex file since d8's generated .dex file's timestamp is
# always Dec 31 1969, always causing a re-build.
gen/$(APK_BASENAME)-unsigned.apk: gen/$(APK_BASENAME)-unsigned-nocode.apk gen/dex/classes.zip lib/arm64-v8a/libcalc.so
	cp gen/$(APK_BASENAME)-unsigned-nocode.apk gen/$(APK_BASENAME)-unsigned.apk
	unzip gen/dex/classes.zip -d gen/dex
	zip -uj gen/$(APK_BASENAME)-unsigned.apk gen/dex/classes.dex
	zip -u gen/$(APK_BASENAME)-unsigned.apk lib/arm64-v8a/libcalc.so

gen/com/cananito/barbellcalc/R.java gen/$(APK_BASENAME)-unsigned-nocode.apk: gen/res/values_strings.arsc.flat gen/res/values_styles.arsc.flat
	$(AAPT) link -o gen/$(APK_BASENAME)-unsigned-nocode.apk -I $(ANDROID_CLASS_PATH) --manifest AndroidManifest.xml --target-sdk-version 34 --java gen gen/res/values_strings.arsc.flat gen/res/values_styles.arsc.flat

gen/res/values_strings.arsc.flat gen/res/values_styles.arsc.flat: res/values/strings.xml res/values/styles.xml
	mkdir -p gen/res/
	$(AAPT) compile --dir res -o gen/res/

gen/dex/classes.zip: gen/classes/com/cananito/barbellcalc/MainActivity.class gen/classes/com/cananito/barbellcalc/MainActivity$$1.class gen/classes/com/cananito/barbellcalc/MainActivity$$2.class gen/classes/com/cananito/barbellcalc/R.class gen/classes/com/cananito/barbellcalc/R$$string.class gen/classes/com/cananito/barbellcalc/R$$style.class
	mkdir -p gen/dex
	$(D8) --lib $(ANDROID_CLASS_PATH) --output gen/dex/classes.zip gen/classes/com/cananito/barbellcalc/*.class

gen/classes/com/cananito/barbellcalc/MainActivity.class gen/classes/com/cananito/barbellcalc/MainActivity$$1.class gen/classes/com/cananito/barbellcalc/MainActivity$$2.class gen/classes/com/cananito/barbellcalc/R.class gen/classes/com/cananito/barbellcalc/R$$string.class gen/classes/com/cananito/barbellcalc/R$$style.class: src/MainActivity.java gen/com/cananito/barbellcalc/R.java
	javac -classpath $(ANDROID_CLASS_PATH) -d gen/classes src/MainActivity.java gen/com/cananito/barbellcalc/R.java

# TODO: Support all architectures!
lib/arm64-v8a/libcalc.so: calc.o jni_calc.o
	mkdir -p lib/arm64-v8a
	$(CC) -shared calc.o jni_calc.o -o lib/arm64-v8a/libcalc.so

jni_calc.o: ../calc.h src/jni_calc.c
	$(CC) $(CFLAGS) -I ../ -c src/jni_calc.c -o jni_calc.o

calc.o: ../calc.c
	$(CC) $(CFLAGS) -c ../calc.c -o calc.o

clean:
	rm -rf gen
	rm -rf calc.o
	rm -rf jni_calc.o
	rm -rf lib
