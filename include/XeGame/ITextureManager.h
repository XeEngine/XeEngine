#pragma once
#include <XeSDK/IStorage.h>
#include <XeSDK/XeGraphicsImaging.h>

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
		int Width, Height;
		int ReferencesCount;
		int ClutsCount;
	};

	struct TextureManagerProfile
	{
		int TexturesCount;
		int MaxTexturesCount;
		int ClutsCount;
		int MaxClutsCount;
		int TexturesReferencesCount;
		int MaxClutsCountPerTexture;
		TextureManagerProfileEntry *Textures;
	};

	struct ITextureManager
	{
		//! \brief Create a texture from an image.
		//! \param[in] image Image that will be loaded and transformed as a texture.
		//! \param[in] name An unique name from the image, usually used to cache them; cannot be null..
		//! \return A proper TexId or TexInvalid.
		virtual TexId Create(Xe::Graphics::Imaging::IImage& image, ctstring name) = 0;

		//! \brief Get the TexId from the unique name
		//! \param[in] name Unique name of the image; cannot be null.
		//! \return A proper TexId or TexInvalid if it was not found.
		virtual TexId Get(ctstring name) const = 0;

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
		virtual TexId GetTexture(ClutId clutId) const = 0;

		//! \brief Get the data for the specified Clut
		//! \param[in] clutId
		//! \param[out] cllutData
		virtual void GetClutData(ClutId clutId, ClutData& clutData) = 0;

		//! \brief Get a snapshot of the current usage profile
		//! \param[out] profile Profile to fill
		//! \param[in] maxEntriesCount The maximum number of entries that the profile can hold.
		//! \return true if the profile was available; usually returns false on RELEASE configurations.
		virtual bool GetProfile(TextureManagerProfile& profile, int maxEntriesCount) = 0;
	};
} }
