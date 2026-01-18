/**
 * MelodyEditorEventFilter.cpp
 *
 * Copyright (c) 2025 - 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 */

 #include <QDebug>

#include "MelodyEditorEventFilter.h"
#include "MelodyEditorTextArea.h"

namespace lmms::gui
{

    MelodyEditorEventFilter::MelodyEditorEventFilter(QObject* parent) : QObject(parent) {}

    bool MelodyEditorEventFilter::eventFilter(QObject* obj, QEvent* event) {
        
        if(event->type() == QEvent::KeyPress)
        {
            qDebug() << "Key Press...1: "; //  << sender()->objectName();
            if(QPlainTextEdit *pte = qobject_cast<QPlainTextEdit*>(sender()))
            {
                qDebug() << "sender is PTE";
                qDebug() << "Text changed in:" << pte->objectName();
                event->accept();
                return true;
            }

            if(MelodyEditorTextArea *meta = qobject_cast<MelodyEditorTextArea*>(sender()))
            {
                qDebug() << "sender is meta";
                event->accept();
                return true;
            }
        }

        if(event->type() == QEvent::MouseButtonPress)
        {
            qDebug() << "Mouse press...";
            event->accept();
            return true;
        }

        // if(QPlainTextEdit *pte = qobject_cast<QPlainTextEdit*>(sender()))
        // {
        //     if(event->type() == QEvent::KeyPress)
        //     {
        //         qDebug() << "key pressed!";
        //         QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        //         event->accept();
        //         return true;
        //     }
        // }

        // if(MelodyEditorTextArea *meta = qobject_cast<MelodyEditorTextArea*>(sender()))
        // {
        //     if(event->type() == QEvent::KeyPress)
        //     {
        //         qDebug() << "MelodyEditorTextArea key press!";
        //         QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        //         event->accept();
        //         return true;
        //     }
        // }

        // if(QPushButton *button = qobject_cast<QPushButton*>(sender()))
        // {
        //     if(event->type() == QEvent::MouseButtonPress) // mouse click
        //     {
        //         qDebug() << "button pressed!";
        //         event->accept();
        //         return true;
        //     }
        // }
        
        return QObject::eventFilter(obj, event);
    }
}