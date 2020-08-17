#include <EditorLayers/WorldLayer.h>

using namespace Vortex;

DEFINE_LOGGER(LogWorld);

WorldLayer::WorldLayer(bool* isWorldOpen, bool* isPropertiesOpen)
	: m_IsWorldOpen(isWorldOpen), m_IsPropertiesOpen(isPropertiesOpen)
{
	CREATE_LOGGER(LogWorld, spdlog::level::trace);

	m_World = new World;
	auto test = m_World->CreateEntity("Test Entity");
	test->AddCameraComponent();
	test->GetTransform().SetPosition({ 0.f, 0.f, -2.f });

	auto square = m_World->CreateEntity("Square");
	square->AddSpriteComponent();
	square->GetTransform().SetPosition({ 0.f, 0.f, 2.f });

	memset(m_NewEntityNameBuffer, 0, sizeof(m_NewEntityNameBuffer));
	memset(m_EntityNameBuffer, 0, sizeof(m_EntityNameBuffer));
}

WorldLayer::~WorldLayer()
{
	delete m_World;
}

void WorldLayer::OnAttach()
{
	GRenderer->RenderWorld(m_World);
}

void WorldLayer::OnDetach()
{
	GRenderer->RenderWorld(m_World);
}

void WorldLayer::Tick(float deltaTime)
{

}

void WorldLayer::OnGuiRender()
{
	if (*m_IsWorldOpen)
	{
		if (ImGui::Begin("World", m_IsWorldOpen))
		{
			ImGui::BeginChild("Entities", { 0, -ImGui::GetFontSize() * 1.8f });
			DisplayChildren(m_World->GetHierarchy());
			ImGui::EndChild();

			if (ImGui::Button("Add Entity"))
			{
				m_OpenAddEntity = true;
			}

			DrawAddEntity();

			ImGui::End();
		}
		else
		{
			ImGui::End();
		}
	}

	if (*m_IsPropertiesOpen)
	{
		if (ImGui::Begin("Properties", m_IsPropertiesOpen))
		{
			if (!m_CurrentlySelectedEntity)
			{
				ImGui::Text("Select an Entity to view it's properties");
			}
			else
			{
				DrawProperties();
			}

			ImGui::End();
		}
		else
		{
			ImGui::End();
		}
	}
}

void WorldLayer::DisplayChildren(const World::WorldNode& node)
{
	for (auto& child : node.children)
	{
		if (child.children.empty())
		{
			if (ImGui::Selectable(child.entity->GetName().c_str(), m_CurrentlySelectedEntity == child.entity))
			{
				SetCurrentEntity(child.entity);
				memset(m_EntityNameBuffer, 0, sizeof(m_EntityNameBuffer));
				memcpy(m_EntityNameBuffer, child.entity->GetName().c_str(), child.entity->GetName().size());
			}
			
			if (ImGui::BeginPopupContextItem(NULL, ImGuiPopupFlags_MouseButtonRight))
			{
				SetCurrentEntity(child.entity);

				if (ImGui::Selectable("Delete"))
				{
					m_World->DestroyEntity(m_CurrentlySelectedEntity);
					m_CurrentlySelectedEntity = nullptr;
				}

				ImGui::EndPopup();
			}
		}
		else if (ImGui::TreeNodeEx(child.entity->GetName().c_str(), ImGuiTreeNodeFlags_OpenOnArrow))
		{
			DisplayChildren(child);

			ImGui::TreePop();
		}
	}
}

void WorldLayer::SetCurrentEntity(Entity* entity)
{
	m_CurrentlySelectedEntity = entity;
	memset(m_EntityNameBuffer, 0, sizeof(m_EntityNameBuffer));
	memcpy(m_EntityNameBuffer, entity->GetName().c_str(), entity->GetName().size());

	m_Position[0] = m_CurrentlySelectedEntity->GetTransform().GetPosition().x;
	m_Position[1] = m_CurrentlySelectedEntity->GetTransform().GetPosition().y;
	m_Position[2] = m_CurrentlySelectedEntity->GetTransform().GetPosition().z;
}

void WorldLayer::DrawAddEntity()
{
	if (m_OpenAddEntity)
	{
		ImGui::OpenPopup("Entity Name");
	}

	if (ImGui::BeginPopup("Entity Name"))
	{
		if (m_OpenAddEntity)
		{
			ImGui::SetKeyboardFocusHere();
			m_OpenAddEntity = false;
		}
		if (ImGui::InputTextWithHint("###New Entity Name Input", "Entity Name",
			m_NewEntityNameBuffer, sizeof(m_NewEntityNameBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			if (std::string(m_NewEntityNameBuffer).empty())
			{
				VX_ERROR(LogWorld, "Cannot create entity with no name!");
				ImGui::CloseCurrentPopup();
			}
			else
			{
				m_World->CreateEntity(m_NewEntityNameBuffer);
				memset(m_NewEntityNameBuffer, 0, sizeof(m_NewEntityNameBuffer));
				ImGui::CloseCurrentPopup();
			}
		}

		ImGui::EndPopup();
	}
}

void WorldLayer::DrawProperties()
{
	if (ImGui::InputTextWithHint("###Entity Name Input", "Entity Name", 
		m_EntityNameBuffer, sizeof(m_EntityNameBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
	{
		m_CurrentlySelectedEntity->SetName(m_EntityNameBuffer);
	}

	ImGui::Separator();

	DrawTransform();
}

void WorldLayer::DrawTransform()
{
	ImGui::Text("Transform");
	if (ImGui::DragFloat3("Position", m_Position, 0.1f))
	{
		m_CurrentlySelectedEntity->GetTransform().SetPosition({ m_Position[0], m_Position[1], m_Position[2] });
	}
}
