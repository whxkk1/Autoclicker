#include "InputNode.hpp"

bool InputNode::init(cocos2d::CCSize inputRect) {
    if(!cocos2d::CCNode::init()) return false;

    this->setContentSize(inputRect);

    auto layout = geode::RowLayout::create();
    layout->setGap(inputRect.width * 0.10f);

    this->setLayout(layout);

    holdTimeInput = geode::TextInput::create(inputRect.width * 0.35f, "Hold");
    releaseTimeInput = geode::TextInput::create(inputRect.width * 0.35f, "Release");
    this->addChild(holdTimeInput);
    this->addChild(releaseTimeInput);

    updateLayout();

    return true;
}

bool InputNode::init(cocos2d::CCSize inputRect, std::pair<size_t, size_t> times) {
    if(!cocos2d::CCNode::init()) return false;

    this->setContentSize(inputRect);

    auto layout = geode::RowLayout::create();
    layout->setGap(inputRect.width * 0.10f);

    this->setLayout(layout);

    holdTimeInput = geode::TextInput::create(inputRect.width * 0.35f, "Hold");
    releaseTimeInput = geode::TextInput::create(inputRect.width * 0.35f, "Release");
    holdTimeInput->setString(fmt::to_string(times.first));
    releaseTimeInput->setString(fmt::to_string(times.second));
    this->addChild(holdTimeInput);
    this->addChild(releaseTimeInput);

    updateLayout();

    return true;
}

InputNode* InputNode::create(cocos2d::CCSize inputRect) {
    auto ret = new InputNode();
    if(ret && ret->init(inputRect)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

InputNode* InputNode::create(cocos2d::CCSize inputRect, std::pair<size_t, size_t> times) {
    auto ret = new InputNode();
    if(ret && ret->init(inputRect, times)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
