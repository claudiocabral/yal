#include <string_view>
#include <parser.h>
#include <file.h>
#include <task_system/task_system.h>

int scripting_mode() {
    printf("One day we will be able invoke this as a script\n");
    return 0;
}

void generate_module(std::string_view filename, TokenList & tokens) {
    auto file = akura::File::open(filename);
    if (!file) { 
        printf("Failed to open file %.*s\n", static_cast<int>(filename.size()), filename.data());
        return;
    }
    lex(file->text, filename, tokens);
    parse(tokens, filename);
}

int app(int argc, char **argv)
{
    if (argc == 1)
        return scripting_mode();



    TaskSystem scheduler;
    std::vector<TokenList> token_lists(argc - 1);
    if (token_lists.size() != 1)
        std::terminate();
    int i = 0;
    while(--argc)
    {
        ++argv;
        scheduler.spawn([&, i = i, input_file = *argv]() {
                generate_module(input_file, token_lists[i]);
                });
        ++i;
    }
    scheduler.run();
    return 0;
}

int main(int argc, char **argv)
{
    return app(argc, argv);
}
