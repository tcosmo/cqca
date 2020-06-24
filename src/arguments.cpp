#include "arguments.h"

const char doc[] = "Welcome to the simulator for the 2D Colatz Quasi Cellular Automaton.\nRefer to the Github repository for more info: https://github.com/tcosmo/simcqca.";
const char* argp_program_bug_address = "tristan.sterin@mu.ie";
const char* argp_program_version = VERSION_LITERAL;

void setInputType(const std::string& arg, Arguments& arguments, InputType inputType)
{
    static const char* modeName[5] = { "None", "line", "col", "border", "cycle" };
    if (arg.size() == 0) {
        printf("Input for mode `%s` should not be empty. Abort.", modeName[inputType]);
        return;
    }
    if (arguments.inputType != NONE) {
        printf("Only one input mode (line/col/border/cycle) should be chosen. Abort.");
        exit(0);
    }
    arguments.inputType = inputType;
    arguments.inputStr = arg;
}

const std::string& orStr(const std::string& one, const std::string& two)
{
    if (one.empty())
        return two;
    return one;
}

std::string getShortOptionStr(char c)
{
    std::string s = "-";
    s.push_back(c);
    return s;
}

std::string getLongOptionStr(const char* l)
{
    std::string s = "--";
    s.append(l);
    return s;
}

void usagePage()
{
    printf("Usage: ./%s ", simcqca_PROG_NAME);
    int iOption = 0;
    while( iOption < options.size() ) {
        printf(" [-%c", options[iOption].shortOption);
        if(options[iOption].argumentHelper)
            printf(" %s", options[iOption].argumentHelper);
        printf("] ");
        iOption += 1;
    }
    iOption = 0;
    while( iOption < options.size() ) {
        printf(" [--%s", options[iOption].longOption);
        if(options[iOption].argumentHelper)
            printf(" %s", options[iOption].argumentHelper);
        printf("] ");
        iOption += 1;
    }
    printf("\n");
}

void helpPage()
{
    printf("Usage ./%s [OPTION...]\n", simcqca_PROG_NAME);
    printf("%s\n\n", doc);
    int iOption = 0;
    while( iOption < options.size() ) {
        printf("   -%c,  --%s", options[iOption].shortOption, options[iOption].longOption);
        if(options[iOption].argumentHelper)
            printf(" %s", options[iOption].argumentHelper);
        printf("\n");
        printf("\t\t\t%s\n", options[iOption].helpString);
        iOption += 1;
    }
    printf("\n");
    printf("Report bugs to: %s\n", argp_program_bug_address);
    printf("\n");
}

void parseArguments(int argc, char* argv[], Arguments& arguments)
{
    InputParser input(argc, argv);
    if (input.cmdOptionExists("-s") || input.cmdOptionExists("--sequential")) {
        arguments.isSequential = true;
    }

    for (int iOption = 1; iOption < 5; iOption += 1) {
        std::string shortStr = getShortOptionStr(options[iOption].shortOption);
        std::string longStr = getLongOptionStr(options[iOption].longOption);
        if (input.cmdOptionExists(shortStr) || input.cmdOptionExists(longStr)) {
            setInputType(orStr(input.getCmdOption(shortStr), input.getCmdOption(longStr)), arguments, static_cast<InputType>(iOption));
        }
    }

    if (input.cmdOptionExists("-V") || input.cmdOptionExists("--version")) {
        printf("%s\n", argp_program_version);
        exit(0);
    }

    if (input.cmdOptionExists("-h") || input.cmdOptionExists("--help")) {
        helpPage();
        exit(0);
    }

    if (input.cmdOptionExists("-u") || input.cmdOptionExists("--usage")) {
        usagePage();
        exit(0);
    }
}
