#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../other/variables.hpp"



class $modify(AutoclickerGJBGLHook, GJBaseGameLayer) {
    struct Fields {
        size_t currentIdx[2] = {0, 0};
        size_t currentInputDelta[2] = {0, 0};
        size_t currentRepeats[2] = {0, 0};
        size_t lastDelta[2] = {0, 0};
        bool isRelease[2] = {false, false}; 
    };

    void processCommands(float dt, bool isHalfTick, bool isLastTick) {
        if(!Autoclicker::g_autoclickerEnabled) return GJBaseGameLayer::processCommands(dt, isHalfTick, isLastTick);
        
        auto f = m_fields.self();
        //geode::log::info("delta1p: {}, actDel: {}, repeat1p {}, repeatAc {}, currentidx1p {}", m_fields->currentInputDelta[0], Autoclicker::g_inputs[0][f->currentIdx[0]].ticks[f->isRelease[0]], f->currentRepeats[0], Autoclicker::g_inputs[0][f->currentIdx[0]].repeatTimes, f->currentIdx[0]);
        //yes the line above makes the game crash if the vector is empty

        for(size_t i = 0; i < 2; i++) {

            if (Autoclicker::g_inputs[i].empty()) continue;

            if(f->currentIdx[i] >= Autoclicker::g_inputs[i].size()) f->currentIdx[i] = 0;
            if(f->currentRepeats[i] >= Autoclicker::g_inputs[i][f->currentIdx[i]].repeatTimes) f->currentRepeats[i] = 0;
            

            while(f->currentInputDelta[i] == f->lastDelta[i]) {

                f->lastDelta[i] = Autoclicker::g_inputs[i][f->currentIdx[i]].ticks[f->isRelease[i]];
                // geode::log::info("lastdelta:{}, isRel: {}", f->lastDelta[i], f->isRelease[i]);

                f->currentInputDelta[i] = 0;
                if(f->isRelease[i]) f->currentRepeats[i]++; //hold && release

                f->isRelease[i] = !f->isRelease[i]; //switch between release and hold because
                GJBaseGameLayer::handleButton(f->isRelease[i], 1, !i); //first is player 1 (!i)

                if(f->currentRepeats[i] >= Autoclicker::g_inputs[i][f->currentIdx[i]].repeatTimes) {
                    f->currentRepeats[i] = 0;
                    f->currentIdx[i]++;
                    if(f->currentIdx[i] >= Autoclicker::g_inputs[i].size()) f->currentIdx[i] = 0;
                }
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

                if(delta.ticks[0] >= deltaGjbgl || delta.ticks[1] >= deltaGjbgl) {
                    smallerDelta = true;
                    break;
                }

            }
            if(!smallerDelta) static_cast<AutoclickerGJBGLHook*>(gjbgl)->m_fields->currentInputDelta[i] = 0;
        }
    }
}

class $modify(AutoclickerPLHook, PlayLayer) {
    void setupHasCompleted() {
        checkDelta();
        PlayLayer::setupHasCompleted();
    }
    void resumeAndRestart(bool fromStart) {
        checkDelta();
        PlayLayer::resumeAndRestart(fromStart);
    }
    void resume() {
        checkDelta();
        PlayLayer::resume();
    }
};