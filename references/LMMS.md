# To compile as a native plugin for LMMS

1. Clone the source code somewhere.
2. Import this plugin into LMMS's plugin repository.

```
cd lmms/plugins
ln -s /media/USER/DISK/path/to/[melody-editor-so.lmms] MelodyEditor
```

Or, copy the plugin's source code (aka melody-editor-so.lmms) into __lmms/plugins__ folder.

3. Enable or Add `MelodyEditor` in `cmake/modules/PluginList.cmake` file.
4. Then `make` or compile the copy of your LMMS.

It will create `libmelodyeditor.so` file into the plugin repository.
There may be a corresponding .dll file in case of Windows.
This plugin is now accessible from the "Tools" Menu.