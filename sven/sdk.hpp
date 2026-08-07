#pragma once

// so much slop shamelessly pasted from hl1 sdk on github

#define	FL_FLY					(1<<0)	// Changes the SV_Movestep() behavior to not need to be on ground
#define	FL_SWIM					(1<<1)	// Changes the SV_Movestep() behavior to not need to be on ground (but stay in water)
#define	FL_CONVEYOR				(1<<2)
#define	FL_CLIENT				(1<<3)
#define	FL_INWATER				(1<<4)
#define	FL_MONSTER				(1<<5)
#define	FL_GODMODE				(1<<6)
#define	FL_NOTARGET				(1<<7)
#define	FL_SKIPLOCALHOST		(1<<8)	// Don't send entity to local host, it's predicting this entity itself
#define	FL_ONGROUND				(1<<9)	// At rest / on the ground
#define	FL_PARTIALGROUND		(1<<10)	// not all corners are valid
#define	FL_WATERJUMP			(1<<11)	// player jumping out of water
#define FL_FROZEN				(1<<12) // Player is frozen for 3rd person camera
#define FL_FAKECLIENT			(1<<13)	// JAC: fake client, simulated server side; don't send network messages to them
#define FL_DUCKING				(1<<14)	// Player flag -- Player is fully crouched
#define FL_FLOAT				(1<<15)	// Apply floating force to this entity when in water
#define FL_GRAPHED				(1<<16) // worldgraph has this ent listed as something that blocks a connection

#define IN_ATTACK	(1 << 0)
#define IN_JUMP		(1 << 1)
#define IN_DUCK		(1 << 2)
#define IN_FORWARD	(1 << 3)
#define IN_BACK		(1 << 4)
#define IN_USE		(1 << 5)
#define IN_CANCEL	(1 << 6)
#define IN_LEFT		(1 << 7)
#define IN_RIGHT	(1 << 8)
#define IN_MOVELEFT	(1 << 9)
#define IN_MOVERIGHT (1 << 10)
#define IN_ATTACK2	(1 << 11)
#define IN_RUN      (1 << 12)
#define IN_RELOAD	(1 << 13)
#define IN_ALT1		(1 << 14)
#define IN_SCORE	(1 << 15)   // Used by client.dll for when scoreboard is held down

typedef int qboolean;

#define qfalse 0
#define qtrue  1

struct vec3_t
{
	float x, y, z;

	inline float Length(void) const { return sqrt(x * x + y * y + z * z); }
};

struct entity_state_s
{
	// Fields which are filled in by routines outside of delta compression
	int			entityType;
	// Index into cl_entities array for this entity.
	int			number;
	float		msg_time;

	// Message number last time the player/entity state was updated.
	int			messagenum;

	// Fields which can be transitted and reconstructed over the network stream
	vec3_t		origin;
	vec3_t		angles;

	int			modelindex;
	int			sequence;
	float		frame;
	int			colormap;
	short		skin;
	short		solid;
	int			effects;
	float		scale;

	byte		eflags;

	// Render information
	int			rendermode;
	int			renderamt;
	DWORD* rendercolor;
	int			renderfx;

	int			movetype;
	float		animtime;
	float		framerate;
	int			body;
	byte		controller[4];
	byte		blending[4];
	vec3_t		velocity;

	// Send bbox down to client for use during prediction.
	vec3_t		mins;
	vec3_t		maxs;

	int			aiment;
	// If owned by a player, the index of that player ( for projectiles ).
	int			owner;

	// Friction, for prediction.
	float		friction;
	// Gravity multiplier
	float		gravity;

	// PLAYER SPECIFIC
	int			team;
	int			playerclass;
	int			health;
	qboolean	spectator;
	int         weaponmodel;
	int			gaitsequence;
	// If standing on conveyor, e.g.
	vec3_t		basevelocity;
	// Use the crouched hull, or the regular player hull.
	int			usehull;
	// Latched buttons last time state updated.
	int			oldbuttons;
	// -1 = in air, else pmove entity number
	int			onground;
	int			iStepLeft;
	// How fast we are falling
	float		flFallVelocity;

	float		fov;
	int			weaponanim;

	// Parametric movement overrides
	vec3_t				startpos;
	vec3_t				endpos;
	float				impacttime;
	float				starttime;

	// For mods
	int			iuser1;
	int			iuser2;
	int			iuser3;
	int			iuser4;
	float		fuser1;
	float		fuser2;
	float		fuser3;
	float		fuser4;
	vec3_t		vuser1;
	vec3_t		vuser2;
	vec3_t		vuser3;
	vec3_t		vuser4;
};
typedef struct entity_state_s entity_state_t;

typedef struct
{
	byte					mouthopen;		// 0 = mouth closed, 255 = mouth agape
	byte					sndcount;		// counter for running average
	int						sndavg;			// running average
} mouth_t;

typedef struct
{
	float					prevanimtime;
	float					sequencetime;
	byte					prevseqblending[2];
	vec3_t					prevorigin;
	vec3_t					prevangles;

	int						prevsequence;
	float					prevframe;

	byte					prevcontroller[4];
	byte					prevblending[2];
} latchedvars_t;

typedef struct
{
	// Time stamp for this movement
	float					animtime;

	vec3_t					origin;
	vec3_t					angles;
} position_history_t;

typedef struct cl_entity_s cl_entity_t;

#define HISTORY_MAX		64  // Must be power of 2
#define HISTORY_MASK	( HISTORY_MAX - 1 )

struct cl_entity_s
{
	int						index;      // Index into cl_entities ( should match actual slot, but not necessarily )

	qboolean				player;     // True if this entity is a "player"

	entity_state_t			baseline;   // The original state from which to delta during an uncompressed message
	entity_state_t			prevstate;  // The state information from the penultimate message received from the server
	entity_state_t			curstate;   // The state information from the last message received from server

	int						current_position;  // Last received history update index
	position_history_t		ph[HISTORY_MAX];   // History of position and angle updates for this player

	mouth_t					mouth;			// For synchronizing mouth movements.

	latchedvars_t			latched;		// Variables used by studio model rendering routines

	// Information based on interplocation, extrapolation, prediction, or just copied from last msg received.
	//
	float					lastmove;

	// Actual render position and angles
	vec3_t					origin;
	vec3_t					angles;

	// Attachment points
	vec3_t					attachment[4];

	// Other entity local information
	int						trivial_accept;

	struct model_s* model;			// cl.model_precache[ curstate.modelindes ];  all visible entities have a model
	struct efrag_s* efrag;			// linked list of efrags
	struct mnode_s* topnode;		// for bmodels, first world node that splits bmodel, or NULL if not split

	float					syncbase;		// for client-side animations -- used by obsolete alias animation system, remove?
	int						visframe;		// last frame this entity was found in an active leaf
	DWORD* cvFloorColor;
};

typedef struct usercmd_s
{
	short	lerp_msec;      // Interpolation time on client
	byte	msec;           // Duration in ms of command
	vec3_t	viewangles;     // Command view angles.

	// intended velocities
	float	forwardmove;    // Forward velocity.
	float	sidemove;       // Sideways velocity.
	float	upmove;         // Upward velocity.
	byte	lightlevel;     // Light level at spot where we are standing.
	unsigned short  buttons;  // Attack buttons
	byte    impulse;          // Impulse command issued.
	byte	weaponselect;	// Current weapon id

	// Experimental player impact stuff.
	int		impact_index;
	vec3_t	impact_position;
} usercmd_t;

typedef struct cl_enginefuncs_s {
	void* pfnSPR_Load;
	void* pfnSPR_Frames;
	void* pfnSPR_Height;
	void* pfnSPR_Width;
	void* pfnSPR_Set;
	void* pfnSPR_Draw;
	void* pfnSPR_DrawHoles;
	void* pfnSPR_DrawAdditive;
	void* pfnSPR_EnableScissor;
	void* pfnSPR_DisableScissor;
	void* pfnSPR_GetList;
	void* pfnFillRGBA;
	void* pfnGetScreenInfo;
	void* pfnSetCrosshair;
	void* pfnRegisterVariable;
	void* pfnGetCvarFloat;
	void* pfnGetCvarString;
	void* pfnAddCommand;
	void* pfnHookUserMsg;
	void* pfnServerCmd;
	void* pfnClientCmd;
	void* pfnGetPlayerInfo;
	void* pfnPlaySoundByName;
	void* pfnPlaySoundByIndex;
	void* pfnAngleVectors;
	void* pfnTextMessageGet;
	void* pfnDrawCharacter;
	int* (__cdecl* pfnDrawConsoleString)(int x, int y, char* string);
	void* (__cdecl* pfnDrawSetTextColor)(float r, float g, float b);
	void* pfnDrawConsoleStringLen;
	void* pfnConsolePrint;
	void* pfnCenterPrint;
	void* GetWindowCenterX;
	void* GetWindowCenterY;
	void* GetViewAngles;
	void* SetViewAngles;
	void* GetMaxClients;
	void* Cvar_SetValue;
	void* Cmd_Argc;
	void* Cmd_Argv;
	void* Con_Printf;
	void* Con_DPrintf;
	void* Con_NPrintf;
	void* Con_NXPrintf;
	void* PhysInfo_ValueForKey;
	void* ServerInfo_ValueForKey;
	void* GetClientMaxspeed;
	void* CheckParm;
	void* Key_Event;
	void* GetMousePosition;
	void* IsNoClipping;
	cl_entity_s* (__cdecl* GetLocalPlayer)(void);
	void* GetViewModel;
	cl_entity_s* (__cdecl* GetEntityByIndex)(int* index);
	void* GetClientTime;
	void* V_CalcShake;
	void* V_ApplyShake;
	void* PM_PointContents;
	void* PM_WaterEntity;
	void* PM_TraceLine;
	void* CL_LoadModel;
	void* CL_CreateVisibleEntity;
	void* GetSpritePointer;
	void* pfnPlaySoundByNameAtLocation;
	void* pfnPrecacheEvent;
	void* pfnPlaybackEvent;
	void* pfnWeaponAnim;
	void* pfnRandomFloat;
	void* pfnRandomLong;
	void* pfnHookEvent;
	void* Con_IsVisible;
	void* pfnGetGameDirectory;
	void* pfnGetCvarPointer;
	void* Key_LookupBinding;
	void* pfnGetLevelName;
	void* pfnGetScreenFade;
	void* pfnSetScreenFade;
	void* VGui_GetPanel;
	void* VGui_ViewportPaintBackground;
	void* COM_LoadFile;
	void* COM_ParseFile;
	void* COM_FreeFile;
	void* pTriAPI;
	void* pEfxAPI;
	void* pEventAPI;
	void* pDemoAPI;
	void* pNetAPI;
	void* pVoiceTweak;
	void* IsSpectateOnly;
	void* LoadMapSprite;
	void* COM_AddAppDirectoryToSearchPath;
	void* COM_ExpandFilename;
	void* PlayerInfo_ValueForKey;
	void* PlayerInfo_SetValueForKey;
	void* GetPlayerUniqueID;
	void* GetTrackerIDForPlayer;
	void* GetPlayerForTrackerID;
	void* pfnServerCmdUnreliable;
	void* pfnGetMousePos;
	void* pfnSetMousePos;
	void* pfnSetMouseEnable;
	void* GetFirstCvarPtr;
	void* GetFirstCmdFunctionHandle;
	void* GetNextCmdFunctionHandle;
	void* GetCmdFunctionName;
	void* hudGetClientOldTime;
	void* hudGetServerGravityValue;
	void* hudGetModelByIndex;
	void* pfnSetFilterMode;
	void* pfnSetFilterColor;
	void* pfnSetFilterBrightness;
	void* pfnSequenceGet;
	void* pfnSPR_DrawGeneric;
	void* pfnSequencePickSentence;
	void* pfnDrawString;
	void* pfnDrawStringReverse;
	void* LocalPlayerInfo_ValueForKey;
	void* pfnVGUI2DrawCharacter;
	void* pfnVGUI2DrawCharacterAdd;
	void* COM_GetApproxWavePlayLength;
	void* pfnGetCareerUI;
	void* Cvar_Set;
	void* pfnIsCareerMatch;
	void* pfnPlaySoundVoiceByName;
	void* pfnPrimeMusicStream;
	void* GetAbsoluteTime;
	void* pfnProcessTutorMessageDecayBuffer;
	void* pfnConstructTutorMessageDecayBuffer;
	void* pfnResetTutorMessageDecayData;
	void* pfnPlaySoundByNameAtPitch;
	void* pfnFillRGBABlend;
	void* pfnGetAppID;
	void* pfnGetAliasList;
	void* pfnVguiWrap2_GetMouseDelta;
	void* pfnFilteredClientCmd;
} cl_enginefunc_t;

typedef struct playermove_s
{
	int				player_index;  // So we don't try to run the PM_CheckStuck nudging too quickly.
	qboolean		server;        // For debugging, are we running physics code on server side?

	qboolean		multiplayer;   // 1 == multiplayer server
	float			time;          // realtime on host, for reckoning duck timing
	float			frametime;	   // Duration of this frame

	vec3_t			forward, right, up; // Vectors for angles
	// player state
	vec3_t			origin;        // Movement origin.
	vec3_t			angles;        // Movement view angles.
	vec3_t			oldangles;     // Angles before movement view angles were looked at.
	vec3_t			velocity;      // Current movement direction.
	vec3_t			movedir;       // For waterjumping, a forced forward velocity so we can fly over lip of ledge.
	vec3_t			basevelocity;  // Velocity of the conveyor we are standing, e.g.

	// For ducking/dead
	vec3_t			view_ofs;      // Our eye position.
	float			flDuckTime;    // Time we started duck
	qboolean		bInDuck;       // In process of ducking or ducked already?

	// For walking/falling
	int				flTimeStepSound;  // Next time we can play a step sound
	int				iStepLeft;

	float			flFallVelocity;
	vec3_t			punchangle;

	float			flSwimTime;

	float			flNextPrimaryAttack;

	int				effects;		// MUZZLE FLASH, e.g.

	int				flags;         // FL_ONGROUND, FL_DUCKING, etc.
	int				usehull;       // 0 = regular player hull, 1 = ducked player hull, 2 = point hull
	float			gravity;       // Our current gravity and friction.
	float			friction;
	int				oldbuttons;    // Buttons last usercmd
	float			waterjumptime; // Amount of time left in jumping out of water cycle.
	qboolean		dead;          // Are we a dead player?
	int				deadflag;
	int				spectator;     // Should we use spectator physics model?
	int				movetype;      // Our movement type, NOCLIP, WALK, FLY

	int				onground;
	int				waterlevel;
	int				watertype;
	int				oldwaterlevel;

	char			sztexturename[256];
	char			chtexturetype;

	float			maxspeed;
	float			clientmaxspeed; // Player specific maxspeed

	// For mods
	int				iuser1;
	int				iuser2;
	int				iuser3;
	int				iuser4;
	float			fuser1;
	float			fuser2;
	float			fuser3;
	float			fuser4;
	vec3_t			vuser1;
	vec3_t			vuser2;
	vec3_t			vuser3;
	vec3_t			vuser4;
} playermove_t;


cl_enginefunc_t* GetEngineFuncs() {
	return (cl_enginefunc_t*)((uintptr_t)GetModuleHandleA("client.dll") + 0x1F8998);
}

playermove_t* GetPlayerMove() {
	return *(playermove_t**)((uintptr_t)GetModuleHandleA("client.dll") + 0x649088);
}