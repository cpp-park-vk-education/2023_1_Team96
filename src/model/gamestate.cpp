#include "model/gamestate.hpp"

#include <iostream>

using std::cout, std::endl;

Game::Game(unique_ptr<SFMLWindow> monitor, unique_ptr<InputHandler> handler)
    : monitor_(std::move(monitor)),
      handler_(std::move(handler)),
      state_(State::PREPARE),
      cell_(-1, 1),
      obj_(nullptr),
      Turn(true) {
    uint rows = 9;
    uint cols = 15;
    unique_ptr<SFMLFieldModel> field_model =
        monitor_->getFieldModel(rows, cols);
    field_ = std::make_unique<Field>(rows, cols, move(field_model));
}

void Game::StartGame() {
    while (!monitor_->isEnd()) {
        HandleInput();
        Update();
        Render();
    }
}

void Game::HandleInput() {
    GameEvent ev = handler_->Handle();
    State new_state = State::ERROR;
    if (ev.type < transitions[state_].size())
        new_state = transitions[state_][ev.type](ev);
    if (new_state != State::ERROR) state_ = new_state;
}

void Game::Render() {
    monitor_->Prepare();
    field_->Draw();
    monitor_->Draw();
}

State Game::OnPrepareChose(GameEvent ev) {
    cell_ = ev.cords;
    cout << "Chosen!" << endl;
    return State::PREPARE_CELL_CHOSEN;
}

State Game::OnPrepareFinish(GameEvent ev) {
    Turn = false;
    cout << "wait!" << endl;
    return State::WAIT;
}

State Game::OnPrepareCellChosenChose(GameEvent ev) {
    cell_ = ev.cords;
    cout << "Chosen!" << endl;
    return State::PREPARE_CELL_CHOSEN;
}

State Game::OnPrepareCellChosenUnchose(GameEvent ev) {
    cell_ = {-1, -1};
    cout << "Unchosen!" << endl;
    return State::PREPARE;
}

State Game::OnPrepareCreateObject(GameEvent ev) {
    if (!field_->IsValidPosition(cell_)) return State::ERROR;
    if (!field_->Empty(cell_)) return State::ERROR;

    field_->CreateUnit(ev.unit_type, Turn,
                       std::move(monitor_->getModel(ModelType::B_MODEL)),
                       cell_);

    cout << "Created!" << endl;
    return State::PREPARE;
}

State Game::OnWaitFinish(GameEvent ev) {
    Turn = true;
    cout << "Wait finished!" << endl;
    return State::STEP;
}

State Game::OnStepChose(GameEvent ev) {
    shared_ptr<GameObject> chosen_object = field_->GetObject(ev.cords);
    if (chosen_object == nullptr) return State::ERROR;
    if (!chosen_object->is_mine()) return State::ERROR;

    obj_ = chosen_object;
    cell_ = ev.cords;

    cout << "Unit chosen!" << endl;
    return State::UNIT_CHOSEN;
}

State Game::OnStepFinish(GameEvent ev) {
    Turn = false;
    cout << "wait!" << endl;
    return State::WAIT;
}

State Game::OnUnitChosenChose(GameEvent ev) {
    sf::Vector2i chosen_cell = ev.cords;

    if (field_->Empty(chosen_cell)) {
        if (obj_->CanDoAction(ActionType::MOVE, chosen_cell)) {
            obj_->DoAction(ActionType::MOVE, chosen_cell);
            field_->MoveObject(cell_, chosen_cell);

            cout << "Moved!" << endl;
            return State::STEP;
        } else {
            cout << "Cannot move to this cell" << endl;
            return State::ERROR;
        }
    } else {
        cout << "Здесь должна быть атака, но ее не будет" << endl;
        return State::STEP;
    }
}

State Game::OnUnitChosenUnchose(GameEvent ev) {
    cout << "Unit unchosen!" << endl;
    return State::STEP;
}
