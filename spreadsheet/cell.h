#pragma once

#include "common.h"
#include "formula.h"

#include <functional>
#include <unordered_set>
#include <set>
#include <optional>

//class Sheet;

class Cell : public CellInterface
{
public:
    Cell(SheetInterface &sheet);
    ~Cell();

    void Set(std::string text);
    void Clear();

    Value GetValue() const override;
    std::string GetText() const override;
    std::vector<Position> GetReferencedCells() const override;
    void AddRferenced(const Position& ref) override;

    bool IsReferenced() const;
    //void UpdateReference();
    bool CheckCircularDeps(std::set<Position>& visited){
        for(const Position& pos : reference_cells_){
            if(visited.count(pos)) return false;
            visited.insert(pos);
            return sheet_.GetCell(pos)->CheckCircularDeps(visited);
        }
        return true;
    }

private:
    class Impl
    {
    public:
        virtual Cell::Value GetValue() = 0;
        virtual std::string GetText() = 0;
        virtual std::vector<Position> GetReference() = 0;
    };

    class EmptyImpl : public Impl
    {
    public:
        Cell::Value GetValue()
        {
            return "";
        }
        std::string GetText()
        {
            return "";
        }
        std::vector<Position> GetReference(){
            return {};
        }
    };
    class TextImpl : public Impl
    {
    public:
        TextImpl(std::string inp) : val(inp) {}
        Cell::Value GetValue()
        {
            if (val.size() > 0)
            {
                if (val.at(0) == ESCAPE_SIGN)
                {
                    return std::string(val.begin() + 1, val.end());
                }
                else
                {
                    return val;
                }
            }
            return val;
        }
        std::string GetText()
        {
            return val;
        }
        std::vector<Position> GetReference(){
            return {};
        }

    private:
        std::string val;
    };
    class FormulaImpl : public Impl
    {
    public:
        FormulaImpl(std::string inp, SheetInterface &sheet) : sheet_(sheet), formula(inp){}
        Cell::Value GetValue()
        {
            auto result = formula.Evaluate(sheet_);
            if (result.index() == 0)
            {
                return std::get<0>(result);
            }
            else
            {
                return std::get<1>(result);
            }
        }
        std::string GetText()
        {
            return FORMULA_SIGN + formula.GetExpression();
        }
        std::vector<Position> GetReference()
        {
            return formula.GetReferencedCells();
        }
    private:
        SheetInterface &sheet_;    
        Formula formula;
    };


    std::unique_ptr<Impl> impl_;


    mutable std::optional<Cell::Value> cache_; // Кэш ячейки
    SheetInterface &sheet_;               //Таблица
    std::set<Position> reference_cells_;  // Ячейки, от которых зависит данная
    std::set<Position> cells_reference_;  // Ячейки, которые завият от данной
};