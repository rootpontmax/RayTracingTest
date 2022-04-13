////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////
struct Vec3
{    
    float x;
    float y;
    float z;

    explicit Vec3() : x( 0.0f ), y( 0.0f ), z( 0.0f ) {}
    explicit Vec3( float _x, float _y, float _z ) : x( _x ), y( _y ), z( _z ) {}

    Vec3&   operator=( const Vec3& rhs );
    Vec3&   operator*=( float rhs );
    Vec3&   operator/=( float rhs );
    Vec3&   operator+=( const Vec3& rhs );
    Vec3&   operator-=( const Vec3& rhs );
    Vec3&   operator*=( const Vec3& rhs );
    Vec3    operator-() const;
    float   Length() const;
    float   LengthSqr() const;
    Vec3    Normalized() const;
};
////////////////////////////////////////////////////////////////////////////////////////////////////
Vec3 operator*( const Vec3& lhs, float rhs );
Vec3 operator*( float lhs, const Vec3& rhs );
Vec3 operator/( const Vec3& lhs, float rhs );
Vec3 operator*( const Vec3& lhs, const Vec3& rhs );
Vec3 operator+( const Vec3& lhs, const Vec3& rhs );
Vec3 operator+( const Vec3& lhs, const float rhs );
Vec3 operator-( const Vec3& lhs, const Vec3& rhs );
Vec3 operator-( const Vec3& lhs, const float rhs );
bool operator!=( const Vec3& lhs, const Vec3& rhs );
float Dot( const Vec3& lhs, const Vec3& rhs );
////////////////////////////////////////////////////////////////////////////////////////////////////
