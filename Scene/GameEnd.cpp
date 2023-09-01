#include "GameEnd.h"
#include "SceneManager.h"
#include "SceneTitle.h"

#include "../game.h"
#include "../InputState.h"
#include "DxLib.h"

GameEnd::GameEnd(SceneManager& manager) : SceneBase(manager),updateFunc_(&GameEnd::fadeInUpdate)
{
}

GameEnd::~GameEnd()
{
}

void GameEnd::update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

void GameEnd::draw()
{
	DrawString(0, 0, "GameEnd", 0xffffff);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	//‰æ–Ê‘S‘Ì‚ğ^‚Á•‚É“h‚è‚Â‚Ô‚·
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameEnd::fadeInUpdate(const InputState& input)
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		updateFunc_ = &GameEnd::normalUpdate;
		fadeValue_ = 0;
	}
}

void GameEnd::normalUpdate(const InputState& input)
{
	if (input.isTriggered(InputType::next)) {
		updateFunc_ = &GameEnd::fadeOutUpdate;
	}
}

void GameEnd::fadeOutUpdate(const InputState& input)
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		manager_.changeScene(new SceneTitle(manager_));
		return;
	}
}
