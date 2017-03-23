#include "sdi_mainwindow.h"

void SDI_MainWindow::createActions()
{
    openAct = new QAction(QIcon(":/images/icons/Letters.ico"), tr("Mở ảnh"), this);
    openAct->setShortcut(QKeySequence::Open);

    foreach(QByteArray imageFormat, QImageWriter::supportedImageFormats())
    {
        QString text = tr("%1...").arg(QString(imageFormat).toUpper());
        QAction* saveAct = new QAction(text, this);
        saveAct->setData(imageFormat);
        //connect
        saveAsActs.append(saveAct);
    }

    printAct = new QAction(QIcon(":/images/icons/Letters.ico"), tr("In ảnh"), this);
    printAct->setShortcut(QKeySequence::Print);

    quitAct = new QAction(QIcon(":/images/icons/Letters.ico"), tr("Thoát"), this);
    quitAct->setShortcut(QKeySequence::Quit);
    QObject::connect(quitAct, SIGNAL(triggered()), qApp, SLOT(quit()));

    showToolBarAct = new QAction(QIcon(":/images/icons/Letters.ico"), tr("Hiện thanh công cụ phụ"), this);
    showToolBarAct->setCheckable(true);
    showToolBarAct->setChecked(true);

    showDockWidgetAct = new QAction(QIcon(":/images/icons/Letters.ico"), tr("Hiện thanh công cụ chính"), this);
    showDockWidgetAct->setCheckable(true);
    showDockWidgetAct->setChecked(true);

    show2DModeAct = new QAction(QIcon(":/images/icons/Letters.ico"), tr("Chế độ đồ họa 2D"), this);

    show3DModeAct = new QAction(QIcon(":/images/icons/Letters.ico"), tr("Chế độ đồ họa 3D"), this);

    optionAct = new QAction(QIcon(":/images/icons/Letters.ico"), tr("Tùy chỉnh..."), this);
    optionAct->setStatusTip("Tùy chỉnh giao diện, ngôn ngữ...");

    pickPenColorAct = new QAction(QIcon(":/images/icons/Letters.ico"), tr("Chọn màu vẽ"), this);

    pickPenWidthAct = new QAction(QIcon(":/images/icons/Letters.ico"), tr("Chọn độ dày của nét vẽ"), this);

    clearScreenAct = new QAction(QIcon(":/images/icons/Letters.ico"), tr("Xóa màn hình"), this);

    aboutSDI_PaintingAct = new QAction(QIcon(":/images/icons/Letters.ico"), tr("SDI Basic Painting"), this);
    QObject::connect(aboutSDI_PaintingAct, SIGNAL(triggered()), this, SLOT(aboutSDI_Painting()));

    aboutQtAct = new QAction(QIcon(":/images/icons/Letters.ico"), tr("Thông tin về Qt/Bản quyền"), this);
    QObject::connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

}

void SDI_MainWindow::createMenus()
{
    saveAsMenu = new QMenu(tr("Lưu thành..."), this);
    foreach(QAction* act, saveAsActs)
        saveAsMenu->addAction(act);

    FileMenu = new QMenu(tr("Tập tin"), this);
    FileMenu->addAction(openAct);
    FileMenu->addMenu(saveAsMenu);
    FileMenu->addAction(printAct);
    FileMenu->addSeparator();
    FileMenu->addAction(quitAct);
    menuBar()->addMenu(FileMenu);

    ViewMenu = new QMenu(tr("Xem"), this);
    ViewMenu->addAction(showToolBarAct);
    ViewMenu->addAction(showDockWidgetAct);
    ViewMenu->addSeparator();
    ViewMenu->addAction(show2DModeAct);
    ViewMenu->addAction(show3DModeAct);
    menuBar()->addMenu(ViewMenu);

    ToolsMenu = new QMenu(tr("Công cụ"), this);
    ToolsMenu->addAction(pickPenColorAct);
    ToolsMenu->addAction(pickPenWidthAct);
    ToolsMenu->addSeparator();
    ToolsMenu->addAction(optionAct);
    menuBar()->addMenu(ToolsMenu);

    HelpMenu = new QMenu(tr("Trợ giúp"), this);
    HelpMenu->addAction(aboutSDI_PaintingAct);
    HelpMenu->addAction(aboutQtAct);
    menuBar()->addMenu(HelpMenu);
}

void SDI_MainWindow::createToolsBar()
{

}

void SDI_MainWindow::createDockWidget()
{

}

bool SDI_MainWindow::mayBeSave()
{

}

bool SDI_MainWindow::saveFile()
{

}

void SDI_MainWindow::closeEvent(QCloseEvent *closeEvent)
{

}

void SDI_MainWindow::aboutSDI_Painting()
{
    QMessageBox::information(this, tr("Thông tin đồ án SDI Basic Painting"),
                             tr("<h1> Đồ án Kỹ thuật Đồ họa</h1>"
                                "<p> Nhóm thực hiện: "
                                "   <ol>"
                                "       <li> Thiều Quang Tuấn - N14DCCN136</li>"
                                "       <li> Lê Hoàng Tùng - N14DCCN113 </li>"
                                "       <li> Hoàng Lê Anh Minh - N14DCCN138</li>"
                                "   </ol>"
                                "</p>"));
}

SDI_MainWindow::SDI_MainWindow(QWidget *parent)
    : QMainWindow(parent),
      paintWidget(new QWidget(this))
{
    createActions();
    createMenus();
    setCentralWidget(paintWidget);
    setWindowTitle("SDI Basic Painting");
    statusBar()->showMessage("Khởi tạo chương trình");
}

SDI_MainWindow::~SDI_MainWindow()
{

}
