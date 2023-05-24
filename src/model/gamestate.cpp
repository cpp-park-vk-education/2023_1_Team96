#include "model/gamestate.hpp"

#include <iostream>
#include <sstream>

using std::cout, std::endl;

Game::Game(unique_ptr<SFMLWindow> monitor, unique_ptr<InputHandler> handler)
    : monitor_(std::move(monitor)),
      handler_(std::move(handler)),
      state_(State::PREPARE),
      cell_(-1, 1),
      obj_(nullptr),
      turn_(true),
      commands_() {
    uint rows = 9;
    uint cols = 15;
    unique_ptr<SFMLFieldModel> field_model =
        monitor_->getFieldModel(rows, cols);
    field_ = std::make_unique<Field>(rows, cols, move(field_model));
}

void Game::StartGame() {
    //while (!monitor_->isEnd()) {
        HandleInput();
        Update();
        Render();
    //}
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

void Game::HandleCommands(string commands) {
    std::istringstream command_stream(commands);
    char cmd;
    do {
        command_stream >> cmd;
        cout << cmd << endl;
        switch (cmd) {
            case 'c': {
                char unit_type_c = 0;
                command_stream >> unit_type_c;
                UnitType unit_type = MapUnitType(unit_type_c);

                sf::Vector2i pos{0, 0};
                command_stream >> pos.x >> pos.y;
                RevertXCord(pos.x);

                field_->CreateUnit(
                    unit_type, turn_,
                    std::move(monitor_->getModel(ModelType::B_MODEL)), pos);
            } break;

            case 'a': {
                cout << "Здесь должна быть атака, но ее не будет" << endl;
            } break;

            case 'm': {
                sf::Vector2i from{0, 0};
                sf::Vector2i to{0, 0};

                command_stream >> from.x >> from.y;
                command_stream >> to.x >> to.y;

                RevertXCord(from.x);
                RevertXCord(to.x);

                shared_ptr<GameObject> chosen_object = field_->GetObject(from);
                obj_->DoAction(ActionType::MOVE, (sf::Vector2u)to);
                field_->MoveObject(from, to);
            }

            break;
        }
    } while (cmd != 'e');
}

UnitType Game::MapUnitType(char type) {
    switch (type) {
        case 'b':
            return UnitType::B;
            break;
        case 'k':
            return UnitType::K;
            break;
    }
}

string Game::CreateObjectCmd(UnitType type, sf::Vector2i pos) {
    std::stringstream cmd;
    cmd << "c"
        << " ";
    switch (type) {
        case UnitType::B:
            cmd << "b"
                << " ";
            break;

        default:
            break;
    }

    cmd << pos.x << " " << pos.y << " ";
    cmd << "e";

    return cmd.str();
}

string Game::MoveObjectCmd(sf::Vector2i from, sf::Vector2i to) {
    std::stringstream cmd;
    cmd << "m"
        << " ";
    cmd << from.x << " " << from.y << " ";
    cmd << to.x << " " << to.y << " ";

    cmd << "e";
    return cmd.str();
}
string Game::AttackObjectCmd(sf::Vector2i from, sf::Vector2i to) {
    std::stringstream cmd;
    cmd << "a"
        << " ";
    cmd << from.x << " " << from.y << " ";
    cmd << to.x << " " << to.y << " ";

    cmd << "e";
    return cmd.str();
}

State Game::OnPrepareChose(GameEvent ev) {
    cell_ = ev.cords;
    cout << "Chosen!" << endl;
    return State::PREPARE_CELL_CHOSEN;
}

State Game::OnPrepareFinish(GameEvent ev) {
    turn_ = false;

    // отправка на сервер
    commands_ = "";

    cout << "Wait!" << endl;
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

    field_->CreateUnit(ev.unit_type, turn_,
                       std::move(monitor_->getModel(ModelType::B_MODEL)),
                       cell_);

    commands_ += CreateObjectCmd(ev.unit_type, cell_);

    cout << "Created!" << endl;
    return State::PREPARE;
}

State Game::OnWaitFinish(GameEvent ev) {
    HandleCommands(ev.cmds);
    turn_ = true;
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
    turn_ = false;

    // отправка на сервер
    commands_ = "";

    cout << "Wait!" << endl;
    return State::WAIT;
}

State Game::OnUnitChosenChose(GameEvent ev) {
    sf::Vector2i chosen_cell = { ev.cords.x, ev.cords.y };

    if (field_->Empty(chosen_cell)) {
        if (obj_->CanDoAction(ActionType::MOVE, (sf::Vector2u)chosen_cell)) {
            obj_->DoAction(ActionType::MOVE, (sf::Vector2u)chosen_cell);
            field_->MoveObject(cell_, chosen_cell);

            commands_ += MoveObjectCmd(cell_, chosen_cell);

            cout << "Moved!" << endl;
            return State::STEP;
        } else {
            cout << "Cannot move to this cell" << endl;
            return State::ERROR;
        }
    } else {
        cout << "Здесь должна быть атака, но ее не будет" << endl;
        commands_ += AttackObjectCmd(cell_, chosen_cell);
        return State::STEP;
    }
}

State Game::OnUnitChosenUnchose(GameEvent ev) {
    cout << "Unit unchosen!" << endl;
    return State::STEP;
}
