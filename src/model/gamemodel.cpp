#include "model/gamemodel.hpp"

Game::Game(unique_ptr<SFMLWindow> monitor,
           unique_ptr<SFMLWindowHandler> _w_handler,
           unique_ptr<NetworkHandler> _n_handler)
    : monitor_(move(monitor)),
      w_handler(move(_w_handler)),
      n_handler(move(_n_handler)),
      state(GameState::PREPARE) {
    uint rows = 9;
    uint cols = 15;
    unique_ptr<SFMLFieldModel> field_model =
        monitor_->getFieldModel(rows, cols);
    field_ = std::make_unique<Field>(rows, cols, move(field_model));

    handler_ = w_handler.get();

    handler_->AddBinding(EventType::CELL, new ChooseCommand(*this));
    handler_->AddBinding(EventType::CANCEL, new CancelCommand(*this));
    handler_->AddBinding(EventType::SEND, new SendCommand(*this));
}

void Game::StartGame() {
    while (!monitor_->isEnd()) {
        handleInput();
        update();
        render();
    }
}

void Game::render() {
    monitor_->Prepare();

    field_->draw();

    monitor_->Draw();
}