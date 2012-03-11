#pragma once

//dll読み込みヘルパー
class LoadLibraryHelper
{
private:
	//DLL インスタンス.
	HMODULE DllInstance;

public:
	LoadLibraryHelper()
	{
		this->DllInstance = NULL;
	}
	virtual ~LoadLibraryHelper()
	{
		if (this->DllInstance != NULL)
		{
			::FreeLibrary(this->DllInstance);
			this->DllInstance = NULL;
		}
	}
	bool Load( const std::string& inDLLName )//std読んでいない化石環境とかのために const char* で作る.
	{
		this->DllInstance = ::LoadLibraryA(inDLLName.c_str() );
		if ( this->DllInstance == NULL )
		{
			DWORD lastError = ::GetLastError();
			return true;
		}
		return true;
	}
	FARPROC GetProcAddress(const char* inProcName)
	{
		return ::GetProcAddress(this->DllInstance,inProcName);
	}
};
