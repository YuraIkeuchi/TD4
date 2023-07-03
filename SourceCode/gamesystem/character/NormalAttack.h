#pragma once
#include "InterAttack.h"
class NormalAttack :
    public InterAttack
{
public:
	void Init() override;

	void Upda() override;

	void Draw() override;

	void SpriteDraw() override;
private:
	
};

