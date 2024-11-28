// Gabriel Ponce, Samuel Cook
// PROG2100
// 2024-11-14
// main.cpp
// The logic for the menu interface users will interact with.

#include "DerivedAccounts.h"
#include "AccountHolder.h"


#include <iostream>
#include <sqlite3.h>

static int selectQuery(void *data, int numColumns, char **columnValues, char **columnNames); // Function used for running selection queries.

// Main Function
int main() {

    sqlite3 *dbHandler; // Pointer variable used to carry out database interactions.
    int dbStatus; // Variable used to track the status of recently executed queries. Successful connections are 0s.

    dbStatus = sqlite3_open("bank_system.db", &dbHandler); // Attempt to open a database called bank_system.db. If it doesn't exist, make it.

    if (dbStatus != 0) { // Check to see if the database connection failed.
        std::cout << "An error occurred while trying to access the database.\nExiting..." << std::endl;
        system("pause");
        exit(0); // Exit the program.
    }

    // Create a variable that will hold a query used to create tables for the database.
    const char *createTables = R"(

        CREATE TABLE IF NOT EXISTS `account_types` (
            `type_id` int(3) NOT NULL AUTO_INCREMENT,
            `type` varchar(50) NOT NULL,
            PRIMARY KEY (`type_id`)
        );


        CREATE TABLE IF NOT EXISTS `users` (
            `user_id` int(11) NOT NULL AUTO_INCREMENT,
            `email` varchar(100) NOT NULL,
            `password` varchar(32) NOT NULL,
            `name` varchar(50) NOT NULL,
            `address` varchar(100) NOT NULL,
            `phone` varchar(14) NOT NULL,
            PRIMARY KEY (`user_id`)
        );

        CREATE TABLE IF NOT EXISTS `active_accounts` (
            `account_number` int(3) NOT NULL AUTO_INCREMENT,
            `user_id` int(11) NOT NULL,
            `type_id` int(11) NOT NULL,
            `balance` double NOT NULL,
            PRIMARY KEY (`account_number`)
        );

        );
    )";

    char *errorMessage = nullptr; // This is used to display the resulting error message (if there is an error).
    dbStatus = sqlite3_exec(dbHandler, createTables, 0, 0, &errorMessage); // this will return a 0 if the query executes successfully.
    // sqlite3_exec executes a query, it takes the database, the query, an error message, and some other shit as arguments.

    if (dbStatus != 0) {
        std::cout << "There was an error creating the table: " << errorMessage << std::endl;
    }
    else {
        std::cout << "Tables created successfully (or already exist)." << std::endl;
    }


    // // Here's how to add data from CPP files.
    // const char *sqlInsert = "INSERT INTO person (name, age, gender, country) VALUES "
    //                             "('Samuel Cook', 23, 'Male', 'Canada'),"
    //                             "('Al Bowlly', 126, 'Male', 'Mozambique'),"
    //                             "('Michael Myers', 72, 'Male', 'USA')";

    // dbStatus = sqlite3_exec(dbHandler, sqlInsert, 0, 0, &errorMessage);
    // if (dbStatus != 0) {
    //     std::cout << "There was an error adding data to the tables: " << errorMessage << std::endl;
    // }
    // else {
    //     std::cout << "Data inserted successfully!" << std::endl;
    // }

    // // Here's how to update data
    // const char *sqlUpdate = "UPDATE person SET country = 'CA' WHERE id = 1;";

    // dbStatus = sqlite3_exec(dbHandler, sqlUpdate, 0, 0, &errorMessage);
    // if (dbStatus != 0) {
    //     std::cout << "There was an error updating data in the tables: " << errorMessage << std::endl;
    // }
    // else {
    //     std::cout << "Data updated successfully!" << std::endl;
    // }

    // // Remember to make pointers null after they have been used, or free memory like the error messages when you are not using them.

    

    // // Here's how to select data for viewing.
    // const char *sqlSelect = "SELECT * FROM person WHERE id > 1;";

    // dbStatus = sqlite3_exec(dbHandler, sqlSelect, selectQuery, 0, &errorMessage); // The one parameter is filled with a function that handles displaying the results
    // if (dbStatus != SQLITE_OK) {
    //     std::cerr << "SQL Error: " << errorMessage << std::endl;
    // }
    // else {
    //     std::cout << "Data retrieved successfully." << std::endl;
    // }


    sqlite3_close(dbHandler);

    return 0;
}

static int selectQuery(void *data, int numColumns, char **columnValues, char **columnNames) {
    for (int i=0; i<numColumns; i++) {
        std::cout << columnNames[i] << ": " << (columnNames[i] ? columnValues[i] : "NULL") << std::endl;
    }
    return 0;
}
