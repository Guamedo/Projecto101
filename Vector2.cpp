//
// Created by GPA on 15/05/2017.
//

#include <algorithm>
#include <cassert>
#include <stdio.h>
#include "Vector2.h"

const float Vector2::epsilon = 0.0001f;

Vector2::Vector2() {
    m_v[0] = 0.0f;
    m_v[1] = 0.0f;
}

Vector2::Vector2(float x, float y) {
    m_v[0] = x;
    m_v[1] = y;
}

Vector2::Vector2(const float *ptr) {
    m_v[0] = *ptr++;
    m_v[1] = *ptr;
}

Vector2::Vector2(const Vector2 & vec) {
    m_v[0] = vec.m_v[0];
    m_v[1] = vec.m_v[1];
}

Vector2 & Vector2::operator=(const Vector2 & vec) {
    if (&vec != this) {
        m_v[0] = vec.m_v[0];
        m_v[1] = vec.m_v[1];
    }
    return *this;
}

void Vector2::swap(Vector2 & vec) {
    std::swap(m_v, vec.m_v);
}

// Coordinate access
float Vector2::operator[](int i) const { return m_v[i]; }
float & Vector2::operator[](int i) { return m_v[i]; }

// Coordinate access.
float Vector2::x () const { return m_v[0]; }
float & Vector2::x () { return m_v[0]; };
float Vector2::y () const { return m_v[1]; };
float & Vector2::y () { return m_v[1]; };

// Arithmetic operations.
void Vector2::operator +=(const Vector2 & rhs) {
    m_v[0] += rhs.m_v[0];
    m_v[1] += rhs.m_v[1];
}

void Vector2::operator -=(const Vector2 & rhs) {
    m_v[0] -= rhs.m_v[0];
    m_v[1] -= rhs.m_v[1];
}

void Vector2::operator /=(float scalar) {
    m_v[0] /= scalar;
    m_v[1] /= scalar;
}

void Vector2::operator *=(float scalar) {
    m_v[0] *= scalar;
    m_v[1] *= scalar;
}

Vector2 operator+(const Vector2& lhs, const Vector2& rhs) {
    return Vector2(lhs.m_v[0] + rhs.m_v[0],
                   lhs.m_v[1] + rhs.m_v[1]);
};

Vector2 operator-(const Vector2& lhs, const Vector2& rhs) {
    return Vector2(lhs.m_v[0] - rhs.m_v[0],
                   lhs.m_v[1] - rhs.m_v[1]);
};

// component-wise multiplication
Vector2 operator*(const Vector2& lhs, const Vector2& rhs) {
    return Vector2(lhs.m_v[0] * rhs.m_v[0],
                   lhs.m_v[1] * rhs.m_v[1]);
}

Vector2 operator*(const Vector2& vec, float scalar) {
    return Vector2(vec.m_v[0] *scalar,
                   vec.m_v[1] *scalar);
}

Vector2 operator*(float scalar, const Vector2& vec) {
    return Vector2(vec.m_v[0] *scalar,
                   vec.m_v[1] *scalar);
}

Vector2 operator/ (const Vector2& vec, float scalar) {
    assert(scalar != 0.0f);
    return Vector2(vec.m_v[0] / scalar,
                   vec.m_v[1] / scalar);
}

float Vector2::dot(const Vector2& rhs) const {
    return m_v[0] * rhs.m_v[0] + m_v[1] * rhs.m_v[1];
}

float Vector2::length() const {
    return sqrtf(m_v[0] * m_v[0] + m_v[1] * m_v[1]);
}

float Vector2::lengthSquare() const {
    return m_v[0] * m_v[0] + m_v[1] * m_v[1];
}

// Set vector to unit length.
// Returns the old length
// If zero vector, set to UNIT_Z vector.
float Vector2::normalize() {
    float mod = 0.0f;
    float mod2 = m_v[0]*m_v[0] + m_v[1]*m_v[1];
    if( mod2 > epsilon ) {
        mod = 1.0f / sqrtf( mod2 );
        m_v[0] *= mod;
        m_v[1] *= mod;
    }
    else {
        m_v[0] = 0.0f;
        m_v[1] = 1.0f;
    }
    return mod;
}

void Vector2::print() const {
    printf("(%.4f, %.4f)", m_v[0], m_v[1]);
};