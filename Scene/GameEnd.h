#pragma once
#include "SceneBase.h"

class GameEnd : public SceneBase
{
public:
	GameEnd(SceneManager& manager);
	virtual ~GameEnd();

	void update(const InputState& input);
	void draw();

private:
	void fadeInUpdate(const InputState& input);
	void normalUpdate(const InputState& input);
	void fadeOutUpdate(const InputState& input);

	static constexpr int fadeInterval_ = 60;
	int fadeTimer_ = fadeInterval_;
	int fadeValue_ = 255;
	int fadeColor_ = 0x000000;

	void (GameEnd::* updateFunc_)(const InputState& input);

};

