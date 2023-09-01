#include "SceneTitle.h"
#include "SceneManager.h"
#include "GameMain.h"

#include "../game.h"
#include "../InputState.h"
#include "DxLib.h"

SceneTitle::SceneTitle(SceneManager& manager) : SceneBase(manager), updateFunc_(&SceneTitle::fadeInUpdate)
{

}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

void SceneTitle::draw()
{
	DrawString(0, 0, "SceneTitle", 0xffffff);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	//âÊñ ëSëÃÇê^Ç¡çïÇ…ìhÇËÇ¬Ç‘Ç∑
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SceneTitle::fadeInUpdate(const InputState& input)
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		updateFunc_ = &SceneTitle::normalUpdate;
		fadeValue_ = 0;
	}
}

void SceneTitle::normalUpdate(const InputState& input)
{
	if (input.isTriggered(InputType::next)) {
		updateFunc_ = &SceneTitle::fadeOutUpdate;
	}
}

void SceneTitle::fadeOutUpdate(const InputState& input)
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		manager_.changeScene(new GameMain(manager_));
		return;
	}
}
