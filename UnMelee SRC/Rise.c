#include "./MatchCodes.h";
#define MaxTension 2000


void LoadRise(GOBJ *fighter_gobj, ArchiveInfo *wpind_archive)
{
	FighterData *fighter_data = fighter_gobj->userdata;

	//Initialize Mega Meter
	// HSD_Archive *wpind_archive = Archive_LoadFile("PlSmHUD.dat");//("TensionHUD.dat");
    // ArchiveInfo *wpind_archive = File_Load("GrNLa.dat");
	Symbol_WpInd_scene_models *symbol_WpInd_scene_models = File_GetSymbol(wpind_archive, "Rise_scene_models");
	JOBJSet *jobjset_WpInd = symbol_WpInd_scene_models->WpInd_scene_models;
	// GOBJ *gobj_Tension = GObj_Create(3, 0, 0);
	GOBJ *gobj_WpInd = JOBJ_LoadSet(0, jobjset_WpInd, 0, 0, 0, 11, 1, RiseLogic);
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
void RiseLogic(GOBJ *gobj)//, GOBJ *gobj_Tension)
{
	
    JOBJ *jobj_Rise = gobj->hsd_object;
    JOBJ *jobj_CurrCharge; //Number group JOBJ
    JOBJ_GetChild(jobj_Rise, &jobj_CurrCharge, 0, -1); //Riset 0, meter and shit
    // TensionVars *RC_hud_data = gobj->userdata;

	// GOBJ *gobj_Tension = GObj_Create(3, 0, 0);
	// void *UserData = calloc(sizeof(TensionVars));          //Set up a pointer for the UserData which will be based on the Combo Variables struct
    // GObj_AddUserData(gobj_Tension, 4, HSD_Free, UserData);
	TensionVars *Tension_data = gobj->userdata;

	// Tension_data->Tension = Tension_data->Tension+ 1000;
	// OSReport("%d \n", Tension_data->Tension);

    RiseOnframe(Tension_data->fighter_gobj, gobj);
	FighterData *fighterData = Tension_data->fighter_gobj->userdata;

    if (Tension_data->fighter_gobj != 0 && (Pause_CheckStatus(1) != 2) && (Fighter_GetStocks(Tension_data->ply) > 0))
    {
        
        // SM2Vars *sm2_vars = &ft_data->fighter_var;
        jobj_Rise->trans = *Match_GetPlayerHUDPos(fighterData->ply);
        jobj_Rise->trans.X = jobj_Rise->trans.X -4.6; //-5.04;
        jobj_Rise->trans.Y = jobj_Rise->trans.Y + 0.95;
		// jobj_Rise->trans.X = jobj_Rise->trans.X -0;
        // jobj_Rise->trans.Y = jobj_Rise->trans.Y - 0.75;
		// jobj_Rise->scale.Y = -1;
		// jobj_Rise->rot.Z = -1.57;
       
	    if (!(JOBJ_GetAnimFrame(jobj_CurrCharge) == (int)(Tension_data->Tension /(MaxTension / 100))))
		{

			if (JOBJ_GetAnimFrame(jobj_CurrCharge) < (int)(Tension_data->Tension / (MaxTension / 100))) 
			{
				JOBJ_ReqAnimAll(jobj_CurrCharge, JOBJ_GetAnimFrame(jobj_CurrCharge) + 1);
			}
			if (JOBJ_GetAnimFrame(jobj_CurrCharge) > (int)(Tension_data->Tension / (MaxTension / 100)))
			{
				JOBJ_ReqAnimAll(jobj_CurrCharge, JOBJ_GetAnimFrame(jobj_CurrCharge) - 1);
			}
			//JOBJ_RunAObjCallback(jobj_CurrCharge, 6, 0x20, AOBJ_StopAnim, 6, 0, 0);
			JOBJ_AnimAll(jobj_CurrCharge);
		}
        JOBJ_ClearFlagsAll(jobj_Rise, JOBJ_HIDDEN);
        JOBJ_SetMtxDirtySub(jobj_Rise);
		
		if(fighterData->state_id < 14 || fighterData->state_id == ASID_ENTRY || Tension_data->Tension > 2000)
		{
			Tension_data->Tension = 2000;
		}

		if (fighterData->hitbox->active != 0 || fighterData->dmg.time_since_hit == 0)
		{
			Tension_data->Tension += 20;
		}

		if(Tension_data->Tension < 0)// || fighterData->state_id < 14)
		{
			Tension_data->Tension = 0;
		}
    }
    else
    {
        JOBJ_SetFlagsAll(jobj_Rise, JOBJ_HIDDEN);
    }

}



/////////////////////////////////
//      TENSION FUNCTIONS      //
/////////////////////////////////




void RiseOnframe(GOBJ *fighter_gobj, GOBJ *tension_gobj)
{
	// FighterData *fighterdata = fighter_gobj->userdata;
	FighterData *fighterData = fighter_gobj->userdata;
	FighterData *fp = fighter_gobj->userdata;
	TensionVars *Tension_data = tension_gobj->userdata;

	
	if (((fighterData->kind == JIGGLYPUFF) && (fighterData->state_id > 368 && fighterData->state_id < 373)) ||
	((fighterData->kind == YOSHI ) && ((fighterData->state_id == 364) || (fighterData->state_id == 365)) && fighterData->state.frame < 19) ||
	((fighterData->kind == DK) && (fighterData->state_id > 340 && fighterData->state_id < 369)) ||
	((fighterData->kind == KIRBY) && (fighterData->state_id > 352 && fighterData->state_id < 383)) ||
	((fighterData->kind == BOWSER) && (fighterData->state_id > 346 && fighterData->state_id < 359)) ||
	((fighterData->kind == LINK || fighterData->kind == YLINK) && (fighterData->state_id > 349 && fighterData->state_id < 356)) ||
	((fighterData->kind == WOLF) && (fighterData->state_id == 341 || fighterData->state_id == 344)) ||
	(fighterData->grab.attacker))
	{
		return;
	}	
	
	// OSReport("%d \n", fighterData->fighter_var.ft_var51);
	// OSReport("%f \n", fighterData->input_stickangle);
	// OSReport("%d \n", (fighterData->common_state_num + sizeof(FtState) + 3));

	
	Vec3 zero;
		zero.X = 0;
		zero.Y = 0;
		zero.Z = 0;
		
	
	if (Tension_data->Tension  == 2000){
		if (fighterData->input.held & (HSD_TRIGGER_R))
		{	
			if (fighterData->input.held & (HSD_TRIGGER_L)){

			// if (fighterData->state_id < 205 || fighterData->state_id > 235)
			// {
				if ((fighterData->state_id < ASID_GUARDOFF || fighterData->state_id > ASID_OTTOTTO) || (fighterData->state_id > ASID_DOWNSPOTD && fighterData->state_id < ASID_PASSIVEWALL) ||
				(fighterData->state_id == ASID_DOWNATTACKD) || (fighterData->state_id == ASID_ESCAPEAIR) || (fighterData->state_id >= ASID_THROWF && fighterData->state_id <= ASID_THROWLW))
				{	
				
					if (fighterData->state_id < ASID_DAMAGEHI1 || fighterData->state_id > ASID_DAMAGEFLYROLL)
					{
						if (fighterData->state_id > ASID_REBIRTH )
						{   
							if (fighterData->state_id == 266 || fighterData->state_id == ASID_PASS || fighterData->state_id == ASID_GUARDOFF || fighterData->state_id == ASID_FALL) {}
							else
							{
								for (int i = 0; i < 4; i++)
								{
									Vec3 pos;

									if (Fighter_GetGObj(i) != 0 && i != fighterData->ply)
									{
										FighterData *victimData = Fighter_GetGObj(i)->userdata;
										// Fighter_SetAirborne(victimData);
										if (&victimData->phys.kb_vel != &zero)
										{
											if (!(fighterData->jump.jumps_used == 0 && victimData->jump.jumps_used == 0))
											{
												ActionStateChange(0,1,0,fighter_gobj,ASID_FALL,0,0);
											}
											if (victimData->jump.jumps_used == 0){
												ActionStateChange(0,1,0,Fighter_GetGObj(i),ASID_DAMAGEFLYROLL,0,0);
											}
											
											if (victimData->phys.kb_vel.Y < 4)
											{
												victimData->phys.kb_vel.Y = 4;
											}
											else
											{
												victimData->phys.kb_vel.Y = victimData->phys.kb_vel.Y*2;
											}

											victimData->phys.kb_vel.X = 0;
											victimData->cb.IASA = null;

											Effect_SpawnAsyncLookup(Fighter_GetGObj(i), 198, 4, 0, 1, &zero, &zero);								
											Fighter_PlaySFX(fighter_gobj, 315, 127, 0);
											Tension_data->Tension -= 2000;
										}
									}
								}		
								// fighterData->phys.self_vel.X = 0;		
								// fighterData->phys.self_vel.Y = 0;
							}
						}
					}
				}
			}
		}
	}
}
