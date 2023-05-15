#pragma once

#include <iostream>
#include <sstream>

#include "graphics/sfml_monitor.hpp"
#include "input/network_input.hpp"
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
    WAIT_FOR_POS,
};

class Game {
   private:
    unique_ptr<Field> field_;
    unique_ptr<SFMLWindow> monitor_;
    unique_ptr<SFMLWindowHandler> w_handler;
    unique_ptr<NetworkHandler> n_handler;
    InputHandler *handler_;
    vector<ICommand *> last;
    GameState state;

    void setState(GameState state);

    class CreateUnit : public ICommand {
       private:
        Game &game;
        sf::Vector2i pos;
        bool isMine;

       public:
        CreateUnit(Game &_game, bool _isMine = true)
            : game(_game), isMine(_isMine) {}

        void Execute(GameEvent event) override {
            game.field_->createUnit(
                event.unit_type, isMine,
                std::move(game.monitor_->getModel(ModelType::B_MODEL)));
            pos = game.field_->getCurrent();
            game.last.push_back(this);

            game.handler_->ChangeBinding(UNIT_NUM, new CreateUnit(game, isMine));
        }

        void Undo() override { game.field_->deleteUnit((sf::Vector2u)pos); }

        std::string string() {
            std::ostringstream ostrm;
            ostrm << "c b " << (14 - pos.x) << " " << pos.y;
            return ostrm.str();
        }
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
                return;
            }

            game.field_->setCurrent(pos);
            std::cout << "choose!" << std::endl;
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
                delete game.last.back();
                game.last.pop_back();
            }
        }
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
            if (game.field_->change(from, to)) {
                GameObject &obj = *game.field_->getObject(to);

                obj.DoAction(ActionType::MOVE, (Vector2u)to);

                game.last.push_back(this);

                game.field_->resetCurrent();
            }

            game.handler_->ChangeBinding(CELL, new ChooseCommand(game));
        }

        void Undo() override {
            if (!game.field_->change(to, from)) return;

            GameObject &obj = *game.field_->getObject(from);

            obj.DoAction(ActionType::MOVE, (Vector2u)from);
        }
    };

    class ReturnCommand : public ICommand {
       private:
        Game &game;

       public:
        ReturnCommand(Game &_game) : game(_game) {}
        void Execute(GameEvent event) {
            std::cout << "return" << std::endl;
            game.setState(STEP);
        }
    };

    class SendCommand : public ICommand {
       private:
        Game &game;

       public:
        SendCommand(Game &_game) : game(_game) {}

        void Execute(GameEvent event) override {
            std::cout << "send" << std::endl;

            std::string commands = "";

            for (ICommand *cmd : game.last) {
                commands += cmd->string();
                commands += " ";
            }

            commands += "e";

            game.n_handler->send(commands);

            game.setState(GameState::WAIT_FOR_POS);

            game.last.clear();
        }
    };

    void handleInput() { handler_->Handle(); }

    void update() {}

    void render();

   public:
    Game(unique_ptr<SFMLWindow> monitor,
         unique_ptr<SFMLWindowHandler> _w_handler,
         unique_ptr<NetworkHandler> _n_handler);

    void StartGame();
};
