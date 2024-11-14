#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

// Data structure to store Symbol Table entries
struct SymbolTableEntry {
    string symbol;
    int address;
};

// Function to load the Symbol Table
void loadSymbolTable(unordered_map<string, int>& symbolTable) {
    symbolTable["BETA"] = 1012;
    symbolTable["ALPHA"] = 1009;
    symbolTable["FIRST"] = 1000;
    symbolTable["GAMMA"] = 1013;
    symbolTable["COPY"] = 0;
}

// Function to process the intermediate code and generate the final machine code
void processIntermediateCode(unordered_map<string, int>& symbolTable, istream& intermediateCode) {
    string line;

    // Loop through each line of intermediate code
    while (getline(intermediateCode, line)) {
        istringstream iss(line);
        string label, opcode, operand;

        // Read components of each line: label (optional), opcode, and operand
        iss >> label >> opcode >> operand;

        // If opcode is START or END, handle it specially
        if (opcode == "START") {
            cout << "START at address " << operand << endl;
            continue;
        } else if (opcode == "END") {
            cout << "END of program" << endl;
            continue;
        }

        // Generate machine code by replacing symbolic references with actual addresses
        if (symbolTable.find(operand) != symbolTable.end()) {
            int address = symbolTable[operand];
            cout << opcode << " " << address << endl;
        } else {
            cout << opcode << " " << operand << " // Operand not found in symbol table" << endl;
        }
    }
}

int main() {
    // Load the Symbol Table
    unordered_map<string, int> symbolTable;
    loadSymbolTable(symbolTable);

    // Example Intermediate Code input stream (could be read from a file)
    string intermediateCodeText =
        "COPY START 1000\n"
        "MOV ALPHA\n"
        "ADD BETA\n"
        "SUB GAMMA\n"
        "JMP FIRST\n"
        "COPY END\n";
    istringstream intermediateCode(intermediateCodeText);

    // Process the intermediate code
    processIntermediateCode(symbolTable, intermediateCode);

    return 0;
}
