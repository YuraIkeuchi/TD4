#include "ObjCommon.h"
#include"SphereCollider.h"
#include"CollisionAttribute.h"
#include"TouchableObject.h"
#include"CollisionManager.h"
#include"Collision.h"

ObjCommon::~ObjCommon()
{
	//Destroy_unique(m_Object);
	//Destroy_unique(m_fbxObject);
	////Destroy(m_Model);
	////Destroy(m_fbxModel);
}

bool ObjCommon::Initialize() {
	return true;
}

void ObjCommon::SetCollider()
{
	//float radius = 5.0f;
	//m_Object->SetCollider(new SphereCollider(XMVECTOR({ 0,radius,0,0 }), radius));
	//m_Object->collider->SetAttribute(COLLISION_ATTR_ALLIES);
}
//オブジェクトのセット
void ObjCommon::Obj_SetParam() {
	m_Object->SetPosition(m_Position);
	m_Object->SetRotation(m_Rotation);
	m_Object->SetScale(m_Scale);
	m_Object->SetColor(m_Color);
	m_Object->SetAddColor(m_Addcolor);
	m_Object->SetDisolve(m_AddDisolve);
	m_Object->SetChangeColor(m_ChangeColor);
	m_Object->Update();
	UpdateWorldMatrix();
}

//オブジェクトのセット
void ObjCommon::FollowObj_SetParam(XMMATRIX matworld) {
	//m_FollowObject->SetScale(m_Scale);
	//m_FollowObject->SetColor(m_Color);
	m_FollowObject->SetAddColor(m_Addcolor);
	m_FollowObject->SetDisolve(m_AddDisolve);
	m_FollowObject->SetChangeColor(m_ChangeColor);
	m_FollowObject->FollowUpdate(matworld);
	UpdateWorldMatrix();
}

//FBXのセット
void ObjCommon::Fbx_SetParam() {
	m_fbxObject->SetPosition(m_Position);
	m_fbxObject->SetRotation(m_Rotation);
	//m_fbxObject->SetScale(m_Scale);
	m_fbxObject->SetAddColor(m_Addcolor);
	m_fbxObject->SetDisolve(m_AddDisolve);
	m_fbxObject->SetChangeColor(m_ChangeColor);
}

void ObjCommon::Obj_Draw()
{
	IKEObject3d::PreDraw();
	m_Object->Draw();
	IKEObject3d::PostDraw();
}

void ObjCommon::FollowObj_Draw()
{
	IKEObject3d::PreDraw();
	m_FollowObject->Draw();
	IKEObject3d::PostDraw();
}

void ObjCommon::Fbx_Draw(DirectXCommon* dxCommon)
{
	m_fbxObject->Draw(dxCommon->GetCmdList());
}

