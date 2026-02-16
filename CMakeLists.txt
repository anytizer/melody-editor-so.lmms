# Clone ABC-parser library in root of this project.
# git clone https://github.com/MeesCode/ABC-parser.git ABC-parser

INCLUDE(BuildPlugin)

BUILD_PLUGIN(
  melodyeditor
    MelodyEditor.cpp
    MelodyEditorTextArea.cpp
    MelodyEditorView.cpp
    src/includes/NotationsFormatter.cpp
    src/includes/Utilities.cpp
    src/parsers/ABCParser.cpp
    src/parsers/AldaParser.cpp
    src/parsers/BhatkandeParser.cpp
    src/parsers/SimpleParser.cpp
    src/parsers/StringReader.cpp

    ABC-parser/abc_parser.c

  MOCFILES
    MelodyEditor.h
    MelodyEditorTextArea.h
    MelodyEditorView.h
    src/includes/NotationsFormatter.h
    src/includes/Utilities.h

    ABC-parser/abc_parser.h
 
  EMBEDDED_RESOURCES
    src/assets/logo.svg
    src/assets/logo.ico

    # Notation System Icons
    src/assets/ns-abc.svg
    src/assets/ns-alda.svg
    src/assets/ns-hindustani.svg
    src/assets/ns-english.svg
)
