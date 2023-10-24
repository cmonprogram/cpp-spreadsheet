#include "cell.h"

#include <cassert>
#include <iostream>
#include <string>
#include <optional>

Cell::Cell(SheetInterface& sheet) : sheet_(sheet)
{
    impl_ = std::make_unique<EmptyImpl>();
}

Cell::~Cell()
{
    impl_.~unique_ptr();
}

void Cell::Set(std::string text)
{
    if (text.size() > 0)
    {
        if (text.at(0) == FORMULA_SIGN)
        {
            if (text.size() > 1)
            {
                try
                {
                    impl_ = std::make_unique<FormulaImpl>(std::string(text.begin() + 1, text.end()), sheet_);
                }
                catch (FormulaException &e)
                {
                    throw e;
                }
            }
            else
            {
                impl_ = std::make_unique<TextImpl>(text);
            }
        }
        else
        {
           impl_ = std::make_unique<TextImpl>(text);
        }
    }
    else
    {
        impl_ = std::make_unique<EmptyImpl>();
    }
    
    //Add reference
    auto ref = impl_->GetReference();
    reference_cells_ = std::set<Position>(ref.begin(), ref.end());
    for(const Position& pos : reference_cells_){
        auto cell = sheet_.GetCell(pos);
        if(cell == nullptr){
            sheet_.SetCell(pos, "");
            sheet_.GetCell(pos)->AddRferenced(pos);
        }
    }
}

void Cell::Clear()
{
    impl_ = std::make_unique<EmptyImpl>();
}

Cell::Value Cell::GetValue() const
{
    /*
    if(cache_){
        return cache_.value();
    }else{
        auto result = impl_->GetValue();
        cache_ = result;
        return result;
    }
    */
   return impl_->GetValue();
}
std::string Cell::GetText() const
{
    return impl_->GetText();
}

std::vector<Position> Cell::GetReferencedCells() const 
{
    return std::vector<Position>(reference_cells_.begin(), reference_cells_.end());
}

void Cell::AddRferenced(const Position& ref) {
    cells_reference_.insert(ref);
}
