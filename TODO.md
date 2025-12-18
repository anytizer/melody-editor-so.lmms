# ToDos for MelodyEditor

- Placeholder text effect is not experienced.
- Notify the need of Qt6 requirements to the developers.
- Once notations file is loaded, the scrollbar does not behave better.

Difficult/Overlapping notes:
- Arp
- Chords
- Strum
- Arbitrarily overlapping notes

- ~~Support for multiple instruments with same set of notations~~
  - ~~Change notations in different scales or octaves~~ no need.
  - Response to base note change. (managed!)

- ~~Use standard mono-spaced, larger font across all OS'es | Consolas?~~
- ~~File system watcher for a dropped in file, if it is edited externally: it updates here, silently.~~
- If using external file, operate the editor in readonly mode.
	- Use a file system watcher.
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

- Open source the repository when the tests are satisfactory.
- Reserve the namespaces with the community polls.
