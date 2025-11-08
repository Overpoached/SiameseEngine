#pragma once

#include "input/InputCodes.h"

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

        double GetMouseX() const { return mouseX; }
        double GetMouseY() const { return mouseY; }
        double GetScrollX() const { return scrollX; }
        double GetScrollY() const { return scrollY; }

        void ClearTextInput();
        const std::u32string& GetTextInput() const { return textInput; }

        void SetRepeatDelay(double delay) { repeatDelay = delay; }
        void SetRepeatRate(double rate) { repeatRate = rate; }

    private:

        std::unordered_map<KeyCode, KeyInfo> keys;
        std::unordered_map<MouseButton, KeyInfo> mouseButtons;

        std::u32string textInput;
        double mouseX = 0.0, mouseY = 0.0;
        double scrollX = 0.0, scrollY = 0.0;

        double repeatDelay = 0.5; // seconds before repeat
        double repeatRate = 0.05; // seconds between repeats
    };
}