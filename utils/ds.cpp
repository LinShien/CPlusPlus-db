#include "ds.h"
#include "utils.h"

#include <fstream>
#include <iostream>

Pager::Pager(std::string file_name) {
    fs.open(file_name, std::fstream::out | std::fstream::app);

    if (fs.fail()) {
        std::cout << "Unable to open file" << std::endl;
        exit(EXIT_FAILURE);
    }

    fs.seekg(0, fs.end);
    file_length = fs.tellg();
    fs.seekg(0, fs.beg);

    std::cout << fs.is_open() << std::endl;

    for (int i = 0; i < TABLE_MAX_PAGES; ++i) {
        pages[i] = nullptr;
    }
}

Pager::~Pager() {
    for (int i = 0; i < TABLE_MAX_PAGES; ++i) {
        delete (char*) pages[i];
        pages[i] = nullptr;
    }

    std::cout << "Pager destructor called" << std::endl;

    fs.flush();
    fs.close();
}

void Pager::flush(int page_num, unsigned int size) {
    fs.seekp(page_num * PAGE_SIZE);
    fs.write((char*) pages[page_num], size);
}

/**
 * Page 0 at offset 0, page 1 at offset 4096, page 2 at offset 8192, etc.
 * If the requested page lies outside the bounds of the file, we know 
 * it should be blank, so we just allocate some memory and return it.
 */

void* Pager::get_page(int page_num) {
    if (page_num > TABLE_MAX_PAGES) {
        std::cerr << "Tried to fetch page number out of bounds. " 
                  << page_num << " > " << TABLE_MAX_PAGES << std::endl;
        exit(EXIT_FAILURE);
    }

    if (pages[page_num] == nullptr) {         
        // Cache miss. Allocate memory and load more data from the filestream
        void* page = malloc(PAGE_SIZE);

        unsigned int num_pages = file_length / PAGE_SIZE;    // uint32_t

        if (file_length % PAGE_SIZE) {                       // a not full page
            num_pages += 1;
        }
 
        if (page_num <= num_pages) {    
            fs.seekg(page_num * PAGE_SIZE, std::ios::beg);
            fs.read((char*) page, PAGE_SIZE);
        }

        if (fs.eof()) {
            fs.clear();
        }
        
        pages[page_num] = page;
    }

    return pages[page_num];
}

Table::Table(std::string file_name) {      // database open
    pager = new Pager(file_name);
    num_rows = pager->file_length / ROW_SIZE;
}      

/**
 * database close and flush all the caches to the disk
 * , destructor will not be called after exit() called
 */
Table::~Table() {         
    std::cout << num_rows << std::endl;                  
    unsigned int num_full_pages = num_rows / ROWS_PER_PAGE;

    std::cout << "Table destructor called" << std::endl;

    for (unsigned int i = 0; i < num_full_pages; ++i) {
        if (pager->pages[i] == nullptr) {
            continue;
        }

        pager->flush(i, PAGE_SIZE);
    }

    // Last partial page
    unsigned int num_additional_rows = num_rows % ROWS_PER_PAGE;

    if (num_additional_rows) {
        if (pager->pages[num_full_pages] != nullptr) {
            pager->flush(num_full_pages, num_additional_rows * ROW_SIZE);
        }
    }
    
    delete pager;
}

