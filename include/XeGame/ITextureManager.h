#pragma once
#include <XeSDK/IStorage.h>
#include <XeSDK/XeGraphicsImaging.h>
#include <XeSDK/XeMathVector2.h>
#include <XeSDK/IDelegate.h>

namespace Xe { namespace Game {
	struct _TexId;
	typedef _TexId* TexId;

	struct _ClutId;
	typedef _ClutId* ClutId;

	const TexId TexInvalid = (TexId)-1;
	const ClutId ClutInvalid = (ClutId)-1;

	struct ClutData
	{
		u32* Data;
		int ColorsCount;
		int Pitch;
	};

	struct TextureManagerProfileEntry
	{
		std::string Name;
		TexId TexId;
		int Width, Height;
		int ReferencesCount;
		std::list<ClutId> Cluts;
	};

	struct TextureManagerProfile
	{
		int TexturesCount;
		int MaxTexturesCount;
		int ClutsCount;
		int MaxClutsCount;
		int TexturesReferencesCount;
		int MaxClutsCountPerTexture;
		std::list<TextureManagerProfileEntry> Textures;
	};

	struct TextureManagerLoadRequest
	{
		//! \brief Output image that will hold the loaded image
		Xe::ObjPtr<Xe::Graphics::Imaging::IImage> Image;

		//! \brief The file name of the image to load
		Xe::String FileName;
	};

	interface ITextureManager;

	template <class T>
	struct TextureManagerArgs : public EventArgs<ITextureManager*, T> {};

	template <class T>
	interface TextureManagerDelegate :
		public Xe::IDelegate<const TextureManagerArgs<T>&> {};

	typedef TextureManagerDelegate<TextureManagerLoadRequest> TextureManagerLoadRequestDelegate;

	interface ITextureManager :
		public virtual IObject
	{
		virtual void SetLoadRequestDelegate(TextureManagerLoadRequestDelegate* delegate) = 0;

		//! \brief Create a texture from an image.
		//! \param[in] fileName Path of the image to open
		/** \details If the file name has been opened before, a new reference
		 * will be added and a valid TexId will be returned. If the file name
		 * has been previously unloaded, uncached or never loaded, the
		 * specified TextureManagerLoadRequestDelegate will be called.
		 * If TextureManagerLoadRequestDelegate is null or will return invalid
		 * data, then TexInvalid will be returend.
		 */
		virtual TexId Open(const Xe::StringSpan& fileName) = 0;

		//! \brief Create a texture from an image.
		//! \param[in] image Image that will be loaded and transformed as a texture.
		//! \param[in] name An unique name from the image, usually used to cache them; cannot be null..
		//! \return A proper TexId or TexInvalid.
		//! \warning DEPRECATED! Use Open and SetLoadRequestDelegate instead
//#if _MSC_VER >= 1911
//		[[deprecated]]
//#endif
		virtual TexId Create(Xe::Graphics::Imaging::IImage* image, ctstring name) = 0;

		//! \brief Get the TexId from the unique name
		//! \param[in] name Unique name of the image; cannot be null.
		//! \return A proper TexId or TexInvalid if it was not found.
		virtual TexId GetTextureId(ctstring name) const = 0;

		//! \brief Get the surface from a texture id.
		virtual Xe::Graphics::ISurface* GetSurface(TexId texId) = 0;

		//! \brief Get the surface used as a color look-up table.
		virtual Xe::Graphics::ISurface* GetClutSurface() = 0;

		//! \brief Select the texture for the next draw.
		virtual void SelectTexture(TexId texId, int slot) = 0;

		//! \brief Select the texture for the next draw.
		virtual void SelectClut(int slot) = 0;

		//! \brief Get the size of the specified texture
		//! \param[in] texId Must be a valid TexId
		virtual Math::Vector2i GetTextureSize(TexId texId) const = 0;

		//! \brief Transform the specified UV coordinates into the correct versions.
		virtual void GetUv(TexId texId, Math::Vector2f(uv)[4]) = 0;

		//! \brief Get the UV coordinates from the LTRB.
		virtual void GetUvLTRB(TexId texId, Math::Vector2f(uv)[4], u16 left, u16 top, s16 right, s16 bottom) = 0;

		//! \brief Increases the references count of a texture.
		//! \param[in] texId lower the references count by 1.
		/** \details When the same texture has to be used by different objects
		 * then it is convenient to increase the reference count.
		 */
		virtual TexId AddReference(TexId) = 0;

		//! \brief Remove a reference or delete a texture.
		//! \param[in] texId lower the references count by 1.
		//! \return texId if GetReferencesCount was greater than 1, else TexInvalid.
		/** \details Contrary of AddReference. If GetReferenceCount is 1 and
		 * this is called, then the texture will be deleted and the TexId will
		 * not be longer valid.
		 */
		virtual TexId RemoveReference(TexId) = 0;

		//! \brief Get the references count for a specific texture.
		//! \return A number higher than 0 if the texture is valid.
		virtual int GetReferencesCount(TexId texId) const = 0;

		//!	\brief Get the default Clut associated with the texture.
		//! \param[in] texId Texture which optionally contains the Clut.
		//! \return A valid ClutId or ClutInvalid if the texture does not have any palette.
		virtual ClutId GetDefaultClut(TexId texId) const = 0;

		//!	\brief Create a new Clut from the default one.
		//! \param[in] texId The texture that will holds the specified Clut.
		//! \return A new ClutId or ClutInvalid if the texture cannot hold any more Cluts.
		virtual ClutId AllocateClut(TexId texId) = 0;

		//! \brief Free a previously allocated Clut.
		//! \param[in] clutId Clut to free; must not be a clut obtained from GetDefaultClut.
		//! \return Returns the default Clut or ClutInvalid.
		virtual ClutId FreeClut(ClutId clutId) = 0;

		//! \brief Get the texture from the ClutId
		virtual TexId GetTextureId(ClutId clutId) const = 0;

		//! \brief Get the normalized lines index.
		virtual float GetClutIndexf(ClutId clutId) const = 0;

		//! \brief Lock the data for the specified Clut. The data must be then unlocked.
		//! \param[in] clutId
		//! \param[out] cllutData
		virtual void LockClut(ClutId clutId, ClutData& clutData) = 0;

		//! \brief Unlock a previously locked data.
		virtual void UnlockClut(ClutId clutId, ClutData& clutData) = 0;

		//! \brief Get a snapshot of the current usage profile
		//! \param[out] profile Profile to fill
		//! \param[in] maxEntriesCount The maximum number of entries that the profile can hold.
		//! \return true if the profile was available; usually returns false on RELEASE configurations.
		virtual bool GetProfile(TextureManagerProfile& profile) = 0;
	};
} }
