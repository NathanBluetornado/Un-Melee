#ifndef MEX_H_BM
#define MEX_H_BM
#include "mex.h"

void Fighter_SetStocks(int ply, int stocks); // Added by Cappy for Blazeball
float exp(float x);                          // Added by fennel for Luigi + Boo
int IASA_Decide_Aerials(FighterData *fighter_data);

typedef struct TEVcolor
{
    char r;
    char g;
    char b;
    char a;
} TEVcolor;

typedef struct TEV
{
    char color_op;
    char alpha_op;
    char color_bias;
    char alpha_bias;
    char color_scale;
    char alpha_scale;
    char color_clamp;
    char alpha_clamp;
    char color_a_in;
    char color_b_in;
    char color_c_in;
    char color_d_in;
    char alpha_a_in;
    char alpha_b_in;
    char alpha_c_in;
    char alpha_d_in;
    TEVcolor constant;
    TEVcolor tev0;
	TEVcolor tev1;
	int active_flags;
} TEV;

DOBJ *JOBJ_DOBJLookup(JOBJ *jobj, int dobj_index)
{
    DOBJ *return_dobj = JOBJ_GetDObj(jobj);
    for(int i = 0; i < dobj_index; i++)
    {
        return_dobj = return_dobj->next;
    }
    return return_dobj;
}

float GetDistance(float x1, float y1, float x2, float y2)
{
    return sqrtf(pow(fabs(x2 - x1), 2) + pow(fabs(y2 - y1), 2));
}

int checkReverseInput(FighterData *fighter_data)
{
    if (fighter_data->facing_direction > 0 && fighter_data->input.lstick.X < -0.275)
    {
        return 1;
    } else if (fighter_data->facing_direction < 0 && fighter_data->input.lstick.X > 0.275)
    {
        return 1;
    } else {
        return 0;
    }
}

int checkReverseInput_cstick(FighterData *fighter_data)
{
    if (fighter_data->facing_direction > 0 && fighter_data->input.cstick.X < -0.275)
    {
        return 1;
    } else if (fighter_data->facing_direction < 0 && fighter_data->input.cstick.X > 0.275)
    {
        return 1;
    } else {
        return 0;
    }
}

int Fighter_CollAir_GrabFacingLedgeWalljump(GOBJ *fighter, void *perFrame, void *onLand); // this will handle entering cliffcatch / walljump. all in one collision func
int Fighter_CollAir_GrabBothLedgesWalljump(GOBJ *fighter, void *onLand); // this will handle entering cliffcatch / walljump. all in one collision func

#endif