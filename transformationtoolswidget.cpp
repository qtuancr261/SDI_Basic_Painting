#include "transformationtoolswidget.h"
TransformationToolsWidget::TransformationToolsWidget(QWidget *parent) : QWidget(parent)
{
    setupGUI();
    if (QSysInfo::WindowsVersion >= QSysInfo::WV_WINDOWS7)
        setFont(QFont("Segoe UI", 11));
}

void TransformationToolsWidget::setInfoBox(QString shapeName, QString shapeData)
{
    shapeNameLabel->setText(shapeName);
    shapeDataTEdit->setText(shapeData);
}

void TransformationToolsWidget::setupGUI()
{
    // select Mode Group Box
    graphic2DMode = new QPushButton(QIcon(":/images/icons/oxy_coordinate.png"), tr("Đồ họa 2D - Hệ trục Oxyz"), this);
    setGraphicModeButtonStyle(graphic2DMode);
    QObject::connect(graphic2DMode, &QPushButton::toggled,
                     [this](bool checked){if (checked) emit changeGraphicsMode(GraphicsMode::GM_2D);});

    graphic3DMode = new QPushButton(QIcon(":/images/icons/oxyz_coordinate.png"), tr("Đồ họa 3D - Hệ trục Oxyz"), this);
    setGraphicModeButtonStyle(graphic3DMode);
    QObject::connect(graphic3DMode, &QPushButton::toggled,
                     [this](bool checked){ if (checked) emit changeGraphicsMode(GraphicsMode::GM_3D); });

    QVBoxLayout* selectModeLayout{new QVBoxLayout(this)};
    selectModeLayout->addWidget(graphic2DMode);
    selectModeLayout->addWidget(graphic3DMode);
    QGroupBox* selectModeGroupBox{new QGroupBox(tr("CHỌN CHẾ ĐỘ"), this)};
    selectModeGroupBox->setLayout(selectModeLayout);
    //----------------------------------------------------------------------
    // translate Group Box
    OxTranslateSlider = new QSlider(Qt::Horizontal, this);
    OxTranslateBox = new QSpinBox(this);
    setSlider_BoxSytle(OxTranslateSlider, OxTranslateBox, -500, 500);
    OyTranslateSlider = new QSlider(Qt::Horizontal, this);
    OyTranslateBox = new QSpinBox(this);
    setSlider_BoxSytle(OyTranslateSlider, OyTranslateBox, -500, 500);
    doTranslate = new QPushButton(QIcon(":/images/icons/pointHand.png"),tr("Tịnh tiến đối tượng"), this);
    doTranslate->setAutoRepeat(true);
    QObject::connect(doTranslate, &QPushButton::pressed,
                     [this](){ emit translateSelectedShape(OxTranslateBox->value(), OyTranslateBox->value());});
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
    // rotate Group Box
    rotateSlider = new QSlider(Qt::Horizontal, this);
    rotateBox = new QSpinBox(this);
    setSlider_BoxSytle(rotateSlider, rotateBox, -360, 360);
    shapeCentralRotate = new QRadioButton(tr("Tâm đối tượng"), this);
    shapeCentralRotate->setAutoExclusive(true);
    userOriginPosRotate = new QRadioButton( tr("Gốc tọa độ"), this);
    userOriginPosRotate->setAutoExclusive(true);
    doRotate = new QPushButton(QIcon(":/images/icons/pointHand.png"),tr("Xoay đối tượng"), this);
    doRotate->setAutoRepeat(true);
    QObject::connect(doRotate, &QPushButton::pressed, this, &TransformationToolsWidget::takeRotateParameters);
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
    //symmetry Group Box
    centralSymmetry = new QRadioButton(tr("Đối xứng qua gốc tọa độ"), this);
    centralSymmetry->setAutoExclusive(true);
    OxSymmetry = new QRadioButton(tr("Đối xứng qua Ox"), this);
    OxSymmetry->setAutoExclusive(true);
    OySymmetry = new QRadioButton(tr("Đối xứng qua Oy"), this);
    OySymmetry->setAutoExclusive(true);
    doSymmetry = new QPushButton(QIcon(":/images/icons/pointHand.png"), tr("Lấy đối xứng đối tượng"), this);
    doSymmetry->setAutoRepeat(true);
    QObject::connect(doSymmetry, &QPushButton::pressed, this, &TransformationToolsWidget::takeSymmetryParameters);
    QVBoxLayout* symmetryLayout{new QVBoxLayout()};
    symmetryLayout->addWidget(centralSymmetry);
    symmetryLayout->addWidget(OxSymmetry);
    symmetryLayout->addWidget(OySymmetry);
    symmetryLayout->addWidget(doSymmetry);
    symmetryLayout->addItem(new QSpacerItem(10, 10, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    QGroupBox* symmetryGroupBox{new QGroupBox()};
    symmetryGroupBox->setLayout(symmetryLayout);
    //---------------------------------------------------------------------
    // zoom Group box
    zoomBox = new QDoubleSpinBox(this);
    zoomBox->setRange(0.1, 5.0);
    zoomBox->setValue(1.0);
    zoomBox->setSingleStep(0.1);
    zoomBox->setSuffix("x");
    doZoom = new QPushButton(QIcon(":/images/icons/pointHand.png"), tr("Biến đồi đối tượng"), this);
    doZoom->setAutoRepeat(true);
    //QObject::connect(doZoom, SIGNAL(clicked(bool)), this, SLOT(takeScaleParameters()));
    QObject::connect(doZoom, &QPushButton::pressed,
                     [this](){ emit scaleSelectedShape(zoomBox->value(), zoomBox->value());});
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

    mousePositionLabel = new QLabel("Tọa độ");
    mousePositionLabel->setAlignment(Qt::AlignCenter);
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
    transformBox->setCurrentIndex(3);
    QSpacerItem* verticalSpacer{new QSpacerItem(10, 10, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding)};
    QVBoxLayout* mainLayout{new QVBoxLayout(this)};
    mainLayout->addWidget(selectModeGroupBox);
    mainLayout->addWidget(transformBox);
    mainLayout->addWidget(infoGroupBox);
    mainLayout->addWidget(mousePositionLabel);
    mainLayout->addItem(verticalSpacer);
    setLayout(mainLayout);
}

void TransformationToolsWidget::setGraphicModeButtonStyle(QPushButton *button)
{
    button->setIconSize(QSize(32, 32));
    button->setCheckable(true);
    button->setAutoExclusive(true);
}

void TransformationToolsWidget::setSlider_BoxSytle(QSlider *slider, QSpinBox *box, int minValue, int MaxValue)
{
    slider->setRange(minValue, MaxValue);
    box->setRange(minValue, MaxValue);
    QObject::connect(slider, SIGNAL(valueChanged(int)), box, SLOT(setValue(int)));
    QObject::connect(box, SIGNAL(valueChanged(int)), slider, SLOT(setValue(int)));
}

void TransformationToolsWidget::takeRotateParameters()
{
    if (shapeCentralRotate->isChecked())
        emit rotateSelectedShape(rotateBox->value(), 1); // 1 : using SDI_GeometricShape::centralPoint
    else if (userOriginPosRotate->isChecked())
        emit rotateSelectedShape(rotateBox->value(), 0); // 0 : using SDI_GeometricShape::OriginPos
}

void TransformationToolsWidget::takeSymmetryParameters()
{
    if (centralSymmetry->isChecked())
        emit centralSymmetrySelectedShape();
    else if (OxSymmetry->isChecked())
        emit OxSymmetrySelectedShape();
    else if (OySymmetry->isChecked())
        emit OySymmetrySelectedShape();
}

void TransformationToolsWidget::showMousePosition(QString posInfo)
{
    mousePositionLabel->setText(posInfo);
}

