// form_save.cpp
#include "form_save.hpp"
#include "imgui.h"
#include "input_handler.hpp"  // To access inputs::cooks, dish_map, etc.
#include <fstream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#include <commdlg.h>
#elif defined(__linux__)
#include <cstdio>   // For popen, pclose
#endif

bool FormSave::SaveToFile(const std::string &filename) {
    std::ofstream ofs(filename);
    if (!ofs.is_open())
        return false;

    // --- Write cooks ---
    ofs << "# number of cooks\n";
    ofs << "M " << inputs::cooks << " # Increased cooks slightly for more students/dishes\n\n";

    // --- Write students ---
    ofs << "# number of students (veg) (non-veg) (jain)\n";
    ofs << "S " << inputs::studnets_veg << " " << inputs::students_non_veg << " " << inputs::studnets_jain << " # Increased student numbers\n\n";

    // --- Write resources ---
    ofs << "# resources and their quantities\n";
    ofs << "# resource-id resource-name resource-count\n";
    ofs << "C " << inputs::resource_map.size() << "\n";
    for (const auto &r : inputs::resource_map) {
        ofs << r.first << " " << r.second.name_ << " " << r.second.count_ << "\n";
    }
    ofs << "\n";

    // --- Write process details ---
    ofs << "# process details\n";
    ofs << "# Process-id Process-name Time Batch-Size num-resource Resources Required\n";
    ofs << "# Based on Figure 1 from ProjectTopics.pdf & common sense\n";
    ofs << "C " << inputs::process_map.size() << "\n";
    for (const auto &p : inputs::process_map) {
        const auto &proc = p.second;
        ofs << p.first << " " << proc.name_ << " " << proc.time_ << " " << proc.batch_size_ << " ";
        // Write number of resources for the process
        ofs << proc.resources_.size();
        // Write each resource id
        for (int rid : proc.resources_) {
            ofs << " " << rid;
        }
        ofs << "\n";
    }
    ofs << "\n";

    // --- Write dishes and their allocation ---
    ofs << "# food items and their allocation\n";
    ofs << "# dish_id dish_name for-veg for-non-veg for-jain node-count\n";
    ofs << "#   dish-process-id process-id\n";
    ofs << "# dish graph\n";
    ofs << "#   count start-node end-node ...\n";
    ofs << "C " << inputs::dish_map.size() << "\n";

    for (const auto &d : inputs::dish_map) {
        const auto &dish = d.second;
        // For this example, we assume that the Dish object contains:
        // id_, name_, student_count_, and a graph_ map containing DishNode objects.
        // Since the example input file has separate counts for veg, non-veg, and jain,
        // you may need to adjust these values from your Dish data.
        // Here we simply assume student_count_ applies to veg and set non-veg and jain to zero.
        // Also assume that the node-count is the size of the dish graph.
        int node_count = dish.graph_.size();

        ofs << dish.id_ << " " << dish.name_ << " " << dish.served_veg << " " << dish.served_nonveg << " " << dish.served_jain << " " << node_count << "\n";

        // Now, write each node entry.
        // The original example shows each node line as:
        //   dish-process-id process-id
        // For simplicity, here we output each node's id and its process id.
        for (const auto &kv : dish.graph_) {
            const auto &node = kv.second;
            ofs << node.node_id << " " << node.node_process_id_ << "\n";
        }
        // Next, write a placeholder for the dish graph edges.
        ofs << dish.connections_.size() << ' ';
        for (const auto &conn : dish.connections_) {
            ofs << conn.first << " " << conn.second << " ";
        }
        ofs << "\n"; 
    }

    ofs.close();
    return true;
}

#ifdef _WIN32
// Windows file dialog using GetSaveFileName API.
bool OpenWindowsSaveFileDialog(std::string &filePath) {
    char filename[MAX_PATH] = "";
    OPENFILENAMEA ofn = {0};  // Use OPENFILENAMEA for ANSI (char *).
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.lpstrDefExt = "txt";
    ofn.Flags = OFN_OVERWRITEPROMPT;
    if (GetSaveFileNameA(&ofn)) {
        filePath = filename;
        return true;
    }
    return false;
}
#elif defined(__linux__)
// Linux file dialog using Zenity via popen().
bool OpenLinuxSaveFileDialog(std::string &filePath) {
    // Construct command with default file path.
    std::string command = "zenity --file-selection --save --confirm-overwrite --filename=\"" + filePath + "\" 2>/dev/null";
    FILE *pipe = popen(command.c_str(), "r");
    if (!pipe) return false;
    char buffer[512];
    std::string result;
    while (fgets(buffer, sizeof(buffer), pipe)) {
       result += buffer;
    }
    int retCode = pclose(pipe);
    if (!result.empty()) {
         // Remove trailing newline if present.
         if (result.back() == '\n')
             result.pop_back();
         filePath = result;
         return true;
    }
    return false;
}
#endif

bool FormSave::Render() {
    bool saved = false;

    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(io.DisplaySize);
    ImGui::Begin("Save Input File", nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

    // Input field to display and edit the file path.
    char path_buffer[260];
    memset(path_buffer, 0, sizeof(path_buffer));
    strcpy_s(path_buffer, file_path_.c_str());
    ImGui::InputText("Save File Path", path_buffer, IM_ARRAYSIZE(path_buffer));
    file_path_ = path_buffer;

    // "Browse" button to open the file dialog.
    if (ImGui::Button("Browse")) {
#ifdef _WIN32
        if (OpenWindowsSaveFileDialog(file_path_)) {
            // File path updated from Windows dialog.
        }
#elif defined(__linux__)
        if (OpenLinuxSaveFileDialog(file_path_)) {
            // File path updated from Zenity.
        }
#endif
    }

    // Button to save the file.
    if (ImGui::Button("Save Input File")) {
        if (SaveToFile(file_path_)) {
            saved = true;
        }
    }
    if (saved) {
        ImGui::Text("File saved successfully to %s", file_path_.c_str());
    }
    ImGui::End();
    return saved;
}
