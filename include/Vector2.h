#pragma once
#include <cmath>

struct Vector2 {
    float x, y;

    // Constructors
    constexpr Vector2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

    // Addition
    Vector2 operator+(const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2& operator+=(const Vector2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    // Subtraction
    Vector2 operator-(const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }

    Vector2& operator-=(const Vector2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    // Scalar multiplication
    Vector2 operator*(float scalar) const {
        return Vector2(x * scalar, y * scalar);
    }

    Vector2& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    // Scalar division
    Vector2 operator/(float scalar) const {
        return Vector2(x / scalar, y / scalar);
    }

    Vector2& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    // Vector length (magnitude)
    float length() const {
        return std::sqrt(x * x + y * y);
    }

    // Normalize the vector (make length 1)
    Vector2 normalized() const {
        float len = length();
        if (len == 0) return Vector2(0, 0);
        return Vector2(x / len, y / len);
    }

    // Dot product
    float dot(const Vector2& other) const {
        return x * other.x + y * other.y;
    }
};
