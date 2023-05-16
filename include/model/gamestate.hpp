#pragma once

#include <memory>
#include <vector>
#include <stack>
#include <functional>

#include "field.hpp"
#include "graphics/sfml_monitor.hpp"
#include "input/sfml_input.hpp"

using namespace std::placeholders;
using std::unique_ptr;
using std::vector;
using std::stack;
using std::shared_ptr;

#define BIND(function) std::bind(&Game::function, this, std::placeholders::_1)

struct ICommand {
    virtual void Execute(GameEvent event) = 0;
    virtual void Undo() {};
    virtual std::string string() { return "";};

    ~ICommand() {}
};

enum State { PREPARE, PREPARE_CELL_CHOSEN, WAIT, STEP, UNIT_CHOSEN, STEP_CELL_CHOSEN, ERROR };

class Game {
   private:
    unique_ptr<SFMLWindow> monitor_;
    unique_ptr<InputHandler> handler_;
    unique_ptr<Field> field_;

    State state_;
    sf::Vector2i cell_;
    shared_ptr<GameObject> obj_;
    bool Turn;

    stack<State> state_stack_;

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
    Game(unique_ptr<SFMLWindow> monitor,
         unique_ptr<InputHandler> handler);

    void StartGame();

    void HandleInput();
    void Update() {}
    void Render();    
};
