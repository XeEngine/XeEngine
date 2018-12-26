#include "pch.h"
#include <XeGame/XeGameToolsFilePicker.h>
#include <imgui/imgui.h>
#include "../XeSDK/XeStorageFile.h"

using namespace Xe::Game;
using namespace Xe::Game::Tools;

namespace Xe { namespace Game { namespace Tools {
	FilePicker::FilePicker() :
		m_CurrentDirectory(""),
		m_CurrentFileName(""),
		m_DelegateResult(nullptr),
		m_Title("File picker"),
		m_PickerType(PickerType_OpenFile),
		m_FileEntrySelectedIndex(-1),
		m_IsVisible(true),
		m_IsOpen(false)
	{
		GetStorage(&m_Storage);
	}
	
	FilePicker::~FilePicker()
	{ }

	FilePicker::PickerType FilePicker::GetPickerType() const
	{
		return m_PickerType;
	}

	void FilePicker::SetPickerType(PickerType pickerType)
	{
		m_PickerType = pickerType;
	}

	const char* FilePicker::GetTitle() const
	{
		return m_Title;
	}

	void FilePicker::SetTitle(const StringSpan& title)
	{
		m_Title = title;
	}

	StringSpan FilePicker::GetFileName() const
	{
		return m_CurrentFileName;
	}

	void FilePicker::SetFileName(const StringSpan& path)
	{
		SetCurrentDirectory(path);
		SetCurrentFileName(path);
		FetchDirectoryContent();
	}

	bool FilePicker::IsVisible() const
	{
		return m_IsVisible;
	}

	void FilePicker::SetVisible(bool visible)
	{
		m_IsVisible = visible;
	}

	void FilePicker::Open(Xe::IDelegate<const EventArgs<FilePicker&, PickerResult>&>* delegate)
	{
		FetchDirectoryContent();
		m_IsOpen = true;
		m_DelegateResult = delegate;
	}

	void FilePicker::operator()(const ImGuiFrameArgs& args)
	{
		if (!m_IsVisible || !m_IsOpen) return;

		ImGui::ShowDemoWindow();

		bool isOpen = true;
		if (ImGui::Begin(GetTitle(), &isOpen))
		{
			bool isFolderSelected = false;
			
			if (ImGui::InputText("Path", m_CurrentDirectory, sizeof(m_CurrentDirectory)))
			{
				if (m_Storage->DirectoryExists(m_CurrentDirectory))
				{
					FetchDirectoryContent();
				}
			}

			if (ImGui::InputText("File", m_CurrentFileName, sizeof(m_CurrentFileName)))
			{
			}

			ImGui::SameLine();
			if (ImGui::Button("Ok"))
			{
				CloseWithResult(PickerResult_Ok);
			}

			if (isFolderSelected)
			{
				FetchDirectoryContent();
				m_FileEntrySelectedIndex = -1;
			}

			if (ImGui::BeginChild("#FilesList"))
			{
				ImGui::Columns(3, "#FilesGrid");
				ImGui::Text("Name"); ImGui::NextColumn();
				ImGui::Text("Size"); ImGui::NextColumn();
				ImGui::Text("Undef"); ImGui::NextColumn();
				ImGui::Separator();

				int index = 0;
				for (const auto& entry : m_DirEntries)
				{
					if (ImGui::Selectable(entry.Name.GetData(), IsFileEntrySelected(index++)))
					{
						SetCurrentDirectory(Storage::Path::Combine(m_CurrentDirectory, entry.Name));
						isFolderSelected = true;
					}

					ImGui::NextColumn();
					ImGui::NextColumn();
					ImGui::NextColumn();
				}

				const char* SizeSuffix[]
				{
					"B ", "KB", "MB", "GB", "TB", "PB"
				};

				for (const auto& entry : m_FileEntries)
				{
					if (ImGui::Selectable(entry.Name.GetData(), false))
					{
						SetCurrentFileName(Xe::Storage::Path::Combine(m_CurrentDirectory, entry.Name));
					}

					ImGui::NextColumn();

					// Calculate the short version of file size
					int suffixIndex;
					double fileSize = (double)entry.Size;
					for (suffixIndex = 0; suffixIndex < lengthof(SizeSuffix) && fileSize >= 1000; ++suffixIndex)
					{
						fileSize /= 1024.0;
					}

					ImGui::Text("%.2f %s", fileSize, SizeSuffix[suffixIndex]);
					ImGui::NextColumn();
					ImGui::NextColumn();
				}
			}

			if (isFolderSelected)
			{
				FetchDirectoryContent();
			}

			ImGui::EndChild();
		}

		if (!isOpen)
		{
			CloseWithResult(PickerResult_Cancel);
		}

		ImGui::End();
	}


	void FilePicker::FetchDirectoryContent()
	{
		m_DirEntries.clear();
		m_FileEntries.clear();

		Storage::IDirectory* pDirectory;
		if (m_Storage->DirectoryOpen(&pDirectory, m_CurrentDirectory) != Error::OK)
		{
			return;
		}

		while (pDirectory->Next())
		{
			FileEntry entry;
			entry.Name = pDirectory->GetName();
			entry.Size = pDirectory->GetLength();

			if (pDirectory->IsDirectory())
			{
				m_DirEntries.push_back(entry);
			}
			else
			{
				entry.Index = m_FileEntries.size();
				m_FileEntries.push_back(entry);
			}
		}
	}

	void FilePicker::SetCurrentDirectory(String path)
	{
		/*while (!m_Storage->DirectoryExists(path))
		{
			path = StringSpan(Storage::Path::GetDirectoryName(path));
		}*/

		SetCString(m_CurrentDirectory, sizeof(m_CurrentDirectory), path);
	}

	void FilePicker::SetCurrentFileName(String path)
	{
		SetCString(m_CurrentFileName, sizeof(m_CurrentDirectory), Storage::Path::GetFileName(path));
	}

	bool FilePicker::IsFileEntrySelected(int index)
	{
		if (index >= 0 && (size_t)index < m_FileEntries.size())
		{
			return m_FileEntries[index].Name == StringSpan(m_CurrentFileName);
		}

		return false;
	}

	void FilePicker::CloseWithResult(PickerResult result)
	{
		m_IsOpen = false;
		if (m_DelegateResult)
		{
			EventArgs<FilePicker&, PickerResult> args
			{
				*this, result
			};

			(*m_DelegateResult)(args);
		}
	}

	void FilePicker::SetCString(char* outStr, size_t length, const StringSpan& str)
	{
		length = Xe::Math::Min<size_t>(str.GetLength(), length - 1);
		Memory::Copy(outStr, str.GetData(), length);
		outStr[length] = '\0';
	}

} } }