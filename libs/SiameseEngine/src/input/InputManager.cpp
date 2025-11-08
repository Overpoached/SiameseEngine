#include "sepch.h"

#include "input/InputManager.h"

#include "core/Clock.h"

void sengine::InputManager::Update(double currentTime)
{
    for (auto& [key, info] : keys)
    {
        switch (info.state)
        {
        case KeyState::Pressed:
        {
            if (!info.isHeld)
            {
                //set state to held in the next frame instead
                info.isHeld = true;
                //signal pressed 
            }
            else
            {
                info.lastPressTime = currentTime;
                info.state = KeyState::Held;
                //signal held
            }
            break;
        }
        case KeyState::Held:
        {
            //signal held 
            if ((currentTime - info.lastPressTime) > repeatDelay)
            {
                // first repeat trigger 
                info.lastPressTime = currentTime;
                info.state = KeyState::Repeat;
            }
            break;
        }
        case KeyState::Repeat:
        {
            //signal repeat, immediately transition back to repeating
            info.state = KeyState::Repeating;
            info.lastPressTime = currentTime;
            break;
        }
        case KeyState::Repeating:
        {
            //signal held 
            if ((currentTime - info.lastPressTime) > repeatRate)
            {
                info.lastPressTime = currentTime;
                info.state = KeyState::Repeat;
            }
            break;
        }
        case KeyState::Released:
        {
            info.state = KeyState::Idle;
            info.lastPressTime = 0;
            info.isHeld = false;
            //signal release 
            break;
        }
        default:
            break;
        }
    }
}

void sengine::InputManager::ProcessKey(KeyCode key, KeyState action, double currentTime)
{
    auto& info = keys[key];
    if (action == KeyState::Pressed)
    {
        info.state = KeyState::Pressed;
        info.lastPressTime = currentTime;
        info.isHeld = false;
    }
    else if (action == KeyState::Released)
    {
        info.state = KeyState::Released;
        info.isHeld = false;
    }
}

void sengine::InputManager::ProcessMouseButton(MouseButton button, KeyState action, double currentTime)
{
    auto& info = mouseButtons[button];
    if (action == KeyState::Pressed)
    {
        info.state = KeyState::Pressed;
        info.lastPressTime = currentTime;
    }
    else if (action == KeyState::Released)
    {
        info.state = KeyState::Released;
    }
}

void sengine::InputManager::ProcessChar(unsigned int codepoint)
{
    textInput.push_back(static_cast<char32_t>(codepoint));
}

void sengine::InputManager::ProcessMousePosition(double x, double y)
{
    mouseX = x;
    mouseY = y;
}

void sengine::InputManager::ProcessScroll(double xoffset, double yoffset)
{
    scrollX = xoffset;
    scrollY = yoffset;
}

bool sengine::InputManager::IsKeyDown(KeyCode key) const
{
    auto it = keys.find(key);
    return it != keys.end() && (it->second.state != KeyState::Idle && it->second.state != KeyState::Released);
}

bool sengine::InputManager::IsKeyRepeat(KeyCode key) const
{
    auto it = keys.find(key);
    return it != keys.end() && it->second.state == KeyState::Repeat;
}

bool sengine::InputManager::IsKeyPressed(KeyCode key) const
{
    auto it = keys.find(key);
    return it != keys.end() && it->second.state == KeyState::Pressed;
}

bool sengine::InputManager::IsKeyReleased(KeyCode key) const
{
    auto it = keys.find(key);
    return it != keys.end() && it->second.state == KeyState::Released;
}

bool sengine::InputManager::IsMouseButtonDown(MouseButton button) const
{
    auto it = mouseButtons.find(button);
    return it != mouseButtons.end() && (it->second.state == KeyState::Pressed || it->second.state == KeyState::Held);
}

bool sengine::InputManager::IsMouseButtonPressed(MouseButton button) const
{
    auto it = mouseButtons.find(button);
    return it != mouseButtons.end() && it->second.state == KeyState::Pressed;
}

bool sengine::InputManager::IsMouseButtonReleased(MouseButton button) const
{
    auto it = mouseButtons.find(button);
    return it != mouseButtons.end() && it->second.state == KeyState::Released;
}

void sengine::InputManager::ClearTextInput()
{
    textInput.clear();
}
