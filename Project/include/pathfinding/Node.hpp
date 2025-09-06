#pragma once

#include <cstdint>

struct Node {
    
    uint32_t x = 0;
    uint32_t y = 0;

    bool Walkable = true;
    bool Path = false;
    bool Visited = false;

    int FCost = -1;
    int GCost = -1;
    int HCost = -1;

    Node* Parent = nullptr;

    Node(uint32_t x_, uint32_t y_)
        : x(x_), y(y_)
    {
    }

    bool operator==(const Node& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Node& other) const {
        return !(*this == other);
    }

    void Reset() {
        Walkable = true;
        Path = false;
        Visited = false;

        FCost = -1;
        GCost = -1;
        HCost = -1;

        Parent = nullptr;
    }
};
