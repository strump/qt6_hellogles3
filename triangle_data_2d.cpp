// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "triangle_data_2d.h"
#include <qmath.h>

TriangleData2D::TriangleData2D()
{
    m_data.resize(3 * 3); //6 points with 3 floats each

    triangle2D( 0.0f,  0.5f, 0.0f,
               -0.5f, -0.5f, 0.0f,
                0.5f, -0.5f, 0.0f);
}

void TriangleData2D::add(const QVector3D &v)
{
    GLfloat *p = m_data.data() + m_count;
    *p++ = v.x();
    *p++ = v.y();
    *p++ = v.z();
    m_count += 3;
}

void TriangleData2D::triangle2D(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3)
{
    add(QVector3D(x1, y1, z1));
    add(QVector3D(x2, y2, z2));
    add(QVector3D(x3, y3, z3));
}
