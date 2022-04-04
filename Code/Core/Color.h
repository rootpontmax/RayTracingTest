#pragma once
#include <cstdint>

////////////////////////////////////////////////////////////////////////////////////////////////////
struct SColor
{
    SColor();
    SColor( const SColor& rhs );
    explicit SColor( float _r, float _g, float _b, float _a );
    explicit SColor( int _r, int _g, int _b, int _a );
    SColor& operator+=( const SColor& rhs );
    SColor& operator*=( float rhs );
    SColor& operator/=( float rhs );
    
    uint8_t GetByteR() const;
    uint8_t GetByteG() const;
    uint8_t GetByteB() const;
    uint8_t GetByteA() const;

    float r;
    float g;
    float b;
    float a;
};
////////////////////////////////////////////////////////////////////////////////////////////////////
SColor operator+( const SColor& lhs, const SColor& rhs );
SColor operator*( const SColor& lhs, float rhs );
SColor operator*( float lhs, const SColor& rhs );
SColor operator*( const SColor& lhs, const SColor& rhs );
SColor operator/( const SColor& lhs, float rhs );
SColor LerpColor( const SColor& from, const SColor& to, const float coef );
////////////////////////////////////////////////////////////////////////////////////////////////////
