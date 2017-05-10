#include "sdi_mainwindow.h"

void SDI_MainWindow::createActions()
{
    openAct = new QAction(QIcon(":/images/icons/openImg.png"), tr("Mở ảnh"), this);
    openAct->setShortcut(QKeySequence::Open);
    openAct->setStatusTip(tr("Mở ảnh để chỉnh sửa cơ bản"));
    QObject::connect(openAct, SIGNAL(triggered(bool)), this, SLOT(open()));

    foreach(QByteArray imageFormat, QImageWriter::supportedImageFormats())
    {
        QString text = tr("%1...").arg(QString(imageFormat).toUpper());
        QAction* saveAct = new QAction(text, this);
        saveAct->setData(imageFormat);
        QObject::connect(saveAct, SIGNAL(triggered(bool)), this, SLOT(save()));
        saveAsActs.append(saveAct);
    }

    printAct = new QAction(QIcon(":/images/icons/print.png"), tr("In ảnh"), this);
    printAct->setShortcut(QKeySequence::Print);
    printAct->setStatusTip(tr("In thành tập tin văn bản"));
    QObject::connect(printAct, SIGNAL(triggered(bool)), central2DWidget, SLOT(print()));

    quitAct = new QAction(QIcon(":/images/icons/exit.png"), tr("Thoát"), this);
    quitAct->setShortcut(QKeySequence::Quit);
    QObject::connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    showToolBarAct = new QAction(QIcon(":/images/icons/Letters.ico"), tr("Hiện thanh công cụ phụ"), this);
    showToolBarAct->setCheckable(true);
    showToolBarAct->setChecked(true);

    showDockWidgetAct = new QAction(QIcon(":/images/icons/Letters.ico"), tr("Hiện thanh công cụ chính"), this);
    showDockWidgetAct->setCheckable(true);
    showDockWidgetAct->setChecked(true);
    QObject::connect(showDockWidgetAct, SIGNAL(toggled(bool)), this, SLOT(showDockWidget(bool)));

    show2DModeAct = new QAction(QIcon(":/images/icons/oxy_coordinate.png"), tr("Chế độ đồ họa 2D"), this);

    show3DModeAct = new QAction(QIcon(":/images/icons/oxyz_coordinate.png"), tr("Chế độ đồ họa 3D"), this);

    optionAct = new QAction(QIcon(":/images/icons/Letters.ico"), tr("Tùy chỉnh..."), this);
    optionAct->setStatusTip("Tùy chỉnh giao diện, ngôn ngữ...");

    pickPenColorAct = new QAction(QIcon(":/images/icons/penColor.png"), tr("Màu vẽ"), this);
    pickPenColorAct->setStatusTip(tr("Chọn màu vẽ"));
    QObject::connect(pickPenColorAct, SIGNAL(triggered(bool)), this, SLOT(penColor()));
    QObject::connect(penWidthBox, SIGNAL(valueChanged(int)), this ,SLOT(penWidth(int)));

    clearScreenAct = new QAction(QIcon(":/images/icons/clearScreen.png"), tr("Xóa màn hình"), this);
    clearScreenAct->setShortcut(Qt::CTRL + Qt::Key_L);
    clearScreenAct->setStatusTip(tr("Xóa tất cả các hình đã vẽ"));
    clearScreenAct->setToolTip(tr("Ctrl+L"));
    QObject::connect(clearScreenAct, SIGNAL(triggered(bool)), central2DWidget, SLOT(clearImage()));

    aboutSDI_PaintingAct = new QAction(QIcon(":/images/icons/SDI_Basic_Painting.ico"), tr("Thông tin chương trình"), this);
    QObject::connect(aboutSDI_PaintingAct, SIGNAL(triggered()), this, SLOT(aboutSDI_Painting()));

    aboutQtAct = new QAction(QIcon(":/images/icons/QtIcon.png"), tr("Thông tin về Qt/Bản quyền"), this);
    QObject::connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    QAction* selectShapeAct{new QAction(QIcon(":/images/icons/scribble.png"), tr("Chọn hình"), this)};
    selectShapeAct->setShortcut(Qt::CTRL + Qt::Key_0);
    selectShapeAct->setToolTip(tr("Ctrl+0"));
    selectShapeAct->setStatusTip(tr("Chế độ chọn hình dựa trên tọa độ điểm được chỉ ra trên hệ trục tọa độ ..."));
    setupDrawAct(selectShapeAct);

    QAction* drawPointAct{new QAction(QIcon(":/images/icons/point.png"), tr("Điểm"), this)};
    drawPointAct->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_P);
    drawPointAct->setStatusTip(tr("Vẽ một điểm trên hệ trục, dùng chuột để chọn điểm vẽ hoặc dùng nút Thêm đối tượng để nhập vào thông số..."));
    //setupDrawAct(drawPointAct);

    QAction* drawLineAct{new QAction(QIcon(":/images/icons/line.png"), tr("Đoạn thẳng"), this)};
    drawLineAct->setShortcut(Qt::CTRL + Qt::Key_1);
    drawLineAct->setToolTip(tr("Ctrl+1"));
    drawLineAct->setStatusTip(tr("Vẽ một đoạn thẳng trên hệ trục, dùng chuột để chọn 2 điểm..."));
    setupDrawAct(drawLineAct);

    QAction* drawRectAct{new QAction(QIcon(":/images/icons/rect.png"), tr("Hình chữ nhật"), this)};
    drawRectAct->setShortcut(Qt::CTRL + Qt::Key_2);
    drawRectAct->setToolTip(tr("Ctrl+2"));
    drawRectAct->setStatusTip(tr("Vẽ một hình chữ nhật trên hệ trục, dùng chuột để chọn 2 điểm tạo nên đường chéo hình chữ nhật..."));
    setupDrawAct(drawRectAct);

    QAction* drawSquareAct{new QAction(QIcon(":/images/icons/square"), tr("Hình vuông"), this)};
    drawSquareAct->setShortcut(Qt::CTRL + Qt::Key_3);
    drawSquareAct->setToolTip(tr("Ctrl+3"));
    drawSquareAct->setStatusTip(tr("Vẽ một hình vuông trên hệ trục, dùng chuột để chọn 2 điểm tạo nên đường chéo hình vuông..."));
    setupDrawAct(drawSquareAct);

    QAction* drawParallelogramAct{new QAction(QIcon(":/images/icons/parallelogram.png"), tr("Hình bình hành"), this)};
    drawParallelogramAct->setShortcut(Qt::CTRL + Qt::Key_4);
    drawParallelogramAct->setToolTip(tr("Ctrl+4"));
    drawParallelogramAct->setStatusTip(tr("Vẽ một hình binh hành trên hệ trục, dùng chuột để chọn 4 điểm thuộc hình bình hành..."));
    setupDrawAct(drawParallelogramAct);


    QAction* drawCircleAct{new QAction(QIcon(":/images/icons/circle.png"), tr("Hình tròn"), this)};
    drawCircleAct->setShortcut(Qt::CTRL + Qt::Key_5);
    drawCircleAct->setToolTip(tr("Ctrl+5"));
    drawCircleAct->setStatusTip(tr("Vẽ một hình tròn trên hệ trục, dùng chuột để chọn tâm đường tròn và điểm thuộc đường tròn..."));
    setupDrawAct(drawCircleAct);

    QAction* drawTriangleAct{new QAction(QIcon(":/images/icons/triangle.png"), tr("Tam giác"), this)};
    drawTriangleAct->setShortcut(Qt::CTRL + Qt::Key_6);
    drawTriangleAct->setToolTip(tr("Ctrl+6"));
    drawTriangleAct->setStatusTip(tr("Vẽ một hình tam giác trên hệ trục, dùng chuột để chọn 3 điểm tạo nên hình tam giác..."));
    QObject::connect(drawTriangleAct, SIGNAL(toggled(bool)), triangleTypes, SLOT(setEnabled(bool)));
    setupDrawAct(drawTriangleAct);

    QSignalMapper* draw2DObjectMapper{new QSignalMapper(this)};
    for (int i{}; i < draw2DObjectActs.size(); i++)
    {
        draw2DObjectMapper->setMapping(draw2DObjectActs[i], i);
        QObject::connect(draw2DObjectActs[i], SIGNAL(toggled(bool)), draw2DObjectMapper, SLOT(map()));
    }
    QObject::connect(draw2DObjectMapper, SIGNAL(mapped(int)), central2DWidget , SLOT(setDraw2DObjectMode(int)));
    selectShapeAct->setChecked(true); // default draw mode
}

void SDI_MainWindow::createMenus()
{
    saveAsMenu = new QMenu(tr("Lưu thành..."), this);
    saveAsMenu->setIcon(QIcon(":/images/icons/save.png"));
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
    ToolsMenu->addSeparator();
    ToolsMenu->addAction(clearScreenAct);
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

    mainToolBar = addToolBar(tr("Tùy chỉnh vẽ"));
    mainToolBar->addAction(pickPenColorAct);
    mainToolBar->addWidget(new QLabel(tr("Bề dày nét vẽ"), this));
    penWidthBox->setRange(1, 10);
    mainToolBar->addWidget(penWidthBox);
    mainToolBar->addAction(clearScreenAct);

    mainToolBar = addToolBar(tr("Vẽ các đối tượng cơ bản"));
    mainToolBar->addActions(draw2DObjectActs);
    triangleTypes->addItem(tr("Thường"));
    triangleTypes->addItem(tr("Vuông cân"));
    triangleTypes->setDisabled(true);
    QObject::connect(triangleTypes, SIGNAL(currentIndexChanged(int)), central2DWidget, SLOT(setTriangleTypeID(int)));
    mainToolBar->addWidget(triangleTypes);

}

void SDI_MainWindow::createDockWidget()
{
    QObject::connect(mainToolsWidget, SIGNAL(changeGraphicsMode(int)), central2DWidget, SLOT(setGraphicsMode(int)));
    dockWidget->setWidget(mainToolsWidget);
    dockWidget->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    dockWidget->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, dockWidget);
}

void SDI_MainWindow::setupDrawAct(QAction *drawAct)
{
    drawAct->setCheckable(true);
    drawAct->setActionGroup(draw2DGroupActs);
    draw2DObjectActs.append(drawAct);
}

bool SDI_MainWindow::mayBeSave()
{
    if (central2DWidget->isModified())
    {
        QMessageBox::StandardButton returnButton;
        returnButton = QMessageBox::warning(this, tr("SDI Basic Painting"),
                                    tr("Thay dổi chưa được lưu lại.\n"
                                       "Bạn có muốn lưu lại ảnh trước khi thoát ?"), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (returnButton == QMessageBox::Save)
            return saveFile("png"); // default image format
        else if (returnButton == QMessageBox::Cancel)
            return false;
    }
    return true;
}

bool SDI_MainWindow::saveFile(const QByteArray &fileFormat)
{
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

          QString fileName = QFileDialog::getSaveFileName(this, tr("Lưu lại thành..."),
                                     initialPath,
                                     tr("%1 Files (*.%2);;All Files (*)")
                                     .arg(QString::fromLatin1(fileFormat.toUpper()))
                                     .arg(QString::fromLatin1(fileFormat)));
          if (fileName.isEmpty()) {
              return false;
          } else {
              return central2DWidget->saveImage(fileName, fileFormat.constData());
          }
}
void SDI_MainWindow::closeEvent(QCloseEvent *closeEvent)
{
    if (mayBeSave())
        closeEvent->accept();
    else
        closeEvent->ignore();
}

void SDI_MainWindow::open()
{
    if (mayBeSave())
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Mở tập tin"), QDir::currentPath(), tr("Common formats (*.jpeg *.jpg *.jp2 *.bmp);;"
                                                                                                        "Portable formats (*.pbm *.png *.pgm *.ppm);;"
                                                                                                        "Icon/Cursor format (*.ico *.icns *.cur);;"
                                                                                                        "Tagged formats (*.tif *.tiff)"
                                                                                                        "Wireless/Web/X11 formats (*.webp *.wbmp *.xbm *.xpm)"));
        if (!fileName.isEmpty())
            central2DWidget->openImage(fileName);
    }
}

void SDI_MainWindow::save()
{
    QAction* action = qobject_cast<QAction*>(sender());
    QByteArray fileFormat(action->data().toByteArray());
    saveFile(fileFormat);
}

void SDI_MainWindow::penColor()
{
    QColor newColor = QColorDialog::getColor(central2DWidget->penColor(), nullptr, tr("Chọn màu vẽ"));
    if (newColor.isValid())
        central2DWidget->setPenColor(newColor);
}

void SDI_MainWindow::penWidth(int newWidth)
{
    central2DWidget->setPenWidth(newWidth);
}

void SDI_MainWindow::aboutSDI_Painting()
{
    QString osInfo{"Linux"};
    if (QSysInfo::WindowsVersion >= QSysInfo::WV_WINDOWS7)
        osInfo = QString("Windows");
    QMessageBox::about(this, tr("Thông tin"),
                             tr("<h1> Đồ án Kỹ thuật Đồ họa</h1>"
                                "<p> Nhóm thực hiện: "
                                "   <ol>"
                                "       <li> Thiều Quang Tuấn - N14DCCN136</li>"
                                "       <li> Lê Hoàng Tùng - N14DCCN113 </li>"
                                "       <li> Hoàng Lê Anh Minh - N14DCCN138</li>"
                                "   </ol>"
                                "</p>"
                                "</p> Thông số kỹ thuật:"
                                "   <ul>"
                                "       <li> Tên chương trình: SDI Basic Painting</li>"
                                "       <li> Phiên bản dành cho %1 %2</li>"
                                "   </ul>"
                                "</p>").arg(osInfo).arg(QSysInfo::buildCpuArchitecture()));
}

void SDI_MainWindow::showDockWidget(bool enable)
{
    if (enable)
        restoreDockWidget(dockWidget);
    else
        removeDockWidget(dockWidget);
}

void SDI_MainWindow::showMessage(QString message)
{
    statusBar()->showMessage(message);
}

void SDI_MainWindow::showSelectedShape(SDI_GeometricShape *shape)
{
    activatedShape = shape;
    if (shape == nullptr)
        mainToolsWidget->setInfoBox("<b>Không định vị được</b>", "ZZZZZzzzzzZZZZZ");
    else
        mainToolsWidget->setInfoBox("<b>"+shape->getShapeName() + "</b>", shape->getShapeData());
}

void SDI_MainWindow::translateShape(int xtrans, int ytrans)
{
    if (activatedShape == nullptr)
        showSelectedShape(nullptr);
    else
    {
        activatedShape->translate(xtrans, ytrans);
        central2DWidget->drawObject(QPoint(0,0), 3);
        showSelectedShape(activatedShape);
    }
}

void SDI_MainWindow::scaleShape(double xscale, double yscale)
{
    if (activatedShape == nullptr)
        showSelectedShape(nullptr);
    else
    {
        activatedShape->scale(xscale, yscale);
        central2DWidget->drawObject(QPoint(0,0), 3);
        showSelectedShape(activatedShape);
    }
}

void SDI_MainWindow::rotateShape(double degree)
{
    if (activatedShape == nullptr)
        showSelectedShape(nullptr);
    else
    {
        activatedShape->rotate(degree);
        central2DWidget->drawObject(QPoint(0,0), 3);
        showSelectedShape(activatedShape);
    }
}

void SDI_MainWindow::centralSymmetryShape()
{
    if (activatedShape == nullptr)
        showSelectedShape(nullptr);
    else
    {
        activatedShape->originPosSymmetry();
        central2DWidget->drawObject(QPoint(0,0), 3);
        showSelectedShape(activatedShape);
    }
}

void SDI_MainWindow::OxSymmetryShape()
{
    if (activatedShape == nullptr)
        showSelectedShape(nullptr);
    else
    {
        activatedShape->OxSymmetry();
        central2DWidget->drawObject(QPoint(0,0), 3);
        showSelectedShape(activatedShape);
    }
}

void SDI_MainWindow::OySymmetryShape()
{
    if (activatedShape == nullptr)
        showSelectedShape(nullptr);
    else
    {
        activatedShape->OySymmetry();
        central2DWidget->drawObject(QPoint(0,0), 3);
        showSelectedShape(activatedShape);
    }
}

SDI_MainWindow::SDI_MainWindow(QWidget *parent)
    : QMainWindow(parent),
      dockWidget{new QDockWidget(this)},
      central2DWidget{new draw2DWidget(this)},
      mainToolsWidget{new leftToolsWidget(this)},
      draw2DGroupActs{new QActionGroup(this)},
      triangleTypes{ new QComboBox(this)},
      penWidthBox{new QSpinBox(this)}
{
    createActions();
    createMenus();
    createToolsBar();
    createDockWidget();
    setCentralWidget(central2DWidget);
    setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    setToolTipDuration(15);
    if (QSysInfo::WindowsVersion == QSysInfo::WV_WINDOWS10)
        setFont(QFont("Tahoma", 10));
    setWindowTitle("SDI Basic Painting");
    setWindowIcon(QIcon(":/images/icons/SDI_Basic_Painting.ico"));
    statusBar()->showMessage("Demo 0.4 - 04/05/2017");
    QObject::connect(central2DWidget, SIGNAL(mouseMoveTo(QString)),mainToolsWidget, SLOT(showPosition(QString)));
    QObject::connect(central2DWidget, SIGNAL(selectedShape(SDI_GeometricShape*)), this, SLOT(showSelectedShape(SDI_GeometricShape*)));
    QObject::connect(mainToolsWidget, SIGNAL(translateSelectedShape(int,int)), this, SLOT(translateShape(int,int)));
    QObject::connect(mainToolsWidget, SIGNAL(scaleSelectedShape(double,double)), this, SLOT(scaleShape(double,double)));
    QObject::connect(mainToolsWidget, SIGNAL(rotateSelectedShape(double)), this, SLOT(rotateShape(double)));
    QObject::connect(mainToolsWidget, SIGNAL(centralSymmetrySelectedShape()), this, SLOT(centralSymmetryShape()));
    QObject::connect(mainToolsWidget, SIGNAL(OxSymmetrySelectedShape()), this, SLOT(OxSymmetryShape()));
    QObject::connect(mainToolsWidget, SIGNAL(OySymmetrySelectedShape()), this, SLOT(OySymmetryShape()));
}

SDI_MainWindow::~SDI_MainWindow()
{

}
