#ifndef SDI_MAINWINDOW_H
#define SDI_MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QDockWidget>
#include <QStatusBar>
class SDI_MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QMenu* FileMenu;
    QMenu* saveAsMenu;
    QMenu* ViewMenu;
    QMenu* ToolsMenu;
    QMenu* HelpMenu;
    // File Actions
    QAction* openAct;
    QAction* saveAsAct;
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

public:
    explicit SDI_MainWindow(QWidget *parent = 0);
    ~SDI_MainWindow();
};

#endif // SDI_MAINWINDOW_H
