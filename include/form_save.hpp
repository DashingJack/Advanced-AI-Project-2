// form_save.hpp
#pragma once
#include <string>

class FormSave {
public:
    // Render returns true if the "Save" operation was executed.
    bool Render();
private:
    // Helper to generate the input file contents and save to disk.
    bool SaveToFile(const std::string &filename);

    // File path that the user can specify.
    std::string file_path_ = "input_file.txt"; // default filename
};
