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

const uint UNITS_NUM = 3;
const uint STEP_POINTS = 999;

using std::cout, std::endl;

Game::Game(unique_ptr<SFMLWindow> monitor, unique_ptr<InputHandler> handler)
    : monitor_(std::move(monitor)),
      handler_(std::move(handler)),
      state_(State::PREPARE),
      cell_(0, 0),
      obj_(nullptr),
      turn_(true),
      points_(UNITS_NUM),
      commands_() {
    uint rows = FIELD_HEIGHT;
    uint cols = FILED_WIDTH;
    unique_ptr<SFMLFieldModel> field_model =
        monitor_->GetFieldModel();
    field_ = std::make_unique<Field>(rows, cols, move(field_model));
}

void Game::StartGame() {
    //while (!monitor_->isEnd()) {
        GameEvent ev = handler_->Handle();
        HandleInput(ev);
        Render();
    //}
}

void Game::HandleInput(GameEvent ev) {
    State new_state = State::ERROR;
    if (-1 < ev.type && ev.type < transitions[state_].size())
        new_state = transitions[state_][ev.type](ev);
    if (new_state != State::ERROR) state_ = new_state;
}

void Game::Render() {
    monitor_->Prepare();
    std::cout << "render" << std::endl;
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
                    std::move(monitor_->GetModel(ModelType::B_MODEL, turn_)),
                    pos);
            } break;

            case ATTACK_COMMAND: {
                sf::Vector2u from{0, 0};
                sf::Vector2u to{0, 0};

                command_stream >> from.x >> from.y;
                command_stream >> to.x >> to.y;

                RevertXCord(from.x);
                RevertXCord(to.x);

                unique_ptr<Attack> attack =
                    std::make_unique<Attack>(0, 0, from, false);
                shared_ptr<GameObject> attack_obj = field_->GetObject(from);
                shared_ptr<GameObject> attacked_obj = field_->GetObject(to);

                attack_obj->DoAction(ActionType::ATTACK, attack.get());
                attacked_obj->DoAction(ActionType::GET_ATTACKED, attack.get());

                if (attack->is_dead) field_->DeleteObject(to);

            } break;

            case MOVE_COMMAND: {
                sf::Vector2u from{0, 0};
                sf::Vector2u to{0, 0};

                command_stream >> from.x >> from.y;
                command_stream >> to.x >> to.y;

                RevertXCord(from.x);
                RevertXCord(to.x);

                shared_ptr<GameObject> chosen_object = field_->GetObject(from);
                chosen_object->DoAction(ActionType::MOVE, to);
                field_->MoveObject(from, to);
            } break;
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

bool Game::AmIWon() {
    auto objects = field_->Objects();
    int alive_enemy_objects = 0;
    for (auto obj : objects) {
        if (obj != nullptr && !obj->IsMine()) alive_enemy_objects++;
    }
    return alive_enemy_objects == 0;
}

bool Game::AmILost() {
    auto objects = field_->Objects();
    int alive_my_objects = 0;
    for (auto obj : objects) {
        if (obj != nullptr && obj->IsMine()) alive_my_objects++;
    }
    return alive_my_objects == 0;
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
    if (points_ <= 0) {
        cout << "Step points are over!" << endl;
        return State::ERROR;
    }

    if (!field_->IsMyPart(cell_)) return State::ERROR;
    if (!field_->IsEmpty(cell_)) return State::ERROR;

    field_->CreateUnit(ev.unit_type, turn_,
                       std::move(monitor_->GetModel(ModelType::B_MODEL, turn_)),
                       cell_);

    commands_ += CreateObjectCmd(ev.unit_type, cell_);
    field_->Reset();
    --points_;

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

    if (AmILost()) {
        cout << "LOSE!" << endl;
        exit(0);
    }

    turn_ = true;
    points_ = STEP_POINTS;
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
    field_->Reset();

    commands_ += END_COMMAND;
    // отправка на сервер
    commands_ = "";

    if (AmIWon()) {
        cout << "WIN!" << endl;
        exit(0);
    }

    cout << "Wait!" << endl;
    return State::WAIT;
}

State Game::OnUnitChosenChose(GameEvent ev) {
    if (points_ <= 0) {
        cout << "Step points are over!" << endl;
        return State::ERROR;
    }

    sf::Vector2u chosen_cell = {ev.cords.x, ev.cords.y};

    if (field_->IsEmpty(chosen_cell) &&
        field_->GetObject(chosen_cell) != obj_) {
        if (obj_->CanDoAction(ActionType::MOVE, chosen_cell)) {
            obj_->DoAction(ActionType::MOVE, chosen_cell);
            field_->MoveObject(cell_, chosen_cell);

            commands_ += MoveObjectCmd(cell_, chosen_cell);
            field_->Reset();
            --points_;

            cout << "Moved!" << endl;
            return State::STEP;
        } else {
            cout << "Cannot move to this cell" << endl;
            return State::ERROR;
        }
    } else {
        unique_ptr<Attack> attack =
            std::make_unique<Attack>(0, 0, cell_, false);

        if (obj_->CanDoAction(ActionType::ATTACK, attack.get())) {
            obj_->DoAction(ActionType::ATTACK, attack.get());
            obj_->GetModel().Attack();
        }

        shared_ptr<GameObject> attacked_obj = field_->GetObject(chosen_cell);
        if (attacked_obj->CanDoAction(ActionType::GET_ATTACKED, attack.get())) {
            attacked_obj->DoAction(ActionType::GET_ATTACKED, attack.get());

            if (attack->is_dead) field_->DeleteObject(chosen_cell);

            commands_ += AttackObjectCmd(cell_, chosen_cell);
            field_->Reset();
            --points_;

            cout << "Attacked!" << endl;
            return State::STEP;
        } else {
            cout << "Cannot attack this unit" << endl;
            return State::ERROR;
        }
    }
}

State Game::OnUnitChosenUnchose(GameEvent ev) {
    field_->Reset();
    cout << "Unit unchosen!" << endl;
    return State::STEP;
}
