#include "closemenubar.h"

CloseMenuBar::CloseMenuBar(QWidget *parent)
    : QMenuBar(parent)
{
    QAction* closeAction = new QAction("X", this);

    addAction(closeAction);
    closeAction->setObjectName("X");
    setStyleSheet("::item {"
                  "margin-top: 0px;"
                  "background: #990000;"
                  "color: white"
                  "}"
                  "QMenuBar::item:pressed {"
                  "background: #FF0000;"
                  "color: white"
                  "}"
                  "::item:selected {"
                  "background: #CC0000;"
                  "color: white}"
                  );
    connect(closeAction, SIGNAL(triggered()), this, SIGNAL(onClose()));

}

CloseMenuBar::~CloseMenuBar()
{

}

