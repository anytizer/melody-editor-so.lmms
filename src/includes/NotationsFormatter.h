#include <QString>

namespace lmms::PLUGIN_NAME
{
    class NotationsFormatter
    {
        private:
            int width = 6;
            //QString formatColumns(QString line);
        
        public:
            QString format(QString notations);
    };
}