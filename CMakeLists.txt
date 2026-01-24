
INCLUDE(BuildPlugin)

BUILD_PLUGIN(
  melodyeditor
    MelodyEditor.cpp
    MelodyEditorTextArea.cpp
    MelodyEditorView.cpp
    src/includes/Utilities.cpp
    src/parsers/AldaParser.cpp
    src/parsers/SimpleParser.cpp
    src/parsers/StringReader.cpp

  MOCFILES
    MelodyEditor.h
    MelodyEditorTextArea.h
    MelodyEditorView.h
    src/includes/Utilities.h
 
  EMBEDDED_RESOURCES
    src/assets/logo.svg
    src/assets/logo.ico

    # Notation System Icons
    src/assets/ns-alda.svg
    src/assets/ns-hindustani.svg
    src/assets/ns-english.svg
)