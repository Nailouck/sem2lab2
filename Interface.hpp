#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include "ArraySequence.hpp"
#include "ListSequence.hpp"
#include "error.hpp"

void ClearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int GetIntInput(const std::string& prompt = "") {
    int value;
    if (!prompt.empty()) std::cout << prompt;
    if (!(std::cin >> value)) {
        ClearInput();
        throw Errors::InvalidArgument();
    }
    return value;
}

template<typename T>
T GetTypedInput(const std::string& prompt = "Enter value: ") {
    T value;
    std::cout << prompt;
    if (!(std::cin >> value)) {
        ClearInput();
        throw Errors::InvalidArgument();
    }
    return value;
}

struct ISequenceWrapper {
    virtual ~ISequenceWrapper() = default;
    virtual void Show() const = 0;
    virtual void Append() = 0;
    virtual void Prepend() = 0;
    virtual void InsertAt() = 0;
    virtual void RemoveAt() = 0;
    virtual void GetAt() const = 0;
    virtual ISequenceWrapper* GetSubsequence() const = 0;
    virtual ISequenceWrapper* Concat(const ISequenceWrapper* other) const = 0;
    virtual const std::string& TypeKey() const = 0;
};

template<typename T>
struct SequenceWrapper : public ISequenceWrapper {
    Sequence<T>* seq;
    std::string structure;
    std::string type_key;

    SequenceWrapper(const std::string& structure_, const std::string& key) : structure(structure_), type_key(key) {
        if (structure == "array") seq = new MutableArraySequence<T>();
        else seq = new MutableListSequence<T>();
    }

    ~SequenceWrapper() override {
        delete seq;
    }

    void Show() const override {
        std::cout << "\n[ ";
        for (int i = 0; i < seq->GetLength(); ++i)
            std::cout << seq->Get(i) << " ";
        std::cout << "] (Type: " << type_key << ", Structure: " << structure << ")\n";
    }

    void Append() override {
        T val = GetTypedInput<T>("\nEnter value to append: ");
        seq->Append(val);
    }

    void Prepend() override {
        T val = GetTypedInput<T>("\nEnter value to prepend: ");
        seq->Prepend(val);
    }

    void InsertAt() override {
        std::cout << "Valid position: 0 to " << seq->GetLength() << "\n";
        int index = GetIntInput("Enter position: ");
        if (index < 0 || index > seq->GetLength()) throw Errors::IndexOutOfRange();
        T val = GetTypedInput<T>();
        seq->InsertAt(val, index);
    }

    void RemoveAt() override {
        std::cout << "Valid index: 0 to " << seq->GetLength() - 1 << "\n";
        int index = GetIntInput("Enter index to remove: ");
        if (index < 0 || index >= seq->GetLength()) throw Errors::IndexOutOfRange();
        seq->Remove(index);
    }

    void GetAt() const override {
        std::cout << "Valid index: 0 to " << seq->GetLength() - 1 << "\n";
        int index = GetIntInput("Enter index to get: ");
        if (index < 0 || index >= seq->GetLength()) throw Errors::IndexOutOfRange();
        std::cout << "Element: " << seq->Get(index) << "\n";
    }

    ISequenceWrapper* GetSubsequence() const override {
        std::cout << "Valid range: 0 to " << seq->GetLength() - 1 << "\n";
        int start = GetIntInput("Enter start: ");
        int end = GetIntInput("Enter end: ");
        if (start < 0 || end >= seq->GetLength() || start > end) throw Errors::IndexOutOfRange();
        Sequence<T>* sub = seq->GetSubsequence(start, end);
        auto* result = new SequenceWrapper<T>(structure, type_key);
        delete result->seq;
        result->seq = sub;
        return result;
    }

    ISequenceWrapper* Concat(const ISequenceWrapper* other) const override {
        auto other_casted = dynamic_cast<const SequenceWrapper<T>*>(other);
        if (!other_casted) throw Errors::ConcatTypeMismatchError();

        Sequence<T>* new_seq;
        if (structure == "array") {
            new_seq = new MutableArraySequence<T>();
        }
        else if (structure == "list") {
            new_seq = new MutableListSequence<T>();
        }
        else {
            throw Errors::InvalidArgument();
        }

        for (int i = 0; i < seq->GetLength(); ++i)
            new_seq->Append(seq->Get(i));
        for (int i = 0; i < other_casted->seq->GetLength(); ++i)
            new_seq->Append(other_casted->seq->Get(i));

        auto* result = new SequenceWrapper<T>(structure, type_key);
        delete result->seq;
        result->seq = new_seq;
        return result;
    }


    const std::string& TypeKey() const override {
        return type_key;
    }
};

void ShowTypeMenu() {
    std::cout << "\nSelect type:\n1. int\n2. double\n3. string\n";
}
void ShowStructureMenu() {
    std::cout << "\nSelect structure:\n1. array\n2. list\n";
}

int interface(std::vector<ISequenceWrapper*>& sequences) {
    while (true) {
        std::cout << "\n1. Show\n2. Append\n3. Prepend\n4. Remove element\n5. Insert element at index\n6. Get element by index\n7. Get subsequence\n8. Concat sequences\n9. Add sequence\n10. Remove sequence\n11. Exit\n\nChoose action: ";

        try {
            int choice = GetIntInput();

            if (choice != 9 && choice != 11 && sequences.empty()) {
                std::cout << "No sequences yet. Add a sequence first.\n\n";
                continue;
            }

            switch (choice) {
            case 1:
                for (size_t i = 0; i < sequences.size(); ++i) {
                    std::cout << i << ": ";
                    sequences[i]->Show();
                }
                break;

            case 2: case 3: case 4: case 5: case 6: case 7: {
                std::cout << "Choose sequence index (from 0 to " << sequences.size() - 1 << "): ";
                int idx = GetIntInput();
                if (idx < 0 || static_cast<size_t>(idx) >= sequences.size()) throw Errors::IndexOutOfRange();
                switch (choice) {
                case 2: sequences[idx]->Append(); break;
                case 3: sequences[idx]->Prepend(); break;
                case 4: sequences[idx]->RemoveAt(); break;
                case 5: sequences[idx]->InsertAt(); break;
                case 6: sequences[idx]->GetAt(); break;
                case 7: {
                    ISequenceWrapper* result = sequences[idx]->GetSubsequence();
                    sequences.push_back(result);
                    std::cout << "Subsequence added as index " << sequences.size() - 1 << "\n";
                    break;
                }
                }
                break;
            }

            case 8: {
                std::cout << "First sequence index (0 to " << sequences.size() - 1 << "): ";
                int i1 = GetIntInput();
                std::cout << "Second sequence index (0 to " << sequences.size() - 1 << "): ";
                int i2 = GetIntInput();
                if (i1 < 0 || i2 < 0 || static_cast<size_t>(i1) >= sequences.size() || static_cast<size_t>(i2) >= sequences.size())
                    throw Errors::IndexOutOfRange();
                ISequenceWrapper* result = sequences[i1]->Concat(sequences[i2]);
                sequences.push_back(result);
                std::cout << "Concatenated sequence added as index " << sequences.size() - 1 << "\n";
                break;
            }

            case 9: {
                ShowTypeMenu();
                int t = GetIntInput("Enter type: ");
                std::string type;
                switch (t) {
                case 1: type = "int"; break;
                case 2: type = "double"; break;
                case 3: type = "string"; break;
                default: throw Errors::InvalidArgument();
                }

                ShowStructureMenu();
                int s = GetIntInput("Enter structure: ");
                std::string structure = (s == 1) ? "array" : (s == 2) ? "list" : throw Errors::InvalidArgument();

                if (type == "int") sequences.push_back(new SequenceWrapper<int>(structure, type));
                else if (type == "double") sequences.push_back(new SequenceWrapper<double>(structure, type));
                else if (type == "string") sequences.push_back(new SequenceWrapper<std::string>(structure, type));
                break;
            }

            case 10: {
                std::cout << "Index to remove (0 to " << sequences.size() - 1 << "): ";
                int idx = GetIntInput();
                if (idx < 0 || static_cast<size_t>(idx) >= sequences.size()) throw Errors::IndexOutOfRange();
                delete sequences[idx];
                sequences.erase(sequences.begin() + idx);
                break;
            }

            case 11:
                std::cout << "Exiting...\n";
                for (auto* ptr : sequences) delete ptr;
                return 0;

            default:
                std::cout << "Invalid action.\n";
            }

        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }
}