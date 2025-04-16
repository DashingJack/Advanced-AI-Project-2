// main_window.hpp
#pragma once
#include "form1.hpp"
#include "form2.hpp"
#include "form3.hpp"
#include "form_save.hpp"

enum class FormState { Form1, Form2, Form3, Form4, Form5 };

class MainWindow {
public:
    void run();

private:
    FormState currentForm = FormState::Form1;

    // ← Create one instance of each form
    Form1 form1;
    Form2 form2;
    Form3 form3;
    FormSave form4;
};
