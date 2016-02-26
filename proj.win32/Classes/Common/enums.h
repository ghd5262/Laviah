typedef enum eOBJECT_TAG{
	eOBJECT_TAG_none = -1,
	eOBJECT_TAG_soccerPitch = 0,
	eOBJECT_TAG_myTeam = 1,
	eOBJECT_TAG_otherTeam = 2,
	eOBJECT_TAG_soccerBall = 3,

	eOBJECT_TAG_max
};

typedef enum eOBJECT_Z_ORDER{
	eOBJECT_Z_ORDER_none = 0,
	eOBJECT_Z_ORDER_soccerPitch = 1,
	eOBJECT_Z_ORDER_soccerTeam = 2,
	eOBJECT_Z_ORDER_soccerBall = 3,

	eOBJECT_Z_ORDER_popup = 100,
};