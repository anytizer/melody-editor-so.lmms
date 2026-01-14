
INCLUDE(BuildPlugin)

# SET(RESOURCES icons.qrc)

BUILD_PLUGIN(
  # @see Utilities.cpp => internal identifiers | They MUST match
  melodyeditor
    MelodyEditor.cpp
    MelodyEditorTextArea.cpp
    MelodyEditorView.cpp
    src/includes/Utilities.cpp
    src/parsers/AbstractParser.cpp
    src/parsers/CarnaticParser.cpp
    src/parsers/EnglishParser.cpp
    src/parsers/GermanParser.cpp
    src/parsers/HindustaniParser.cpp
    src/parsers/MissingParser.cpp
    src/parsers/NashvilleParser.cpp
    src/parsers/ParsersFactory.cpp
    src/parsers/VirtualpianoParser.cpp
 
  MOCFILES
    MelodyEditor.h
    MelodyEditorTextArea.h
    MelodyEditorView.h
    src/includes/Utilities.h
 
  EMBEDDED_RESOURCES
    src/assets/logo.svg
    src/assets/logo.ico

    # Notation System Icons
    src/assets/ns-hindustani.svg
    src/assets/ns-carnatic.svg
    src/assets/ns-english.svg
)