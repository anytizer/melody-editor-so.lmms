# Significant Changes (Monthly)

## 2026-06

- Documentation retouch.
- Help text and F1 key introduced.

## 2026-02

- Documents placed together in references.
- Bhatkhande like support has been added by @allejok96.
  - manually assigned keys
  - repeatitions of notations
  - macros
  - glides

## 2026-01

 - Converted/Created simple parsers to use different dialects.
 - Ctrl + / to toggle a comment with # sign.
 - Auto close a chord boundary: [] {} () "" in the editor
 - Format option added, hiding bar lines.
 - Zoom slider control added.
 - Line numbers implemented.
 - Default true for Live code.
 - Only support the ascii characters, else discarded for processing.
 - Colored selection for partial clip parsing.
 - Using Note.h for directly putting notes into MIDI Clip, without xml/xpt. 
 - Parsers mentioned in example notations.
 - Live coding enable/disable with a button.
 - Namespace simplified | __needs to reserve__.
 - User selected text preferred first, instead of parsing whole text.
 - Placeholder icons added.
 - Activated a most of the parsers with empty functionalities.
 - ALDA parser added from @allejok96's efforts.
 - Started to study [ALDA](https://alda.io/).
 - Started to study [ABC notation](https://abcnotation.com/) system.
 - Using __const_cast__ instead of modifying LMMS's code, to pull piano roll in editor mode.
 - Temporarily removed incomplete parsers.
 - Completely removed missing and none parsers.

## 2025-12

 - (c) Year bump up.
 - White logos.
 - Replacements dataset centralized in AbstractParser.
 - Progress log now on monthly basis.
 - Better variable names.
 - Enhance documentation, and links.
 - Screenshot added [here](src/assets/screenshot.png).
 - Icons for identification of the notation system.
 - Clearly point which files to modify to compile it as an LMMS Plugin.
 - A notification is added to open a Piano-Roll first; when __Update MIDI Clip__ applied to an empty target.
 - Changed binary name to more specific name libmelodyeditor.so from libmelody.so
 - Setting stay-on-top window flag by default.
 - Smoother text zoom; factor realculations
 - Applied scroll bar with text zoom.
 - Removed unnecessary UI elements that were originally intended to write to external files.

## 2025-09

 - Using custom text editor for zooming text with mouse wheel and ctrl key
 - Open a notation file with double click on the editor
 - comment a line ctrl+slash
 - ctrl+enter to update the midi clip instantly
 - Plugin compiled back in Qt 5 as well. However, Qt6 preferred.
 - Updated the documentations.

## 2025-07

 - Disabled incomplete parsers. Rework and Help required.
 - Decopupling MelodyEditor and MelodyEditorView. Remove friend functions.
 - Implemented QFileSystemWatcher when an external file was dropped in.
 - Support drag/drop of external notation file in .txt format
 - Added the list of keys allowed in specific system.
 - Some sample notations added for testing purpose.
 - Example for testing English notes added.

## 2025-06

 - Early version of Carnatic parser added.
 - Individualized parsers on their own files.
 - Project started as a decopupled code from previous efforts.
 - `class` to `struct` for Cell, and FindAndReplace
 - Improve and fix typos in comments.
 - Bring .h file contents together on top of the code.
 - Bit length controller in pixels - comments changed for Cell.
 - fr (Finds and Replaces) changed to replaces.
 - Removed some completed todos.
