#define _USE_MATH_DEFINES	//Allowing use of PI

#include "Unicorn.h"
#include "Constants.h"


Unicorn::Unicorn(NotIndexedTexturedMesh* texMesh, Texture2D* texture, Vector3 startPos, Vector3 scale) : UnindexedObject(texMesh, texture, startPos, scale)
{
	mMoveVector.y = 0.0f;
	mPauseTimer = 0;
	mGravityForce = INITIAL_GRAVITY_FORCE;

	mPreviousDirectionFacingAngle = 0.0f;

	GenerateDirection();
}


Unicorn::~Unicorn()
{
}

void Unicorn::Update(float deltaTime)
{
	if (mPosition.y > -6.0f)	//Adding gravity
	{
		mPosition.y -= mGravityForce * deltaTime;
		mGravityForce += GRAVITY_FORCE_DECREMENT * deltaTime;
	}
	else    //Movement once on the floor
	{
		if (mDirectionTimer <= 0.0f)    //If time to change direction
		{
			if (mPauseTimer <= 0.0f)    //If paused for long enough
			{
				mDirectionTimer = rand() % 2000;	//Time between 2 and 0 seconds

				GenerateDirection();
			}
			else
			{
				mPauseTimer -= deltaTime;
			}
		}
		else    //If not keep moving and update collider
		{
			mDirectionTimer -= deltaTime;

			mPosition.x += mMoveVector.x;
			mPosition.z += mMoveVector.z;

            //Update collider
			for (int i = 0; i < 8; i++)
			{
				mCollider.vertices[i] = mCollider.vertices[i] + mMoveVector;
			}

			mCollider.max = mCollider.max + mMoveVector;
			mCollider.min = mCollider.min + mMoveVector;

			mPauseTimer = rand() % 2000;
		}

		if (mPosition.x <= 10.0f || mPosition.x >= 85.0f || mPosition.z <= -60.0f || mPosition.z >= 15.0f)  //If at boundaries
		{
			GenerateDirection();

			mPauseTimer = rand() % 2000;
			mDirectionTimer = rand() % 2000;
		}

		if (mDead)
		{
			mDeathRotation += 2.0f;
		}
	}
}

void Unicorn::Display()
{

	glPushMatrix();

	glScalef(mScale.x, mScale.y, mScale.z);
	glTranslatef(mPosition.x, mPosition.y, mPosition.z);
	glRotatef(mDirectionFacingAngle, 0, 1, 0);

	UnindexedObject::Display();

	glPopMatrix();

	glPointSize(20);

	glBegin(GL_POINTS);

	glVertex3f(mCollider.max.x, mCollider.max.y, mCollider.max.z);
	glColor3f(1.0f, 1.0f, 1.0f);

	glVertex3f(mCollider.min.x, mCollider.min.y, mCollider.min.z);
	glColor3f(1.0f, 1.0f, 1.0f);

	glEnd();
}

void Unicorn::SetDeathState(bool state)
{
	mDead = state;
}

float Unicorn::GetDeathRotation()
{
	return mDeathRotation;
}

void Unicorn::GenerateDirection()
{
    //Create random forward directions
	float angle = rand() % 361;

	mMoveVector.x = sin((angle * M_PI) / 180.0f) * 0.1f;

	angle = rand() % 361;

	mMoveVector.z = -cos((angle * M_PI) / 180.0f) * 0.1f;

    //Get angle between new forward vector and old forward vector
	mPreviousDirectionFacingAngle = mDirectionFacingAngle;

	mDirectionFacingAngle = acos((mMoveVector.z) / sqrt((pow(mMoveVector.x, 2) + pow(mMoveVector.z, 2))));

	if (mMoveVector.x < 0)
	{
		mDirectionFacingAngle = (2 * M_PI) - mDirectionFacingAngle;
	}

    //Creating rotation matrix
	float rotationMatrix[3][3] =
	{
	{ cos(mDirectionFacingAngle - mPreviousDirectionFacingAngle), 0.0f, sin(mDirectionFacingAngle - mPreviousDirectionFacingAngle) },
	{ 0.0f, 1.0f, 0.0f },
	{ -sin(mDirectionFacingAngle - mPreviousDirectionFacingAngle), 0.0f, cos(mDirectionFacingAngle - mPreviousDirectionFacingAngle) }
	};

	mDirectionFacingAngle = mDirectionFacingAngle * 180 / M_PI;     //Convert to degrees

	for (int i = 0; i < 8; i++)	//Using transformation matrix on each vertices
	{
		mCollider.vertices[i].x = (mCollider.vertices[i].x * rotationMatrix[0][0]) + (mCollider.vertices[i].z * rotationMatrix[0][2]);
		mCollider.vertices[i].y = mCollider.vertices[i].y * rotationMatrix[1][2];
		mCollider.vertices[i].z = (mCollider.vertices[i].x * rotationMatrix[2][0]) + (mCollider.vertices[i].z * rotationMatrix[2][2]);
	}

	mCollider.Recalculate();    //Recalculate collider
}
