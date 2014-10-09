#include "dialog.h"
#include "ui_dialog.h"

#include <QComboBox>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->treeWidget->setAlternatingRowColors(true);
    ui->treeWidget->setEditTriggers( QAbstractItemView::SelectedClicked |
                             QAbstractItemView::DoubleClicked );

    QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidget,
                                                QStringList() << "test1"
                                                              << "test2"
                                                              << "test3");
    ui->treeWidget->topLevelItem(0)->addChild(item);
    QComboBox* box = new QComboBox(ui->treeWidget);
    box->addItems(QStringList() << "a" << "b" << "c");

    ui->treeWidget->setItemWidget(item, 1, box);
}

Dialog::~Dialog()
{
    delete ui;
}
