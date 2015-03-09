#include "managermenubar.h"

#include <QMessageBox>
#include <QDebug>
#include <QMouseEvent>
#include "dependencymanagermain.h"

#include <QCursor>
#include <QPainter>
#include <QFontMetrics>
#include <QFont>

ManagerMenuBar::ManagerMenuBar(QWidget *parent)
    : QMenuBar(parent)
{
    //set
    moveWindow = false;
    title = "Basic title";
}

ManagerMenuBar::~ManagerMenuBar()
{

}

void ManagerMenuBar::mousePressEvent(QMouseEvent *e)
{
    moveWindow = true;
    oldMousePos = e->pos();

    QMenuBar::mousePressEvent(e);
}

void ManagerMenuBar::mouseReleaseEvent(QMouseEvent *e)
{
    moveWindow = false;
    QMenuBar::mouseReleaseEvent(e);
}

void ManagerMenuBar::mouseMoveEvent(QMouseEvent *e)
{
    if (moveWindow)
    {
        if (e->buttons() && Qt::LeftButton)
        {
            auto temp = ((QMainWindow*)parent())->pos();
            ((QMainWindow*)parent())->move(e->pos() - oldMousePos + temp);
        }
    }
    QMenuBar::mouseMoveEvent(e);

}

void ManagerMenuBar::holdCurrentWindowPos(QPoint &p)
{
    currentWindowPos = p;
}

void ManagerMenuBar::setTitle(const QString &text)
{
    title = text;
}

void ManagerMenuBar::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);

    QFont font;
    QFontMetrics metric(font);

    int titleWidth = metric.width(title);

    QImage icon(":/res/Resources/logo_white_16.png");
    painter.drawImage((width() - titleWidth) / 2 - 20, 1, icon);
    painter.drawText(QPointF((width() - titleWidth) / 2, 12),  title);

    QMenuBar::paintEvent(e);
}

