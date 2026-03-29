#pragma once

class InputNode : public cocos2d::CCNode {
private:
    geode::TextInput* holdTimeInput = nullptr;
    geode::TextInput* releaseTimeInput = nullptr;
    geode::TextInput* repeatInput = nullptr;
protected:
    bool init(cocos2d::CCSize inputRect);
    bool init(cocos2d::CCSize inputRect, std::pair<size_t, size_t> times, size_t repeatTimes);
public:
    static InputNode* create(cocos2d::CCSize inputRect);
    static InputNode* create(cocos2d::CCSize inputRect, std::pair<size_t, size_t> times, size_t repeatTimes);
    
    inline std::pair<gd::string, gd::string> getTextStringInputs() {
        std::pair<gd::string, gd::string> str = {"", ""};

        if (holdTimeInput != nullptr) {
            str.first = holdTimeInput->getString();
        }

        if (releaseTimeInput != nullptr) {
            str.second = releaseTimeInput->getString();
        }

        return str;
    }
    inline gd::string getRepeat() {
        if(repeatInput != nullptr) return repeatInput->getString();

        return "";
    }
};