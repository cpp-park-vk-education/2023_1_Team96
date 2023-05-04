#pragma once

#include "objectfactory.hpp"
#include "objectrepository.hpp"
#include "graphics/graphics.hpp"
#include "input/input_handler.hpp"

enum GameState
{
    PREPARE,
    STEP,
    WAIT,
};

class Game
{
private:
    std::unique_ptr<IObjectRepository> object_repo_;
    std::unique_ptr<IMonitor> monitor_;
    std::unique_ptr<InputHandler> handler_;
    std::shared_ptr<Player> first_player_;
    std::shared_ptr<Player> second_player_;
    GameState state;

    void CreateObject(std::shared_ptr<Player> player, sf::Vector2u top_left_cords,
                      IModel model, IObjectFactory* factory);

    void handleInput()
    {
        // обработка ввода и выполнение соответствующей команды
        handler_->Handle();
    }

    void update(){
        // обновление игрового состояния, не зависящее от ввода
    }

    void render()
    {
        monitor_->Prepare();
        // отрисовка моделей всех нужных объектов
        monitor_->Draw();
    }

public:
    Game(std::unique_ptr<IMonitor> monitor,
         std::unique_ptr<InputHandler> handler) : monitor_(std::move(monitor)),
                                                  handler_(std::move(handler))
    {}

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

    inline const std::unique_ptr<IObjectRepository> &Repository() const
    {
        return object_repo_;
    }
};
