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
#include <QWidget>
#include <QImage>
#include <QImageWriter>
#include <QMessageBox>
class SDI_MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit SDI_MainWindow(QWidget *parent = 0);
    ~SDI_MainWindow();
private:
    QMenu* FileMenu;
    QMenu* saveAsMenu;
    QMenu* ViewMenu;
    QMenu* ToolsMenu;
    QMenu* HelpMenu;
    QWidget* paintWidget;
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
    QAction* pickPenWidthAct;
    QAction* clearScreenAct;
    // Help Actions
    QAction* aboutSDI_PaintingAct;
    QAction* aboutQtAct;
    // Draw Actions
    QActionGroup* draw2DGroupActs;
    QList<QAction*> draw2DObjectActs;
    //Private Functions
    void createActions();
    void createMenus();
    void createToolsBar();
    void createDockWidget();
    void setupDrawAct(QAction* drawAct);
    bool mayBeSave();
    bool saveFile();
protected:
    virtual void closeEvent(QCloseEvent* closeEvent) override;
private slots:
    void aboutSDI_Painting();

};

#endif // SDI_MAINWINDOW_H
