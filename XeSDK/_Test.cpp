#include "pch.h"
/*#include <XeSDK.h>

namespace Xe {
	class App : public Xe::Application {
		Xe::Storage::IStorage *m_pStorage;
		Xe::Graphics::ISurface *m_pSurface;
		Xe::Game::Tiled *m_pTiled;

		Xe::Math::Vector2f m_pos;
		Xe::Math::Vector2f m_RedRectPos;
		Xe::Math::Vector2f m_RedRectPosInc;

		void MoveMap(const Math::Vector2f& pos);
	public:
		bool Query(IObject **obj, Xe::UID id);

		App();
		~App();

		void OnContextInitialize(Xe::Graphics::ContextProperties& properties);
		bool OnInitialize();
		void OnDestroy();
		bool OnUpdate(double deltaTime);
		void OnDraw();

		void OnKeybDown(const Key &key);
		void OnSizeChanged(const Graphics::Size& size);

		bool CreateSurfaceFromFilename(Xe::Graphics::ISurface **surface, Xe::Graphics::IContext *context, ctstring filename);
	};
}

void CreateApplication(Xe::Application** app) {
	*app = new Xe::App;
}

int main() {
	Xe::Application *pApp = nullptr;
	CreateApplication(&pApp);
	if (pApp != nullptr) {
		bool r = Xe::Run(pApp);
		pApp->Release();
		return r ? 0 : 1;
	}
	LOG(Xe::Debug::Log::Priority_Error,
		Xe::Debug::Log::Type_Generic,
		_T("CreateApplication has not provided a valid Application pointer."));
	return 1;
}

namespace Xe {
	bool App::Query(IObject **obj, Xe::UID id) {
		switch (id) {
		case Xe::Application::ID:
		case Xe::IApplication::ID:
		case Xe::IObject::ID:
			AddRef();
			*obj = this;
			return true;
		case Xe::Storage::IStorage::ID:
			if (m_pStorage) m_pStorage->AddRef();
			*obj = m_pStorage;
			return m_pStorage != 0;
		}
		return false;
	}

	App::App() :
		m_pStorage(0),
		m_pSurface(0),
		m_pTiled(0) {
	}
	App::~App() {
	}
	void App::OnContextInitialize(Xe::Graphics::ContextProperties& properties) {
		properties.VideoCardIndex = 0;
		properties.IsResizable = true;
	}
	bool App::OnInitialize() {
		if (!GetDefaultContainer(&m_pStorage))
			return false;
		CreateSurfaceFromFilename(&m_pSurface, m_pContext, _T("MAPS/TEST/TILESET.PNG"));
		m_pContext->SetClearColor(Xe::Graphics::Color::SlateGray);

		// Set Tilemap2d attributes
		Xe::Graphics::Tilemap2d *pTilemap;
		m_pContext->CreateTilemap(&pTilemap);
		pTilemap->SelectSurface(m_pSurface);
		pTilemap->SetViewSize(Math::Vector2f(480.0f, 270.0f));
		// Load a map from a file
		IO::IFileStream *fStream;
		if (m_pStorage->FileOpen(&fStream, _T("MAPS/TEST/MAP.TMX"), Storage::Access_Read, Storage::Mode_Open) == Error::OK) {
			Game::Tiled::Open(&m_pTiled, pTilemap, fStream);
			fStream->Release();
		}
		pTilemap->Release();
		// anim stuff
		m_RedRectPosInc.x = 1.0f;
		m_RedRectPosInc.y = 1.0f;
		// Success :D
		return true;
	}
	void App::OnDestroy() {
		if (m_pTiled) m_pTiled->Release();
		if (m_pSurface) m_pSurface->Release();
		if (m_pStorage) m_pStorage->Release();
	}
	bool App::OnUpdate(double deltaTime) {
		if (m_pTiled) m_pTiled->Update(deltaTime);

		m_RedRectPos.x += m_RedRectPosInc.x * (float)deltaTime;
		if (m_RedRectPos.x >= 1.0f) {
			m_RedRectPos.x = 1.0f;
			m_RedRectPosInc.x *= -1.0f;
		}
		else if (m_RedRectPos.x <= 0.0f) {
			m_RedRectPos.x = 0.0f;
			m_RedRectPosInc.x *= -1.0f;
		}

		return true;
	}
	void App::OnDraw() {
		const Math::Vector2f POS[] = {
			Math::Vector2f(-1.0f, +1.0f) + m_RedRectPos,
			Math::Vector2f(+0.0f, -1.0f) + m_RedRectPos,
		};
		static const Math::Vector2f POS2[] = {
			Math::Vector2f(+0.8f, -0.8f),
			Math::Vector2f(-0.2f, +0.2f),
		};
		static const Math::Vector2f UV[] = {
			Math::Vector2f(0.0f, 0.0f),
			Math::Vector2f(1.0f, 0.0f),
			Math::Vector2f(0.0f, 1.0f),
			Math::Vector2f(1.0f, 1.0f),
		};

		m_pContext->Clear(Graphics::Clear_Color | Graphics::Clear_Depth | Graphics::Clear_Stencil);
		Xe::Graphics::IDrawing2d *pDrawing;
		m_pContext->GetDrawing(&pDrawing);
		pDrawing->DrawRectangle(POS, Graphics::Color::Red);
		m_pContext->SelectSurface(m_pSurface, 0);
		pDrawing->DrawSurface(POS2, UV, Graphics::Color::White, pDrawing->MODE_TEXTURED);
		pDrawing->Flush();
		pDrawing->Release();

		Xe::Graphics::Tilemap2d *pTilemap;
		m_pContext->CreateTilemap(&pTilemap);
		//pTilemap->Draw();
		pTilemap->Release();

		m_pContext->SwapBuffers(m_pContext->VBlank_Wait);
	}
	void App::OnKeybDown(const Key &key) {
		switch (key.key) {
		case 0x25: // LEFT
			MoveMap(Math::Vector2f(-1.0f, 0.0f));
			break;
		case 0x26: // UP
			MoveMap(Math::Vector2f(0.0f, -1.0f));
			break;
		case 0x27: // RIGHT
			MoveMap(Math::Vector2f(+1.0f, 0.0f));
			break;
		case 0x28: // DOWN
			MoveMap(Math::Vector2f(0.0f, +1.0f));
			break;
		}
	}
	void App::OnSizeChanged(const Graphics::Size& size) {
		m_pContext->SetInternalResolution(size);
	}
	void App::MoveMap(const Math::Vector2f& pos) {
		m_pos += pos * 4.0f;
		m_pTiled->SetPosition(m_pos);
	}
	bool App::CreateSurfaceFromFilename(Xe::Graphics::ISurface **surface, Xe::Graphics::IContext *context, ctstring filename) {
		bool res = false;
		*surface = nullptr;

		IO::IFileStream *fStream;
		if (Storage::File::Open(&fStream, filename, Storage::Access_Read, Storage::Mode_Open)) {
			Graphics::Imaging::IImage *image;
			if (Graphics::Imaging::PngOpen(&image, fStream)) {
				res = Graphics::Imaging::CreateSurface(context, surface, Graphics::SurfaceType_Texture, image);
				image->Release();
			}
			fStream->Release();
		}
		return res;
	}
}*/