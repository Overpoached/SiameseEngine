#pragma once

class GLFWwindow;
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

		void PollEvents() const;
		bool ShouldClose() const;

		WindowConfig m_config{};
	private:
		GLFWwindow* m_window{ nullptr };

	public:
		static Window* Create(const WindowConfig& config);
		static void InitGlfw();
		static void TerminateGlfw();
	};
}