#pragma once
#include "UnindexedObject.h"
class Unicorn : public UnindexedObject
{
public:
	Unicorn(NotIndexedTexturedMesh* texMesh, Texture2D* texture, Vector3 startPos, Vector3 scale);
	~Unicorn();

	void Update(float deltaTime);
	void Display();

	void SetDeathState(bool state);

	float GetDeathRotation();

private:
	float mDirectionTimer;
	float mPauseTimer;

	Vector3 mMoveVector;

	bool mDead;

	float mDeathRotation;

	float mDirectionFacingAngle;
	float MpreviousDirectionFacingAngle;

	float mGravityForce;

	void GenerateDirection();
};

