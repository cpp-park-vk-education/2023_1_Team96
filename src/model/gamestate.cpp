#include "model/gamestate.hpp"

#include <iostream>
#include <sstream>

const uint FIELD_HEIGHT = 9;
const uint FILED_WIDTH = 15;

const char CREATE_COMMAND = 'c';
const char MOVE_COMMAND = 'm';
const char ATTACK_COMMAND = 'a';
const char END_COMMAND = 'e';

const char B_UNIT = 'b';
const char K_UNIT = 'k';

using std::cout, std::endl;

Game::Game(unique_ptr<SFMLWindow> monitor, unique_ptr<InputHandler> handler)
    : monitor_(std::move(monitor)),
      handler_(std::move(handler)),
      state_(State::PREPARE),
      cell_(-1, 1),
      obj_(nullptr),
      turn_(true),
      commands_() {
    uint rows = FIELD_HEIGHT;
    uint cols = FILED_WIDTH;
    unique_ptr<SFMLFieldModel> field_model =
        monitor_->getFieldModel(rows, cols);
    field_ = std::make_unique<Field>(rows, cols, move(field_model));
}

void Game::StartGame() {
    while (!monitor_->isEnd()) {
        HandleInput();
        Render();
    }
}

void Game::HandleInput() {
    GameEvent ev = handler_->Handle();
    State new_state = State::ERROR;
    if (-1 < ev.type && ev.type < transitions[state_].size())
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
        switch (cmd) {
            case CREATE_COMMAND: {
                char unit_type_c = 0;
                command_stream >> unit_type_c;
                UnitType unit_type = MapUnitType(unit_type_c);

                sf::Vector2u pos{0, 0};
                command_stream >> pos.x >> pos.y;
                RevertXCord(pos.x);

                field_->CreateUnit(
                    unit_type, turn_,
                    std::move(monitor_->getModel(ModelType::B_MODEL, false)),
                    pos);
            } break;

            case ATTACK_COMMAND: {
                cout << "Здесь должна быть атака, но ее не будет" << endl;
            } break;

            case MOVE_COMMAND: {
                sf::Vector2u from{0, 0};
                sf::Vector2u to{0, 0};

                command_stream >> from.x >> from.y;
                command_stream >> to.x >> to.y;

                RevertXCord(from.x);
                RevertXCord(to.x);

                shared_ptr<GameObject> chosen_object = field_->GetObject(from);
                obj_->DoAction(ActionType::MOVE, to);
                field_->MoveObject(from, to);
            }

            break;
        }
    } while (cmd != END_COMMAND);
}

UnitType Game::MapUnitType(char type) {
    switch (type) {
        case B_UNIT:
            return UnitType::B;
            break;
        case K_UNIT:
            return UnitType::K;
            break;
    }
}

char Game::MapUnitType(UnitType type) {
    switch (type) {
        case UnitType::B:
            return B_UNIT;
            break;
        case UnitType::K:
            return K_UNIT;
            break;
    }
}

string Game::CreateObjectCmd(UnitType type, sf::Vector2u pos) {
    std::stringstream cmd;
    cmd << CREATE_COMMAND << " ";

    cmd << MapUnitType(type) << " ";

    cmd << pos.x << " " << pos.y << " ";

    return cmd.str();
}

string Game::MoveObjectCmd(sf::Vector2u from, sf::Vector2u to) {
    std::stringstream cmd;
    cmd << MOVE_COMMAND << " ";

    cmd << from.x << " " << from.y << " ";
    cmd << to.x << " " << to.y << " ";

    return cmd.str();
}
string Game::AttackObjectCmd(sf::Vector2u from, sf::Vector2u to) {
    std::stringstream cmd;
    cmd << ATTACK_COMMAND << " ";

    cmd << from.x << " " << from.y << " ";
    cmd << to.x << " " << to.y << " ";

    return cmd.str();
}

State Game::OnPrepareChose(GameEvent ev) {
    cell_ = ev.cords;
    cout << "Chosen!" << endl;
    field_->Choose(ev.cords);
    return State::PREPARE_CELL_CHOSEN;
}

State Game::OnPrepareCellChosenChose(GameEvent ev) {
    cell_ = ev.cords;
    cout << "Chosen!" << endl;
    field_->Choose(ev.cords);
    return State::PREPARE_CELL_CHOSEN;
}

State Game::OnPrepareCellChosenUnchose(GameEvent ev) {
    field_->Reset();
    cout << "Unchosen!" << endl;
    return State::PREPARE;
}

State Game::OnPrepareCreateObject(GameEvent ev) {
    if (!field_->IsMyPart(cell_)) return State::ERROR;
    if (!field_->IsEmpty(cell_)) return State::ERROR;

    field_->CreateUnit(ev.unit_type, turn_,
                       std::move(monitor_->getModel(ModelType::B_MODEL, turn_)),
                       cell_);

    commands_ += CreateObjectCmd(ev.unit_type, cell_);

    cout << "Created!" << endl;
    return State::PREPARE;
}

State Game::OnPrepareFinish(GameEvent ev) {
    turn_ = false;
    field_->Reset();

    commands_ += END_COMMAND;
    // отправка на сервер
    commands_ = "";

    cout << "Wait!" << endl;
    return State::WAIT;
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
    if (!chosen_object->IsMine()) return State::ERROR;

    field_->Choose(ev.cords);

    obj_ = chosen_object;
    cell_ = ev.cords;

    cout << "Unit chosen!" << endl;
    return State::UNIT_CHOSEN;
}

State Game::OnStepFinish(GameEvent ev) {
    turn_ = false;

    commands_ += END_COMMAND;
    // отправка на сервер
    commands_ = "";

    cout << "Wait!" << endl;
    return State::WAIT;
}

State Game::OnUnitChosenChose(GameEvent ev) {
    sf::Vector2u chosen_cell = {ev.cords.x, ev.cords.y};

    if (field_->IsEmpty(chosen_cell) &&
        field_->GetObject(chosen_cell) != obj_) {
        if (obj_->CanDoAction(ActionType::MOVE, chosen_cell)) {
            obj_->DoAction(ActionType::MOVE, chosen_cell);
            field_->MoveObject(cell_, chosen_cell);

            commands_ += MoveObjectCmd(cell_, chosen_cell);

            field_->Reset();
            cout << "Moved!" << endl;
            return State::STEP;
        } else {
            cout << "Cannot move to this cell" << endl;
            return State::ERROR;
        }
    } else {
        cout << "Здесь должна быть атака, но ее не будет" << endl;
        commands_ += AttackObjectCmd(cell_, chosen_cell);
        field_->Reset();
        return State::STEP;
    }
}

State Game::OnUnitChosenUnchose(GameEvent ev) {
    field_->Reset();
    cout << "Unit unchosen!" << endl;
    return State::STEP;
}
