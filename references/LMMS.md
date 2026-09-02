# To compile MelodyEditor as a native plugin for LMMS:

- Clone the source code somewhere.
- Import this plugin into LMMS's plugin repository.

```
cd lmms/plugins
ln -s /media/USER/DISK/path/to/[melody-editor-so.lmms] MelodyEditor
```

Or, copy the plugin's source code (aka melody-editor-so.lmms) into `lmms/plugins` folder.

- Enable or Add `MelodyEditor` in `cmake/modules/PluginList.cmake` file.
- Then `make` or compile the copy of your LMMS.

Optionally, you can add this project as git a subodule for lmms.
(Details not provided).

It will create `libmelodyeditor.so` file into the plugin repository.
There may be a corresponding .dll file in case of Windows.

This plugin is now accessible from the "Tools" Menu.
