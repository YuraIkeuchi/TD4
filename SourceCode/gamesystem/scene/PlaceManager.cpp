#include "PlaceManager.h"

#include "CollisionAttribute.h"
#include "SphereCollider.h"


void PlaceManager::ArgmentObj(bool& aflag, IKEModel* model)
{
	if (!aflag)return;

	std::unique_ptr<IKEObject3d> newobj;
	newobj = std::make_unique<IKEObject3d>();

	newobj->Initialize();
	newobj->SetModel(model);

	newobj->SetPosition(S_Pos);
	float radius = 5.0f;
	newobj->SetCollider(new SphereCollider(XMVECTOR({ 10, radius, 10, 0 }), radius));
	newobj->collider->SetAttribute(COLLISION_ATTR_ALLIES);

	boxes.push_back(std::move(newobj));
	aflag = false;
}
