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
#include "transformationtoolswidget.h"
#include "paintwidget.h"
class SDI_MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit SDI_MainWindow(QWidget *parent = nullptr);
    ~SDI_MainWindow();
private:
    QToolBar* shape2DToolBar;
    QToolBar* shape3DToolBar;
    QDockWidget* dockWidget;
    PaintWidget* central2DWidget;
    TransformationToolsWidget* mainToolsWidget;
    QLabel* modeToolTip;
    // File Actions
    QAction* openAct;
    QList<QAction*> saveAsActs;
    QAction* printAct;
    QAction* quitAct;
    // View Actions
    QAction* showDockWidgetAct;
    QAction* showUserCoordinateAct;
    // Tools Actions
    QAction* optionAct;
    QAction* pickPenColorAct;
    QAction* clearScreenAct;
    QAction* restoreDefaultScreenAct;
    // Help Actions
    QAction* aboutSDI_PaintingAct;
    QAction* aboutQtAct;
    // Draw Actions
    QActionGroup* draw2DShapeActGroup;
    QActionGroup* draw3DShapeActGroup;
    QComboBox* triangleTypes;
    QSpinBox* penWidthBox;
    QSharedPointer<SDI_GeometricShape> activatedShape;
    QWeakPointer<SDI_GeometricShape> weakActivatedShape;
    //Private Functions
    void setupActions();
    void setupActionProperties(QAction* action,const QKeySequence& shortcut, const QString& toolTip, const QString& statusTip);
    void setupActionGroup(QAction* action, QActionGroup* actGroup);
    void setupActionGroupMapping(QActionGroup* actGroupSender, GraphicsMode mode);
    void setupMenuBar();
    void setupToolBars();
    void setupDockWidget();
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
    void changeGraphicsMode(GraphicsMode newMode);
    void showDockWidget(bool enable);
    void showMessage(const QString& message);
    void showSelectedShape(QWeakPointer<SDI_GeometricShape> shape);
    void enableUserCoordinate(bool enable);
    void restoreDefaultScreen();
    void translateShape(int xtrans, int ytrans);
    void scaleShape(double xscale, double yscale);
    void rotateShape(double degree, int centralPointID);
    void centralSymmetryShape();
    void OxSymmetryShape();
    void OySymmetryShape();
signals:
    void triangleTypeChanged(int triangleTypeID);
    void displayCoordinateStateChanged(DisplayCoordinateState newState);
};

#endif // SDI_MAINWINDOW_H
