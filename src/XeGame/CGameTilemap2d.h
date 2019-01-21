#pragma once
#include <XeGame/IGameTilemap2d.h>
#include <list>

namespace Xe { namespace Game {
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

		struct Layer
		{
			TileData* Data;
			Xe::Math::Vector2i Size;
			bool Visible;
		};

		struct TileSequence
		{
			TileData TileId;
			float Speed;
			MyVector<TileFrame> Sequence;
		};

		TilemapRequestTilesDelegate* m_RequestTilesDelegate;
		TilemapDrawDelegate* m_DrawDelegate;
		Math::Vector2i m_CameraSize;
		Math::Vector2f m_CameraPosition;
		Xe::Math::Vector2i m_TileSize;
		Xe::Math::Vector2i m_BufferSize;
		TilesetProperties m_Tileset;
		Graphics::Color m_BgColor;
		double m_Timer;
		Layer m_Layer;
		std::list<TileSequence> m_AnimatedTiles;

		MyVector<TilemapDrawVertex> m_DrawVertices;
		MyVector<TilemapDrawIndex> m_DrawIndices;
		MyVector<Xe::Graphics::Color> m_DrawColors;
		MyVector<float> m_DrawTextureModes;

		static void ResizeLayer(const Xe::Math::Vector2i& size, Layer& layer);

		template <class T>
		static T* EnsureCapacity(T* mem, size_t currentCapacity, size_t requiredCapacity)
		{
			return currentCapacity < requiredCapacity ? (T*)Memory::Resize(mem, requiredCapacity * sizeof(T)) : mem;
		}

		u16 PushColor(const Xe::Graphics::Color& color);
		u16 PushTexMode(float mode);
		u16 PushTexModeNoTexture();
		u16 PushTexModeTexture();
		u16 PushTexModePalette(float palette);

		TileData GetTileData(TileData tile) const;
	public:
		CTilemap2d();
		~CTilemap2d();

		void SetRequestTilesCallback(TilemapRequestTilesDelegate* delegate);
		void SetDrawCallback(TilemapDrawDelegate* delegate);

		const Xe::Graphics::Color& GetBackgroundColor() const;
		void SetBackgroundColor(const Xe::Graphics::Color& color);

		const Math::Vector2i& GetCameraSize() const;
		void SetCameraSize(const Math::Vector2i& cameraSize);

		const Math::Vector2f& GetCameraPosition() const;
		void SetCameraPosition(const Math::Vector2f& cameraPosition);

		const Xe::Math::Vector2i& GetTileSize() const;
		void SetTileSize(const Xe::Math::Vector2i& tileSize);

		bool GetTileSequence(TileData tile, std::vector<TileFrame>& frames) const;
		void AddTileSequence(TileData tile, const Xe::Collections::Array<TileFrame>& frames);
		void RemoveTileSequence(TileData tile);

		const Xe::Math::Vector2i& GetBufferSize() const;
		void SetBufferSize(const Xe::Math::Vector2i& bufferSize);

		bool IsLayerVisible() const;
		void SetLayerVisible(bool visible);

		bool GetBuffer(TilemapData* layer);

		void SetTileset(const TilesetProperties& tileset);

		void Update(double deltaTime);
		void Flush();
		void Draw(int flags);
	};
} }