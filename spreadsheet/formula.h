#pragma once
#include "common.h"
#include "FormulaAST.h"

#include <memory>
#include <variant>

// Формула, позволяющая вычислять и обновлять арифметическое выражение.
// Поддерживаемые возможности:
// * Простые бинарные операции и числа, скобки: 1+2*3, 2.5*(2+3.5/7)
class FormulaInterface
{
public:
    using Value = std::variant<double, FormulaError>;

    virtual ~FormulaInterface() = default;

    // Возвращает вычисленное значение формулы либо ошибку. На данном этапе
    // мы создали только 1 вид ошибки -- деление на 0.
    virtual Value Evaluate(SheetInterface& sheet) const = 0;

    // Возвращает выражение, которое описывает формулу.
    // Не содержит пробелов и лишних скобок.
    virtual std::string GetExpression() const = 0;
    
    // Возвращает список ячеек, которые непосредственно задействованы в данной
    // формуле. Список отсортирован по возрастанию и не содержит повторяющихся
    // ячеек. В случае текстовой ячейки список пуст.
    virtual const std::vector<Position> GetReferencedCells() const = 0;
};

// Парсит переданное выражение и возвращает объект формулы.
// Бросает FormulaException в случае, если формула синтаксически некорректна.
std::unique_ptr<FormulaInterface> ParseFormula(std::string expression);

class Formula : public FormulaInterface
{
public:
    explicit Formula(std::string expression);
    Value Evaluate(SheetInterface& sheet) const override;
    std::string GetExpression() const override;
    const std::vector<Position> GetReferencedCells() const override;
private:
    FormulaAST ast_;
};