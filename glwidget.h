// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QVector3D>
#include "logo.h"
#include "triangle_data.h"

QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)
QT_FORWARD_DECLARE_CLASS(QOpenGLBuffer)
QT_FORWARD_DECLARE_CLASS(QOpenGLVertexArrayObject)

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT
    Q_PROPERTY(float z READ z WRITE setZ)
    Q_PROPERTY(float r READ r WRITE setR)
    Q_PROPERTY(float r2 READ r2 WRITE setR2)

public:
    GLWidget();
    ~GLWidget();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    float z() const { return m_eye.z(); }
    void setZ(float v);

    float r() const { return m_r; }
    void setR(float v);
    float r2() const { return m_r2; }
    void setR2(float v);
    void dumpErrors();
private slots:
    void startSecondStage();
private:
    QOpenGLShaderProgram *m_program = nullptr;
    QOpenGLBuffer *m_vbo = nullptr;
    QOpenGLVertexArrayObject *m_vao = nullptr;
    Logo m_logo;
    TriangleData m_triangleData;
    int m_projMatrixLoc = 0;
    int m_camMatrixLoc = 0;
    int m_worldMatrixLoc = 0;
    int m_myMatrixLoc = 0;
    QMatrix4x4 m_proj;
    QMatrix4x4 m_world;
    QVector3D m_eye;
    QVector3D m_target = {0, 0, -1};
    bool m_uniformsDirty = true;
    float m_r = 0;
    float m_r2 = 0;
};

#endif
