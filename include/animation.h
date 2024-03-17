#pragma once

#include <vector>
#include <iostream>
#include <unordered_map>

class AnimationState {
public:
    AnimationState(int* control_, std::vector<std::pair<float,int> > keyFrames_);
    void Update(float deltaTime);
	void Reset();
private:
	int*control;
	int now;
	std::vector<std::pair<float,int> > keyFrames;
	float timeAccumulator_ = 0.0f;
};

class AnimationStateManager {
public:
	AnimationStateManager(){ }
    void AddState(const std::string&s, const AnimationState& state);
	void SetState(int x);
	void SetState(std::string s);
	std::string GetState();
    void Update(float deltaTime);
private:
	int now = 0;
	std::unordered_map<std::string, int> named_states;
    std::vector<std::pair<std::string,AnimationState> > states_;
};
