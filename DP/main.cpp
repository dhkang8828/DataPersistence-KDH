#include <iostream>
#include <windows.h>
#include "Database.h"

static void printAll(Database& db) {
    auto items = db.selectAll();
    if (items.empty()) {
        std::cout << "  (데이터 없음)\n";
        return;
    }
    std::cout << "  ID  | 품번         | 품명               | 수량  | 단가\n";
    std::cout << "  ----|--------------|--------------------|----- |----------\n";
    for (const auto& item : items) {
        printf("  %-4d| %-13s| %-19s| %-5d| %.2f\n",
            item.id, item.partNumber.c_str(), item.partName.c_str(),
            item.quantity, item.unitPrice);
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    try {
        Database db("inventory.db");
        db.createTable();

        std::cout << "\n=== INSERT ===\n";
        db.insertInventory({ 0, "SEM-001", "DRAM 16GB DDR5",  1000, 8500.0 });
        db.insertInventory({ 0, "SEM-002", "NAND Flash 512GB", 500, 12000.0 });
        db.insertInventory({ 0, "SEM-003", "AP Chip A17",      200, 35000.0 });

        std::cout << "\n=== SELECT ALL ===\n";
        printAll(db);

        std::cout << "\n=== UPDATE (SEM-001 수량 변경: 1000 -> 850) ===\n";
        db.updateQuantity(1, 850);
        printAll(db);

        std::cout << "\n=== DELETE (SEM-003 삭제) ===\n";
        db.deleteInventory(3);
        printAll(db);

    } catch (const std::exception& e) {
        std::cerr << "[오류] " << e.what() << "\n";
        return 1;
    }
    return 0;
}
