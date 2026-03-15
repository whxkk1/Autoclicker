#include <Geode/modify/PauseLayer.hpp>
#include "UI/AutoclickerPopup.hpp"
class $modify(PLHook, PauseLayer) {

	void customSetup() {
		
		PauseLayer::customSetup();

		
		auto btn = CCMenuItemSpriteExtra::create(geode::CircleButtonSprite::createWithSpriteFrameName("gjHand_01_001.png"), this, menu_selector(PLHook::onMyButton));

		auto rightMenu = this->getChildByID("right-button-menu");
		if(!rightMenu) return;

		rightMenu->addChild(btn);
		rightMenu->updateLayout();
		
	}
	
	void onMyButton(CCObject*) {
		AutoclickerPopup::create()->show();
	}
};