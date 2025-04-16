// form3.cpp
#include "form3.hpp"
#include "input_handler.hpp"
#include <imgui_internal.h>
using namespace inputs;

void Form3::ResetPopup() {
    type_idx = 0;
    popup_type = NodeType::Ingredient;
    popup_ing_name[0] = '\0';
    popup_proc_sel = -1;
    popup_proc_name[0] = '\0';
    popup_time = 0; popup_batch = 0;
    popup_res_mask.assign(inputs::resource_map.size(), false);
}

TempNode* Form3::FindTempNode(int id) {
    for (auto &n: nodes) if (n.temp_id == id) return &n;
    return nullptr;
}

bool Form3::Render() {
    ImGuiIO& io = ImGui::GetIO();
    static bool first = true;
    if (first) { ResetPopup(); dish_name[0]=0; first=false; }

    ImGui::SetNextWindowPos({0,0});
    ImGui::SetNextWindowSize(io.DisplaySize);
    ImGui::Begin("Form3: Build DAG", nullptr,
                 ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoTitleBar);

    // Controls
    if (ImGui::Button("Add Node")) { ResetPopup(); ImGui::OpenPopup("AddNode"); }
    ImGui::SameLine(); ImGui::InputText("Dish Name", dish_name,64);
    ImGui::SameLine();
    ImGui::Checkbox("Veg", &dish_veg);
    ImGui::SameLine();
    ImGui::Checkbox("Non-Veg", &dish_nonveg);
    ImGui::SameLine();
    ImGui::Checkbox("Jain", &dish_jain);
    if (ImGui::Button("Add Dish") && dish_name[0] && !nodes.empty()) {
        // 1) Create new Dish
        Dish d;
        d.id_ = next_dish_id++;
        d.name_ = dish_name;
        // Set the student count as before (or update as needed)
        d.student_count_ = inputs::cooks; // Or adjust this based on your logic
    
        // NEW: Set the dish serving types from the checkboxes
        d.served_veg = dish_veg;
        d.served_nonveg = dish_nonveg;
        d.served_jain = dish_jain;
        
        // 2) Build DishNode objects in d.graph_
        for (auto &tn : nodes) {
            if (tn.type == NodeType::Process) {
                DishNode dn;
                dn.node_id = tn.temp_id;
                dn.instances_required_ = 1;
                dn.instances_done = 0;
                dn.is_done = false;
                dn.node_process_id_ = tn.process_id;
                d.graph_[dn.node_id] = dn;
            }
        }
    
        // 3) Store the connections (parent, child) pairs in the Dish.
        std::vector<std::pair<int,int>> validEdges;
        for (auto &e : edges) {
            if (d.graph_.find(e.first) != d.graph_.end() &&
                d.graph_.find(e.second) != d.graph_.end())
            {
                validEdges.push_back(e);
            }
        }
        d.connections_ = validEdges;
    
        // 4) Identify starting/ending nodes if needed (this may be optional if you use connections)
    
        // 5) Store in global map
        inputs::dish_map[d.id_] = std::move(d);
        dish_ids.push_back(d.id_);
    
        // 6) Clear the canvas for the next dish
        nodes.clear();
        edges.clear();
        next_temp_id = 0;
        dish_name[0] = '\0';
    
        // Reset the dish serving type checkboxes as well:
        dish_veg = false;
        dish_nonveg = false;
        dish_jain = false;
    }
    ImGui::Separator();

    // Popup
    if (ImGui::BeginPopupModal("AddNode",nullptr,ImGuiWindowFlags_AlwaysAutoResize)) {
        const char* types[]={"Ingredient","Process","Complete"};
        if (ImGui::Combo("Node Type",&type_idx,types,3)) {
            popup_type = NodeType(type_idx);
            popup_proc_sel=-1;
        }
        ImGui::Spacing();

        if (popup_type==NodeType::Ingredient) {
            ImGui::InputText("Ingredient Name",popup_ing_name,64);
        }
        else if (popup_type==NodeType::Process) {
            // Process dropdown
            std::vector<const char*> proc_names;
            for (auto &p: process_map) proc_names.push_back(p.second.name_.c_str());
            proc_names.push_back("<New>");
            if (ImGui::Combo("Process",&popup_proc_sel,proc_names.data(),proc_names.size())) {
                if (popup_proc_sel < (int)process_map.size()) {
                    // existing
                    auto it = std::next(process_map.begin(),popup_proc_sel);
                    popup_proc_name[0]=0;
                    popup_time = it->second.time_;
                    popup_batch= it->second.batch_size_;
                    popup_res_mask.assign(inputs::resource_map.size(), false);
                    for (int rid : it->second.resources_) {
                        int idx = 0;
                        for (auto &kv : resource_map) {
                            if (kv.first == rid) {
                                popup_res_mask[idx] = true;
                                break;
                            }
                            idx++;
                        }
                    }
                } else {
                    popup_proc_name[0]=0;
                    popup_time=0; popup_batch=0;
                }
            }
            if (popup_proc_sel==(int)process_map.size()) {
                ImGui::InputText("New Proc Name",popup_proc_name,64);
            }
            ImGui::InputFloat("Time (min)",&popup_time);
            ImGui::InputInt("Batch Size",&popup_batch);

            // Resource dropdown
            ImGui::Text("Select Resources:");
            int idx = 0;
            for (auto &kv : resource_map) {
                const auto &res = kv.second;
                // Render each resource checkbox visibly
                bool tmp = popup_res_mask[idx];
                if (ImGui::Checkbox(res.name_.c_str(), &tmp)) {
                    popup_res_mask[idx] = tmp;
                }
                idx++;
            }
            
        }
        // Complete → nothing

        ImGui::Spacing();
        if (ImGui::Button("Create")) {
            TempNode tn;
            tn.temp_id = next_temp_id++;
            tn.type = popup_type;
            if (popup_type==NodeType::Ingredient) {
                tn.name = popup_ing_name;
            } else if (popup_type==NodeType::Process) {
                // ensure process_map entry
                if (popup_proc_sel==(int)process_map.size()) {
                    Process pr;
                    pr.id_ = process_map.size();
                    pr.name_ = popup_proc_name;
                    pr.time_ = popup_time;
                    pr.batch_size_ = popup_batch;
                    for (int idx = 0; idx < (int)popup_res_mask.size(); ++idx) {
                        if (popup_res_mask[idx]) {
                            auto it = std::next(resource_map.begin(), idx);
                            pr.resources_.push_back(it->first);
                        }
                    }
                    process_map[pr.id_] = pr;
                    tn.process_id = pr.id_;
                } else {
                    auto it = std::next(process_map.begin(),popup_proc_sel);
                    tn.process_id = it->first;
                    tn.resource_ids = it->second.resources_;
                }
                tn.name = process_map[tn.process_id].name_;
                tn.time = popup_time;
                tn.batch= popup_batch;
                tn.resource_ids.clear();
                int idx = 0;
                for (auto &kv : resource_map) {
                if (popup_res_mask[idx]) {
                    tn.resource_ids.push_back(kv.first);
                }
                    idx++;
                }
            } else {
                tn.name="Complete"; tn.process_id=-1;
            }
            tn.pos = ImGui::GetCursorScreenPos();
            nodes.push_back(tn);
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }

    // Canvas
    ImGui::BeginChild("Canvas",ImVec2(0,0),true);
      DrawConnections();
      for (auto &n: nodes) DrawNode(n);
    ImGui::EndChild();

    // Dish list + Next
    ImGui::Separator();
    for (int id: dish_ids) {
      ImGui::Text("%s",dish_map[id].name_.c_str());
      ImGui::SameLine();
      if (ImGui::Button((std::string("Del##")+std::to_string(id)).c_str())) {
        dish_map.erase(id);
        dish_ids.erase(std::remove(dish_ids.begin(),dish_ids.end(),id),dish_ids.end());
      }
    }
    ImGui::Separator();
    bool done=false;
    float bw=120, cx=(io.DisplaySize.x-bw)/2;
    ImGui::SetCursorPosX(cx);
    if (ImGui::Button("Next",ImVec2(bw,0))) done=true;

    ImGui::End();
    return done;
}

void Form3::DrawConnections(){
    auto &dl=*ImGui::GetWindowDrawList();
    for (auto &e: edges){
        auto *p=FindTempNode(e.first);
        auto *c=FindTempNode(e.second);
        if(!p||!c) continue;
        // anchor Y: ingredient bottom at 40, process bottom at 80; complete top at 40
        ImVec2 pb{p->pos.x+75, p->pos.y + (p->type==NodeType::Ingredient?40:80)};
        ImVec2 ct{c->pos.x+75, c->pos.y + (c->type==NodeType::Complete?40:0)};
        dl.AddLine(pb,ct,IM_COL32(200,200,100,255),2.0f);
    }
}

void Form3::DrawNode(TempNode &n){
    ImGui::PushID(n.temp_id);
    ImGui::SetCursorScreenPos(n.pos);
    ImGui::InvisibleButton("drag",ImVec2(150,80));
    if(ImGui::IsItemActive()&&ImGui::IsMouseDragging(0)){
        n.pos.x+=ImGui::GetIO().MouseDelta.x;
        n.pos.y+=ImGui::GetIO().MouseDelta.y;
    }
    auto &dl=*ImGui::GetWindowDrawList();
    dl.AddRectFilled(n.pos,{n.pos.x+150,n.pos.y+80},IM_COL32(50,50,60,255),6.0f);

    // Anchors
    ImVec2 top{n.pos.x+75,n.pos.y + (n.type==NodeType::Complete?40:0)};
    ImVec2 bot{n.pos.x+75,n.pos.y + (n.type==NodeType::Ingredient?40:80)};
    if(n.type!=NodeType::Ingredient) dl.AddCircleFilled(top,5,IM_COL32(200,200,100,255));
    if(n.type!=NodeType::Complete ) dl.AddCircleFilled(bot,5,IM_COL32(200,200,100,255));

    // Drag logic
    ImVec2 m=ImGui::GetIO().MousePos;
    if(ImGui::IsMouseClicked(0)){
        if(n.type!=NodeType::Ingredient && fabs(m.x-top.x)<8&&fabs(m.y-top.y)<8 && drag_start_node>=0){
            edges.emplace_back(drag_start_node,n.temp_id);
            drag_start_node=-1;
        }
        if(n.type!=NodeType::Complete && fabs(m.x-bot.x)<8&&fabs(m.y-bot.y)<8){
            drag_start_node=n.temp_id;
        }
    }

    // Content
    ImGui::SetCursorScreenPos({n.pos.x+5,n.pos.y+5});
    ImGui::TextUnformatted(n.name.c_str());
    if(n.type==NodeType::Process){
        ImGui::Text("T:%.1f B:%d",n.time,n.batch);
    }
    // Delete
    ImGui::SetCursorScreenPos({n.pos.x+5,n.pos.y+60});
    if(ImGui::Button("Delete")) {
        int id = n.temp_id;
        nodes.erase(std::remove_if(nodes.begin(), nodes.end(),
                [&](auto &x){ return x.temp_id == id; }), nodes.end());
        edges.erase(std::remove_if(edges.begin(), edges.end(),
                [&](auto &e){ return e.first == id || e.second == id; }), edges.end());
        ImGui::PopID();
        return;
    }
    ImGui::PopID();
}
