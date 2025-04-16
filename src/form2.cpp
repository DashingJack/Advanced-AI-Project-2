#include "form2.hpp"
#include "input_handler.hpp"
#include <imgui.h>
#include <unordered_map>

class Form2Impl {
public:
    char resourceName[128] = "";
    int resourceCount = 0;
    std::unordered_map<int, Resource> temp_resources;
};

bool Form2::Render() {
    static Form2Impl state;
    using namespace inputs;

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    ImGui::Begin("Form 2: Resource Entry", nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

    ImGui::TextColored(ImVec4(0.6f, 0.7f, 1.0f, 1.0f), "Step 2 of 3: Add Required Resources");
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::TextWrapped("List all resources required. You can add multiple items before proceeding.");
    ImGui::Spacing();

    float inputWidth = 300.0f;

    ImGui::Text("Resource Name");
    ImGui::SetNextItemWidth(inputWidth);
    ImGui::InputText("##resourceName", state.resourceName, IM_ARRAYSIZE(state.resourceName));
    ImGui::Spacing();

    ImGui::Text("Resource Count");
    ImGui::SetNextItemWidth(inputWidth);
    ImGui::InputInt("##resourceCount", &state.resourceCount);
    ImGui::Spacing();

    float buttonWidth = 160.0f;
    float centerX = (ImGui::GetContentRegionAvail().x - buttonWidth) / 2.0f;
    ImGui::SetCursorPosX(centerX);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.87f, 0.56f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.93f, 0.67f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.95f, 0.80f, 0.40f, 1.0f));

    if (ImGui::Button("Add Resource", ImVec2(buttonWidth, 0))) {
        if (strlen(state.resourceName) > 0 && state.resourceCount > 0) {
            int newId = state.temp_resources.size();
            state.temp_resources[newId] = Resource{newId, state.resourceCount, state.resourceName};
            state.resourceName[0] = '\0';
            state.resourceCount = 0;
        }
    }

    ImGui::PopStyleColor(3);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text("Added Resources:");
    ImGui::BeginChild("ResourceList", ImVec2(0, 220), true);

    for (auto it = state.temp_resources.begin(); it != state.temp_resources.end();) {
        ImGui::BulletText("%s (x%d)", it->second.name_.c_str(), it->second.count_);
        ImGui::SameLine();

        std::string delButton = "Delete##" + std::to_string(it->first);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.80f, 0.20f, 0.20f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.90f, 0.30f, 0.30f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.70f, 0.10f, 0.10f, 1.0f));

        if (ImGui::Button(delButton.c_str())) {
            it = state.temp_resources.erase(it);
        } else {
            ++it;
        }

        ImGui::PopStyleColor(3);
    }

    ImGui::EndChild();
    ImGui::Spacing();
    ImGui::Separator();

    bool next = false;
    float nextWidth = 120.0f;
    centerX = (ImGui::GetContentRegionAvail().x - nextWidth) / 2.0f;
    ImGui::SetCursorPosX(centerX);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.45f, 0.95f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.55f, 1.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15f, 0.35f, 0.85f, 1.0f));

    if (ImGui::Button("Next", ImVec2(nextWidth, 0))) {
        inputs::resource_map = state.temp_resources;
        next = true;
    }

    ImGui::PopStyleColor(3);
    ImGui::End();

    return next;
}
