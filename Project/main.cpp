#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>
using namespace std;

struct Patient {
    char id[20];
    char name[30];
    int age;
    char gender[10];
    char disease[30];
    char phone[15];
};

/* ---------- UTILITY VALIDATION FUNCTIONS ---------- */
bool isDigitsOnly(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i]))
            return false;
    }
    return true;
}

/* ---------- CHECK DUPLICATE ID ---------- */
bool idExists(const char* fileName, const char* checkId) {
    Patient p;
    fstream file(fileName, ios::in | ios::binary);

    if (!file) return false;

    while (file.read((char*)&p, sizeof(p))) {
        if (strcmp(p.id, checkId) == 0) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

/* ---------- REGISTER PATIENT ---------- */
void registerPatient(const char* fileName) {
    Patient p;

    cout << "\n--- Register Patient ---\n";
    cout << "Enter Patient ID: ";
    cin >> p.id;

    if (idExists(fileName, p.id)) {
        cout << "❌ Error: Patient ID already exists!\n";
        return;
    }

    cout << "Enter Name: ";
    cin >> p.name;

    do {
        cout << "Enter Age (1-120): ";
        cin >> p.age;
    } while (p.age < 1 || p.age > 120);

    do {
        cout << "Enter Gender (male/female): ";
        cin >> p.gender;
    } while (strcmp(p.gender, "male") != 0 && strcmp(p.gender, "female") != 0);

    cout << "Enter Disease: ";
    cin >> p.disease;

    do {
        cout << "Enter Phone (digits only): ";
        cin >> p.phone;
    } while (!isDigitsOnly(p.phone) || strlen(p.phone) < 7 || strlen(p.phone) > 14);

    fstream file(fileName, ios::out | ios::app | ios::binary);
    file.write((char*)&p, sizeof(p));
    file.close();

    cout << "✅ Patient registered successfully!\n";
}

/* ---------- DISPLAY ALL ---------- */
void displayAllPatients(const char* fileName) {
    Patient p;
    fstream file(fileName, ios::in | ios::binary);

    if (!file) {
        cout << "⚠ No records found.\n";
        return;
    }

    cout << "\n===== PATIENT LIST =====\n";
    while (file.read((char*)&p, sizeof(p))) {
        cout << "ID      : " << p.id << endl;
        cout << "Name    : " << p.name << endl;
        cout << "Age     : " << p.age << endl;
        cout << "Gender  : " << p.gender << endl;
        cout << "Disease : " << p.disease << endl;
        cout << "Phone   : " << p.phone << endl;
        cout << "-------------------------\n";
    }
    file.close();
}

/* ---------- SEARCH PATIENT ---------- */
void searchPatient(const char* fileName) {
    char searchId[20];
    Patient p;
    bool found = false;

    cout << "Enter Patient ID to search: ";
    cin >> searchId;

    fstream file(fileName, ios::in | ios::binary);

    while (file.read((char*)&p, sizeof(p))) {
        if (strcmp(p.id, searchId) == 0) {
            cout << "\n✅ Patient Found!\n";
            cout << "Name    : " << p.name << endl;
            cout << "Age     : " << p.age << endl;
            cout << "Gender  : " << p.gender << endl;
            cout << "Disease : " << p.disease << endl;
            cout << "Phone   : " << p.phone << endl;
            found = true;
            break;
        }
    }

    if (!found)
        cout << "❌ Patient not found!\n";

    file.close();
}

/* ---------- UPDATE PATIENT ---------- */
void updatePatient(const char* fileName) {
    char updateId[20];
    Patient p;
    bool found = false;

    cout << "Enter Patient ID to update: ";
    cin >> updateId;

    fstream file(fileName, ios::in | ios::out | ios::binary);

    while (file.read((char*)&p, sizeof(p))) {
        if (strcmp(p.id, updateId) == 0) {

            cout << "Enter New Name: ";
            cin >> p.name;

            do {
                cout << "Enter New Age (1-120): ";
                cin >> p.age;
            } while (p.age < 1 || p.age > 120);

            do {
                cout << "Enter New Gender (male/female): ";
                cin >> p.gender;
            } while (strcmp(p.gender, "male") != 0 && strcmp(p.gender, "female") != 0);

            cout << "Enter New Disease: ";
            cin >> p.disease;

            do {
                cout << "Enter New Phone: ";
                cin >> p.phone;
            } while (!isDigitsOnly(p.phone));

            file.seekp(-sizeof(p), ios::cur);
            file.write((char*)&p, sizeof(p));

            found = true;
            cout << "✅ Patient updated successfully!\n";
            break;
        }
    }

    if (!found)
        cout << "❌ Patient ID not found!\n";

    file.close();
}

/* ---------- DELETE PATIENT ---------- */
void deletePatient(const char* fileName) {
    char deleteId[20];
    Patient p;
    bool found = false;

    cout << "Enter Patient ID to delete: ";
    cin >> deleteId;

    fstream file(fileName, ios::in | ios::binary);
    fstream temp("temp.dat", ios::out | ios::binary);

    while (file.read((char*)&p, sizeof(p))) {
        if (strcmp(p.id, deleteId) != 0) {
            temp.write((char*)&p, sizeof(p));
        } else {
            found = true;
        }
    }

    file.close();
    temp.close();

    remove(fileName);
    rename("temp.dat", fileName);

    if (found)
        cout << "✅ Patient deleted successfully!\n";
    else
        cout << "❌ Patient ID not found!\n";
}

/* ---------- MAIN ---------- */
int main() {
    const char* fileName = "patients.dat";
    int choice;

    while (true) {
        cout << "\n===== HOSPITAL MENU =====\n";
        cout << "1. Register Patient\n";
        cout << "2. Display All Patients\n";
        cout << "3. Search Patient\n";
        cout << "4. Update Patient\n";
        cout << "5. Delete Patient\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "❌ Invalid input! Enter a number.\n";
            continue;
        }

        switch (choice) {
        case 1: registerPatient(fileName); break;
        case 2: displayAllPatients(fileName); break;
        case 3: searchPatient(fileName); break;
        case 4: updatePatient(fileName); break;
        case 5: deletePatient(fileName); break;
        case 6: return 0;
        default: cout << "❌ Invalid choice!\n";
        }
    }
}

