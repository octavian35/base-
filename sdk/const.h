#pragma once

static int seeds[] = {
0x2 ,
0x3 ,
0x4 ,
0x0A,
0x0D,
0x0F,
0x1B,
0x1D,
0x25,
0x26,
0x29,
0x2E,
0x2F,
0x31,
0x32,
0x34,
0x38,
0x39,
0x3A,
0x3E,
0x42,
0x44,
0x46,
0x48,
0x4C,
0x53,
0x57,
0x61,
0x6C,
0x6D,
0x6E,
0x6F,
0x71,
0x79,
0x7A,
0x7D,
0x87,
0x1 ,
0x5 ,
0x6 ,
0x8 ,
0x11,
0x19,
0x1A,
0x1C,
0x1F,
0x20,
0x24,
0x27,
0x2C,
0x33,
0x3D,
0x43,
0x47,
0x4A,
0x4D,
0x4E,
0x4F,
0x50,
0x51,
0x54,
0x59,
0x5A,
0x5B,
0x5C,
0x60,
0x62,
0x65,
0x66,
0x68,
0x6B,
0x72,
0x73,
0x77,
0x9 ,
0x0B,
0x10,
0x13,
0x14,
0x16,
0x1E,
0x30,
0x36,
0x3B,
0x3C,
0x41,
0x45,
0x49,
0x4B,
0x52,
0x55,
0x56,
0x58,
0x5E,
0x63,
0x67,
0x69,
0x70,
0x74,
0x75,
0x76,
0x7C,
0x0 ,
0x7 ,
0x0C,
0x0E,
0x12,
0x15,
0x17,
0x18,
0x21,
0x22,
0x23,
0x28,
0x2A,
0x2B,
0x2D,
0x35,
0x37,
0x3F,
0x40,
0x5D,
0x5F,
0x64,
0x6A,
0x78,
0x85,
0x89
};

enum ECSClientClass
{
	CTestTraceline = 156,
	CTEWorldDecal = 157,
	CTESpriteSpray = 154,
	CTESprite = 153,
	CTESparks = 152,
	CTESmoke = 151,
	CTEShowLine = 149,
	CTEProjectedDecal = 146,
	CTEPlayerDecal = 145,
	CTEPhysicsProp = 142,
	CTEParticleSystem = 141,
	CTEMuzzleFlash = 140,
	CTELargeFunnel = 138,
	CTEKillPlayerAttachments = 137,
	CTEImpact = 136,
	CTEGlowSprite = 135,
	CTEShatterSurface = 148,
	CTEFootprintDecal = 133,
	CTEFizz = 132,
	CTEExplosion = 130,
	CTEEnergySplash = 129,
	CTEEffectDispatch = 128,
	CTEDynamicLight = 127,
	CTEDecal = 125,
	CTEClientProjectile = 124,
	CTEBubbleTrail = 123,
	CTEBubbles = 122,
	CTEBSPDecal = 121,
	CTEBreakModel = 120,
	CTEBloodStream = 119,
	CTEBloodSprite = 118,
	CTEBeamSpline = 117,
	CTEBeamRingPoint = 116,
	CTEBeamRing = 115,
	CTEBeamPoints = 114,
	CTEBeamLaser = 113,
	CTEBeamFollow = 112,
	CTEBeamEnts = 111,
	CTEBeamEntPoint = 110,
	CTEBaseBeam = 109,
	CTEArmorRicochet = 108,
	CTEMetalSparks = 139,
	CSteamJet = 104,
	CSmokeStack = 99,
	DustTrail = 188,
	CFireTrail = 47,
	SporeTrail = 195,
	SporeExplosion = 194,
	RocketTrail = 192,
	SmokeTrail = 193,
	CPropVehicleDriveable = 90,
	ParticleSmokeGrenade = 191,
	CParticleFire = 73,
	MovieExplosion = 189,
	CTEGaussExplosion = 134,
	CEnvQuadraticBeam = 41,
	CEmbers = 34,
	CEnvWind = 45,
	CPrecipitation = 87,
	CBaseTempEntity = 17,
	CWeaponXM1014 = 186,
	CWeaponUSP = 185,
	CWeaponUMP45 = 184,
	CWeaponTMP = 183,
	CSmokeGrenade = 98,
	CWeaponSG552 = 182,
	CWeaponSG550 = 181,
	CWeaponScout = 180,
	CWeaponP90 = 179,
	CWeaponP228 = 178,
	CWeaponMP5Navy = 177,
	CWeaponMAC10 = 176,
	CWeaponM4A1 = 175,
	CWeaponM3 = 174,
	CWeaponM249 = 173,
	CKnife = 70,
	CHEGrenade = 65,
	CWeaponGlock = 172,
	CWeaponGalil = 171,
	CWeaponG3SG1 = 170,
	CFlashbang = 49,
	CWeaponFiveSeven = 169,
	CWeaponFamas = 168,
	CWeaponElite = 167,
	CDEagle = 31,
	CWeaponCSBaseGun = 165,
	CWeaponCSBase = 164,
	CC4 = 23,
	CBaseCSGrenade = 6,
	CWeaponAWP = 163,
	CWeaponAug = 162,
	CAK47 = 1,
	NextBotCombatCharacter = 190,
	CFootstepControl = 51,
	CCSGameRulesProxy = 26,
	CWeaponCubemap = 0,
	CWeaponCycler = 166,
	CTEPlantBomb = 143,
	CTEFireBullets = 131,
	CTERadioIcon = 147,
	CPlantedC4 = 81,
	CCSTeam = 30,
	CCSPlayerResource = 28,
	CCSPlayer = 27,
	CCSRagdoll = 29,
	CTEPlayerAnimEvent = 144,
	CHostage = 66,
	CBaseCSGrenadeProjectile = 7,
	CHandleTest = 64,
	CTeamplayRoundBasedRulesProxy = 107,
	CSpriteTrail = 103,
	CSpriteOriented = 102,
	CSprite = 101,
	CRagdollPropAttached = 93,
	CRagdollProp = 92,
	CPredictedViewModel = 89,
	CPoseController = 86,
	CGameRulesProxy = 63,
	CInfoLadderDismount = 67,
	CFuncLadder = 56,
	CEnvDetailController = 38,
	CWorld = 261,
	CWaterLODControl = 161,
	CWaterBullet = 160,
	CVoteController = 159,
	CVGuiScreen = 158,
	CPropJeep = 89,
	CPropVehicleChoreoGeneric = 0,
	CTest_ProxyToggle_Networkable = 155,
	CTesla = 150,
	CTeamTrainWatcher = 0,
	CBaseTeamObjectiveResource = 16,
	CTeam = 106,
	CSun = 105,
	CParticlePerformanceMonitor = 74,
	CSpotlightEnd = 100,
	CSlideshowDisplay = 97,
	CShadowControl = 96,
	CSceneEntity = 95,
	CRopeKeyframe = 94,
	CRagdollManager = 91,
	CPhysicsPropMultiplayer = 79,
	CPhysBoxMultiplayer = 77,
	CBasePropDoor = 15,
	CDynamicProp = 33,
	CPointCommentaryNode = 85,
	CPointCamera = 84,
	CPlayerResource = 83,
	CPlasma = 82,
	CPhysMagnet = 80,
	CPhysicsProp = 78,
	CPhysBox = 76,
	CParticleSystem = 75,
	CMaterialModifyControl = 72,
	CLightGlow = 71,
	CInfoOverlayAccessor = 69,
	CFuncTrackTrain = 62,
	CFuncSmokeVolume = 61,
	CFuncRotating = 60,
	CFuncReflectiveGlass = 59,
	CFuncOccluder = 58,
	CFuncMonitor = 57,
	CFunc_LOD = 53,
	CTEDust = 126,
	CFunc_Dust = 52,
	CFuncConveyor = 55,
	CBreakableSurface = 22,
	CFuncAreaPortalWindow = 54,
	CFish = 48,
	CEntityFlame = 36,
	CFireSmoke = 46,
	CEnvTonemapController = 44,
	CEnvScreenEffect = 42,
	CEnvScreenOverlay = 43,
	CEnvProjectedTexture = 40,
	CEnvParticleScript = 39,
	CFogController = 50,
	CEntityParticleTrail = 37,
	CEntityDissolve = 35,
	CDynamicLight = 32,
	CColorCorrectionVolume = 25,
	CColorCorrection = 24,
	CBreakableProp = 21,
	CBasePlayer = 13,
	CBaseFlex = 10,
	CBaseEntity = 9,
	CBaseDoor = 8,
	CBaseCombatCharacter = 4,
	CBaseAnimatingOverlay = 3,
	CBoneFollower = 20,
	CBaseAnimating = 2,
	CInfoLightingRelative = 68,
	CAI_BaseNPC = 0,
	CBeam = 19,
	CBaseViewModel = 18,
	CBaseProjectile = 14,
	CBaseParticleEntity = 12,
	CBaseGrenade = 11,
	CBaseCombatWeapon = 5
};

#define	CONTENTS_EMPTY			0		// No contents

#define	CONTENTS_SOLID			0x1		// an eye is never valid in a solid
#define	CONTENTS_WINDOW			0x2		// translucent, but not watery (glass)
#define	CONTENTS_AUX			0x4
#define	CONTENTS_GRATE			0x8		// alpha-tested "grate" textures.  Bullets/sight pass through, but solids don't
#define	CONTENTS_SLIME			0x10
#define	CONTENTS_WATER			0x20
#define	CONTENTS_BLOCKLOS		0x40	// block AI line of sight
#define CONTENTS_OPAQUE			0x80	// things that cannot be seen through (may be non-solid though)
#define	LAST_VISIBLE_CONTENTS	0x80

#define ALL_VISIBLE_CONTENTS (LAST_VISIBLE_CONTENTS | (LAST_VISIBLE_CONTENTS-1))

#define CONTENTS_TESTFOGVOLUME	0x100
#define CONTENTS_UNUSED			0x200	

// unused 
// NOTE: If it's visible, grab from the top + update LAST_VISIBLE_CONTENTS
// if not visible, then grab from the bottom.
#define CONTENTS_UNUSED6		0x400

#define CONTENTS_TEAM1			0x800	// per team contents used to differentiate collisions 
#define CONTENTS_TEAM2			0x1000	// between players and objects on different teams

// ignore CONTENTS_OPAQUE on surfaces that have SURF_NODRAW
#define CONTENTS_IGNORE_NODRAW_OPAQUE	0x2000

// hits entities which are MOVETYPE_PUSH (doors, plats, etc.)
#define CONTENTS_MOVEABLE		0x4000

// remaining contents are non-visible, and don't eat brushes
#define	CONTENTS_AREAPORTAL		0x8000

#define	CONTENTS_PLAYERCLIP		0x10000
#define	CONTENTS_MONSTERCLIP	0x20000

// currents can be added to any other contents, and may be mixed
#define	CONTENTS_CURRENT_0		0x40000
#define	CONTENTS_CURRENT_90		0x80000
#define	CONTENTS_CURRENT_180	0x100000
#define	CONTENTS_CURRENT_270	0x200000
#define	CONTENTS_CURRENT_UP		0x400000
#define	CONTENTS_CURRENT_DOWN	0x800000

#define	CONTENTS_ORIGIN			0x1000000	// removed before bsping an entity

#define	CONTENTS_MONSTER		0x2000000	// should never be on a brush, only in game
#define	CONTENTS_DEBRIS			0x4000000
#define	CONTENTS_DETAIL			0x8000000	// brushes to be added after vis leafs
#define	CONTENTS_TRANSLUCENT	0x10000000	// auto set if any surface has trans
#define	CONTENTS_LADDER			0x20000000
#define CONTENTS_HITBOX			0x40000000	// use accurate hitboxes on trace


// NOTE: These are stored in a short in the engine now.  Don't use more than 16 bits
#define	SURF_LIGHT		0x0001		// value will hold the light strength
#define	SURF_SKY2D		0x0002		// don't draw, indicates we should skylight + draw 2d sky but not draw the 3D skybox
#define	SURF_SKY		0x0004		// don't draw, but add to skybox
#define	SURF_WARP		0x0008		// turbulent water warp
#define	SURF_TRANS		0x0010
#define SURF_NOPORTAL	0x0020	// the surface can not have a portal placed on it
#define	SURF_TRIGGER	0x0040	// FIXME: This is an xbox hack to work around elimination of trigger surfaces, which breaks occluders
#define	SURF_NODRAW		0x0080	// don't bother referencing the texture

#define	SURF_HINT		0x0100	// make a primary bsp splitter

#define	SURF_SKIP		0x0200	// completely ignore, allowing non-closed brushes
#define SURF_NOLIGHT	0x0400	// Don't calculate light
#define SURF_BUMPLIGHT	0x0800	// calculate three lightmaps for the surface for bumpmapping
#define SURF_NOSHADOWS	0x1000	// Don't receive shadows
#define SURF_NODECALS	0x2000	// Don't receive decals
#define SURF_NOCHOP		0x4000	// Don't subdivide patches on this surface 
#define SURF_HITBOX		0x8000	// surface is part of a hitbox

#define	MASK_ALL					(0xFFFFFFFF)
// everything that is normally solid
#define	MASK_SOLID					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
// everything that blocks player movement
#define	MASK_PLAYERSOLID			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
// blocks npc movement
#define	MASK_NPCSOLID				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
// water physics in these contents
#define	MASK_WATER					(CONTENTS_WATER|CONTENTS_MOVEABLE|CONTENTS_SLIME)
// everything that blocks lighting
#define	MASK_OPAQUE					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_OPAQUE)
// everything that blocks lighting, but with monsters added.
#define MASK_OPAQUE_AND_NPCS		(MASK_OPAQUE|CONTENTS_MONSTER)
// everything that blocks line of sight for AI
#define MASK_BLOCKLOS				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_BLOCKLOS)
// everything that blocks line of sight for AI plus NPCs
#define MASK_BLOCKLOS_AND_NPCS		(MASK_BLOCKLOS|CONTENTS_MONSTER)
// everything that blocks line of sight for players
#define	MASK_VISIBLE					(MASK_OPAQUE|CONTENTS_IGNORE_NODRAW_OPAQUE)
// everything that blocks line of sight for players, but with monsters added.
#define MASK_VISIBLE_AND_NPCS		(MASK_OPAQUE_AND_NPCS|CONTENTS_IGNORE_NODRAW_OPAQUE)
// bullets see these as solid
#define	MASK_SHOT					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX)
// non-raycasted weapons see this as solid (includes grates)
#define MASK_SHOT_HULL				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_GRATE)
// hits solids (not grates) and passes through everything else
#define MASK_SHOT_PORTAL			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER)
// everything normally solid, except monsters (world+brush only)
#define MASK_SOLID_BRUSHONLY		(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_GRATE)
// everything normally solid for player movement, except monsters (world+brush only)
#define MASK_PLAYERSOLID_BRUSHONLY	(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_PLAYERCLIP|CONTENTS_GRATE)
// everything normally solid for npc movement, except monsters (world+brush only)
#define MASK_NPCSOLID_BRUSHONLY		(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
// just the world, used for route rebuilding
#define MASK_NPCWORLDSTATIC			(CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
// These are things that can split areaportals
#define MASK_SPLITAREAPORTAL		(CONTENTS_WATER|CONTENTS_SLIME)

// UNDONE: This is untested, any moving water
#define MASK_CURRENT				(CONTENTS_CURRENT_0|CONTENTS_CURRENT_90|CONTENTS_CURRENT_180|CONTENTS_CURRENT_270|CONTENTS_CURRENT_UP|CONTENTS_CURRENT_DOWN)

// everything that blocks corpse movement
// UNDONE: Not used yet / may be deleted
#define	MASK_DEADSOLID				(CONTENTS_SOLID|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_GRATE)