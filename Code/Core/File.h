////////////////////////////////////////////////////////////////////////////////////////////////////
// Класс для работы с файлами и вспомогательные функции.                                          //
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <stdio.h>
#include "Assert.h"

namespace NCore
{
////////////////////////////////////////////////////////////////////////////////////////////////////
class CFile
{
public:
    explicit CFile();
    explicit CFile( const char *pName );
    ~CFile();

    void    Close();

    bool    OpenRead( const char *pName );
    void    OpenRewrite( const char *pName );
    
    bool    IsOpen() const;
    size_t  GetSize() const { return m_size; }

    bool    Load( void *pBuffer );
    bool    Save( void *pBuffer, const size_t size );
    
    long    GetPos() const;
    void    SetPos( long pos );
    
    bool    Read( void *pBuffer, const size_t size );
    void    WriteString( const wchar_t *pStr );
    void    WriteString( const char *pStr );
    bool    Write( void *pBuffer, const size_t size );
    
    // Шаблонные функции для чтения типизированных данных из файлового потока
    template< typename T >
    bool Read( T *pValue )
    {
        ASSERT( m_pData, "File wasn't open" );
        ASSERT( pValue, "Input value. Pointer must be valid" );
        const size_t refSize = sizeof( T );
        const size_t readSize = fread( pValue, 1, refSize, m_pData );
        return ( refSize == readSize );
    }
    
    template< typename T >
    T Read()
    {        
        ASSERT( m_pData, "File wasn't open" );
        T data;
        const size_t refSize = sizeof( T );
        const size_t readSize = fread( &data, 1, refSize, m_pData );
        ASSERT( refSize == readSize, "Can't read enough bytes. End of file" );
        return data;
    }

    template< typename T >
    void Write( const T& value )
    {
        ASSERT( m_pData, "File wasn't open" );
        const size_t refSize = sizeof( T );
        const size_t resSize = fwrite( &value, 1, refSize, m_pData );
        ASSERT( refSize == resSize, "Can't write enough bytes. End of file" );
    }

    

private:
    size_t  CalcFileSize( FILE *pFile );

    FILE       *m_pData;
    size_t      m_size;
};
////////////////////////////////////////////////////////////////////////////////////////////////////
} // end of namespace
