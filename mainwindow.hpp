#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

/** mainwindow.hpp
 * MainWindow est la classe principale.
 * Elle gêre toutes les actions, coordonne les signaux/slot,
 * et s'occupe de la communication entre les sous-widgets (EditLevelArea, TilesetArea et CollisionTableView)
 *
 * Toufi 05/01/2018
*/

#include <QApplication>
#include <QMainWindow>
#include <QSettings>
#include <QCloseEvent>
#include <QDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QTextStream>
#include <QPixmap>
#include <QFile>
#include <QFileInfo>

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QActionGroup>

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QFont>
#include <QLineEdit>
#include <QSpinBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "main.hpp"
#include "editlevelarea.hpp"
#include "tilesetarea.hpp"
#include "collisionwidget.hpp"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onUsedTileChanged(int tile);
    void onUsedCollisionChanged(int collision);
    void onActualLayerChanged(int layer);

    bool onNewLvl();
    bool onOpenLvl();
    bool onSave();
    bool onSaveAs();
    bool onReset();

    void changeMode(QAction* mode);

    void dialDocumentation();
    void dialAbout();
    void dialAboutQt();

protected:
    void initMenusActions();
    void connectActions();
    void loadSettings();
    void saveSettings();

    void setActionsEnabled(bool enabl);

    bool saveLvl();
    bool saveLvl(QString &path);

    void closeEvent(QCloseEvent *e);

protected:
    QWidget *_wCentral;
    QHBoxLayout *_layCentral;

    QFrame *_wLeftSide;
        QVBoxLayout *_layLeftSide;
        EditLevelArea *_editLvlArea;
        QLabel *_lblLayer;
    QFrame *_wRightSideTileMode;
        QVBoxLayout *_layRightSideTileMode;
        TilesetArea *_tilesetArea;
        QLabel *_lblUsedTile;
    QFrame *_wRightSideCollisionMode;
        QVBoxLayout *_layRightSideCollisionMode;
        CollisionWidget *_collisionWidget;
        QPushButton *_btnAddRow;
        QPushButton *_btnDelRow;

    QPixmap _tileset;
    QString _pathToActualLvl;

    QMenu *_menuFiles;
        QAction *_actNewLvl;
        QAction *_actOpenLvl;
        QAction *_actSave;
        QAction *_actSaveAs;
        QAction *_actReset;
        QAction *_actQuit;
    QMenu *_menuEdit;
        QAction *_actSelect;
        QAction *_actCut;
        QAction *_actCopy;
        QAction *_actPaste;
        QAction *_actErase;
        QAction *_actAddColumn;
        QAction *_actDelColumn;
        QAction *_actAddRow;
        QAction *_actDelRow;
    QMenu *_menuDisplay;
        QActionGroup *_groupActionsMode;
            QAction *_actTileMode;
            QAction *_actCollisionMode;
        QAction *_actShowGrid;
    QMenu *_menuHelp;
        QAction *_actDoc;
        QAction *_actAbout;
        QAction *_actAboutQt;

    static QString defaultPathTileset;
    static QString defaultPathOpenLvl;
    static QString defaultPathSaveLvl;
};


#endif // MAINWINDOW_HPP
