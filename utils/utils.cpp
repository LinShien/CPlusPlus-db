#include "utils.h"
#include <iostream>
#include <memory>

void print_prompt() {
    std::cout << "c++db > ";
}

void print_row(const Row& row) {
    std::cout << "(" << row.id << ", " << row.username << ", " << row.email << ")" << std::endl;
}

MetaCommandResult execute_meta_command(std::string command) {
    if (command.compare(".exit") == 0 || command.compare(".EXIT") == 0) {
        exit(EXIT_SUCCESS);
    } else {
        return MetaCommandResult::UNRECOGNIZED_META_COMMAND;
    }

    return MetaCommandResult::META_COMMAND_SUCCESS;
}

// SQL Compiler
PrepareStatementResult prepare_statement(std::string command, Statement& statement) {  
    if (command.compare(0, 6, "insert") == 0) {
        statement.type = StatementType::STATEMENT_INSERT;

        int args = sscanf(command.c_str(), "insert %d %s %s"
                                    , &(statement.row_to_insert.id)
                                    , statement.row_to_insert.username
                                    , statement.row_to_insert.email);

        if (args < 3) {
            return PrepareStatementResult::PREPARE_SYNTAX_ERROR;
        }                          

        return PrepareStatementResult::PREPARE_STATEMENT_SUCCESS;

    } else if (command.compare(0, 6, "select") == 0) {
        statement.type = StatementType::STATEMENT_SELECT;
        return PrepareStatementResult::PREPARE_STATEMENT_SUCCESS;
    }

    return PrepareStatementResult::UNRECOGNIZED_PREPARE_STATEMENT;
}

ExecuteResult execute_insert(const Statement& statement, Table& table) {
    if (table.num_rows >= TABLE_MAX_ROWS) {
        return ExecuteResult::EXECUTE_TABLE_FULL;
    }

    // insert into table
    serialize_row(&(statement.row_to_insert), fetch_row(table, table.num_rows));
    table.num_rows++;

    return ExecuteResult::EXECUTE_SUCCESS;
}

ExecuteResult execute_select(const Statement& statement, Table& table) {
    Row row;

    for (int i = 0; i < table.num_rows; ++i) {
        deserialize_row(fetch_row(table, i), &row);
        print_row(row);
    }

    return ExecuteResult::EXECUTE_SUCCESS;
}

// virtual machine
ExecuteResult execute_statement(const Statement& statement, Table& table) {
  switch (statement.type) {
    case (StatementType::STATEMENT_INSERT): {
        return execute_insert(statement, table);  
    }
      
    case (StatementType:: STATEMENT_SELECT): {
      return execute_select(statement, table);  
    }
  }

  return ExecuteResult::EXECUTE_SUCCESS;
}

void serialize_row(const Row* source, void* destination) {
    memcpy(((char*) destination) + ID_OFFSET, &(source->id), ID_SIZE);
    memcpy(((char*) destination) + USERNAME_OFFSET, &(source->username), USERNAME_SIZE);
    memcpy(((char*) destination) + EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
}

void deserialize_row(const void* source, Row* desination) {
    memcpy(&(desination->id), ((char*) source) + ID_OFFSET, ID_SIZE);
    memcpy(&(desination->username), ((char*) source) + USERNAME_OFFSET, USERNAME_SIZE);
    memcpy(&(desination->email), ((char*) source) + EMAIL_OFFSET, EMAIL_SIZE);
}

void* fetch_row(Table& table, int row_num) {
    int page_num = row_num / ROWS_PER_PAGE;
    void* page = table.pages[page_num];

    if (page == nullptr) {
        table.pages[page_num] = malloc(PAGE_SIZE);
        page = table.pages[page_num];
    }

    int row_offset = row_num % ROWS_PER_PAGE;     
    int byte_offset = row_offset * ROW_SIZE;

    return ((char*) page) + byte_offset;                   // offset in whole table 
}