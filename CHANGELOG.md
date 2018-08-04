# Changelog

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

