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
    QToolBar* shape2DToolBar;
    QToolBar* shape3DToolsBar;
    QDockWidget* dockWidget;
    draw2DWidget* central2DWidget;
    leftToolsWidget* mainToolsWidget;
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
    QActionGroup* draw3DGroupActs;
    QList<QAction*> draw2DShapeActs;
    QList<QAction*> draw3DShapeActs;
    QComboBox* triangleTypes;
    QSpinBox* penWidthBox;
    SDI_GeometricShape* activatedShape;
    //Private Functions
    void createActions();
    void createMenus();
    void createToolsBar();
    void createDockWidget();
    void setupDraw2DAct(QAction* drawAct);
    void setupDraw3DAct(QAction* drawAct);
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
    void showSelectedShape(SDI_GeometricShape *shape);
    void translateShape(int xtrans, int ytrans);
    void scaleShape(double xscale, double yscale);
    void rotateShape(double degree);
    void centralSymmetryShape();
    void OxSymmetryShape();
    void OySymmetryShape();
signals:
    void triangleTypeChanged(int triangleTypeID);
};

#endif // SDI_MAINWINDOW_H
