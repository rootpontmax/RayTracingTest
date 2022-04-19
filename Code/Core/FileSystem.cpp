#include "FileSystem.h"
#include "Assert.h"
#include "Platform.h"

#include <iostream>

namespace NCore
{
////////////////////////////////////////////////////////////////////////////////////////////////////
void CFileSystem::Init()
{
#if defined( MS_PLATFORM_WIN32 )
    const NStd::wstring cacheFolder = L"Cache/";
	m_resourcePath = L"";
    m_documentPath = L"";
    m_developmentPath = L"";
    m_cachePath = m_resourcePath + cacheFolder;
#elif defined( MS_PLATFORM_IOS ) || ( defined( MS_PLATFORM_OSX ) && defined( STAND_ALONE ) )
	CFBundleRef pBundle = CFBundleGetMainBundle();
	ASSERT( pBundle, "Invalid bundle pointer" );
	CFURLRef pURL = CFBundleCopyBundleURL( pBundle );
	ASSERT( pURL, "Invalid URL pointer" );
	const size_t bufSize = 1000;
	char buffer[bufSize];
	VERIFY( TRUE == CFURLGetFileSystemRepresentation( pURL, TRUE, (UInt8 *)buffer, bufSize ) );
	CFRelease(pURL);
	
	std::string ansiPath( buffer );
 
#if defined( STAND_ALONE )
    std::string binPath( "/Contents/Bin/" );
#else   
	std::string binPath( "/Bin/" );
#endif
    m_resourcePath = ansiPath + binPath;
	
    // Documents Path
    {
        NSArray * const allPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString * const dirPath = [allPaths objectAtIndex:0];
        NSString * const filePath = [NSString stringWithFormat:@"%@/", dirPath];
        const char * const szFilePath = [[NSFileManager defaultManager] fileSystemRepresentationWithPath: filePath];
        
        std::string ansiPath( szFilePath );
        m_documentPath = ansiPath;
    }
    
    // Cache path
    {
        NSArray * const allPaths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
        NSString * const dirPath = [allPaths objectAtIndex:0];
        NSString * const filePath = [NSString stringWithFormat:@"%@/", dirPath];
        const char * const szFilePath = [[NSFileManager defaultManager] fileSystemRepresentationWithPath: filePath];
        
        std::string ansiPath( szFilePath );
        m_cachePath = ansiPath;
    }
#endif


#if !defined( MS_PLATFORM_WIN32 )
    // Для нужд разработки получаем локальный путь к исходному коду

	// Для того, чтобы определить папку проекта используем переменную SOURCE_ROOT_PATH, которая
	// определяется в настройкаъ проекта, в разделе "GCC 4.2 Language" -> "Other C Flags" и
	// "Other C++ Flags". В этих разделах пишем следующую строку:
	// -DSOURCE_ROOT_PATH="\"$(SOURCE_ROOT)\""
	// Более подробно об этом читай по ссылке:
	// http://developer.apple.com/qa/qa2007/qa1549.html
	//
    
    
	
	// От строки необходимо отрезать часть, которая соответствует папки "__MacSpec", т.е.
	// необходимо уменьшить длину строки на 9 символов, а затем прибавить строку "Bin\"
	char devBuffer[1000];
 
	strcpy( devBuffer, SOURCE_ROOT_PATH );
	size_t len = strlen( devBuffer );
	ASSERT( len >= 9, "SOURCE_ROOT_PATH is empty or is not correct" );
    
    int slashPos = static_cast< int >( len - 1 );
    while( slashPos >= 0 )
    {
        if( '/' == devBuffer[slashPos] )
            break;
        --slashPos;
    }
    
    
    //std::cout << "Full path: " << SOURCE_ROOT_PATH << std::endl;
    
    devBuffer[ slashPos + 1 ] = 'B';
    devBuffer[ slashPos + 2 ] = 'i';
    devBuffer[ slashPos + 3 ] = 'n';
    devBuffer[ slashPos + 4 ] = '/';
    devBuffer[ slashPos + 5 ] = 0;
	
    /*
	devBuffer[ len - 9 ] = 'B';
	devBuffer[ len - 8 ] = 'i';
	devBuffer[ len - 7 ] = 'n';
	devBuffer[ len - 6 ] = '/';
	devBuffer[ len - 5 ] = 0;
    */
    
	std::string ansiDevPath( devBuffer );
    m_developmentPath = ansiDevPath;
#endif
    
#if defined( MS_PLATFORM_IOS_SIMULATOR ) || ( defined( MS_PLATFORM_OSX ) && !defined( STAND_ALONE ) )
	m_resourcePath = m_developmentPath;
    m_documentPath = m_resourcePath;
    const std::string cacheFolder = "Cache/";
    m_cachePath = m_resourcePath + cacheFolder;
#endif

    //std::cout << "Resource path: " << m_resourcePath << std::endl;
    //std::cout << "Document path: " << m_documentPath << std::endl;
    //std::cout << "Cache path: " << m_cachePath << std::endl;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
std::string CFileSystem::GetResourceFileName( const char *pFileName ) const
{
	ASSERT( pFileName, "The string is empty" );
	std::string fileName( pFileName );
	std::string fullPath = m_resourcePath + fileName;
	return fullPath;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
std::string CFileSystem::GetDocumentFileName( const char *pFileName ) const
{
    ASSERT( pFileName, "The string is empty" );
	std::string fileName( pFileName );
	std::string fullPath = m_documentPath + fileName;
	return fullPath;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
std::string CFileSystem::GetCacheFileName( const char *pFileName ) const
{
    ASSERT( pFileName, "The string is empty" );
	std::string fileName( pFileName );
	std::string fullPath = m_cachePath + fileName;
	return fullPath;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
std::string CFileSystem::GetDevelopmentFileName( const char *pFileName ) const
{
    ASSERT( pFileName, "The string is empty" );
    std::string fileName( pFileName );
    std::string fullPath = m_developmentPath + fileName;
    return fullPath;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
CFileSystem& FileSystem()
{
	static CFileSystem thisSystem;
	return thisSystem;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
} // end of namespace
