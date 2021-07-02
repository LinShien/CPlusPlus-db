#include "ds.h"

Table::Table() {
    num_rows = 0;

    for (int i = 0; i < TABLE_MAX_PAGES; ++i) {
        pages[i] = nullptr;
    }
}

Table::~Table() {
    for (int i = 0; i < TABLE_MAX_PAGES; ++i) {
        delete (char*) pages[i];
    }
}