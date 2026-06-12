#pragma once
#include <string>

struct Inventory {
    int         id;
    std::string partNumber;   // 품번
    std::string partName;     // 품명
    int         quantity;     // 수량
    double      unitPrice;    // 단가
    std::string updatedAt;    // 최종 수정일시
};
