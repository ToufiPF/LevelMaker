#ifndef EDITLVLAREA_HPP
#define EDITLVLAREA_HPP

/** editlevelarea.hpp
 * EditLevelArea fournit une interface haut niveau pour interagir avec EditLevelWidget :
 * Il s'occuppe de transmettre les signaux à ce dernier, et affiche les scrollbars
 *
 * Toufi 05/01/2018
*/

#include <QWidget>
#include <QScrollArea>
#include <QScrollBar>
#include <QVBoxLayout>

#include <QResizeEvent>

#include "main.hpp"
#include "editlevelwidget.hpp"

class EditLevelArea : public QAbstractScrollArea
{
    Q_OBJECT
public:
    explicit EditLevelArea(QPixmap tileset = QPixmap(), QWidget *parent=nullptr);

    void setTileset(QPixmap tileset);
    void setEditMode(int editMode);

    bool isEmpty() const;

    void writeLvl(QTextStream &oStream) const;
    void loadLvl(QTextStream &iStream);

signals:
    void usedTileChanged(int);
    void actualLayerChanged(int);

public slots:
    void resetLvl();

    void createLvl(int nbTilesX, int nbTilesY);
    void createLvl(const QPoint &nbTiles);

    void updateUsedTile(int tile);
    void updateUsedCollision(int collision);

    void toggleIsSelecting(bool isSelecting);
    void toggleDisplayGrid(bool displayGrid);

    void updateScrollBarsPos(const QPoint& viewStart);
    void updateScrollBarsRange(const QPoint& nbTiles);

protected:
    void resizeEvent(QResizeEvent *e);
    EditLevelWidget* _wEditLvl;
    QVBoxLayout *_layViewport;

};


#endif // EDITLVLAREA_HPP
