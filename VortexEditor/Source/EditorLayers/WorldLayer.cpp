#include <EditorLayers/WorldLayer.h>
#include <EditorLayers/AssetLayer.h>
#include <im3d_math.h>

using namespace Vortex;

DEFINE_LOGGER(LogWorld);

constexpr float PI = 3.14159265f;

WorldLayer::WorldLayer(bool* isWorldOpen, bool* isPropertiesOpen)
	: m_IsWorldOpen(isWorldOpen), m_IsPropertiesOpen(isPropertiesOpen)
{
	CREATE_LOGGER(LogWorld, spdlog::level::trace);

	m_World = new World;

	m_EditorEntity = m_World->CreateNonHierarchicalEntity("Editor Entity");
	m_EditorEntity->GetTransform()->SetPosition({ 0.f, 5.f, 0.f });

	auto camera = m_EditorEntity->AddCameraComponent();
	m_World->SetMainCamera(camera);
	camera->SetNearPlane(2.f);

	m_World->AddGrid(1000.f, 10.f, 1.f, { 1.f, 1.f, 1.f, 0.15f });

	memset(m_NewEntityNameBuffer, 0, sizeof(m_NewEntityNameBuffer));
	memset(m_EntityNameBuffer, 0, sizeof(m_EntityNameBuffer));
}

WorldLayer::~WorldLayer()
{
	delete m_World;
}

void WorldLayer::OnAttach()
{
	Im3d::GetContext().m_gizmoHeightPixels = 250.f;
	GInput->AddKeyBinding(BIND_INPUT(this->FocusEntity), InputCode::F, Binding::Pressed);
	GRenderer->RenderWorld(m_World);
}

void WorldLayer::OnDetach()
{
	GInput->RemoveKeyBinding(InputCode::F, Binding::Pressed);
	GRenderer->RenderWorld(nullptr);
}

void WorldLayer::Tick(float deltaTime)
{

}

void WorldLayer::OnGuiRender()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, { 200, 200 });
	if (*m_IsWorldOpen)
	{
		if (ImGui::Begin("World", m_IsWorldOpen))
		{
			m_IsWorldActive = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);

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
				ImGui::Text("Select an Entity to view its properties");
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
	ImGui::PopStyleVar();
}

void WorldLayer::On3dUiRender()
{
	DrawGizmos();

	DrawCameraFrustrums();
}

void WorldLayer::FocusEntity()
{
	if (m_CurrentlySelectedEntity && m_IsWorldActive)
	{
		auto editorTransfom = m_EditorEntity->GetTransform();
		auto objectTransform = m_CurrentlySelectedEntity->GetTransform();

		if (m_CurrentlySelectedEntity->GetSpriteComponent())
		{
			auto sprite = m_CurrentlySelectedEntity->GetSpriteComponent();
			editorTransfom->SetPosition(objectTransform->GetPosition() + 
				Math::Vector(0.f, 0.f, -sprite->GetQuad().GetHeight() * objectTransform->GetScale().y * 1.5f) * objectTransform->GetRotationMatrix());
		}
		else
		{
			editorTransfom->SetPosition(objectTransform->GetPosition() + Math::Vector(0.f, 0.f, -5.f) * objectTransform->GetRotationMatrix());
		}

		editorTransfom->SetRotation({ objectTransform->GetRotation().x, 0.f, 0.f });
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

	m_Position[0] = transform->GetPosition().x;
	m_Position[1] = transform->GetPosition().y;
	m_Position[2] = transform->GetPosition().z;

	m_Rotation[0] = transform->GetRotation().x;
	m_Rotation[1] = transform->GetRotation().y;
	m_Rotation[2] = transform->GetRotation().z;

	m_Scale[0] = transform->GetScale().x;
	m_Scale[1] = transform->GetScale().y;
	m_Scale[2] = transform->GetScale().z;

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

		bool set = false;
		for (auto pair : GTextureAssets)
		{
			if (pair.second == sprite->GetTexture())
			{
				m_CurrentTexture = pair.first;
				set = true;
				break;
			}
		}
		if (!set)
		{
			m_CurrentTexture = "None";
		}
	}

	if (auto camera = m_CurrentlySelectedEntity->GetCameraComponent())
	{
		m_Projection = (int)camera->GetProjectionMode();
		m_AspectRatio = camera->GetAspectRatio();
		m_NearPlane = camera->GetNearPlane();
		m_FarPlane = camera->GetFarPlane();
		m_FOVMultiple = camera->GetFOVMultiple();
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
			m_World->CreateEntity(m_NewEntityNameBuffer);
			memset(m_NewEntityNameBuffer, 0, sizeof(m_NewEntityNameBuffer));
			ImGui::CloseCurrentPopup();
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
				SetCurrentEntity(m_CurrentlySelectedEntity);
			}
		}
		if (!m_CurrentlySelectedEntity->GetMeshComponent())
		{
			if (ImGui::Selectable("Mesh Component"))
			{
				m_CurrentlySelectedEntity->AddMeshComponent();
				SetCurrentEntity(m_CurrentlySelectedEntity);
			}
		}
		if (!m_CurrentlySelectedEntity->GetCameraComponent())
		{
			if (ImGui::Selectable("Camera Component"))
			{
				m_CurrentlySelectedEntity->AddCameraComponent();
				GRenderer->ResizeAllCameras();
				SetCurrentEntity(m_CurrentlySelectedEntity);
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
	if (ImGui::DragScalarN("Position", ImGuiDataType_Float, m_Position, 3, 0.05f, nullptr, nullptr, "%.3f vu"))
	{
		m_CurrentlySelectedEntity->GetTransform()->SetPosition(m_Position);
	}

	if (ImGui::DragScalarN("Rotation", ImGuiDataType_Float, m_Rotation, 3, 2.f, nullptr, nullptr, "%.3f deg"))
	{
		for (float& val : m_Rotation)
		{
			if (val > 180.f)
			{
				val -= 360.f;
			}
			else if (val < -180.f)
			{
				val += 360.f;
			}
		}

		m_CurrentlySelectedEntity->GetTransform()->SetRotation(m_Rotation);
	}

	if (ImGui::DragScalarN("Scale", ImGuiDataType_Float, m_Scale, 3, 0.05f, nullptr, nullptr, "%.3f"))
	{
		for (float& val : m_Scale)
		{
			if (val < 0.f)
			{
				val = 0.f;
			}
		}

		m_CurrentlySelectedEntity->GetTransform()->SetScale(m_Scale);
	}

	ImGui::Separator();
}

void WorldLayer::DrawSprite()
{
	if (!m_CurrentlySelectedEntity->GetSpriteComponent()) return;

	ImGui::Text("Sprite Component");

	if (ImGui::DragScalarN("Quad Size", ImGuiDataType_Float, m_QuadSize, 2, 0.1f, nullptr, nullptr, "%.3f vu"))
	{
		m_CurrentlySelectedEntity->GetSpriteComponent()->GetQuad().SetSize(m_QuadSize[0], m_QuadSize[1]);
	}

	if (ImGui::ColorEdit3("###Vertex1", m_VertexCol1, ImGuiColorEditFlags_NoInputs))
	{
		m_CurrentlySelectedEntity->GetSpriteComponent()->GetQuad().SetColors
		(Math::Matrix::Columns(
			m_VertexCol1,
			m_VertexCol2,
			m_VertexCol3,
			m_VertexCol4
		));
	}

	ImGui::SameLine();

	if (ImGui::ColorEdit3("###Vertex2", m_VertexCol2, ImGuiColorEditFlags_NoInputs))
	{
		m_CurrentlySelectedEntity->GetSpriteComponent()->GetQuad().SetColors
		(Math::Matrix::Columns(
			m_VertexCol1,
			m_VertexCol2,
			m_VertexCol3,
			m_VertexCol4
		));
	}

	if (ImGui::ColorEdit3("###Vertex3", m_VertexCol3, ImGuiColorEditFlags_NoInputs))
	{
		m_CurrentlySelectedEntity->GetSpriteComponent()->GetQuad().SetColors
		(Math::Matrix::Columns(
			m_VertexCol1,
			m_VertexCol2,
			m_VertexCol3,
			m_VertexCol4
		));
	}

	ImGui::SameLine();

	if (ImGui::ColorEdit3("###Vertex4", m_VertexCol4, ImGuiColorEditFlags_NoInputs))
	{
		m_CurrentlySelectedEntity->GetSpriteComponent()->GetQuad().SetColors
		(Math::Matrix::Columns(
			m_VertexCol1,
			m_VertexCol2,
			m_VertexCol3,
			m_VertexCol4
		));
	}

	if (ImGui::BeginCombo("Texture", m_CurrentTexture.c_str()))
	{
		if (ImGui::Selectable("None", false))
		{
			m_CurrentTexture = "None";
			m_CurrentlySelectedEntity->GetSpriteComponent()->SetTexture(nullptr);
		}

		for (auto pair : GTextureAssets)
		{
			if (ImGui::Selectable(pair.first.c_str(), false))
			{
				m_CurrentTexture = pair.first;
				m_CurrentlySelectedEntity->GetSpriteComponent()->SetTexture(pair.second);
			}
		}

		ImGui::EndCombo();
	}

	ImGui::Separator();
}

void WorldLayer::DrawCamera()
{
	if (!m_CurrentlySelectedEntity->GetCameraComponent()) return;

	ImGui::Text("Camera Component");

	std::string proj;
	if (m_Projection == (int)CameraProjection::Perspective)
	{
		proj = "Perspective";
	}
	else
	{
		proj = "Orthographic";
	}
	if (ImGui::SliderInt("Projection", &m_Projection, 0, 1, proj.c_str()))
	{
		m_CurrentlySelectedEntity->GetCameraComponent()->SetProjection((CameraProjection) m_Projection);
	}

	if (ImGui::DragFloat("Near Plane", &m_NearPlane, 0.05f))
	{
		m_CurrentlySelectedEntity->GetCameraComponent()->SetNearPlane(m_NearPlane);
	}

	if (ImGui::DragFloat("Far Plane", &m_FarPlane, 0.05f))
	{
		m_CurrentlySelectedEntity->GetCameraComponent()->SetFarPlane(m_FarPlane);
	}
	
	if (ImGui::InputScalar("FOV Multiple", ImGuiDataType_Float, &m_FOVMultiple, nullptr, nullptr, "%.3f"))
	{
		m_CurrentlySelectedEntity->GetCameraComponent()->SetFOVMultiple(m_FOVMultiple);
	}

	if (m_World->GetMainCamera() != m_CurrentlySelectedEntity->GetCameraComponent())
	{
		if (ImGui::Button("Preview###Preview Button", { ImGui::GetContentRegionAvail().x, 0.f }))
		{
			m_World->SetMainCamera(m_CurrentlySelectedEntity->GetCameraComponent());
		}
	}
	else
	{
		if (ImGui::Button("Stop Preview###Preview Button", { ImGui::GetContentRegionAvail().x, 0.f }))
		{
			m_World->SetMainCamera(m_EditorEntity->GetCameraComponent());
		}
	}

	ImGui::Separator();
}

void WorldLayer::DrawGizmos()
{
	if (m_CurrentlySelectedEntity)
	{
		if (m_World->GetMainCamera() != m_EditorEntity->GetCameraComponent()) return;

		Im3d::Mat4 mat;
		mat.setTranslation({ m_Position[0], m_Position[1], m_Position[2] });
		Im3d::Mat3 rotation = Im3d::FromEulerXYZ(Im3d::Vec3
		(
			(float)-m_Rotation[0] * PI / 180.f,
			(float)-m_Rotation[1] * PI / 180.f,
			(float)-m_Rotation[2] * PI / 180.f
		));
		mat.setRotation(rotation);
		mat.setScale({ m_Scale[0], m_Scale[1], m_Scale[2] });

		Im3d::PushMatrix(mat);
		Im3d::PushSize(3.f);
		switch (Im3d::GetContext().m_gizmoMode)
		{
		case Im3d::GizmoMode_Translation:
			if (Im3d::GizmoTranslation("Translation", m_Position))
			{
				m_CurrentlySelectedEntity->GetTransform()->SetPosition(m_Position);
			}
			break;
		case Im3d::GizmoMode_Rotation:
			if (Im3d::GizmoRotation("Rotation", m_Rotation))
			{
				m_CurrentlySelectedEntity->GetTransform()->SetRotation(m_Rotation);
			}
			break;
		case Im3d::GizmoMode_Scale:
			if (Im3d::GizmoScale("Scale", m_Scale))
			{
				m_CurrentlySelectedEntity->GetTransform()->SetScale(m_Scale);
			}
			break;
		}
		Im3d::PopSize();
		Im3d::PopMatrix();
	}
}

void WorldLayer::DrawCameraFrustrums()
{
	for (auto camera : m_World->GetCameras())
	{
		if (m_World->GetMainCamera() == camera) continue;

		if (m_CurrentlySelectedEntity && m_CurrentlySelectedEntity->GetCameraComponent() == camera)
			Im3d::PushSize(3.f);
		else
			Im3d::PushSize(1.f);
		Im3d::PushColor(Im3d::Color_Green);

		auto matrix = camera->GetTransform()->GetTransform();

		Math::Vector neara, nearb, nearc, neard;

		neara.x = -camera->GetWidth() / 2.f;
		neara.y = camera->GetHeight() / 2.f;
		nearb.x = camera->GetWidth() / 2.f;
		nearb.y = camera->GetHeight() / 2.f;
		nearc.x = camera->GetWidth() / 2.f;
		nearc.y = -camera->GetHeight() / 2.f;
		neard.x = -camera->GetWidth() / 2.f;
		neard.y = -camera->GetHeight() / 2.f;
		neara.z = nearb.z = nearc.z = neard.z = camera->GetNearPlane();

		neara *= matrix;
		nearb *= matrix;
		nearc *= matrix;
		neard *= matrix;

		Im3d::DrawQuad({ neara.x, neara.y, neara.z }, { nearb.x, nearb.y, nearb.z }, { nearc.x, nearc.y, nearc.z }, { neard.x, neard.y, neard.z });

		Math::Vector fara, farb, farc, fard;

		if (camera->GetProjectionMode() == CameraProjection::Orthographic)
		{
			fara.x = -camera->GetWidth() / 2.f;
			fara.y = camera->GetHeight() / 2.f;
			farb.x = camera->GetWidth() / 2.f;
			farb.y = camera->GetHeight() / 2.f;
			farc.x = camera->GetWidth() / 2.f;
			farc.y = -camera->GetHeight() / 2.f;
			fard.x = -camera->GetWidth() / 2.f;
			fard.y = -camera->GetHeight() / 2.f;
			fara.z = farb.z = farc.z = fard.z = camera->GetFarPlane();

			fara *= matrix;
			farb *= matrix;
			farc *= matrix;
			fard *= matrix;

			Im3d::DrawQuad({ fara.x, fara.y, fara.z }, { farb.x, farb.y, farb.z }, { farc.x, farc.y, farc.z }, { fard.x, fard.y, fard.z });
		}
		else
		{
			fara = neara - camera->GetTransform()->GetPosition();
			fara *= camera->GetFarPlane();
			farb = nearb - camera->GetTransform()->GetPosition();
			farb *= camera->GetFarPlane();
			farc = nearc - camera->GetTransform()->GetPosition();
			farc *= camera->GetFarPlane();
			fard = neard - camera->GetTransform()->GetPosition();
			fard *= camera->GetFarPlane();

			Im3d::DrawQuad({ fara.x, fara.y, fara.z }, { farb.x, farb.y, farb.z }, { farc.x, farc.y, farc.z }, { fard.x, fard.y, fard.z });
		}

		Im3d::BeginLines();
		Im3d::Vertex(neara.x, neara.y, neara.z);
		Im3d::Vertex(fara.x, fara.y, fara.z);

		Im3d::Vertex(nearb.x, nearb.y, nearb.z);
		Im3d::Vertex(farb.x, farb.y, farb.z);

		Im3d::Vertex(nearc.x, nearc.y, nearc.z);
		Im3d::Vertex(farc.x, farc.y, farc.z);

		Im3d::Vertex(neard.x, neard.y, neard.z);
		Im3d::Vertex(fard.x, fard.y, fard.z);
		Im3d::End();


		Im3d::PopColor();
		Im3d::PopSize();
	}
}
