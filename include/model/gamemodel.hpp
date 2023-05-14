#pragma once

#include <iostream>

#include "graphics/sfml_monitor.hpp"
#include "input/sfml_input.hpp"
#include "model/field.hpp"

using sf::Vector2u;
using std::move;
using std::shared_ptr;
using std::unique_ptr;

enum GameState {
    PREPARE,
    STEP,
    WAIT,
};

class Game {
   private:
    unique_ptr<Field> field_;
    unique_ptr<SFMLWindow> monitor_;
    unique_ptr<SFMLWindowHandler> handler_;
    vector<ICommand *> last;
    GameState state;

    class CreateUnit : public ICommand {
       private:
        Game &game;
        sf::Vector2i last;

       public:
        CreateUnit(Game &_game) : game(_game) {}

        void Execute(GameEvent event) override {
            game.field_->createUnit(
                event.unit_type,
                std::move(game.monitor_->getModel(ModelType::B_MODEL)));
            last = game.field_->getCurrent();
            game.last.push_back(this);
        }

        void Undo() override { game.field_->deleteUnit((sf::Vector2u)last); }
    };

    class ChooseCommand : public ICommand {
       private:
        Game &game;
        sf::Vector2i last = {-1, -1};

       public:
        ChooseCommand(Game &_game) : game(_game) {}

        void Execute(GameEvent event) override {
            sf::Vector2i pos = sf::Vector2i{event.cords.x, event.cords.y};
            if (pos == game.field_->getCurrent()) {
                last = pos;
                game.field_->resetCurrent();

                std::cout << "unchoose!" << std::endl;

                game.handler_->DeleteBindings(UNIT_NUM);
                return;
            }

            last = game.field_->getCurrent();
            game.field_->setCurrent(pos);
            std::cout << "choose!" << std::endl;

            game.handler_->AddBinding(EventType::UNIT_NUM,
                                      new CreateUnit(game));
        }

        void Undo() override {
            if (last.x == -1) {
                game.field_->resetCurrent();
                last = {-1, -1};
            } else {
                game.field_->setCurrent(last);
                last = {-1, -1};
            }
        }
    };

    class CancelCommand : public ICommand {
       private:
        Game &game;

       public:
        CancelCommand(Game &_game) : game(_game) {}

        void Execute(GameEvent event) override {
            if (game.last.size() != 0) {
                game.last.back()->Undo();
                game.last.pop_back();
            }
        }

        void Undo() override {}
    };

    class MoveCommand : public ICommand {
       private:
        Game &game;

       public:
        MoveCommand(Game &_game) : game(_game) {}

        void Execute(GameEvent event) override {
            
        }

        void Undo() override {}
    };

    void handleInput() { handler_->Handle(); }

    void update() {}

    void render();

   public:
    
    Game(unique_ptr<SFMLWindow> monitor, unique_ptr<SFMLWindowHandler> handler);

    void StartGame();
};
