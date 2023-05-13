#include "model/field.hpp"

#include "field.hpp"

bool Field::CreateUnit(UnitType type, std::unique_ptr<IModel> model) {
    return false;
}
bool Field::DeleteUnit(sf::Vector2u pos) { return false; }

void Field::SetCurrent(sf::Vector2i pos) {}

void Field::ResetCurrent() {}

sf::Vector2i Field::GetCurrent() { return sf::Vector2i(); }

void Field::Draw() {}

class Field : public IObjectRepository {
   private:
    vector<std::shared_ptr<GameObject>> objects;
    unsigned int h, w;
    std::unique_ptr<IFieldModel> model;
    sf::Vector2i current = {-1, -1};

   public:
    Field(sf::Vector2u size, std::unique_ptr<IFieldModel>&& f_model)
        : objects(size.x * size.y), model(std::move(f_model)) {
        h = size.x;
        w = size.y;
    }

    bool createUnit(UnitType type, std::unique_ptr<IModel> model) {
        sf::Vector2i pos = current;
        int index = pos.y * w + pos.x;
        std::cout << pos.x << std::endl;
        std::cout << pos.y << std::endl;
        std::cout << index << std::endl;

        if (index >= objects.size()) return false;

        if (pos.x >= 7) return false;

        if (objects[index] != nullptr) return false;

        switch (type) {
            case B:

                objects[index] = std::make_shared<GameObject>(std::move(model));
                objects[index]->AddAction(
                    ActionType::MOVE,
                    std::move(std::make_unique<MoveAction>(
                        *(objects[index]), sf::Vector2u{pos.x, pos.y}, 1)));

                std::cout << "created!" << std::endl;

                return true;

            case K:

                break;
        }
    };

    void deleteUnit(sf::Vector2u pos) override {
        objects[pos.y * w + pos.x] = nullptr;
    };

    void Add(std::unique_ptr<GameObject> obj){

    };

    std::shared_ptr<GameObject> GetByCords(sf::Vector2u cords){

    };

    void setCurrent(sf::Vector2i pos) override {
        current.x = pos.x;
        current.y = pos.y;
        model->setCurrent(current);
        std::cout << pos.x << std::endl;
    };

    void resetCurrent() override {
        sf::Vector2i res = current;
        current.x = -1;
        current.y = -1;
        model->resetCurrent();
    };

    sf::Vector2i getCurrent() override { return current; };

    std::shared_ptr<GameObject> GetByPlayer(Player player){};
    void Delete(std::shared_ptr<GameObject> obj){};
    void DeleteByCords(sf::Vector2u cords){};
    void draw() {
        model->render();
        for (std::shared_ptr<GameObject> object : objects) {
            if (object) object->draw();
        }
    };
};
