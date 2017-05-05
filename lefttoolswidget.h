//GUI
#ifndef LEFTTOOLSWIDGET_H
#define LEFTTOOLSWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>
#include <QRadioButton>
#include <QGroupBox>
#include <QTextEdit>
#include <QColorDialog>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
class leftToolsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit leftToolsWidget(QWidget *parent = nullptr);
    ~leftToolsWidget() = default;
    void setInfoBox(QString shapeNameLabel, QString shapeData);
    QPushButton* graphic2DMode;
    QPushButton* graphic3DMode;
    QPushButton* helpMode;

    QSlider* OxTranslateSlider;
    QSpinBox* OxTranslateBox;
    QSlider* OyTranslateSlider;
    QSpinBox* OyTranslateBox;
    QSlider* OzTranslateSlider;
    QSpinBox* OzTranslateBox;    

    QSlider* rotateSlider;
    QSpinBox* rotateBox;

    QRadioButton* centralSymmetry;
    QRadioButton* OxSymmetry;
    QRadioButton* OySymmetry;
    QPushButton* takeSymmetry;

    QSlider* zoomSlider;
    QSpinBox* zoomBox;
    QLabel* shapeNameLabel;
    QTextEdit* shapeDataTEdit;
private:
    void setupGUI();
    void setDrawModeButtonStyle(QPushButton* button);
    void setSlider_BoxSytle(QSlider* slider, QSpinBox* box, int minValue, int MaxValue);

signals:
    void changeGraphicsMode(int newID);
private slots:
    void decideNewGraphicMode();
};

#endif // LEFTTOOLSWIDGET_H
