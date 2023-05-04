#pragma once

#include "commandmanager.hpp"
#include "objectfactory.hpp"
#include "objectrepository.hpp"

struct IGameModel {
    virtual void StartGame() = 0;
    virtual void ProcessCommand(const Command& cmd) = 0;
    virtual const std::unique_ptr<IObjectRepository>& Repository() const = 0;
    virtual void EndGame() = 0;

    virtual ~IGameModel() {}
};

class GameModel : public IGameModel {
   private:
    std::unique_ptr<IObjectRepository> object_repo_;
    std::unique_ptr<ICommandManager> command_manager_;
    std::shared_ptr<Player> first_player_;
    std::shared_ptr<Player> second_player_;

    void ConfigCommandManager();
    void CreateObject(std::shared_ptr<Player> player, Cell top_left_cords,
                      IObjectFactory* factory);

   public:
    GameModel(std::unique_ptr<Player> first_player,
              std::unique_ptr<Player> second_player,
              std::unique_ptr<IObjectRepository> ObjectRepository)
        : object_repo_(std::move(ObjectRepository)),
          first_player_(std::move(first_player)),
          second_player_(std::move(second_player)) {}

    void StartGame() override;

    inline void ProcessCommand(const Command& cmd) override {
        command_manager_->ProcessCommand(cmd);
    }

    inline const std::unique_ptr<IObjectRepository>& Repository()
        const override {
        return object_repo_;
    }

    void EndGame() override;
};
