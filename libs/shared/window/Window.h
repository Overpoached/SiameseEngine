#pragma once

class GLFWwindow;
namespace sengine {
	class InputManager;	
	class Clock;	
}
namespace sshared
{
	struct WindowConfig
	{
		std::string title{ "Window" };
		int width{ 1280 }, height{ 720 };
	};

	class Window
	{
	public:
		~Window();

		void SetUpGlfwInputCallbacks(std::shared_ptr<sengine::InputManager> inputManagerPtr, std::shared_ptr<sengine::Clock>clockPtr);
		bool ShouldClose() const;

		void PollEvents() const;

		WindowConfig m_config{};
	private:
		GLFWwindow* m_window{ nullptr };

	public:
		static Window* Create(const WindowConfig& config);
		static void InitGlfw();
		static void TerminateGlfw();
	};
}