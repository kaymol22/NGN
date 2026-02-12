#include "ImGuiLayer.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "Core/Application.h"
#include "Core/Window.h"
#include "Core/Profile.h"

namespace NGN
{

	void ImGuiLayer::OnAttach()
	{
		// TODO: Profile here
		/*NGN_PROFILE_FUNCTION();*/

		// Set up Dear ImGui Context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		// Config flags 
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();

		Application& app = Application::Get();
		GLFWwindow* window = app.GetWindow().GetHandle();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 460");
	}

	void ImGuiLayer::OnDetach()
	{
		// TODO: Profile here
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		if (m_BlockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}
	// Start of Frame
	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}
	// End of Frame 
	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();

		io.DisplaySize = ImVec2((float)app.GetWindow().GetFramebufferSize().x,
								(float)app.GetWindow().GetFramebufferSize().y);

		// Render ImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Multi viewport handling
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* curr_context_backup = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(curr_context_backup);
		}
	}

	void ImGuiLayer::SetDarkThemeColors()
	{
		auto& colors = ImGui::GetStyle().Colors;

		// Backgrounds
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.08f, 0.08f, 0.10f, 1.0f };
		colors[ImGuiCol_ChildBg] = ImVec4{ 0.09f, 0.09f, 0.12f, 1.0f };
		colors[ImGuiCol_PopupBg] = ImVec4{ 0.08f, 0.08f, 0.10f, 0.95f };

		// Headers / Panels
		colors[ImGuiCol_Header] = ImVec4{ 0.18f, 0.18f, 0.22f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.25f, 0.25f, 0.35f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.20f, 0.20f, 0.28f, 1.0f };

		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.20f, 0.22f, 0.28f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.30f, 0.32f, 0.40f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.25f, 0.27f, 0.35f, 1.0f };

		// Frame/Inputs
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.18f, 0.18f, 0.20f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.25f, 0.25f, 0.28f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.22f, 0.22f, 0.25f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.15f, 0.18f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.35f, 0.37f, 0.45f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.30f, 0.38f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.12f, 0.12f, 0.15f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.20f, 0.22f, 0.28f, 1.0f };

		// Titles
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.10f, 0.10f, 0.12f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.18f, 0.20f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.08f, 0.08f, 0.10f, 0.95f };

		// Optional accents (links, highlights)
		colors[ImGuiCol_CheckMark] = ImVec4{ 0.40f, 0.60f, 0.85f, 1.0f };
		colors[ImGuiCol_SliderGrab] = ImVec4{ 0.35f, 0.55f, 0.85f, 1.0f };
		colors[ImGuiCol_SliderGrabActive] = ImVec4{ 0.45f, 0.65f, 0.95f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.45f, 0.55f, 0.70f, 1.0f };
	}

	uint32_t ImGuiLayer::GetActiveWidgetID() const
	{
		return ImGui::GetActiveID();
	}
}