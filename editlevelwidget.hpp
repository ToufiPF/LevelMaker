#ifndef EDITLEVELWIDGET_HPP
#define EDITLEVELWIDGET_HPP

/** editlevelwidget.hpp
 * EditLevelWidget est la classe la plus complexe du programme.
 * C'est elle qui contrôle les maps, et qui s'occupe de les afficher
 * C'est une classe bas-niveau, on utilise EditLevelArea qui l'englobe.
 *
 * Toufi 05/01/2018
*/

#include <QApplication>
#include <QTextStream>
#include <QWidget>

#include <QPainter>
#include <QPen>
#include <QColor>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>

#include <QPixmap>
#include <QList>
#include <QPoint>
#include <QSize>
#include <QRect>

#include "main.hpp"

class EditLevelWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EditLevelWidget(const QPixmap &tileset = QPixmap(), QWidget *parent = nullptr);

    QPoint getViewStart() const;
    QPoint getNbTiles() const;

    void setTileset(QPixmap tileset);
    void setEditMode(int editMode);

    bool isEmpty() const;

    void writeLvl(QTextStream &oStream) const;
    void loadLvl(QTextStream &iStream);

signals:
    void usedTileChanged(int);
    void usedCollisionChanged(int);
    void actualLayerChanged(int);
    void viewStartChanged(const QPoint&);

public slots:
    void resetLvl();
    void createLvl(QPoint nbTiles);

    void updateUsedTile(int tile);
    void updateUsedCollision(int collision);

    void toggleIsSelecting(bool isSelecting);
    void toggleDisplayGrid(bool displayGrid);

    void setViewStart(const QPoint& start);
    void setViewStartX(int x);
    void setViewStartY(int y);

    void moveViewStart(const QPoint& offset);
    void moveViewStartX(int x);
    void moveViewStartY(int y);

protected:
    void checkViewStart();

    void writeMap(const QList< QList<quint32> > &map, QTextStream &stream) const;
    void loadMap(QList< QList<quint32> > &map, QTextStream &stream);

    void paintMapTile(const QList< QList<quint32> > &map, QPainter &painter);
    void paintMapCollisions(const QList< QList<quint32> > &map, QPainter &painter);
    void paintGrid(QPainter &painter);

    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);

protected:
    int _usedTile, _usedCollision;

    bool _isSelecting;
    QRect _selectionRect;
    bool _mustDisplayGrid;

    bool _isLeftButtonClicked, _isMiddleButtonClicked;
    QPoint _middleClickPos;

    int _editMode;
    int _actualLayer;

    QPoint _nbTiles;
    QPoint _viewStart;

    QPixmap _tileset;

    // 0 : affiché en 1er ; 2 : affiché au dessus
    QList<QList<quint32> > _mapTilesLay0;
    QList<QList<quint32> > _mapTilesLay1;
    QList<QList<quint32> > _mapTilesLay2;
    QList<QList<quint32> > _mapCollisions;
};

#endif // EDITLEVELWIDGET_HPP
