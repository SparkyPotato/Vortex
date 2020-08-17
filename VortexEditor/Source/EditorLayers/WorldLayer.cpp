#include <EditorLayers/WorldLayer.h>

using namespace Vortex;

DEFINE_LOGGER(LogWorld);

WorldLayer::WorldLayer(bool* isWorldOpen, bool* isPropertiesOpen)
	: m_IsWorldOpen(isWorldOpen), m_IsPropertiesOpen(isPropertiesOpen)
{
	CREATE_LOGGER(LogWorld, spdlog::level::trace);

	m_World = new World;
	auto test = m_World->CreateEntity("Test Entity");
	m_World->SetMainCamera(test->AddCameraComponent());
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

				DrawAddComponent();
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
			if (ImGui::Selectable((child.entity->GetName() + "##" + std::to_string(child.entity->GetID())).c_str(), m_CurrentlySelectedEntity == child.entity))
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

	auto transform = m_CurrentlySelectedEntity->GetTransform();

	m_Position[0] = transform.GetPosition().x;
	m_Position[1] = transform.GetPosition().y;
	m_Position[2] = transform.GetPosition().z;

	m_Rotation[0] = transform.GetRotation().x;
	m_Rotation[1] = transform.GetRotation().y;
	m_Rotation[2] = transform.GetRotation().z;

	m_Scale[0] = transform.GetScale().x;
	m_Scale[1] = transform.GetScale().y;
	m_Scale[2] = transform.GetScale().z;

	if (auto sprite = m_CurrentlySelectedEntity->GetSpriteComponent())
	{
		m_QuadSize[0] = sprite->GetQuad().GetHeight();
		m_QuadSize[1] = sprite->GetQuad().GetWidth();

		auto colors = sprite->GetQuad().GetColors();

		m_VertexCol1[0] = colors.columns[0].x;
		m_VertexCol1[1] = colors.columns[0].y;
		m_VertexCol1[2] = colors.columns[0].z;

		m_VertexCol2[0] = colors.columns[1].x;
		m_VertexCol2[1] = colors.columns[1].y;
		m_VertexCol2[2] = colors.columns[1].z;

		m_VertexCol3[0] = colors.columns[2].x;
		m_VertexCol3[1] = colors.columns[2].y;
		m_VertexCol3[2] = colors.columns[2].z;

		m_VertexCol4[0] = colors.columns[3].x;
		m_VertexCol4[1] = colors.columns[3].y;
		m_VertexCol4[2] = colors.columns[3].z;
	}

	if (auto camera = m_CurrentlySelectedEntity->GetCameraComponent())
	{
		m_Projection = (int) camera->GetProjectionMode();
		m_AspectRatio = camera->GetAspectRatio();
		m_NearPlane = camera->GetNearPlane();
		m_FarPlane = camera->GetFarPlane();
	}
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

void WorldLayer::DrawAddComponent()
{
	if (m_OpenAddComponent)
	{
		ImGui::OpenPopup("Choose Component");
		m_OpenAddComponent = false;
	}

	if (ImGui::BeginPopup("Choose Component"))
	{
		if (!m_CurrentlySelectedEntity->GetSpriteComponent())
		{
			if (ImGui::Selectable("Sprite Component"))
			{
				m_CurrentlySelectedEntity->AddSpriteComponent();
			}
		}
		if (!m_CurrentlySelectedEntity->GetMeshComponent())
		{
			if (ImGui::Selectable("Mesh Component"))
			{
				m_CurrentlySelectedEntity->AddMeshComponent();
			}
		}
		if (!m_CurrentlySelectedEntity->GetCameraComponent())
		{
			if (ImGui::Selectable("Camera Component"))
			{
				m_CurrentlySelectedEntity->AddCameraComponent();
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

	DrawSprite();

	DrawCamera();

	if (ImGui::Button("Add Component", { ImGui::GetContentRegionAvail().x, 0 }))
	{
		m_OpenAddComponent = true;
	}
}

void WorldLayer::DrawTransform()
{
	ImGui::Text("Transform");
	if (ImGui::DragFloat3("Position", m_Position, 0.05f))
	{
		m_CurrentlySelectedEntity->GetTransform().SetPosition({ m_Position[0], m_Position[1], m_Position[2] });
	}
	if (ImGui::DragFloat3("Rotation", m_Rotation, 2.f))
	{
		for (float& val : m_Rotation)
		{
			if (val > 360.f)
			{
				val = 0.f;
			}
			else if (val < 0.f)
			{
				val += 360.f;
			}
		}

		m_CurrentlySelectedEntity->GetTransform().SetRotation({ m_Rotation[0], m_Rotation[1], m_Rotation[2] });
	}
	if (ImGui::DragFloat3("Scale", m_Scale, 0.05f))
	{
		for (float& val : m_Scale)
		{
			if (val < 0.f)
			{
				val = 0.f;
			}
		}

		m_CurrentlySelectedEntity->GetTransform().SetScale({ m_Scale[0], m_Scale[1], m_Scale[2] });
	}

	ImGui::Separator();
}

void WorldLayer::DrawSprite()
{
	if (!m_CurrentlySelectedEntity->GetSpriteComponent()) return;

	ImGui::Text("Sprite Component");

	if (ImGui::DragFloat2("Quad Size", m_QuadSize, 0.1f))
	{
		m_CurrentlySelectedEntity->GetSpriteComponent()->GetQuad().SetSize(m_QuadSize[0], m_QuadSize[1]);
	}

	if (ImGui::ColorEdit3("1###Vertex1", m_VertexCol1))
	{
		m_CurrentlySelectedEntity->GetSpriteComponent()->GetQuad().SetColors
		({
			{ m_VertexCol1[0], m_VertexCol2[0], m_VertexCol3[0], m_VertexCol4[0] },
			{ m_VertexCol1[1], m_VertexCol2[1], m_VertexCol3[1], m_VertexCol4[1] },
			{ m_VertexCol1[2], m_VertexCol2[2], m_VertexCol3[2], m_VertexCol4[2] },
			{ 1.f,             1.f,             1.f,             1.f,            }
		});
	}

	if (ImGui::ColorEdit3("2###Vertex2", m_VertexCol2))
	{
		m_CurrentlySelectedEntity->GetSpriteComponent()->GetQuad().SetColors
		({
			{ m_VertexCol1[0], m_VertexCol2[0], m_VertexCol3[0], m_VertexCol4[0] },
			{ m_VertexCol1[1], m_VertexCol2[1], m_VertexCol3[1], m_VertexCol4[1] },
			{ m_VertexCol1[2], m_VertexCol2[2], m_VertexCol3[2], m_VertexCol4[2] },
			{ 1.f,             1.f,             1.f,             1.f,            }
		});
	}

	if (ImGui::ColorEdit3("3###Vertex3", m_VertexCol3))
	{
		m_CurrentlySelectedEntity->GetSpriteComponent()->GetQuad().SetColors
		({
			{ m_VertexCol1[0], m_VertexCol2[0], m_VertexCol3[0], m_VertexCol4[0] },
			{ m_VertexCol1[1], m_VertexCol2[1], m_VertexCol3[1], m_VertexCol4[1] },
			{ m_VertexCol1[2], m_VertexCol2[2], m_VertexCol3[2], m_VertexCol4[2] },
			{ 1.f,             1.f,             1.f,             1.f,            }
		});
	}

	if (ImGui::ColorEdit3("4###Vertex4", m_VertexCol4))
	{
		m_CurrentlySelectedEntity->GetSpriteComponent()->GetQuad().SetColors
		({
			{ m_VertexCol1[0], m_VertexCol2[0], m_VertexCol3[0], m_VertexCol4[0] },
			{ m_VertexCol1[1], m_VertexCol2[1], m_VertexCol3[1], m_VertexCol4[1] },
			{ m_VertexCol1[2], m_VertexCol2[2], m_VertexCol3[2], m_VertexCol4[2] },
			{ 1.f,             1.f,             1.f,             1.f,            }
		});
	}

	ImGui::Separator();
}

void WorldLayer::DrawCamera()
{
	if (!m_CurrentlySelectedEntity->GetCameraComponent()) return;

	ImGui::Text("Camera Component");

	std::string proj;
	if (m_Projection == (int) CameraProjection::Perspective)
	{
		proj = "Perspective";
	}
	else
	{
		proj = "Orthographic";
	}
	if (ImGui::SliderInt("Projection", &m_Projection, 0, 1, proj.c_str()))
	{
		m_CurrentlySelectedEntity->GetCameraComponent()->SetCameraSettings((CameraProjection) m_Projection, m_AspectRatio, m_NearPlane, m_FarPlane);
	}

	if (ImGui::DragFloat("Near Plane", &m_NearPlane, 0.05f))
	{
		m_CurrentlySelectedEntity->GetCameraComponent()->SetCameraSettings((CameraProjection)m_Projection, m_AspectRatio, m_NearPlane, m_FarPlane);
	}

	if (ImGui::DragFloat("Far Plane", &m_FarPlane, 0.05f))
	{
		m_CurrentlySelectedEntity->GetCameraComponent()->SetCameraSettings((CameraProjection)m_Projection, m_AspectRatio, m_NearPlane, m_FarPlane);
	}

	if (ImGui::Button("Set Main Camera", { ImGui::GetContentRegionAvail().x, 0.f }))
	{
		m_World->SetMainCamera(m_CurrentlySelectedEntity->GetCameraComponent());
	}

	ImGui::Separator();
}
