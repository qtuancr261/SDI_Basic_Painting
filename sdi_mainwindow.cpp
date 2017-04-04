#include "sdi_mainwindow.h"

void SDI_MainWindow::createActions()
{
    openAct = new QAction(QIcon(":/images/icons/Letters.ico"), tr("Mở ảnh"), this);
    openAct->setShortcut(QKeySequence::Open);
    openAct->setStatusTip(tr("Mở ảnh để chỉnh sửa cơ bản"));

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
    printAct->setStatusTip(tr("In thành tập tin văn bản"));

    quitAct = new QAction(QIcon(":/images/icons/Letters.ico"), tr("Thoát"), this);
    quitAct->setShortcut(QKeySequence::Quit);
    QObject::connect(quitAct, SIGNAL(triggered()), qApp, SLOT(quit()));

    showToolBarAct = new QAction(QIcon(":/images/icons/Letters.ico"), tr("Hiện thanh công cụ phụ"), this);
    showToolBarAct->setCheckable(true);
    showToolBarAct->setChecked(true);

    showDockWidgetAct = new QAction(QIcon(":/images/icons/Letters.ico"), tr("Hiện thanh công cụ chính"), this);
    showDockWidgetAct->setCheckable(true);
    showDockWidgetAct->setChecked(true);
    QObject::connect(showDockWidgetAct, SIGNAL(toggled(bool)), this, SLOT(showDockWidget(bool)));

    show2DModeAct = new QAction(QIcon(":/images/icons/Letters.ico"), tr("Chế độ đồ họa 2D"), this);

    show3DModeAct = new QAction(QIcon(":/images/icons/Letters.ico"), tr("Chế độ đồ họa 3D"), this);

    optionAct = new QAction(QIcon(":/images/icons/Letters.ico"), tr("Tùy chỉnh..."), this);
    optionAct->setStatusTip("Tùy chỉnh giao diện, ngôn ngữ...");

    pickPenColorAct = new QAction(QIcon(":/images/icons/Letters.ico"), tr("Chọn màu vẽ"), this);
    pickPenColorAct->setStatusTip(tr("Chọn màu vẽ"));
    pickPenWidthAct = new QAction(QIcon(":/images/icons/Letters.ico"), tr("Chọn độ dày của nét vẽ"), this);

    clearScreenAct = new QAction(QIcon(":/images/icons/Letters.ico"), tr("Xóa màn hình"), this);

    aboutSDI_PaintingAct = new QAction(QIcon(":/images/icons/Letters.ico"), tr("SDI Basic Painting"), this);
    QObject::connect(aboutSDI_PaintingAct, SIGNAL(triggered()), this, SLOT(aboutSDI_Painting()));

    aboutQtAct = new QAction(QIcon(":/images/icons/Letters.ico"), tr("Thông tin về Qt/Bản quyền"), this);
    QObject::connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    QAction* drawPointAct{new QAction(QIcon(":/images/icons/Letters.ico"), tr("Vẽ điểm"), this)};
    drawPointAct->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_P);
    drawPointAct->setStatusTip(tr("Vẽ một điểm trên hệ trục, dùng chuột để chọn điểm vẽ hoặc dùng nút Thêm đối tượng để nhập vào thông số..."));
    setupDrawAct(drawPointAct);

    QAction* drawLineAct{new QAction(QIcon(":/images/icons/Letters.ico"), tr("Vẽ đoạn thẳng"), this)};
    drawLineAct->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_L);
    drawLineAct->setStatusTip(tr("Vẽ một đoạn thẳng trên hệ trục, dùng chuột để chọn 2 điểm mút hoặc dùng Thêm đối tượng để nhập vào thông số..."));
    setupDrawAct(drawLineAct);

    QAction* drawRectAct{new QAction(QIcon(":/images/icons/Letters.ico"), tr("Vẽ hình chữ nhật"), this)};
    drawRectAct->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_R);
    drawRectAct->setStatusTip(tr("Vẽ một hình chữ nhật trên hệ trục, dùng chuột để chọn 2 điểm tạo nên đường chéo hoặc dùng Thêm đối tượng để nhập vào thông số..."));
    setupDrawAct(drawRectAct);

    QAction* drawSquareAct{new QAction(QIcon(":/images/icons/Letters.ico"), tr("Vẽ hình vuông"), this)};
    drawSquareAct->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_S);
    drawSquareAct->setStatusTip(tr("Vẽ một hình vuông trên hệ trục, dùng chuột để chọn 2 điểm tạo nên đường chéo hoặc dùng Thêm đối tượng để nhập vào thông số..."));
    setupDrawAct(drawSquareAct);

    QAction* drawParallelogramAct{new QAction(QIcon(":/images/icons/Letters.ico"), tr("Vẽ hình bình hảnh"), this)};
    drawParallelogramAct->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_S);
    drawParallelogramAct->setStatusTip(tr("Vẽ một hình binh hành trên hệ trục, dùng chuột để chọn 3 điểm tạo nên đường chéo hoặc dùng Thêm đối tượng để nhập vào thông số..."));
    setupDrawAct(drawParallelogramAct);


    QAction* drawCircleAct{new QAction(QIcon(":/images/icons/Letters.ico"), tr("Vẽ hình tròn"), this)};
    drawCircleAct->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_C);
    drawCircleAct->setStatusTip(tr("Vẽ một hình tròn trên hệ trục, dùng chuột để chọn tâm đường tròn và điểm thuộc đường tròn hoặc dùng Thêm đối tượng để nhập vào thông số..."));
    setupDrawAct(drawCircleAct);

    QAction* drawTriangleAct{new QAction(QIcon(":/images/icons/Letters.ico"), tr("Vẽ hình tam giác"), this)};
    drawTriangleAct->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_T);
    drawTriangleAct->setStatusTip(tr("Vẽ một hình tam giác trên hệ trục, dùng chuột để chọn 3 điểm tạo nên hình tam giác hoặc dùng Thêm đối tượng để nhập vào thông số..."));
    QObject::connect(drawTriangleAct, SIGNAL(toggled(bool)), triangleTypes, SLOT(setEnabled(bool)));
    setupDrawAct(drawTriangleAct);

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
    ViewMenu->addAction(showDockWidgetAct);
    ViewMenu->addAction(showToolBarAct);
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
    QToolBar* mainToolBar;
    mainToolBar = addToolBar(tr("Tâp tin"));
    mainToolBar->addAction(openAct);
    mainToolBar->addAction(printAct);

    mainToolBar = addToolBar(tr("Vẽ các đối tượng cơ bản"));
    mainToolBar->addActions(draw2DObjectActs);
    mainToolBar->addSeparator();
    triangleTypes->addItem(tr("Tam giác thường"));
    triangleTypes->addItem(tr("Tam giác cân"));
    triangleTypes->addItem(tr("Tam giác đều"));
    triangleTypes->setDisabled(true);
    mainToolBar->addWidget(triangleTypes);

    mainToolBar = addToolBar(tr("Tùy chỉnh vẽ"));
    mainToolBar->addAction(pickPenColorAct);
    mainToolBar->addSeparator();
    mainToolBar->addWidget(new QLabel(tr("Bề dày nét vẽ"), this));
    penWidthBox->setRange(1, 10);
    mainToolBar->addWidget(penWidthBox);

}

void SDI_MainWindow::createDockWidget()
{
    QGroupBox* leftToolsBox{new QGroupBox(tr("Thanh công cụ chính"), this)};
    leftToolsWidget* widget{new leftToolsWidget(this)};
    QVBoxLayout* leftToolsBoxLayout{new QVBoxLayout(leftToolsBox)};
    leftToolsBoxLayout->addWidget(widget);
    leftToolsBox->setLayout(leftToolsBoxLayout);
    leftSideDockWidget->setWidget(leftToolsBox);
    leftSideDockWidget->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    leftSideDockWidget->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, leftSideDockWidget);
}

void SDI_MainWindow::setupDrawAct(QAction *drawAct)
{
    drawAct->setCheckable(true);
    drawAct->setActionGroup(draw2DGroupActs);
    draw2DObjectActs.append(drawAct);
}

bool SDI_MainWindow::mayBeSave()
{
    return true;
}

bool SDI_MainWindow::saveFile()
{
    return true;
}

void SDI_MainWindow::closeEvent(QCloseEvent *closeEvent)
{
    closeEvent->accept();
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

void SDI_MainWindow::showDockWidget(bool enable)
{
    if (enable)
        restoreDockWidget(leftSideDockWidget);
    else
        removeDockWidget(leftSideDockWidget);
}

SDI_MainWindow::SDI_MainWindow(QWidget *parent)
    : QMainWindow(parent),
      paintWidget(new QWidget(this)),
      draw2DGroupActs{new QActionGroup(this)},
      triangleTypes{ new QComboBox(this)},
      penWidthBox{new QSpinBox(this)},
      leftSideDockWidget{new QDockWidget(this)}
{
    createActions();
    createMenus();
    createToolsBar();
    createDockWidget();
    setCentralWidget(paintWidget);
    setFont(QFont("Tahoma", 10));
    setWindowTitle("SDI Basic Painting");
    statusBar()->showMessage("Khởi tạo chương trình");
}

SDI_MainWindow::~SDI_MainWindow()
{

}
