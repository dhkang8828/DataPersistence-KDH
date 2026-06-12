#include "Database.h"
#include "sqlite/sqlite3.h"
#include <stdexcept>
#include <iostream>

Database::Database(const std::string& dbPath) {
    if (sqlite3_open(dbPath.c_str(), &db_) != SQLITE_OK) {
        throw std::runtime_error("DB 열기 실패: " + std::string(sqlite3_errmsg(db_)));
    }
    std::cout << "[DB] 연결 성공: " << dbPath << "\n";
}

Database::~Database() {
    if (db_) {
        sqlite3_close(db_);
        std::cout << "[DB] 연결 종료\n";
    }
}

void Database::exec(const std::string& sql) {
    char* errMsg = nullptr;
    if (sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::string msg = errMsg;
        sqlite3_free(errMsg);
        throw std::runtime_error("SQL 실행 오류: " + msg);
    }
}

void Database::createTable() {
    exec(R"(
        CREATE TABLE IF NOT EXISTS inventory (
            id          INTEGER PRIMARY KEY AUTOINCREMENT,
            part_number TEXT    NOT NULL UNIQUE,
            part_name   TEXT    NOT NULL,
            quantity    INTEGER NOT NULL DEFAULT 0,
            unit_price  REAL    NOT NULL DEFAULT 0.0,
            updated_at  TEXT    NOT NULL DEFAULT (datetime('now','localtime'))
        );
    )");
    std::cout << "[DB] inventory 테이블 준비 완료\n";
}

bool Database::insertInventory(const Inventory& item) {
    const std::string sql =
        "INSERT INTO inventory (part_number, part_name, quantity, unit_price) "
        "VALUES ('" + item.partNumber + "', '" + item.partName + "', " +
        std::to_string(item.quantity) + ", " + std::to_string(item.unitPrice) + ");";

    try { exec(sql); return true; }
    catch (const std::exception& e) {
        std::cerr << "[INSERT 오류] " << e.what() << "\n";
        return false;
    }
}

bool Database::updateQuantity(int id, int newQuantity) {
    const std::string sql =
        "UPDATE inventory SET quantity=" + std::to_string(newQuantity) +
        ", updated_at=datetime('now','localtime') WHERE id=" + std::to_string(id) + ";";

    try { exec(sql); return sqlite3_changes(db_) > 0; }
    catch (const std::exception& e) {
        std::cerr << "[UPDATE 오류] " << e.what() << "\n";
        return false;
    }
}

bool Database::deleteInventory(int id) {
    const std::string sql =
        "DELETE FROM inventory WHERE id=" + std::to_string(id) + ";";

    try { exec(sql); return sqlite3_changes(db_) > 0; }
    catch (const std::exception& e) {
        std::cerr << "[DELETE 오류] " << e.what() << "\n";
        return false;
    }
}

std::vector<Inventory> Database::selectAll() {
    std::vector<Inventory> result;

    const char* sql = "SELECT id, part_number, part_name, quantity, unit_price, updated_at FROM inventory ORDER BY id;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK)
        throw std::runtime_error("SELECT 준비 실패");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Inventory item;
        item.id          = sqlite3_column_int(stmt, 0);
        item.partNumber  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        item.partName    = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        item.quantity    = sqlite3_column_int(stmt, 3);
        item.unitPrice   = sqlite3_column_double(stmt, 4);
        item.updatedAt   = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        result.push_back(item);
    }
    sqlite3_finalize(stmt);
    return result;
}

Inventory Database::selectById(int id) {
    const std::string sql =
        "SELECT id, part_number, part_name, quantity, unit_price, updated_at "
        "FROM inventory WHERE id=" + std::to_string(id) + ";";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        throw std::runtime_error("SELECT 준비 실패");

    Inventory item{};
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        item.id         = sqlite3_column_int(stmt, 0);
        item.partNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        item.partName   = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        item.quantity   = sqlite3_column_int(stmt, 3);
        item.unitPrice  = sqlite3_column_double(stmt, 4);
        item.updatedAt  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
    }
    sqlite3_finalize(stmt);
    return item;
}
