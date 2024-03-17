#include <vector>

#include "animation.h"

AnimationState::AnimationState(int* control_, std::vector<std::pair<float,int> > keyFrames_)
	:control(control_), keyFrames(keyFrames_) {
	now = 0;
}

void AnimationState::Update(float deltaTime)
{
    timeAccumulator_ += deltaTime;
	float time_ = keyFrames[now].first;
    if (timeAccumulator_ >= time_)
    {
        timeAccumulator_ -= time_;
		now = (now+1)%keyFrames.size();
    }
	*control = keyFrames[now].second;
}
void AnimationState::Reset(){
	now = 0;
	timeAccumulator_ = 0;
}


void AnimationStateManager::AddState(const std::string&s, const AnimationState& state)
{
    states_.push_back({s,state});
	named_states[s] = states_.size()-1;
}

void AnimationStateManager::SetState(int x){
	if( now!=x ){
		states_[now].second.Reset();
		now = x;
	}
}
void AnimationStateManager::SetState(std::string s){
	//assert(named_states.count(s));
	int x = named_states[s];
	SetState(x);
}

std::string AnimationStateManager::GetState(){
	return states_[now].first;
}

void AnimationStateManager::Update(float deltaTime)
{
    states_[now].second.Update(deltaTime);
}
