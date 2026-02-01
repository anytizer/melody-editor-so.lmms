# melody-editor-so.lmms

```
Creates a plugin: libmelodyeditor.so - for LMMS under Qt6.
```

A text based melody parser plugin for [LMMS](https://github.com/LMMS/lmms);
feeding Alda, SARGAM and English Notations.
It is asscessible from `Tools > Melody Editor`.

This plguin allows a user to type in or drop in song notations file into an editor area.
These notations are then parsed to create a MIDI Clip Data within the Piano-Roll window.

<!-- ~~An intermediary XML string is created on the fly that simulates Import MIDI Clip feature; as if you are working with .xpt file.~~ Direct pasting into Piano Roll. -->

# Features

- Ctrl + / toggles a comment in block of selections.
- Ctrl + Wheel to zoom in/out notations font size.
- Double click on text area to choose a notation file.
- Drag and Drop of a notation file into the melody editor area.
- Extremely powerful parsers
  - English and SARGAM notations
  - A little memory usage while operating.
  - Fast, light weight, and small footprint.
  - 20kb limits applied on input size.
  - Does not obstruct live playing in the Piano Roll.
  - Stable

~~Multiple parsers in progress (Carnatic, English, ABC, ALDA and some more).~~

# Quick Manual

 - Open song editor.
 - Double click on it to open a piano roll window.
 - Go to: Tools > Melody Editor.
 - Fill in the notations
 - Choose your proper parser.
 - Click on Update MIDI Clip.

 Thats it!

## Next Steps

 - Learn about how to write notations [here](https://github.com/anytizer/melody-editor.so.lmms) for this plugin.
 - Also in LMMS Discussion page [here](https://github.com/LMMS/lmms/discussions/7950).

# Compile as a native plugin for LMMS

1. Clone the source code somewhere.
2. Import this plugin into LMMS's plugin repository.

```
cd lmms/plugins
ln -s /media/USER/DISK/path/to/[melody-editor-so.lmms] MelodyEditor
```

Or, copy the plugin's source code (aka melody-editor-so.lmms) into __lmms/plugins__ folder.

3. Enable/Add `MelodyEditor` in `cmake/modules/PluginList.cmake` file.
4. Then `make` or compile the copy of your LMMS.

It will create `libmelodyeditor.so` file into the plugin repository.
There may be a corresponding .dll file in case of Windows.
This plugin is now accessible from the Tools Menu;

# Documentation

You need to format your notations into human readable form with the following considerations.

Features              | Western notes Parser           | SARGAM notes Parser          | Virtual Keyboard Parser
----------------------|--------------------------------|------------------------------|-------------------------
Natural notes         | `C D E F G A B` | `S R G P D N`, `m` | Alpha-Numerals (QWERTY) keyboard
Shaprs/Flat notes     | `C# D# F# G# A#` | `r g d n`, `M`    | Alpha-Numerals (QWERTY) keyboard
Higher octaves marker | Append octave number eg. `C5 D6` | Append an asterisk `*` eg. `S*, R**` | ??
Lower octaves marker  | Append octave number eg. `C3 D2` | Append a dot `.` eg. `S., R..` | ??
Chord Making `*`      | Brace notes within `[` and `]` | Brace notes within `[` and `]` | Brace notes within `[` and `]`
Longer chord lengths  | Add a dash `-` BEFORE `]`.     | Add a dash `-` before `]`.     | Add a dash `-` before `]`.
Time shared / split notes | Comma eg. `C#,D#`          | Comma eg. `r,g`                | ??
Silence / Pause marker| Use `x` wherever needed.       | Use `x` wherever needed.       | `SPACE` character
Longer silence marker | Introduce another `x`.         | Introduce another `x`.         | Introduce a pipe `|`.
Continuation mark     | Add a dash `-`                 | Add a dash `-`                 | ??
Comments per line     | Start a line with hash `#`.    | Start a line with hash `#`.    | ??
Sharps / Flats        | Only sharps understood. ~~flats~~ | Lower case for komal notes. | ??
Example 1 - chords    | `[ C E G  - - - ] A A,B A G`   | `[ S R G - - - ] D D,N D P`    | [qwe]tyu-iop
Example 2 - long note | `G3 A3 E4 D4 - G4,A4 B4 D5 C5 - - -` | `P. D. G R - P,D N R* S* - - -`  | ??
Example 3 - silence / pause | `C x D x E x G -`  | `S x R x G x P -` | ??
Help Link / Discuss   | [here](https://github.com/LMMS/lmms/discussions/7950) | [here](https://github.com/LMMS/lmms/discussions/7950) | [here](https://github.com/LMMS/lmms/discussions/7950)

`*`: Feature is in test mode

# Trouble-Shooting

* Chords must have been terminated with corresponding `]`.
* __Error Count__: Number of notes that were NOT converted for use in LMMS is returned. __Remedies__:
  - Look for accidentally joined notations.
  - Or illegal notes or characters like: __E#__, __J__.

# Testing Examples

* Set signature: 3/4, 180 bpm.
* Set active midi clip window.
* Open LMMS Menu > Tools > Melody Editor.
* Paste one of the notes from below, in the input area.
* Select the correct notation type or parsing engine.
* Hit "Update MIDI Clip".
* Now, play the song ;-)

## Example: Happy Birthday song

- Courtesy: AI search results and human edits.
- Details [here](examples/).

### Example 1: SARGAM in Romanized Letters

```
x r,r g  - r M - m -
x r,r g  - r M - M -
x r,r r* - n M - g -
x N,N n  - M d - M -
```

For this notation set, pick "SARGAM".

### Example 2: SARGAM in Devaganari Scripts

```
x रे॒,रे॒   ग॒   -  रे॒  मे  -  म -
x रे॒,रे॒   ग॒   -  रे॒  मे  -  मे -
x रे॒,रे॒   रे॒*  -  नि॒  मे  -  ग॒ -
x नि,नि  नि॒  -  मे  नि॒  -  मे -
```

For this notation set, pick "SARGAM".

### Example 3: English, with Symbols

```
x C#,C# D#  - C# F# - F  -
x C#,C# D#  - C# F# - F# -
x C#,C# C#* - A# F# - D# -
x B,B   A#  - F# G# - F# -
```

For this notation set, pick "ENGLISH".

### Example 4: English, with Octave Numbers

```
x C#4,C#4 D#4 - C#4 F#4 - F4  - 
x C#4,C#4 D#4 - C#4 F#4 - F#4 -
x C#4,C#4 C#5 - A#4 F#4 - D#4 -
x B4,B4   A#4 - F#4 G#4 - F#4 -
```

For this notation set, pick "ENGLISH".

# Notes Allowed

You can pick your notation system and allowed notes. [reference reading...](https://en.wikipedia.org/wiki/Key_signature_names_and_translations)

MIDI Key       | 60 | 61  | 62 | 63  | 64 | 65 | 66  | 67 | 68 | 69 | 70  | 71 
---------------|----|-----|----|-----|----|----|-----|----|----|----|-----|----
SARGAM         | S  | r   | R  | g   | G  | m  | M   | P  | d  | D  | n   | N  
स र ग म        | सा  | रे॒   | रे  | ग॒    | ग  | म  | म॑   | प  | ध॒  | ध  | नि॒   | नि 
English        | C  | C#  | D  | D#  | E  | F  | F#  | G  | G# | A  | A#  | B
English        | C4 | C#4 | D4 | D#4 | E4 | F4 | F#4 | G4 | G#4| A4 | A#4 | B4

<!--
German         | C  | C#  | D  | D#  | E  | F  | F#  | G  | G# | A  | A#  | H
Carnatic`*`    | S  | r   | R  | g   | G  | m  | M   | P  | d  | D  | n   | N
Numeric        |    |     |    |     |    |    |     |    |    |    |     |  
Virtual Piano  |    |     |    |     |    |    |     |    |    |    |     |  
[ALDA](https://alda.io/)           |    |     |    |     |    |    |     |    |    |    |     |  
[ABC Notation](https://abcnotation.com/)   |    |     |    |     |    |    |     |    |    |    |     |  

`*`: Partial performance
-->

## Additional Notes (Table Expansion for Notes Allowed)
Completed?   | MIDI           | Notes                                            | Help Wanted?
-------------|----------------|--------------------------------------------------|--------------
[ Yes ]      | SARGAM         | Romanized Letters                                | Yes
[ Yes ]      | स र ग म        | komal or flats are underlined. Devangari Scripts | Yes
[ No ]       | Carnatic       | `Partial Performance`; NOT tested | Yes
[ Yes ]      | English        | Without octave number | Yes
[ Yes ]      | English        | With octave number | Yes
[ No ]       | German         | Without octave number | Yes
[ No ]       | Numeric        | - | Yes
[ No ]       | Virtual Piano  | - | Yes
[ No ]       | Alda           | - | Yes
[ No ]       | ABC Notation   | - | Yes

Note: This table (Additional Notes) is a horizontal expansion of the table from above (Notes Allowed).

## Symbols Allowed

- `-`: Note continuation marker eg. `S - - -` or `C - - -`
- `x`: Silence marker eg. `S x R x G` or `C x D x E` - lower cased 'x'
- `.`: Lower octave marker eg. `A#..` - the dot
- `*`: Higher octave marker. eg. `A#**` - the asterisk
- `,`: Beat's time sharing separated with a comma eg. `A#,B`
- `NUMERALS`: 0 - 9, for the Octave Number representation
- `|`: Optional meassures separator (bibhag or divisions)

Some special keys are also allowed, in order to be maximum user friendly typed notaions.
Example: `/` for `|`. `~` for `-`. Please study the source code variables for find and replace.

# ToDOs and History

- Need to support multiple instruments at once?
- Please see [ToDOs](TODO.md) file. Also, a help is needed.
- Please see [History](HISTORY.md) file for change logs.
- Please see [references](WHERE.md) used to learn.