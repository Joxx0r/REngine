#pragma once

struct RevInputState
{
    int m_x;
    int m_y;
    int m_startX;
    int m_startY;
    
    bool m_leftButton;
    bool m_middleButton;
    bool m_rightButton;
    bool m_ctrl;
    bool m_shift;
    bool m_alt;
    bool m_left;
    bool m_right;
    bool m_up;
    bool m_down;
    bool m_forward;
    bool m_back;
};


struct RevWindowData
{
public:
    RevWindowData(){ }
    RevWindowData(UINT width, UINT height, std::wstring title):m_width(width), m_height(height), m_title(title){ }
    UINT m_width;
    UINT m_height;
    std::wstring m_title;

    float GetAspectRatio() const
    {
        return static_cast<float>(m_width) / static_cast<float>(m_height);
    }
    
};
struct RevEngineInitializationData
{
    static RevEngineInitializationData Construct(UINT width, UINT height, std::wstring title, bool rasterDefault)
    {
        assert(width > 0 && height > 0);
        RevEngineInitializationData returnData = {};
        returnData.m_windowData = RevWindowData(width, height, title);
        returnData.m_rasterDefault = rasterDefault;
        return returnData;
    }

    float GetAspectRatio() const
    {
        return m_windowData.GetAspectRatio();
    }
    
    RevWindowData m_windowData;
    bool m_rasterDefault;
};
