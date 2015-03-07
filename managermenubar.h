#ifndef MANAGERMENUBAR_H
#define MANAGERMENUBAR_H

#include <QObject>
#include <QMenuBar>
#include <QPoint>

class ManagerMenuBar : public QMenuBar
{
    Q_OBJECT
public:
    explicit ManagerMenuBar(QWidget *parent = 0);
    ~ManagerMenuBar();

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;

public slots:
    void holdCurrentWindowPos(QPoint&);

signals:
    void windowChangedPos(QPoint&);

private:
    bool moveWindow;
    QPoint oldMousePos;
    QPoint currentWindowPos;
};

#endif // MANAGERMENUBAR_H
