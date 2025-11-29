#pragma once
using namespace System;

[Serializable]
public ref class BankClient {
public:
    property String^ Name;
    property String^ Passport;
    property double Balance;

    BankClient(String^ name, String^ passport, double balance) {
        Name = name;
        Passport = passport;
        Balance = balance;
    }
};