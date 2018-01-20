#include "tilesetarea.hpp"

TilesetArea::TilesetArea(QPixmap tileset, QWidget *parent)
    : QAbstractScrollArea(parent)
{
    _wTileset = new TilesetWidget(tileset, this);
    _layViewport = new QVBoxLayout(this);
    _layViewport->addWidget(_wTileset);
    viewport()->setLayout(_layViewport);

    setSizeAdjustPolicy(QScrollArea::AdjustToContents);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);


    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    verticalScrollBar()->setSingleStep(2);

    updateScrollBarRange();

    connect(_wTileset, SIGNAL(usedTileChanged(int)), this, SIGNAL(usedTileChanged(int)));
    connect(verticalScrollBar(), SIGNAL(valueChanged(int)), _wTileset, SLOT(setViewStartY(int)));
    connect(_wTileset, SIGNAL(viewStartYChanged(int)), verticalScrollBar(), SLOT(setValue(int)));
}


void TilesetArea::setTileset(QPixmap tileset) {
    _wTileset->setTileset(tileset);
    updateScrollBarRange();
}

void TilesetArea::resizeEvent(QResizeEvent *e) {
    QAbstractScrollArea::resizeEvent(e);
    updateScrollBarRange();

    e->ignore();
}

void TilesetArea::updateScrollBarRange() {
    verticalScrollBar()->setRange(0, _wTileset->getTilesetSize().height() - _wTileset->size().height());
}
