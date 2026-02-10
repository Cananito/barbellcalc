APK_BASENAME=barbellcalc

BUILD_TOOLS_VERSION:=$(shell ls $(ANDROID_HOME)/build-tools | sort -n |tail -1)
PLATFORM_VERSION:=$(shell ls $(ANDROID_HOME)/platforms | sort -nr |tail -1)

ANDROID_CLASS_PATH=$(ANDROID_HOME)/platforms/$(PLATFORM_VERSION)/android.jar
AAPT=$(ANDROID_HOME)/build-tools/$(BUILD_TOOLS_VERSION)/aapt2
D8=$(ANDROID_HOME)/build-tools/$(BUILD_TOOLS_VERSION)/d8
ZIPALIGN=$(ANDROID_HOME)/build-tools/$(BUILD_TOOLS_VERSION)/zipalign
APKSIGNER=$(ANDROID_HOME)/build-tools/$(BUILD_TOOLS_VERSION)/apksigner
ADB=$(ANDROID_HOME)/platform-tools/adb

BUILD_ARTIFACTS=gen classes.dex

build: gen/$(APK_BASENAME).apk

gen/$(APK_BASENAME).apk:
	rm -rf $(BUILD_ARTIFACTS)
	mkdir -p gen/res
	mkdir -p gen/dex
	$(AAPT) compile --dir res -o gen/res/
	$(AAPT) link -I $(ANDROID_CLASS_PATH) --manifest AndroidManifest.xml --target-sdk-version 34 --java gen gen/res/*.flat -o gen/$(APK_BASENAME)-unsigned.apk
	javac -classpath $(ANDROID_CLASS_PATH) -d gen/classes src/com/cananito/barbellcalc/MainActivity.java gen/com/cananito/barbellcalc/R.java
	$(D8) --lib $(ANDROID_CLASS_PATH) --output gen/dex/classes.zip gen/classes/com/cananito/barbellcalc/*.class
	unzip gen/dex/classes.zip -d gen/dex
	zip -uj gen/$(APK_BASENAME)-unsigned.apk gen/dex/classes.dex
	$(ZIPALIGN) -p 4 gen/$(APK_BASENAME)-unsigned.apk gen/$(APK_BASENAME)-aligned.apk
	$(APKSIGNER) sign --ks $(ANDROID_HOME)/debug.keystore --ks-key-alias androiddebugkey --out gen/$(APK_BASENAME).apk gen/$(APK_BASENAME)-aligned.apk

install: gen/$(APK_BASENAME).apk
	$(ADB) install -r gen/$(APK_BASENAME).apk

clean:
	rm -rf $(BUILD_ARTIFACTS)
