#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShader>
#include <QOpenGLTexture>

class GLTextureWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit GLTextureWidget(QWidget *parent = 0);
    ~GLTextureWidget();

    static const GLfloat vertices[];
    static const GLuint indices[];
    static const GLfloat textureVertex[];

protected:
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void initializeGL() Q_DECL_OVERRIDE;

private:
    QOpenGLShader *m_vshader1;
    QOpenGLShader *m_fshader1;
    QOpenGLShaderProgram *m_program1;
    QOpenGLBuffer m_vbo1;
    int m_vertexAttr1;
    int m_texAttr1;
    QOpenGLTexture *texture_pfd_back;
    QOpenGLTexture *texture_pfd_ladd;

signals:

public slots:
};

#endif // MYGLWIDGET_H
