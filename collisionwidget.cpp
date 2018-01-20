#include "collisionwidget.hpp"

CollisionWidget::CollisionWidget(QWidget *parent)
    : QTableWidget(parent)
{
    _defaultFont.setPointSize(15);
    setMaximumWidth(300);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    setColumnCount(2);
    setSelectionBehavior(QTableWidget::SelectRows);
    setSelectionMode(QTableWidget::SingleSelection);
    setDragDropMode(QTableWidget::InternalMove);

    horizontalHeader()->setVisible(false);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    horizontalHeader()->setStretchLastSection(true);
    setColumnWidth(0, 50);

    verticalHeader()->setVisible(true);
    verticalHeader()->setSectionsMovable(false);
    verticalHeader()->setSectionsClickable(false);
    verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    addNewRow();
    item(0, 1)->setText(tr("Vide"));
    addNewRow();
    item(1, 1)->setText(tr("Plein"));

    selectRow(0);

    connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(onSelectionChanged()));
}

void CollisionWidget::addNewRow() {

    int count = rowCount();
    insertRow(count);

    setVerticalHeaderItem(count, new QTableWidgetItem(QString::number(count)));

    setItem(count, 0, new QTableWidgetItem(QString()));
    setItem(count, 1, new QTableWidgetItem(QString()));

    item(count, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable |
                             Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
    item(count, 1)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable |
                             Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable);
    item(count, 1)->setFont(_defaultFont);

    setRowHeight(count, 50);
}
void CollisionWidget::deleteSelectedRow() {

    if (selectionModel()->selectedRows().size() != 1)
        return;

    if (currentRow() == 0 || currentRow() == 1)
        return;

    removeRow(selectionModel()->selectedRows().at(0).row());

    // On renomme les lignes apres la suppression
    for (int i = 0 ; i < rowCount() ; i++)
        verticalHeaderItem(i)->setText(QString::number(i));


}

void CollisionWidget::onSelectionChanged() {
    if (selectionModel()->selectedRows().size() != 1)
        selectRow(0);
    emit collisionChanged(currentRow());
}

void CollisionWidget::dragEnterEvent(QDragEnterEvent *e) {

    if (rowAt(e->pos().y()) == 0 || rowAt(e->pos().y()) == 1)
        e->ignore();
    else
        e->accept();

    return;
}
void CollisionWidget::dragMoveEvent(QDragMoveEvent *e) {

    if (rowAt(e->pos().y()) == 0 || rowAt(e->pos().y()) == 1)
        e->ignore();
    else
        e->accept();

    return;
}
void CollisionWidget::dragLeaveEvent(QDragLeaveEvent *e) {
    e->ignore();
}

void CollisionWidget::dropEvent(QDropEvent *e) {
    if (e->source() != this) {
        e->ignore();
        return;
    }
    e->setDropAction(Qt::IgnoreAction);

    // On copie la row a la place de l'autre, et vice versa
    // D'abord l'icone

    // Et ensuite le string
    QString tmp;
    tmp = item(rowAt(e->pos().y()), 1)->text();
    item(rowAt(e->pos().y()), 1)->setText(item(rowAt(_clickPos.y()), 1)->text());
    item(rowAt(_clickPos.y()), 1)->setText(tmp);

    QTableWidget::dropEvent(e);
    e->accept();
}

void CollisionWidget::mousePressEvent(QMouseEvent *e) {
    if (e->button() == Qt::LeftButton)
        _clickPos = e->pos();
    QTableWidget::mousePressEvent(e);
    e->accept();
}
void CollisionWidget::mouseReleaseEvent(QMouseEvent *e) {
    QTableWidget::mouseReleaseEvent(e);
    e->accept();
}
void CollisionWidget::mouseMoveEvent(QMouseEvent *e) {

    QTableWidget::mouseMoveEvent(e);
    e->accept();
}
