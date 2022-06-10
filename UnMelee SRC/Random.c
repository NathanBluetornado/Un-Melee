#include "./MatchCodes.h";
#define MaxTension 2000


void LoadRandom(GOBJ *fighter_gobj, ArchiveInfo *wpind_archive)
{
	FighterData *fighter_data = fighter_gobj->userdata;

	//Initialize Mega Meter
	// HSD_Archive *wpind_archive = Archive_LoadFile("PlSmHUD.dat");//("TensionHUD.dat");
    // ArchiveInfo *wpind_archive = File_Load("GrNLa.dat");
	Symbol_WpInd_scene_models *symbol_WpInd_scene_models = File_GetSymbol(wpind_archive, "Random_scene_models");
	JOBJSet *jobjset_WpInd = symbol_WpInd_scene_models->WpInd_scene_models;
	GOBJ *gobj_WpInd = JOBJ_LoadSet(0, jobjset_WpInd, 0, 0, 0, 11, 1, RandomLogic);
	void *UserData = calloc(sizeof(TensionVars));
	GObj_AddUserData(gobj_WpInd, 4, HSD_Free, UserData);
	TensionVars *Tension_data = gobj_WpInd->userdata;
	Tension_data->fighter_gobj = fighter_gobj;
	Tension_data->ply = fighter_data->player_controller_number;
	// break;
}

/////////////////////////////////
//        TENSION GUAGE        //
/////////////////////////////////
void RandomLogic(GOBJ *gobj)//, GOBJ *gobj_Tension)
{
	
    JOBJ *jobj_Random = gobj->hsd_object;
    // JOBJ *jobj_CurrCharge; //Number group JOBJ
    // JOBJ_GetChild(jobj_Random, &jobj_CurrCharge, 0, -1);

	TensionVars *Tension_data = gobj->userdata;
    if (Tension_data->fighter_gobj != 0 && (Pause_CheckStatus(1) != 2) && (Fighter_GetStocks(Tension_data->ply) > 0))
    {
        
        // SM2Vars *sm2_vars = &ft_data->fighter_var;
        jobj_Random->trans = *Match_GetPlayerHUDPos(Tension_data->ply);
		int player = Tension_data->ply;
        // jobj_Random->trans.X = jobj_Random->trans.X -4.6; //-5.04;
        // jobj_Random->trans.Y = jobj_Random->trans.Y + 0.95;
		// jobj_Random->trans.X = jobj_Random->trans.X -0;
        // jobj_Random->trans.Y = jobj_Random->trans.Y - 0.75;
		// jobj_Random->scale.Y = -1;
		// jobj_Random->rot.Z = -1.57;
		// switch (player)
		// {
		// case 0:
		// 	jobj_Random->dobj->mobj->mat->diffuse.r = 255U;
		// 	jobj_Random->dobj->mobj->mat->diffuse.b = 0U;
		// 	jobj_Random->dobj->mobj->mat->diffuse.g = 0U;
		// 	break;
		// case 1:
		// 	jobj_Random->dobj->mobj->mat->diffuse.r = 0U;
		// 	jobj_Random->dobj->mobj->mat->diffuse.b = 255U;
		// 	jobj_Random->dobj->mobj->mat->diffuse.g = 0U;
		// 	break;
		// case 2:
		// 	jobj_Random->dobj->mobj->mat->diffuse.r = 255U;
		// 	jobj_Random->dobj->mobj->mat->diffuse.b = 0U;
		// 	jobj_Random->dobj->mobj->mat->diffuse.g = 255U;
		// 	break;
		// case 3:
		// 	jobj_Random->dobj->mobj->mat->diffuse.r = 0U;
		// 	jobj_Random->dobj->mobj->mat->diffuse.b = 0U;
		// 	jobj_Random->dobj->mobj->mat->diffuse.g = 255U;
		// 	break;
		// }

        JOBJ_ClearFlagsAll(jobj_Random, JOBJ_HIDDEN);
        JOBJ_SetMtxDirtySub(jobj_Random);
    }
    else
    {
        JOBJ_SetFlagsAll(jobj_Random, JOBJ_HIDDEN);
    }

}