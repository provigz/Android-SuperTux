APP_PROJECT_PATH := $(call my-dir)/..

# Available libraries: mad sdl_mixer sdl_image sdl_ttf sdl_net sdl_blitpool
# sdl_mixer depends on tremor and optionally mad
# sdl_image depends on png and jpeg
# sdl_ttf depends on freetype

APP_MODULES := application sdl sdl_main sdl_image sdl_mixer tremor png jpeg mikmod iconv

APP_ABI := armeabi

APP_PLATFORM := android-8
