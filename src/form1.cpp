#include "form1.hpp"
#include "input_handler.hpp"
#include <imgui.h>

class Form1Impl {
public:
    int temp_cooks = 0;
    int temp_veg = 0;
    int temp_nonveg = 0;
    int temp_jain = 0;

    void ApplyModernTheme() {
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* colors = style.Colors;

        colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.18f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.2f, 0.3f, 0.5f, 0.65f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.3f, 0.4f, 0.6f, 0.8f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.35f, 0.45f, 0.7f, 1.0f);
        colors[ImGuiCol_Button] = ImVec4(0.2f, 0.45f, 0.95f, 1.0f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.3f, 0.55f, 1.0f, 1.0f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.35f, 0.85f, 1.0f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.18f, 0.19f, 0.23f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.2f, 0.25f, 0.3f, 1.0f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.25f, 0.3f, 0.35f, 1.0f);

        style.WindowRounding = 6.0f;
        style.FrameRounding = 5.0f;
        style.ScrollbarRounding = 6.0f;
        style.GrabRounding = 5.0f;
        style.ItemSpacing = ImVec2(10, 10);
    }
};

bool Form1::Render() {
    static Form1Impl state;
    using namespace inputs;

    state.ApplyModernTheme();
    ImGuiIO& io = ImGui::GetIO();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(io.DisplaySize);
    ImGui::Begin("Form 1: Student Meal Details", nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

    ImGui::TextColored(ImVec4(0.6f, 0.7f, 1.0f, 1.0f), "Step 1 of 3: Meal Info");
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::TextWrapped("Please provide the meal preparation details for today's student meals.");
    ImGui::Spacing();

    float inputWidth = 200.0f;

    ImGui::Text("Number of Cooks");
    ImGui::SetNextItemWidth(inputWidth);
    ImGui::InputInt("##Cooks", &state.temp_cooks);
    ImGui::Spacing();

    ImGui::Text("Students (Veg)");
    ImGui::SetNextItemWidth(inputWidth);
    ImGui::InputInt("##Veg", &state.temp_veg);
    ImGui::Spacing();

    ImGui::Text("Students (Non-Veg)");
    ImGui::SetNextItemWidth(inputWidth);
    ImGui::InputInt("##NonVeg", &state.temp_nonveg);
    ImGui::Spacing();

    ImGui::Text("Students (Jain)");
    ImGui::SetNextItemWidth(inputWidth);
    ImGui::InputInt("##Jain", &state.temp_jain);
    ImGui::Spacing();

    ImGui::Separator();
    ImGui::Spacing();

    bool next = false;
    float buttonWidth = 120.0f;
    float centerX = (ImGui::GetContentRegionAvail().x - buttonWidth) / 2.0f;
    ImGui::SetCursorPosX(centerX);

    if (ImGui::Button("Next", ImVec2(buttonWidth, 0))) {
        inputs::cooks = state.temp_cooks;
        inputs::studnets_veg = state.temp_veg;
        inputs::students_non_veg = state.temp_nonveg;
        inputs::studnets_jain = state.temp_jain;
        next = true;
    }

    ImGui::End();
    return next;
}
