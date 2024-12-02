// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef TRIANGLE_DATA_H
#define TRIANGLE_DATA_H

#include <qopengl.h>
#include <QList>
#include <QVector3D>

class TriangleData2D
{
public:
    TriangleData2D();
    const GLfloat *constData() const { return m_data.constData(); }
    int count() const { return m_count; }
    int vertexCount() const { return m_count / 3; }

private:
    void triangle2D(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3);
    void add(const QVector3D &v);

    QList<GLfloat> m_data;
    int m_count = 0;
};

#endif // TRIANGLE_DATA_H
