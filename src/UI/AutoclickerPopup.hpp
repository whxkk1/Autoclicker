#pragma once
#include "InputNode.hpp"
#include "../other/variables.hpp"

class AutoclickerPopup : public geode::Popup {
private:
    bool toggleState;
    bool unsavedChanges = false;
    float width;
    float height;
    geode::ScrollLayer* scroll = nullptr;
    std::vector<InputNode*> textInputs[2]; //p1 p2 xd
    void toggleAutoclicker(cocos2d::CCObject* sender);
    void saveChanges(cocos2d::CCObject* sender);
    void addInput(cocos2d::CCObject* sender);
    void loadGlobalInputs();
    size_t currentPlayer = 0; //0 = p1, 1 = p2
    ButtonSprite* p1MenuSpr = nullptr;
    ButtonSprite* p2MenuSpr = nullptr;

    void switchTo1p(cocos2d::CCObject* sender);
    void switchTo2p(cocos2d::CCObject* sender);

    void removeLastInput(cocos2d::CCObject* sender);
    void removeAllInput(cocos2d::CCObject* sender);

protected:
    bool init() override;
    void onClose(cocos2d::CCObject* sender) override;
    virtual ~AutoclickerPopup();
public:
    static AutoclickerPopup* create();
};