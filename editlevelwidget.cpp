#include "editlevelwidget.hpp"

EditLevelWidget::EditLevelWidget(const QPixmap &tileset, QWidget *parent) : QWidget(parent)
{
    _isSelecting = false;
    _isLeftButtonClicked = _isMiddleButtonClicked = false;
    _editMode = EditModes::Tiles;
    _actualLayer = Layer::Layer0;
    _tileset = tileset;

    resetLvl();
}


QPoint EditLevelWidget::getViewStart() const { return _viewStart; }
QPoint EditLevelWidget::getNbTiles() const { return _nbTiles; }

void EditLevelWidget::setTileset(QPixmap tileset) { _tileset = tileset; repaint(); }
void EditLevelWidget::setEditMode(int editMode) { _editMode = editMode; repaint(); }

bool EditLevelWidget::isEmpty() const { return _nbTiles.x() == 0 && _nbTiles.y() == 0; }

void EditLevelWidget::resetLvl() {

    for (int i = 0 ; i < _mapTilesLay0.size() ; i++) {
        _mapTilesLay0[i].clear();
        _mapTilesLay1[i].clear();
        _mapTilesLay2[i].clear();
        _mapCollisions[i].clear();
    }

    _mapTilesLay0.clear();
    _mapTilesLay1.clear();
    _mapTilesLay2.clear();
    _mapCollisions.clear();

    _usedTile = 0;
    _usedCollision = 0;
    _selectionRect = QRect(0, 0, 0, 0);

    _nbTiles.rx() = _nbTiles.ry() = 0;
    setViewStart(QPoint(0,0));

    _actualLayer = Layer::LayerAll;
    emit actualLayerChanged(_actualLayer);

    repaint();
}
void EditLevelWidget::createLvl(QPoint nbTiles) {
    // On commence par reset le niveau
    resetLvl();
    _nbTiles = nbTiles;

    _mapTilesLay0.reserve(nbTiles.y());
    _mapTilesLay1.reserve(nbTiles.y());
    _mapTilesLay2.reserve(nbTiles.y());
    _mapCollisions.reserve(nbTiles.y());

    for (int y = 0 ; y < nbTiles.y() ; y++) {
        _mapTilesLay0.append(QList<quint32>());
        _mapTilesLay1.append(QList<quint32>());
        _mapTilesLay2.append(QList<quint32>());
        _mapCollisions.append(QList<quint32>());

        _mapTilesLay0[y].reserve(nbTiles.x());
        _mapTilesLay1[y].reserve(nbTiles.x());
        _mapTilesLay2[y].reserve(nbTiles.x());
        _mapCollisions[y].reserve(nbTiles.x());

        for (int x = 0 ; x < nbTiles.x() ; x++) {
            _mapTilesLay0[y].append(0);
            _mapTilesLay1[y].append(0);
            _mapTilesLay2[y].append(0);
            _mapCollisions[y].append(0);
        }
    }
    repaint();
}

void EditLevelWidget::writeMap(const QList<QList<quint32> > &map, QTextStream &stream) const {

    for (int y = 0 ; y < _nbTiles.y() ; y++) {
        for (int x = 0 ; x < _nbTiles.x() ; x++) {
            stream << map.at(y).at(x) << ' ';
        }
        stream << '\n';
    }
}
void EditLevelWidget::writeLvl(QTextStream &oStream) const {

    oStream << _nbTiles.x() << ' ' << _nbTiles.y() << '\n';

    writeMap(_mapTilesLay0, oStream);
    writeMap(_mapTilesLay1, oStream);
    writeMap(_mapTilesLay2, oStream);
    writeMap(_mapCollisions, oStream);
}

void EditLevelWidget::loadMap(QList<QList<quint32> > &map, QTextStream &stream) {

    for (int y = 0 ; y < _nbTiles.y() ; y++) {
        for (int x = 0 ; x < _nbTiles.x() ; x++) {
            stream >> map[y][x];
        }
    }
}
void EditLevelWidget::loadLvl(QTextStream &iStream) {

    iStream >> _nbTiles.rx() >> _nbTiles.ry();

    createLvl(_nbTiles);

    loadMap(_mapTilesLay0, iStream);
    loadMap(_mapTilesLay1, iStream);
    loadMap(_mapTilesLay2, iStream);
    loadMap(_mapCollisions, iStream);

    repaint();
}

void EditLevelWidget::updateUsedTile(int tile) { _usedTile = tile; }
void EditLevelWidget::updateUsedCollision(int collision) { _usedCollision = collision; }

void EditLevelWidget::toggleIsSelecting(bool isSelecting) {
    _isSelecting = isSelecting;
    if (!_isSelecting)
        _selectionRect.setRect(0, 0, 0, 0);
    repaint();
}
void EditLevelWidget::toggleDisplayGrid(bool displayGrid) { _mustDisplayGrid = displayGrid; repaint(); }

void EditLevelWidget::setViewStart(const QPoint& start) {
    _viewStart = start;
    checkViewStart();

    emit viewStartChanged(_viewStart);
}
void EditLevelWidget::setViewStartX(int x) { setViewStart(QPoint(x, _viewStart.y())); }
void EditLevelWidget::setViewStartY(int y) { setViewStart(QPoint(_viewStart.x(), y)); }

void EditLevelWidget::moveViewStart(const QPoint& offset) {
    _viewStart += offset;
    checkViewStart();

    emit viewStartChanged(_viewStart);
}
void EditLevelWidget::moveViewStartX(int x) { moveViewStart(QPoint(x, 0)); }
void EditLevelWidget::moveViewStartY(int y) { moveViewStart(QPoint(0, y)); }

void EditLevelWidget::checkViewStart() {

    if (_viewStart.x() > _nbTiles.x() * TILE_SIZE - size().width())
        _viewStart.rx() = _nbTiles.x() * TILE_SIZE - size().width();
    if (_viewStart.x() < 0)
        _viewStart.rx() = 0;

    if (_viewStart.y() > _nbTiles.y() * TILE_SIZE - size().height())
        _viewStart.ry() = _nbTiles.y() * TILE_SIZE - size().height();
    if (_viewStart.y() < 0)
        _viewStart.ry() = 0;
}

void EditLevelWidget::paintMapTile(const QList<QList<quint32> > &map, QPainter &painter) {

    // On definit le coin en haut a gauche
    // Si _viewStart n'est pas un multiple de TILE_SIZE,
    // on rend son reste négatif pour afficher la tile en partie cachée
    QPoint topleftCorner;
    topleftCorner.rx() = (_viewStart.x() % TILE_SIZE) * -1;
    topleftCorner.ry() = (_viewStart.y() % TILE_SIZE) * -1;

    // Puis le coin en bas a droite
    QPoint bottomrightCorner;
    bottomrightCorner.rx() = size().width();
    bottomrightCorner.ry() = size().height();

    int xTile = 0, yTile = 0;

    yTile = _viewStart.y() / TILE_SIZE;
    for (int yView = topleftCorner.y() ; yView < bottomrightCorner.y() ; yView += TILE_SIZE, yTile++) {
        // Si la tile n'est plus dans le tableau,
        // on peut quitter
        if (yTile >= map.size()) {
            qDebug() << "PaintMap : Out Of Range sur les Y !";
            return;
        }

        xTile = _viewStart.x() / TILE_SIZE;
        for (int xView = topleftCorner.x() ; xView < bottomrightCorner.x() ; xView += TILE_SIZE, xTile++) {

            // Si la tile n'est pas dans le tableau,
            // on sort du 1er for, mais pas du 2e,
            // car il faut parser les tiles qui sont dessinables (avec un Y + grand, mais un X + petit)
            if (xTile >= map.at(yTile).size()) {
                qDebug() << "PaintMap : Out Of Range sur les X !!";
                break;
            }

            painter.drawPixmap(QPoint(xView, yView), _tileset,
                               QRect(map.at(yTile).at(xTile) % TILESET_WIDTH_IN_TILES * TILE_SIZE,
                                     map.at(yTile).at(xTile) / TILESET_WIDTH_IN_TILES * TILE_SIZE, TILE_SIZE, TILE_SIZE));
        }
    }
}
void EditLevelWidget::paintMapCollisions(const QList< QList<quint32> > &map, QPainter &painter) {

}
void EditLevelWidget::paintGrid(QPainter &painter) {

    QPoint topleftCorner;
    topleftCorner.rx() = (_viewStart.x() % TILE_SIZE) * -1;
    topleftCorner.ry() = (_viewStart.y() % TILE_SIZE) * -1;

    QPoint bottomrightCorner;
    bottomrightCorner.rx() = size().width();
    bottomrightCorner.ry() = size().height();

    QPoint maxOfLine(size().width(), size().height());
    if (maxOfLine.x() > _nbTiles.x() * TILE_SIZE)
        maxOfLine.rx() = _nbTiles.x() * TILE_SIZE;
    if (maxOfLine.y() > _nbTiles.y() * TILE_SIZE)
        maxOfLine.ry() = _nbTiles.y() * TILE_SIZE;

    int xTile = _viewStart.x() / TILE_SIZE;
    for (int xView = topleftCorner.x() ; xView <= bottomrightCorner.x() ; xView += TILE_SIZE, xTile++) {

        if (xTile >= _nbTiles.x()) {
            qDebug() << "PaintGrid : Out Of Range sur les X !";
            painter.drawLine(xView, 0, xView, maxOfLine.y());
            break;
        }
        painter.drawLine(xView, 0, xView, maxOfLine.y());
    }

    int yTile = _viewStart.y() / TILE_SIZE;
    for (int yView = topleftCorner.y() ; yView <= bottomrightCorner.y() ; yView += TILE_SIZE, yTile++) {

        if (yTile >= _nbTiles.y()) {
            qDebug() << "PaintGrid : Out Of Range sur les Y !";
            painter.drawLine(0, yView, maxOfLine.x(), yView);
            return;
        }
        painter.drawLine(0, yView, maxOfLine.x(), yView);
    }
}

void EditLevelWidget::paintEvent(QPaintEvent *e) {
    QWidget::paintEvent(e);

    if (_tileset.isNull())
        return;

    QPainter painter(this);

    switch (_editMode) {
    case EditModes::Tiles:

        switch (_actualLayer) {
        case Layer::Layer0:
            painter.setOpacity(1);
            paintMapTile(_mapTilesLay0, painter);
            painter.setOpacity(0.3);
            paintMapTile(_mapTilesLay1, painter);
            paintMapTile(_mapTilesLay2, painter);

            break;
        case Layer::Layer1:
            painter.setOpacity(0.3);
            paintMapTile(_mapTilesLay0, painter);
            painter.setOpacity(1);
            paintMapTile(_mapTilesLay1, painter);
            painter.setOpacity(0.3);
            paintMapTile(_mapTilesLay2, painter);

            break;
        case Layer::Layer2:
            painter.setOpacity(0.3);
            paintMapTile(_mapTilesLay0, painter);
            paintMapTile(_mapTilesLay1, painter);
            painter.setOpacity(1);
            paintMapTile(_mapTilesLay2, painter);

            break;
        default:
            painter.setOpacity(1);
            paintMapTile(_mapTilesLay0, painter);
            paintMapTile(_mapTilesLay1, painter);
            paintMapTile(_mapTilesLay2, painter);

            break;
        }

        break;
    case EditModes::Collisions:

        painter.setOpacity(1);
        paintMapTile(_mapTilesLay0, painter);
        paintMapTile(_mapTilesLay1, painter);
        paintMapTile(_mapTilesLay2, painter);
        painter.setOpacity(0.7);
        paintMapCollisions(_mapCollisions, painter);

        break;
    default:
        break;
    }

    if (_mustDisplayGrid) {
        painter.setOpacity(1);
        paintGrid(painter);
    }

    if (_isSelecting) {
        qDebug() << _selectionRect;
        if (_isLeftButtonClicked) {
            painter.setOpacity(0.4);
            painter.fillRect(_selectionRect, QColor(100, 100, 240));
            painter.setOpacity(0.7);
            painter.setPen(QColor(50, 50, 255));
            painter.drawRect(_selectionRect);
        }
    }


    painter.end();

    e->accept();
}

void EditLevelWidget::mousePressEvent(QMouseEvent *e) {
    QWidget::mousePressEvent(e);
    e->ignore();

    if (e->pos().x() < 0 || e->pos().y() < 0
            || e->pos().x() >= size().width() || e->pos().y() >= size().height())
        return;
    if (_viewStart.x() + e->pos().x() > _nbTiles.x() * TILE_SIZE
            || _viewStart.y() + e->pos().y() > _nbTiles.y() * TILE_SIZE)
        return;

    if (e->button() == Qt::RightButton) {

            if (_editMode == EditModes::Tiles) {
                switch (_actualLayer) {
                case Layer::Layer0:
                    _usedTile = _mapTilesLay0
                            [(_viewStart.y() + e->pos().y()) / TILE_SIZE][(_viewStart.x() + e->pos().x()) / TILE_SIZE];
                    break;
                case Layer::Layer1:
                    _usedTile = _mapTilesLay1
                            [(_viewStart.y() + e->pos().y()) / TILE_SIZE][(_viewStart.x() + e->pos().x()) / TILE_SIZE];
                    break;
                case Layer::Layer2:
                    _usedTile = _mapTilesLay2
                            [(_viewStart.y() + e->pos().y()) / TILE_SIZE][(_viewStart.x() + e->pos().x()) / TILE_SIZE];
                    break;
                }
                emit usedTileChanged(_usedTile);
            }
            else if (_editMode == EditModes::Collisions) {
                _usedCollision = _mapCollisions
                        [(_viewStart.y() + e->pos().y()) / TILE_SIZE][(_viewStart.x() + e->pos().x()) / TILE_SIZE];
                emit usedCollisionChanged(_usedCollision);
            }

        }

    if (e->button() == Qt::LeftButton) {


        if (_isSelecting) {
            _selectionRect.setTopLeft(e->pos());
            _selectionRect.setSize(QSize(0, 0));
        }
        else {
            if (_editMode == EditModes::Tiles) {
                switch (_actualLayer) {
                case Layer::Layer0:
                    _mapTilesLay0[(_viewStart.y() + e->pos().y()) / TILE_SIZE][(_viewStart.x() + e->pos().x()) / TILE_SIZE]
                            = _usedTile;
                    break;
                case Layer::Layer1:
                    _mapTilesLay1[(_viewStart.y() + e->pos().y()) / TILE_SIZE][(_viewStart.x() + e->pos().x()) / TILE_SIZE]
                            = _usedTile;
                    break;
                case Layer::Layer2:
                    _mapTilesLay2[(_viewStart.y() + e->pos().y()) / TILE_SIZE][(_viewStart.x() + e->pos().x()) / TILE_SIZE]
                            = _usedTile;
                    break;
                }
            }
            else if (_editMode == EditModes::Collisions) {
                _mapCollisions [(_viewStart.y() + e->pos().y()) / TILE_SIZE][(_viewStart.x() + e->pos().x()) / TILE_SIZE]
                        = _usedCollision;
            }
        }

        _isLeftButtonClicked = true;
    }

    if (e->button() == Qt::MiddleButton) {
        _middleClickPos = e->pos();
        _isMiddleButtonClicked = true;

        QApplication::setOverrideCursor(Qt::SizeAllCursor);
    }
    repaint();

    e->accept();
}
void EditLevelWidget::mouseReleaseEvent(QMouseEvent *e) {
    QWidget::mouseReleaseEvent(e);

    if (e->button() == Qt::LeftButton) {
        _isLeftButtonClicked = false;
        repaint();
    }
    if (e->button() == Qt::MiddleButton) {
        _isMiddleButtonClicked = false;

        QApplication::restoreOverrideCursor();
    }

    e->ignore();
}
void EditLevelWidget::mouseMoveEvent(QMouseEvent *e) {
    QWidget::mouseMoveEvent(e);

    if (_isLeftButtonClicked) {

        if (e->pos().x() < 0 || e->pos().y() < 0
                || e->pos().x() >= size().width() || e->pos().y() >= size().height())
            return;
        if (_viewStart.x() + e->pos().x() > _nbTiles.x() * TILE_SIZE
                || _viewStart.y() + e->pos().y() > _nbTiles.y() * TILE_SIZE)
            return;
        if (_isSelecting) {
            _selectionRect.setBottomRight(e->pos());
        }
        else {

            if (_editMode == EditModes::Tiles) {
                switch (_actualLayer) {
                case Layer::Layer0:
                    _mapTilesLay0[(_viewStart.y() + e->pos().y()) / TILE_SIZE][(_viewStart.x() + e->pos().x()) / TILE_SIZE]
                            = _usedTile;
                    break;
                case Layer::Layer1:
                    _mapTilesLay1[(_viewStart.y() + e->pos().y()) / TILE_SIZE][(_viewStart.x() + e->pos().x()) / TILE_SIZE]
                            = _usedTile;
                    break;
                case Layer::Layer2:
                    _mapTilesLay2[(_viewStart.y() + e->pos().y()) / TILE_SIZE][(_viewStart.x() + e->pos().x()) / TILE_SIZE]
                            = _usedTile;
                    break;
                }
            }
            else if (_editMode == EditModes::Collisions) {
                _mapCollisions [(_viewStart.y() + e->pos().y()) / TILE_SIZE][(_viewStart.x() + e->pos().x()) / TILE_SIZE]
                        = _usedCollision;
            }
        }
        repaint();
    }
    if (_isMiddleButtonClicked) {
        QPoint delta = e->pos() - _middleClickPos;
        delta *= 5;
        _middleClickPos = e->pos();
        moveViewStart(delta);
    }
    repaint();

    e->accept();
}

void EditLevelWidget::wheelEvent(QWheelEvent *e) {
    QWidget::wheelEvent(e);
    e->accept();

    if (_editMode != EditModes::Tiles)
        return;

    if (e->angleDelta().y() > 0) {
        _actualLayer++;
        if (_actualLayer >= 4)
            _actualLayer = 0;
        emit actualLayerChanged(_actualLayer);
    }
    else if (e->angleDelta().y() < 0) {
        _actualLayer--;
        if (_actualLayer < 0)
            _actualLayer = 3;
        emit actualLayerChanged(_actualLayer);
    }
    repaint();
}

void EditLevelWidget::keyPressEvent(QKeyEvent *e) {
    QWidget::keyPressEvent(e);
    e->ignore();
}
void EditLevelWidget::keyReleaseEvent(QKeyEvent *e) {
    QWidget::keyReleaseEvent(e);
    e->ignore();
}

