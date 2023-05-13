#pragma once

#include "objectrepository.hpp"
#include "objectfactory.hpp"

class Field {
   private:
    IObjectRepository repo_;
    std::unique_ptr<IFieldModel> model_;
    sf::Vector2i current_ = {-1, -1};

   public:
    Field(IObjectRepository repository, std::unique_ptr<IFieldModel> f_model)
        : repo_(repository), model(std::move(f_model)) {}

    bool CreateUnit(UnitType type, std::unique_ptr<IModel> model);
    bool DeleteUnit(sf::Vector2u pos);

    void SetCurrent(sf::Vector2i pos);
    void ResetCurrent();
    sf::Vector2i GetCurrent();

    void Draw();
};
