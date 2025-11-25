#include "../include/parser.h"
#include "../include/ir.h"

using std::cout;
using std::endl;

int main() {
    if (program) {
        cout << "Generating IR..." << endl;
        IRGenerator irGen;
        irGen.generate(program.get());
        
        cout << "--- Intermediate Representation (TAC) ---" << endl;
        irGen.printInstructions();
    }

    return 0;
}
