#pragma once
#pragma once
#include <XeGame/IGameToolsFrame.h>
#include <XeSDK/IDelegate.h>

namespace Xe { namespace Game { namespace Tools
{
	class FilePicker :	public Xe::Game::Tools::IFrame
	{
	public:
		enum PickerType
		{
			PickerType_OpenFile,
			PickerType_SaveFile,
			PickerType_OpenFolder
		};

		enum PickerResult
		{
			PickerResult_Ok,
			PickerResult_Cancel
		};

		FilePicker();
		virtual ~FilePicker();

		PickerType GetPickerType() const;
		void SetPickerType(PickerType pickerType);

		const char* GetTitle() const;
		void SetTitle(const StringSpan& title);

		StringSpan GetFileName() const;
		void SetFileName(const StringSpan& path);

		bool IsVisible() const;
		void SetVisible(bool visible);

		void Open(Xe::IDelegate<const EventArgs<FilePicker&, PickerResult>&>* delegate);

		void operator()(const Xe::Game::ImGuiFrameArgs& args);

	private:
		struct FileEntry
		{
			String Name;
			u64 Size;
			int Index;
		};

		char m_CurrentDirectory[1024];
		char m_CurrentFileName[256];

		ObjPtr<Xe::Storage::IStorage> m_Storage;
		Xe::IDelegate<const EventArgs<FilePicker&, PickerResult>&>* m_DelegateResult;
		String m_Title;
		PickerType m_PickerType;
		int m_FileEntrySelectedIndex;
		bool m_IsVisible;
		bool m_IsOpen;

		std::vector<FileEntry> m_DirEntries;
		std::vector<FileEntry> m_FileEntries;

		void FetchDirectoryContent();
		void SetCurrentDirectory(String path);
		void SetCurrentFileName(String path);
		bool IsFileEntrySelected(int index);
		void CloseWithResult(PickerResult result);

		static void SetCString(char* outStr, size_t length, const StringSpan& str);
	};
} } }