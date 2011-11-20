// FMod.cpp: FMod クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "FMod.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

FMod::FMod()
{
	RefCounter = 0;
}

FMod::~FMod()
{

}

FMod* FMod::getInstance()
{
	static FMod	p;
	return &p;
}


//!初期化できなかったら RDoNotInitaliseException
//!関数が読み込めなかったら RDoNotInitaliseException
void FMod::Create() throw() 
{
	AutoLib.Create("fmod.dll") ;
	FSOUND_Close = (FSOUND_CLOSE) AutoLib.GetProcAddress("_FSOUND_Close@0");
	FSOUND_Stream_Close = (FSOUND_STREAM_CLOSE) AutoLib.GetProcAddress("_FSOUND_Stream_Close@4");
	FSOUND_Stream_GetLengthMs = (FSOUND_STREAM_GETLENGTHMS) AutoLib.GetProcAddress("_FSOUND_Stream_GetLengthMs@4");
	FSOUND_Stream_GetTime = (FSOUND_STREAM_GETTIME) AutoLib.GetProcAddress("_FSOUND_Stream_GetTime@4");
	FSOUND_Stream_GetLength = (FSOUND_STREAM_GETLENGTH) AutoLib.GetProcAddress("_FSOUND_Stream_GetLength@4");
	FSOUND_Stream_GetPosition = (FSOUND_STREAM_GETPOSITION) AutoLib.GetProcAddress("_FSOUND_Stream_GetPosition@4");
	FSOUND_Stream_SetTime = (FSOUND_STREAM_SETTIME) AutoLib.GetProcAddress("_FSOUND_Stream_SetTime@8");
	FSOUND_SetPaused = (FSOUND_SETPAUSED) AutoLib.GetProcAddress("_FSOUND_SetPaused@8");
	FSOUND_Stream_PlayEx = (FSOUND_STREAM_PLAYEX) AutoLib.GetProcAddress("_FSOUND_Stream_PlayEx@16");
	FSOUND_Sample_GetName = (FSOUND_SAMPLE_GETNAME) AutoLib.GetProcAddress("_FSOUND_Sample_GetName@4");
	FSOUND_Sample_GetDefaults = (FSOUND_SAMPLE_GETDEFAULTS) AutoLib.GetProcAddress("_FSOUND_Sample_GetDefaults@20");
	FSOUND_Stream_GetSample = (FSOUND_SAMPLE_GETSAMPLE) AutoLib.GetProcAddress("_FSOUND_Stream_GetSample@4");
	FSOUND_Stream_SetSynchCallback = (FSOUND_SAMPLE_SETSYNCHCALLBACK) AutoLib.GetProcAddress("_FSOUND_Stream_SetSynchCallback@12");
	FSOUND_Stream_SetEndCallback = (FSOUND_SAMPLE_SETENDCALLBACK) AutoLib.GetProcAddress("_FSOUND_Stream_SetEndCallback@12");
	FSOUND_GetError = (FSOUND_GETERROR) AutoLib.GetProcAddress("_FSOUND_GetError@0");
	FSOUND_SetDriver = (FSOUND_SETDRIVER) AutoLib.GetProcAddress("_FSOUND_SetDriver@4");
	FSOUND_GetDriver = (FSOUND_GETDRIVER) AutoLib.GetProcAddress("_FSOUND_GetDriver@0");
	FSOUND_GetDriverName = (FSOUND_SETDRIVERNAME) AutoLib.GetProcAddress("_FSOUND_GetDriverName@4");
	FSOUND_GetNumDrivers = (FSOUND_GETNUMDRIVERS) AutoLib.GetProcAddress("_FSOUND_GetNumDrivers@0");
	FSOUND_GetOutput = (FSOUND_GETOUTPUT) AutoLib.GetProcAddress("_FSOUND_GetOutput@0");
	FSOUND_SetOutput = (FSOUND_SETOUTPUT) AutoLib.GetProcAddress("_FSOUND_SetOutput@4");
	FSOUND_GetVersion = (FSOUND_GETVERSION) AutoLib.GetProcAddress("_FSOUND_GetVersion@0");
	FSOUND_Init = (FSOUND_INIT) AutoLib.GetProcAddress("_FSOUND_Init@12");
	FSOUND_Stream_OpenFile = (FSOUND_STREAM_OPENFILE) AutoLib.GetProcAddress("_FSOUND_Stream_OpenFile@12");
	FSOUND_StopSound = (FSOUND_STOPSOUND) AutoLib.GetProcAddress("_FSOUND_StopSound@4");
	FSOUND_GetPan = (FSOUND_GETPAN) AutoLib.GetProcAddress("_FSOUND_GetPan@4");
	FSOUND_GetVolume = (FSOUND_GETVOLUME) AutoLib.GetProcAddress("_FSOUND_GetVolume@4");
	FSOUND_SetVolume = (FSOUND_SETVOLUME) AutoLib.GetProcAddress("_FSOUND_SetVolume@8");
	FSOUND_SetPan = (FSOUND_SETPOS) AutoLib.GetProcAddress("_FSOUND_SetPan@8");
	FSOUND_Stream_SetPosition = (FSOUND_STREAM_SETPOSITION) AutoLib.GetProcAddress("_FSOUND_Stream_SetPosition@8");
	FSOUND_IsPlaying = (FSOUND_ISPLAYING) AutoLib.GetProcAddress("_FSOUND_IsPlaying@4");
	FSOUND_GetPaused = (FSOUND_GETPAUSED) AutoLib.GetProcAddress("_FSOUND_GetPaused@4");
}


void FMod::RaiseException() const throw()
{
	int errorcode = FSOUND_GetError();
	switch (errorcode)
	{
		case FMOD_ERR_NONE:				return ;
		case FMOD_ERR_BUSY:				throw RFModException(errorcode, "Cannot call this command after FSOUND_Init.  Call FSOUND_Close first.");
		case FMOD_ERR_UNINITIALIZED:	throw RFModException(errorcode, "This command failed because FSOUND_Init was not called");
		case FMOD_ERR_PLAY:				throw RFModException(errorcode, "Playing the sound failed.");
		case FMOD_ERR_INIT:				throw RFModException(errorcode, "Error initializing output device.");
		case FMOD_ERR_ALLOCATED:		throw RFModException(errorcode, "The output device is already in use and cannot be reused.");
		case FMOD_ERR_OUTPUT_FORMAT:	throw RFModException(errorcode, "Soundcard does not support the features needed for this soundsystem (16bit stereo output)");
		case FMOD_ERR_COOPERATIVELEVEL:	throw RFModException(errorcode, "Error setting cooperative level for hardware.");
		case FMOD_ERR_CREATEBUFFER:		throw RFModException(errorcode, "Error creating hardware sound buffer.");
		case FMOD_ERR_FILE_NOTFOUND:	throw RFModException(errorcode, "File not found");
		case FMOD_ERR_FILE_FORMAT:		throw RFModException(errorcode, "Unknown file format");
		case FMOD_ERR_FILE_BAD:			throw RFModException(errorcode, "Error loading file");
		case FMOD_ERR_MEMORY:			throw RFModException(errorcode, "Not enough memory ");
		case FMOD_ERR_VERSION:			throw RFModException(errorcode, "The version number of this file format is not supported");
		case FMOD_ERR_INVALID_PARAM:	throw RFModException(errorcode, "An invalid parameter was passed to this function");
		case FMOD_ERR_NO_EAX:			throw RFModException(errorcode, "Tried to use an EAX command on a non EAX enabled channel or output.");
		case FMOD_ERR_CHANNEL_ALLOC:	throw RFModException(errorcode, "Failed to allocate a new channel");
		case FMOD_ERR_RECORD:			throw RFModException(errorcode, "Recording not supported on this device");
		case FMOD_ERR_MEDIAPLAYER:		throw RFModException(errorcode, "Required Mediaplayer codec is not installed");

		default :						throw RFModException(errorcode, "Unknown error");
	};
}


