# Building

Use a Linux machine to save yourself a lot of trouble.

You will need:

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
```

3. Install the API 8 (for Android v2.2, which this project targets). To do that, first list all available packages to download in the Android SDK:

```
~/android-sdk-linux/tools/android list sdk --all
```

Gather the ID of the API 8 entry. Then run the following command with the respective ID:

```
~/android-sdk-linux/tools/android update sdk --no-ui --all --filter <ID>
```

4. Download Android SDK Build Tools and move them to the proper directory:

```
cd ~/android-sdk-linux
mkdir -p build-tools
wget https://dl-ssl.google.com/android/repository/build-tools_r24.0.3-linux.zip
unzip build-tools_r24.0.3-linux.zip -d build-tools/temp
mv build-tools/temp/android-7.0 build-tools/24.0.3
rm -rf build-tools/temp
rm build-tools_r24.0.3-linux.zip
```

5. Go to the directory of this project and build the C++ code first with the NDK (you can specify more threads for faster build) and afterwards the Java wrapper with Ant:

```
ndk-build -j {BUILDCORES}
ant clean && ant debug
```

If working properly, you should have a "bin" directory in the root of a project with a "SuperTux-debug.apk" inside.

You can install this port on devices running Android 1.5+. On newer Android it may refuse to install.
