#include "lefttoolswidget.h"

leftToolsWidget::leftToolsWidget(QWidget *parent) : QWidget(parent)
{
    //setFont(QFont("Tahoma", 10));
    setupGUI();
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

    helpMode = new QPushButton(QIcon(":/images/icons/about.png"), tr("Giới thiệu"), this);
    setDrawModeButtonStyle(helpMode);
    QVBoxLayout* selectModeLayout{new QVBoxLayout(this)};
    selectModeLayout->addWidget(graphic2DMode);
    selectModeLayout->addWidget(graphic3DMode);
    selectModeLayout->addWidget(helpMode);
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
    OzTranslateSlider = new QSlider(Qt::Horizontal, this);
    OzTranslateBox = new QSpinBox(this);
    setSlider_BoxSytle(OzTranslateSlider, OzTranslateBox, -500, 500);
    OzTranslateBox->setDisabled(true);
    OzTranslateSlider->setDisabled(true);
    QGridLayout* translationLayout{new QGridLayout(this)};
    translationLayout->addWidget(new QLabel("Ox"), 0, 0);
    translationLayout->addWidget(OxTranslateSlider, 0, 1);
    translationLayout->addWidget(OxTranslateBox, 0, 2);
    translationLayout->addWidget(new QLabel("Oy"), 1, 0);
    translationLayout->addWidget(OyTranslateSlider, 1, 1);
    translationLayout->addWidget(OyTranslateBox, 1, 2);
    translationLayout->addWidget(new QLabel("Oz"), 2, 0);
    translationLayout->addWidget(OzTranslateSlider, 2, 1);
    translationLayout->addWidget(OzTranslateBox, 2, 2);
    QGroupBox* translationGroupBox{new QGroupBox(tr("PHÉP TỊNH TIẾN"), this)};
    translationGroupBox->setLayout(translationLayout);
    //--------------------------------------------------------------------
    // rotate mode
    rotateSlider = new QSlider(Qt::Horizontal, this);
    rotateBox = new QSpinBox(this);
    setSlider_BoxSytle(rotateSlider, rotateBox, -360, 360);
    QGridLayout* rotateLayout{new QGridLayout(this)};
    rotateLayout->addWidget(new QLabel(tr("Góc quay")), 0, 0);
    rotateLayout->addWidget(rotateSlider, 0, 1);
    rotateLayout->addWidget(rotateBox, 0, 2);
    QGroupBox* rotateGroupBox{new QGroupBox(tr("Phép xoay"), this)};
    rotateGroupBox->setLayout(rotateLayout);
    //---------------------------------------------------------------------
    //symmetry mode
    centralSymmetry = new QRadioButton(tr("Đối xứng qua gốc tọa độ"), this);
    centralSymmetry->setAutoExclusive(true);
    OxSymmetry = new QRadioButton(tr("Đối xứng qua Ox"), this);
    OxSymmetry->setAutoExclusive(true);
    OySymmetry = new QRadioButton(tr("Đối xứng qua Oy"), this);
    OySymmetry->setAutoExclusive(true);
    takeSymmetry = new QPushButton(tr("Lấy đối xứng đối tượng"));
    QVBoxLayout* symmetryLayout{new QVBoxLayout()};
    symmetryLayout->addWidget(centralSymmetry);
    symmetryLayout->addWidget(OxSymmetry);
    symmetryLayout->addWidget(OySymmetry);
    symmetryLayout->addWidget(takeSymmetry);
    QGroupBox* symmetryGroupBox{new QGroupBox(tr("PHÉP ĐỐI XỨNG"), this)};
    symmetryGroupBox->setLayout(symmetryLayout);
    //---------------------------------------------------------------------
    // zoom mode
    zoomSlider = new QSlider(Qt::Horizontal, this);
    zoomBox = new QSpinBox(this);
    setSlider_BoxSytle(zoomSlider, zoomBox, -5, 5);
    //QObject::connect(zoomSlider, SIGNAL(valueChanged(int)), infoText, SLOT(zoomIn(int)));
    QGridLayout* zoomLayout{new QGridLayout(this)};
    zoomLayout->addWidget(new QLabel(tr("Tỉ lệ")), 0, 0);
    zoomLayout->addWidget(zoomSlider, 0, 1);
    zoomLayout->addWidget(zoomBox, 0, 2);
    QGroupBox* zoomGroupBox{new QGroupBox(tr("PHÉP BIẾN ĐỔI TỈ LỆ"), this)};
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
    //---------------------------------------------------------------------
    QSpacerItem* verticalSpacer{new QSpacerItem(20, 180, QSizePolicy::Minimum, QSizePolicy::Expanding)};
    QVBoxLayout* mainLayout{new QVBoxLayout(this)};
    mainLayout->addWidget(selectModeGroupBox);
    mainLayout->addWidget(translationGroupBox);
    mainLayout->addWidget(rotateGroupBox);
    mainLayout->addWidget(symmetryGroupBox);
    mainLayout->addWidget(zoomGroupBox);
    mainLayout->addWidget(infoGroupBox);
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
