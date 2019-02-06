#pragma once
#include <XeGame/IGameTilemap2d.h>

using namespace Xe::Math;

namespace Xe { namespace Game {
	class CTilemapLayer;
	class CTilemap2d : public ITilemap2d
	{
		template <class T>
		struct MyVector
		{
			T* Data;
			size_t Capacity;
			size_t Count;

			MyVector() : Data(nullptr), Capacity(0), Count(0) {}
			inline void Realloc(size_t capacity) { Data = (T*)Xe::Memory::Resize(capacity); }
			inline void Release() { if (Data) Xe::Memory::Free(Data); }
			inline void Clear() { Count = 0; }
			inline void Reserve(size_t newElements) { EnsureCapacity(Count += newElements); }

			inline void EnsureCapacity(size_t requiredCapacity)
			{
				if (Capacity < requiredCapacity)
				{
					SetCapacity(requiredCapacity + requiredCapacity / 4); // * 1.25
				}
			}

			inline void SetCapacity(size_t capacity)
			{
				Capacity = capacity;
				Data = (T*)Memory::Resize(Data, Capacity * sizeof(T));
			}
		};

		struct TileSequence
		{
			TileData TileId;
			float Speed;
			MyVector<TileFrame> Sequence;
		};

		TilemapRequestTilesDelegate* m_RequestTilesDelegate;
		Math::Vector2i m_CameraSize;
		Math::Vector2f m_CameraPosition;
		Xe::Math::Vector2i m_TileSize;
		mutable Xe::Math::Vector2i m_BufferSizeDELETEME;
		Graphics::Color m_BgColor;
		double m_Timer;
		std::list<TileSequence> m_AnimatedTiles;
		std::vector<ObjPtr<ITilemapLayer>> m_Layers;

		MyVector<TilemapDrawVertex> m_DrawVertices;
		MyVector<TilemapDrawIndex> m_DrawIndices;
		MyVector<Xe::Graphics::Color> m_DrawColors;
		MyVector<float> m_DrawTextureModes;

		template <class T>
		static T* EnsureCapacity(T* mem, size_t currentCapacity, size_t requiredCapacity)
		{
			return currentCapacity < requiredCapacity ? (T*)Memory::Resize(mem, requiredCapacity * sizeof(T)) : mem;
		}

		u16 PeekColor() const;
		u16 PushColor(const Xe::Graphics::Color& color);

		u16 PeekTexMode() const;
		u16 PushTexMode(float mode);
		u16 PushTexModeNoTexture();
		u16 PushTexModeTexture();
		u16 PushTexModePalette(float palette);

		TileData GetTileData(TileData tile) const;

		void FetchLayer(CTilemapLayer& layer, size_t layerIndex);

		bool RenderBegin(TilemapDrawArgs& drawArgs);
		void RenderEnd(TilemapDrawArgs& drawArgs);
		void DrawBackground();
		void DrawLayer(const CTilemapLayer& layer, TilemapDrawFlags drawFlags);
	public:
		CTilemap2d();
		~CTilemap2d();

		void SetRequestTilesCallback(TilemapRequestTilesDelegate* delegate);

		const Xe::Graphics::Color& GetBackgroundColor() const;
		void SetBackgroundColor(const Xe::Graphics::Color& color);

		const Math::Vector2i& GetCameraSize() const;
		void SetCameraSize(const Math::Vector2i& cameraSize);

		const Xe::Math::Vector2f& GetCameraPosition() const;
		void SetCameraPosition(const Xe::Math::Vector2f& cameraPosition);

		const Xe::Math::Vector2i& GetTileSize() const;
		void SetTileSize(const Xe::Math::Vector2i& tileSize);

		bool GetTileSequence(TileData tile, std::vector<TileFrame>& frames) const;
		void AddTileSequence(TileData tile, const Xe::Collections::Array<TileFrame>& frames);
		void RemoveTileSequence(TileData tile);

		size_t GetLayerCount() const;
		void SetLayersCount(size_t layersCount);
		ObjPtr<ITilemapLayer> GetLayer(size_t index);

		const Xe::Math::Vector2i& GetBufferSize() const;
		void SetBufferSize(const Xe::Math::Vector2i& bufferSize);

		void Update(double deltaTime);
		void Flush();

		void Draw(TilemapDrawArgs& drawArgs, TilemapDrawFlags drawFlags);
		void DrawLayer(size_t layerIndex, TilemapDrawArgs& drawArgs, TilemapDrawFlags drawFlags);
	};
} }