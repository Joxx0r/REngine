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
    static RevWindowData Construct(UINT width, UINT height, std::wstring title)
    {
        assert(width > 0 && height > 0);
        RevWindowData returnData = {};
        returnData.m_width = width;
        returnData.m_height = height;
        returnData.m_title = title;
        return returnData;
    }

    float GetAspectRatio() const
    {
        return static_cast<float>(m_width) / static_cast<float>(m_height);
    }
    
    UINT m_width;
    UINT m_height;
    std::wstring m_title;
};
