#ifndef MAIN_HPP
#define MAIN_HPP

/** main.hpp
 * Le header global. Il est inclus dans tous les autres.
 * Il pose des constantes et enum utiles dans tout le programme.
 *
 * Toufi 05/01/2018
*/

#include <QDebug>
#include <QString>

const char ACTUAL_VERSION[] = "Alpha 0.1.1";
const QString ORGARNISATION_NAME("PouetForce");
const QString APPLICATION_NAME("LevelMaker");
const int TILE_SIZE = 32;
const int TILESET_WIDTH_IN_TILES = 10;
const int TILESET_WIDTH_IN_PIXEL = TILESET_WIDTH_IN_TILES * TILE_SIZE;

namespace EditModes {
    enum EditModes
    {
        Tiles,
        Collisions
    };
}
namespace Layer {
    enum Layer
    {
        LayerAll,
        Layer0,
        Layer1,
        Layer2
    };
}


#endif // MAIN_HPP
