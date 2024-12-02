// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef TRIANGLE_DATA_H
#define TRIANGLE_DATA_H

#include <qopengl.h>
#include <QList>
#include <QVector3D>

class TriangleData
{
public:
    TriangleData();
    const GLfloat *constData() const { return m_data.constData(); }
    int count() const { return m_count; }
    int vertexCount() const { return m_count / 6; }

private:
    void triangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3);
    void add(const QVector3D &v, const QVector3D &n);

    QList<GLfloat> m_data;
    int m_count = 0;
};

#endif // TRIANGLE_DATA_H
