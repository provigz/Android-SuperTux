# Building

Use a Linux machine to save yourself a lot of trouble.

You will need:

* **JDK (Java Development Kit) 8**
* [**Crystax Android NDK v4**](https://www.crystax.net/android/ndk/4) - for building the C++ side of the app.
* **Legacy Android SDK (+ Build Tools)** (we will use r24.4.1 here) - for Android APIs.
* **Apache Ant** - for building the Java wrapper. Install through package manager.

1. Download the Crystax NDK, extract it and append its directory to the `PATH` environment variable:

```
export PATH=$PATH:pathtondk
```

2. Download the Android SDK to the home directory:

```
cd ~
wget https://dl.google.com/android/android-sdk_r24.4.1-linux.tgz
tar -xf android-sdk_r24.4.1-linux.tgz
del android-sdk_r24.4.1-linux.tgz
```

3. Install the API 8 (for Android v2.2, which this project targets):

```
~/android-sdk-linux/tools/android update sdk --no-ui --all --filter android-8
```

4. Download Android SDK Build Tools, extract them and move them to the proper directory:

```
cd ~/android-sdk-linux
mkdir -p build-tools
wget https://dl-ssl.google.com/android/repository/build-tools_r24.0.3-linux.zip --quiet
unzip -q build-tools_r24.0.3-linux.zip -d build-tools/temp
SRC_DIR=$(ls -d build-tools/temp/* | head -n 1)
mv "$SRC_DIR" build-tools/24.0.3
rm -rf build-tools/temp build-tools_r24.0.3-linux.zip
```

5. Go to the directory of this project and build the C++ code first with the NDK and afterwards the Java wrapper with Ant:

```
ndk-build -j$(nproc)
ant clean && ant debug
```

If working properly, you should have a "bin" directory in the root of a project with a "SuperTux-debug.apk" inside.

You can install this port on devices running Android 1.5+. On newer Android it may refuse to install.
