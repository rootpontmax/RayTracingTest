////////////////////////////////////////////////////////////////////////////////////////////////////
// Класс синглтон файловой системы. Отвечает за определение путей к локальным данным.			  //
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>

namespace NCore
{
////////////////////////////////////////////////////////////////////////////////////////////////////
class CFileSystem
{
public:
	void	Init();
	std::string     GetResourceFileName( const char *pFileName ) const;
    std::string     GetDocumentFileName( const char *pFileName ) const;
    std::string     GetCacheFileName( const char *pFileName ) const;
    std::string     GetDevelopmentFileName( const char *pFileName ) const;
    
private:
	CFileSystem() {}
	~CFileSystem() {}
	CFileSystem( const CFileSystem& );
	CFileSystem& operator=( const CFileSystem& );

    std::string m_resourcePath;
    std::string m_documentPath;
    std::string m_cachePath;
    std::string m_developmentPath;
	
friend
	CFileSystem& FileSystem();
};
////////////////////////////////////////////////////////////////////////////////////////////////////
CFileSystem& FileSystem();
////////////////////////////////////////////////////////////////////////////////////////////////////
} // end of namespace
