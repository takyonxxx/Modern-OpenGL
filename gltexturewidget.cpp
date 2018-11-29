#include "gltexturewidget.h"

//Set up vertex data (and buffer(s)) and attribute pointers
const GLfloat  GLTextureWidget::vertices[] = {
    1.0f,  1.0f, 0.0f,  // Top Right
    1.0f, -1.0f, 0.0f,// Bottom Right
    -1.0f, -1.0f, 0.0f,// Bottom Left
    -1.0f,  1.0f, 0.0f// Top Left
};

//setup the order of vertices (indexes)
const GLuint GLTextureWidget::indices[] = {  // Note that we start from 0!
                                             0, 3, 2,  // First Triangle
                                             2, 1, 0   // Second Triangle 0, 1, 2
                                          };


//Set up vertex for textures
const GLfloat  GLTextureWidget::textureVertex[] = {

    1.0f,  1.0f,  // Top Right
    1.0f,  0.0f,// Bottom Right
    0.0f,  0.0f,// Bottom Left
    0.0f,  1.0f// Top Left
};


GLTextureWidget::GLTextureWidget(QWidget *parent):
    QOpenGLWidget(parent)
{

}

GLTextureWidget::~GLTextureWidget()
{
    // And now release all OpenGL resources.
    makeCurrent();

    delete m_vshader1;
    delete m_fshader1;
    delete texture_pfd_back;
    delete m_program1;
    m_vbo1.destroy();
    doneCurrent();
}


void GLTextureWidget::initializeGL()
{
    initializeOpenGLFunctions();
    m_vshader1 = new QOpenGLShader(QOpenGLShader::Vertex);
    const char *vsrc1 =
            "attribute vec3 aPosition; \n"
            "attribute vec2 aTexCoord; \n"
            "varying vec2 vTexCoord; \n"
            "void main() \n"
            "{ \n"
            " gl_Position = vec4(aPosition, 1.0); \n"
            " vTexCoord = aTexCoord; \n"
            "} \n";

    bool r = m_vshader1->compileSourceCode(vsrc1);
    qDebug()<<"Compilation shader = "<<r;

    m_fshader1 = new QOpenGLShader(QOpenGLShader::Fragment);
    const char *fsrc1 =
            "varying vec2 vTexCoord; \n"
            "uniform sampler2D ourTexture; \n"
            "void main() \n"
            "{ \n"
            " gl_FragColor =texture2D(ourTexture, vTexCoord); \n"
            "} \n";
    r = m_fshader1->compileSourceCode(fsrc1);
    qDebug()<<"Compilation shaders = "<<r;

    m_program1 = new QOpenGLShaderProgram;
    m_program1->addShader(m_vshader1);
    m_program1->addShader(m_fshader1);
    qDebug()<<"Link = "<<m_program1->link();

    m_vertexAttr1 = m_program1->attributeLocation("aPosition");
    qDebug()<<"Aposition index = "<<m_vertexAttr1;
    m_texAttr1 = m_program1->attributeLocation("aTexCoord");
    qDebug()<<"aTexCoord index = "<<m_texAttr1;


    // Use a vertex buffer object. Client-side pointers are old-school and should be avoided.
    m_vbo1.create();
    m_vbo1.bind();
    m_vbo1.allocate(sizeof(vertices) + sizeof(textureVertex));
    //add vertices
    m_vbo1.write(0, vertices, sizeof(vertices));
    //add texture vertices
    m_vbo1.write(sizeof(vertices), textureVertex, sizeof(textureVertex));

    //texture
    texture_pfd_back = new QOpenGLTexture(QImage(":/images/pfd_adi_back.svg").mirrored());
    texture_pfd_back->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture_pfd_back->setMagnificationFilter(QOpenGLTexture::Linear);

    texture_pfd_ladd = new QOpenGLTexture(QImage(":/images/pfd_adi_ladd.svg").mirrored());
    texture_pfd_ladd->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture_pfd_ladd->setMagnificationFilter(QOpenGLTexture::Linear);

    m_vbo1.release();

    // configure global opengl state
    // -----------------------------
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

}

void GLTextureWidget::resizeGL(int w, int h)
{

}


void GLTextureWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    m_program1->bind();
    m_program1->enableAttributeArray(m_vertexAttr1);
    m_program1->enableAttributeArray(m_texAttr1);

    m_vbo1.bind();
    m_program1->setAttributeBuffer(m_vertexAttr1, GL_FLOAT, 0, 3);
    m_program1->setAttributeBuffer(m_texAttr1, GL_FLOAT, sizeof(vertices), 2);


    texture_pfd_back->bind(0);
    //texture_pfd_ladd->bind(0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);

    m_vbo1.release();

    m_program1->disableAttributeArray(m_vertexAttr1);
    m_program1->disableAttributeArray(m_texAttr1);
    m_program1->release();
}
