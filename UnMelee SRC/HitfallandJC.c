#include "./MatchCodes.h";
#define LCancelWindow 7


void LoadRivals(GOBJ *fighter_gobj)
{
	FighterData *fighter_data = fighter_gobj->userdata;
    ArchiveInfo *wpind_archive = File_Load("GrNLa.dat");
	Symbol_WpInd_scene_models *symbol_WpInd_scene_models = File_GetSymbol(wpind_archive, "Rivals_scene_models");
	JOBJSet *jobjset_WpInd = symbol_WpInd_scene_models->WpInd_scene_models;
	GOBJ *gobj_WpInd = JOBJ_LoadSet(0, jobjset_WpInd, 0, 0, 0, 11, 1, RivalsLogic);
	void *UserData = calloc(sizeof(RivalsVars));
	GObj_AddUserData(gobj_WpInd, 4, HSD_Free, UserData);
	RivalsVars *Rivals_data = gobj_WpInd->userdata;
	Rivals_data->fighter_gobj = fighter_gobj;
	Rivals_data->ply = fighter_data->player_controller_number;
	// break;
}


void LCancel_Think(GOBJ *fighter_gobj)
{
	FighterData *fighterData = fighter_gobj->userdata;

	// check if they just landed an aerial attack (this does not check special cases like game and watch)
	if ((fighterData->state_id >= ASID_LANDINGAIRN && fighterData->state_id <= ASID_LANDINGAIRLW) &&
		fighterData->state.frame == 0.0)
	{
		// fighter just landed an aerial attack, adjust their anim rate to 2
		// Fighter_SetAnimRate(fighter_gobj, 2.0);
		
	}
}




/////////////////////////////////
//        TENSION GUAGE        //
/////////////////////////////////
void RivalsLogic(GOBJ *gobj)//, GOBJ *gobj_Tension)
{
	
    JOBJ *jobj_Rivals = gobj->hsd_object;
    JOBJ *jobj_CurrCharge; //Number group JOBJ
    JOBJ_GetChild(jobj_Rivals, &jobj_CurrCharge, 0, -1);
    // RivalsVars *RC_hud_data = gobj->userdata;

	RivalsVars *Rivals_data = gobj->userdata;

	// OSReport("%d \n", Rivals_data->Rivals);

	FighterData *fighterData = Rivals_data->fighter_gobj->userdata;

    if (Rivals_data->fighter_gobj != 0 && (Pause_CheckStatus(1) != 2) && (Fighter_GetStocks(Rivals_data->ply) > 0))
    {
        
        // SM2Vars *sm2_vars = &ft_data->fighter_var;
        jobj_Rivals->trans = *Match_GetPlayerHUDPos(fighterData->ply);
        jobj_Rivals->trans.X = jobj_Rivals->trans.X -4.6; //-5.04;
        jobj_Rivals->trans.Y = jobj_Rivals->trans.Y + 0.95;
		// jobj_Rivals->trans.X = jobj_Rivals->trans.X -0;
        // jobj_Rivals->trans.Y = jobj_Rivals->trans.Y - 0.75;
		// jobj_Rivals->scale.Y = -1;
		// jobj_Rivals->rot.Z = -1.57;
       
        JOBJ_ClearFlagsAll(jobj_Rivals, JOBJ_HIDDEN);
        JOBJ_SetMtxDirtySub(jobj_Rivals);
    }
    else
    {
        JOBJ_SetFlagsAll(jobj_Rivals, JOBJ_HIDDEN);
    }

	if (fighterData->hitbox->victims->victim_gobj != 0 && fighterData->hitbox->active != 0)
	{
		FighterData *vic = fighterData->hitbox->victims->victim_gobj;
		Check(Rivals_data->fighter_gobj, gobj);
	}

	if (Rivals_data->Window > 0)
	{
		LCancel_Think(Rivals_data->fighter_gobj);
		Rivals_data->Window -= 1;
	}

	if (fighterData->state_id == ASID_ESCAPEAIR)
	{
		Rivals_Airdodge(Rivals_data->fighter_gobj);
	}

}



/////////////////////////////////
//      TENSION FUNCTIONS      //
/////////////////////////////////


void Rivals_Airdodge_anim(GOBJ *fighter_gobj)
{
	if (FrameTimerCheck == 0)
	{
		Fighter_EnterFall(fighter_gobj);
	}
}

void Rivals_Airdodge(GOBJ *fighter_gobj)
{
	FighterData *fighterData = fighter_gobj->userdata;
	fighterData->cb.Anim = Rivals_Airdodge_anim;
}

void Check(GOBJ *fighter_gobj, GOBJ *Rivals_gobj)
{
	// FighterData *fighterdata = fighter_gobj->userdata;
	FighterData *fighterData = fighter_gobj->userdata;
	FighterData *fp = fighter_gobj->userdata;
	RivalsVars *Rivals_data = Rivals_gobj->userdata;

	
	// OSReport("%d \n", fighterData->fighter_var.ft_var51);
	// OSReport("%f \n", fighterData->input_stickangle);
	// OSReport("%d \n", (fighterData->common_state_num + sizeof(FtState) + 3));

	
	Vec3 zero;
		zero.X = 0;
		zero.Y = 0;
		zero.Z = 0;
		
	
	if (fighterData->input.lstick.Y < -0.1 && Rivals_data->Window == 0 && (fighterData->state_id >= ASID_ATTACKAIRN && fighterData->state_id <= ASID_ATTACKAIRLW))
	{	
		// Hitfall
		Rivals_data->Window = LCancelWindow;
		fighterData->phys.self_vel.Y -= 3;
		Effect_SpawnAsyncLookup(fighter_gobj, 160, 4, 0, 1, &zero, &zero);
	}
	
}
