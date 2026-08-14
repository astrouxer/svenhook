#pragma once

// so much slop shamelessly pasted from hl1 sdk on github
// a lot of sdk was lowkey just commented or replaced with DWORD im too lazy lololol

#define STUDIO_RENDER 1
#define STUDIO_EVENTS 2

#define MAX_CLIENTS			32
#define	MAX_EDICTS			900

#define MAX_MODEL_NAME		64
#define MAX_MAP_HULLS		4
#define	MIPLEVELS			4
#define	NUM_AMBIENTS		4		// automatic ambient sounds
#define	MAXLIGHTMAPS		4
#define	PLANE_ANYZ			5

#define ALIAS_Z_CLIP_PLANE	5

// flags in finalvert_t.flags
#define ALIAS_LEFT_CLIP				0x0001
#define ALIAS_TOP_CLIP				0x0002
#define ALIAS_RIGHT_CLIP			0x0004
#define ALIAS_BOTTOM_CLIP			0x0008
#define ALIAS_Z_CLIP				0x0010
#define ALIAS_ONSEAM				0x0020
#define ALIAS_XY_CLIP_MASK			0x000F

#define	ZISCALE	((float)0x8000)

#define CACHE_SIZE	32		// used to align key data structures

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

typedef struct SCREENINFO_s
{
	int		iSize;
	int		iWidth;
	int		iHeight;
	int		iFlags;
	int		iCharHeight;
	short	charWidths[256];
} SCREENINFO;

typedef struct
{
	float		mins[3], maxs[3];
	float		origin[3];
	int			headnode[MAX_MAP_HULLS];
	int			visleafs;		// not including the solid leaf 0
	int			firstface, numfaces;
} dmodel_t;

// plane_t structure
typedef struct mplane_s
{
	vec3_t	normal;			// surface normal
	float	dist;			// closest appoach to origin
	byte	type;			// for texture axis selection and fast side tests
	byte	signbits;		// signx + signy<<1 + signz<<1
	byte	pad[2];
} mplane_t;

typedef struct
{
	vec3_t		position;
} mvertex_t;

typedef struct
{
	unsigned short	v[2];
	unsigned int	cachededgeoffset;
} medge_t;

typedef struct texture_s
{
	char		name[16];
	unsigned	width, height;
	int			anim_total;				// total tenths in sequence ( 0 = no)
	int			anim_min, anim_max;		// time for this frame min <=time< max
	struct texture_s* anim_next;		// in the animation sequence
	struct texture_s* alternate_anims;	// bmodels in frame 1 use these
	unsigned	offsets[MIPLEVELS];		// four mip maps stored
	unsigned	paloffset;
} texture_t;

typedef struct
{
	float		vecs[2][4];		// [s/t] unit vectors in world space. 
	// [i][3] is the s/t offset relative to the origin.
	// s or t = dot(3Dpoint,vecs[i])+vecs[i][3]
	float		mipadjust;		// ?? mipmap limits for very small surfaces
	texture_t* texture;
	int			flags;			// sky or slime, no lightmap or 256 subdivision
} mtexinfo_t;

typedef struct mnode_s
{
	// common with leaf
	int			contents;		// 0, to differentiate from leafs
	int			visframe;		// node needs to be traversed if current

	short		minmaxs[6];		// for bounding box culling

	struct mnode_s* parent;

	// node specific
	mplane_t* plane;
	struct mnode_s* children[2];

	unsigned short		firstsurface;
	unsigned short		numsurfaces;
} mnode_t;

typedef struct msurface_s	msurface_t;
typedef struct decal_s		decal_t;

// JAY: Compress this as much as possible
struct decal_s
{
	decal_t* pnext;			// linked list for each surface
	msurface_t* psurface;		// Surface id for persistence / unlinking
	short		dx;				// Offsets into surface texture (in texture coordinates, so we don't need floats)
	short		dy;
	short		texture;		// Decal texture
	byte		scale;			// Pixel scale
	byte		flags;			// Decal flags

	short		entityIndex;	// Entity this is attached to
};

typedef struct mleaf_s
{
	// common with node
	int			contents;		// wil be a negative contents number
	int			visframe;		// node needs to be traversed if current

	short		minmaxs[6];		// for bounding box culling

	struct mnode_s* parent;

	// leaf specific
	byte* compressed_vis;
	struct efrag_s* efrags;

	msurface_t** firstmarksurface;
	int			nummarksurfaces;
	int			key;			// BSP sequence number for leaf's contents
	byte		ambient_sound_level[NUM_AMBIENTS];
} mleaf_t;

struct msurface_s
{
	int			visframe;		// should be drawn when node is crossed

	int			dlightframe;	// last frame the surface was checked by an animated light
	int			dlightbits;		// dynamically generated. Indicates if the surface illumination 
	// is modified by an animated light.

	mplane_t* plane;			// pointer to shared plane			
	int			flags;			// see SURF_ #defines

	int			firstedge;	// look up in model->surfedges[], negative numbers
	int			numedges;	// are backwards edges

	// surface generation data
	struct surfcache_s* cachespots[MIPLEVELS];

	short		texturemins[2]; // smallest s/t position on the surface.
	short		extents[2];		// ?? s/t texture size, 1..256 for all non-sky surfaces

	mtexinfo_t* texinfo;

	// lighting info
	byte		styles[MAXLIGHTMAPS]; // index into d_lightstylevalue[] for animated lights 
	// no one surface can be effected by more than 4 
	// animated lights.
	DWORD* samples;

	decal_t* pdecals;
};

typedef struct
{
	int			planenum;
	short		children[2];	// negative numbers are contents
} dclipnode_t;

typedef struct hull_s
{
	dclipnode_t* clipnodes;
	mplane_t* planes;
	int			firstclipnode;
	int			lastclipnode;
	vec3_t		clip_mins;
	vec3_t		clip_maxs;
} hull_t;

typedef struct cache_user_s
{
	void* data;
} cache_user_t;

typedef struct model_s
{
	char		name[MAX_MODEL_NAME];
	qboolean	needload;		// bmodels and sprites don't cache normally

	DWORD*	type;
	int			numframes;
	DWORD*	synctype;

	int			flags;

	//
	// volume occupied by the model
	//		
	vec3_t		mins, maxs;
	float		radius;

	//
	// brush model
	//
	int			firstmodelsurface, nummodelsurfaces;

	int			numsubmodels;
	dmodel_t* submodels;

	int			numplanes;
	mplane_t* planes;

	int			numleafs;		// number of visible leafs, not counting 0
	struct mleaf_s* leafs;

	int			numvertexes;
	mvertex_t* vertexes;

	int			numedges;
	medge_t* edges;

	int			numnodes;
	mnode_t* nodes;

	int			numtexinfo;
	mtexinfo_t* texinfo;

	int			numsurfaces;
	msurface_t* surfaces;

	int			numsurfedges;
	int* surfedges;

	int			numclipnodes;
	dclipnode_t* clipnodes;

	int			nummarksurfaces;
	msurface_t** marksurfaces;

	hull_t		hulls[MAX_MAP_HULLS];

	int			numtextures;
	texture_t** textures;

	byte* visdata;

	DWORD* lightdata;

	char* entities;

	//
	// additional model data
	//
	cache_user_t	cache;		// only access through Mod_Extradata

} model_t;

typedef struct r_studio_interface_s
{
	int				version;
	int				(*StudioDrawModel)			(int flags);
	int				(*StudioDrawPlayer)			(int flags, struct entity_state_s* pplayer);
} r_studio_interface_t;

typedef struct engine_studio_api_s
{
	// Allocate number*size bytes and zero it
	void* (*Mem_Calloc)				(int number, size_t size);
	// Check to see if pointer is in the cache
	void* (*Cache_Check)				(struct cache_user_s* c);
	// Load file into cache ( can be swapped out on demand )
	void			(*LoadCacheFile)				(char* path, struct cache_user_s* cu);
	// Retrieve model pointer for the named model
	struct model_s* (*Mod_ForName)				(const char* name, int crash_if_missing);
	// Retrieve pointer to studio model data block from a model
	void* (*Mod_Extradata)				(struct model_s* mod);
	// Retrieve indexed model from client side model precache list
	struct model_s* (*GetModelByIndex)			(int index);
	// Get entity that is set for rendering
	struct cl_entity_s* (*GetCurrentEntity)		(void);
	// Get referenced player_info_t
	struct player_info_s* (*PlayerInfo)			(int index);
	// Get most recently received player state data from network system
	struct entity_state_s* (*GetPlayerState)		(int index);
	// Get viewentity
	struct cl_entity_s* (*GetViewEntity)			(void);
	// Get current frame count, and last two timestampes on client
	void			(*GetTimes)					(int* framecount, double* current, double* old);
	// Get a pointer to a cvar by name
	struct cvar_s* (*GetCvar)					(const char* name);
	// Get current render origin and view vectors ( up, right and vpn )
	void			(*GetViewInfo)				(float* origin, float* upv, float* rightv, float* vpnv);
	// Get sprite model used for applying chrome effect
	struct model_s* (*GetChromeSprite)			(void);
	// Get model counters so we can incement instrumentation
	void			(*GetModelCounters)			(int** s, int** a);
	// Get software scaling coefficients
	void			(*GetAliasScale)				(float* x, float* y);

	// Get bone, light, alias, and rotation matrices
	float**** (*StudioGetBoneTransform) (void);
	float**** (*StudioGetLightTransform)(void);
	float*** (*StudioGetAliasTransform) (void);
	float*** (*StudioGetRotationMatrix) (void);

	// Set up body part, and get submodel pointers
	void			(*StudioSetupModel)			(int bodypart, void** ppbodypart, void** ppsubmodel);
	// Check if entity's bbox is in the view frustum
	int				(*StudioCheckBBox)			(void);
	// Apply lighting effects to model
	void			(*StudioDynamicLight)			(struct cl_entity_s* ent, struct alight_s* plight);
	void			(*StudioEntityLight)			(struct alight_s* plight);
	void			(*StudioSetupLighting)		(struct alight_s* plighting);

	// Draw mesh vertices
	void			(*StudioDrawPoints)			(void);

	// Draw hulls around bones
	void			(*StudioDrawHulls)			(void);
	// Draw bbox around studio models
	void			(*StudioDrawAbsBBox)			(void);
	// Draws bones
	void			(*StudioDrawBones)			(void);
	// Loads in appropriate texture for model
	void			(*StudioSetupSkin)			(void* ptexturehdr, int index);
	// Sets up for remapped colors
	void			(*StudioSetRemapColors)		(int top, int bottom);
	// Set's player model and returns model pointer
	struct model_s* (*SetupPlayerModel)			(int index);
	// Fires any events embedded in animation
	void			(*StudioClientEvents)			(void);
	// Retrieve/set forced render effects flags
	int				(*GetForceFaceFlags)			(void);
	void			(*SetForceFaceFlags)			(int flags);
	// Tell engine the value of the studio model header
	void			(*StudioSetHeader)			(void* header);
	// Tell engine which model_t * is being renderered
	void			(*SetRenderModel)				(struct model_s* model);

	// Final state setup and restore for rendering
	void			(*SetupRenderer)				(int rendermode);
	void			(*RestoreRenderer)			(void);

	// Set render origin for applying chrome effect
	void			(*SetChromeOrigin)			(void);

	// True if using D3D/OpenGL
	int				(*IsHardware)					(void);

	// Only called by hardware interface
	void			(*GL_StudioDrawShadow)		(void);
	void			(*GL_SetRenderMode)			(int mode);

	void			(*StudioSetRenderamt)			(int iRenderamt); 	//!!!CZERO added for rendering glass on viewmodels
	void			(*StudioSetCullState)			(int iCull);
	void			(*StudioRenderShadow)			(int iSprite, float* p1, float* p2, float* p3, float* p4);
} engine_studio_api_t;

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
	int (__cdecl* pfnGetScreenInfo)(struct SCREENINFO_s* pscrinfo);
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
	int (__cdecl* pfnDrawConsoleString)(int x, int y, char* string);
	void* (__cdecl* pfnDrawSetTextColor)(float r, float g, float b);
	void* pfnDrawConsoleStringLen;
	void* pfnConsolePrint;
	void* pfnCenterPrint;
	void* GetWindowCenterX;
	void* GetWindowCenterY;
	void* GetViewAngles;
	void* SetViewAngles;
	int (__cdecl* GetMaxClients)(void);
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
	cl_entity_s* (__cdecl* GetViewModel)(void);
	cl_entity_s* (__cdecl* GetEntityByIndex)(int* index);
	void* GetClientTime;
	void* V_CalcShake;
	void* V_ApplyShake;
	void* PM_PointContents;
	void* PM_WaterEntity;
	void* PM_TraceLine;
	struct model_s* (__cdecl* CL_LoadModel)(const char* modelname, int* index);
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

inline cl_enginefunc_t* GetEngineFuncs() 
{
	return (cl_enginefunc_t*)((uintptr_t)GetModuleHandleA("client.dll") + 0x1F8998); // not sure if u can export these /shrug im newgen
}

inline playermove_t* GetPlayerMove() 
{
	return *(playermove_t**)((uintptr_t)GetModuleHandleA("client.dll") + 0x649088);
}

inline engine_studio_api_t* GetEngineStudio()
{
	return (engine_studio_api_t*)((uintptr_t)GetModuleHandleA("client.dll") + 0x63C870);
}

inline r_studio_interface_t* GetStudioInterface()
{
	return (r_studio_interface_t*)((uintptr_t)GetModuleHandleA("client.dll") + 0x1A25B4);
}