#ifndef INPUTTEXT_H_DEFINED
#define INPUTTEXT_H_DEFINED

#include "pch.h"

class InputText
{
public:
    InputText(cpu_font& _font) : m_font(&_font) {}
    InputText(cpu_font& _font, XMINT2 _pos, int _anchor) : m_font(&_font), m_pos(_pos), m_anchor(_anchor) {}

    void HandleInput();
    void Render();
    void Reset();
    
    bool IsActive() const { return m_isActive;  }
    void SetActive(bool _state) { m_isActive = _state; }
    bool IsFinished() const { return m_finished;}
    
    const std::string& GetText() const { return m_text; }
    void SetText(const std::string& _text) { m_text = _text; }

    void SetPos(XMINT2 _pos) { m_pos = _pos; }
    XMINT2 GetPos() const { return m_pos; }

private:
    int m_anchor = 0;
    bool m_isActive = false;
    XMINT2 m_pos = {0, 0};
    cpu_font* m_font;
    std::string m_text;
    bool m_finished = false;
};

#endif
