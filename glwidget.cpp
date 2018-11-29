#include "glwidget.h"
#include <QMouseEvent>

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent), m_vertexBuffer( QOpenGLBuffer::VertexBuffer )
{

}

GLWidget::~GLWidget()
{
    // Make OpenGL context current and call cleanupGL()
    makeCurrent();
    doneCurrent();
}

void GLWidget::mousePressEvent(QMouseEvent * /*event*/)
{
    // Clear input samples and redraw
    inputSamples.clear();
    repaint();
}

void GLWidget::mouseMoveEvent(QMouseEvent * event)
{
    // Add input sample and redraw
    inputSamples.push_back(event->pos());
    repaint();
}

void GLWidget::mouseReleaseEvent(QMouseEvent * /*event*/)
{
    // Nothing to do
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    // get context opengl-version
    qDebug() << "Context valid:  " << context()->isValid();
    qDebug() << "Widget OpenGl:  " << context()->format().majorVersion() << "." << context()->format().minorVersion();
    qDebug() << "VENDOR:         " << (const char*)glGetString(GL_VENDOR);
    qDebug() << "RENDERDER:      " << (const char*)glGetString(GL_RENDERER);
    qDebug() << "VERSION:        " << (const char*)glGetString(GL_VERSION);
    qDebug() << "GLSL VERSION:   " << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

    QGLFormat glFormat;

    if(context()->isValid())
        glFormat.setVersion( context()->format().majorVersion() , context()->format().minorVersion() );

    glFormat.setProfile( QGLFormat::CoreProfile ); // Requires >=Qt-4.8.0
    glFormat.setSampleBuffers( true );

    if ( !glFormat.sampleBuffers() )
        qWarning() << "Could not enable sample buffers";

    // Set clear color
    glClearColor(0.0f, 0.7f, 0.4f, 1.0f);

    // Prepare a complete shader program...
    if ( !prepareShaderProgram( ":/std.vert", ":/std.geom",":/std.frag" ) )
        return;

    // Prepare a complete vertex...
    if ( !prepareVertex())
        return;

    // Bind the shader program so that we can associate variables from
    // our application to the shaders
    if ( !m_shaderProgram.bind() )
    {
        qWarning() << "Could not bind shader program to context";
        return;
    }

    // Enable the "vertex" attribute to bind it to our currently bound
    // vertex buffer.
    m_shaderProgram.setAttributeBuffer( "vertex", GL_FLOAT, 0, sizeof(GLfloat) );
    m_shaderProgram.enableAttributeArray( "vertex" );

}

void GLWidget::keyPressEvent( QKeyEvent* e )
{
    switch ( e->key() )
    {
    case Qt::Key_Escape:
        exit(0);
        break;
    }
}

void GLWidget::resizeGL(int w, int h)
{   
    // Set the viewport to window dimensions
    glViewport( 0, 0, w, qMax( h, 1 ) );
}

void GLWidget::paintGL()
{
    // Clear the buffer with the current clearing color
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    drawTriangle();
}

bool GLWidget::prepareVertex()
{
    m_vertexBuffer.create();
    m_vertexBuffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
    if ( !m_vertexBuffer.bind() )
    {
        qWarning() << "Could not bind vertex buffer to the context";
        return false;
    }

    return true;
}

bool GLWidget::prepareShaderProgram( const QString& vertexShaderPath,
                                     const QString& geometryShaderPath,
                                     const QString& fragmentShaderPath
                                     )
{

    // First we load and compile the vertex shader...
    bool result = m_shaderProgram.addShaderFromSourceFile( QOpenGLShader::Vertex, vertexShaderPath );
    if ( !result )
        qWarning() << m_shaderProgram.log();

    result = m_shaderProgram.addShaderFromSourceFile( QOpenGLShader::Geometry, geometryShaderPath );
    if ( !result )
        qWarning() << m_shaderProgram.log();

    // ...now the fragment shader...
    result = m_shaderProgram.addShaderFromSourceFile( QOpenGLShader::Fragment, fragmentShaderPath );
    if ( !result )
        qWarning() << m_shaderProgram.log();

    // ...and finally we link them to resolve any references.
    result = m_shaderProgram.link();
    if ( !result )
        qWarning() << "Could not link shader program:" << m_shaderProgram.log();

    return result;
}

void GLWidget::drawTriangle()
{  
    m_vertexData.append(-0.5f);
    m_vertexData.append(-0.5f);
    m_vertexData.append(0.0f);
    m_vertexData.append(1.0f);

    m_vertexData.append(0.5f);
    m_vertexData.append(-0.5f);
    m_vertexData.append(0.0f);
    m_vertexData.append(1.0f);

    m_vertexData.append(0.0f);
    m_vertexData.append(0.5f);
    m_vertexData.append(0.0f);
    m_vertexData.append(1.0f);

    m_vertexBuffer.allocate(m_vertexData.constData(), m_vertexData.count() * sizeof(GLfloat));
    // Draw stuff
    glDrawArrays(GL_TRIANGLES, 0, 3 );

}
