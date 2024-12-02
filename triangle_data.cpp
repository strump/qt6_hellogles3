// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "triangle_data.h"
#include <qmath.h>

TriangleData::TriangleData()
{
    m_data.resize(6 * 6); //6 points with 6 floats each

    const GLfloat x1 =  0.0f;
    const GLfloat y1 =  0.5f;
    const GLfloat x2 = -0.5f;
    const GLfloat y2 = -0.5f;
    const GLfloat x3 =  0.5f;
    const GLfloat y3 = -0.5f;

    triangle(x1, y1, x2, y2, x3, y3);
    triangle(x1, y1, x3, y3, x2, y2);
}

void TriangleData::add(const QVector3D &v, const QVector3D &n)
{
    GLfloat *p = m_data.data() + m_count;
    *p++ = v.x();
    *p++ = v.y();
    *p++ = v.z();
    *p++ = n.x();
    *p++ = n.y();
    *p++ = n.z();
    m_count += 6;
}

void TriangleData::triangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3)
{
    QVector3D n = QVector3D::normal(QVector3D(x3 - x1, y3 - y1, 0.0f), QVector3D(x2 - x1, y2 - y1, 0.0f));

    add(QVector3D(x1, y1, -0.05f), n);
    add(QVector3D(x2, y2, -0.05f), n);
    add(QVector3D(x3, y3, -0.05f), n);
}
