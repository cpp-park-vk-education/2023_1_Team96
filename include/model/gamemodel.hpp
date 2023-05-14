#pragma once

#include <iostream>

#include "graphics/sfml_monitor.hpp"
#include "input/sfml_input.hpp"
#include "model/field.hpp"

using sf::Vector2i;
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

       public:
        ChooseCommand(Game &_game) : game(_game) {}

        void Execute(GameEvent event) override {
            sf::Vector2i pos = sf::Vector2i{event.cords.x, event.cords.y};
            if (pos == game.field_->getCurrent()) {
                game.field_->resetCurrent();

                std::cout << "unchoose!" << std::endl;

                game.handler_->DeleteBindings(UNIT_NUM);
                return;
            }

            game.field_->setCurrent(pos);
            std::cout << "choose!" << std::endl;

            switch (game.state) {
                case PREPARE:
                    game.handler_->AddBinding(EventType::UNIT_NUM,
                                              new CreateUnit(game));
                    break;

                case STEP:

                    if (game.field_->getObject(game.field_->getCurrent()) !=
                        nullptr)
                        game.handler_->ChangeBinding(
                            EventType::CELL,
                            new MoveCommand(game, game.field_->getCurrent()));
                    break;
            }
        }

        void Undo() override {}
    };

    class CancelCommand : public ICommand {
       private:
        Game &game;

       public:
        CancelCommand(Game &_game) : game(_game) {}

        void Execute(GameEvent event) override {
            if (game.last.size() != 0) {
                game.last.back()->Undo();
                delete game.last.back();
                game.last.pop_back();
            }
        }

        void Undo() override {}
    };

    class MoveCommand : public ICommand {
       private:
        Game &game;
        Vector2i from;
        Vector2i to;

       public:
        MoveCommand(Game &_game, Vector2i _from) : game(_game), from(_from) {}

        void Execute(GameEvent event) override {
            to = event.cords;
            if (!game.field_->change(from, to)) return;

            GameObject &obj = *game.field_->getObject(to);

            obj.DoAction(ActionType::MOVE, (Vector2u)to);

            game.last.push_back(this);

            game.field_->resetCurrent();

            game.handler_->ChangeBinding(CELL, new ChooseCommand(game));
        }

        void Undo() override {
            if (!game.field_->change(to, from)) return;

            GameObject &obj = *game.field_->getObject(from);

            obj.DoAction(ActionType::MOVE, (Vector2u)from);
        }
    };

    class SendCommand : public ICommand {
       private:
        Game &game;

       public:
        SendCommand(Game &_game) : game(_game) {}

        void Execute(GameEvent event) override { 
            std::cout << "send" << std::endl; 
            game.state = STEP; 
            game.handler_->DeleteBindings(UNIT_NUM);
            game.last.clear();
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
