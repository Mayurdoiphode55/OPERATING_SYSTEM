#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

// Structure to store a macro's entry in the Macro Name Table (MNT)
struct MNTEntry {
    string macroName;
    int mdtIndex;
};

// Structure to store a macro's definition in the Macro Definition Table (MDT)
struct MDTEntry {
    string instruction;
};

// Function to load the MNT (Macro Name Table)
void loadMNT(vector<MNTEntry>& MNT) {
    MNT.push_back({"ADDM", 0});
}

// Function to load the MDT (Macro Definition Table)
void loadMDT(vector<MDTEntry>& MDT) {
    MDT.push_back({"LDA &ARG1"});
    MDT.push_back({"ADD &ARG2"});
    MDT.push_back({"STA &ARG1"});
    MDT.push_back({"MEND"});
}

// Function to process each line of intermediate code and expand macros
void processIntermediateCode(vector<MNTEntry>& MNT, vector<MDTEntry>& MDT, istream& intermediateCode) {
    string line;

    // Loop through each line of intermediate code
    while (getline(intermediateCode, line)) {
        istringstream iss(line);
        string label, opcode, operand1, operand2;

        iss >> label >> opcode >> operand1 >> operand2;

        bool isMacro = false;

        // Check if the opcode is a macro name in the MNT
        for (const auto& entry : MNT) {
            if (opcode == entry.macroName) {
                isMacro = true;
                int mdtIndex = entry.mdtIndex;
                unordered_map<string, string> argMap;

                // Parse macro arguments
                istringstream argStream(operand1 + "," + operand2);
                string arg1, arg2;
                getline(argStream, arg1, ',');
                getline(argStream, arg2, ',');

                // Map arguments to placeholders (ARG1 and ARG2)
                argMap["&ARG1"] = arg1;
                argMap["&ARG2"] = arg2;

                // Expand the macro by replacing placeholders with actual arguments
                for (int i = mdtIndex; MDT[i].instruction != "MEND"; i++) {
                    string expandedLine = MDT[i].instruction;
                    for (const auto& arg : argMap) {
                        size_t pos = expandedLine.find(arg.first);
                        if (pos != string::npos) {
                            expandedLine.replace(pos, arg.first.length(), arg.second);
                        }
                    }
                    cout << expandedLine << endl;  // Output expanded line
                }
                break;
            }
        }

        // If not a macro, simply output the line as-is
        if (!isMacro) {
            cout << line << endl;
        }
    }
}

int main() {
    // Load MNT and MDT for this example
    vector<MNTEntry> MNT;
    vector<MDTEntry> MDT;

    loadMNT(MNT);
    loadMDT(MDT);

    // Example Intermediate Code input stream (could be read from a file)
    string intermediateCodeText =
        "START \n"
        "LOOP ADDM A, B \n"
        "END";
    istringstream intermediateCode(intermediateCodeText);

    // Process the intermediate code
    processIntermediateCode(MNT, MDT, intermediateCode);

    return 0;
}
