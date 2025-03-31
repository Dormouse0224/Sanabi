#pragma once

union COLLISION_ID
{
	struct
	{
		UINT left;
		UINT right;
	};

	DWORD_PTR ID;
};



class CCollider2D;

class CCollisionMgr
	: public Singleton<CCollisionMgr>
{
	SINGLE(CCollisionMgr);
private:
	UINT				 m_Matrix[(UINT)LAYER::END];
	map<DWORD_PTR, bool> m_mapInfo;


//public:
//	void Tick();
//	void CollisionLayerCheck(UINT _Left, UINT _Right);
//
//private:
//	void CollisionLayer(UINT _Left, UINT _Right);
//	bool IsOverlap(CCollider2D* _LeftCol, CCollider2D* _RightCol);

};

