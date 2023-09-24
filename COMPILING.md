# Building a DOS version of Russian Doom

## Step 1: Setting up build environment

The primary IDE for building Russian Doom is Open Watcom C,
which may be downloaded on its [official site](http://www.openwatcom.org/download.php).

## Step 2: Compiling project

After installation of Open Watcom C, open its IDE and then choose menu "File" > "Open Project..."  
Choose `rusdoom.wpj` and press 'Make target' button.

At the end of the compilation process, you will find compiled binary `rusdoom.exe` in the source code directory.

Please note: to run the compiled game executable, you will need to copy
`rusdoom.exe`, `rusdoom.wad` and `dos4gw.exe` files to your Doom directory, as well as official IWAD files.
