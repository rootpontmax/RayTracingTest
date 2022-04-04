////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  main.cpp
//  RayTracer
//
//  Created by Michael Scherbakov on 01.04.2022.
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <vector>
#include <cfloat>
#include <chrono>


#include "../Code/Core/Assert.h"
#include "../Code/Core/Color.h"
#include "../Code/RayTrace/Sphere.h"
#include "../Code/RayTrace/Ray.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
static const int SCREEN_SIZE_X = 640;
static const int SCREEN_SIZE_Y = 480;
static const float SCREEN_ASPECT = static_cast< float >( SCREEN_SIZE_X ) / static_cast< float >( SCREEN_SIZE_Y );
////////////////////////////////////////////////////////////////////////////////////////////////////
static void MakeTestGradient( uint8_t *pBuffer, const int sizeX, const int sizeY )
{
    CHECK( pBuffer, "Invalid buffer pointer", return );
    
    const float fullX = static_cast< float >( sizeX - 1 );
    const float fullY = static_cast< float >( sizeY - 1 );
    for( int y = 0; y < sizeY; ++y )
        for( int x = 0; x < sizeX; ++x )
        {
            const float coefX = static_cast< float >( x ) / fullX;
            const float coefY = static_cast< float >( y ) / fullY;
            
            const int offset = ( y * sizeX + x ) * 3;
            const uint8_t colX = static_cast< uint8_t >( 255 * coefX );
            const uint8_t colY = static_cast< uint8_t >( 255 * coefY );
            pBuffer[offset  ] = 255;
            pBuffer[offset+1] = colX;
            pBuffer[offset+2] = colY;
        }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
static void WriteToPPM( const char *pFilename, const uint8_t *pBuffer, const int sizeX, const int sizeY )
{
    CHECK( pBuffer, "Invalid buffer pointer", return );
    
    std::ofstream ofs;
    
    ofs.open(pFilename, std::ios::out);
    
    // Header
    ofs << "P3" << std::endl;
    ofs << sizeX << " " << sizeY << std::endl;
    ofs << "255" << std::endl;
    
    // Data
    for( int y = ( sizeY - 1 ); y >= 0; --y )
        for( int x = 0; x < sizeX; ++x )
        {
            const int offset = ( y * sizeX + x ) * 3;
            const int r = pBuffer[offset];
            const int g = pBuffer[offset+1];
            const int b = pBuffer[offset+2];
            ofs << r << " " << g << " " << b << std::endl;
        }
    
    ofs.close();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
SColor GetSceneColor( const NRayTrace::SRay& ray, const std::vector< const NRayTrace::IHitable* >& scene )
{
    NRayTrace::SHitInfo workInfo;
    NRayTrace::SHitInfo trueInfo;
    float minT = FLT_MAX;
    bool bWasFoundAnything = false;
    for( size_t i = 0; i < scene.size(); ++i )
    {
        if( scene[i]->HasHit( &workInfo, ray, 0.0f, 100.0f ) && workInfo.t < minT )
        {
            trueInfo = workInfo;
            minT = workInfo.t;
            bWasFoundAnything = true;
        }
    }
    
    if( bWasFoundAnything )
    {
        const Vec3 norCol = trueInfo.nor * 0.5f + 0.5f;
        return SColor( norCol.x, norCol.y, norCol.z, 1.0f );
    }
    
    return SColor( 0.0f, 0.0f, 0.0f, 1.0f );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, const char * argv[])
{
    // Buffer
    std::unique_ptr< uint8_t[] >buffer( new uint8_t[SCREEN_SIZE_X * SCREEN_SIZE_Y * 3] );
    
    // Scene
    NRayTrace::CSphere sphereA( Vec3( 0.0f, 0.0f, -1.0f ), 0.5f );
    NRayTrace::CSphere sphereB( Vec3( 0.0f, -100.5f, -1.0f ), 100.0f );
    std::vector< const NRayTrace::IHitable* > scene;
    scene.reserve(2);
    scene.push_back( &sphereA );
    scene.push_back( &sphereB );
    
    // Camera
    const float viewPortSizeY = 2.0f;
    const float viewPortSizeX = viewPortSizeY * SCREEN_ASPECT;
    const float focaLlength = 1.0f;
    
    const Vec3 camPos = Vec3( 0.0f, 0.0f, 0.0f );
    const Vec3 screenVecX = Vec3( viewPortSizeX, 0.0f, 0.0f );
    const Vec3 screenVecY = Vec3( 0.0f, viewPortSizeY, 0.0f );
    const Vec3 lowerLeftCorner = camPos - screenVecX * 0.5f - screenVecY * 0.5f - Vec3( 0.0f, 0.0f, focaLlength );
    
    // Render
    //MakeTestGradient( buffer.get(), SCREEN_SIZE_X, SCREEN_SIZE_Y );
    const float fullX = static_cast< float >( SCREEN_SIZE_X - 1 );
    const float fullY = static_cast< float >( SCREEN_SIZE_Y - 1 );
    
    std::cout << "Creating image\n";
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    
    for( int y = ( SCREEN_SIZE_Y - 1 ); y >= 0; --y )
        for( int x = 0; x < SCREEN_SIZE_X; ++x )
        {
            const float u = static_cast< float >( x ) / fullX;
            const float v = static_cast< float >( y ) / fullY;
            const Vec3 dir = lowerLeftCorner + u * screenVecX + v * screenVecY;
            const NRayTrace::SRay ray( camPos, dir );
            const SColor col = GetSceneColor( ray, scene );
            
            const int offset = ( y * SCREEN_SIZE_X + x ) * 3;
            buffer.get()[offset  ] = col.GetByteR();
            buffer.get()[offset+1] = col.GetByteG();
            buffer.get()[offset+2] = col.GetByteB();
        }
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> duration = t2 - t1;
    std::cout << "Buffer created for " << duration.count() << " ms" << std::endl;
    
    WriteToPPM( "RayTest.ppm", buffer.get(), SCREEN_SIZE_X, SCREEN_SIZE_Y );
    
    
    std::cout << "Done!\n";
    
    return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
