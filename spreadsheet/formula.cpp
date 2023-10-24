#include "formula.h"

#include "FormulaAST.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <sstream>

using namespace std::literals;

std::ostream &operator<<(std::ostream &output, FormulaError fe)
{
    return output << "#DIV/0!";
}

std::unique_ptr<FormulaInterface> ParseFormula(std::string expression)
{
    try{
        return std::make_unique<Formula>(std::move(expression));
    }catch(...){
        throw FormulaException("");
    }
}

Formula::Formula(std::string expression) : ast_(ParseFormulaAST(expression)){}

Formula::Value Formula::Evaluate(SheetInterface& sheet) const
{
    try
    {
        return ast_.Execute(sheet);
    }
    catch (FormulaError &e)
    {
        return e;
    }
}
std::string Formula::GetExpression() const
{
    std::stringstream ss;
    ast_.PrintFormula(ss);
    return ss.str();
}

const std::vector<Position> Formula::GetReferencedCells() const{
    std::vector<Position> result(ast_.GetCells().begin(), ast_.GetCells().end());
    auto last = std::unique(result.begin(), result.end());
    result.erase(last, result.end());
    return result;
}

FormulaError::FormulaError(Category category) : category_(category){}
FormulaError::Category FormulaError::GetCategory() const{
    return category_;
}
bool FormulaError::operator==(FormulaError rhs) const {
    return category_ == rhs.GetCategory();
}
std::string_view FormulaError::ToString() const{
    if(category_ == FormulaError::Category::Div0){
        return "#DIV/0!";
    }
    if(category_ == FormulaError::Category::Ref){
        return "#REF!";
    }
    if(category_ == FormulaError::Category::Value){
        return "#VALUE!";
    }
    return "#UNKNOWN";
}
