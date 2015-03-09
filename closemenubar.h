#ifndef CLOSEMENUBAR_H
#define CLOSEMENUBAR_H

#include <QObject>
#include <QMenuBar>

class CloseMenuBar : public QMenuBar
{
    Q_OBJECT
public:
    explicit CloseMenuBar(QWidget *parent = nullptr);
    ~CloseMenuBar();

signals:
    void onClose();

};

#endif // CLOSEMENUBAR_H
