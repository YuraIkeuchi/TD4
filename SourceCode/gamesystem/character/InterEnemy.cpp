#include "InterEnemy.h"
//Player* InterEnemy::player = nullptr;
//������
bool InterEnemy::Initialize() {
	return true;
}
//�X�V
void InterEnemy::Update() {
	//�s��
	Action();
}
//�`��
void InterEnemy::Draw(DirectXCommon* dxCommon) {
}
//void InterEnemy::PlayerGet(Player* _player) {
//
//	//InterEnemy::player = _player;
//
//}