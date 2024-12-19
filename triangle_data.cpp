// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "triangle_data.h"
#include <qmath.h>

TriangleData::TriangleData()
{
    m_data.resize(6 * 3); //6 points with 3 floats each

    const GLfloat x1 =  0.0f;
    const GLfloat y1 =  0.5f;
    const GLfloat x2 = -0.5f;
    const GLfloat y2 = -0.5f;
    const GLfloat x3 =  0.5f;
    const GLfloat y3 = -0.5f;

    triangle(x1, y1, x2, y2, x3, y3);
    triangle(x1, y1, x3, y3, x2, y2);
}

void TriangleData::add(const QVector3D &v)
{
    GLfloat *p = m_data.data() + m_count;
    *p++ = v.x();
    *p++ = v.y();
    *p++ = v.z();
    m_count += 3;
}

void TriangleData::triangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3)
{
    add(QVector3D(x1, y1, -0.05f));
    add(QVector3D(x2, y2, -0.05f));
    add(QVector3D(x3, y3, -0.05f));
}
