#pragma once
#include "sqlite/sqlite3.h"
#include "Inventory.h"
#include <vector>
#include <string>

class Database {
public:
    explicit Database(const std::string& dbPath);
    ~Database();

    // DDL
    void createTable();

    // CRUD
    bool        insertInventory(const Inventory& item);
    bool        updateQuantity(int id, int newQuantity);
    bool        deleteInventory(int id);
    std::vector<Inventory> selectAll();
    Inventory   selectById(int id);

private:
    sqlite3* db_ = nullptr;

    void exec(const std::string& sql);
};
