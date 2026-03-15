#pragma once

#include "input/InputCodes.h"
#include "memory/SiameseAllocator.h"

namespace sengine
{
    struct KeyInfo
    {
        KeyState state = KeyState::Idle;
        double lastPressTime = 0.0;
        bool isHeld = false;
    };

    class InputManager
    {
    public:
        // Call once per frame
        void Update(double currentTime);

        // Platform input forwarding
        void ProcessKey(KeyCode key, KeyState action, double currentTime);
        void ProcessMouseButton(MouseButton button, KeyState action, double currentTime);
        void ProcessChar(unsigned int codepoint);
        void ProcessMousePosition(double x, double y);
        void ProcessScroll(double xoffset, double yoffset);

        // Query API
        bool IsKeyDown(KeyCode key) const;
        bool IsKeyRepeat(KeyCode key) const;
        bool IsKeyPressed(KeyCode key) const;
        bool IsKeyReleased(KeyCode key) const;

        bool IsMouseButtonDown(MouseButton button) const;
        bool IsMouseButtonPressed(MouseButton button) const;
        bool IsMouseButtonReleased(MouseButton button) const;

        double GetMouseX() const { return m_mouseX; }
        double GetMouseY() const { return m_mouseY; }
        double GetScrollX() const { return m_scrollX; }
        double GetScrollY() const { return m_scrollY; }

        void ClearTextInput();
        const SString<Systems::Input>& GetTextInput() const { return m_textInput; }

        void SetRepeatDelay(double delay) { m_repeatDelay = delay; }
        void SetRepeatRate(double rate) { m_repeatRate = rate; }

    private:
		SUnorderedMap<KeyCode, KeyInfo, Systems::Input> m_keys;
        SUnorderedMap<MouseButton, KeyInfo, Systems::Input> m_mouseButtons;

        SString<Systems::Input> m_textInput;
        double m_mouseX = 0.0, m_mouseY = 0.0;
        double m_scrollX = 0.0, m_scrollY = 0.0;

        double m_repeatDelay = 0.5; // seconds before repeat
        double m_repeatRate = 0.05; // seconds between repeats
    };
}