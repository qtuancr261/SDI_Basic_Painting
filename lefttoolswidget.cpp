#include "lefttoolswidget.h"

leftToolsWidget::leftToolsWidget(QWidget *parent) : QWidget(parent)
{
    setupGUI();
}

void leftToolsWidget::setupGUI()
{
    // select graphic mode
    graphic2DMode = new QPushButton(QIcon(":/images/icons/Letters.ico"), tr("Hệ trục tọa độ Oxy"), this);
    setDrawModeButtonStyle(graphic2DMode);
    graphic3DMode = new QPushButton(QIcon(":/images/icons/Letters.ico"), tr("Hệ trục tọa độ Oxyz"), this);
    setDrawModeButtonStyle(graphic3DMode);
    helpMode = new QPushButton(QIcon(":/images/icons/Letters.ico"), tr("Giới thiệu"), this);
    setDrawModeButtonStyle(helpMode);
    QVBoxLayout* selectModeLayout{new QVBoxLayout(this)};
    selectModeLayout->addWidget(graphic2DMode);
    selectModeLayout->addWidget(graphic3DMode);
    selectModeLayout->addWidget(helpMode);
    QGroupBox* selectModeGroupBox{new QGroupBox(tr("Chọn chế độ"), this)};
    selectModeGroupBox->setLayout(selectModeLayout);
    //----------------------------------------------------------------------
    // translate mode
    OxTranslateSlider = new QSlider(Qt::Horizontal, this);
    OxTranslateBox = new QSpinBox(this);
    setSlider_BoxSytle(OxTranslateSlider, OxTranslateBox, -50, 50);
    OyTranslateSlider = new QSlider(Qt::Horizontal, this);
    OyTranslateBox = new QSpinBox(this);
    setSlider_BoxSytle(OyTranslateSlider, OyTranslateBox, -50, 50);
    OzTranslateSlider = new QSlider(Qt::Horizontal, this);
    OzTranslateBox = new QSpinBox(this);
    setSlider_BoxSytle(OzTranslateSlider, OzTranslateBox, -50, 50);
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
    QGroupBox* translationGroupBox{new QGroupBox(tr("Phép tịnh tiến"), this)};
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
    QGroupBox* symmetryGroupBox{new QGroupBox(tr("Phép đối xứng"), this)};
    symmetryGroupBox->setLayout(symmetryLayout);
    //---------------------------------------------------------------------
    // zoom mode
    zoomSlider = new QSlider(Qt::Horizontal, this);
    zoomBox = new QSpinBox(this);
    setSlider_BoxSytle(zoomSlider, zoomBox, -5, 5);
    QGridLayout* zoomLayout{new QGridLayout(this)};
    zoomLayout->addWidget(new QLabel(tr("Tỉ lệ")), 0, 0);
    zoomLayout->addWidget(zoomSlider, 0, 1);
    zoomLayout->addWidget(zoomBox, 0, 2);
    QGroupBox* zoomGroupBox{new QGroupBox(tr("Phép biến đổi tỉ lệ"), this)};
    zoomGroupBox->setLayout(zoomLayout);

    //---------------------------------------------------------------------
    QSpacerItem* verticalSpacer{new QSpacerItem(20, 180, QSizePolicy::Minimum, QSizePolicy::Expanding)};
    QVBoxLayout* mainLayout{new QVBoxLayout(this)};
    mainLayout->addWidget(selectModeGroupBox);
    mainLayout->addWidget(translationGroupBox);
    mainLayout->addWidget(rotateGroupBox);
    mainLayout->addWidget(symmetryGroupBox);
    mainLayout->addWidget(zoomGroupBox);
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
}
