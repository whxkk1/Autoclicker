#include "AutoclickerPopup.hpp"
#include "../other/variables.hpp"
#include "InputNode.hpp"

bool AutoclickerPopup::init() {
    auto winSize = cocos2d::CCDirector::get()->getWinSize();
    width = winSize.width * 0.6f;
    height = winSize.height * 0.7f;
    if(!geode::Popup::init(width, height)) return false;

    this->setTitle("Autoclicker");
    this->m_noElasticity = true;

    auto tabMenu = cocos2d::CCMenu::create();
    tabMenu->setPosition({winSize.width / 2, height + height * 0.05f});
    tabMenu->setScale(0.35);

    auto tabMenuLayout = geode::RowLayout::create();
    tabMenuLayout->setGap(width * 0.30);
    tabMenu->setLayout(tabMenuLayout);

    
    p1MenuSpr = ButtonSprite::create("Player 1", "bigFont.fnt", "GJ_button_04.png", 0.8f);
    auto p1ButtonMenu = CCMenuItemSpriteExtra::create(p1MenuSpr, this, menu_selector(AutoclickerPopup::switchTo1p));
    p1MenuSpr->updateBGImage("GJ_button_02.png");


    p2MenuSpr = ButtonSprite::create("Player 2", "bigFont.fnt", "GJ_button_04.png", 0.8f);
    auto p2ButtonMenu = CCMenuItemSpriteExtra::create(p2MenuSpr, this, menu_selector(AutoclickerPopup::switchTo2p));

    tabMenu->addChild(p1ButtonMenu);
    tabMenu->addChild(p2ButtonMenu);

    tabMenu->updateLayout();


    auto sideButtonsMenu = cocos2d::CCMenu::create(); 
    sideButtonsMenu->ignoreAnchorPointForPosition(false);
    sideButtonsMenu->setContentSize({width * 0.075f, height});
    sideButtonsMenu->setPosition({winSize.width / 2 + width / 2 - width * 0.055f, winSize.height / 2});

    auto sideButtonsMenuLayout = geode::ColumnLayout::create();
    sideButtonsMenuLayout->setAxisAlignment(geode::AxisAlignment::Center);
    sideButtonsMenuLayout->setGap(height * 0.1);
    sideButtonsMenuLayout->setAxisReverse(true);
    sideButtonsMenuLayout->setAutoScale(false);
    

    sideButtonsMenu->setLayout(sideButtonsMenuLayout);


    auto addActionButton = CCMenuItemSpriteExtra::create(cocos2d::CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png"), this, menu_selector(AutoclickerPopup::addInput));
    auto removeLastActionButton = CCMenuItemSpriteExtra::create(cocos2d::CCSprite::createWithSpriteFrameName("GJ_deleteBtn_001.png"), this, menu_selector(AutoclickerPopup::removeLastInput));
    auto removeAllActionButton = CCMenuItemSpriteExtra::create(cocos2d::CCSprite::createWithSpriteFrameName("GJ_trashBtn_001.png"), this, menu_selector(AutoclickerPopup::removeAllInput));
    addActionButton->setScale(width * 0.075 / addActionButton->getContentWidth());
    addActionButton->m_baseScale = width * 0.075 / addActionButton->getContentWidth();

    removeLastActionButton->setScale(width * 0.075 / removeLastActionButton->getContentWidth());
    removeLastActionButton->m_baseScale = width * 0.075 / removeLastActionButton->getContentWidth();

    removeAllActionButton->setScale(width * 0.075 / removeAllActionButton->getContentWidth());
    removeAllActionButton->m_baseScale = width * 0.075 / removeAllActionButton->getContentWidth();



    sideButtonsMenu->addChild(addActionButton);
    sideButtonsMenu->addChild(removeLastActionButton);
    sideButtonsMenu->addChild(removeAllActionButton);

    
    sideButtonsMenu->updateLayout();


    auto bottomMenu = cocos2d::CCMenu::create();
    bottomMenu->ignoreAnchorPointForPosition(false);
    bottomMenu->setContentSize({width * 0.65f, height * 0.05f});
    bottomMenu->setPosition({winSize.width / 2, height * 0.31f});

    auto bottomMenuLayout = geode::RowLayout::create();
    bottomMenuLayout->setGap(15);
    bottomMenu->setLayout(bottomMenuLayout);


    auto applyButton = CCMenuItemSpriteExtra::create(ButtonSprite::create("Save"), this, menu_selector(AutoclickerPopup::saveChanges));
    applyButton->m_baseScale = 0.80f;
    applyButton->setScale(0.80f);
    //applyButton->setPosition({width / 2 + width * 0.1f, height * 0.1f});

    auto offSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    auto onSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");

    auto toggledAutoclicker = CCMenuItemToggler::create(offSpr, onSpr, this, menu_selector(AutoclickerPopup::toggleAutoclicker));
    toggledAutoclicker->setScale(0.80f);
    toggleState = Autoclicker::g_autoclickerEnabled;
    toggledAutoclicker->toggle(toggleState);
    //toggledAutoclicker->setPosition({width / 2 - width * 0.1f, height * 0.1f});

    auto toggleAutoclickerLabel = cocos2d::CCLabelBMFont::create("Toggle", "bigFont.fnt");


    bottomMenu->addChild(applyButton);
    bottomMenu->addChild(toggledAutoclicker);
    bottomMenu->addChild(toggleAutoclickerLabel);


    cocos2d::CCSize inputSize = {width * 0.8f, height * 0.60f};

    auto inputBG = cocos2d::extension::CCScale9Sprite::create("square02b_001.png");
    inputBG->setColor({0, 0, 0});
    inputBG->setOpacity(35);
    inputBG->setContentSize(inputSize); //height * 0.75
    inputBG->ignoreAnchorPointForPosition(false);
    inputBG->setPosition({winSize.width / 2, winSize.height / 2});

    scroll = geode::ScrollLayer::create(inputSize, true, true);
    scroll->ignoreAnchorPointForPosition(false);
    scroll->setPosition({winSize.width / 2, winSize.height / 2});
    auto content = scroll->m_contentLayer;

    auto scrollLayout = geode::ColumnLayout::create();
    scrollLayout->setAutoScale(false);
    scrollLayout->setGap(15.f);

    scrollLayout->setAutoGrowAxis(true);
    scrollLayout->ignoreInvisibleChildren(false); //xd

    scrollLayout->setAxisReverse(true);

    content->setLayout(scrollLayout);

    loadGlobalInputs();

    bottomMenu->updateLayout();
    scroll->updateLayout();
    

    this->addChild(tabMenu);
    this->addChild(sideButtonsMenu);
    this->addChild(bottomMenu);
    this->addChild(inputBG);
    this->addChild(scroll);

    return true;
}

AutoclickerPopup* AutoclickerPopup::create() {
    auto popup = new AutoclickerPopup();
    if(popup->init()) {
        popup->autorelease();
        return popup;
    }
    delete popup;
    return nullptr;
}

void AutoclickerPopup::toggleAutoclicker(cocos2d::CCObject* sender) {
    toggleState = !toggleState;
    unsavedChanges = true;
}

void AutoclickerPopup::saveChanges(cocos2d::CCObject* sender) {
    Autoclicker::g_inputs[0].clear();
    Autoclicker::g_inputs[1].clear();
    for(size_t i = 0; i < 2; i++) {

        if(textInputs[i].empty()) continue;
        bool differentFromZero = false;

        for(const auto& node : textInputs[i]) {
            auto inputs = node->getTextStringInputs();
            auto holdTime = geode::utils::numFromString<size_t>(inputs.first);
            auto releaseTime = geode::utils::numFromString<size_t>(inputs.second);

            if(holdTime.isErr() || releaseTime.isErr()) {
                FLAlertLayer::create("Invalid input", "There is an invalid input", "Ok")->show();
                Autoclicker::g_inputs[i].clear();
                return;
            }

            auto holdTimeResult = holdTime.unwrap();
            auto releaseTimeResult = releaseTime.unwrap();
            if(holdTimeResult || releaseTimeResult) differentFromZero = true;

            Autoclicker::g_inputs[i].emplace_back(holdTimeResult, false, !i); //first is 0, so, is false (bool p1)
            Autoclicker::g_inputs[i].emplace_back(releaseTimeResult, true, !i);
        }
        if(!differentFromZero) {
            FLAlertLayer::create("Invalid input", "At least one input needs a delay different from zero.", "Ok")->show();
            Autoclicker::g_inputs[i].clear();
            return;
        }
    }
    Autoclicker::g_autoclickerEnabled = toggleState;
    unsavedChanges = false;
}

void AutoclickerPopup::addInput(cocos2d::CCObject* sender) {
    auto content = scroll->m_contentLayer;
    InputNode* node = InputNode::create({width * 0.8f, height * 0.10f});
    node->retain(); //scary
    textInputs[currentPlayer].push_back(node);
    content->addChild(node);


    content->updateLayout();
    scroll->scrollToTop();

    unsavedChanges = true;
}

void AutoclickerPopup::onClose(cocos2d::CCObject* sender) {
    if(!unsavedChanges) geode::Popup::onClose(sender);
    else {
        geode::createQuickPopup(
            "Unsaved Changes",
            "You have unsaved changes.\nDo You want to close?",
            "No", "Yes",
            [this, sender](auto, bool btn2) {
                if(btn2) geode::Popup::onClose(sender);
            }
        );
    }
}

void AutoclickerPopup::loadGlobalInputs() {

    auto content = scroll->m_contentLayer;

    for(size_t i = 0; i < 2; i++) {
        if(Autoclicker::g_inputs[i].empty()) continue;
            for(size_t j = 0; j < Autoclicker::g_inputs[i].size(); j += 2) {
                InputNode* node = InputNode::create({width * 0.8f, height * 0.10f}, {Autoclicker::g_inputs[i][j].ticks, Autoclicker::g_inputs[i][j + 1].ticks});
                node->retain();
                textInputs[i].push_back(node);
                if(i == 0) content->addChild(node); //p1 is default
                
            }

    }

    content->updateLayout();
    scroll->scrollToTop();
}

void AutoclickerPopup::switchTo1p(cocos2d::CCObject* sender) {
    if(currentPlayer == 0) return; //0 1p

    p1MenuSpr->updateBGImage("GJ_button_02.png");
    p2MenuSpr->updateBGImage("GJ_button_04.png");

    currentPlayer = 0;
    for (auto* node : textInputs[1]) {
        node->removeFromParentAndCleanup(false);
    }

    auto content = scroll->m_contentLayer;
    for (auto* node: textInputs[0]) {
        content->addChild(node);
    }
    content->updateLayout();
    scroll->scrollToTop();
}

void AutoclickerPopup::switchTo2p(cocos2d::CCObject* sender) {
    if(currentPlayer == 1) return; //1 2p

    p2MenuSpr->updateBGImage("GJ_button_02.png");
    p1MenuSpr->updateBGImage("GJ_button_04.png");

    currentPlayer = 1;
    for (auto* node : textInputs[0]) {
        node->removeFromParentAndCleanup(false);
    }

    auto content = scroll->m_contentLayer;
    for (auto* node: textInputs[1]) {
        content->addChild(node);
    }
    content->updateLayout();
    scroll->scrollToTop();
}

AutoclickerPopup::~AutoclickerPopup() {
    for(int i = 0; i < 2; i++) {
        for(auto* node : textInputs[i]) {
            node->release();
        }
        textInputs[i].clear();
    }
    
}
    
void AutoclickerPopup::removeLastInput(cocos2d::CCObject* sender) {
    if(textInputs[currentPlayer].empty()) return;

    textInputs[currentPlayer].back()->removeFromParent();
    textInputs[currentPlayer].back()->release();
    textInputs[currentPlayer].pop_back();

    unsavedChanges = true;
    
}

void AutoclickerPopup::removeAllInput(cocos2d::CCObject* sender) {
    if(textInputs[currentPlayer].empty()) return;

    geode::createQuickPopup(
        "Remove All Inputs",
        "This will remove all of your inputs on the current player.\n Continue?",
        "No", "Yes",
        [this, sender](auto, bool btn2) {
            if(btn2) {
                for(auto& node : textInputs[currentPlayer]) {
                    textInputs[currentPlayer].back()->removeFromParent();
                    textInputs[currentPlayer].back()->release();
                    textInputs[currentPlayer].pop_back();
                }
                unsavedChanges = true;
            }
        }
    );
}