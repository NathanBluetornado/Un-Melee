#ifndef STAGE_GENERAL_H
#define STAGE_GENERAL_H
#include "../MexTK/mex.h"
// #include "../../SwordTrails/sword_trails.h"
// #include "../../../CustomHeaders/general.h"



/****************
 * FIGHTER HUDs *
 ****************/
//* Structs
typedef struct HUDVars
{
    GOBJ *fighter_gobj;
    int *ply;
} HUDVars;

typedef struct Symbol_WpInd_scene_models //defines structure for the symbols used, where there's only a JOBJSet inside
{
    JOBJSet *WpInd_scene_models;
} Symbol_WpInd_scene_models;

//* Functions

// void SetupFighterHUD();
// void FayWpIndLogic(GOBJ *gobj);
// void Sm2MgMtrLogic(GOBJ *gobj);
// void TjLcLogic(GOBJ *gobj);

void RCLogic(GOBJ *gobj);//, GOBJ *gobj_Tension);
void RelaxLogic(GOBJ *gobj);//, GOBJ *gobj_Tension);
void JoinLogic(GOBJ *gobj);
void PullLogic(GOBJ *gobj);
void SwapLogic(GOBJ *gobj);
void RivalsLogic(GOBJ *gobj);
void WorldLogic(GOBJ *gobj);
// void LoadWorld(GOBJ *gobj);
void GearLogic(GOBJ *gobj);
void GhostLogic(GOBJ *gobj);
void RiseLogic(GOBJ *gobj);
void RandomLogic(GOBJ *gobj);

void *DelayLogic(GOBJ *gobj);

typedef struct SM2Vars
{
    int unk[34];
    int grabFlag;
    FighterData *enemies[4];
    int megaMeterDisplayPoints;
    int lowerMegaLimit;
    int megaPoints;
    int doubleJumpsUsed;
    int beamLockoutFrames;
    char hitboxFlag[4];
    int megaState;
    float damageDealt;
    int transformFrames;
    int doubleJumpFlag;
    int doubleJumpRisingFrames;
    GOBJ *beam_gobj;
} SM2Vars;
/*****************
 * COMBO COUNTER *
 *****************/

//* Structs
typedef struct ComboVars
{
    int PlComboed[4];               //Combo counter(based on victim)
    float PlLastPercent[4];         //Last percentage storage
    int PlFramesSinceHitstunEnd[4]; //Framecounter for after hitstun ends (melee combos are mostly not true, necessary to account for situations like that)
    int PlComboedTrue[4];           //Indicator if combo is 0 - True , 1 - Breakable, 2 - String
    int PlComboedLastValue[4];      //Last combo counter value storage
    int PlAttacking[4];             //Attacking player ID storage
    int PlEmblem[4];
} ComboVars;



/****************
 * ROMAN CANCELS *
 ***************s*/
//* Structs
typedef struct TensionVars
{
    GOBJ *fighter_gobj;
    int *ply;
    int Tension;
    int framesleft;
    Vec3 stoppos[4];
    Vec3 kb[4];
} TensionVars;

//* Functions
int DamageReload(GOBJ *fighter_gobj);//, TensionVars *Tension_data);

//* Functions
void SetupTensionGOBJs();
void *TensionLogic();
void Airdash(GOBJ *fighter_gobj);
void Superjump(GOBJ *fighter_gobj);
void Gatling(GOBJ *fighter_gobj);
void JoinOnframe(GOBJ *fighter_gobj, GOBJ *tension_gobj);
void PullOnframe(GOBJ *fighter_gobj, GOBJ *tension_gobj);
void Rivals_Airdodge(GOBJ *fighter_gobj);
void superjumpASC(fighter_gobj);

/********************
 * LEDGE GRAB LIMIT *
 ********************/
//* Structs
typedef struct TimeVars
{
    int framescount[100];
} TimeVars;

typedef struct SwitchVars
{
    Vec3 pos;
} SwitchVars;

typedef struct RivalsVars
{
    GOBJ *fighter_gobj;
    int *ply;
    int Window;
} RivalsVars;

//* Functions
void SetupLGLGOBJs();
void *LGLLogic();

typedef struct DelayVars
{
    int FrameDelay;
    int prenumber[4];
} DelayVars;

#endif