#include <EditorLayers/AssetLayer.h>
#ifdef PLATFORM_WINDOWS
#include <Private/Platforms/Windows/WWindow.h>
#include <Private/Platforms/DirectX11/DX11Texture.h>
#endif

using namespace Vortex;

std::map<std::string, GPTexture*> GTextureAssets;

DEFINE_LOGGER(LogAssetManager);

AssetLayer::AssetLayer(bool* isAssetManagerOpen)
	: m_IsOpen(isAssetManagerOpen)
{
	CREATE_LOGGER(LogAssetManager, spdlog::level::trace);
	memset(m_AssetNameBuffer, 0, sizeof(m_AssetNameBuffer));

	#ifdef PLATFORM_WINDOWS
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		VX_ERROR(LogAssetManager, "Failed to initialize COM! Cannot proceed.");
		throw std::exception("COM initialization failed.");
	}
	#endif
}

AssetLayer::~AssetLayer()
{
	#ifdef PLATFORM_WINDOWS
	CoUninitialize();
	#endif
}

void AssetLayer::OnAttach()
{
	HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&p_FileOpen));
	if (FAILED(hr))
	{
		VX_ERROR(LogAssetManager, "Failed to create file open dialog! Cannot proceed.");
		throw std::exception("File open dialog creation failed.");
	}

	p_FileOpen->SetTitle(L"Import Asset");
	p_FileOpen->SetOkButtonLabel(L"Import");
}

void AssetLayer::OnDetach()
{
	p_FileOpen->Release();

	for (auto& pair : GTextureAssets)
	{
		delete pair.second;
	}
}

void AssetLayer::Tick(float deltaTime)
{
	
}

void AssetLayer::OnGuiRender()
{
	if (*m_IsOpen)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, { 400, 200 });
		if (ImGui::Begin("Asset Manager", m_IsOpen))
		{
			if (ImGui::Button("Add Asset"))
			{
				std::thread loadThread(&AssetLayer::LoadTextureFromFile, this);
				loadThread.detach();
			}

			ImVec2 region = ImGui::GetContentRegionAvail();
			ImVec2 offset = { ImGui::GetFontSize(), ImGui::GetFontSize() * 5.f };
			for (auto& texture : GTextureAssets)
			{
				ImGui::SetCursorPos(offset);
				if (GraphicsContext::Get()->GetAPI() == GraphicsAPI::DirectX11)
				{
					ImGui::ImageButton(reinterpret_cast<DX11Texture*>(texture.second)->GetShaderResource(), { 100, 100 });
					if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
					{
						m_ViewedTextureName = texture.first;
						m_ViewedTexture = texture.second;
						m_IsTextureViewerOpen = true;
					}
				}

				ImGui::SetCursorPos({ offset.x, offset.y + 100 + ImGui::GetFontSize() });

				ImGui::Text(texture.first.c_str());

				offset.x += 100 + ImGui::GetFontSize();
				if (offset.x > region.x - (100 + ImGui::GetFontSize()))
				{
					offset.x = ImGui::GetFontSize();
					offset.y += 100 + ImGui::GetFontSize() * 3.f;
				}
			}

			ImGui::End();
		}
		else
		{
			ImGui::End();
		}
		ImGui::PopStyleVar();

		if (m_IsLoadingTexture)
		{
			if (!m_LoadedTexture)
			{
				int corner = 3;
				ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImVec2 work_area_pos = viewport->GetWorkPos();   // Instead of using viewport->Pos we use GetWorkPos() to avoid menu bars, if any!
				ImVec2 work_area_size = viewport->GetWorkSize();
				ImVec2 window_pos = ImVec2((corner & 1) ? (work_area_pos.x + work_area_size.x - 100) : (work_area_pos.x + 100),
					(corner & 2) ? (work_area_pos.y + work_area_size.y - 100) : (work_area_pos.y + 100));
				ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
				ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
				ImGui::SetNextWindowViewport(viewport->ID);

				ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
				ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
					ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;

				if (ImGui::Begin("Loading Overlay", nullptr, windowFlags))
				{
					ImGui::Text("Loading Texture...");

					ImGui::End();
				}
				else
				{
					ImGui::End();
				}
			} 
			else if (m_HasEnteredName)
			{
				m_IsLoadingTexture = false;
				m_HasEnteredName = false;
				GTextureAssets.insert({ m_AssetName, m_LoadedTexture });
				m_LoadedTexture = nullptr;
			}
		}
		else
		{
			if (m_LoadedTexture)
			{
				VX_TRACE(LogAssetManager, "Here");
				delete m_LoadedTexture;
				m_LoadedTexture = nullptr;
			}
		}

		OpenErrorPopup();
		ShowAssetNamePopup();
	}

	if (m_ViewedTexture && m_IsTextureViewerOpen)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, { 200, 200 });
		if (ImGui::Begin(("Texture Viewer - " + m_ViewedTextureName + "###Texture Viewer").c_str(), &m_IsTextureViewerOpen))
		{
			ImVec2 windowSize = ImGui::GetContentRegionAvail();

			float aspectRatio = (float) m_ViewedTexture->GetWidth() / (float) m_ViewedTexture->GetHeight();
			if (windowSize.x * aspectRatio >= windowSize.y)
			{
				if (windowSize.x * aspectRatio > windowSize.y)
				{
					ImGui::SetCursorPosX((windowSize.x - windowSize.y * aspectRatio) / 2.f);
				}

				if (GraphicsContext::Get()->GetAPI() == GraphicsAPI::DirectX11)
				{
					ImGui::Image
					(
						reinterpret_cast<DX11Texture*>(m_ViewedTexture)->GetShaderResource(),
						{ windowSize.y * aspectRatio, windowSize.y }
					);
				}
			}
			else
			{
				if (GraphicsContext::Get()->GetAPI() == GraphicsAPI::DirectX11)
				{
					ImGui::Image
					(
						reinterpret_cast<DX11Texture*>(m_ViewedTexture)->GetShaderResource(),
						{ windowSize.x * aspectRatio, windowSize.x }
					);
				}
			}

			ImGui::End();
		}
		else
		{
			ImGui::End();
		}
		ImGui::PopStyleVar();
	}
}

void AssetLayer::ShowAssetNamePopup()
{
	if (m_ShowAssetName)
	{
		ImGui::OpenPopup("Asset Name");
	}

	if (ImGui::BeginPopup("Asset Name"))
	{
		if (m_ShowAssetName)
		{
			ImGui::SetKeyboardFocusHere();
			m_ShowAssetName = false;
		}

		if (ImGui::InputTextWithHint("###Name", "Asset Name", m_AssetNameBuffer, sizeof(m_AssetNameBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			m_AssetName = std::string(m_AssetNameBuffer);
			if (!m_AssetName.empty())
			{
				if (GTextureAssets.find(m_AssetName) == GTextureAssets.end())
				{
					m_HasEnteredName = true;
				}
				else
				{
					m_OpenError = true;
					m_ErrorMessage = "Cannot create two assets with the same name!";
				}
			}
			else
			{
				m_IsLoadingTexture = false;
			}
			ImGui::CloseCurrentPopup();
			memset(m_AssetNameBuffer, 0, sizeof(m_AssetNameBuffer));
		}

		ImGui::EndPopup();
	}
	else
	{
		if(!m_HasEnteredName) m_IsLoadingTexture = false;
	}
}

void AssetLayer::OpenErrorPopup()
{
	if (m_OpenError)
	{
		ImGui::OpenPopup("Error###Popup");
		m_OpenError = false;
	}

	ImGui::SetNextWindowPos({ ImGui::GetIO().DisplaySize.x / 2 - 200, ImGui::GetIO().DisplaySize.y / 2 - 100 });
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, { 400, 200 });
	if (ImGui::BeginPopupModal("Error###Popup", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
	{
		ImGui::Text("%s", m_ErrorMessage.c_str());

		if (ImGui::Button("Close", { ImGui::GetContentRegionAvail().x, 0 }))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
	ImGui::PopStyleVar();
}

void AssetLayer::LoadTextureFromFile()
{
	try
	{
	#ifdef PLATFORM_WINDOWS
		HRESULT hr = p_FileOpen->Show(reinterpret_cast<WWindow*>(GWindow)->GetWindowHandle());

		if (SUCCEEDED(hr))
		{
			IShellItem* item;
			hr = p_FileOpen->GetResult(&item);

			if (SUCCEEDED(hr))
			{
				LPWSTR filePath;
				hr = item->GetDisplayName(SIGDN_FILESYSPATH, &filePath);

				if (SUCCEEDED(hr))
				{
					m_ShowAssetName = true;

					std::wstring temp(filePath);
					int size_needed = WideCharToMultiByte(CP_UTF8, 0, temp.c_str(), -1, NULL, 0, NULL, NULL);
					std::string path(size_needed, 0);
					WideCharToMultiByte(CP_UTF8, 0, temp.c_str(), (int)temp.size(), &path[0], size_needed, NULL, NULL);

					m_IsLoadingTexture = true;
					m_LoadedTexture = GPTexture::Create(path);
				}
				else
				{
					VX_ERROR(LogAssetManager, "Failed to get file path while loading texture!");
				}

				item->Release();
			}
			else
			{
				VX_ERROR(LogAssetManager, "Failed to get selected file!");
			}
		}
		else
		{
			VX_ERROR(LogAssetManager, "Failed to show file open dialog!");
		}
	#endif
	}
	catch (...)
	{
		m_OpenError = true;
		m_ErrorMessage = "Failed to load texture!";
	}
}
