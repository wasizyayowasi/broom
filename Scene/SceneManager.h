#pragma once
#include <deque>

class SceneBase;
class InputState;

class SceneManager
{
public:
	void changeScene(SceneBase* scene);
	void pushScene(SceneBase* scene);
	void popScene();
	void update(const InputState& input);
	void draw();
private:
	std::deque<SceneBase*> scenes_;
};

