#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

enum GameState {
    Menu, Pause, Playing
};

class GameStateManager {
private:
    static GameState state;
public:
    static void setGameState(const GameState& state_){
        state = state_;
    }

    static void process(){
        switch(state){
            case GameState::Menu: {

            }
            case GameState::Pause: {

            }
            case GameState::Playing: {
                
            }
        }
    }

};

#endif