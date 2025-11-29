#pragma once
#include "BankClient.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing; 
using namespace System::Text::RegularExpressions;

public ref class ClientEditForm : public Form {
private:
    TextBox^ txtName;
    TextBox^ txtPassport;
    NumericUpDown^ numBalance;
    Button^ btnOK;
    Button^ btnCancel;
    BankClient^ clientToEdit;

public:
    property BankClient^ Client;

    ClientEditForm(BankClient^ client) {
        clientToEdit = client;
        InitializeComponent();
        if (client != nullptr) {
            Text = "Редактировать клиента";
            txtName->Text = client->Name;
            txtPassport->Text = client->Passport;
            numBalance->Value = Math::Min(numBalance->Maximum, static_cast<Decimal>(client->Balance));
        }
        else {
            Text = "Добавить клиента";
        }
    }

private:
    void InitializeComponent() {
        this->Size = System::Drawing::Size(400, 250);
        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
        this->MaximizeBox = false;
        this->StartPosition = FormStartPosition::CenterParent;

        Label^ lblName = gcnew Label(); lblName->Text = "Имя:"; lblName->Location = Point(20, 20);
        txtName = gcnew TextBox(); txtName->Location = Point(120, 20); txtName->Width = 240;

        Label^ lblPassport = gcnew Label(); lblPassport->Text = "Паспорт:"; lblPassport->Location = Point(20, 60);
        txtPassport = gcnew TextBox(); txtPassport->Location = Point(120, 60); txtPassport->Width = 240;

        Label^ lblBalance = gcnew Label(); lblBalance->Text = "Баланс:"; lblBalance->Location = Point(20, 100);
        numBalance = gcnew NumericUpDown();
        numBalance->Location = Point(120, 100);
        numBalance->Minimum = 0;
        numBalance->Maximum = 1000000;
        numBalance->DecimalPlaces = 2;
        numBalance->Width = 120;

        btnOK = gcnew Button(); btnOK->Text = "OK"; btnOK->Location = Point(100, 160); btnOK->Width = 80;
        btnOK->Click += gcnew EventHandler(this, &ClientEditForm::OnOKClick);

        btnCancel = gcnew Button(); btnCancel->Text = "Отмена"; btnCancel->Location = Point(200, 160); btnCancel->Width = 80;
        btnCancel->Click += gcnew EventHandler(this, &ClientEditForm::OnCancelClick);

        Controls->Add(lblName);
        Controls->Add(txtName);
        Controls->Add(lblPassport);
        Controls->Add(txtPassport);
        Controls->Add(lblBalance);
        Controls->Add(numBalance);
        Controls->Add(btnOK);
        Controls->Add(btnCancel);
    }

    void OnOKClick(Object^ sender, EventArgs^ e) {
        if (String::IsNullOrWhiteSpace(txtName->Text)) {
            MessageBox::Show("Имя не может быть пустым.", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
            return;
        }
        if (txtPassport->Text->Length != 8 || !Regex::IsMatch(txtPassport->Text, "^[a-zA-Z0-9]+$")) {
            MessageBox::Show("Паспорт должен содержать ровно 8 буквенно-цифровых символов.", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
            return;
        }

        Client = gcnew BankClient(txtName->Text, txtPassport->Text, static_cast<double>(numBalance->Value));
        if (clientToEdit != nullptr) {
            clientToEdit->Name = Client->Name;
            clientToEdit->Passport = Client->Passport;
            clientToEdit->Balance = Client->Balance;
        }
        this->DialogResult = System::Windows::Forms::DialogResult::OK;
        this->Close();
    }

    void OnCancelClick(Object^ sender, EventArgs^ e) {
        this->DialogResult = System::Windows::Forms::DialogResult::Cancel;
        this->Close();
    }
};