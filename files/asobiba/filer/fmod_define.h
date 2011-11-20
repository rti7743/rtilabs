#if !defined(AFX_FMOD_DEFINE_H__83F886AC_09F7_4DCA_A298_F82DE1089607__INCLUDED_)
#define AFX_FMOD_DEFINE_H__83F886AC_09F7_4DCA_A298_F82DE1089607__INCLUDED_
/* ========================================================================================== */
/* FMOD Main header file. Copyright (c), Firelight Technologies Pty, Ltd 1999-2002.           */
/* ========================================================================================== */

#define FMOD_VERSION    3.6f

/* 
    FMOD defined types 
*/
typedef struct FSOUND_SAMPLE    FSOUND_SAMPLE;
typedef struct FSOUND_STREAM    FSOUND_STREAM;
typedef struct FSOUND_DSPUNIT   FSOUND_DSPUNIT;
typedef struct FMUSIC_MODULE    FMUSIC_MODULE;

/* 
    Callback types
*/
typedef void *      (_cdecl *FSOUND_DSPCALLBACK)    (void *originalbuffer, void *newbuffer, int length, int param);
typedef signed char (_cdecl *FSOUND_STREAMCALLBACK) (FSOUND_STREAM *stream, void *buff, int len, int param);
typedef void *      (_cdecl *FSOUND_ALLOCCALLBACK)  (unsigned int size);
typedef void *      (_cdecl *FSOUND_REALLOCCALLBACK)(void *ptr, unsigned int size);
typedef void        (_cdecl *FSOUND_FREECALLBACK)   (void *ptr);
typedef unsigned int(_cdecl *FSOUND_OPENCALLBACK)   (const char *name);
typedef void        (_cdecl *FSOUND_CLOSECALLBACK)  (unsigned int handle);
typedef int         (_cdecl *FSOUND_READCALLBACK)   (void *buffer, int size, unsigned int handle);
typedef int         (_cdecl *FSOUND_SEEKCALLBACK)   (unsigned int handle, int pos, signed char mode);
typedef int         (_cdecl *FSOUND_TELLCALLBACK)   (unsigned int handle);
typedef void        (_cdecl *FMUSIC_CALLBACK)       (FMUSIC_MODULE *mod, unsigned char param);


/*
[ENUM]
[
    [DESCRIPTION]   
    On failure of commands in FMOD, use FSOUND_GetError to attain what happened.
    
    [SEE_ALSO]      
    FSOUND_GetError
]
*/
enum FMOD_ERRORS 
{
    FMOD_ERR_NONE,             /* No errors */
    FMOD_ERR_BUSY,             /* Cannot call this command after FSOUND_Init.  Call FSOUND_Close first. */
    FMOD_ERR_UNINITIALIZED,    /* This command failed because FSOUND_Init or FSOUND_SetOutput was not called */
    FMOD_ERR_INIT,             /* Error initializing output device. */
    FMOD_ERR_ALLOCATED,        /* Error initializing output device, but more specifically, the output device is already in use and cannot be reused. */
    FMOD_ERR_PLAY,             /* Playing the sound failed. */
    FMOD_ERR_OUTPUT_FORMAT,    /* Soundcard does not support the features needed for this soundsystem (16bit stereo output) */
    FMOD_ERR_COOPERATIVELEVEL, /* Error setting cooperative level for hardware. */
    FMOD_ERR_CREATEBUFFER,     /* Error creating hardware sound buffer. */
    FMOD_ERR_FILE_NOTFOUND,    /* File not found */
    FMOD_ERR_FILE_FORMAT,      /* Unknown file format */
    FMOD_ERR_FILE_BAD,         /* Error loading file */
    FMOD_ERR_MEMORY,           /* Not enough memory or resources */
    FMOD_ERR_VERSION,          /* The version number of this file format is not supported */
    FMOD_ERR_INVALID_PARAM,    /* An invalid parameter was passed to this function */
    FMOD_ERR_NO_EAX,           /* Tried to use an EAX command on a non EAX enabled channel or output. */
    FMOD_ERR_CHANNEL_ALLOC,    /* Failed to allocate a new channel */
    FMOD_ERR_RECORD,           /* Recording is not supported on this machine */
    FMOD_ERR_MEDIAPLAYER,      /* Windows Media Player not installed so cannot play wma or use internet streaming. */
    FMOD_ERR_CDDEVICE          /* An error occured trying to open the specified CD device */
};


/*
[ENUM]
[
    [DESCRIPTION]   
    These output types are used with FSOUND_SetOutput, to choose which output driver to use.
    
    FSOUND_OUTPUT_DSOUND will not support hardware 3d acceleration if the sound card driver 
    does not support DirectX 6 Voice Manager Extensions.

    FSOUND_OUTPUT_WINMM is recommended for NT and CE.

    [SEE_ALSO]      
    FSOUND_SetOutput
    FSOUND_GetOutput
]
*/
enum FSOUND_OUTPUTTYPES
{
    FSOUND_OUTPUT_NOSOUND,    /* NoSound driver, all calls to this succeed but do nothing. */
    FSOUND_OUTPUT_WINMM,      /* Windows Multimedia driver. */
    FSOUND_OUTPUT_DSOUND,     /* DirectSound driver.  You need this to get EAX2 or EAX3 support, or FX api support. */
    FSOUND_OUTPUT_A3D,        /* A3D driver.  not supported any more. */

    FSOUND_OUTPUT_OSS,        /* Linux/Unix OSS (Open Sound System) driver, i.e. the kernel sound drivers. */
    FSOUND_OUTPUT_ESD,        /* Linux/Unix ESD (Enlightment Sound Daemon) driver. */
    FSOUND_OUTPUT_ALSA,       /* Linux Alsa driver. */

    FSOUND_OUTPUT_ASIO,       /* Low latency ASIO driver */
    FSOUND_OUTPUT_XBOX,       /* Xbox driver */
    FSOUND_OUTPUT_PS2,        /* PlayStation 2 driver */
    FSOUND_OUTPUT_MAC         /* Mac SoundMager driver */
};


/*
[ENUM]
[
    [DESCRIPTION]   
    These mixer types are used with FSOUND_SetMixer, to choose which mixer to use, or to act 
    upon for other reasons using FSOUND_GetMixer.
    It is not nescessary to set the mixer.  FMOD will autodetect the best mixer for you.

    [SEE_ALSO]      
    FSOUND_SetMixer
    FSOUND_GetMixer
]
*/
enum FSOUND_MIXERTYPES
{
    FSOUND_MIXER_AUTODETECT,        /* CE/PS2 Only - Non interpolating/low quality mixer. */
    FSOUND_MIXER_BLENDMODE,         /* removed / obsolete. */
    FSOUND_MIXER_MMXP5,             /* removed / obsolete. */
    FSOUND_MIXER_MMXP6,             /* removed / obsolete. */

    FSOUND_MIXER_QUALITY_AUTODETECT,/* All platforms - Autodetect the fastest quality mixer based on your cpu. */
    FSOUND_MIXER_QUALITY_FPU,       /* Win32/Linux only - Interpolating/volume ramping FPU mixer.  */
    FSOUND_MIXER_QUALITY_MMXP5,     /* Win32/Linux only - Interpolating/volume ramping P5 MMX mixer.  */
    FSOUND_MIXER_QUALITY_MMXP6,     /* Win32/Linux only - Interpolating/volume ramping ppro+ MMX mixer. */

    FSOUND_MIXER_MONO,              /* CE/PS2 only - MONO non interpolating/low quality mixer. For speed*/
    FSOUND_MIXER_QUALITY_MONO,      /* CE/PS2 only - MONO Interpolating mixer.  For speed */

    FSOUND_MIXER_MAX
};


/*
[ENUM]
[
    [DESCRIPTION]   
    These definitions describe the type of song being played.

    [SEE_ALSO]      
    FMUSIC_GetType  
]
*/
enum FMUSIC_TYPES
{
    FMUSIC_TYPE_NONE,       
    FMUSIC_TYPE_MOD,        /* Protracker / Fasttracker */
    FMUSIC_TYPE_S3M,        /* ScreamTracker 3 */
    FMUSIC_TYPE_XM,         /* FastTracker 2 */
    FMUSIC_TYPE_IT,         /* Impulse Tracker. */
    FMUSIC_TYPE_MIDI        /* MIDI file */
};


/*
[DEFINE_START] 
[
    [NAME] 
    FSOUND_DSP_PRIORITIES

    [DESCRIPTION]   
    These default priorities are used by FMOD internal system DSP units.  They describe the 
    position of the DSP chain, and the order of how audio processing is executed.
    You can actually through the use of FSOUND_DSP_GetxxxUnit (where xxx is the name of the DSP
    unit), disable or even change the priority of a DSP unit.

    [SEE_ALSO]      
    FSOUND_DSP_Create
    FSOUND_DSP_SetPriority
    FSOUND_DSP_GetSpectrum
]
*/
#define FSOUND_DSP_DEFAULTPRIORITY_CLEARUNIT        0       /* DSP CLEAR unit - done first */
#define FSOUND_DSP_DEFAULTPRIORITY_SFXUNIT          100     /* DSP SFX unit - done second */
#define FSOUND_DSP_DEFAULTPRIORITY_MUSICUNIT        200     /* DSP MUSIC unit - done third */
#define FSOUND_DSP_DEFAULTPRIORITY_USER             300     /* User priority, use this as reference */
#define FSOUND_DSP_DEFAULTPRIORITY_FFTUNIT          900     /* This reads data for FSOUND_DSP_GetSpectrum, so it comes after user units */
#define FSOUND_DSP_DEFAULTPRIORITY_CLIPANDCOPYUNIT  1000    /* DSP CLIP AND COPY unit - last */
/* [DEFINE_END] */


/*
[DEFINE_START] 
[
    [NAME] 
    FSOUND_CAPS

    [DESCRIPTION]   
    Driver description bitfields.  Use FSOUND_Driver_GetCaps to determine if a driver enumerated
    has the settings you are after.  The enumerated driver depends on the output mode, see
    FSOUND_OUTPUTTYPES

    [SEE_ALSO]
    FSOUND_GetDriverCaps
    FSOUND_OUTPUTTYPES
]
*/
#define FSOUND_CAPS_HARDWARE                0x1     /* This driver supports hardware accelerated 3d sound. */
#define FSOUND_CAPS_EAX2                    0x2     /* This driver supports EAX 2 reverb */
#define FSOUND_CAPS_EAX3                    0x10    /* This driver supports EAX 3 reverb */
/* [DEFINE_END] */


/*
[DEFINE_START] 
[
    [NAME] 
    FSOUND_MODES
    
    [DESCRIPTION]   
    Sample description bitfields, OR them together for loading and describing samples.
    NOTE.  If the file format being loaded already has a defined format, such as WAV or MP3, then 
    trying to override the pre-defined format with a new set of format flags will not work.  For
    example, an 8 bit WAV file will not load as 16bit if you specify FSOUND_16BITS.  It will just
    ignore the flag and go ahead loading it as 8bits.  For these type of formats the only flags
    you can specify that will really alter the behaviour of how it is loaded, are the following.

    FSOUND_LOOP_OFF     
    FSOUND_LOOP_NORMAL  
    FSOUND_LOOP_BIDI    
    FSOUND_HW3D
    FSOUND_2D
    FSOUND_STREAMABLE   
    FSOUND_LOADMEMORY   
    FSOUND_LOADRAW          
    FSOUND_MPEGACCURATE     

    See flag descriptions for what these do.
]
*/
#define FSOUND_LOOP_OFF     0x00000001  /* For non looping samples. */
#define FSOUND_LOOP_NORMAL  0x00000002  /* For forward looping samples. */
#define FSOUND_LOOP_BIDI    0x00000004  /* For bidirectional looping samples.  (no effect if in hardware). */
#define FSOUND_8BITS        0x00000008  /* For 8 bit samples. */
#define FSOUND_16BITS       0x00000010  /* For 16 bit samples. */
#define FSOUND_MONO         0x00000020  /* For mono samples. */
#define FSOUND_STEREO       0x00000040  /* For stereo samples. */
#define FSOUND_UNSIGNED     0x00000080  /* For user created source data containing unsigned samples. */
#define FSOUND_SIGNED       0x00000100  /* For user created source data containing signed data. */
#define FSOUND_DELTA        0x00000200  /* For user created source data stored as delta values. */
#define FSOUND_IT214        0x00000400  /* For user created source data stored using IT214 compression. */
#define FSOUND_IT215        0x00000800  /* For user created source data stored using IT215 compression. */
#define FSOUND_HW3D         0x00001000  /* Attempts to make samples use 3d hardware acceleration. (if the card supports it) */
#define FSOUND_2D           0x00002000  /* Tells software (not hardware) based sample not to be included in 3d processing. */
#define FSOUND_STREAMABLE   0x00004000  /* For a streamimg sound where you feed the data to it. */
#define FSOUND_LOADMEMORY   0x00008000  /* "name" will be interpreted as a pointer to data for streaming and samples. */
#define FSOUND_LOADRAW      0x00010000  /* Will ignore file format and treat as raw pcm. */
#define FSOUND_MPEGACCURATE 0x00020000  /* For FSOUND_Stream_OpenFile - for accurate FSOUND_Stream_GetLengthMs/FSOUND_Stream_SetTime.  WARNING, see FSOUND_Stream_OpenFile for inital opening time performance issues. */
#define FSOUND_FORCEMONO    0x00040000  /* For forcing stereo streams and samples to be mono - needed if using FSOUND_HW3D and stereo data - incurs a small speed hit for streams */
#define FSOUND_HW2D         0x00080000  /* 2D hardware sounds.  allows hardware specific effects */
#define FSOUND_ENABLEFX     0x00100000  /* Allows DX8 FX to be played back on a sound.  Requires DirectX 8 - Note these sounds cannot be played more than once, be 8 bit, be less than a certain size, or have a changing frequency */
#define FSOUND_MPEGHALFRATE 0x00200000  /* For FMODCE only - decodes mpeg streams using a lower quality decode, but faster execution */
#define FSOUND_XADPCM       0x00400000  /* For XBOX only - Describes a user sample that its contents are compressed as XADPCM  */
#define FSOUND_VAG          0x00800000  /* For PS2 only - Describes a user sample that its contents are compressed as Sony VAG format */
#define FSOUND_NONBLOCKING  0x01000000  /* For FSOUND_Stream_OpenFile - Causes stream to open in the background and not block the foreground app - stream functions only work when ready.  Poll any stream function determine when it IS ready. */

#define FSOUND_NORMAL       (FSOUND_16BITS | FSOUND_SIGNED | FSOUND_MONO)      
/* [DEFINE_END] */



/*
[DEFINE_START] 
[
    [NAME] 
    FSOUND_CDPLAYMODES
    
    [DESCRIPTION]   
    Playback method for a CD Audio track, with FSOUND_CD_SetPlayMode

    [SEE_ALSO]    
    FSOUND_CD_SetPlayMode  
    FSOUND_CD_Play
]
*/
#define FSOUND_CD_PLAYCONTINUOUS    0   /* Starts from the current track and plays to end of CD. */
#define FSOUND_CD_PLAYONCE          1   /* Plays the specified track then stops. */
#define FSOUND_CD_PLAYLOOPED        2   /* Plays the specified track looped, forever until stopped manually. */
#define FSOUND_CD_PLAYRANDOM        3   /* Plays tracks in random order */
/* [DEFINE_END] */


/*
[DEFINE_START] 
[
    [NAME] 
    FSOUND_MISC_VALUES
    
    [DESCRIPTION]
    Miscellaneous values for FMOD functions.

    [SEE_ALSO]
    FSOUND_PlaySound
    FSOUND_PlaySoundEx
    FSOUND_Sample_Alloc
    FSOUND_Sample_Load
    FSOUND_SetPan
]
*/
#define FSOUND_FREE             -1      /* value to play on any free channel, or to allocate a sample in a free sample slot. */
#define FSOUND_UNMANAGED        -2      /* value to allocate a sample that is NOT managed by FSOUND or placed in a sample slot. */
#define FSOUND_ALL              -3      /* for a channel index , this flag will affect ALL channels available!  Not supported by every function. */
#define FSOUND_STEREOPAN        -1      /* value for FSOUND_SetPan so that stereo sounds are not played at half volume.  See FSOUND_SetPan for more on this. */
#define FSOUND_SYSTEMCHANNEL    -1000   /* special 'channel' ID for all channel based functions that want to alter the global FSOUND software mixing output channel */
#define FSOUND_SYSTEMSAMPLE     -1000   /* special 'sample' ID for all sample based functions that want to alter the global FSOUND software mixing output sample */

/* [DEFINE_END] */


/*
[STRUCTURE] 
[
    [DESCRIPTION]
    Structure defining a reverb environment.

    For more indepth descriptions of the reverb properties under win32, please see the EAX2 and EAX3
    documentation at http://developer.creative.com/ under the 'downloads' section.
    If they do not have the EAX3 documentation, then most information can be attained from
    the EAX2 documentation, as EAX3 only adds some more parameters and functionality on top of 
    EAX2.
    Note the default reverb properties are the same as the FSOUND_PRESET_GENERIC preset.
    Note that integer values that typically range from -10,000 to 1000 are represented in 
    decibels, and are of a logarithmic scale, not linear, wheras float values are typically linear.
    PORTABILITY: Each member has the platform it supports in braces ie (win32/xbox).  
    Some reverb parameters are only supported in win32 and some only on xbox. If all parameters are set then
    the reverb should product a similar effect on either platform.
    Linux and FMODCE do not support the reverb api.
    
    The numerical values listed below are the maximum, minimum and default values for each variable respectively.

    [SEE_ALSO]
    FSOUND_Reverb_SetProperties
    FSOUND_Reverb_GetProperties
    FSOUND_REVERB_PRESETS
    FSOUND_REVERB_FLAGS
]
*/
typedef struct _FSOUND_REVERB_PROPERTIES /* MIN     MAX    DEFAULT   DESCRIPTION */
{                                   
    unsigned int Environment;            /* 0     , 25    , 0      , sets all listener properties (win32/ps2 only) */
    float        EnvSize;                /* 1.0   , 100.0 , 7.5    , environment size in meters (win32 only) */
    float        EnvDiffusion;           /* 0.0   , 1.0   , 1.0    , environment diffusion (win32/xbox) */
    int          Room;                   /* -10000, 0     , -1000  , room effect level (at mid frequencies) (win32/xbox/ps2) */
    int          RoomHF;                 /* -10000, 0     , -100   , relative room effect level at high frequencies (win32/xbox) */
    int          RoomLF;                 /* -10000, 0     , 0      , relative room effect level at low frequencies (win32 only) */
    float        DecayTime;              /* 0.1   , 20.0  , 1.49   , reverberation decay time at mid frequencies (win32/xbox) */
    float        DecayHFRatio;           /* 0.1   , 2.0   , 0.83   , high-frequency to mid-frequency decay time ratio (win32/xbox) */
    float        DecayLFRatio;           /* 0.1   , 2.0   , 1.0    , low-frequency to mid-frequency decay time ratio (win32 only) */
    int          Reflections;            /* -10000, 1000  , -2602  , early reflections level relative to room effect (win32/xbox) */
    float        ReflectionsDelay;       /* 0.0   , 0.3   , 0.007  , initial reflection delay time (win32/xbox) */
    float        ReflectionsPan[3];      /*       ,       , [0,0,0], early reflections panning vector (win32 only) */
    int          Reverb;                 /* -10000, 2000  , 200    , late reverberation level relative to room effect (win32/xbox) */
    float        ReverbDelay;            /* 0.0   , 0.1   , 0.011  , late reverberation delay time relative to initial reflection (win32/xbox) */
    float        ReverbPan[3];           /*       ,       , [0,0,0], late reverberation panning vector (win32 only) */
    float        EchoTime;               /* .075  , 0.25  , 0.25   , echo time (win32 only) */
    float        EchoDepth;              /* 0.0   , 1.0   , 0.0    , echo depth (win32 only) */
    float        ModulationTime;         /* 0.04  , 4.0   , 0.25   , modulation time (win32 only) */
    float        ModulationDepth;        /* 0.0   , 1.0   , 0.0    , modulation depth (win32 only) */
    float        AirAbsorptionHF;        /* -100  , 0.0   , -5.0   , change in level per meter at high frequencies (win32 only) */
    float        HFReference;            /* 1000.0, 20000 , 5000.0 , reference high frequency (hz) (win32/xbox) */
    float        LFReference;            /* 20.0  , 1000.0, 250.0  , reference low frequency (hz) (win32 only) */
    float        RoomRolloffFactor;      /* 0.0   , 10.0  , 0.0    , like FSOUND_3D_Listener_SetRolloffFactor but for room effect (win32/xbox) */
    float        Diffusion;              /* 0.0   , 100.0 , 100.0  , Value that controls the echo density in the late reverberation decay. (xbox only) */
    float        Density;                /* 0.0   , 100.0 , 100.0  , Value that controls the modal density in the late reverberation decay (xbox only) */
    unsigned int Flags;                  /* FSOUND_REVERB_FLAGS - modifies the behavior of above properties (win32 only) */
} FSOUND_REVERB_PROPERTIES;


/*
[DEFINE_START] 
[
    [NAME] 
    FSOUND_REVERB_FLAGS
    
    [DESCRIPTION]
    Values for the Flags member of the FSOUND_REVERB_PROPERTIES structure.

    [SEE_ALSO]
    FSOUND_REVERB_PROPERTIES
]
*/
#define FSOUND_REVERB_FLAGS_DECAYTIMESCALE        0x00000001 /* 'EnvSize' affects reverberation decay time */
#define FSOUND_REVERB_FLAGS_REFLECTIONSSCALE      0x00000002 /* 'EnvSize' affects reflection level */
#define FSOUND_REVERB_FLAGS_REFLECTIONSDELAYSCALE 0x00000004 /* 'EnvSize' affects initial reflection delay time */
#define FSOUND_REVERB_FLAGS_REVERBSCALE           0x00000008 /* 'EnvSize' affects reflections level */
#define FSOUND_REVERB_FLAGS_REVERBDELAYSCALE      0x00000010 /* 'EnvSize' affects late reverberation delay time */
#define FSOUND_REVERB_FLAGS_DECAYHFLIMIT          0x00000020 /* AirAbsorptionHF affects DecayHFRatio */
#define FSOUND_REVERB_FLAGS_ECHOTIMESCALE         0x00000040 /* 'EnvSize' affects echo time */
#define FSOUND_REVERB_FLAGS_MODULATIONTIMESCALE   0x00000080 /* 'EnvSize' affects modulation time */
#define FSOUND_REVERB_FLAGS_CORE0                 0x00000100 /* PS2 Only - Reverb is applied to CORE0 (hw voices 0-23) */
#define FSOUND_REVERB_FLAGS_CORE1                 0x00000200 /* PS2 Only - Reverb is applied to CORE1 (hw voices 24-47) */
#define FSOUND_REVERB_FLAGS_DEFAULT              (FSOUND_REVERB_FLAGS_DECAYTIMESCALE |        \
                                                  FSOUND_REVERB_FLAGS_REFLECTIONSSCALE |      \
                                                  FSOUND_REVERB_FLAGS_REFLECTIONSDELAYSCALE | \
                                                  FSOUND_REVERB_FLAGS_REVERBSCALE |           \
                                                  FSOUND_REVERB_FLAGS_REVERBDELAYSCALE |      \
                                                  FSOUND_REVERB_FLAGS_DECAYHFLIMIT |          \
                                                  FSOUND_REVERB_FLAGS_CORE0 |                 \
                                                  FSOUND_REVERB_FLAGS_CORE1 )
/* [DEFINE_END] */




/*
[DEFINE_START] 
[
    [NAME] 
    FSOUND_REVERB_PRESETS
    
    [DESCRIPTION]   
    A set of predefined environment PARAMETERS, created by Creative Labs
    These are used to initialize an FSOUND_REVERB_PROPERTIES structure statically.
    ie 
    FSOUND_REVERB_PROPERTIES prop = FSOUND_PRESET_GENERIC;

    [SEE_ALSO]
    FSOUND_Reverb_SetProperties
]
*/
/*                                     Env  Size    Diffus  Room   RoomHF  RmLF DecTm   DecHF  DecLF   Refl  RefDel  RefPan               Revb  RevDel  ReverbPan           EchoTm  EchDp  ModTm  ModDp  AirAbs  HFRef    LFRef  RRlOff Diffus  Densty  FLAGS */
#define FSOUND_PRESET_OFF              {0,	7.5f,	1.00f, -10000, -10000, 0,   1.00f,  1.00f, 1.0f,  -2602, 0.007f, { 0.0f,0.0f,0.0f },   200, 0.011f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f,   0.0f,   0.0f, 0x33f }
#define FSOUND_PRESET_GENERIC          {0,	7.5f,	1.00f, -1000,  -100,   0,   1.49f,  0.83f, 1.0f,  -2602, 0.007f, { 0.0f,0.0f,0.0f },   200, 0.011f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x3f }
#define FSOUND_PRESET_PADDEDCELL       {1,	1.4f,	1.00f, -1000,  -6000,  0,   0.17f,  0.10f, 1.0f,  -1204, 0.001f, { 0.0f,0.0f,0.0f },   207, 0.002f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x3f }
#define FSOUND_PRESET_ROOM 	           {2,	1.9f,	1.00f, -1000,  -454,   0,   0.40f,  0.83f, 1.0f,  -1646, 0.002f, { 0.0f,0.0f,0.0f },    53, 0.003f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x3f }
#define FSOUND_PRESET_BATHROOM 	       {3,	1.4f,	1.00f, -1000,  -1200,  0,   1.49f,  0.54f, 1.0f,   -370, 0.007f, { 0.0f,0.0f,0.0f },  1030, 0.011f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f,  60.0f, 0x3f }
#define FSOUND_PRESET_LIVINGROOM       {4,	2.5f,	1.00f, -1000,  -6000,  0,   0.50f,  0.10f, 1.0f,  -1376, 0.003f, { 0.0f,0.0f,0.0f }, -1104, 0.004f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x3f }
#define FSOUND_PRESET_STONEROOM        {5,	11.6f,	1.00f, -1000,  -300,   0,   2.31f,  0.64f, 1.0f,   -711, 0.012f, { 0.0f,0.0f,0.0f },    83, 0.017f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x3f }
#define FSOUND_PRESET_AUDITORIUM       {6,	21.6f,	1.00f, -1000,  -476,   0,   4.32f,  0.59f, 1.0f,   -789, 0.020f, { 0.0f,0.0f,0.0f },  -289, 0.030f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x3f }
#define FSOUND_PRESET_CONCERTHALL      {7,	19.6f,	1.00f, -1000,  -500,   0,   3.92f,  0.70f, 1.0f,  -1230, 0.020f, { 0.0f,0.0f,0.0f },    -2, 0.029f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x3f }
#define FSOUND_PRESET_CAVE             {8,	14.6f,	1.00f, -1000,  0,      0,   2.91f,  1.30f, 1.0f,   -602, 0.015f, { 0.0f,0.0f,0.0f },  -302, 0.022f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x1f }
#define FSOUND_PRESET_ARENA            {9,	36.2f,	1.00f, -1000,  -698,   0,   7.24f,  0.33f, 1.0f,  -1166, 0.020f, { 0.0f,0.0f,0.0f },    16, 0.030f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x3f }
#define FSOUND_PRESET_HANGAR           {10,	50.3f,	1.00f, -1000,  -1000,  0,   10.05f, 0.23f, 1.0f,   -602, 0.020f, { 0.0f,0.0f,0.0f },   198, 0.030f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x3f }
#define FSOUND_PRESET_CARPETTEDHALLWAY {11,	1.9f,	1.00f, -1000,  -4000,  0,   0.30f,  0.10f, 1.0f,  -1831, 0.002f, { 0.0f,0.0f,0.0f }, -1630, 0.030f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x3f }
#define FSOUND_PRESET_HALLWAY          {12,	1.8f,	1.00f, -1000,  -300,   0,   1.49f,  0.59f, 1.0f,  -1219, 0.007f, { 0.0f,0.0f,0.0f },   441, 0.011f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x3f }
#define FSOUND_PRESET_STONECORRIDOR    {13,	13.5f,	1.00f, -1000,  -237,   0,   2.70f,  0.79f, 1.0f,  -1214, 0.013f, { 0.0f,0.0f,0.0f },   395, 0.020f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x3f }
#define FSOUND_PRESET_ALLEY 	       {14,	7.5f,	0.30f, -1000,  -270,   0,   1.49f,  0.86f, 1.0f,  -1204, 0.007f, { 0.0f,0.0f,0.0f },    -4, 0.011f, { 0.0f,0.0f,0.0f }, 0.125f, 0.95f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x3f }
#define FSOUND_PRESET_FOREST 	       {15,	38.0f,	0.30f, -1000,  -3300,  0,   1.49f,  0.54f, 1.0f,  -2560, 0.162f, { 0.0f,0.0f,0.0f },  -229, 0.088f, { 0.0f,0.0f,0.0f }, 0.125f, 1.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f,  79.0f, 100.0f, 0x3f }
#define FSOUND_PRESET_CITY             {16,	7.5f,	0.50f, -1000,  -800,   0,   1.49f,  0.67f, 1.0f,  -2273, 0.007f, { 0.0f,0.0f,0.0f }, -1691, 0.011f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f,  50.0f, 100.0f, 0x3f }
#define FSOUND_PRESET_MOUNTAINS        {17,	100.0f, 0.27f, -1000,  -2500,  0,   1.49f,  0.21f, 1.0f,  -2780, 0.300f, { 0.0f,0.0f,0.0f }, -1434, 0.100f, { 0.0f,0.0f,0.0f }, 0.250f, 1.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f,  27.0f, 100.0f, 0x1f }
#define FSOUND_PRESET_QUARRY           {18,	17.5f,	1.00f, -1000,  -1000,  0,   1.49f,  0.83f, 1.0f, -10000, 0.061f, { 0.0f,0.0f,0.0f },   500, 0.025f, { 0.0f,0.0f,0.0f }, 0.125f, 0.70f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x3f }
#define FSOUND_PRESET_PLAIN            {19,	42.5f,	0.21f, -1000,  -2000,  0,   1.49f,  0.50f, 1.0f,  -2466, 0.179f, { 0.0f,0.0f,0.0f }, -1926, 0.100f, { 0.0f,0.0f,0.0f }, 0.250f, 1.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f,  21.0f, 100.0f, 0x3f }
#define FSOUND_PRESET_PARKINGLOT       {20,	8.3f,	1.00f, -1000,  0,      0,   1.65f,  1.50f, 1.0f,  -1363, 0.008f, { 0.0f,0.0f,0.0f }, -1153, 0.012f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x1f }
#define FSOUND_PRESET_SEWERPIPE        {21,	1.7f,	0.80f, -1000,  -1000,  0,   2.81f,  0.14f, 1.0f,    429, 0.014f, { 0.0f,0.0f,0.0f },  1023, 0.021f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f,  80.0f,  60.0f, 0x3f }
#define FSOUND_PRESET_UNDERWATER       {22,	1.8f,	1.00f, -1000,  -4000,  0,   1.49f,  0.10f, 1.0f,   -449, 0.007f, { 0.0f,0.0f,0.0f },  1700, 0.011f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 1.18f, 0.348f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x3f }

/* Non I3DL2 presets */

#define FSOUND_PRESET_DRUGGED          {23,	1.9f,	0.50f, -1000,  0,      0,   8.39f,  1.39f, 1.0f,  -115,  0.002f, { 0.0f,0.0f,0.0f },   985, 0.030f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 1.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x1f }
#define FSOUND_PRESET_DIZZY            {24,	1.8f,	0.60f, -1000,  -400,   0,   17.23f, 0.56f, 1.0f,  -1713, 0.020f, { 0.0f,0.0f,0.0f },  -613, 0.030f, { 0.0f,0.0f,0.0f }, 0.250f, 1.00f, 0.81f, 0.310f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x1f }
#define FSOUND_PRESET_PSYCHOTIC        {25,	1.0f,	0.50f, -1000,  -151,   0,   7.56f,  0.91f, 1.0f,  -626,  0.020f, { 0.0f,0.0f,0.0f },   774, 0.030f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 4.00f, 1.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x1f }

/* PlayStation 2 Only presets */

#define FSOUND_PRESET_PS2_ROOM         {1,	0,	    0,         0,  0,      0,   0.0f,   0.0f,  0.0f,     0,  0.000f, { 0.0f,0.0f,0.0f },     0, 0.000f, { 0.0f,0.0f,0.0f }, 0.000f, 0.00f, 0.00f, 0.000f,  0.0f, 0000.0f,   0.0f, 0.0f,   0.0f,   0.0f, 0x31f }
#define FSOUND_PRESET_PS2_STUDIO_A     {2,	0,	    0,         0,  0,      0,   0.0f,   0.0f,  0.0f,     0,  0.000f, { 0.0f,0.0f,0.0f },     0, 0.000f, { 0.0f,0.0f,0.0f }, 0.000f, 0.00f, 0.00f, 0.000f,  0.0f, 0000.0f,   0.0f, 0.0f,   0.0f,   0.0f, 0x31f }
#define FSOUND_PRESET_PS2_STUDIO_B     {3,	0,	    0,         0,  0,      0,   0.0f,   0.0f,  0.0f,     0,  0.000f, { 0.0f,0.0f,0.0f },     0, 0.000f, { 0.0f,0.0f,0.0f }, 0.000f, 0.00f, 0.00f, 0.000f,  0.0f, 0000.0f,   0.0f, 0.0f,   0.0f,   0.0f, 0x31f }
#define FSOUND_PRESET_PS2_STUDIO_C     {4,	0,	    0,         0,  0,      0,   0.0f,   0.0f,  0.0f,     0,  0.000f, { 0.0f,0.0f,0.0f },     0, 0.000f, { 0.0f,0.0f,0.0f }, 0.000f, 0.00f, 0.00f, 0.000f,  0.0f, 0000.0f,   0.0f, 0.0f,   0.0f,   0.0f, 0x31f }
#define FSOUND_PRESET_PS2_HALL         {5,	0,	    0,         0,  0,      0,   0.0f,   0.0f,  0.0f,     0,  0.000f, { 0.0f,0.0f,0.0f },     0, 0.000f, { 0.0f,0.0f,0.0f }, 0.000f, 0.00f, 0.00f, 0.000f,  0.0f, 0000.0f,   0.0f, 0.0f,   0.0f,   0.0f, 0x31f }
#define FSOUND_PRESET_PS2_SPACE        {6,	0,	    0,         0,  0,      0,   0.0f,   0.0f,  0.0f,     0,  0.000f, { 0.0f,0.0f,0.0f },     0, 0.000f, { 0.0f,0.0f,0.0f }, 0.000f, 0.00f, 0.00f, 0.000f,  0.0f, 0000.0f,   0.0f, 0.0f,   0.0f,   0.0f, 0x31f }
#define FSOUND_PRESET_PS2_ECHO         {7,	0,	    0,         0,  0,      0,   0.0f,   0.0f,  0.0f,     0,  0.000f, { 0.0f,0.0f,0.0f },     0, 0.000f, { 0.0f,0.0f,0.0f }, 0.000f, 0.00f, 0.00f, 0.000f,  0.0f, 0000.0f,   0.0f, 0.0f,   0.0f,   0.0f, 0x31f }
#define FSOUND_PRESET_PS2_DELAY        {8,	0,	    0,         0,  0,      0,   0.0f,   0.0f,  0.0f,     0,  0.000f, { 0.0f,0.0f,0.0f },     0, 0.000f, { 0.0f,0.0f,0.0f }, 0.000f, 0.00f, 0.00f, 0.000f,  0.0f, 0000.0f,   0.0f, 0.0f,   0.0f,   0.0f, 0x31f }
#define FSOUND_PRESET_PS2_PIPE         {9,	0,	    0,         0,  0,      0,   0.0f,   0.0f,  0.0f,     0,  0.000f, { 0.0f,0.0f,0.0f },     0, 0.000f, { 0.0f,0.0f,0.0f }, 0.000f, 0.00f, 0.00f, 0.000f,  0.0f, 0000.0f,   0.0f, 0.0f,   0.0f,   0.0f, 0x31f }

/* [DEFINE_END] */


/*
[STRUCTURE] 
[
    [DESCRIPTION]
    Structure defining the properties for a reverb source, related to a FSOUND channel.

    For more indepth descriptions of the reverb properties under win32, please see the EAX3
    documentation at http://developer.creative.com/ under the 'downloads' section.
    If they do not have the EAX3 documentation, then most information can be attained from
    the EAX2 documentation, as EAX3 only adds some more parameters and functionality on top of 
    EAX2.
    
    Note the default reverb properties are the same as the FSOUND_PRESET_GENERIC preset.
    Note that integer values that typically range from -10,000 to 1000 are represented in 
    decibels, and are of a logarithmic scale, not linear, wheras float values are typically linear.
    PORTABILITY: Each member has the platform it supports in braces ie (win32/xbox).  
    Some reverb parameters are only supported in win32 and some only on xbox. If all parameters are set then
    the reverb should product a similar effect on either platform.
    Linux and FMODCE do not support the reverb api.
    
    The numerical values listed below are the maximum, minimum and default values for each variable respectively.

    [SEE_ALSO]
    FSOUND_Reverb_SetChannelProperties
    FSOUND_Reverb_GetChannelProperties
    FSOUND_REVERB_CHANNELFLAGS
]
*/
typedef struct _FSOUND_REVERB_CHANNELPROPERTIES /* MIN     MAX    DEFAULT */
{                                   
    int    Direct;                              /* -10000, 1000,  0,    direct path level (at low and mid frequencies) (win32/xbox) */
    int    DirectHF;                            /* -10000, 0,     0,    relative direct path level at high frequencies (win32/xbox) */
    int    Room;                                /* -10000, 1000,  0,    room effect level (at low and mid frequencies) (win32/xbox) */
    int    RoomHF;                              /* -10000, 0,     0,    relative room effect level at high frequencies (win32/xbox) */
    int    Obstruction;                         /* -10000, 0,     0,    main obstruction control (attenuation at high frequencies)  (win32/xbox) */
    float  ObstructionLFRatio;                  /* 0.0,    1.0,   0.0,  obstruction low-frequency level re. main control (win32/xbox) */
    int    Occlusion;                           /* -10000, 0,     0,    main occlusion control (attenuation at high frequencies) (win32/xbox) */
    float  OcclusionLFRatio;                    /* 0.0,    1.0,   0.25, occlusion low-frequency level re. main control (win32/xbox) */
    float  OcclusionRoomRatio;                  /* 0.0,    10.0,  1.5,  relative occlusion control for room effect (win32) */
    float  OcclusionDirectRatio;                /* 0.0,    10.0,  1.0,  relative occlusion control for direct path (win32) */
    int    Exclusion;                           /* -10000, 0,     0,    main exlusion control (attenuation at high frequencies) (win32) */
    float  ExclusionLFRatio;                    /* 0.0,    1.0,   1.0,  exclusion low-frequency level re. main control (win32) */
    int    OutsideVolumeHF;                     /* -10000, 0,     0,    outside sound cone level at high frequencies (win32) */
    float  DopplerFactor;                       /* 0.0,    10.0,  0.0,  like DS3D flDopplerFactor but per source (win32) */
    float  RolloffFactor;                       /* 0.0,    10.0,  0.0,  like DS3D flRolloffFactor but per source (win32) */
    float  RoomRolloffFactor;                   /* 0.0,    10.0,  0.0,  like DS3D flRolloffFactor but for room effect (win32/xbox) */
    float  AirAbsorptionFactor;                 /* 0.0,    10.0,  1.0,  multiplies AirAbsorptionHF member of FSOUND_REVERB_PROPERTIES (win32) */
    int    Flags;                               /* FSOUND_REVERB_CHANNELFLAGS - modifies the behavior of properties (win32) */
} FSOUND_REVERB_CHANNELPROPERTIES;


/*
[DEFINE_START] 
[
    [NAME] 
    FSOUND_REVERB_CHANNELFLAGS
    
    [DESCRIPTION]
    Values for the Flags member of the FSOUND_REVERB_CHANNELPROPERTIES structure.

    [SEE_ALSO]
    FSOUND_REVERB_CHANNELPROPERTIES
]
*/
#define FSOUND_REVERB_CHANNELFLAGS_DIRECTHFAUTO  0x00000001 /* Automatic setting of 'Direct'  due to distance from listener */
#define FSOUND_REVERB_CHANNELFLAGS_ROOMAUTO      0x00000002 /* Automatic setting of 'Room'  due to distance from listener */
#define FSOUND_REVERB_CHANNELFLAGS_ROOMHFAUTO    0x00000004 /* Automatic setting of 'RoomHF' due to distance from listener */
#define FSOUND_REVERB_CHANNELFLAGS_DEFAULT       (FSOUND_REVERB_CHANNELFLAGS_DIRECTHFAUTO |   \
                                                  FSOUND_REVERB_CHANNELFLAGS_ROOMAUTO|        \
                                                  FSOUND_REVERB_CHANNELFLAGS_ROOMHFAUTO)
/* [DEFINE_END] */


/*
[ENUM] 
[
    [DESCRIPTION]
    These values are used with FSOUND_FX_Enable to enable DirectX 8 FX for a channel.

    [SEE_ALSO]
    FSOUND_FX_Enable
    FSOUND_FX_Disable
    FSOUND_FX_SetChorus
    FSOUND_FX_SetCompressor
    FSOUND_FX_SetDistortion
    FSOUND_FX_SetEcho
    FSOUND_FX_SetFlanger
    FSOUND_FX_SetGargle
    FSOUND_FX_SetI3DL2Reverb
    FSOUND_FX_SetParamEQ
    FSOUND_FX_SetWavesReverb
]
*/
enum FSOUND_FX_MODES
{
    FSOUND_FX_CHORUS,
    FSOUND_FX_COMPRESSOR,
    FSOUND_FX_DISTORTION,
    FSOUND_FX_ECHO,
    FSOUND_FX_FLANGER,
    FSOUND_FX_GARGLE,
    FSOUND_FX_I3DL2REVERB,
    FSOUND_FX_PARAMEQ,
    FSOUND_FX_WAVES_REVERB,

    FSOUND_FX_MAX
};

/*
[ENUM]
[
    [DESCRIPTION]   
    These are speaker types defined for use with the FSOUND_SetSpeakerMode command.
    Note - Only reliably works with FSOUND_OUTPUT_DSOUND or FSOUND_OUTPUT_XBOX output modes.  Other output modes will only 
    interpret FSOUND_SPEAKERMODE_MONO and set everything else to be stereo.

    [SEE_ALSO]
    FSOUND_SetSpeakerMode
]
*/
enum FSOUND_SPEAKERMODES
{
    FSOUND_SPEAKERMODE_DOLBYDIGITAL,  /* The audio is played through a speaker arrangement of surround speakers with a subwoofer. */
    FSOUND_SPEAKERMODE_HEADPHONES,    /* The speakers are headphones. */
    FSOUND_SPEAKERMODE_MONO,          /* The speakers are monaural. */
    FSOUND_SPEAKERMODE_QUAD,          /* The speakers are quadraphonic.  */
    FSOUND_SPEAKERMODE_STEREO,        /* The speakers are stereo (default value). */
    FSOUND_SPEAKERMODE_SURROUND,      /* The speakers are surround sound. */
    FSOUND_SPEAKERMODE_DTS            /* (XBOX Only) The audio is played through a speaker arrangement of surround speakers with a subwoofer. */
};


/*
[DEFINE_START] 
[
    [NAME] 
    FSOUND_INIT_FLAGS
    
    [DESCRIPTION]   
    Initialization flags.  Use them with FSOUND_Init in the flags parameter to change various behaviour.
    
    FSOUND_INIT_ENABLEOUTPUTFX Is an init mode which enables the FSOUND mixer buffer to be affected by DirectX 8 effects.
    Note that due to limitations of DirectSound, FSOUND_Init may fail if this is enabled because the buffersize is too small.
    This can be fixed with FSOUND_SetBufferSize.  Increase the BufferSize until it works.
    When it is enabled you can use the FSOUND_FX api, and use FSOUND_SYSTEMCHANNEL as the channel id when setting parameters.

    [SEE_ALSO]
    FSOUND_Init
]
*/
#define FSOUND_INIT_USEDEFAULTMIDISYNTH     0x01    /* Causes MIDI playback to force software decoding. */
#define FSOUND_INIT_GLOBALFOCUS             0x02    /* For DirectSound output - sound is not muted when window is out of focus. */
#define FSOUND_INIT_ENABLEOUTPUTFX          0x04    /* For DirectSound output - Allows FSOUND_FX api to be used on global software mixer output! */
#define FSOUND_INIT_ACCURATEVULEVELS        0x08    /* This latency adjusts FSOUND_GetCurrentLevels, but incurs a small cpu and memory hit */
#define FSOUND_INIT_DISABLE_CORE0_REVERB    0x10    /* PS2 only - Disable reverb on CORE 0 to regain SRAM */
#define FSOUND_INIT_DISABLE_CORE1_REVERB    0x20    /* PS2 only - Disable reverb on CORE 1 to regain SRAM */
/* [DEFINE_END] */

#endif // !defined(AFX_FMOD_DEFINE_H__83F886AC_09F7_4DCA_A298_F82DE1089607__INCLUDED_)
