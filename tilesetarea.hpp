#ifndef TILESETAREA_HPP
#define TILESETAREA_HPP

/** tilesetarea.hpp
 * TilesetArea fournit une interface haut niveau pour interagir avec TilesetWidget :
 * Il s'occuppe de transmettre les signaux de ce dernier, et affiche les scrollbars
 *
 * Toufi 09/01/2018
*/

#include <QWidget>
#include <QScrollArea>
#include <QScrollBar>
#include <QVBoxLayout>

#include <QResizeEvent>

#include "main.hpp"
#include "tilesetwidget.hpp"

class TilesetArea : public QAbstractScrollArea
{
    Q_OBJECT
public:
    explicit TilesetArea(QPixmap tileset = QPixmap(), QWidget* parent = nullptr);

    void setTileset(QPixmap tileset);

signals:
    void usedTileChanged(int);

public slots:

protected:
    void resizeEvent(QResizeEvent *e);

    void updateScrollBarRange();

    TilesetWidget *_wTileset;
    QVBoxLayout *_layViewport;
};


#endif // TILESETAREA_HPP
