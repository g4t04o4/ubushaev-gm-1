#include "mainwindow.h"
#include "graphwidget.h"

#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle(tr("FDL"));

    widget  = new GraphWidget;
    setCentralWidget(widget);

    createActions();
    createMenus();
}

void MainWindow::createActions()
{

    openAct = new QAction(tr("&Open"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open a file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(openFile()));

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save a file"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(saveFile()));

    saveAsAct = new QAction(tr("Save &As"), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save a file as something"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAsFile()));

    exportAct = new QAction(tr("&Export"), this);
    exportAct->setShortcuts(QKeySequence::Print);
    exportAct->setStatusTip(tr("Export a file"));
    connect(exportAct, SIGNAL(triggered()), this, SLOT(exportFile()));

    recreateAct = new QAction(tr("&Recreate"), this);
    recreateAct->setShortcuts(QKeySequence::Refresh);
    recreateAct->setStatusTip(tr("Recreate graph"));
    connect(recreateAct, SIGNAL(triggered()), this, SLOT(recreateGraph()));

    clearAct = new QAction(tr("&Clear"), this);
    clearAct->setShortcuts(QKeySequence::New);
    clearAct->setStatusTip(tr("Clear graph"));
    connect(clearAct, SIGNAL(triggered()), this, SLOT(clearGraph()));

    shuffleAct = new QAction(tr("&Shuffle"), this);
    //shuffleAct->setShortcuts(QKeySequence::);
    shuffleAct->setStatusTip(tr("Shuffle graph"));
    connect(shuffleAct, SIGNAL(triggered()), this, SLOT(shuffleGraph()));

    fillAct = new QAction(tr("&Fill"), this);
    //fillAct->setShortcuts(QKeySequence::);
    fillAct->setStatusTip(tr("Fill graph"));
    connect(fillAct, SIGNAL(triggered()), this, SLOT(fillGraph()));

    zoomInAct = new QAction(tr("&ZoomIn"), this);
    zoomInAct->setShortcuts(QKeySequence::ZoomIn);
    zoomInAct->setStatusTip(tr("Zoom In"));
    connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

    zoomOutAct = new QAction(tr("&ZoomOut"), this);
    zoomOutAct->setShortcuts(QKeySequence::ZoomOut);
    zoomOutAct->setStatusTip(tr("Zoom Out"));
    connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

    pauseAct = new QAction(tr("&Pause"), this);
    //pauseAct->setShortcuts(QKeySequence::);
    pauseAct->setStatusTip(tr("Pause"));
    connect(pauseAct, SIGNAL(triggered()), this, SLOT(pause()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setShortcuts(QKeySequence::HelpContents);
    aboutAct->setStatusTip(tr("About"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exportAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(recreateAct);
    editMenu->addAction(clearAct);
    editMenu->addAction(shuffleAct);
    editMenu->addAction(fillAct);

    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(zoomInAct);
    viewMenu->addAction(zoomOutAct);
    viewMenu->addSeparator();
    viewMenu->addAction(pauseAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

void MainWindow::openFile()
{

}

void MainWindow::saveFile()
{
    saveAsFile();
}

void MainWindow::saveAsFile()
{
    QString filename = QFileDialog::getSaveFileName(this);
    widget->exportToTXT(filename);
}

void MainWindow::exportFile()
{
    QString filename = QFileDialog::getSaveFileName(this);
    widget->exportToPNG(filename);
}

void MainWindow::recreateGraph()
{
    widget->recreate();
}

void MainWindow::clearGraph()
{
    widget->clearScreen();
}

void MainWindow::shuffleGraph()
{
    widget->shuffle();
}

void MainWindow::fillGraph()
{
    widget->fillGraph(32, 50);
}

void MainWindow::zoomIn()
{
    widget->zoomIn();
}

void MainWindow::zoomOut()
{
    widget->zoomOut();
}

void MainWindow::pause()
{
    widget->pause();
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("FM. Read It."),
                       tr("shuffle - Space, zoomIn - +, zoomOut - -, "
                          "pause - P, exportToPNG - S, exportToText - T, "
                          "clearScreen - C, recreate - R"));
}