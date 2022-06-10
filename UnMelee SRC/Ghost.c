#include "./MatchCodes.h";
#define MaxTension 2000
#define GhostFrames 150


void LoadGhost(GOBJ *fighter_gobj, ArchiveInfo *wpind_archive)
{
	FighterData *fighter_data = fighter_gobj->userdata;

	//Initialize Mega Meter
	// HSD_Archive *wpind_archive = Archive_LoadFile("PlSmHUD.dat");//("TensionHUD.dat");
    // ArchiveInfo *wpind_archive = File_Load("GrNLa.dat");
	Symbol_WpInd_scene_models *symbol_WpInd_scene_models = File_GetSymbol(wpind_archive, "Ghost_scene_models");
	JOBJSet *jobjset_WpInd = symbol_WpInd_scene_models->WpInd_scene_models;
	// GOBJ *gobj_Tension = GObj_Create(3, 0, 0);
	GOBJ *gobj_WpInd = JOBJ_LoadSet(0, jobjset_WpInd, 0, 0, 0, 11, 1, GhostLogic);
	void *UserData = calloc(sizeof(TensionVars));
	GObj_AddUserData(gobj_WpInd, 4, HSD_Free, UserData);
	TensionVars *Tension_data = gobj_WpInd->userdata;
	Tension_data->fighter_gobj = fighter_gobj;
	Tension_data->ply = fighter_data->player_controller_number;
	// break;
}


void ghost_phys(GOBJ *fighter_gobj)
{
	FighterData *fp = fighter_gobj->userdata;

	if ((fabs(fp->input.lstick.X) > 0.1) || (fabs(fp->input.lstick.Y) > 0.1 ))
	{
		fp->phys.self_vel.X = fp->input.lstick.X * 2.5;
		fp->phys.self_vel.Y = fp->input.lstick.Y * 2.5;
	}
}

void ghost_ontakedmg(GOBJ *fighter_gobj)
{
	FighterData *fp = fighter_gobj->userdata;

	Fighter_GiveDamage(fp, 15);
}

void ghost_IASA(GOBJ *fighter_gobj)
{
	FighterData *fp = fighter_gobj->userdata;
	FighterData *fighterData = fighter_gobj->userdata;

	// Fighter_EnterFall(fighter_gobj);

	if ((fp->input.down & (HSD_BUTTON_A)) || (fabs(fp->input.cstick.X) > 0.1 || fabs(fp->input.cstick.Y) > 0.1))
	{
		Fighter_Interrupt_AllAerial(fighter_gobj);
		fp->cb.Phys = ghost_phys;
		fp->cb.IASA = ghost_IASA;
		fp->cb.Coll = null;
		fp->cb.OnTakeDamage = ghost_ontakedmg;
	}

	if (!((fp->state_id >= ASID_ATTACKAIRN && fp->state_id <= ASID_ATTACKAIRLW) || (fp->state_id == ASID_FALL)))
	{
		ActionStateChange(0, 1, 0, fighter_gobj, ASID_FALL, 0, 0);
	}
}


/////////////////////////////////
//        TENSION GUAGE        //
/////////////////////////////////
void GhostLogic(GOBJ *gobj)//, GOBJ *gobj_Tension)
{
	
    JOBJ *jobj_Ghost = gobj->hsd_object;
    JOBJ *jobj_CurrCharge; //Number group JOBJ
    JOBJ_GetChild(jobj_Ghost, &jobj_CurrCharge, 0, -1); //Ghostt 0, meter and shit
    // TensionVars *RC_hud_data = gobj->userdata;

	// GOBJ *gobj_Tension = GObj_Create(3, 0, 0);
	// void *UserData = calloc(sizeof(TensionVars));          //Set up a pointer for the UserData which will be based on the Combo Variables struct
    // GObj_AddUserData(gobj_Tension, 4, HSD_Free, UserData);
	TensionVars *Tension_data = gobj->userdata;

	// Tension_data->Tension = Tension_data->Tension+ 1000;
	// OSReport("%d \n", Tension_data->Tension);
	Vec3 zero;
		zero.X = 0;
		zero.Y = 0;
		zero.Z = 0;

    GhostOnframe(Tension_data->fighter_gobj, gobj);
	FighterData *fighterData = Tension_data->fighter_gobj->userdata;

    if (Tension_data->fighter_gobj != 0 && (Pause_CheckStatus(1) != 2) && (Fighter_GetStocks(Tension_data->ply) > 0))
    {
        
        // SM2Vars *sm2_vars = &ft_data->fighter_var;
        jobj_Ghost->trans = *Match_GetPlayerHUDPos(fighterData->ply);
        jobj_Ghost->trans.X = jobj_Ghost->trans.X -4.6; //-5.04;
        jobj_Ghost->trans.Y = jobj_Ghost->trans.Y + 0.95;
		// jobj_Ghost->trans.X = jobj_Ghost->trans.X -0;
        // jobj_Ghost->trans.Y = jobj_Ghost->trans.Y - 0.75;
		// jobj_Ghost->scale.Y = -1;
		// jobj_Ghost->rot.Z = -1.57;
       
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
        JOBJ_ClearFlagsAll(jobj_Ghost, JOBJ_HIDDEN);
        JOBJ_SetMtxDirtySub(jobj_Ghost);
		
		if(fighterData->state_id < 14 || fighterData->state_id == ASID_ENTRY)
		{
			Tension_data->Tension = 1000;
		}

		if (fighterData->hitbox->victims->victim_gobj != 0 && fighterData->hitbox->active != 0)
		{
			FighterData *vic = fighterData->hitbox->victims->victim_gobj;
			Tension_data->Tension += fighterData->hitbox->dmg;// + 10;
			// Gatling(Tension_data->fighter_gobj);
		}

		if (/*fighterData->dmg.source->userdata != 0 && */ fighterData->dmg.time_since_hit == 0)
		{
			Tension_data->Tension += 5;
		}

		if(Tension_data->Tension < 0)// || fighterData->state_id < 14)
		{
			Tension_data->Tension = 0;
		}

		if (Tension_data->Tension > MaxTension)
		{
			Tension_data->Tension = MaxTension;
		}

		
    }
    else
    {
        JOBJ_SetFlagsAll(jobj_Ghost, JOBJ_HIDDEN);
    }

}



/////////////////////////////////
//      TENSION FUNCTIONS      //
/////////////////////////////////


void GhostOnframe(GOBJ *fighter_gobj, GOBJ *tension_gobj)
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

	if(fighterData->state_id < ASID_RUNBRAKE & fighterData->state_id > ASID_WAIT)
	{
		Tension_data->Tension = Tension_data->Tension + 2;
	}

	if (fighterData->state_id == ASID_RUN)
	{
		for (int i = 0; i < 4; i++)
		{
			Vec3 pos;

			if (GetEnemyPosition(fighterData, i, &pos) != 0)
			{
				if ((fighterData->facing_direction == 1 && fighterData->phys.pos.X < pos.X) || (fighterData->facing_direction < 1 && fighterData->phys.pos.X > pos.X))
				{
					Tension_data->Tension = Tension_data->Tension + 4;
				}
			}
		}
	}
	
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
								// fighterData->phys.self_vel.X = 0;		
								// fighterData->phys.self_vel.Y = 0;								
								Fighter_PlaySFX(fighter_gobj, 315, 127, 0);
								Tension_data->Tension -= 2000;
								Tension_data->framesleft = GhostFrames;
								Tension_data->stoppos[1] = fighterData->phys.pos;
								Fighter_EnterFall(fighter_gobj);
								Effect_SpawnAsyncLookup(fighter_gobj, 438, 4, 0, 1, &zero, &zero);
								// Fighter_ApplyIntang(fighter_gobj, Tension_data->framesleft);
								fp->cb.Phys = ghost_phys;
								fp->cb.IASA = ghost_IASA;
								fp->cb.Coll = null;
								fp->cb.OnTakeDamage = ghost_ontakedmg;
								fp->jump.jumps_used = 2;
							}
						}
					}
				}
			}
		}
	}

	if(Tension_data->framesleft > 0)// || fighterData->state_id < 14)
	{
		Tension_data->framesleft -= 1;
		
		fighterData->cb.Phys = ghost_phys;
		fighterData->cb.IASA = ghost_IASA;
		fighterData->cb.Coll = null;
		fighterData->cb.OnTakeDamage = ghost_ontakedmg;
		
		Fighter_ApplyColAnim(fighterData, 52, 2);
		
		if (fighterData->input.held & (HSD_TRIGGER_R) && Tension_data->framesleft < (GhostFrames - 15))
		{	
			if (fighterData->input.held & (HSD_TRIGGER_L))
			{
				Fighter_ColorRemove(fighterData, 52);
				Effect_SpawnAsyncLookup(Tension_data->fighter_gobj, 142, 4, 0, 1, &zero, &zero);
				Fighter_EnableCollUpdate(fighterData);
				Fighter_EnterSpecialFall(Tension_data->fighter_gobj, 0, 1, 0, 0, 30);
				Tension_data->framesleft = 0;
				Fighter_KillAllVelocity(Tension_data->fighter_gobj);
				fighterData->coll_data.topN_Curr = fighterData->phys.pos;
				fighterData->coll_data.topN_Prev = fighterData->coll_data.topN_Curr;
				return;
			}
		}

		if(Tension_data->framesleft == 0)// || fighterData->state_id < 14)
		{
			Fighter_ColorRemove(fighterData, 52);
			
			Effect_SpawnAsyncLookup(Tension_data->fighter_gobj, 142, 4, 0, 1, &zero, &zero);
			// Fighter_EnterSpecialFall(Tension_data->fighter_gobj, 0, 1, 0, 0, 30);
			Fighter_EnterFall(Tension_data->fighter_gobj);
			fighterData->phys.pos = Tension_data->stoppos[1];
			// update ecb pos
			fighterData->coll_data.topN_Curr = fighterData->phys.pos;
			fighterData->coll_data.topN_Prev = fighterData->coll_data.topN_Curr;
			Effect_SpawnAsyncLookup(Tension_data->fighter_gobj, 142, 4, 0, 1, &zero, &zero);
			fighterData->jump.jumps_used = 1;
		}
	}
}
