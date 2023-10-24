#include "sheet.h"

#include "cell.h"
#include "common.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <optional>

using namespace std::literals;

Sheet::~Sheet() {}

void Sheet::SetCell(Position pos, std::string text)
{
    if (!pos.IsValid())
        throw InvalidPositionException("");

    auto cell = std::make_unique<Cell>(*this);
    cell->Set(text);
    
    //check Circular
    //ParseFormulaAST(expression)
    std::set<Position> visited;
    visited.insert(pos);
    if(!cell->CheckCircularDeps(visited)){ 
        throw CircularDependencyException("");
    }
    table[pos] = std::move(cell);
}

const CellInterface *Sheet::GetCell(Position pos) const
{
    if (!pos.IsValid())
        throw InvalidPositionException("");

    if (auto it = table.find(pos); it != table.end())
    {
        return &*(it->second);
    }
    else
    {
        return nullptr;
    }
}

CellInterface *Sheet::GetCell(Position pos)
{
    if (!pos.IsValid())
        throw InvalidPositionException("");

    if (auto it = table.find(pos); it != table.end())
    {
        return &*(it->second);
    }
    else
    {
        return nullptr;
    }
}

void Sheet::ClearCell(Position pos)
{
    if (!pos.IsValid())
        throw InvalidPositionException("");
    table.erase(pos);
}

Size Sheet::GetPrintableSize() const
{
    int max_row = 0;
    int max_col = 0;
    for (const auto &cell : table)
    {
        if (cell.first.row + 1 > max_row)
            max_row = cell.first.row + 1;
        if (cell.first.col + 1 > max_col)
            max_col = cell.first.col + 1;
    }
    return {max_row, max_col};
}

void Sheet::PrintValues(std::ostream &output) const
{
    Size size = GetPrintableSize();
    for (int x = 0; x < size.rows; ++x)
    {
        for (int y = 0; y < size.rows; ++y)
        {
            if (y != 0)
                output << '\t';
            auto pos = Position{x, y};
            if (table.count(pos))
            {
                std::visit(
                    [&](const auto &inp)
                    {
                        output << inp;
                    },
                    table.at(pos)->GetValue());
            }
        }
        output << '\n';
    }
}
void Sheet::PrintTexts(std::ostream &output) const
{
    Size size = GetPrintableSize();
    for (int x = 0; x < size.rows; ++x)
    {
        for (int y = 0; y < size.rows; ++y)
        {
            if (y != 0)
                output << '\t';
            auto pos = Position{x, y};
            if (table.count(pos))
            {
                output << table.at(pos)->GetText();
            }
        }
        output << '\n';
    }
}

std::unique_ptr<SheetInterface> CreateSheet()
{
    return std::make_unique<Sheet>();
}
