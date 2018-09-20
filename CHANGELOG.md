# Changelog

## [0.9.1] - 2018-09-??

### Added in XeSDK
- Xe::IDelegate and Xe::Event as oberver pattern implementation.
- Added IApplicationHandler::OnDevice, triggered when gamepads or other devices are connected or disconnected.

### Changed in XeSDK
- IApplicationHandler now needs to implement OnDevice.
- Dev builds are now optimized.

### Added in XeGame
- Added TimerComponent, which triggers an event based on the specified elapsed time.

### Changed in XeGame
- If a BGM with the same ID of the current playing one is pushed, a new music will be played.
- Improved the performance of ISoundManager, recycling internally the IAudioBuffer.

### Fixed in XeGame
- Fixed a bug where ISoundManager::PushBgm sometimes was returning nullptr.
- Fixed a bug where ISoundEntity::SetLoops was not setting the first loop correctly.


## [0.9.0] - 2018-09-18

### Added in XeSDK
- Implemented the support for Universal Windows Applications.
- Implemented the support for Durango (Xbox One and Xbox One X).
- It is now possible to create and register Rendering, Audio and Gamepad drivers thorugh Xe::Drivers.
- Xe::Core::Initialize will register all the drivers compatible with the running OS.
- Implemented Xe::Core::GetHostInfo to get information about the OS/HW that is hosting the running app.
- Added a XESDK_VER macro.
- Added Xe::ILogHandler to create and use custom log implementations.
- Added Xe::Graphics::IContext::GetCapabilities.
- Added Xe::Graphics::IContext::SelectRenderingSurface.
- Added a Null driver for IAudio.
- An IAudio now needs AudioInitDesc to be created.
- WaveFormat now includes bpp and wave type for hardware decoding.
- IAudioSource now has loop-start and loop-end support.
- Added GenericAudioSource to implement easily Audio Source drivers.
- Implemented Ogg/Vorbis IAudioSource.
- Implemented SoundFormatFactory to get automatically the correct IAudioSource from the file extension.
- It is now possible to be notified when a controller has been connected or disconnected.

### Changed in XeSDK
- Xe::Core::GetEngineInfo now returns a value instead of using an output parameter.
- Now every interface uses the keyword INTERFACE instead of struct or class.
- It is now preferrable to use LOG* (LOGE, LOGW, LOGI, etc.) instead of Xe::Logger functions directly.
- Xe::Graphics::IContext, IAudio and Gamepads can now be created only from driver's factories.
- IView has been splitted in IApplicationHandler, IFrameHandler, IKeyboardHandler and IPointerHandler.
- IFrameView should be used to handle the title, size, DPI, orientation and event dispatching.
- IFrameView can now be created manually from Xe::Core::Factory.
- Renamed Xe::Graphics::ContextProperties to Xe::Graphics::ContextInitDesc.
- Moved Xe::Core::IView::InitProperties to Xe::Core::FrameViewInitDesc.
- The default window's resolution has changed from 960x540 to 1280x720.
- Xe::Graphics::Create has been replaced by IRenderingDriver::Factory.
- Renamed WaveFormat into WaveDesc.
- Renamed few fields on IAudioSource.
- The Audio buffer now does not need to be decoded in a float anymore, following the BitFormat's WaveDesc.
- Moved from IAudioBuffer::ICallback to IAudioBufferCallback.
- Now IAudio::CreateBuffer does not accept anymore an ICallback as parameter.
- IAudioBuffer::SetCallback can be specified to set a callback for the buffer.
- Improvements on WAV audio decoder.
- Updated zlib from 1.2.8 to 1.2.11.
- Updated libpng from 1.6.25 to 1.6.35.

### Fixed in XeSDK
- Fixed an error on SimpleTextureManager's GetSurface when an invalid texture was specified.
- Fixed a bug that results distorted sound when converting from BitFormat_S24L samples.

### Removed from XeSDK
- Disabled OpenGL rendering context.
- Disabled the support for UTF16 (basically the use of _S and _T).
- Removed IObject::Query
- Removed Xe::Core::IView.
- Removed Xe::Graphics::Create.

### Added in XeGame
 - Added ISoundManager to manage multiple BGMs and SFXs.

### Fixed in XeGame
- Fixed an error on SimpleTextureManager's GetProfile that was not returning the correct values on Dev.


## [0.8.1] - 2018-08-04

### Added in XeSDK
- Added ISurface::Lock and ISurface::Unlock.
- Added Xe::Crypto::Crc32 and Xe::Crypto::Crc64.
- Added Xe::Crypto::Fnv1a32 and Xe::Crypto::Fnv1a64.

### Added in XeGame
- Added Xe::Game library.
- Added IGamePlatform to manage the running gaming platform.
- Implemented a dummy GamePlatform
- Added Steam's support for GamePlatform.
- Added ITextureManager and a SimpleTextureManager.

### Fixed in XeSDK
- Fixed a regression where gamepads from XInput never read the pressed buttons.

## [0.8.0] - 2018-05-06

### Added
- Added Xe::Core::Initialize, that needs to be called on custom entrypoints.
- Added buffer management into Xe::Graphics::IContext
- Added Xe::Logger to replace the old Xe::Debug::Log.
- Now Xe::IO::Gamepad contains gamepad's capabilities.
- Native support for Dualshock 4 via USB.

### Changed
- Expanded the methods for Xe::IO::Gamepad.
- Now IObject::Query implementation is mandatory.
- Renamed IGraphicsTilemap::Color field to BlendColor.
- Renamed Xe::Application::Validate method to Flush.

### Fixed
- Fixed a memory leak on while logging on builds generated with GCC.

### Removed
- Removed IDebugConsole
- Removed Xe::Debug::Log.

## [0.7.5] - 2018-04-29

### Added
- Now it is possible to convert a Xe::Math::Vector3 to a Xe::Math::Vector2.

### Fixed
- Fixed a bug where the OpenGL's IContext was crashing when flushing an empty VertexBuffer.

## [0.7.4] - 2017-12-10

### Added
- Added the volume management on a Xe::Sound::IPlayable.
- Now a Xe::Graphics::Color can be created starting from the single RGBA components.
- Implemeneted a software full-screen for Win32.

### Fixed
- Fixed a bug where the unknown chunks of a WAV file causes the file loading to fail.
- Fixed a memory leak on Xe::Sound::IPlayable.

## [0.7.3] - 2017-06-04

### Added
- Added the type Xe::Bool.
- Added more methods to Xe::Value.
- Added Xe::Graphics::ITilemap to replace Xe::Graphics::ITilemap2d.

### Fixed
- Fixed a rare crash when releasing an Xe::Sound::IPlayable.

### Removed
- Removed the minimum and maximum types INT*_MIN and INT*_MAX.
- Removed Xe::Graphics::ITilemap2d.
- Removed Xe::Game::Tiled.

## [0.7.2] - 2017-03-13

### Added
- Added new methods on Xe::Value.
- Added new methods on Xe::Strings.

## [0.7.1] - 2017-03-10

### Added
- It is now possible to swap the endianness with Xe::Value.

### Fixed
- Fixed a bug where Xe::Net::Socket's ports were on the wrong endianness.

## [0.7.0] - 2017-03-09

### Added
- Added Xe::Value to encapsulate the primitive types.
- Added Xe::Net::Socket as a basic and low-level network API.
- Added more operators on Xe::Math::Rectangle.

### Fixed
- Fixed a bug when a NULL value was sent to IContext::SelectTexture on Direct3D11 context.
- Fixed a bug on Direct3D11 context where some color formats were not handlded correctly.

## [0.6.6] - 2017-01-15

### Added
- IDrawing2D now has the Z coordinate as parameter.
- Added Vector*i and Vector*u.
- It is now possible to expand a VectorN to VectorN+1.

### Changed
- Updated libpng from 1.6.21 to 1.6.25.

### Fixed
- Fixed a bug on indexed PNG without the alpha channel.
- Fixed a bug on Win32 IStorage where opened files had the wrong permissions.
- Fixed a bug where Color::Lerp was inverting the colors.
- Fixed a bug where the return value of pad initialization was not correctly returned.

## [0.6.5] - 2016-08-17

### Added
First usable version of XeEngine.

