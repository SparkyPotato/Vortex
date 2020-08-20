#pragma once
#include <VortexMinimal.h>
#ifdef PLATFORM_WINDOWS
#include <shobjidl.h>
#endif

DECLARE_LOGGER(LogAssetManager);

class AssetLayer : public Vortex::Layer
{
public:
	AssetLayer(bool* isAssetManagerOpen);
	virtual ~AssetLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	
	virtual void Tick(float deltaTime) override;
	virtual void OnGuiRender() override;

private:
	void ShowAssetNamePopup();
	void OpenErrorPopup();
	void LoadTextureFromFile();

	bool* m_IsOpen;

	bool m_OpenError = false;
	std::string m_ErrorMessage;

	bool m_IsLoadingTexture = false;
	bool m_HasEnteredName = false;
	bool m_ShowAssetName = false;
	char m_AssetNameBuffer[1024];
	std::string m_AssetName;

	Vortex::GPTexture* m_LoadedTexture = nullptr;
	bool m_IsTextureViewerOpen = false;

	Vortex::GPTexture* m_ViewedTexture = nullptr;
	std::string m_ViewedTextureName;

	#ifdef PLATFORM_WINDOWS
	IFileOpenDialog* p_FileOpen;
	#endif
};

extern std::map<std::string, Vortex::GPTexture*> GTextureAssets;
