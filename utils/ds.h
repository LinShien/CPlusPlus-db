#ifndef _DS_H_
#define _DS_H_

const int COLUMN_USERNAME_SIZE = 32;
const int COLUMN_EMAIL_SIZE = 255;

/**
 * Simple and hardcoded row represent data structure
 * column	        type
 * 
 * id	            integer
 * username	        varchar(32)
 * email	        varchar(255)
 */
struct Row {
    int id;
    char username[COLUMN_USERNAME_SIZE];
    char email[COLUMN_EMAIL_SIZE];
};

/**
 *  column	  size (bytes)	 offset
 * 
 *  id	      4	             0
 *  username  32             4
 *  email	  255	         36
 *  total	  291
 */

const int ID_SIZE = sizeof(((Row*) 0)->id);
const int USERNAME_SIZE = sizeof(((Row*) 0)->username);
const int EMAIL_SIZE = sizeof(((Row*) 0)->email);
const int ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

const int ID_OFFSET = 0;
const int USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const int EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;

const int PAGE_SIZE = 4096;           // same as big as page in virtual memory
const int TABLE_MAX_PAGES = 100;
const int ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
const int TABLE_MAX_ROWS = TABLE_MAX_PAGES * ROWS_PER_PAGE;

/** 
 * A data structure to represent Table in database
 * For simplicity, We now store all the rows into pages represented as array instead of B-tree which is a very practical way 
 */
class Table {
    public:
        unsigned int num_rows;
        void* pages[TABLE_MAX_PAGES];

        Table();
        ~Table();       
};

// struct Table {
//     unsigned int num_rows;
//     void* pages[TABLE_MAX_PAGES];
// };

#endif