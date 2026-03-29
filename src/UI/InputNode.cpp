#include "InputNode.hpp"

bool InputNode::init(cocos2d::CCSize inputRect) {
    if(!cocos2d::CCNode::init()) return false;

    this->setContentSize(inputRect);

    auto layout = geode::RowLayout::create();
    layout->setGap(inputRect.width * 0.10f);

    this->setLayout(layout);

    holdTimeInput = geode::TextInput::create(inputRect.width * 0.35f, "Hold");
    releaseTimeInput = geode::TextInput::create(inputRect.width * 0.35f, "Release");
    repeatInput = geode::TextInput::create(inputRect.width * 0.20f, "Repeats");
    repeatInput->setString("1");

    this->addChild(holdTimeInput);
    this->addChild(releaseTimeInput);
    this->addChild(repeatInput);

    updateLayout();

    return true;
}

bool InputNode::init(cocos2d::CCSize inputRect, std::pair<size_t, size_t> times, size_t repeatTimes) {
    if(!cocos2d::CCNode::init()) return false;

    this->setContentSize(inputRect);

    auto layout = geode::RowLayout::create();
    layout->setGap(inputRect.width * 0.10f);

    this->setLayout(layout);

    holdTimeInput = geode::TextInput::create(inputRect.width * 0.35f, "Hold");
    releaseTimeInput = geode::TextInput::create(inputRect.width * 0.35f, "Release");
    repeatInput = geode::TextInput::create(inputRect.width * 0.20f, "Repeats");

    holdTimeInput->setString(fmt::to_string(times.first));
    releaseTimeInput->setString(fmt::to_string(times.second));
    repeatInput->setString(fmt::to_string(repeatTimes));

    this->addChild(holdTimeInput);
    this->addChild(releaseTimeInput);
    this->addChild(repeatInput);

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

InputNode* InputNode::create(cocos2d::CCSize inputRect, std::pair<size_t, size_t> times, size_t repeatTimes) {
    auto ret = new InputNode();
    if(ret && ret->init(inputRect, times, repeatTimes)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
