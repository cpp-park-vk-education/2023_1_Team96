#pragma once

#include <iostream>
#include "objectfactory.hpp"
#include "objectrepository.hpp"
#include "graphics/graphics.hpp"
#include "input/input_handler.hpp"

using sf::Vector2u;
using std::move;
using std::shared_ptr;
using std::unique_ptr;

enum GameState
{
    PREPARE,
    STEP,
    WAIT,
};

class Game
{
private:
    unique_ptr<IObjectRepository> object_repo_;
    unique_ptr<IMonitor> monitor_;
    unique_ptr<InputHandler> handler_;
    shared_ptr<Player> first_player_;
    shared_ptr<Player> second_player_;
    ICommand *last;
    GameState state;

    class CreateUnit : public ICommand
    {
    private:
        Game &game;
        sf::Vector2i last;
    public:
        CreateUnit(Game &_game) : game(_game) {}

        void Execute(GameEvent event) override
        {
            game.object_repo_->createUnit(event.unit_type, std::move(game.monitor_->getModel(ModelType::B_MODEL)));
            last = game.object_repo_->getCurrent();
            game.last = this;
        }

        void Undo() override 
        {
            game.object_repo_->deleteUnit((sf::Vector2u)last);
        }
    };

    class ChooseCommand : public ICommand
    {
    private:
        Game &game;
        sf::Vector2i last = {-1, -1};

    public:
        ChooseCommand(Game &_game) : game(_game) {}

        void Execute(GameEvent event) override
        {
            sf::Vector2i pos = sf::Vector2i{event.cords.x / 63, event.cords.y / 63};
            if (pos == game.object_repo_->getCurrent())
            {
                last = pos;
                game.object_repo_->resetCurrent();
                std::cout << "unchoose!" << std::endl;

                game.handler_->DeleteBindings(UNIT_NUM);

                game.last = this;
                return;
            }

            last = {-1, -1};
            game.object_repo_->setCurrent(pos);
            std::cout << "choose!" << std::endl;

            game.handler_->AddBinding(EventType::UNIT_NUM, new CreateUnit(game));

            game.last = this;
        }

        void Undo() override
        {
            if (last.x == -1)
            {
                game.object_repo_->resetCurrent();
                last = {-1, -1};
            }
            else
            {
                game.object_repo_->setCurrent(last);
                last = {-1, -1};
            }

            game.last = nullptr;
        }
    };

    class CancelCommand : public ICommand
    {
    private:
        Game &game;

    public:
        CancelCommand(Game &_game) : game(_game) {}

        void Execute(GameEvent event) override
        {
            if (game.last)
                game.last->Undo();
        }

        void Undo() override {}
    };

    void
    handleInput()
    {
        handler_->Handle();
    }

    void update()
    {
    }

    void render()
    {
        monitor_->Prepare();

        object_repo_->draw();

        monitor_->Draw();
    }

public:
    Game(unique_ptr<IMonitor> monitor,
         unique_ptr<InputHandler> handler) : monitor_(move(monitor)),
                                             handler_(move(handler)),
                                             state(GameState::PREPARE)
    {
        object_repo_ = std::make_unique<Field>(Vector2u{9, 15}, move(monitor_->getFieldModel(Vector2u{5, 6})));
        handler_->AddBinding(EventType::CELL, new ChooseCommand(*this));
        handler_->AddBinding(EventType::CANCEL, new CancelCommand(*this));
        // handler_->AddBinding()
    }

    void StartGame()
    {
        // игровой цикл
        while (!monitor_->isEnd())
        {
            handleInput();
            update();
            render();
        }
    }

    inline const unique_ptr<IObjectRepository> &Repository() const
    {
        return object_repo_;
    }
};
