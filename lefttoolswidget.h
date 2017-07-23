//GUI
#ifndef LEFTTOOLSWIDGET_H
#define LEFTTOOLSWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QtDebug>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QRadioButton>
#include <QGroupBox>
#include <QTextEdit>
#include <QColorDialog>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QToolBox>
#include <QSpacerItem>
#include "sdi_namespace.h"
class leftToolsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit leftToolsWidget(QWidget *parent = nullptr);
    ~leftToolsWidget() = default;
    void setInfoBox(QString shapeNameLabel, QString shapeData);

private:
    QPushButton* graphic2DMode;
    QPushButton* graphic3DMode;
    //QPushButton* helpMode;

    QSlider* OxTranslateSlider;
    QSpinBox* OxTranslateBox;
    QSlider* OyTranslateSlider;
    QSpinBox* OyTranslateBox;
    QPushButton* doTranslate;

    QSlider* rotateSlider;
    QSpinBox* rotateBox;
    QRadioButton* shapeCentralRotate;
    QRadioButton* userOriginPosRotate;
    QPushButton* doRotate;

    QRadioButton* centralSymmetry;
    QRadioButton* OxSymmetry;
    QRadioButton* OySymmetry;
    QPushButton* doSymmetry;

    QDoubleSpinBox* zoomBox;
    QPushButton* doZoom;

    QLabel* shapeNameLabel;
    QTextEdit* shapeDataTEdit;
    QLabel* positionLabel;
    void setupGUI();
    void setDrawModeButtonStyle(QPushButton* button);
    void setSlider_BoxSytle(QSlider* slider, QSpinBox* box, int minValue, int MaxValue);

signals:
    void changeGraphicsMode(GraphicsMode newMode);
    void translateSelectedShape(int xtrans, int ytrans);
    void scaleSelectedShape(double xscale, double yscale);
    void rotateSelectedShape(double degree, int centralPointID);
    void centralSymmetrySelectedShape();
    void OxSymmetrySelectedShape();
    void OySymmetrySelectedShape();

private slots:

    void takeTranslateParameters();
    void takeScaleParameters();
    void takeRotateParameters();
    void takeSymmetryParameters();

public slots:
    void showPosition(QString posInfo);
};

#endif // LEFTTOOLSWIDGET_H
