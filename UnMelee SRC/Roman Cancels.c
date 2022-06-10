#include "./MatchCodes.h";
#define MaxTension 2000


void LoadTension(GOBJ *fighter_gobj, ArchiveInfo *wpind_archive)
{
	FighterData *fighter_data = fighter_gobj->userdata;

	//Initialize Mega Meter
	// HSD_Archive *wpind_archive = Archive_LoadFile("PlSmHUD.dat");//("TensionHUD.dat");
    // ArchiveInfo *wpind_archive = File_Load("GrNLa.dat");
	Symbol_WpInd_scene_models *symbol_WpInd_scene_models = File_GetSymbol(wpind_archive, "RC_scene_models");
	JOBJSet *jobjset_WpInd = symbol_WpInd_scene_models->WpInd_scene_models;
	// GOBJ *gobj_Tension = GObj_Create(3, 0, 0);
	GOBJ *gobj_WpInd = JOBJ_LoadSet(0, jobjset_WpInd, 0, 0, 0, 11, 1, RCLogic);
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
void RCLogic(GOBJ *gobj)//, GOBJ *gobj_Tension)
{
	
    JOBJ *jobj_RC = gobj->hsd_object;
    JOBJ *jobj_CurrCharge; //Number group JOBJ
    JOBJ_GetChild(jobj_RC, &jobj_CurrCharge, 0, -1); //joint 0, meter and shit
    // TensionVars *RC_hud_data = gobj->userdata;

	// GOBJ *gobj_Tension = GObj_Create(3, 0, 0);
	// void *UserData = calloc(sizeof(TensionVars));          //Set up a pointer for the UserData which will be based on the Combo Variables struct
    // GObj_AddUserData(gobj_Tension, 4, HSD_Free, UserData);
	TensionVars *Tension_data = gobj->userdata;

	// Tension_data->Tension = Tension_data->Tension+ 1000;
	// OSReport("%d \n", Tension_data->Tension);

    RomanCancelOnfame(Tension_data->fighter_gobj, gobj);
	
    if (Tension_data->fighter_gobj != 0 && (Pause_CheckStatus(1) != 2) && (Fighter_GetStocks(Tension_data->ply) > 0))
    {
        FighterData *fighterData = Tension_data->fighter_gobj->userdata;
        // SM2Vars *sm2_vars = &ft_data->fighter_var;
        jobj_RC->trans = *Match_GetPlayerHUDPos(fighterData->ply);
        jobj_RC->trans.X = jobj_RC->trans.X -4.6; //-5.04;
        jobj_RC->trans.Y = jobj_RC->trans.Y + 0.95;
		// jobj_RC->trans.X = jobj_RC->trans.X -0;
        // jobj_RC->trans.Y = jobj_RC->trans.Y - 0.75;
		// jobj_RC->scale.Y = -1;
		// jobj_RC->rot.Z = -1.57;
       
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
        JOBJ_ClearFlagsAll(jobj_RC, JOBJ_HIDDEN);
        JOBJ_SetMtxDirtySub(jobj_RC);
		
		// if (((fighterData->kind == LUIGI) && (fighterData->state_id < 343 || fighterData->state_id > 354)) ||
		// 	((fighterData->kind == PIKACHU) && (fighterData->state_id < 343 || fighterData->state_id > 352)) ||
		// 	((fighterData->kind == PICHU) && (fighterData->state_id < 343 || fighterData->state_id > 352)) ||
		// 	(!(fighterData->kind == LUIGI || fighterData->kind == PIKACHU || fighterData->kind == PICHU)))
		// {
			if (fighterData->hitbox->victims->victim_gobj != 0 && fighterData->hitbox->active != 0)
			{
				FighterData *vic = fighterData->hitbox->victims->victim_gobj;
				Tension_data->Tension += fighterData->hitbox->dmg *2;// + 10;
				// Gatling(Tension_data->fighter_gobj);
			}
		// }

		if (/*fighterData->dmg.source->userdata != 0 && */ fighterData->dmg.time_since_hit == 0)
		{
			Tension_data->Tension += 5*2;
		}

		if(fighterData->shield.dmg_taken > 0)
		{
			Tension_data->Tension += 5*2;
		}

		if (Tension_data->Tension > MaxTension)
		{
			Tension_data->Tension = MaxTension;
		}

		if(Tension_data->Tension < 0)// || fighterData->state_id < 14)
		{
			Tension_data->Tension = 0;
		}

		if(fighterData->state_id < 14 || fighterData->state_id == ASID_ENTRY)
		{
			Tension_data->Tension = 1000;
		}	
    }
    else
    {
        JOBJ_SetFlagsAll(jobj_RC, JOBJ_HIDDEN);
    }
}


/////////////////////////////////
//      TENSION FUNCTIONS      //
/////////////////////////////////


void TimeSlow (GOBJ *fighter_gobj, float rate, float Ykb, float KB_type) //kb_type 0 = no ykb, 1 = regular, 2 = YRC
{
	FighterData *fighterData = fighter_gobj->userdata;
	for (int i = 0; i < 4; i++)
		{
			Vec3 pos;

			if (GetEnemyPosition(fighterData, i, &pos) != 0)
			{
				float distance = GetDistance(fighterData->phys.pos.X, fighterData->phys.pos.Y, pos.X, pos.Y);
				if (distance < 35)
				{
					FighterData *victimData = Fighter_GetGObj(i)->userdata;
					
					if (victimData->hurtstatus.game == 0)
					{
						if (KB_type == 2){

							// if (fighterData->phys.air_state == 1 )
							// {
								ActionStateChange(0, rate, 0, Fighter_GetGObj(i), ASID_DAMAGEFLYROLL, 0, 0);
								victimData->phys.kb_vel.X = 0;
								victimData->phys.kb_vel.Y = Ykb;
								fighterData->cb.IASA == null;
							// }
							// else{
								// ActionStateChange(0, rate, 0, Fighter_GetGObj(i), ASID_STOPWALL, 0, 0);
							// }
							
						}
						if (!(rate == 1))
						{
							ActionStateChange(victimData->state.frame, rate, victimData->state.blend, Fighter_GetGObj(i), victimData->state_id, 0, 0);
						}
						// victimData->phys.kb_vel.X = victimData->phys.kb_vel.X * 0.5;
						// victimData->phys.kb_vel.Y = victimData->phys.kb_vel.Y *0.5;
						// Fighter_KillAllVelocity(victimData);					
						if (KB_type == 1){
							victimData->phys.kb_vel.X = 0;
							victimData->phys.kb_vel.Y = Ykb;
						}
					}
				}
			}
		}
}

void RomanCancelOnfame(GOBJ *fighter_gobj, GOBJ *tension_gobj)
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
		Tension_data->Tension = Tension_data->Tension + 1;
		// MeterLimit(Tension_data->Tension);
	}

	// if(fighterData->state_id < ASID_RUNBRAKE & fighterData->state_id > ASID_WAIT)
	if (fighterData->state_id == ASID_RUN)// || fighterData->state_id == ASID_DASH)
	{
		for (int i = 0; i < 4; i++)
		{
			Vec3 pos;

			if (GetEnemyPosition(fighterData, i, &pos) != 0)
			{
				if ((fighterData->facing_direction == 1 && fighterData->phys.pos.X < pos.X) || (fighterData->facing_direction < 1 && fighterData->phys.pos.X > pos.X))
				{
					Tension_data->Tension = Tension_data->Tension + 5;
				}
			}
		}
		// MeterLimit(Tension_data->Tension);
	}	
	
	Vec3 zero;
		zero.X = 0;
		zero.Y = 0;
		zero.Z = 0;
		
	
	if (Tension_data->Tension  >= 1000){
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
										if ((fighterData->hitbox->victims->victim_gobj != 0 && fighterData->hitbox->active != 0) ||
										(fighterData->state_id >= ASID_THROWF && fighterData->state_id <= ASID_THROWLW)){ //RRC
											// OSReport("RC CANCEL \n");
											fighterData->phys.self_vel.X = 0;
											fighterData->phys.self_vel.Y = 1;

											if (fighterData->phys.air_state == 1 )
											{
												Fighter_EnterFall(fighter_gobj);
											}
											else
											{
												ActionStateChange(1,1,1,fighter_gobj,ASID_GUARDOFF,0,0);
											}
											
											Effect_SpawnAsyncLookup(fighter_gobj, 199, 4, 0, 1, &zero, &zero);
											// Effect_SpawnFtEffectLookup(fighter_gobj, 199, 4, 0, 1); //red
											Tension_data->Tension -= 1000;
											TimeSlow(fighter_gobj, 1, 2.5, 1);
											
											if (fighterData->grab.victim)
											{
												Fighter_ThrownRelease(fighter_gobj, fighterData->grab.victim);
											}
										}

										else{

											if ((fighterData->state_id > 13 && fighterData->state_id < 44) || (fighterData->state_id > 232 && fighterData->state_id < 237) || (fighterData->state_id == 182) || (fighterData->state_id == 178) || (fighterData->state_id == 35) ||
											(fighterData->kind == YOSHI && fighterData->state_id == 341) ){ //BRC
											// else{
												// OSReport("Blue RC \n");
												// fighterData->phys.self_vel.X = fighterData->input.lstick.X * 1.3;
												// fighterData->phys.self_vel.Y = 1; 

												if (fighterData->phys.air_state == 1 )
												{
													// Fighter_EnterFall(fighter_gobj);
													ActionStateChange(1,1,1,fighter_gobj,ASID_PASS,0,0);
													fighterData->phys.self_vel.X = fighterData->input.lstick.X * 1.3;
													fighterData->phys.self_vel.Y = 1;
												}
												else
												{
													ActionStateChange(1,1,1,fighter_gobj,ASID_GUARDOFF,0,0);
													fighterData->phys.self_vel_ground.X = fighterData->input.lstick.X * 3;
												}
												
												Effect_SpawnAsyncLookup(fighter_gobj, 144, 4, 0, 1, &zero, &zero);
												// Effect_SpawnFtEffectLookup(fighter_gobj, 144, 4, 0, 1); //blue
												Tension_data->Tension -= 1000;
												TimeSlow(fighter_gobj, 0.2, 3, 1);
											}
											
											else if (!(fighterData->state_id < 179 || fighterData->state_id > 181)){ //YRC
												
												// OSReport("Yellow RC \n");

												ActionStateChange(0,0.5,1,fighter_gobj,ASID_GUARDOFF,0,0);
												
												Effect_SpawnAsyncLookup(fighter_gobj, 143, 4, 0, 1, &zero, &zero);
												Tension_data->Tension -= 1000;
												TimeSlow(fighter_gobj, 0.7, -1.5, 2);
											}
											
											else{ //PRC

												// OSReport("Purple RC \n");

												if (fighterData->phys.air_state == 1 )
												{
													// Fighter_EnterFall(fighter_gobj);
													ActionStateChange(1,1,1,fighter_gobj,ASID_PASS,0,0); //ASID_PASS
													
													fighterData->phys.self_vel.X = fighterData->input.lstick.X * 1.3;
													fighterData->phys.self_vel.Y = 1;
												}
												else
												{
													ActionStateChange(1,1,1,fighter_gobj,ASID_GUARDOFF,0,0);
													fighterData->phys.self_vel_ground.X = fighterData->input.lstick.X * 2.5;
												}
												
												Effect_SpawnAsyncLookup(fighter_gobj, 349, 4, 0, 1, &zero, &zero);
												Tension_data->Tension -= 1000;
												TimeSlow(fighter_gobj, 0.5, 0, 0);
												}
											}
										}
										
										Fighter_PlaySFX(fighter_gobj, 315, 127, 0);
									}
								}
							}
						}
						// else if (fighterData->flags.hitstun)
						// {
						// 	fighterData->phys.self_vel.X = 0;
						// 	fighterData->phys.self_vel.Y = 1;
						// 	if (fighterData->phys.air_state == 1 )
						// 	{
						// 		Fighter_EnterFall(fighter_gobj);
						// 	}
						// 	else
						// 	{
						// 		ActionStateChange(1,1,1,fighter_gobj,ASID_GUARDOFF,0,0);
						// 	}
						// 	Effect_SpawnAsyncLookup(fighter_gobj, 199, 4, 0, 1, &zero, &zero);
						// 	// Effect_SpawnFtEffectLookup(fighter_gobj, 199, 4, 0, 1); //red
						// 	Tension_data->Tension -= 1000;
						// 	TimeSlow(fighter_gobj, 1, 2.5, 1);
						// }
				}
			}
}
