#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include "openglwindow.h"

#include <QWidget>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QPushButton>

// hhj
#include <QColor>
#include <QGroupBox>

class QOpenGLShaderProgram;
class QOpenGLBuffer;
class QOpenGLVertexArrayObject;

class RenderWindow : public OpenGLWindow
{
public:
    RenderWindow();

    void initialize();
    void render();
    void checkError(const QString& prefix);

    public slots:
    // hhj - implementation of virtual function
    void select_bg_color();
    void select_up_color();
    void select_rt_color();
    void select_lt_color();

private:

    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;

    QOpenGLShaderProgram *m_program;
    QOpenGLVertexArrayObject* m_vao;
    QOpenGLBuffer *m_vbo;
    QOpenGLBuffer *m_ibo;
    int m_frame;
    bool m_useSourceCode;
    
    // hhj - background color for the render window
    QColor m_bg;
    QColor m_up;
    QColor m_rt;
    QColor m_lt;
};

class RenderWindowWidget : public QWidget
{
public:
  RenderWindowWidget(QWidget* parent): QWidget(parent) {

      renWin = new RenderWindow();

      QWidget* widget = QWidget::createWindowContainer(renWin);
//      QHBoxLayout* layout = new QHBoxLayout(this);
      
      // hhj - add a new button
      QVBoxLayout* layout = new QVBoxLayout(this);
      QGroupBox* box = new QGroupBox();
      QVBoxLayout* sub_layout = new QVBoxLayout(box);
      QPushButton* button = new QPushButton();
      button->setText("Change Background Color");
      QPushButton* button1 = new QPushButton();
      button1->setText("Change tri top pt Color");
      QPushButton* button2 = new QPushButton();
      button2->setText("Change tri right pt Color");
      QPushButton* button3 = new QPushButton();
      button3->setText("Change tri left pt Color");
      sub_layout->addWidget(button);
      sub_layout->addWidget(button1);
      sub_layout->addWidget(button2);
      sub_layout->addWidget(button3);
      
      layout->addWidget(widget);
      layout->addWidget(box);
      
      // hhj - connect signal with slot
      // hhj - rewin will call openglWindow in this case
      // hhj - write virtual function in the openglWindow and overwrite it in the renderWindow
      QObject::connect(button, SIGNAL(clicked()), renWin, SLOT(select_bg_color()));
      QObject::connect(button1, SIGNAL(clicked()), renWin, SLOT(select_up_color()));
      QObject::connect(button2, SIGNAL(clicked()), renWin, SLOT(select_rt_color()));
      QObject::connect(button3, SIGNAL(clicked()), renWin, SLOT(select_lt_color()));
  }

  RenderWindow* GetRenderWindow() {
      return renWin;
  }

private:
  RenderWindow* renWin;
};


#endif // RENDERWINDOW_H
