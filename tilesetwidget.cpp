#include "tilesetwidget.hpp"

TilesetWidget::TilesetWidget(const QPixmap &tileset, QWidget *parent) : QWidget(parent)
{
    setFixedWidth(TILESET_WIDTH_IN_PIXEL);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    setTileset(tileset);
    emit usedTileChanged(0);
}

void TilesetWidget::setTileset(const QPixmap &tileset) {
    _tileset = tileset;
    _viewStartY = 0;
    emit viewStartYChanged(_viewStartY);
    repaint();
}
QSize TilesetWidget::getTilesetSize() { return _tileset.size(); }

void TilesetWidget::setViewStartY(int y) {

    _viewStartY = y;
    if (_viewStartY > _tileset.size().height() - size().height())
        _viewStartY = _tileset.size().height() - size().height();
    if (_viewStartY < 0)
        _viewStartY = 0;

    emit viewStartYChanged(_viewStartY);
}

void TilesetWidget::paintEvent(QPaintEvent *e) {
    QWidget::paintEvent(e);
    e->accept();

    if (_tileset.isNull())
        return;

    QPainter painter(this);
    painter.drawPixmap(QPoint(0, 0), _tileset,
                       QRect(0, _viewStartY, size().width(), size().height()));
    painter.end();
}

void TilesetWidget::mousePressEvent(QMouseEvent *e) {
    QWidget::mousePressEvent(e);
    e->ignore();

    if (_tileset.isNull())
        return;

    if (e->pos().x() < 0 || e->pos().x() >= size().width()
            || e->pos().y() < 0 || e->pos().y() >= size().height())
        return;

    if (e->pos().x() >= _tileset.size().width()
            || e->pos().y() + _viewStartY >= _tileset.size().height())
        return;

    int usedTile = ((_viewStartY + e->pos().y()) / TILE_SIZE * TILESET_WIDTH_IN_TILES
            + e->pos().x() / TILE_SIZE);

    emit usedTileChanged(usedTile);

    e->accept();
}
void TilesetWidget::mouseReleaseEvent(QMouseEvent *e) {
    QWidget::mouseReleaseEvent(e);
    e->accept();
}
