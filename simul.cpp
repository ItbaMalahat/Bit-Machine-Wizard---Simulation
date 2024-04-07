#include <iostream>
#include <string>
using namespace std;

string symbolTable[32] = {"AND", "ADD", "LDA", "STA", "BUN", "BSA", "ISZ", "OR", "XOR", "SUB", "ILDA", "ISTA", "IBUN", "IADD", "CLA", "CLE", "CMA", "CME", "CIR", "CIL", "INC", "SPA", "SNA", "SZA", "SZE", "HLT", "INP", "OUT", "SKI", "SKO", "ION", "IOF"};

class simulator
{
private:
    string memory[4096];
    int ac;
    int dr, pc, ar, tr, outr, inpr;
    string ir;
    int sc;
    int ien;
    int fgo, fgi;
    int Cout;

public:
    simulator()
    {
        for (int i = 0; i < 4096; i++)
            memory[i] = "0";

        inpr = outr = ac = 0;

        sc = dr = pc = ar = tr = ien = fgo = fgi = 0;
        ir = "0";
    }

    void artpc() // ar to pc. ar <- pc
    {
        ar = pc;
    }

    void mtoir() // memory to ir. ir <- m[ar]
    {
        if (ar < 4096 && ar >= 0) // fixed off-by-one error
            ir = memory[ar];
    }

    void incpc() // increment pc
    {
        pc++; // fixed syntax error
    }

    void decoder()
    {
        // empty function body
    }

    void load_program()
    {
        int ch;
        cout << "Press 1- TO write your own program, 2-TO load a ready made program into memory to run\n";
        cin >> ch;
        if (ch == 1)
        {
            string instruction;
            int loc, val;
            cout << "To store an inatruction or data at a memory location, specify the memory location as well as the value. (press -1 when asked for the memory location to terminate the loop)\n";

            while (1)
            {
                cout << "enter the memory location\n";
                cin >> loc;

                if (loc < 0)
                    break;

                if (loc >= 4095)
                {
                    cout << "out of range\n";
                    continue;
                }

                cout << "enter the instruction/data you want to enter at location " << loc << endl;
                cin.ignore();
                getline(cin, instruction);

                memory[loc] = instruction;
            }
        }
        else if (ch == 2)
        {
            cout << "What program would you like to run?\n";
            cout << "1- Add numbers in an array, 2- Subroutine, 3- Multiplication\n";
            cin >> ch;

            if (ch == 1)
            {
                memory[0] = "CLA";
                memory[1] = "IADD 7";
                memory[2] = "ISZ 7";
                memory[3] = "ISZ 8";
                memory[4] = "BUN 1";
                memory[5] = "STA 9";
                memory[6] = "HLT";
                memory[7] = "10";
                memory[8] = "-3";
                memory[10] = "1";
                memory[11] = "4";
                memory[12] = "3";

                for (int i = 0; i <= 12; i++)
                {
                    cout << "loc: " << i << "-> " << memory[i] << endl;
                }
            }

            else if (ch == 2)
            {

                cout << "program for subroutine\n";
                memory[0] = "LDA 7";
                memory[1] = "BSA 9";
                memory[2] = "STA 14";
                memory[3] = "LDA 8";
                memory[4] = "BSA 9";
                memory[5] = "STA 15";
                memory[6] = "HLT";
                memory[7] = "1";
                memory[8] = "2";
                memory[9] = "0";
                memory[10] = "CIL";
                memory[11] = "CIL";
                memory[12] = "CIL";
                memory[13] = "CIL";
                memory[14] = "IBUN 9";

                for (int i = 0; i <= 14; i++)
                {
                    cout << "loc: " << i << " -> " << memory[i] << endl;
                }
            }

            else if (ch == 3)
            {
                cout << "program for 15 * 11\n";

                memory[0] = "LDA 6";
                memory[1] = "ADD 7";
                memory[2] = "STA 6";
                memory[3] = "ISZ 8";
                memory[4] = "BUN 0";
                memory[5] = "HLT";
                memory[6] = "0";
                memory[7] = "15";
                memory[8] = "-11";

                for (int i = 0; i <= 8; i++)
                {
                    cout << "loc: " << i << " -> " << memory[i] << endl;
                }
            }
        }
    }

    int instruction_decoder(string opcode) // given the opcode, it returns the instruction number using the symbol table. for add, returns 0 etc;
    {
        for (int i = 0; i < 32; i++)
        {
            if (symbolTable[i] == opcode)
                return i;
        }

        return -1;
    }

    // memory reference instructions

    // AND:    DR <- M[AR],   AC <- AC ^ DR, SC <- 0
    void AND()
    {
        dr = stoi(memory[ar]);
        ac = ac & dr;
        sc = 0;
    }

    // ADD:  DR <- M[AR],    AC <- AC + DR, E <- cout, SC <- 0
    void ADD()
    {
        Cout = 0;
        dr = stoi(memory[ar]);
        ac = ac + dr;
        if (ac > 65536) // for overflow
        {
            Cout = 1;
        }
        else
        {
            Cout = 0;
        }
        sc = 0;
    }

    // LDA: DR <- M[AR],   AC <- DR, SC <- 0
    void LDA()
    {
        dr = stoi(memory[ar]);
        ac = dr;
        sc = 0;
    }

    // STA: M[AR] <- AC, SC <- 0
    void STA()
    {
        memory[ar] = to_string(ac);
        sc = 0;
    }

    // BUN:  PC <- AR, SC <- 0
    void BUN()
    {
        pc = ar;
        sc = 0;
    }

    // BSA: M[AR] <- PC, AR <- AR + 1,     PC <- AR, SC <- 0
    void BSA()
    {
        memory[ar] = to_string(pc);
        ar++;
        pc = ar;
        //  pc++;
        sc = 0;
    }

    // ISZ: DR <- M[AR],   DR <- DR + 1,     M[AR] <- DR, if (DR = 0) then PC <- PC + 1, SC <- 0
    void ISZ()
    {
        dr = stoi(memory[ar]);
        dr++;
        memory[ar] = to_string(dr);
        if (dr == 0)
        {
            pc++;
        }
        sc = 0;
    }

    // OR: DR <- M[AR],      AC <- AC v DR, SC <- 0
    void OR()
    {
        dr = stoi(memory[ar]);
        ac = ac | dr;
        sc = 0;
    }

    // XOR: DR <- M[AR], AC <- AC ⊕ DR, SC <- 0
    void XOR()
    {
        dr = stoi(memory[ar]);
        ac = ac ^ dr;
        sc = 0;
    }

    // SUB: DR <- M[AR], AC <- AC - DR, SC <- 0
    void SUB()
    {
        dr = stoi(memory[ar]);
        ac = ac - dr;
        sc = 0;
    }

    // ILDA: AR <- M[AR], DR <- M[AR] , AC <- DR, SC <- 0
    void ILDA()
    {
        ar = stoi(memory[ar]);
        dr = stoi(memory[ar]);
        ac = dr;
        sc = 0;
    }

    // ISTA: AR <- M[AR], M[AR] <- AC, SC <- 0
    void ISTA()
    {
        ar = stoi(memory[ar]);
        memory[ar] = to_string(ac);
        sc = 0;
    }

    // IBUN: AR <- M[AR], PC -< AR, SC <- 0

    void IBUN()
    {
        ar = stoi(memory[ar]);
        pc = ar;
        sc = 0;
    }

    // IADD: AR <- M[AR], DR <- M[AR], AC <- AC + DR, E <- cout, SC <- 0
    void IADD()
    {
        Cout = 0;
        ar = stoi(memory[ar]);
        dr = stoi(memory[ar]);
        ac = ac + dr;
        if (ac > 65536) // greater than 16 bits
        {
            Cout = 1;
        }
        else
        {
            Cout = 0;
        }
        sc = 0;
    }

    void run_program() // fetch decode execute cycle runs until hlt is encountered
    {
        string opcode, operand_str;
        int operand;
        cout << "choose the initial value of pc (between 0 - 4095)\n";
        cin >> pc;

        while (pc < 4096)
        {
            // fetch cycle
            ar = pc;
            ir = memory[ar];
            pc++;

            // decode cycle
            // split ir into opcode and operand

            size_t pos = ir.find(' '); // finds space in instruction

            if (pos == string::npos) // if no space
            {
                opcode = ir;
                operand = 0; // set operand to some default value
                ar = 0;
            }
            else // split into opcode and operand
            {
                opcode = ir.substr(0, pos); // from 0 - space gets stored in opcode
                operand_str = ir.substr(pos + 1);
                operand = stoi(operand_str);
                ar = operand;
            }

            if (opcode == "HLT")
                break;

            int num_instruction = instruction_decoder(opcode);

            if (num_instruction >= 0 && num_instruction <= 13) // memory reference instructions
            {
                // int operandAddress = symbolTable[operand];
                switch (num_instruction)
                {
                case 0:
                    AND();
                    break;
                case 1:
                    ADD();
                    break;
                case 2:
                    LDA();
                    break;
                case 3:
                    STA();
                    break;
                case 4:
                    BUN();
                    break;
                case 5:
                    BSA();
                    break;
                case 6:
                    ISZ();
                    break;
                case 7:
                    OR();
                    break;
                case 8:
                    XOR();
                    break;
                case 9:
                    SUB();
                    break;
                case 10:
                    ILDA();
                    break;
                case 11:
                    ISTA();
                    break;
                case 12:
                    IBUN();
                    break;
                case 13:
                    IADD();
                    break;
                }
            }

            else if (num_instruction >= 14 && num_instruction <= 25) // register reference instructions
            {
                switch (num_instruction)
                {
                case 14:
                    CLA();
                    break;
                case 15:
                    CLE();
                    break;
                case 16:
                    CMA();
                    break;
                case 17:
                    CME();
                    break;
                case 18:
                    CIR();
                    break;
                case 19:
                    CIL();
                    break;
                case 20:
                    INC();
                    break;
                case 21:
                    SPA();
                    break;
                case 22:
                    SNA();
                    break;
                case 23:
                    SZA();
                    break;
                case 24:
                    SZE();
                    break;
                case 25:
                    HLT();
                    break;
                }
            }
            else if (num_instruction >= 26 && num_instruction <= 29) // i/o instructions
            {
                switch (num_instruction)
                {
                case 26:
                    INP();
                    break;
                case 27:
                    OUT();
                    break;
                case 28:
                    SKI();
                    break;
                case 29:
                    SKO();
                    break;
                case 30:
                    ION();
                    break;
                case 31:
                    IOF();
                    break;

                default:
                    cerr << "Error: Invalid opcode " << opcode << endl;
                    break;
                }
            }
        }
    }

    void INP()
    {

        ac = inpr; // copy contents of INPR to AC

        fgi = 0; // clear FGI (input operation complete)
    }
    void OUT()
    {

        outr = ac; // copy contents of AC to OUTR

        fgo = 0; // clear FGO (output operation complete)
    }

    void SKI()
    {
        if (fgi == 1)
        {
            incpc();
        }
        sc = 0;
    }

    void SKO()
    {
        if (fgo == 1)
        {
            incpc();
        }
        sc = 0;
    }

    void ION()
    {
        ien = 1;
        sc = 0;
    }

    void IOF()
    {
        ien = 0; // enable interrupt flag
        sc = 0;  // clear sc (interrupt occurred)
    }

    void CLA()
    {
        ac = 0;
    }

    void SZE()
    {

        if ((ac & (1 << 16)) == 0)
        {
            // The 17th bit of the number is 0.
            pc++;
        }
        // else ignore
    }

    void HLT()
    {
        // sc = 0;
    }

    void SZA()
    {
        if (ac == 0)
            pc++;
    }

    void SNA()
    {

        if (ac < 0)
        {
            pc++;
        }
    }

    void SPA()
    {
        if (ac >= 0)
        {
            pc++;
        }
    }

    void INC()
    {
        ac++;
    }

    void CME()
    {
        ac *= -1;
    }

    void CMA()
    {
        ac = ac ^ 0xFFFF;
    }

    void CLE()
    {
        if (ac < 0)
            ac *= -1;
    }

    void CIL()
    {
        int temp = ac & 0x10000;
        // performing left shift
        ac <<= 1;
        // making 18th bit 0 in case E was 1
        ac &= ~(1u << 17);
        // Move the value of temp to the LSB
        ac |= (temp >> 16);
    }

    void CIR()
    {
        // storing lsb separately
        int temp = ac & 1;
        // performing right shift
        ac >>= 1;
        // making the actual msb of int ac 0
        ac &= 0x7FFFFFFF;
        // Move the value of temp to the MSB
        ac |= (temp << 16);
    }

    void display()
    {
        while (1)
        {
            int ch;
            cout << "Press: 1-display memory, 2-display registers 3-display flags 4-terminate loop\n";
            cin >> ch;
            if (ch == 1)
            {
                cout << "Press 1 to display all the instructions from 0 to HLT\n2- display a certain memory location\n";
                cin >> ch;

                if (ch == 1)
                {
                    for (int i = 0; i < 4096; i++)
                    {
                        cout << memory[i] << endl;

                        if (memory[i] == "HLT")
                            break;
                    }
                }

                else if (ch == 2)
                {
                    cout << "What memory location would you like to display?\n";
                    cin >> ch;

                    if (ch > 0 && ch < 4096)
                        cout << memory[ch] << endl;
                    else
                        cout << "out of range\n";
                }
            }
            else if (ch == 2)
            {
                cout << "DR: " << dr << endl;
                cout << "PC:" << pc << endl;
                cout << "AR: " << ar << endl;
                cout << "TR: " << tr << endl;
                cout << "AC: " << ac << endl;
                cout << "OUTR: " << outr << endl;
                cout << "INPR: " << inpr << endl;
                cout << "IR:" << ir << endl;
            }
            else if (ch == 3)
            {
                cout << "SC: " << sc << endl;
                cout << "IEN: " << ien << endl;
                cout << "FGO: " << fgo << endl;
                cout << "FGI: " << fgi << endl;
            }
            else
                break;
        }
    }
};

int main()
{
    simulator bmw;

    bmw.load_program();
    bmw.run_program();
    bmw.display();

    return 0;
}
