// form3.hpp
#pragma once
#include <imgui.h>
#include <vector>
#include <string>
#include "input_handler.hpp"

enum class NodeType { Ingredient, Process, Complete };

struct TempNode {
    int                temp_id;
    NodeType           type;
    std::string        name;
    int                process_id;   // for Process
    float              time;
    int                batch;
    std::vector<int> resource_ids;
    ImVec2             pos;
};

class Form3 {
public:
    bool Render();

private:
    // Popup
    bool        show_popup = false;
    int         type_idx;
    NodeType    popup_type;
    char        popup_ing_name[64];
    int         popup_proc_sel;      // index into process_map list, or -1 for new
    char        popup_proc_name[64];
    float       popup_time;
    int         popup_batch;
    std::vector<bool> popup_res_mask; // checkbox state for each resource
    bool dish_veg = false;
    bool dish_nonveg = false;
    bool dish_jain = false;


    // Temp graph
    std::vector<TempNode> nodes;
    std::vector<std::pair<int,int>> edges; 
    int        next_temp_id = 0;

    // Dishes
    char                  dish_name[128];
    std::vector<int>      dish_ids;
    int                   next_dish_id = 0;

    // Drag state
    int drag_start_node = -1;

    // Helpers
    void ResetPopup();
    void DrawNode(TempNode &n);
    void DrawConnections();
    TempNode* FindTempNode(int id);
};
