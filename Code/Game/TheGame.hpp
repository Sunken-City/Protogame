#pragma once

class TheGame
{
public:
    //CONSTRUCTORS/////////////////////////////////////////////////////////////////////
    TheGame();
    ~TheGame();

    //FUNCTIONS/////////////////////////////////////////////////////////////////////
    void InitializeMainCamera();
    void Update(float deltaTime);
    void Render() const;
    void Begin3DPerspective() const;
    void End3DPerspective() const;
    void RenderAxisLines() const;

    //STATIC VARIABLES/////////////////////////////////////////////////////////////////////
    static TheGame* instance;

    //MEMBER VARIABLES/////////////////////////////////////////////////////////////////////
    bool m_renderAxisLines = true;
};
