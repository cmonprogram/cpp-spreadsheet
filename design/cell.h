#pragma once

#include "common.h"
#include "formula.h"

#include <functional>
#include <unordered_set>
#include <set>
#include <optional>

class Sheet;

class Cell : public CellInterface {
public:
    Cell(Sheet& sheet);
    ~Cell();

    void Set(std::string text);
    void Clear();

    Value GetValue() const override;
    std::string GetText() const override;
    std::vector<Position> GetReferencedCells() const override;

    bool IsReferenced() const;

private:
    class Impl;
    class EmptyImpl;
    class TextImpl;
    class FormulaImpl;

    std::unique_ptr<Impl> impl_;

	std::set<Position> reference_cells_ //Зависимости ячейки
	std::set<Position> cells_reference_ //Ячейки, которые завият от данной
	mutable std::optional<double> cache_; //Кэш ячейки
	Sheet& sheet_; //Таблица
};
