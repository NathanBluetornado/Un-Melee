#include "./MatchCodes.h";
#define MaxTension 2000




void LoadRelax(GOBJ *fighter_gobj, ArchiveInfo *wpind_archive)
{
	FighterData *fighter_data = fighter_gobj->userdata;

	//Initialize Mega Meter
	// HSD_Archive *wpind_archive = Archive_LoadFile("PlSmHUD.dat");//("TensionHUD.dat");
    // ArchiveInfo *wpind_archive = File_Load("GrNLa.dat");
	Symbol_WpInd_scene_models *symbol_WpInd_scene_models = File_GetSymbol(wpind_archive, "Relax_scene_models");
	JOBJSet *jobjset_WpInd = symbol_WpInd_scene_models->WpInd_scene_models;
	// GOBJ *gobj_Tension = GObj_Create(3, 0, 0);
	GOBJ *gobj_WpInd = JOBJ_LoadSet(0, jobjset_WpInd, 0, 0, 0, 11, 1, RelaxLogic);
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
void RelaxLogic(GOBJ *gobj)//, GOBJ *gobj_Tension)
{
	
    JOBJ *jobj_Relax = gobj->hsd_object;
    JOBJ *jobj_CurrCharge; //Number group JOBJ
    JOBJ_GetChild(jobj_Relax, &jobj_CurrCharge, 0, -1); //joint 0, meter and shit
    // TensionVars *RC_hud_data = gobj->userdata;

	// GOBJ *gobj_Tension = GObj_Create(3, 0, 0);
	// void *UserData = calloc(sizeof(TensionVars));          //Set up a pointer for the UserData which will be based on the Combo Variables struct
    // GObj_AddUserData(gobj_Tension, 4, HSD_Free, UserData);
	TensionVars *Tension_data = gobj->userdata;

	// Tension_data->Tension = Tension_data->Tension+ 1000;
	// OSReport("%d \n", Tension_data->Tension);

   RelaxOnfame(Tension_data->fighter_gobj, gobj);
	
    if (Tension_data->fighter_gobj != 0 && (Pause_CheckStatus(1) != 2) && (Fighter_GetStocks(Tension_data->ply) > 0))
    {
        FighterData *fighterData = Tension_data->fighter_gobj->userdata;
        // SM2Vars *sm2_vars = &ft_data->fighter_var;
        jobj_Relax->trans = *Match_GetPlayerHUDPos(fighterData->ply);
        jobj_Relax->trans.X = jobj_Relax->trans.X -4.6; //-5.04;
        jobj_Relax->trans.Y = jobj_Relax->trans.Y + 0.95;
		// jobj_Relax->trans.X = jobj_Relax->trans.X -0;
        // jobj_Relax->trans.Y = jobj_Relax->trans.Y - 0.75;
		// jobj_Relax->scale.Y = -1;
		// jobj_Relax->rot.Z = -1.57;
       
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
        JOBJ_ClearFlagsAll(jobj_Relax, JOBJ_HIDDEN);
        JOBJ_SetMtxDirtySub(jobj_Relax);
		
		// if (((fighterData->kind == LUIGI) && (fighterData->state_id < 343 || fighterData->state_id > 354)) ||
		// 	((fighterData->kind == PIKACHU) && (fighterData->state_id < 343 || fighterData->state_id > 352)) ||
		// 	((fighterData->kind == PICHU) && (fighterData->state_id < 343 || fighterData->state_id > 352)) ||
		// 	(!(fighterData->kind == LUIGI || fighterData->kind == PIKACHU || fighterData->kind == PICHU)))
		// {
		// if (fighterData->hitbox->victims->victim_gobj != 0 && fighterData->hitbox->active != 0)
		// {
		// 	FighterData *vic = fighterData->hitbox->victims->victim_gobj;
		// 	Tension_data->Tension += fighterData->hitbox->dmg + 10;
		// 	// Gatling(Tension_data->fighter_gobj);
		// }
		// }

		// if (/*fighterData->dmg.source->userdata != 0 && */ fighterData->dmg.time_since_hit == 0)
		// {
		// 	Tension_data->Tension += 10;
		// }

		// if(fighterData->state_id == ASID_DAMAGESONG || fighterData->state_id == ASID_DAMAGESONGWAIT || 
		// (fighterData->state_id == ASID_DAMAGESONGRV && fighterData->state.frame < 5) )
		// {
		// 	Tension_data->Tension += 5;
		// }

		if (Tension_data->Tension > MaxTension)
		{
			Tension_data->Tension = MaxTension;
		}

		if(fighterData->state_id < 14 || fighterData->state_id == ASID_ENTRY)
		{
			Tension_data->Tension = 2000;
		}
    }
    else
    {
        JOBJ_SetFlagsAll(jobj_Relax, JOBJ_HIDDEN);
    }
}


/////////////////////////////////
//      TIMESLOW FUNCTIONS     //
/////////////////////////////////


void RelaxOnfame(GOBJ *fighter_gobj, GOBJ *tension_gobj)
{
	// FighterData *fighterdata = fighter_gobj->userdata;
	FighterData *fighterData = fighter_gobj->userdata;
	FighterData *fp = fighter_gobj->userdata;
	TensionVars *Tension_data = tension_gobj->userdata;

	
	// OSReport("%d \n", fighterData->fighter_var.ft_var51);
	// OSReport("%f \n", fighterData->input_stickangle);
	// OSReport("%d \n", (fighterData->common_state_num + sizeof(FtState) + 3));

	// if(fighterData->state_id < ASID_RUNBRAKE & fighterData->state_id > ASID_WAIT)
	// {
	// 	Tension_data->Tension = Tension_data->Tension + 1;
	// 	// MeterLimit(Tension_data->Tension);
	// }

	// if(fighterData->state_id < ASID_RUNBRAKE & fighterData->state_id > ASID_WAIT)
	// if (fighterData->state_id == ASID_RUN)// || fighterData->state_id == ASID_DASH)
	// {
	// 	for (int i = 0; i < 4; i++)
	// 	{
	// 		Vec3 pos;

	// 		if (GetEnemyPosition(fighterData, i, &pos) != 0)
	// 		{
	// 			if ((fighterData->facing_direction == 1 && fighterData->phys.pos.X < pos.X) || (fighterData->facing_direction < 1 && fighterData->phys.pos.X > pos.X))
	// 			{
	// 				Tension_data->Tension = Tension_data->Tension + 5;
	// 			}
	// 		}
	// 	}
	// 	// MeterLimit(Tension_data->Tension);
	// }	
	
	Vec3 zero;
		zero.X = 0;
		zero.Y = 0;
		zero.Z = 0;
		
	
	
	if (fighterData->input.held & (HSD_TRIGGER_R))
	{	
		if (fighterData->input.held & (HSD_TRIGGER_L))
		{
			if (Tension_data->Tension > 100 && (fighterData->state_id != ASID_DAMAGESONGRV))
			{
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
								if (fighterData->state_id == ASID_ESCAPEAIR)
								{
									ActionStateChange(0,1,0,fighter_gobj,ASID_FALL,0,0);
									fighterData->phys.self_vel.X = 0;
									// fighterData->phys.self_vel.Y = 0;
								}
								Fighter_PlaySFX(fighter_gobj, 315, 127, 0);
								Tension_data->Tension -= 100;
								for (int i = 0; i < 4; i++)
								{
									Vec3 pos;
									if (Fighter_GetGObj(i) != 0 && i != fighterData->ply)
									{
										FighterData *victimData = Fighter_GetGObj(i)->userdata;
										Fighter_KillAllVelocity(Fighter_GetGObj(i));
										Effect_SpawnAsyncLookup(Fighter_GetGObj(i), 80, 4, 0, 1, &zero, &zero);
									}
								}
							}
						}
					}
				}
			}
			// else
			// {	
			// 	if (fighterData->jump.jumps_used == 0)
			// 	{
			// 		ActionStateChange(0, 0.5, 0, fighter_gobj, ASID_DAMAGESONGRV, 0, 0);
			// 		// Effect_SpawnAsyncLookup(fighter_gobj, 144, 4, 0, 1, &zero, &zero);
			// 	}
			// }
		}
	}
	
	// if (Tension_data->Tension  > 0)
	// {
	// 	HSD_SetSpeedEasy(1);
	// 	Fighter_ColorRemove(fighterData, 15);
	// 	if (fighterData->input.held & (HSD_TRIGGER_R))
	// 	{	
	// 		if (fighterData->input.held & (HSD_TRIGGER_L))
	// 		{
	// 		HSD_SetSpeedEasy(0.5);
	// 		Fighter_ApplyColAnim(fighterData, 15, 2);
	// 		Effect_SpawnAsyncLookup(fighter_gobj, 149, 4, 0, 1, &zero, &zero);
	// 		Tension_data->Tension = -Tension_data->Tension;
	// 		Fighter_PlaySFX(fighter_gobj, 315, 127, 0);
	// 		}
	// 	}
	// }
	// else// if (Tension_data->Tension <= 0)
	// {
	// 	Tension_data->Tension = Tension_data->Tension + 20;
	// 	HSD_SetSpeedEasy(0.5);
	// 	// if (Tension_data->Tension >= 0){
	// 	// 	HSD_SetSpeedEasy(1);
	// 	// 	Fighter_ColorRemove(fighterData, 15);
	// 	// 	Tension_data->Tension = 0;
	// 	// }
	// }	
}
