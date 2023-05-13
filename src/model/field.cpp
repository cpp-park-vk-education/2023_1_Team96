#include "model/field.hpp"

#include "field.hpp"

bool Field::CreateUnit(UnitType type, std::unique_ptr<IModel> model) {
    switch (type) {
        case B:
            std::shared_ptr<GameObject> object =
                std::std::make_shared<GameObject>(nullptr, std::move(model),
                                                  current_);

            UnitFactory uf(1, 1, 5, 1, 3);
            uf.AddObjectActions(object);

            return repo_.Add(object);
        case K:

            break;
    }
}

bool Field::DeleteUnit(sf::Vector2u pos) { return repo_.DeleteByCords(pos); }

void Field::SetCurrent(sf::Vector2i pos) {
    current_ = pos;
    model_->setCurrent(current_);
}

void Field::ResetCurrent() {
    current_ = {-1, -1};
    model_->resetCurrent();
}

sf::Vector2i Field::GetCurrent() { return current_; }

void Field::Draw() {
    model_->render();
    std::vector<shared_ptr<GameObject>> objects = repo_.GetAll();
    for (std::shared_ptr<GameObject> object : objects) object->Draw();
}
