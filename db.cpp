#include "utils/utils.h"
#include "utils/ds.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    Table* table = new Table;
    std::string command = "";

    while (true) {
        print_prompt();

        getline(std::cin, command, '\n');

        if (command.length() == 0) {
            continue;
        }

        if (command[0] == '.') {
            switch (execute_meta_command(command)) {
            case (MetaCommandResult::META_COMMAND_SUCCESS):
                continue;
                
            case (MetaCommandResult::UNRECOGNIZED_META_COMMAND): {
                std::cerr << "Unrecognized meta command: '" << command << "'" << std::endl;
                continue;
            }   
            }
        }
        
        Statement statement;

        switch (prepare_statement(command, statement)) {
            case (PrepareStatementResult::PREPARE_STATEMENT_SUCCESS):
                break;
            
            case (PrepareStatementResult::UNRECOGNIZED_PREPARE_STATEMENT): {
                std::cerr << "Unrecognized statement: '" << command << "'" << std::endl;
                continue;
            }

            case (PrepareStatementResult::PREPARE_SYNTAX_ERROR): {
                std::cerr << "Syntax error. Could not parse the statement: '" << command << "'" << std::endl;
                continue;
            }
        }

        switch (execute_statement(statement, *table)) {
            case (ExecuteResult::EXECUTE_SUCCESS): {
                std::cout << "Executed. " << std::endl;
                break;
            }

            case (ExecuteResult::EXECUTE_TABLE_FULL): {
                std::cout << "Error: Table is full now. " << std::endl;
                break;
            }
        }
    }
}