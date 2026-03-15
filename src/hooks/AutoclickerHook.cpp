#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../other/variables.hpp"



class $modify(AutoclickerGJBGLHook, GJBaseGameLayer) {
    struct Fields {
        size_t currentIdx[2] = {0, 0};
        size_t currentInputDelta[2] = {0, 0};
    };

    void processCommands(float dt, bool isHalfTick, bool isLastTick) {
        //geode::log::info("delta1p: {}, delta2p: {}", m_fields->currentInputDelta[0], m_fields->currentInputDelta[1]);
        if(!Autoclicker::g_autoclickerEnabled) return GJBaseGameLayer::processCommands(dt, isHalfTick, isLastTick);

        auto f = m_fields.self();
        for(size_t i = 0; i < 2; i++) {

            if (Autoclicker::g_inputs[i].empty()) continue;

            if(f->currentIdx[i] >= Autoclicker::g_inputs[i].size()) f->currentIdx[i] = 0;

            while(f->currentInputDelta[i] == Autoclicker::g_inputs[i][f->currentIdx[i]].ticks) {
                GJBaseGameLayer::handleButton(!Autoclicker::g_inputs[i][f->currentIdx[i]].release, 1, !i); //first is player 1
                f->currentInputDelta[i] = 0;
                f->currentIdx[i]++;
                if(f->currentIdx[i] >= Autoclicker::g_inputs[i].size()) f->currentIdx[i] = 0;
            }
            f->currentInputDelta[i]++;
        }
        
        GJBaseGameLayer::processCommands(dt, isHalfTick, isLastTick);
    }
    
};

void checkDelta() {
    auto gjbgl = GJBaseGameLayer::get();
    if(gjbgl) {
        for(size_t i = 0; i < 2; i++) {
            bool smallerDelta = false;
            size_t deltaGjbgl = static_cast<AutoclickerGJBGLHook*>(gjbgl)->m_fields->currentInputDelta[i];
            for(const auto& delta : Autoclicker::g_inputs[i]) {

                if(delta.ticks >= deltaGjbgl) {
                    smallerDelta = true;
                    break;
                }

            }
            if(!smallerDelta) static_cast<AutoclickerGJBGLHook*>(gjbgl)->m_fields->currentInputDelta[i] = 0;
        }
    }
    /*
    for context:
    this function checks if the current delta is lower than saved input deltas, becuase the user can change the inputs
    mid game and it can break some things
    
    */
}

class $modify(AutoclickerPLHook, PlayLayer) {
    void resumeAndRestart(bool fromStart) {
        checkDelta();
        PlayLayer::resumeAndRestart(fromStart);
    }
    void resume() {
        checkDelta();
        PlayLayer::resume();
    }
};