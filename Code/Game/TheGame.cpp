#include "Game/TheGame.hpp"
#include "Engine/Input/Console.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/3D/ForwardRenderer.hpp"
#include "Engine/Renderer/3D/Camera3D.hpp"

TheGame* TheGame::instance = nullptr;

TheGame::TheGame()
{
    InitializeRand();
    InitializeMainCamera();
}

//-----------------------------------------------------------------------------------
TheGame::~TheGame()
{

}

//-----------------------------------------------------------------------------------
void TheGame::InitializeMainCamera()
{
    Camera3D* camera = ForwardRenderer::instance->GetMainCamera();
    camera->m_updateFromInput = false;
    camera->m_position = Vector3(30.0f, 30.0f, -10.0f);
    camera->LookAt(Vector3::ZERO);
}

//-----------------------------------------------------------------------------------
void TheGame::Update(float deltaSeconds)
{
    if (!Console::instance->IsActive() && InputSystem::instance->WasKeyJustPressed(InputSystem::ExtraKeys::TILDE))
    {
        Console::instance->ActivateConsole();
    }
    if (Console::instance->IsActive())
    {
        return; //Don't do anything involving input updates.
    }
    else if (InputSystem::instance->WasKeyJustPressed(InputSystem::ExtraKeys::ESC))
    {
        g_isQuitting = true;
        return;
    }
    
    ForwardRenderer::instance->Update(deltaSeconds);
    ForwardRenderer::instance->GetMainCamera()->LookAt(Vector3::ZERO);
}

//-----------------------------------------------------------------------------------
void TheGame::Render() const
{
    ENSURE_NO_MATRIX_STACK_SIDE_EFFECTS(Renderer::instance->m_viewStack);
    ENSURE_NO_MATRIX_STACK_SIDE_EFFECTS(Renderer::instance->m_projStack);
    Begin3DPerspective();
    {
        Renderer::instance->ClearColor(RGBA::LIME);
        ForwardRenderer::instance->Render();
        RenderAxisLines();
    }
    End3DPerspective();
}

//-----------------------------------------------------------------------------------
void TheGame::Begin3DPerspective() const
{
    const float aspect = 16.f / 9.f;
    const float nearDist = 0.1f;
    const float farDist = 1000.0f;
    const float fovY = 50.0f;
    Renderer::instance->BeginPerspective(fovY, aspect, nearDist, farDist);	
    Renderer::instance->PushView(ForwardRenderer::instance->GetMainCamera()->GetViewMatrix());
}

//-----------------------------------------------------------------------------------
void TheGame::End3DPerspective() const
{
    //Reset Perspective
    Renderer::instance->EndPerspective();
    Renderer::instance->PopView();
}

//-----------------------------------------------------------------------------------
void TheGame::RenderAxisLines() const
{
    if (!m_renderAxisLines)
    {
        return;
    }
    const float axisLineLength = 100.0f;
    Renderer::instance->EnableDepthTest(true);

    Renderer::instance->DrawLine(Vector3(0.0f, 0.0f, 0.0f), Vector3(axisLineLength, 0.0f, 0.0f), RGBA::RED, 3.0f);
    Renderer::instance->DrawLine(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, axisLineLength, 0.0f), RGBA::GREEN, 3.0f);
    Renderer::instance->DrawLine(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, axisLineLength), RGBA::BLUE, 3.0f);

    Renderer::instance->EnableDepthTest(false);

    Renderer::instance->DrawLine(Vector3(0.0f, 0.0f, 0.0f), Vector3(axisLineLength, 0.0f, 0.0f), RGBA::RED, 1.0f);
    Renderer::instance->DrawLine(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, axisLineLength, 0.0f), RGBA::GREEN, 1.0f);
    Renderer::instance->DrawLine(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, axisLineLength), RGBA::BLUE, 1.0f);

    Renderer::instance->EnableDepthTest(true);
}
