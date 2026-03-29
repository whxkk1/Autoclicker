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
    auto p1ButtonMenu = CCMenuItemSpriteExtra::create(p1MenuSpr, this, menu_selector(AutoclickerPopup::switchPlayer));
    p1ButtonMenu->setTag(0);
    p1MenuSpr->updateBGImage("GJ_button_02.png");


    p2MenuSpr = ButtonSprite::create("Player 2", "bigFont.fnt", "GJ_button_04.png", 0.8f);
    auto p2ButtonMenu = CCMenuItemSpriteExtra::create(p2MenuSpr, this, menu_selector(AutoclickerPopup::switchPlayer));
    p2ButtonMenu->setTag(1);

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
        
        size_t repeatAccumulation = 0; //when hold == 0 && release == 0, used to check for accumulated repeat time < 1000

        if(textInputs[i].empty()) continue;
        bool differentFromZero = false;

        for(const auto& node : textInputs[i]) {
            auto inputs = node->getTextStringInputs();
            auto repeatTimes = node->getRepeat();

            auto holdTimeResult = geode::utils::numFromString<size_t>(inputs.first);
            auto releaseTimeResult = geode::utils::numFromString<size_t>(inputs.second);
            auto repeatResult = geode::utils::numFromString<size_t>(repeatTimes);

            std::string errorMessage;

            if(!holdTimeResult) errorMessage += "Hold field: " + holdTimeResult.unwrapErr() + '\n';
            if(!releaseTimeResult) errorMessage += "Release field: " + releaseTimeResult.unwrapErr() + '\n';
            if(!repeatResult) errorMessage += "Repeat field: " + repeatResult.unwrapErr() + '\n';

            if(!errorMessage.empty()) {
                FLAlertLayer::create("Invalid input", "Error message/s:\n" + errorMessage, "Ok")->show();
                Autoclicker::g_inputs[i].clear();
                return;
            }


            auto holdTime = holdTimeResult.unwrap();
            auto releaseTime = releaseTimeResult.unwrap();
            auto repeat = repeatResult.unwrap();
            if(holdTime || releaseTime) differentFromZero = true;

            if(repeat == 0) {
                FLAlertLayer::create("Invalid input", "Repeat needs a value above 0.", "Ok")->show();
                Autoclicker::g_inputs[i].clear();
                return;
            }
            if(holdTime == 0 && releaseTime == 0) {

                if(repeat > 1000) {
                    FLAlertLayer::create("Invalid input", "The repeat time cannot exceed 1000 if hold and release are equal to 0.", "Ok")->show();
                    Autoclicker::g_inputs[i].clear();
                    return;
                } else {
                    repeatAccumulation += repeat;
                    if(repeatAccumulation > 1000) {
                        FLAlertLayer::create("Invalid inputs", "The accumulated repeat time for consecutive inputs with a hold and release of 0 cannot exceed 1000.", "Ok")->show();
                        Autoclicker::g_inputs[i].clear();
                        return;
                    }
                }
                
            } else {
                repeatAccumulation = 0;

            }

            Autoclicker::g_inputs[i].emplace_back(Input({holdTime, releaseTime}, repeat, !i)); //first is 0, so, is false (bool p1)
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
    InputNode* node = InputNode::create({width * 0.80f, height * 0.10f});
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

        for(const auto& input : Autoclicker::g_inputs[i]) {
            InputNode* node = InputNode::create({width * 0.80f, height * 0.10f}, {input.ticks[0], input.ticks[1]}, input.repeatTimes);
            node->retain();  //xd
            textInputs[i].emplace_back(node);
            if (i == 0) content->addChild(node); //0 is p1 and its by default
        }

    }

    content->updateLayout();
    scroll->scrollToTop();
}

void AutoclickerPopup::switchPlayer(cocos2d::CCObject* sender) {
    int tag = sender->getTag();
    if(tag < 0 || tag > 1 || currentPlayer == tag) return;
    ButtonSprite* sprites[2] = {p1MenuSpr, p2MenuSpr};
    // 0 = 1p
    // 1 = 2p

    sprites[tag]->updateBGImage("GJ_button_02.png");
    sprites[!tag]->updateBGImage("GJ_button_04.png");
    // ! my beloved

    currentPlayer = tag;

    for(auto* node : textInputs[!tag]) {
        node->removeFromParentAndCleanup(false);
    }

    auto content = scroll->m_contentLayer;
    for(auto* node: textInputs[tag]) {
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