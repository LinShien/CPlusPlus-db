#ifndef _UTILS_H_
#define _UTILS_H_

#include "ds.h"
#include <string>

enum class ExecuteResult {
    EXECUTE_SUCCESS,
    EXECUTE_TABLE_FULL,
};

enum class MetaCommandResult {
    META_COMMAND_SUCCESS,
    UNRECOGNIZED_META_COMMAND
};

enum class PrepareStatementResult {
    PREPARE_STATEMENT_SUCCESS,
    UNRECOGNIZED_PREPARE_STATEMENT,
    PREPARE_SYNTAX_ERROR,
    PREPARE_STRING_OUT_OF_RANGE,
    PREPARE_ID_OUT_OF_RANGE,
    PREPARE_NEGATIVE_ID
};

enum class StatementType {
    STATEMENT_INSERT,
    STATEMENT_SELECT
};

struct Statement {
    StatementType type;
    Row row_to_insert;
};

void print_prompt();

MetaCommandResult execute_meta_command(std::string command);

PrepareStatementResult prepare_insert(const std::string& command, Statement& statement);
PrepareStatementResult prepare_statement(const std::string& command, Statement& statement);

Table* create_table();
void free_table(Table* table);

ExecuteResult execute_insert(const Statement& statement, Table& table);
ExecuteResult execute_select(const Statement& statement, const Table& table);
ExecuteResult execute_statement(const Statement& statement, Table& table);

void serialize_row(const Row* source, void* destination);
void deserialize_row(const void* source, Row* destination);

void* fetch_row(Table& table, int row_num);

#endif