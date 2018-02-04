//GUI
#ifndef TRANSFORMATIONTOOLSWIDGET_H
#define TRANSFORMATIONTOOLSWIDGET_H

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
class TransformationToolsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TransformationToolsWidget(QWidget *parent = nullptr);
    ~TransformationToolsWidget() = default;
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
    QLabel* mousePositionLabel;
    void setupGUI(); // setup everything in here
    void setGraphicModeButtonStyle(QPushButton* button);
    void setSlider_BoxSytle(QSlider* slider, QSpinBox* box, int minValue, int MaxValue);

signals:
    void changeGraphicsMode(GraphicsMode newMode); // will be emitted if graphic2D(3D)Mode button is checked
    void translateSelectedShape(int xtrans, int ytrans); // will be emitted if doTranslate button is pressed
    void scaleSelectedShape(double xscale, double yscale); // will be emitted if doZoom button is pressed
    void rotateSelectedShape(double degree, int centralPointID); // will be emitted if doRotate button is pressed
    void centralSymmetrySelectedShape();
    //will be emitted if doSymmetry button is pressed and centralSymmetry button is checked
    void OxSymmetrySelectedShape();
    //will be emitted if doSymmetry button is pressed and OxSymmetry button is checked
    void OySymmetrySelectedShape();
    //will be emitted if doSymmetry button is pressed and OySymmetry button is checked

private slots:
    void takeRotateParameters();
    void takeSymmetryParameters();

public slots:
    void showMousePosition(QString posInfo); // take current mouse position and show it on the screen
};

#endif // TRANSFORMATIONTOOLSWIDGET_H
