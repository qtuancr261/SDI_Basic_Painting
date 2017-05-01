// GUI
#ifndef SDI_MAINWINDOW_H
#define SDI_MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QList>
#include <QByteArray>
#include <QAction>
#include <QActionGroup>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QDockWidget>
#include <QStatusBar>
#include <QSpinBox>
#include <QLabel>
#include <QComboBox>
#include <QImage>
#include <QImageWriter>
#include <QMessageBox>
#include <QFileDialog>
#include <QColorDialog>
#include "lefttoolswidget.h"
#include "draw2dwidget.h"
class SDI_MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit SDI_MainWindow(QWidget *parent = nullptr);
    ~SDI_MainWindow();
private:
    QMenu* FileMenu;
    QMenu* saveAsMenu;
    QMenu* ViewMenu;
    QMenu* ToolsMenu;
    QMenu* HelpMenu;
    QDockWidget* leftSideDockWidget;
    draw2DWidget* central2DWidget;
    // File Actions
    QAction* openAct;
    QList<QAction*> saveAsActs;
    QAction* printAct;
    QAction* quitAct;
    // View Actions
    QAction* showToolBarAct;
    QAction* showDockWidgetAct;
    QAction* show2DModeAct;
    QAction* show3DModeAct;
    // Tools Actions
    QAction* optionAct;
    QAction* pickPenColorAct;
    QAction* clearScreenAct;
    // Help Actions
    QAction* aboutSDI_PaintingAct;
    QAction* aboutQtAct;
    // Draw Actions
    QActionGroup* draw2DGroupActs;
    QList<QAction*> draw2DObjectActs;
    QComboBox* triangleTypes;
    QSpinBox* penWidthBox;
    //Private Functions
    void createActions();
    void createMenus();
    void createToolsBar();
    void createDockWidget();
    void setupDrawAct(QAction* drawAct);
    bool mayBeSave();
    bool saveFile(const QByteArray& fileFormat);

protected:
    virtual void closeEvent(QCloseEvent* closeEvent) override;
private slots:
    void open();
    void save();
    void penColor();
    void penWidth(int newWidth);
    void aboutSDI_Painting();

public slots:
    void showDockWidget(bool enable);
    void showMessage(QString message);
    void showSelectedShape(const SDI_GeometricShape* shape);
signals:
    void triangleTypeChanged(int triangleTypeID);
};

#endif // SDI_MAINWINDOW_H
