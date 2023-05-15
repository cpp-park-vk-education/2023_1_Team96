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
    WAIT,
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

    class CreateUnit : public ICommand {
       private:
        Game &game;
        sf::Vector2i pos;
        bool isMine;

       public:
        CreateUnit(Game &_game, bool _isMine = true)
            : game(_game), isMine(_isMine) {}

        void Execute(GameEvent event) override {
            game.field_->CreateUnit(
                event.unit_type, isMine,
                std::move(game.monitor_->getModel(ModelType::B_MODEL)));
            pos = game.field_->GetCurrent();
            game.last.push_back(this);
        }

        void Undo() override { game.field_->DeleteObject((sf::Vector2u)pos); }

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
            if (pos == game.field_->GetCurrent()) {
                game.field_->ResetCurrent();

                std::cout << "unchoose!" << std::endl;

                game.handler_->DeleteBindings(UNIT_NUM);
                return;
            }

            game.field_->SetCurrent(pos);
            std::cout << "choose!" << std::endl;

            switch (game.state) {
                case PREPARE:
                    game.handler_->AddBinding(EventType::UNIT_NUM,
                                              new CreateUnit(game, true));
                    break;

                case WAIT:
                    game.handler_->AddBinding(EventType::UNIT_NUM,
                                              new CreateUnit(game, false));
                    break;

                case STEP:

                    if (game.field_->GetObject(game.field_->GetCurrent()) !=
                        nullptr)
                        game.handler_->ChangeBinding(
                            EventType::CELL,
                            new MoveCommand(game, game.field_->GetCurrent()));
                    break;
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
            GameObject &obj = *game.field_->GetObject(to);
            if (obj.CanDoAction(ActionType::MOVE, (Vector2u)to) &&
                game.field_->MoveObject(from, to)) {
                obj.DoAction(ActionType::MOVE, (Vector2u)to);
                game.last.push_back(this);
                game.field_->ResetCurrent();
            }

            game.handler_->ChangeBinding(CELL, new ChooseCommand(game));
        }

        void Undo() override {
            if (!game.field_->MoveObject(to, from)) return;
            GameObject &obj = *game.field_->GetObject(from);
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
            game.handler_ = game.w_handler.get();
            game.state = STEP;
        }
    };

    class SendCommand : public ICommand {
       private:
        Game &game;

       public:
        SendCommand(Game &_game) : game(_game) {}

        void Execute(GameEvent event) override {
            std::cout << "send" << std::endl;
            game.state = WAIT;
            game.handler_->DeleteBindings(UNIT_NUM);

            std::string commands = "";

            for (ICommand *cmd : game.last) {
                commands += cmd->string();
                commands += " ";
            }

            commands += "e";

            game.n_handler->send(commands);
            game.handler_ = game.n_handler.get();
            game.handler_->AddBinding(CELL, new ChooseCommand(game));
            game.handler_->AddBinding(END, new ReturnCommand(game));

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
