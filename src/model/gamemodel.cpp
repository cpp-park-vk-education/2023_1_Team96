#include "model/gamemodel.hpp"

Game::Game(unique_ptr<SFMLWindow> monitor,
           unique_ptr<SFMLWindowHandler> _w_handler,
           unique_ptr<NetworkHandler> _n_handler)
    : monitor_(move(monitor)),
      w_handler(move(_w_handler)),
      n_handler(move(_n_handler)) {
    uint rows = 9;
    uint cols = 15;
    unique_ptr<SFMLFieldModel> field_model =
        monitor_->getFieldModel(rows, cols);
    field_ = std::make_unique<Field>(rows, cols, move(field_model));

    setState(GameState::PREPARE);
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

    field_->Draw();

    monitor_->Draw();
}

void Game::setState(GameState state) {
    switch (state) {
        case PREPARE:

            handler_ = w_handler.get();

            handler_->AddBinding(EventType::CELL, new ChooseCommand(*this));
            handler_->AddBinding(EventType::UNIT_NUM,
                                 new CreateUnit(*this, true));
            handler_->AddBinding(EventType::CANCEL, new CancelCommand(*this));
            handler_->AddBinding(EventType::SEND, new SendCommand(*this));

            state = PREPARE;
            break;

        case WAIT_FOR_POS:

            handler_ = n_handler.get();

            handler_->AddBinding(CELL, new ChooseCommand(*this));
            handler_->AddBinding(EventType::UNIT_NUM,
                                 new CreateUnit(*this, false));
            handler_->AddBinding(END, new ReturnCommand(*this));
            state = WAIT_FOR_POS;
            break;

        case STEP:

            handler_ = w_handler.get();
            handler_->DeleteBinding(UNIT_NUM);
            handler_->AddBinding(CELL, new ChooseCommand(*this));
            state = STEP;
            break;

        default:
            break;
    }
}