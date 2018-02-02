#include "sdi_mainwindow.h"

void SDI_MainWindow::createActions()
{
    openAct = new QAction(QIcon(":/images/icons/openImg.png"), tr("Mở ảnh"), this);
    openAct->setShortcut(QKeySequence::Open);
    openAct->setStatusTip(tr("Nạp một ảnh bất kỳ làm nền vẽ"));
    QObject::connect(openAct, SIGNAL(triggered(bool)), this, SLOT(open()));

    foreach(QByteArray imageFormat, QImageWriter::supportedImageFormats())
    {
        QString text = tr("%1...").arg(QString(imageFormat).toUpper());
        QAction* saveAct = new QAction(text, this);
        saveAct->setData(imageFormat);
        QObject::connect(saveAct, SIGNAL(triggered(bool)), this, SLOT(save()));
        saveAsActs.append(saveAct);
    }

    printAct = new QAction(QIcon(":/images/icons/print.png"), tr("Tạo file PDF"), this);
    printAct->setShortcut(QKeySequence::Print);
    printAct->setStatusTip(tr("In thành tập tin pdf"));
    QObject::connect(printAct, SIGNAL(triggered(bool)), central2DWidget, SLOT(print()));

    quitAct = new QAction(QIcon(":/images/icons/exit.png"), tr("Thoát"), this);
    quitAct->setShortcut(QKeySequence::Quit);
    QObject::connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    showDockWidgetAct = new QAction(QIcon(":/images/icons/add-dock.png"), tr("Hiện thanh công cụ chính"), this);
    showDockWidgetAct->setCheckable(true);
    showDockWidgetAct->setChecked(true);
    QObject::connect(showDockWidgetAct, SIGNAL(toggled(bool)), this, SLOT(showDockWidget(bool)));

    showUserCoordinateAct = new QAction(QIcon(":/images/icons/oxy_coordinate.png"), tr("Hiển thị hệ trục"), this);
    showUserCoordinateAct->setCheckable(true);
    showUserCoordinateAct->setChecked(true);
    QObject::connect(showUserCoordinateAct, SIGNAL(toggled(bool)), this, SLOT(enableUserCoordinate(bool)));

    optionAct = new QAction(QIcon(":/images/icons/Letters.ico"), tr("Tùy chỉnh..."), this);
    optionAct->setStatusTip("Tùy chỉnh giao diện, ngôn ngữ...(chưa bổ sung trong bản build này)");

    pickPenColorAct = new QAction(QIcon(":/images/icons/penColor.png"), tr("Màu vẽ"), this);
    pickPenColorAct->setStatusTip(tr("Chọn màu vẽ"));
    QObject::connect(pickPenColorAct, SIGNAL(triggered(bool)), this, SLOT(penColor()));
    QObject::connect(penWidthBox, SIGNAL(valueChanged(int)), this ,SLOT(penWidth(int)));

    clearScreenAct = new QAction(QIcon(":/images/icons/cleanObject.png"), tr("Xóa mọi đối tượng"), this);
    clearScreenAct->setShortcut(Qt::CTRL + Qt::Key_L);
    clearScreenAct->setStatusTip(tr("Xóa tất cả các hình đã vẽ"));
    clearScreenAct->setToolTip(tr("Ctrl+L"));
    QObject::connect(clearScreenAct, SIGNAL(triggered(bool)), central2DWidget, SLOT(clearImage()));

    restoreDefaultScreenAct = new QAction(QIcon(":/images/icons/clearScreen.png"), tr("Khôi phục nền mặc định"), this);
    QObject::connect(restoreDefaultScreenAct, SIGNAL(triggered(bool)), this, SLOT(restoreDefaultScreen()));

    aboutSDI_PaintingAct = new QAction(QIcon(":/images/icons/SDI_Basic_Painting.ico"), tr("Thông tin chương trình"), this);
    aboutSDI_PaintingAct->setShortcut(Qt::Key_F1);
    QObject::connect(aboutSDI_PaintingAct, SIGNAL(triggered()), this, SLOT(aboutSDI_Painting()));

    aboutQtAct = new QAction(QIcon(":/images/icons/QtIcon.png"), tr("Thông tin về Qt/Bản quyền"), this);
    QObject::connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    //--------------------------------------2D Action
    QAction* selectShapeAct{new QAction(QIcon(":/images/icons/scribble.png"), tr("Chọn hình"), this)};
    setupActionProperties(selectShapeAct, Qt::CTRL + Qt::Key_0, "Ctrl + 0", "Select a shape on the screen");
    setupActionGroup(selectShapeAct, draw2DShapeActGroup);

    QAction* drawLineAct{new QAction(QIcon(":/images/icons/line.png"), tr("Đoạn thẳng"), this)};
    setupActionProperties(drawLineAct, Qt::CTRL + Qt::Key_1, "Ctrl+1", "Draw a line on the screen");
    setupActionGroup(drawLineAct, draw2DShapeActGroup);

    QAction* drawRectAct{new QAction(QIcon(":/images/icons/rect.png"), tr("Hình chữ nhật"), this)};
    setupActionProperties(drawRectAct, Qt::CTRL + Qt::Key_2, "Ctrl+2", "Draw a rectangle on the screen");
    setupActionGroup(drawRectAct, draw2DShapeActGroup);

    QAction* drawSquareAct{new QAction(QIcon(":/images/icons/square"), tr("Hình vuông"), this)};
    setupActionProperties(drawSquareAct, Qt::CTRL + Qt::Key_3, "Ctrl+3", "Draw a square on the screen");
    setupActionGroup(drawSquareAct, draw2DShapeActGroup);

    QAction* drawParallelogramAct{new QAction(QIcon(":/images/icons/parallelogram.png"), tr("Hình bình hành"), this)};
    setupActionProperties(drawParallelogramAct, Qt::CTRL + Qt::Key_4, "Ctrl+4", "Draw a parallelogram on the screen");
    setupActionGroup(drawParallelogramAct, draw2DShapeActGroup);

    QAction* drawCircleAct{new QAction(QIcon(":/images/icons/circle.png"), tr("Hình tròn"), this)};
    setupActionProperties(drawCircleAct, Qt::CTRL + Qt::Key_5, "Ctrl+5", "Draw a circle on the screen");
    setupActionGroup(drawCircleAct, draw2DShapeActGroup);

    QAction* drawTriangleAct{new QAction(QIcon(":/images/icons/triangle.png"), tr("Tam giác"), this)};
    setupActionProperties(drawTriangleAct, Qt::CTRL + Qt::Key_6, "Ctrl+6", "Draw a triangle on the screen");
    QObject::connect(drawTriangleAct, SIGNAL(toggled(bool)), triangleTypes, SLOT(setEnabled(bool)));
    setupActionGroup(drawTriangleAct, draw2DShapeActGroup);

    // Mapping
    setupActionGroupMapping(draw2DShapeActGroup, GraphicsMode::GM_2D);


    //---------------------3D Actions-------------------------------
    QAction* drawParallelepipedAct{new QAction(QIcon(":/images/icons/cube.png"),tr("Hình hộp"), this)};
    drawParallelepipedAct->setStatusTip(tr("Vẽ hình hộp chữ nhật trên hệ trục, dùng chuột di chuyển để chọn 2 điểm tạo nên mặt đáy và di chuyển để xác định chiều cao<"));
    //setupDraw3DAct(drawParallelepipedAct);
    setupActionGroup(drawParallelepipedAct, draw3DShapeActGroup);

    QAction* drawPyramidAct{new QAction(QIcon(":/images/icons/pyramid.png"), tr("Hình chóp"), this)};
    drawPyramidAct->setStatusTip(tr("Vẽ hình chóp trên hệ trục, dùng chuột di chuyển để chọn 2 điểm tạo nên mặt đáy và di chuyển để xác định chiều cao"));
    //setupDraw3DAct(drawPyramidAct);
    setupActionGroup(drawPyramidAct, draw3DShapeActGroup);

    // Mapping
    setupActionGroupMapping(draw3DShapeActGroup, GraphicsMode::GM_3D);
}

void SDI_MainWindow::setupActionProperties(QAction *action, const QKeySequence &shortcut, const QString &toolTip, const QString &statusTip)
{
    action->setShortcut(shortcut);
    action->setToolTip(toolTip);
    action->setStatusTip(statusTip);
}

void SDI_MainWindow::setupActionGroup(QAction *action, QActionGroup *actGroup)
{
    action->setCheckable(true);
    action->setActionGroup(actGroup);
}

void SDI_MainWindow::setupActionGroupMapping(QActionGroup *actGroupSender, GraphicsMode mode)
{
    QList<QAction*> actionsList{actGroupSender->actions()};
    QSignalMapper* actionsMapper{new QSignalMapper(this)};
    for (int i{}; i < actionsList.size(); i++)
    {
        QObject::connect(actionsList.at(i), SIGNAL(toggled(bool)), actionsMapper, SLOT(map()));
        actionsMapper->setMapping(actionsList.at(i), i);
        actionsMapper->setMapping(actionsList.at(i), actionsList.at(i)->statusTip());
    }
    QObject::connect(actionsMapper, SIGNAL(mapped(QString)), this, SLOT(showMessage(QString)));
    switch (mode)
    {
    case GraphicsMode::GM_2D:
        QObject::connect(actionsMapper, SIGNAL(mapped(int)), central2DWidget, SLOT(setDraw2DObjectMode(int)));
        break;
    case GraphicsMode::GM_3D:
        QObject::connect(actionsMapper, SIGNAL(mapped(int)), central2DWidget, SLOT(setDraw3DObjectMode(int)));
        break;
    default:
        break;
    }
}

void SDI_MainWindow::createMenus()
{
    QMenu* saveAsMenu{new QMenu(tr("Lưu thành..."), this)};
    saveAsMenu->setIcon(QIcon(":/images/icons/save.png"));
    foreach(QAction* act, saveAsActs)
        saveAsMenu->addAction(act);

    QMenu* FileMenu{new QMenu(tr("&Tập tin"), this)};
    FileMenu->addAction(openAct);
    FileMenu->addMenu(saveAsMenu);
    FileMenu->addAction(printAct);
    FileMenu->addSeparator();
    FileMenu->addAction(quitAct);
    menuBar()->addMenu(FileMenu);

    QMenu* ViewMenu{new QMenu(tr("&Xem"), this)};
    ViewMenu->addAction(showDockWidgetAct);
    ViewMenu->addSeparator();
    ViewMenu->addAction(showUserCoordinateAct);
    menuBar()->addMenu(ViewMenu);

    QMenu* ToolsMenu{new QMenu(tr("&Công cụ"), this)};
    ToolsMenu->addAction(pickPenColorAct);
    ToolsMenu->addSeparator();
    ToolsMenu->addAction(clearScreenAct);
    ToolsMenu->addAction(restoreDefaultScreenAct);
    ToolsMenu->addAction(optionAct);
    menuBar()->addMenu(ToolsMenu);

    QMenu* HelpMenu{new QMenu(tr("T&hông tin"), this)};
    HelpMenu->addAction(aboutSDI_PaintingAct);
    HelpMenu->addAction(aboutQtAct);
    menuBar()->addMenu(HelpMenu);
}

void SDI_MainWindow::createToolsBar()
{
    // Top tool bars ------------------------------------------------------------------------------------------------------
    QToolBar* mainToolBar;
    mainToolBar = addToolBar(tr("Tâp tin"));
    mainToolBar->addAction(openAct);

    mainToolBar = addToolBar(tr("Tùy chỉnh vẽ"));
    mainToolBar->addAction(pickPenColorAct);
    mainToolBar->addSeparator();
    mainToolBar->addAction(clearScreenAct);
    mainToolBar->addSeparator();
    mainToolBar->addWidget(new QLabel(tr("Bề dày nét vẽ"), this));
    penWidthBox->setRange(1, 10);
    mainToolBar->addWidget(penWidthBox);

    // Left tool bars ------------------------------------------------------------------------------------------------------
    shape2DToolBar = new QToolBar(tr("Các đối tượng 2D"), this);
    shape2DToolBar->addActions(draw2DShapeActGroup->actions());
    shape2DToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    addToolBar(Qt::LeftToolBarArea, shape2DToolBar);

    triangleTypes->addItem(tr("Thường"));
    triangleTypes->addItem(tr("Vuông cân"));
    triangleTypes->setDisabled(true);
    QObject::connect(triangleTypes, SIGNAL(currentIndexChanged(int)), central2DWidget, SLOT(setTriangleTypeID(int)));
    //shape2DToolBar->addWidget(triangleTypes);

    shape3DToolsBar = new QToolBar(tr("Các đối tượng 3D"), this);
    shape3DToolsBar->addActions(draw3DShapeActGroup->actions());
    shape3DToolsBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    addToolBar(Qt::LeftToolBarArea, shape3DToolsBar);
}

void SDI_MainWindow::createDockWidget()
{
    QObject::connect(mainToolsWidget, SIGNAL(changeGraphicsMode(GraphicsMode)), central2DWidget, SLOT(setGraphicsMode(GraphicsMode)));
    QObject::connect(mainToolsWidget, SIGNAL(changeGraphicsMode(GraphicsMode)), this, SLOT(changeGraphicsMode(GraphicsMode)));
    dockWidget->setWidget(mainToolsWidget);
    dockWidget->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    dockWidget->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, dockWidget);
    //dockWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
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
        {
             central2DWidget->openImage(fileName);
             setWindowTitle(QString("SDI Basic Painting | %1[*]").arg(fileName));
        }
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
                                 "<p> SDI Basic Painting là một chương trình ứng dụng các thuật toán dựng hình cơ sở, các phép biến đổi hình học cho phép người sử dụng vẽ các hình 2D, 3D cơ bản. Thực hiện các thao tác như "
                                "chọn các hình đã vẽ, thực hiện biến đổi hình được chọn.</p> "
                                 "   <p> Nhóm thực hiện: "
                                 "   <ol>"
                                 "       <b><li> Thiều Quang Tuấn - N14DCCN136</li>"
                                 "       <li> Lê Hoàng Tùng - N14DCCN113 </li>"
                                 "       <li> Hoàng Lê Anh Minh - N14DCCN138</li>"
                                 "   </ol></p>"
                                 "   <p> Học viện Công nghệ bưu chính viễn thông <br> Lớp D14CQCN02-N</p>"
                                 "   <p> Giảng viên hướng dẫn: <b>Ths. Dương Thanh Thảo</b>"
                                 "   <p> Phiên bản dành cho %1 %2</p>"
                                 "   <p> Icons from <i>www.flaticon.com</p>").arg(osInfo).arg(QSysInfo::buildCpuArchitecture()));
}

void SDI_MainWindow::changeGraphicsMode(GraphicsMode newMode)
{
    if (newMode == GraphicsMode::GM_2D)
    {
        shape2DToolBar->setEnabled(true);
        shape3DToolsBar->setDisabled(true);
        showUserCoordinateAct->setIcon(QIcon(":/images/icons/oxy_coordinate.png"));
    }
    else
    {
        shape3DToolsBar->setEnabled(true);
        shape2DToolBar->setDisabled(true);
        showUserCoordinateAct->setIcon(QIcon(":/images/icons/oxyz_coordinate.png"));
    }
}

void SDI_MainWindow::showDockWidget(bool enable)
{
    if (enable)
        restoreDockWidget(dockWidget);
    else
        removeDockWidget(dockWidget);
}

void SDI_MainWindow::showMessage(const QString &message)
{
    //statusBar()->showMessage(message);
    modeToolTip->setText("<b>" + message + "</b>");
}

void SDI_MainWindow::showSelectedShape(QWeakPointer<SDI_GeometricShape> shape)
{
    weakActivatedShape = shape;
    activatedShape = weakActivatedShape.toStrongRef();
    if (activatedShape.isNull())
        mainToolsWidget->setInfoBox("<b>Không định vị được</b>", "ZZZZZzzzzzZZZZZ");
    else
        mainToolsWidget->setInfoBox("<b>"+activatedShape->getShapeName() + "</b>", activatedShape->getShapeData());
    activatedShape.clear();
}

void SDI_MainWindow::enableUserCoordinate(bool enable)
{
    if (enable)
        emit displayCoordinateStateChanged(DisplayCoordinateState::DCS_Show);
    else
        emit displayCoordinateStateChanged(DisplayCoordinateState::DCS_Hide);
}

void SDI_MainWindow::restoreDefaultScreen()
{
    setWindowTitle("SDI Basic Painting | untitled.png[*]");
    central2DWidget->resetBackground();
}

void SDI_MainWindow::translateShape(int xtrans, int ytrans)
{
    activatedShape = weakActivatedShape.toStrongRef();
    if (activatedShape.isNull())
        mainToolsWidget->setInfoBox("<b>Không định vị được</b>", "ZZZZZzzzzzZZZZZ");
    else
    {
        activatedShape->translate(xtrans, ytrans);
        central2DWidget->drawObject(QPoint(0,0), StateOfShape::SOS_AllExistentShapes);
        showSelectedShape(activatedShape.toWeakRef());
    }
}

void SDI_MainWindow::scaleShape(double xscale, double yscale)
{
    activatedShape = weakActivatedShape.toStrongRef();
    if (activatedShape.isNull())
        mainToolsWidget->setInfoBox("<b>Không định vị được</b>", "ZZZZZzzzzzZZZZZ");
    else
    {
        activatedShape->scale(xscale, yscale);
        central2DWidget->drawObject(QPoint(0,0), StateOfShape::SOS_AllExistentShapes);
        showSelectedShape(activatedShape.toWeakRef());
    }
}

void SDI_MainWindow::rotateShape(double degree, int centralPointID)
{
    activatedShape = weakActivatedShape.toStrongRef();
    if (activatedShape.isNull())
        mainToolsWidget->setInfoBox("<b>Không định vị được</b>", "ZZZZZzzzzzZZZZZ");
    else
    {
        activatedShape->rotate(degree, centralPointID);
        central2DWidget->drawObject(QPoint(0,0), StateOfShape::SOS_AllExistentShapes);
        showSelectedShape(activatedShape.toWeakRef());
    }
}

void SDI_MainWindow::centralSymmetryShape()
{
    activatedShape = weakActivatedShape.toStrongRef();
    if (activatedShape.isNull())
        mainToolsWidget->setInfoBox("<b>Không định vị được</b>", "ZZZZZzzzzzZZZZZ");
    else
    {
        activatedShape->originPosSymmetry();
        central2DWidget->drawObject(QPoint(0,0), StateOfShape::SOS_AllExistentShapes);
        showSelectedShape(activatedShape.toWeakRef());
    }
}

void SDI_MainWindow::OxSymmetryShape()
{
    activatedShape = weakActivatedShape.toStrongRef();
    if (activatedShape.isNull())
        mainToolsWidget->setInfoBox("<b>Không định vị được</b>", "ZZZZZzzzzzZZZZZ");
    else
    {
        activatedShape->OxSymmetry();
        central2DWidget->drawObject(QPoint(0,0), StateOfShape::SOS_AllExistentShapes);
        showSelectedShape(activatedShape.toWeakRef());
    }
}

void SDI_MainWindow::OySymmetryShape()
{
    activatedShape = weakActivatedShape.toStrongRef();
    if (activatedShape.isNull())
        mainToolsWidget->setInfoBox("<b>Không định vị được</b>", "ZZZZZzzzzzZZZZZ");
    else
    {
        activatedShape->OySymmetry();
        central2DWidget->drawObject(QPoint(0,0), StateOfShape::SOS_AllExistentShapes);
        showSelectedShape(activatedShape.toWeakRef());
    }
}

SDI_MainWindow::SDI_MainWindow(QWidget *parent)
    : QMainWindow(parent),
      dockWidget{new QDockWidget(this)},
      central2DWidget{new draw2DWidget(this)},
      mainToolsWidget{new leftToolsWidget(this)},
      modeToolTip{new QLabel("<b>Chọn chế độ đồ họa 2D hoặc 3D để bắt đầu phiên làm việc ... </b>")},      draw2DShapeActGroup{new QActionGroup(this)},
      draw3DShapeActGroup{new QActionGroup(this)},
      triangleTypes{ new QComboBox(this)},
      penWidthBox{new QSpinBox(this)},
      activatedShape{nullptr}
{
    createActions();
    createMenus();
    createToolsBar();
    createDockWidget();
    setCentralWidget(central2DWidget);
    setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    if (QSysInfo::WindowsVersion >= QSysInfo::WV_WINDOWS7)
        setFont(QFont("Segoe UI", 12));
    setWindowTitle("SDI Basic Painting | untitled.png[*]");
    setWindowIcon(QIcon(":/images/icons/SDI_Basic_Painting.ico"));
    modeToolTip->setMinimumSize(modeToolTip->sizeHint());
    statusBar()->addWidget(modeToolTip,1);
    // connect draw2DWidget signals with other SDI_MainWindow components slots
    QObject::connect(central2DWidget, SIGNAL(modificationChanged(bool)), this , SLOT(setWindowModified(bool)));
    QObject::connect(central2DWidget, &draw2DWidget::mouseMoveTo,mainToolsWidget, &leftToolsWidget::showMousePosition);
    QObject::connect(central2DWidget, SIGNAL(selectedShape(QWeakPointer<SDI_GeometricShape>)), this, SLOT(showSelectedShape(QWeakPointer<SDI_GeometricShape>)));
    QObject::connect(central2DWidget, SIGNAL(graphicModeChanged(QString)), this, SLOT(showMessage(QString)));

    // connect leftToolsWidget signals with draw2DWidget slots
    QObject::connect(mainToolsWidget, SIGNAL(translateSelectedShape(int,int)), this, SLOT(translateShape(int,int)));
    QObject::connect(mainToolsWidget, SIGNAL(scaleSelectedShape(double,double)), this, SLOT(scaleShape(double,double)));
    QObject::connect(mainToolsWidget, SIGNAL(rotateSelectedShape(double,int)), this, SLOT(rotateShape(double, int)));
    QObject::connect(mainToolsWidget, SIGNAL(centralSymmetrySelectedShape()), this, SLOT(centralSymmetryShape()));
    QObject::connect(mainToolsWidget, SIGNAL(OxSymmetrySelectedShape()), this, SLOT(OxSymmetryShape()));
    QObject::connect(mainToolsWidget, SIGNAL(OySymmetrySelectedShape()), this, SLOT(OySymmetryShape()));

    // connect SDI_MainWindow signals with other components slots
    QObject::connect(this, SIGNAL(displayCoordinateStateChanged(DisplayCoordinateState)), central2DWidget, SLOT(setDisplayCoordinateMode(DisplayCoordinateState)));
}

SDI_MainWindow::~SDI_MainWindow()
{

}
