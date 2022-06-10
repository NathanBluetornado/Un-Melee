#include "./MatchCodes.h";
#define MaxTension 2000

void ToggleStageAnimsAndEffects(int off)
{
	//Everything in this function was coded by the goat Unclepunch
	//If not for him, I would have had no idea how to do something like this and the World's effect would have lost a lot of impact, so thanks again!
	
	u8 *proc_num = 0x804ce382;
	GOBJProc ***proc_lookup = 0x804D7840;
	GOBJ **gobj_lookup = *stc_gobj_list;

	// toggle stage animation
	for (int i = 0; i < *proc_num; i++)
	{
		GOBJProc *this_proc = proc_lookup[0][i];
		while (this_proc)
		{
			if (this_proc->cb &&
				this_proc->parentGOBJ &&
				(this_proc->parentGOBJ->p_link == MATCHPLINK_MAP || this_proc->parentGOBJ->p_link == MATCHPLINK_LIGHT))
			{
				this_proc->flags ^= 0x80;
				// OSReport("gobj %x's cb %x set to %2x", this_proc->parentGOBJ, this_proc->cb, this_proc->flags & 0x80);
				// this_proc->flags ^= 0x80;
			}

			this_proc = this_proc->next;
		}
	}

	// toggle existing particles
	// (disabled due to frequent visual glitches)
	/*
	Particle2 *this_ptcl = *stc_ptcl;
	while (this_ptcl)
	{

		this_ptcl->kind ^= 0x800; // 80399378

		this_ptcl = this_ptcl->next;

		OSReport("kind %x", this_ptcl->kind);
	}

	// toggle existing particle gens
	ptclGen *this_ptcl_gen = *stc_ptclgen;
	while (this_ptcl_gen)
	{
		this_ptcl_gen->kind ^= 0x800;

		this_ptcl_gen = this_ptcl_gen->next;
	}

	// toggle non-robj model effects
	for (int i = 0; i < 2; i++)
	{
		GOBJ *this_eff = gobj_lookup[MATCHPLINK_EFFECT1 + i];
		while (this_eff)
		{
			// ensure it's an actual effect
			if (this_eff->entity_class == 8)
			{
				Effect *eff_data = this_eff->userdata;

				if (eff_data)
				{

					JOBJ *eff_jobj = this_eff->hsd_object;
					// OSReport("eff %x has parent %x, robj %x", this_eff, eff_data->parent, eff_jobj->RObj);

					if (!eff_jobj->RObj)
					{
						if (eff_data->x28 & 2 & off != 1)
							eff_data->x28 = 0;
						else
							eff_data->x28 = 2;
					}
				}
			}

			this_eff = this_eff->next;
		}
	}
	*/
}
		
void LoadWorld(GOBJ *fighter_gobj, ArchiveInfo *wpind_archive)
{
	FighterData *fighter_data = fighter_gobj->userdata;

	//Initialize Mega Meter
	// HSD_Archive *wpind_archive = Archive_LoadFile("PlSmHUD.dat");//("TensionHUD.dat");
    // ArchiveInfo *wpind_archive = File_Load("GrNLa.dat");
	Symbol_WpInd_scene_models *symbol_WpInd_scene_models = File_GetSymbol(wpind_archive, "World_scene_models");
	JOBJSet *jobjset_WpInd = symbol_WpInd_scene_models->WpInd_scene_models;
	// GOBJ *gobj_Tension = GObj_Create(3, 0, 0);
	GOBJ *gobj_WpInd = JOBJ_LoadSet(0, jobjset_WpInd, 0, 0, 2, 11, 1, WorldLogic);
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
void WorldLogic(GOBJ *gobj)//, GOBJ *gobj_Tension)
{
	
    JOBJ *jobj_World = gobj->hsd_object;
    JOBJ *jobj_CurrCharge; //Number group JOBJ
    JOBJ_GetChild(jobj_World, &jobj_CurrCharge, 0, -1); //joint 0, meter and shit
    // TensionVars *RC_hud_data = gobj->userdata;

	// GOBJ *gobj_Tension = GObj_Create(3, 0, 0);
	// void *UserData = calloc(sizeof(TensionVars));          //Set up a pointer for the UserData which will be based on the Combo Variables struct
    // GObj_AddUserData(gobj_Tension, 4, HSD_Free, UserData);
	TensionVars *Tension_data = gobj->userdata;

	// Tension_data->Tension = Tension_data->Tension+ 1000;
	// OSReport("%d \n", Tension_data->Tension);

   WorldOnfame(Tension_data->fighter_gobj, gobj);
	
    if (Tension_data->fighter_gobj != 0 && (Pause_CheckStatus(1) != 2) && (Fighter_GetStocks(Tension_data->ply) > 0))
    {
        FighterData *fighterData = Tension_data->fighter_gobj->userdata;
        // SM2Vars *sm2_vars = &ft_data->fighter_var;
        jobj_World->trans = *Match_GetPlayerHUDPos(fighterData->ply);
        jobj_World->trans.X = jobj_World->trans.X -4.6; //-5.04;
        jobj_World->trans.Y = jobj_World->trans.Y + 0.95;
		// jobj_World->trans.X = jobj_World->trans.X -0;
        // jobj_World->trans.Y = jobj_World->trans.Y - 0.75;
		// jobj_World->scale.Y = -1;
		// jobj_World->rot.Z = -1.57;

		Vec3 zero;
		zero.X = 0;
		zero.Y = 0;
		zero.Z = 0;
       
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
		if (JOBJ_GetAnimFrame(jobj_CurrCharge) == 200)
		{
			JOBJ_ReqAnimAll(jobj_CurrCharge, 99);
		}
        JOBJ_ClearFlagsAll(jobj_World, JOBJ_HIDDEN);
        JOBJ_SetMtxDirtySub(jobj_World);
		
		// if (((fighterData->kind == LUIGI) && (fighterData->state_id < 343 || fighterData->state_id > 354)) ||
		// 	((fighterData->kind == PIKACHU) && (fighterData->state_id < 343 || fighterData->state_id > 352)) ||
		// 	((fighterData->kind == PICHU) && (fighterData->state_id < 343 || fighterData->state_id > 352)) ||
		// 	(!(fighterData->kind == LUIGI || fighterData->kind == PIKACHU || fighterData->kind == PICHU)))
		// {
		if (fighterData->hitbox->victims->victim_gobj != 0 && fighterData->hitbox->active != 0)
		{
			FighterData *vic = fighterData->hitbox->victims->victim_gobj;
			Tension_data->Tension += fighterData->hitbox->dmg*1.2;///1.5; //+ 10;
			// Gatling(Tension_data->fighter_gobj);
		}
		

		if (Tension_data->Tension > MaxTension)
		{
			Tension_data->Tension = MaxTension;
		}

		if(Tension_data->framesleft > 0)// || fighterData->state_id < 14)
		{
			Tension_data->framesleft -= 1;
			for (int i = 0; i < 4; i++)
			{
				Vec3 pos;

				if (GetEnemyPosition(fighterData, i, &pos) != 0)
				{
					FighterData *victimData = Fighter_GetGObj(i)->userdata;

					if (victimData->state.frame > 0)
					{
						ActionStateChange(victimData->state.frame - 1, 1, 0, Fighter_GetGObj(i), victimData->state_id, 0, 0);
					}
					else
					{
						ActionStateChange(0, 1, 0, Fighter_GetGObj(i), victimData->state_id, 0, 0);
					}

					if (victimData->dmg.time_since_hit == 0)
					{
						Tension_data->kb[i] = victimData->phys.kb_vel;	
					}
					else
					{
						victimData->phys.kb_vel = Tension_data->kb[i];
					}
					if (Tension_data->framesleft == 1)
					{
						break;
					}
					victimData->phys.pos = Tension_data->stoppos[i];
					victimData->cb.IASA = 0;
					victimData->phys.self_vel = zero;
				}
			}
			JOBJ_ReqAnimAll(jobj_CurrCharge, Tension_data->framesleft + 100);
		}
		if (Tension_data->framesleft == 1)
		{
			ToggleStageAnimsAndEffects(1);
		}
    }
    else
    {
        JOBJ_SetFlagsAll(jobj_World, JOBJ_HIDDEN);
    }
}


/////////////////////////////////
//      TIMESLOW FUNCTIONS     //
/////////////////////////////////


void WorldOnfame(GOBJ *fighter_gobj, GOBJ *tension_gobj)
{
	// FighterData *fighterdata = fighter_gobj->userdata;
	FighterData *fighterData = fighter_gobj->userdata;
	FighterData *fp = fighter_gobj->userdata;
	TensionVars *Tension_data = tension_gobj->userdata;

	
	// OSReport("%d \n", fighterData->fighter_var.ft_var51);
	// OSReport("%f \n", fighterData->input_stickangle);
	// OSReport("%d \n", (fighterData->common_state_num + sizeof(FtState) + 3));	
	if(fighterData->state_id < ASID_RUNBRAKE & fighterData->state_id > ASID_WAIT)
	{
		Tension_data->Tension = Tension_data->Tension + 1;
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
					Tension_data->Tension = Tension_data->Tension + 3;
				}
			}
		}
	}
	
	Vec3 zero;
		zero.X = 0;
		zero.Y = 0;
		zero.Z = 0;
		
	
	if (Tension_data->Tension == 2000)
	{
		if (fighterData->input.held & (HSD_TRIGGER_R))
		{	
			if (fighterData->input.held & (HSD_TRIGGER_L))
			{
				
				if (fighterData->state_id > ASID_REBIRTHWAIT)
				{   
					if (fighterData->state_id == 266 || fighterData->state_id == ASID_PASS || fighterData->state_id == ASID_GUARDOFF || fighterData->state_id == ASID_FALL) {}
					else
					{
						
						for (int i = 0; i < 4; i++)
						{
							Vec3 pos;

							if (GetEnemyPosition(fighterData, i, &pos) != 0)
							{
								FighterData *victimData = Fighter_GetGObj(i)->userdata;
								
								if (victimData->state_id > ASID_REBIRTHWAIT)
								{
									Tension_data->stoppos[i] = victimData->phys.pos;
									Tension_data->kb[i] = victimData->phys.kb_vel;		
								}
								else{
									return;
								}
									
							}
						}
						Fighter_PlaySFX(fighter_gobj, 315, 127, 0);
						// Fighter_PlaySFX(fighter_gobj, 528, 127, 0);
						Tension_data->Tension -= 2000;
						Tension_data->framesleft = 100;
						ToggleStageAnimsAndEffects(0);
					}
				}
			}
		}
	}	
}
