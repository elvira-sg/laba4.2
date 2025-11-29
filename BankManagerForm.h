#pragma once
#include "BankClient.h"
#include "ClientEditForm.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;
using namespace System::IO;
using namespace System::Drawing; // ✅ Добавлено!

public ref class BankManagerForm : public Form {
private:
    List<BankClient^>^ clients;
    DataGridView^ dataGridView;
    Button^ btnAdd, ^ btnEdit, ^ btnDelete, ^ btnSave, ^ btnLoad, ^ btnSort;

public:
    BankManagerForm() {
        clients = gcnew List<BankClient^>();
        InitializeComponent();
    }

private:
    // Статический метод для сравнения — замена лямбде
    static int CompareByBalance(BankClient^ a, BankClient^ b) {
        return a->Balance.CompareTo(b->Balance);
    }

    void InitializeComponent() {
        Text = "Банковская система";
        Size = System::Drawing::Size(800, 600); 
        StartPosition = FormStartPosition::CenterScreen;

        dataGridView = gcnew DataGridView();
        dataGridView->Dock = DockStyle::Top;
        dataGridView->Height = 400;
        dataGridView->ReadOnly = true;
        dataGridView->AllowUserToAddRows = false;
        dataGridView->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
        Controls->Add(dataGridView);

        dataGridView->Columns->Add("colName", "Имя");
        dataGridView->Columns->Add("colPassport", "Паспорт");
        dataGridView->Columns->Add("colBalance", "Баланс");

        FlowLayoutPanel^ panel = gcnew FlowLayoutPanel();
        panel->Dock = DockStyle::Bottom;
        panel->Height = 60;
        panel->FlowDirection = FlowDirection::LeftToRight;
        panel->Padding = System::Windows::Forms::Padding(10);

        btnAdd = gcnew Button(); btnAdd->Text = "Добавить"; btnAdd->Width = 100;
        btnAdd->Click += gcnew EventHandler(this, &BankManagerForm::OnAddClick);

        btnEdit = gcnew Button(); btnEdit->Text = "Изменить"; btnEdit->Width = 100;
        btnEdit->Click += gcnew EventHandler(this, &BankManagerForm::OnEditClick);

        btnDelete = gcnew Button(); btnDelete->Text = "Удалить"; btnDelete->Width = 100;
        btnDelete->Click += gcnew EventHandler(this, &BankManagerForm::OnDeleteClick);

        btnSave = gcnew Button(); btnSave->Text = "Сохранить как..."; btnSave->Width = 140;
        btnSave->Click += gcnew EventHandler(this, &BankManagerForm::OnSaveClick);

        btnLoad = gcnew Button(); btnLoad->Text = "Загрузить из..."; btnLoad->Width = 140;
        btnLoad->Click += gcnew EventHandler(this, &BankManagerForm::OnLoadClick);

        btnSort = gcnew Button(); btnSort->Text = "Сортировка по балансу"; btnSort->Width = 160;
        btnSort->Click += gcnew EventHandler(this, &BankManagerForm::OnSortClick);

        panel->Controls->Add(btnAdd);
        panel->Controls->Add(btnEdit);
        panel->Controls->Add(btnDelete);
        panel->Controls->Add(btnSave);
        panel->Controls->Add(btnLoad);
        panel->Controls->Add(btnSort);

        Controls->Add(panel);
    }

    void RefreshGrid() {
        dataGridView->Rows->Clear();
        for each (BankClient ^ c in clients) {
            dataGridView->Rows->Add(c->Name, c->Passport, c->Balance.ToString("F2"));
        }
    }

    void OnAddClick(Object^ sender, EventArgs^ e) {
        ClientEditForm^ form = gcnew ClientEditForm(nullptr);
        if (form->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
            clients->Add(form->Client);
            RefreshGrid();
        }
    }

    void OnEditClick(Object^ sender, EventArgs^ e) {
        if (dataGridView->SelectedRows->Count == 0) {
            MessageBox::Show("Выберите клиента.", "Информация", MessageBoxButtons::OK, MessageBoxIcon::Information);
            return;
        }
        int idx = dataGridView->SelectedRows[0]->Index;
        BankClient^ c = clients[idx];
        ClientEditForm^ form = gcnew ClientEditForm(c);
        if (form->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
            RefreshGrid();
        }
    }

    void OnDeleteClick(Object^ sender, EventArgs^ e) {
        if (dataGridView->SelectedRows->Count == 0) return;
        if (MessageBox::Show("Удалить?", "Подтверждение", MessageBoxButtons::YesNo) == System::Windows::Forms::DialogResult::No) return;
        int idx = dataGridView->SelectedRows[0]->Index;
        clients->RemoveAt(idx);
        RefreshGrid();
    }

    void OnSaveClick(Object^ sender, EventArgs^ e) {
        SaveFileDialog^ dialog = gcnew SaveFileDialog();
        dialog->Filter = "Текстовые файлы (*.txt)|*.txt|Все файлы (*.*)|*.*";
        dialog->DefaultExt = "txt";
        if (dialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
            try {
                StreamWriter^ sw = gcnew StreamWriter(dialog->FileName, false, System::Text::Encoding::UTF8);
                for each (BankClient ^ c in clients) {
                    sw->WriteLine(c->Name + "|" + c->Passport + "|" + c->Balance);
                }
                sw->Close();
                MessageBox::Show("Сохранено!", "Успех", MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
            catch (Exception^ ex) {
                MessageBox::Show("Ошибка: " + ex->Message, "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }
    }

    void OnLoadClick(Object^ sender, EventArgs^ e) {
        OpenFileDialog^ dialog = gcnew OpenFileDialog();
        dialog->Filter = "Текстовые файлы (*.txt)|*.txt|Все файлы (*.*)|*.*";
        if (dialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
            try {
                clients->Clear();
                StreamReader^ sr = gcnew StreamReader(dialog->FileName, System::Text::Encoding::UTF8);
                String^ line;
                while ((line = sr->ReadLine()) != nullptr) {
                    array<String^>^ parts = line->Split('|');
                    if (parts->Length == 3) {
                        double bal;
                        if (double::TryParse(parts[2], bal)) {
                            clients->Add(gcnew BankClient(parts[0], parts[1], bal));
                        }
                    }
                }
                sr->Close();
                RefreshGrid();
                MessageBox::Show("Загружено!", "Успех", MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
            catch (Exception^ ex) {
                MessageBox::Show("Ошибка: " + ex->Message, "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }
    }

    void OnSortClick(Object^ sender, EventArgs^ e) {
        clients->Sort(gcnew Comparison<BankClient^>(&BankManagerForm::CompareByBalance));
        RefreshGrid();
    }
};