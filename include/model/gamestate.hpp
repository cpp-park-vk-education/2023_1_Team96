#pragma once

#include <functional>
#include <memory>
#include <stack>
#include <vector>

#include "field.hpp"
#include "graphics/sfml_monitor.hpp"
#include "input/sfml_input.hpp"
#include "server/client.hpp"

using namespace std::placeholders;
using std::shared_ptr;
using std::stack;
using std::string;
using std::unique_ptr;
using std::vector;

#define BIND(function) std::bind(&Game::function, this, std::placeholders::_1)

enum State {
    PREPARE,
    PREPARE_CELL_CHOSEN,
    WAIT,
    STEP,
    UNIT_CHOSEN,
    STEP_CELL_CHOSEN,
    ERROR
};

class Game {
   private:
    unique_ptr<Client> client_;

    unique_ptr<SFMLWindow> monitor_;
    unique_ptr<InputHandler> handler_;
    unique_ptr<Field> field_;

    State state_;
    sf::Vector2u cell_;
    shared_ptr<GameObject> obj_;
    bool turn_;

    string commands_;

    void HandleCommands(string commands);
    void RevertXCord(uint& x_cord) { x_cord = field_->Width() - 1 - x_cord; };
    UnitType MapUnitType(char type);
    char MapUnitType(UnitType type);

    string CreateObjectCmd(UnitType type, sf::Vector2u pos);
    string MoveObjectCmd(sf::Vector2u from, sf::Vector2u to);
    string AttackObjectCmd(sf::Vector2u from, sf::Vector2u to);

    State OnError(GameEvent ev) { return State::ERROR; }

    State OnPrepareChose(GameEvent ev);
    State OnPrepareFinish(GameEvent ev);

    State OnPrepareCellChosenChose(GameEvent ev);
    State OnPrepareCellChosenUnchose(GameEvent ev);
    State OnPrepareCreateObject(GameEvent ev);

    State OnWaitFinish(GameEvent ev);

    State OnStepChose(GameEvent ev);
    State OnStepFinish(GameEvent ev);

    State OnUnitChosenChose(GameEvent ev);
    State OnUnitChosenUnchose(GameEvent ev);

    using GameEventHandler = std::function<State(GameEvent)>;

    vector<vector<GameEventHandler>> transitions = {
                                  /*CHOSE*/                                                  /*UNCHOSE*/                                      /*CREATE_OBJECT*/                           /*MOVE_CMD*/          /*ATTACK_CMD*/        /*FINISH*/
        /*PREPARE*/             { BIND(OnPrepareChose), /*to PREPARE_CELL_CHOSEN*/           BIND(OnError),                                   BIND(OnError),                              BIND(OnError),        BIND(OnError),        BIND(OnPrepareFinish) /*to WAIT*/ },
        /*PREPARE_CELL_CHOSEN*/ { BIND(OnPrepareCellChosenChose), /*to PREPARE_CELL_CHOSEN*/ BIND(OnPrepareCellChosenUnchose), /*to PREPARE*/ BIND(OnPrepareCreateObject), /*to PREPARE*/ BIND(OnError),        BIND(OnError),        BIND(OnError)                     },
        /*WAIT*/                { BIND(OnError),                                             BIND(OnError),                                   BIND(OnError),                              BIND(OnError),        BIND(OnError),        BIND(OnWaitFinish) /*to STEP*/    },
        /*STEP*/                { BIND(OnStepChose), /*to UNIT_CHOSEN*/                      BIND(OnError),                                   BIND(OnError),                              BIND(OnError),        BIND(OnError),        BIND(OnStepFinish) /*to WAIT*/    },
        /*UNIT_CHOSEN*/         { BIND(OnUnitChosenChose), /*to STEP*/                       BIND(OnUnitChosenUnchose), /*to STEP*/           BIND(OnError),                              BIND(OnError),        BIND(OnError),        BIND(OnError)                     }
    };

   public:
    Game(unique_ptr<SFMLWindow> monitor, unique_ptr<InputHandler> handler);

    void StartGame();

    void HandleInput();
    void Render();
};
