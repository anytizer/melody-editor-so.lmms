#include "../includes/DataStructures.h"
#include "AbstractParser.h"

namespace lmms::gui::editor::pianoroll::parsing
{

    // https://en.wikipedia.org/wiki/Key_signature_names_and_translations
    class GermanParser: public AbstractParser
    {
        public:
            virtual void setup() override;
            QList<NotationCell *> parse(QString text) override;
    };

}