#ifndef TILESETWIDGET_HPP
#define TILESETWIDGET_HPP

/** tilesetwidget.hpp
 * TilesetWidget sert a afficher le tileset
 * et a changer usedtile en emittant un signal
 *
 * Toufi 09/01/2018
*/

#include <QWidget>

#include <QPixmap>
#include <QSize>

#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>

#include "main.hpp"

class TilesetWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TilesetWidget(const QPixmap &tileset = QPixmap(), QWidget *parent = nullptr);

    void setTileset(const QPixmap &tileset);
    QSize getTilesetSize();

signals:
    void usedTileChanged(int);
    void viewStartYChanged(int);

public slots:
    void setViewStartY(int y);

protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

protected:
    QPixmap _tileset;
    int _viewStartY;
};

#endif // TILESETWIDGET_HPP
