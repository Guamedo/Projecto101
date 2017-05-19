//
// Created by GPA on 15/05/2017.
//
#include <algorithm>
#include <cassert>
#include <stdio.h>
#ifndef PROJECT101_VECTOR2_H
#define PROJECT101_VECTOR2_H


class Vector2 {
public:
    static const float epsilon;

    // Constructors
    Vector2();
    Vector2(float x, float y);
    Vector2(const float *ptr);
    Vector2(const Vector2 & vec);
    Vector2 & operator=(const Vector2 & vec);

    // swap two vectors
    void swap(Vector2 & vec);

    // Coordinate access
    float operator[](int idx) const;
    float & operator[](int idx);
    float x () const;
    float & x ();
    float y () const;
    float& y ();

    // Arithmetic operations.
    friend Vector2 operator+(const Vector2& lhs, const Vector2& rhs);
    friend Vector2 operator-(const Vector2& lhs, const Vector2& rhs);
    friend Vector2 operator*(const Vector2& lhs, const Vector2& rhs);
    friend Vector2 operator*(const Vector2& vec, float scalar);
    friend Vector2 operator*(float scalar, const Vector2& vec);
    friend Vector2 operator/(const Vector2& vec, float scalar);
    friend Vector2 operator/(float scalar, const Vector2& vec);
    void operator +=(const Vector2 & rhs);
    void operator -=(const Vector2 & rhs);
    void operator *=(float scalar);
    void operator /=(float scalar);

    float dot(const Vector2& rhs) const;

    float length() const; // vector module
    float lengthSquare() const; // square of length

    // Set vector to unit length.
    // Returns the old length
    // If zero vector, set to UNIT_Z vector.
    float normalize();

    void print() const;

private:
    float m_v[2];

};


#endif //PROJECT101_VECTOR2_H
