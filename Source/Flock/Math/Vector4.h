//
// Copyright (c) 2008-2017 Flock SDK developers & contributors. 
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#pragma once

#include "../Math/Vector3.h"

namespace FlockSDK
{

/// Four-dimensional vector.
class FLOCKSDK_API Vector4
{
public:
    /// Construct a zero vector.
    Vector4() :
        x_(0.0f),
        y_(0.0f),
        z_(0.0f),
        w_(0.0f)
    {
    }

    /// Copy-construct from another vector.
    Vector4(const Vector4 &vector) :
        x_(vector.x_),
        y_(vector.y_),
        z_(vector.z_),
        w_(vector.w_)
    {
    }

    /// Construct from a 3-dimensional vector and the W coordinate.
    Vector4(const Vector3 &vector, float w) :
        x_(vector.x_),
        y_(vector.y_),
        z_(vector.z_),
        w_(w)
    {
    }

    /// Construct from coordinates.
    Vector4(float x, float y, float z, float w) :
        x_(x),
        y_(y),
        z_(z),
        w_(w)
    {
    }

    /// Construct from a float array.
    explicit Vector4(const float* data) :
        x_(data[0]),
        y_(data[1]),
        z_(data[2]),
        w_(data[3])
    {
    }

    /// Assign from another vector.
    Vector4 &operator =(const Vector4 &rhs)
    {
        x_ = rhs.x_;
        y_ = rhs.y_;
        z_ = rhs.z_;
        w_ = rhs.w_;
        return *this;
    }

    /// Test for equality with another vector without epsilon.
    bool operator ==(const Vector4 &rhs) const { return x_ == rhs.x_ && y_ == rhs.y_ && z_ == rhs.z_ && w_ == rhs.w_; }

    /// Test for inequality with another vector without epsilon.
    bool operator !=(const Vector4 &rhs) const { return x_ != rhs.x_ || y_ != rhs.y_ || z_ != rhs.z_ || w_ != rhs.w_; }

    /// Add a vector.
    Vector4 operator +(const Vector4 &rhs) const { return Vector4(x_ + rhs.x_, y_ + rhs.y_, z_ + rhs.z_, w_ + rhs.w_); }

    /// Return negation.
    Vector4 operator -() const { return Vector4(-x_, -y_, -z_, -w_); }

    /// Subtract a vector.
    Vector4 operator -(const Vector4 &rhs) const { return Vector4(x_ - rhs.x_, y_ - rhs.y_, z_ - rhs.z_, w_ - rhs.w_); }

    /// Multiply with a scalar.
    Vector4 operator *(float rhs) const { return Vector4(x_ * rhs, y_ * rhs, z_ * rhs, w_ * rhs); }

    /// Multiply with a vector.
    Vector4 operator *(const Vector4 &rhs) const { return Vector4(x_ * rhs.x_, y_ * rhs.y_, z_ * rhs.z_, w_ * rhs.w_); }

    /// Divide by a scalar.
    Vector4 operator /(float rhs) const { return Vector4(x_ / rhs, y_ / rhs, z_ / rhs, w_ / rhs); }

    /// Divide by a vector.
    Vector4 operator /(const Vector4 &rhs) const { return Vector4(x_ / rhs.x_, y_ / rhs.y_, z_ / rhs.z_, w_ / rhs.w_); }

    /// Add-assign a vector.
    Vector4 &operator +=(const Vector4 &rhs)
    {
        x_ += rhs.x_;
        y_ += rhs.y_;
        z_ += rhs.z_;
        w_ += rhs.w_;
        return *this;
    }

    /// Subtract-assign a vector.
    Vector4 &operator -=(const Vector4 &rhs)
    {
        x_ -= rhs.x_;
        y_ -= rhs.y_;
        z_ -= rhs.z_;
        w_ -= rhs.w_;
        return *this;
    }

    /// Multiply-assign a scalar.
    Vector4 &operator *=(float rhs)
    {
        x_ *= rhs;
        y_ *= rhs;
        z_ *= rhs;
        w_ *= rhs;
        return *this;
    }

    /// Multiply-assign a vector.
    Vector4 &operator *=(const Vector4 &rhs)
    {
        x_ *= rhs.x_;
        y_ *= rhs.y_;
        z_ *= rhs.z_;
        w_ *= rhs.w_;
        return *this;
    }

    /// Divide-assign a scalar.
    Vector4 &operator /=(float rhs)
    {
        float invRhs = 1.0f / rhs;
        x_ *= invRhs;
        y_ *= invRhs;
        z_ *= invRhs;
        w_ *= invRhs;
        return *this;
    }

    /// Divide-assign a vector.
    Vector4 &operator /=(const Vector4 &rhs)
    {
        x_ /= rhs.x_;
        y_ /= rhs.y_;
        z_ /= rhs.z_;
        w_ /= rhs.w_;
        return *this;
    }

    /// Calculate dot product.
    float DotProduct(const Vector4 &rhs) const { return x_ * rhs.x_ + y_ * rhs.y_ + z_ * rhs.z_ + w_ * rhs.w_; }

    /// Calculate absolute dot product.
    float AbsDotProduct(const Vector4 &rhs) const
    {
        return FlockSDK::Abs(x_ * rhs.x_) + FlockSDK::Abs(y_ * rhs.y_) + FlockSDK::Abs(z_ * rhs.z_) + FlockSDK::Abs(w_ * rhs.w_);
    }

    /// Project vector onto axis.
    float ProjectOntoAxis(const Vector3 &axis) const { return DotProduct(Vector4(axis.Normalized(), 0.0f)); }

    /// Return absolute vector.
    Vector4 Abs() const { return Vector4(FlockSDK::Abs(x_), FlockSDK::Abs(y_), FlockSDK::Abs(z_), FlockSDK::Abs(w_)); }

    /// Linear interpolation with another vector.
    Vector4 Lerp(const Vector4 &rhs, float t) const { return *this * (1.0f - t) + rhs * t; }

    /// Test for equality with another vector with epsilon.
    bool Equals(const Vector4 &rhs) const
    {
        return FlockSDK::Equals(x_, rhs.x_) && FlockSDK::Equals(y_, rhs.y_) && FlockSDK::Equals(z_, rhs.z_) && FlockSDK::Equals(w_, rhs.w_);
    }

    /// Return whether is NaN.
    bool IsNaN() const { return FlockSDK::IsNaN(x_) || FlockSDK::IsNaN(y_) || FlockSDK::IsNaN(z_) || FlockSDK::IsNaN(w_); }

    /// Return float data.
    const float* Data() const { return &x_; }

    /// Return as string.
    String ToString() const;

    /// X coordinate.
    float x_;
    /// Y coordinate.
    float y_;
    /// Z coordinate.
    float z_;
    /// W coordinate.
    float w_;

    /// Zero vector.
    static const Vector4 ZERO;
    /// (1,1,1) vector.
    static const Vector4 ONE;
};

/// Multiply Vector4 with a scalar.
inline Vector4 operator *(float lhs, const Vector4 &rhs) { return rhs * lhs; }

/// Per-component linear interpolation between two 4-vectors.
inline Vector4 VectorLerp(const Vector4 &lhs, const Vector4 &rhs, const Vector4 &t) { return lhs + (rhs - lhs) * t; }

/// Per-component min of two 4-vectors.
inline Vector4 VectorMin(const Vector4 &lhs, const Vector4 &rhs) { return Vector4(Min(lhs.x_, rhs.x_), Min(lhs.y_, rhs.y_), Min(lhs.z_, rhs.z_), Min(lhs.w_, rhs.w_)); }

/// Per-component max of two 4-vectors.
inline Vector4 VectorMax(const Vector4 &lhs, const Vector4 &rhs) { return Vector4(Max(lhs.x_, rhs.x_), Max(lhs.y_, rhs.y_), Max(lhs.z_, rhs.z_), Max(lhs.w_, rhs.w_)); }

/// Per-component floor of 4-vector.
inline Vector4 VectorFloor(const Vector4 &vec) { return Vector4(Floor(vec.x_), Floor(vec.y_), Floor(vec.z_), Floor(vec.w_)); }

/// Per-component round of 4-vector.
inline Vector4 VectorRound(const Vector4 &vec) { return Vector4(Round(vec.x_), Round(vec.y_), Round(vec.z_), Round(vec.w_)); }

/// Per-component ceil of 4-vector.
inline Vector4 VectorCeil(const Vector4 &vec) { return Vector4(Ceil(vec.x_), Ceil(vec.y_), Ceil(vec.z_), Ceil(vec.w_)); }

}
