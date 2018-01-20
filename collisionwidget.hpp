#ifndef COLLISIONWIDGET_HPP
#define COLLISIONWIDGET_HPP

#include <QTableWidget>
#include <QHeaderView>
#include <QWidget>

#include <QDropEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>
#include <QMouseEvent>

#include <QPoint>

#include "main.hpp"

class CollisionWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit CollisionWidget(QWidget *parent = nullptr);

signals:
    void collisionChanged(int);

public slots:
    void addNewRow();
    void deleteSelectedRow();

    void onSelectionChanged();

protected:
    void dragEnterEvent(QDragEnterEvent *e);
    void dragMoveEvent(QDragMoveEvent *e);
    void dragLeaveEvent(QDragLeaveEvent *e);
    void dropEvent(QDropEvent *e);

    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

    QPoint _clickPos;
    QFont _defaultFont;
};

#endif // COLLISIONWIDGET_HPP
