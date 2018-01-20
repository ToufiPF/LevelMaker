#include "editlevelarea.hpp"

EditLevelArea::EditLevelArea(QPixmap tileset, QWidget *parent)
    : QAbstractScrollArea(parent)
{
    _wEditLvl = new EditLevelWidget(tileset, this);
    _layViewport = new QVBoxLayout(this);
    _layViewport->addWidget(_wEditLvl);
    viewport()->setLayout(_layViewport);

    connect(_wEditLvl, SIGNAL(usedTileChanged(int)), this, SIGNAL(usedTileChanged(int)));
    connect(_wEditLvl, SIGNAL(actualLayerChanged(int)), this, SIGNAL(actualLayerChanged(int)));

    resetLvl();

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    connect(horizontalScrollBar(), SIGNAL(valueChanged(int)), _wEditLvl, SLOT(setViewStartX(int)));
    connect(verticalScrollBar(), SIGNAL(valueChanged(int)), _wEditLvl, SLOT(setViewStartY(int)));

    connect(_wEditLvl, SIGNAL(viewStartChanged(QPoint)), this, SLOT(updateScrollBarsPos(QPoint)));
}


void EditLevelArea::resetLvl() {
    horizontalScrollBar()->setRange(0, 0);
    verticalScrollBar()->setRange(0, 0);

    _wEditLvl->resetLvl();
}

void EditLevelArea::setTileset(QPixmap tileset) { _wEditLvl->setTileset(tileset); }
void EditLevelArea::setEditMode(int editMode) { _wEditLvl->setEditMode(editMode); }

bool EditLevelArea::isEmpty() const { return _wEditLvl->isEmpty(); }

void EditLevelArea::writeLvl(QTextStream &oStream) const { _wEditLvl->writeLvl(oStream); }
void EditLevelArea::loadLvl(QTextStream &iStream) {
    _wEditLvl->loadLvl(iStream);
    updateScrollBarsRange(_wEditLvl->getNbTiles());
}

void EditLevelArea::createLvl(int nbTilesX, int nbTilesY) { createLvl(QPoint(nbTilesX, nbTilesY)); }
void EditLevelArea::createLvl(const QPoint &nbTiles) {
    resetLvl();
    _wEditLvl->createLvl(nbTiles);
    updateScrollBarsRange(nbTiles);
}

void EditLevelArea::updateUsedTile(int tile) { _wEditLvl->updateUsedTile(tile); }
void EditLevelArea::updateUsedCollision(int collision) { _wEditLvl->updateUsedCollision(collision); }

void EditLevelArea::toggleIsSelecting(bool isSelecting) { _wEditLvl->toggleIsSelecting(isSelecting); }
void EditLevelArea::toggleDisplayGrid(bool displayGrid) { _wEditLvl->toggleDisplayGrid(displayGrid); }

void EditLevelArea::updateScrollBarsPos(const QPoint& viewStart) {
    horizontalScrollBar()->setValue(viewStart.x());
    verticalScrollBar()->setValue(viewStart.y());
}
void EditLevelArea::updateScrollBarsRange(const QPoint& nbTiles) {
    horizontalScrollBar()->setRange(0, nbTiles.x() * TILE_SIZE - _wEditLvl->size().width());
    verticalScrollBar()->setRange(0, nbTiles.y() * TILE_SIZE - _wEditLvl->size().height());
}

void EditLevelArea::resizeEvent(QResizeEvent *e) {
    QAbstractScrollArea::resizeEvent(e);
    updateScrollBarsRange(_wEditLvl->getNbTiles());
    e->ignore();
}
