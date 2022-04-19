#include "File.h"
#include <cwchar>
#include <cstring>

namespace NCore
{
////////////////////////////////////////////////////////////////////////////////////////////////////
CFile::CFile() :
    m_pData( nullptr ),
    m_size( -1 )
{}
////////////////////////////////////////////////////////////////////////////////////////////////////
CFile::CFile( const char *pName ) :
    m_pData( nullptr ),
    m_size( -1 )
{
    CHECK( OpenRead( pName ), "File not found", return );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
CFile::~CFile()
{
    Close();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CFile::Close()
{
    if( m_pData )
    {
        fclose( m_pData );
        m_pData = nullptr;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
bool CFile::OpenRead( const char *pName )
{
#pragma warning ( push )
#pragma warning ( disable : 4996 )
    m_pData = fopen( pName, "rb" );
#pragma warning ( pop )
    if( m_pData )
    {
        m_size = CalcFileSize( m_pData );
        return true;
    }
    else
        return false;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CFile::OpenRewrite( const char *pName )
{
#pragma warning ( push )
#pragma warning ( disable : 4996 )
    m_pData = fopen( pName, "wb" );
#pragma warning ( pop )
    ASSERT( m_pData, "Something wrong. File descriptor is empty" );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
bool CFile::IsOpen() const
{
    if( !m_pData )
        return false;
    else
        return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
bool CFile::Load( void *pBuffer )
{
    CHECK( pBuffer, "Buffer wasn't initialized", return false );
    CHECK( m_pData, "File wasn't open", return false );

    fseek( m_pData, 0, SEEK_SET);
    return ( 1 == fread( pBuffer, m_size, 1, m_pData ) );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
bool CFile::Save( void *pBuffer, const size_t size )
{
    CHECK( pBuffer, "Invalid buffer pointer", return false );
    CHECK( m_pData, "File wasn't open", return false );

    fseek( m_pData, 0, SEEK_SET);
    const size_t resSize = fwrite( pBuffer, 1, size, m_pData );
    ASSERT( resSize == size, "Can't write whole buffer" );
    return ( size == resSize );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
long CFile::GetPos() const
{
    return ftell( m_pData );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CFile::SetPos( long pos )
{
    fseek( m_pData, pos, SEEK_SET );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
bool CFile::Read( void *pBuffer, const size_t size )
{
    CHECK( pBuffer, "Buffer wasn't initialized", return false );
    CHECK( m_pData, "File wasn't open", return false );
    return ( 1 == fread( pBuffer, size, 1, m_pData ) );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CFile::WriteString( const wchar_t *pStr )
{
    CHECK( pStr, "Invalid string pointer", return );
    CHECK( m_pData, "File wasn't open", return );

    const wchar_t zero = 0;
    const int count = static_cast< int >( wcslen( pStr ) );
    for( int i = 0; i < count; ++i )
        Write< wchar_t >( pStr[i] );
    Write< wchar_t >( zero );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CFile::WriteString( const char *pStr )
{
    CHECK( pStr, "Invalid string pointer", return );
    CHECK( m_pData, "File wasn't open", return );

    const char zero = 0;
    const int count = static_cast< int >( strlen( pStr ) );
    for( int i = 0; i < count; ++i )
        Write< char >( pStr[i] );
    Write< char >( zero );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
bool CFile::Write( void *pBuffer, const size_t size )
{
    CHECK( pBuffer, "Invalid buffer pointer", return false );
    CHECK( m_pData, "File wasn't open", return false );

    const size_t resSize = fwrite( pBuffer, 1, size, m_pData );
    ASSERT( resSize == size, "Can't write whole buffer" );
    return ( size == resSize );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
size_t CFile::CalcFileSize( FILE *pFile )
{
    CHECK( pFile, "File must be opened", return -1 );

    fseek( pFile, 0, SEEK_END );
    const size_t size = ftell( pFile );
    fseek( pFile, 0, SEEK_SET );

    return size;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
} // end of namespace
