#pragma once
#include "ParticleManager.h"

using namespace std;         //  ���O��Ԏw��
/// <summary>
/// �p�[�e�B�N���G�~�b�^�[
/// </summary>
class ParticleEmitter
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: //�V���O���g����
	//�R���X�g���N�^���B��
	ParticleEmitter() = default;
	//�f�X�g���N�^���B��
	~ParticleEmitter() = default;
public: //�����o�֐�
	static ParticleEmitter* GetInstance();

	//������
	void Initialize();

	//�X�V
	void Update();

	//�`��
	void FlontDrawAll();
	//�G�t�F�N�g
	void FireEffect(int life,XMFLOAT3 l_pos,float startscale,float endscale,XMFLOAT4 startcolor,XMFLOAT4 endcolor);
	//�폜
	void AllDelete();

private: //�����o�֐�
	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	void LoadTexture();

private: //�����o�ϐ�
	//�p�[�e�B�N���}�l�[�W���[(��)
	unique_ptr<ParticleManager> circleParticle;
};
