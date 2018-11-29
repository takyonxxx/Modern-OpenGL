#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QGLWidget>
#include <QGLFormat>
#include <vector>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
private:
    Q_OBJECT
    Q_DISABLE_COPY(GLWidget)

public:
    GLWidget( QWidget * parent = nullptr);
    ~GLWidget();

    static const GLfloat vertices[];
    static const GLuint indices[];
    static const GLfloat textureVertex[];


private:        
    void drawTriangle();

protected:

    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    virtual void keyPressEvent( QKeyEvent* e );

private:

    // Input mouse data
    std::vector<QPoint> inputSamples;

    // GL resources    
    QVector<GLfloat> m_vertexData;
    bool prepareShaderProgram( const QString& vertexShaderPath,
                               const QString& geometryShaderPath,
                               const QString& fragmentShaderPath );
    bool prepareVertex();

    QOpenGLShaderProgram m_shaderProgram;
    QOpenGLBuffer m_vertexBuffer;
    GLuint vertexShader;
    GLuint fragmentShader;
};

#endif // OPENGLWIDGET_H
