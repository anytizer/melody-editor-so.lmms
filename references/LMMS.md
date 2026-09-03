# Compiling MelodyEditor as a native plugin for LMMS:

- Clone the source code somewhere.
- Download ABC-Parser as dependency.
- Import this plugin into LMMS's plugin repository.
- Notify your cmake.

## Step 1

```
git clone https://github.com/anytizer/melody-editor-so.lmms.git

cd melody-editor-so.lmms
git submodule update --init

cd [LMMS]/plugins
ln -s /media/USER/DISK/path/to/[melody-editor-so.lmms] MelodyEditor
```

Or, copy the plugin's source code (aka melody-editor-so.lmms) into `lmms/plugins` folder, and rename the folder to "MelodyEditor", case-sensitive.

Optionally, you can add this project as git a subodule for lmms.
(Details not provided).

## Step 2

- Enable or Add `MelodyEditor` in [cmake/modules/PluginList.cmake](https://github.com/LMMS/lmms/blob/master/cmake/modules/PluginList.cmake) file.
- Then `make -j$(nproc)` or compile the copy of your LMMS normally.

It will create a new shared object: `libmelodyeditor.so` file into the plugin repository.
There may be a corresponding .dll file in case of Windows.

## Step 3

This plugin is now accessible from the LMMS's "Tools" Menu.
