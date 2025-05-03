//
// Created by Vismay on 5/2/2025.
//

#ifndef CURSOR_H
#define CURSOR_H
#include "SFML/System/Vector2.hpp"

namespace sft {
class Cursor {
public:
    /// The position of the cursor (x being the row, y being the column)
    int row{};

    int col{};

    /// Creates a cursor object with specified row and column
    /// @param row starting row of the cursor
    /// @param col starting column of the cursor
    Cursor(int row, int col);
};

} // sft

#endif //CURSOR_H
