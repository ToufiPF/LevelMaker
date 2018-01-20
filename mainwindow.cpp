#include "mainwindow.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    _wCentral = new QWidget;
    setCentralWidget(_wCentral);

    _wLeftSide = new QFrame(this);
    _wLeftSide->setFrameShape(QFrame::StyledPanel);
    _layLeftSide = new QVBoxLayout(_wLeftSide);
    _editLvlArea = new EditLevelArea(_tileset, this);
    _lblLayer = new QLabel(this);
    QFont f;
    f.setPointSize(16);
    _lblLayer->setFont(f);
    _layLeftSide->addWidget(_editLvlArea);
    _layLeftSide->addWidget(_lblLayer);

    _wRightSideTileMode = new QFrame(this);
    _wRightSideTileMode->setFrameShape(QFrame::StyledPanel);
    _wRightSideTileMode->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    _layRightSideTileMode = new QVBoxLayout(_wRightSideTileMode);
    _tilesetArea = new TilesetArea(_tileset, this);
    _lblUsedTile = new QLabel(this);
    _lblUsedTile->setFixedSize(2 * TILE_SIZE, 2 * TILE_SIZE);
    _layRightSideTileMode->addWidget(_tilesetArea);
    _layRightSideTileMode->addWidget(_lblUsedTile);

    _wRightSideCollisionMode = new QFrame(this);
    _wRightSideCollisionMode->setFrameShape(QFrame::StyledPanel);
    _wRightSideCollisionMode->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    _layRightSideCollisionMode = new QVBoxLayout(_wRightSideCollisionMode);
    _collisionWidget = new CollisionWidget(this);
    _btnAddRow = new QPushButton(tr("Ajouter"), this);
    _btnDelRow = new QPushButton(tr("Supprimer"), this);
    _layRightSideCollisionMode->addWidget(_collisionWidget);
    _layRightSideCollisionMode->addWidget(_btnAddRow);
    _layRightSideCollisionMode->addWidget(_btnDelRow);


    _layCentral = new QHBoxLayout(_wCentral);
    _layCentral->addWidget(_wLeftSide);
    _layCentral->addWidget(_wRightSideTileMode);
    _layCentral->addWidget(_wRightSideCollisionMode);

    initMenusActions();
    connectActions();

    changeMode(_actTileMode);

    connect(_tilesetArea, SIGNAL(usedTileChanged(int)), this, SLOT(onUsedTileChanged(int)));
    connect(_editLvlArea, SIGNAL(usedTileChanged(int)), this, SLOT(onUsedTileChanged(int)));
    connect(_editLvlArea, SIGNAL(actualLayerChanged(int)), this, SLOT(onActualLayerChanged(int)));
    connect(_btnAddRow, SIGNAL(clicked(bool)), _collisionWidget, SLOT(addNewRow()));
    connect(_btnDelRow, SIGNAL(clicked(bool)), _collisionWidget, SLOT(deleteSelectedRow()));

    loadSettings();
}

MainWindow::~MainWindow()
{

}


void MainWindow::initMenusActions() {
    _menuFiles = menuBar()->addMenu(tr("&Fichier"));
        _actNewLvl = new QAction(tr("&Nouveau"), this);
        _actNewLvl->setShortcut(QKeySequence::New);
        _actOpenLvl = new QAction(tr("&Ouvrir"), this);
        _actOpenLvl->setShortcut(QKeySequence::Open);
        _actSave = new QAction(tr("Enregi&strer"), this);
        _actSave->setShortcut(QKeySequence::Save);
        _actSaveAs = new QAction(tr("Enregistrer sous"), this);
        _actSaveAs->setShortcut(QKeySequence::SaveAs);
        _actReset = new QAction(tr("&Réinitialiser"), this);
        _actReset->setShortcut(QKeySequence("Ctrl+R"));
        _actQuit = new QAction(tr("&Quitter"), this);
        _actQuit->setShortcut(QKeySequence::Close);

    _menuFiles->addAction(_actNewLvl);
    _menuFiles->addAction(_actOpenLvl);
    _menuFiles->addAction(_actSave);
    _menuFiles->addAction(_actSaveAs);
    _menuFiles->addAction(_actReset);
    _menuFiles->addAction(_actQuit);

    _menuEdit = menuBar()->addMenu(tr("&Edition"));
        _actSelect = new QAction(tr("Sélectionner"), this);
        _actSelect->setShortcut(QKeySequence("S"));
        _actSelect->setCheckable(true);
        _actCut = new QAction(tr("Couper"), this);
        _actCut->setShortcut(QKeySequence::Cut);
        _actCopy = new QAction(tr("Copier"), this);
        _actCopy->setShortcut(QKeySequence::Copy);
        _actPaste = new QAction(tr("Coller"), this);
        _actPaste->setShortcut(QKeySequence::Paste);
        _actErase = new QAction(tr("Effacer Sélection"), this);
        _actErase->setShortcut(QKeySequence("Ctrl+E"));
        _actAddColumn = new QAction(tr("Ajouter Colonne"), this);
        _actAddColumn->setShortcut(QKeySequence("Shift++"));
        _actAddRow = new QAction(tr("Ajouter Ligne"), this);
        _actAddRow->setShortcut(QKeySequence("Ctrl++"));
        _actDelColumn = new QAction(tr("Supprimer Colonne"), this);
        _actDelColumn->setShortcut(QKeySequence("Shift+-"));
        _actDelRow = new QAction(tr("Supprimer Ligne"), this);
        _actDelRow->setShortcut(QKeySequence("Ctrl+-"));

    _menuEdit->addAction(_actSelect);
    _menuEdit->addSeparator();
    _menuEdit->addAction(_actCut);
    _menuEdit->addAction(_actCopy);
    _menuEdit->addAction(_actPaste);
    _menuEdit->addSeparator();
    _menuEdit->addAction(_actAddColumn);
    _menuEdit->addAction(_actAddRow);
    _menuEdit->addAction(_actDelColumn);
    _menuEdit->addAction(_actDelRow);

    _menuDisplay = menuBar()->addMenu(tr("&Affichage"));
    _groupActionsMode = new QActionGroup(this);
        _actTileMode = new QAction(tr("Mode Tiles"), this);
        _actTileMode->setShortcut(QKeySequence("Ctrl+G"));
        _actTileMode->setProperty("editMode", EditModes::Tiles);
        _actTileMode->setCheckable(true);
        _actTileMode->setChecked(true);
        _actCollisionMode = new QAction(tr("Mode Collisions"), this);
        _actCollisionMode->setShortcut(QKeySequence("Ctrl+H"));
        _actCollisionMode->setProperty("editMode", EditModes::Collisions);
        _actCollisionMode->setCheckable(true);

    _groupActionsMode->addAction(_actTileMode);
    _groupActionsMode->addAction(_actCollisionMode);
    _groupActionsMode->setExclusive(true);

        _actShowGrid = new QAction(tr("Afficher la grille"), this);
        _actShowGrid->setCheckable(true);

    _menuDisplay->addActions(_groupActionsMode->actions());
    _menuDisplay->addSeparator();
    _menuDisplay->addAction(_actShowGrid);

    _menuHelp = menuBar()->addMenu(tr("Aide"));
        _actDoc = new QAction(tr("&Documentation"), this);
        _actAbout = new QAction(tr("A propos"), this);
        _actAboutQt = new QAction(tr("A propos de Qt"), this);

    _menuHelp->addAction(_actDoc);
    _menuHelp->addSeparator();
    _menuHelp->addAction(_actAbout);
    _menuHelp->addAction(_actAboutQt);

    setActionsEnabled(false);
}
void MainWindow::connectActions() {

    connect(_actNewLvl, SIGNAL(triggered(bool)), this, SLOT(onNewLvl()));
    connect(_actOpenLvl, SIGNAL(triggered(bool)), this, SLOT(onOpenLvl()));
    connect(_actSave, SIGNAL(triggered(bool)), this, SLOT(onSave()));
    connect(_actSaveAs, SIGNAL(triggered(bool)), this, SLOT(onSaveAs()));
    connect(_actReset, SIGNAL(triggered(bool)), this, SLOT(onReset()));
    connect(_actQuit, SIGNAL(triggered(bool)), this, SLOT(close()));

    connect(_actSelect, SIGNAL(triggered(bool)), _editLvlArea, SLOT(toggleIsSelecting(bool)));
    //connect(_actCut, SIGNAL(triggered(bool)),
    //connect(_actCopy, SIGNAL(triggered(bool)),
    //connect(_actPaste, SIGNAL(triggered(bool)),
    //connect(_actErase, SIGNAL(triggered(bool)),
    //connect(_actAddColumn, SIGNAL(triggered(bool)),
    //connect(_actDelColumn, SIGNAL(triggered(bool)),
    //connect(_actAddRow, SIGNAL(triggered(bool)),
    //connect(_actDelRow, SIGNAL(triggered(bool)),

    connect(_groupActionsMode, SIGNAL(triggered(QAction*)), this, SLOT(changeMode(QAction*)));
    connect(_actShowGrid, SIGNAL(triggered(bool)), _editLvlArea, SLOT(toggleDisplayGrid(bool)));

    connect(_actDoc, SIGNAL(triggered(bool)), this, SLOT(dialDocumentation()));
    connect(_actAbout, SIGNAL(triggered(bool)), this, SLOT(dialAbout()));
    connect(_actAboutQt, SIGNAL(triggered(bool)), this, SLOT(dialAboutQt()));
}
void MainWindow::loadSettings() {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                       ORGARNISATION_NAME + "/" + APPLICATION_NAME, APPLICATION_NAME, this);

    defaultPathTileset = settings.value("defaultPath/Tileset", QString()).toString();
    defaultPathOpenLvl = settings.value("defaultPath/OpenLvl", QString()).toString();
    defaultPathSaveLvl = settings.value("defaultPath/SaveLvl", QString()).toString();
    defaultPathTileset = defaultPathTileset.replace('\\', '/').trimmed();
    defaultPathOpenLvl = defaultPathOpenLvl.replace('\\', '/').trimmed();
    defaultPathSaveLvl = defaultPathSaveLvl.replace('\\', '/').trimmed();

}
void MainWindow::saveSettings() {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                       ORGARNISATION_NAME + "/" + APPLICATION_NAME, APPLICATION_NAME, this);

    settings.setValue("defaultPath/Tileset", defaultPathTileset);
    settings.setValue("defaultPath/OpenLvl", defaultPathOpenLvl);
    settings.setValue("defaultPath/SaveLvl", defaultPathSaveLvl);

}

void MainWindow::setActionsEnabled(bool enabl) {

    _actSave->setEnabled(enabl);
    _actSaveAs->setEnabled(enabl);
    _actReset->setEnabled(enabl);
}

bool MainWindow::saveLvl() { return saveLvl(_pathToActualLvl); }
bool MainWindow::saveLvl(QString &path) {

    QFile fileLvl(path, this);
    if (!fileLvl.open(QFile::WriteOnly)) {
        qDebug() << "Impossible d'enregistrer " << path << ".";
        qDebug() << fileLvl.errorString();
        return false;
    }

    QTextStream textStream(&fileLvl);
    _editLvlArea->writeLvl(textStream);
    fileLvl.close();

    path.remove(".lvl");
    path += ".png";

    if (!_tileset.save(path)) {
        qDebug() << "Impossible d'enregistrer " << path << ".";
        return false;
    }

    _pathToActualLvl = path;
    return true;
}

void MainWindow::onUsedTileChanged(int tile) {
    _editLvlArea->updateUsedTile(tile);

    _lblUsedTile->setPixmap(_tileset.copy(tile % TILESET_WIDTH_IN_TILES * TILE_SIZE,
                                          tile / TILESET_WIDTH_IN_TILES * TILE_SIZE,
                                          TILE_SIZE, TILE_SIZE).scaled(_lblUsedTile->size()));

}
void MainWindow::onUsedCollisionChanged(int collision) {
    _editLvlArea->updateUsedCollision(collision);
}
void MainWindow::onActualLayerChanged(int layer) {
    switch (layer) {
    case Layer::Layer0:
        _lblLayer->setText(tr("Layer 0"));
        break;
    case Layer::Layer1:
        _lblLayer->setText(tr("Layer 1"));
        break;
    case Layer::Layer2:
        _lblLayer->setText(tr("Layer 2"));
        break;
    default:
        _lblLayer->setText(tr("Tous les layers"));
        break;
    }
}

bool MainWindow::onNewLvl() {
    QDialog dialNewLvl(this, Qt::WindowCloseButtonHint | Qt::WindowTitleHint);
    dialNewLvl.setWindowTitle(tr("Nouveau Niveau"));

    QGridLayout *gridLayout = new QGridLayout(&dialNewLvl);

    QLabel *lblDimensions = new QLabel(tr("Dimensions du niveau :"), &dialNewLvl);

    QSpinBox *spinX = new QSpinBox(&dialNewLvl);
    spinX->setToolTip(tr("Entrez le nombre de tiles sur les x"));
    spinX->setValue(50);
    spinX->setMinimum(1);
    spinX->setMaximum(2000);
    spinX->setAccelerated(true);

    QSpinBox *spinY = new QSpinBox(&dialNewLvl);
    spinY->setToolTip(tr("Entrez le nombre de tiles sur les y"));
    spinY->setValue(50);
    spinY->setMinimum(1);
    spinY->setMaximum(2000);
    spinY->setAccelerated(true);

    QWidget *spacer = new QWidget(&dialNewLvl);
    spacer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    QLabel *lblPath = new QLabel(tr("Chemin vers le tileset"), &dialNewLvl);

    QLineEdit *lineEditPathTileset = new QLineEdit(&dialNewLvl);
    lineEditPathTileset->setMinimumWidth(300);

    QPushButton *btnBrowse = new QPushButton("...", &dialNewLvl);
    btnBrowse->setMaximumWidth(30);

    QPushButton *btnOk = new QPushButton(tr("Ok"), &dialNewLvl);
    btnOk->setEnabled(false);
    connect(btnOk, SIGNAL(clicked(bool)), &dialNewLvl, SLOT(accept()));

    QPushButton *btnCancel = new QPushButton(tr("Annuler"), &dialNewLvl);
    connect(btnCancel, SIGNAL(clicked(bool)), &dialNewLvl, SLOT(reject()));

    QLabel *lblPreviewTileset = new QLabel(tr("Aperçu du Tileset"), &dialNewLvl);
    lblPreviewTileset->setMinimumSize(5 * TILE_SIZE, 8 * TILE_SIZE);
    lblPreviewTileset->setFrameShape(QLabel::StyledPanel);
    lblPreviewTileset->setAlignment(Qt::AlignCenter);

    QPixmap *previewTileset = new QPixmap;

    gridLayout->addWidget(lblDimensions, 0, 0);
    gridLayout->addWidget(spinX, 1, 0);
    gridLayout->addWidget(spinY, 2, 0);
    gridLayout->addWidget(spacer, 3, 0);
    gridLayout->addWidget(lblPath, 4, 0);
    gridLayout->addWidget(lineEditPathTileset, 5, 0, 1, 2);
    gridLayout->addWidget(btnBrowse, 5, 2);
    gridLayout->addWidget(btnOk, 6, 3);
    gridLayout->addWidget(btnCancel, 6, 4);
    gridLayout->addWidget(lblPreviewTileset, 0, 1, 5, 4);


    dialNewLvl.setLayout(gridLayout);

    QFileDialog *dialTileset =
            new QFileDialog(&dialNewLvl, tr("Choisir Tileset"),
                            defaultPathTileset, tr("Images (*.bmp *.png *.gif)"));
    dialTileset->setViewMode(QFileDialog::Detail);
    dialTileset->setFileMode(QFileDialog::ExistingFile);
    dialTileset->setModal(true);

    connect(btnBrowse, &QPushButton::clicked, dialTileset, &QFileDialog::exec);
    connect(dialTileset, &QFileDialog::accepted, &dialNewLvl, [=]() {
        if (!dialTileset->selectedFiles().isEmpty())
           lineEditPathTileset->setText(dialTileset->selectedFiles().at(0));
    });
    connect(lineEditPathTileset, &QLineEdit::textChanged, &dialNewLvl, [=]() {

        if (QFileInfo(lineEditPathTileset->text()).exists() && previewTileset->load(lineEditPathTileset->text())) {
            lblPreviewTileset->setPixmap(previewTileset->scaled(lblPreviewTileset->size(), Qt::KeepAspectRatio));
            btnOk->setEnabled(true);
        }
        else {
            lblPreviewTileset->setText(tr("Aperçu du Tileset"));
            btnOk->setEnabled(false);
        }
    });

    dialNewLvl.exec();

    if (dialNewLvl.result() != QDialog::Accepted)
        return false;

    if (!_tileset.load(lineEditPathTileset->text()))
        return false;

    _editLvlArea->createLvl(spinX->value(), spinY->value());
    _editLvlArea->setTileset(_tileset);
    _tilesetArea->setTileset(_tileset);

    setActionsEnabled(true);

    delete previewTileset;
    return true;
}
bool MainWindow::onOpenLvl() {

    QString path =
            QFileDialog::getOpenFileName(this, tr("Ouvrir un Niveau"), defaultPathOpenLvl, tr("Niveau *.lvl"));

    QFile fileLvl(path, this);
    if (!fileLvl.open(QFile::ReadOnly)) {
        qDebug() << "Impossible de lire " << path << ".";
        qDebug() << fileLvl.errorString();
        return false;
    }

    QTextStream textStream(&fileLvl);
    _editLvlArea->loadLvl(textStream);
    fileLvl.close();

    path.remove(".lvl");
    path += ".png";

    if (!_tileset.load(path)) {
        qDebug() << "Impossible de lire " << path << ".";
        return false;
    }

    _editLvlArea->setTileset(_tileset);
    _tilesetArea->setTileset(_tileset);

    setActionsEnabled(true);
    return true;
}
bool MainWindow::onSave() {

    if (_pathToActualLvl.isEmpty())
        return onSaveAs();
    else
        return saveLvl();
}
bool MainWindow::onSaveAs() {

    QString path(QFileDialog::getSaveFileName(this, tr("Enregistrer Sous"), defaultPathSaveLvl, tr("Niveau *.lvl")));

    return saveLvl(path);
}
bool MainWindow::onReset() {

    if (QMessageBox::question(this, tr("Réinitialiser le Niveau ?"),
                              tr("Voulez vous vraiment réinitialiser ce niveau ?\n"
                                 "Si vous confirmez, les changements non sauvegardés seront perdus."))
            != QMessageBox::Yes)
        return false;

    _tileset = QPixmap();
    _pathToActualLvl.clear();

    _editLvlArea->resetLvl();
    _editLvlArea->setTileset(_tileset);
    _tilesetArea->setTileset(_tileset);

    changeMode(_actTileMode);

    setActionsEnabled(false);

    return true;
}

void MainWindow::changeMode(QAction* mode) {

    switch (mode->property("editMode").toInt()) {
    case EditModes::Tiles:
        _editLvlArea->setEditMode(EditModes::Tiles);
        _wRightSideTileMode->setVisible(true);
        _wRightSideTileMode->setEnabled(true);
        _wRightSideCollisionMode->setVisible(false);
        _wRightSideCollisionMode->setEnabled(false);

        break;
    case EditModes::Collisions:
        _editLvlArea->setEditMode(EditModes::Collisions);
        _wRightSideTileMode->setVisible(false);
        _wRightSideTileMode->setEnabled(false);
        _wRightSideCollisionMode->setVisible(true);
        _wRightSideCollisionMode->setEnabled(true);

        break;
    default:
        qDebug() << "void MainWindow::changeMode() : Mode Inconnu ?!?";
        break;
    }
}

void MainWindow::dialDocumentation() {

}
void MainWindow::dialAbout() {
    QMessageBox::information(this, tr("A propos de LevelMaker"),
                             tr("Vous utilisez la version ") + tr(ACTUAL_VERSION) + ". \n", QMessageBox::Ok);
}
void MainWindow::dialAboutQt() {
    QMessageBox::aboutQt(this);
}

void MainWindow::closeEvent(QCloseEvent *e) {
    saveSettings();
    e->accept();
}

QString MainWindow::defaultPathTileset = QString();
QString MainWindow::defaultPathOpenLvl = QString();
QString MainWindow::defaultPathSaveLvl = QString();
