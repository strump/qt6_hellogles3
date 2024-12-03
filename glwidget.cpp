// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "glwidget.h"
#include <QImage>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLExtraFunctions>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QTimer>


GLWidget::GLWidget()
{
    m_world.setToIdentity();
    m_world.translate(0, 0, 0);
    m_world.rotate(180, 1, 0, 0);
    setZ(5.0f);

    /*QSequentialAnimationGroup *animGroup = new QSequentialAnimationGroup(this);
    animGroup->setLoopCount(-1);
    QPropertyAnimation *zAnim0 = new QPropertyAnimation(this, QByteArrayLiteral("z"));
    zAnim0->setStartValue(1.5f);
    zAnim0->setEndValue(10.0f);
    zAnim0->setDuration(2000);
    animGroup->addAnimation(zAnim0);
    QPropertyAnimation *zAnim1 = new QPropertyAnimation(this, QByteArrayLiteral("z"));
    zAnim1->setStartValue(10.0f);
    zAnim1->setEndValue(50.0f);
    zAnim1->setDuration(4000);
    zAnim1->setEasingCurve(QEasingCurve::OutElastic);
    animGroup->addAnimation(zAnim1);
    QPropertyAnimation *zAnim2 = new QPropertyAnimation(this, QByteArrayLiteral("z"));
    zAnim2->setStartValue(50.0f);
    zAnim2->setEndValue(1.5f);
    zAnim2->setDuration(2000);
    animGroup->addAnimation(zAnim2);
    animGroup->start();*/

    QPropertyAnimation* rAnim = new QPropertyAnimation(this, QByteArrayLiteral("r2"));
    rAnim->setStartValue(0.0f);
    rAnim->setEndValue(360.0f);
    rAnim->setDuration(2000);
    rAnim->setLoopCount(-1);
    rAnim->start();

    //QTimer::singleShot(4000, this, &GLWindow::startSecondStage);
}

GLWidget::~GLWidget()
{
    makeCurrent();
    delete m_texture;
    delete m_program;
    delete m_vbo;
    delete m_vao;
}

void GLWidget::startSecondStage()
{
    QPropertyAnimation* r2Anim = new QPropertyAnimation(this, QByteArrayLiteral("r2"));
    r2Anim->setStartValue(0.0f);
    r2Anim->setEndValue(360.0f);
    r2Anim->setDuration(20000);
    r2Anim->setLoopCount(-1);
    r2Anim->start();
}

void GLWidget::setZ(float v)
{
    m_eye.setZ(v);
    m_uniformsDirty = true;
    update();
}

void GLWidget::setR(float v)
{
    m_r = v;
    m_uniformsDirty = true;
    update();
}

void GLWidget::setR2(float v)
{
    m_r2 = v;
    m_uniformsDirty = true;
    update();
}

static const char *vertexShaderSource =
    "layout(location = 0) in vec4 vertex;\n"
    "layout(location = 1) in vec3 normal;\n"
    "out vec3 color;\n"
    "uniform mat4 projMatrix;\n"
    "uniform mat4 camMatrix;\n"
    "uniform mat4 worldMatrix;\n"
    "uniform mat4 myMatrix;\n"
    "uniform sampler2D sampler;\n"
    "void main() {\n"
    "   mat4 wm = myMatrix * worldMatrix;\n"
    "   color = vec3(0.0, 1.0, 0.0);\n"
    "   gl_Position = projMatrix * camMatrix * wm * vertex;\n"
    "}\n";

static const char *fragmentShaderSource =
    "in highp vec3 color;\n"
    "out highp vec4 fragColor;\n"
    "void main() {\n"
    "   fragColor = vec4(color, 1.0);\n"
    "}\n";

QByteArray versionedShaderCode(const char *src)
{
    QByteArray versionedSrc;

    if (QOpenGLContext::currentContext()->isOpenGLES())
        versionedSrc.append(QByteArrayLiteral("#version 300 es\n"));
    else
        versionedSrc.append(QByteArrayLiteral("#version 330\n"));

    versionedSrc.append(src);
    return versionedSrc;
}

void GLWidget::initializeGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();


    qDebug() << "Qt version:" << qVersion();
    qDebug() << "OpenGL version:" << (const char*)f->glGetString(GL_VERSION);
    qDebug() << "Widget OpenGl: " << format().majorVersion() << "." << format().minorVersion();
    qDebug() << "Context valid: " << context()->isValid();
    qDebug() << "Really used OpenGl: " << context()->format().majorVersion() << "." << context()->format().minorVersion();
    qDebug() << "OpenGl information: VENDOR:       " << (const char*)f->glGetString(GL_VENDOR);
    qDebug() << "                    RENDERDER:    " << (const char*)f->glGetString(GL_RENDERER);
    qDebug() << "                    VERSION:      " << (const char*)f->glGetString(GL_VERSION);
    qDebug() << "                    GLSL VERSION: " << (const char*)f->glGetString(GL_SHADING_LANGUAGE_VERSION);
    qDebug() << "Has GL_KHR_debug" << context()->hasExtension(QByteArray("GL_KHR_debug"));


    QImage img(":/qtlogo.png");
    Q_ASSERT(!img.isNull());
    delete m_texture;
    m_texture = new QOpenGLTexture(img.scaled(32, 36).mirrored());

    delete m_program;
    m_program = new QOpenGLShaderProgram;
    // Prepend the correct version directive to the sources. The rest is the
    // same, thanks to the common GLSL syntax.
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, versionedShaderCode(vertexShaderSource));
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, versionedShaderCode(fragmentShaderSource));
    m_program->link();

    m_projMatrixLoc = m_program->uniformLocation("projMatrix");
    m_camMatrixLoc = m_program->uniformLocation("camMatrix");
    m_worldMatrixLoc = m_program->uniformLocation("worldMatrix");
    m_myMatrixLoc = m_program->uniformLocation("myMatrix");

    // Create a VAO. Not strictly required for ES 3, but it is for plain OpenGL.
    delete m_vao;
    m_vao = new QOpenGLVertexArrayObject;
    if (m_vao->create())
        m_vao->bind();

    m_program->bind();
    delete m_vbo;
    m_vbo = new QOpenGLBuffer;
    m_vbo->create();
    m_vbo->bind();
    //m_vbo->allocate(m_logo.constData(), m_logo.count() * sizeof(GLfloat));
    m_vbo->allocate(m_triangleData.constData(), m_triangleData.count() * sizeof(GLfloat));
    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                             nullptr);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                             reinterpret_cast<void *>(3 * sizeof(GLfloat)));
    m_vbo->release();

    f->glEnable(GL_DEPTH_TEST);
    f->glEnable(GL_CULL_FACE);
}

void GLWidget::resizeGL(int w, int h)
{
    m_proj.setToIdentity();
    m_proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 100.0f);
    m_uniformsDirty = true;
}

void GLWidget::paintGL()
{
    // Now use QOpenGLExtraFunctions instead of QOpenGLFunctions as we want to
    // do more than what GL(ES) 2.0 offers.
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();

    f->glClearColor(0, 0, 0, 1);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program->bind();
    m_texture->bind();

    if (m_uniformsDirty) {
        m_uniformsDirty = false;
        QMatrix4x4 camera;
        camera.lookAt(m_eye, m_eye + m_target, QVector3D(0, 1, 0));
        m_program->setUniformValue(m_projMatrixLoc, m_proj);
        m_program->setUniformValue(m_camMatrixLoc, camera);
        QMatrix4x4 wm = m_world;
        wm.rotate(m_r, 1, 1, 0);
        m_program->setUniformValue(m_worldMatrixLoc, wm);
        QMatrix4x4 mm;
        mm.setToIdentity();
        mm.rotate(-m_r2, 1, 0, 0);
        m_program->setUniformValue(m_myMatrixLoc, mm);
    }

    f->glDrawArrays(GL_TRIANGLES, 0, m_triangleData.vertexCount());

    dumpErrors();
}

void GLWidget::dumpErrors() {
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();

    GLenum error = GL_NO_ERROR;

    do {
        error = f->glGetError();
        if (error != GL_NO_ERROR) {
            qDebug() << "Found error: "<< error;
        }
    } while (error != GL_NO_ERROR);
}
