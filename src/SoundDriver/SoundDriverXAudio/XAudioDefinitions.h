#pragma once

#if defined(PLATFORM_WIN32) || defined(PLATFORM_UWP)
#define AUDIO_XAUDIO2
#endif

#ifdef AUDIO_XAUDIO2

#include <xaudio2.h>
#include <basetyps.h>

#define WAVE_FORMAT_PCM 1

// FROM COMDECL.H
#define DECLSPEC_UUID_WRAPPER(x) __declspec(uuid(#x))
#ifdef INITGUID

#define DEFINE_CLSID(className, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
            class DECLSPEC_UUID_WRAPPER(l##-##w1##-##w2##-##b1##b2##-##b3##b4##b5##b6##b7##b8) className; \
            EXTERN_C const GUID DECLSPEC_SELECTANY CLSID_##className = __uuidof(className)

#define DEFINE_IID(interfaceName, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
            interface DECLSPEC_UUID_WRAPPER(l##-##w1##-##w2##-##b1##b2##-##b3##b4##b5##b6##b7##b8) interfaceName; \
            EXTERN_C const GUID DECLSPEC_SELECTANY IID_##interfaceName = __uuidof(interfaceName)

#else // INITGUID

#define DEFINE_CLSID(className, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
            class DECLSPEC_UUID_WRAPPER(l##-##w1##-##w2##-##b1##b2##-##b3##b4##b5##b6##b7##b8) className; \
            EXTERN_C const GUID CLSID_##className

#define DEFINE_IID(interfaceName, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
            interface DECLSPEC_UUID_WRAPPER(l##-##w1##-##w2##-##b1##b2##-##b3##b4##b5##b6##b7##b8) interfaceName; \
            EXTERN_C const GUID IID_##interfaceName

#endif // INITGUID

DEFINE_CLSID(XAudio2_20, fac23f48, 31f5, 45a8, b4, 9b, 52, 25, d6, 14, 01, aa); // March 2008
DEFINE_CLSID(XAudio2_21, e21a7345, eb21, 468e, be, 50, 80, 4d, b9, 7c, f7, 08); // June 2008
DEFINE_CLSID(XAudio2_22, b802058a, 464a, 42db, bc, 10, b6, 50, d6, f2, 58, 6a); // August 2008
DEFINE_CLSID(XAudio2_23, 4c5e637a, 16c7, 4de3, 9c, 46, 5e, d2, 21, 81, 96, 2d); // November 2008
DEFINE_CLSID(XAudio2_24, 03219e78, 5bc3, 44d1, b9, 2e, f6, 3d, 89, cc, 65, 26); // March 2009
DEFINE_CLSID(XAudio2_25, 4c9b6dde, 6809, 46e6, a2, 78, 9b, 6a, 97, 58, 86, 70); // August 2009
DEFINE_CLSID(XAudio2_26, 3eda9b49, 2085, 498b, 9b, b2, 39, a6, 77, 84, 93, de); // February 2010
DEFINE_CLSID(XAudio2_27, 5a508685, a254, 4fba, 9b, 82, 9a, 24, b0, 03, 06, af); // June 2010
DEFINE_IID(IXAudio2_Pre28, 8bcf1f58, 9fe7, 4583, 8a, c6, e2, ad, c4, 65, c8, bb);

// Used in XAUDIO2_DEVICE_DETAILS below to describe the types of applications
// that the user has specified each device as a default for. 0 means that the
// device isn't the default for any role.
typedef enum XAUDIO2_DEVICE_ROLE
{
	NotDefaultDevice = 0x0,
	DefaultConsoleDevice = 0x1,
	DefaultMultimediaDevice = 0x2,
	DefaultCommunicationsDevice = 0x4,
	DefaultGameDevice = 0x8,
	GlobalDefaultDevice = 0xf,
	InvalidDeviceRole = ~GlobalDefaultDevice
} XAUDIO2_DEVICE_ROLE;

// Returned by IXAudio2::GetDeviceDetails
typedef struct XAUDIO2_DEVICE_DETAILS
{
	WCHAR DeviceID[256];                // String identifier for the audio device.
	WCHAR DisplayName[256];             // Friendly name suitable for display to a human.
	XAUDIO2_DEVICE_ROLE Role;           // Roles that the device should be used for.
	WAVEFORMATEXTENSIBLE OutputFormat;  // The device's native PCM audio output format.
} XAUDIO2_DEVICE_DETAILS;

#endif