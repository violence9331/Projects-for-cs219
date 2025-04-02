#include "calculator.h"

int main(int argc, char* argv[]) {
    if (argc == 1) {
        // 交互模式
        interactive_mode();
    } else {
        // 命令行模式
        handle_command_line(argc, argv);
    }
    return 0;
}