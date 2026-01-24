# ToDos for MelodyEditor

## Implement ALDA Parser

<!--
```
cd alda-lang
chmod +x alda
cd alda
scripts/install-fluidr3
alda start
alda play --file examples/something.alda
alda play --code "piano: f e d c"
alda stop
```
-->

## UI

- Ctrl+UP/DOWN to control LMMS Host Playing.
- ~~Latchable push button for enabling live-write mode.~~

## Others

- Provide an example of copyrights free notation for all parsers registered.
- Apply input/output filters for .abc, .txt, .lrc, .alda (while .lrc is out of scope) in LMMS way.
- Update drop zone accept these file types immediately. Replace openNotationsFileSelector().
- Apply note lengths differences within chords and strums.
- Try with `#pragma once` and remove current long guard names.
- Utilities could be reused from LMMS (open a file, read its contents).
- Continuous / live writing mode - Update MIDI as soon as a **valid** keypress is found.
- ~~Discuss status for `MidiClip* currentMidiClip() ()` to remove `const` portion.~~
- ~~Placeholder text effect is not experienced.~~
- ~~Notify the need of Qt6 requirements to the developers.~~ compiles in Qt5 too.
- Once notations file is loaded, the scrollbar does not behave better, even when set to appear always.
- Change namespace from: lmms::melodyeditor to: lmms::plugins::melodyeditor
- Wheel movement to control scrolling plus text zoom behaviour.
- Apply F11 as widget's fullscreen to enter and exit.
- Text mark as "incomplete" parsers like German and Numered to inform other developers.
- Use tr() translate texts.
- Replace all icons with proper identificatons, eg. ns-abc.
- Control completed status and appearance of parser with is_completed flag. This allows registering all available parsers.

## Discuss

- Reserve a namespace for this plugin, with the community polls.

## Difficult/Overlapping notes:
- Arpeggio
- Chords
- Strum
- Arbitrarily overlapping notes based on individual timings: 1, 2, 4, 8, 16, 32, ...

- ~~Support for multiple instruments with same set of notations~~
  - ~~Change notations in different scales or octaves~~ no need.
  - Response to base note change. (managed!)
  - Remember octave change

- ~~Use standard mono-spaced, larger font across all OS'es | Consolas?~~
- ~~File system watcher for a dropped in file, if it is edited externally: it updates here, silently.~~
- If using external file, in watching mode, operate the editor in readonly mode.
	- Show the currently watched notations file in the title bar.
	- If the file is removed or editor disconnected; stay calm.
	- Silently update audio/notations when file is changed externally.
	- Test with various editors and IDEs, if file system watcher works properly.
		- eg. geany, featherpad
		- eg. Visual Studio Code
		- eg. Notepad++

## Currently out-of-scope

- Make notations formatter for printer friendly.
  - Visually align notations on A4/Letter size.
  - Cross system notations conversion, eg. SARGAM to English.
- Implement shortcuts
	- CTRL+L, CTRL+/ to comment out the selected lines
	- CTRL+ENTER update the MIDI Clip
- Optionally, extract time signature and BPM from file name if available, and update.
    - x-y-z where:
	  - x is numerator,
	  - y is denominator,
	  - z is bpm.
    - eg. notations-3-4-180.txt
	- eg. notations.txt
- Enforce that the dropped file name to be notations.txt or notations-x-y-z.txt
- Introduce file type extension: .melody, .sargam, .notation, .txt | poll
- STo save the melody file to an external path.

## Done

- Obtain white icons for MeldoyEditor.
- Apply `#define` guards to all .cpp and .h files.
- Warn when a too big notation file is dropped, typed in or pasted.
	- prevent LMMS to crash due to this.
	- Prevent CPU loads
- ~~Remind to open PianoRoll when user just hits Update Melody.~~
- ~~Produce documention on how to write the melody for this plugin.~~
- ~~Produce standalone version; needs testing.~~
- ~~MIDI full length is lesser than needed, shadowing some notes at the end.~~
- ~~When midi clip window is closed, it does not connect to a new window opened again.~~
- ~~Warn the user if notation contains invalid notes.~~
- ~~Make parsers[0] icon as application icon. Remove other files.~~
- Open source the repository when the tests are satisfactory.
