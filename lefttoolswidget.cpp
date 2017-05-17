#include "lefttoolswidget.h"

leftToolsWidget::leftToolsWidget(QWidget *parent) : QWidget(parent)
{
    setupGUI();
    if (QSysInfo::WindowsVersion >= QSysInfo::WV_WINDOWS7)
        setFont(QFont("Segoe UI", 11));
}

void leftToolsWidget::setInfoBox(QString shapeName, QString shapeData)
{
    shapeNameLabel->setText(shapeName);
    shapeDataTEdit->setText(shapeData);
}

void leftToolsWidget::setupGUI()
{
    // select graphic mode
    graphic2DMode = new QPushButton(QIcon(":/images/icons/oxy_coordinate.png"), tr("Hệ trục tọa độ Oxy"), this);
    setDrawModeButtonStyle(graphic2DMode);
    QObject::connect(graphic2DMode, SIGNAL(toggled(bool)), this, SLOT(decideNewGraphicMode()));
    graphic3DMode = new QPushButton(QIcon(":/images/icons/oxyz_coordinate.png"), tr("Hệ trục tọa độ Oxyz"), this);
    setDrawModeButtonStyle(graphic3DMode);
    QObject::connect(graphic3DMode, SIGNAL(toggled(bool)), this, SLOT(decideNewGraphicMode()));

    //helpMode = new QPushButton(QIcon(":/images/icons/about.png"), tr("Giới thiệu"), this);
    //setDrawModeButtonStyle(helpMode);
    QVBoxLayout* selectModeLayout{new QVBoxLayout(this)};
    selectModeLayout->addWidget(graphic2DMode);
    selectModeLayout->addWidget(graphic3DMode);
    //selectModeLayout->addWidget(helpMode);
    QGroupBox* selectModeGroupBox{new QGroupBox(tr("CHỌN CHẾ ĐỘ"), this)};
    selectModeGroupBox->setLayout(selectModeLayout);
    //----------------------------------------------------------------------
    // translate mode
    OxTranslateSlider = new QSlider(Qt::Horizontal, this);
    OxTranslateBox = new QSpinBox(this);
    setSlider_BoxSytle(OxTranslateSlider, OxTranslateBox, -500, 500);
    OyTranslateSlider = new QSlider(Qt::Horizontal, this);
    OyTranslateBox = new QSpinBox(this);
    setSlider_BoxSytle(OyTranslateSlider, OyTranslateBox, -500, 500);
    doTranslate = new QPushButton(tr("Tịnh tiến đối tượng"), this);
    doTranslate->setAutoRepeat(true);
    QObject::connect(doTranslate, SIGNAL(pressed()), this, SLOT(takeTranslateParameters()));
    QGridLayout* translationLayout{new QGridLayout(this)};
    translationLayout->addWidget(new QLabel("Ox"), 0, 0);
    translationLayout->addWidget(OxTranslateSlider, 0, 1);
    translationLayout->addWidget(OxTranslateBox, 0, 2);
    translationLayout->addWidget(new QLabel("Oy"), 1, 0);
    translationLayout->addWidget(OyTranslateSlider, 1, 1);
    translationLayout->addWidget(OyTranslateBox, 1, 2);
    translationLayout->addWidget(doTranslate, 2, 0, 1, 3);
    translationLayout->addItem(new QSpacerItem(10, 10, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding),3,0,1,3);
    QGroupBox* translationGroupBox{new QGroupBox()};
    translationGroupBox->setLayout(translationLayout);
    //--------------------------------------------------------------------
    // rotate mode
    rotateSlider = new QSlider(Qt::Horizontal, this);
    rotateBox = new QSpinBox(this);
    setSlider_BoxSytle(rotateSlider, rotateBox, -360, 360);
    shapeCentralRotate = new QRadioButton(tr("Tâm đối tượng"), this);
    shapeCentralRotate->setAutoExclusive(true);
    userOriginPosRotate = new QRadioButton( tr("Gốc tọa độ"), this);
    userOriginPosRotate->setAutoExclusive(true);

    doRotate = new QPushButton(tr("Xoay đối tượng"));
    doRotate->setAutoRepeat(true);
    QObject::connect(doRotate, SIGNAL(clicked(bool)), this, SLOT(takeRotateParameters()));
    QGridLayout* rotateLayout{new QGridLayout(this)};
    rotateLayout->addWidget(new QLabel(tr("Góc xoay: ")), 0, 0);
    rotateLayout->addWidget(rotateSlider, 0, 1);
    rotateLayout->addWidget(rotateBox, 0, 2);
    rotateLayout->addWidget(new QLabel(tr("Tâm xoay: ")), 1, 0);
    rotateLayout->addWidget(userOriginPosRotate, 1, 1, 1, 2);
    rotateLayout->addWidget(shapeCentralRotate, 2, 1, 1, 2);
    rotateLayout->addWidget(doRotate, 3, 0, 1, 3);
    rotateLayout->addItem(new QSpacerItem(10, 10, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding),2,0,1,3);
    QGroupBox* rotateGroupBox{new QGroupBox()};
    rotateGroupBox->setLayout(rotateLayout);
    //---------------------------------------------------------------------
    //symmetry mode
    centralSymmetry = new QRadioButton(tr("Đối xứng qua gốc tọa độ"), this);
    centralSymmetry->setAutoExclusive(true);
    OxSymmetry = new QRadioButton(tr("Đối xứng qua Ox"), this);
    OxSymmetry->setAutoExclusive(true);
    OySymmetry = new QRadioButton(tr("Đối xứng qua Oy"), this);
    OySymmetry->setAutoExclusive(true);
    doSymmetry = new QPushButton(tr("Lấy đối xứng đối tượng"));
    doSymmetry->setAutoRepeat(true);
    QObject::connect(doSymmetry, SIGNAL(clicked(bool)), this, SLOT(takeSymmetryParameters()));
    QVBoxLayout* symmetryLayout{new QVBoxLayout()};
    symmetryLayout->addWidget(centralSymmetry);
    symmetryLayout->addWidget(OxSymmetry);
    symmetryLayout->addWidget(OySymmetry);
    symmetryLayout->addWidget(doSymmetry);
    symmetryLayout->addItem(new QSpacerItem(10, 10, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    QGroupBox* symmetryGroupBox{new QGroupBox()};
    symmetryGroupBox->setLayout(symmetryLayout);
    //---------------------------------------------------------------------
    // zoom mode
    zoomBox = new QDoubleSpinBox(this);
    zoomBox->setRange(0.0, 5.0);
    zoomBox->setSingleStep(0.1);
    zoomBox->setSuffix("x");
    doZoom = new QPushButton(tr("Biến đồi đối tượng"), this);
    doZoom->setAutoRepeat(true);
    QObject::connect(doZoom, SIGNAL(clicked(bool)), this, SLOT(takeScaleParameters()));
    QGridLayout* zoomLayout{new QGridLayout(this)};
    zoomLayout->addWidget(new QLabel(tr("Hệ số tỉ lệ")), 0, 0);
    zoomLayout->addWidget(zoomBox, 0, 1);
    zoomLayout->addWidget(doZoom, 1,0,1,2);
    zoomLayout->addItem(new QSpacerItem(10, 10, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding),2,0,1,2);
    QGroupBox* zoomGroupBox{new QGroupBox()};
    zoomGroupBox->setLayout(zoomLayout);
    //---------------------------------------------------------------------
    // geometric shape infomation
    shapeNameLabel = new QLabel("Noname");
    shapeDataTEdit = new QTextEdit();
    shapeDataTEdit->setReadOnly(true);
    shapeDataTEdit->setText("Read-Only");
    QVBoxLayout* infoLayout{new QVBoxLayout(this)};
    shapeNameLabel->setAlignment(Qt::AlignCenter);
    infoLayout->addWidget(shapeNameLabel);
    infoLayout->addWidget(shapeDataTEdit);
    QGroupBox* infoGroupBox{new QGroupBox(tr("ĐỐI TƯỢNG ĐANG ĐƯỢC CHỌN"))};
    infoGroupBox->setLayout(infoLayout);

    positionLabel = new QLabel("Tọa độ");
    positionLabel->setAlignment(Qt::AlignCenter);
    //---------------------------------------------------------------------
    QToolBox* transformBox{new QToolBox(this)};
    transformBox->addItem(translationGroupBox, "PHÉP TỊNH TIẾN");
    transformBox->setItemIcon(0, QIcon(":/images/icons/move.png"));
    transformBox->addItem(rotateGroupBox, "PHÉP XOAY");
    transformBox->setItemIcon(1, QIcon(":/images/icons/rotate.png"));
    transformBox->addItem(symmetryGroupBox, "PHÉP ĐỐI XỨNG");
    transformBox->setItemIcon(2, QIcon(":/images/icons/symmetry.png"));
    transformBox->addItem(zoomGroupBox, "PHÉP BIẾN ĐỔI ĐỒNG DẠNG");
    transformBox->setItemIcon(3, QIcon(":/images/icons/zoom.png"));
    transformBox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    QSpacerItem* verticalSpacer{new QSpacerItem(10, 10, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding)};
    QVBoxLayout* mainLayout{new QVBoxLayout(this)};
    mainLayout->addWidget(selectModeGroupBox);
    mainLayout->addWidget(transformBox);
    mainLayout->addWidget(infoGroupBox);
    mainLayout->addWidget(positionLabel);
    mainLayout->addItem(verticalSpacer);
    setLayout(mainLayout);
}

void leftToolsWidget::setDrawModeButtonStyle(QPushButton *button)
{
    button->setIconSize(QSize(32, 32));
    button->setCheckable(true);
    button->setAutoExclusive(true);
}

void leftToolsWidget::setSlider_BoxSytle(QSlider *slider, QSpinBox *box, int minValue, int MaxValue)
{
    slider->setRange(minValue, MaxValue);
    box->setRange(minValue, MaxValue);
    QObject::connect(slider, SIGNAL(valueChanged(int)), box, SLOT(setValue(int)));
    QObject::connect(box, SIGNAL(valueChanged(int)), slider, SLOT(setValue(int)));
}

void leftToolsWidget::decideNewGraphicMode()
{
    if (graphic2DMode->isChecked())
        emit changeGraphicsMode(2);
    else if (graphic3DMode->isChecked())
        emit changeGraphicsMode(3);
}

void leftToolsWidget::takeTranslateParameters()
{
    emit translateSelectedShape(OxTranslateBox->value(), OyTranslateBox->value());
}

void leftToolsWidget::takeScaleParameters()
{
    emit scaleSelectedShape(zoomBox->value(), zoomBox->value());
}

void leftToolsWidget::takeRotateParameters()
{
    if (shapeCentralRotate->isChecked())
        emit rotateSelectedShape(rotateBox->value(), 1); // 1 : using SDI_GeometricShape::centralPoint
    else if (userOriginPosRotate->isChecked())
        emit rotateSelectedShape(rotateBox->value(), 0); // 0 : using SDI_GeometricShape::OriginPos
}

void leftToolsWidget::takeSymmetryParameters()
{
    if (centralSymmetry->isChecked())
        emit centralSymmetrySelectedShape();
    else if (OxSymmetry->isChecked())
        emit OxSymmetrySelectedShape();
    else if (OySymmetry->isChecked())
        emit OySymmetrySelectedShape();
}

void leftToolsWidget::showPosition(QString posInfo)
{
    positionLabel->setText(posInfo);
}

