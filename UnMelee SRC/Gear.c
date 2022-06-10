#include "./MatchCodes.h";
#define MaxTension 2000


void LoadGear(GOBJ *fighter_gobj)
{
	FighterData *fighter_data = fighter_gobj->userdata;

	//Initialize Mega Meter
	// HSD_Archive *wpind_archive = Archive_LoadFile("PlSmHUD.dat");//("TensionHUD.dat");
    ArchiveInfo *wpind_archive = File_Load("GrNLa.dat");
	Symbol_WpInd_scene_models *symbol_WpInd_scene_models = File_GetSymbol(wpind_archive, "Gear_scene_models");
	JOBJSet *jobjset_WpInd = symbol_WpInd_scene_models->WpInd_scene_models;
	// GOBJ *gobj_Tension = GObj_Create(3, 0, 0);
	GOBJ *gobj_WpInd = JOBJ_LoadSet(0, jobjset_WpInd, 0, 0, 0, 11, 1, GearLogic);
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
void GearLogic(GOBJ *gobj)//, GOBJ *gobj_Tension)
{
	
    JOBJ *jobj_Gear = gobj->hsd_object;
    JOBJ *jobj_CurrCharge; //Number group JOBJ
    JOBJ_GetChild(jobj_Gear, &jobj_CurrCharge, 0, -1); //joint 0, meter and shit
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
        jobj_Gear->trans = *Match_GetPlayerHUDPos(fighterData->ply);
        jobj_Gear->trans.X = jobj_Gear->trans.X -4.6; //-5.04;
        jobj_Gear->trans.Y = jobj_Gear->trans.Y + 0.95;
		// jobj_Gear->trans.X = jobj_Gear->trans.X -0;
        // jobj_Gear->trans.Y = jobj_Gear->trans.Y - 0.75;
		// jobj_Gear->scale.Y = -1;
		// jobj_Gear->rot.Z = -1.57;
       
		if(fighterData->state_id == ASID_ESCAPEAIR && fighterData->state.frame == 2 && (fighterData->input.lstick.Y < 0.1 && fighterData->input.lstick.Y > -0.1) &&
		(fighterData->input.lstick.X > 0.1 || fighterData->input.lstick.X < -0.1) && fighterData->jump.jumps_used == 1)
		{
			Airdash(Tension_data->fighter_gobj);
			fighterData->jump.jumps_used += 1;
		}

		if(fighterData->state_id == ASID_SQUAT)
		{
			Superjump(Tension_data->fighter_gobj);
		}

		if (fighterData->hitbox->victims->victim_gobj != 0 && fighterData->hitbox->active != 0)
		{
			FighterData *vic = fighterData->hitbox->victims->victim_gobj;
			JC(Tension_data->fighter_gobj);
		}

		JOBJ_ClearFlagsAll(jobj_Gear, JOBJ_HIDDEN);
        JOBJ_SetMtxDirtySub(jobj_Gear);
    }
    else
    {
        JOBJ_SetFlagsAll(jobj_Gear, JOBJ_HIDDEN);
    }
}

/////////////////////////////////
//      TENSION FUNCTIONS      //
/////////////////////////////////
// void MeterLimit(TensionVars *Tension_data)
// {
// 	if (Tension_data->Tension > 2000 )
// 	{
// 		Tension_data->Tension = 2000;
// 	}
// 	if(Tension_data->Tension < 0)
// 	{
// 		Tension_data->Tension = 0;
// 	}
// }
// int DamageReload (GOBJ *fighter_gobj)//, TensionVars *Tension_data)
// {
// 	FighterData *fighterData = fighter_gobj->userdata;
// 	TensionVars *Tension_data = fighter_gobj->userdata;
// 	Tension_data->Tension += 13*fighterData->hitbox->dmg;
// 	return;
// 	// MeterLimit(Tension_data);
// }


/////////////////////
//      Airdash    //
/////////////////////

void airdash_anim(GOBJ *fighter_gobj)
{
	FighterData *fp = fighter_gobj->userdata;

	if (FrameTimerCheck(fighter_gobj) == 0)
    {
        Fighter_EnterFall(fighter_gobj);
    }
}

void fairdash_IASA(GOBJ *fighter_gobj)
{
	FighterData *fp = fighter_gobj->userdata;

	if (fp->state.frame > 15)
    {
        // Fighter_EnterFall(fighter_gobj);
		if ((fp->input.down & (HSD_BUTTON_A | HSD_BUTTON_B)) || (fabs(fp->input.cstick.X) > 0.1 || fabs(fp->input.cstick.Y) > 0.1))
        {
            Fighter_Interrupt_AllAerial(fighter_gobj);
        }
    }
	
}

void bairdash_IASA(GOBJ *fighter_gobj)
{
	FighterData *fp = fighter_gobj->userdata;

	if (fp->state.frame > 10)
    {
       // Fighter_EnterFall(fighter_gobj);
		if ((fp->input.down & (HSD_BUTTON_A | HSD_BUTTON_B)) || (fabs(fp->input.cstick.X) > 0.1 || fabs(fp->input.cstick.Y) > 0.1))
        {
            Fighter_Interrupt_AllAerial(fighter_gobj);
        }
    }
	
}

void airdash_phys(GOBJ *fighter_gobj)
{
	FighterData *fp = fighter_gobj->userdata;

	fp->phys.self_vel.Y = 0;
	Fighter_PhysAir_DecayXVelocity(fp, 0.01);
}

void airdash_coll(GOBJ *fighter_gobj)
{
	FighterData *fp = fighter_gobj->userdata;

	if (Fighter_CollAir_GrabLedge(fighter_gobj, 2) != 0)
    {
        Fighter_EnterLanding(fighter_gobj);
    }
}

void Airdash(GOBJ *fighter_gobj)
{
	FighterData *fp = fighter_gobj->userdata;
	Vec3 zero;
		zero.X = 0;
		zero.Y = 0;
		zero.Z = 0;

	if ((fp->facing_direction == 1 && fp->input.lstick.X > 0.1) || (fp->facing_direction == -1 && fp->input.lstick.X < -0.1))
	{
		ActionStateChange(2, 3, 0, fighter_gobj, ASID_JUMPAERIALF, 0, 0);
		fp->phys.self_vel.X = 2 * fp->facing_direction; //forward
		fp->cb.IASA = fairdash_IASA;
	}
	else
	{
		ActionStateChange(2, 4, 0, fighter_gobj, ASID_JUMPAERIALB, 0, 0);
		fp->phys.self_vel.X = -1 * fp->facing_direction; //back
		fp->cb.IASA = bairdash_IASA;
	}

	Effect_SpawnAsyncLookup(fighter_gobj, 39, 4, 0, 1, &zero, &zero);
	fp->cb.Anim = airdash_anim;
	fp->cb.Phys = airdash_phys;
	fp->cb.Coll = airdash_coll;

	return;
}

/////////////////////////
//         JC	       //
/////////////////////////

void JC(GOBJ *fighter_gobj)
{
	FighterData *fp = fighter_gobj->userdata;

	switch (fp->state_id)
	{
	case ASID_ATTACK100END: //jabs (can cancel into specials, smashes, and tilts)
	case ASID_ATTACK100LOOP:
	case ASID_ATTACK100START:
	case ASID_ATTACK11:
	case ASID_ATTACK12:
	case ASID_ATTACK13:
	case ASID_ATTACKDASH:
	case ASID_ATTACKHI3:
	case ASID_ATTACKLW3:
	case ASID_ATTACKS3HI:
	case ASID_ATTACKS3HIS:
	case ASID_ATTACKS3LW:
	case ASID_ATTACKS3LWS:
	case ASID_ATTACKS3S:
		if (Fighter_CheckJumpInput(fighter_gobj))
		{
			if(fp->input.lstick.Y < -0.1)
			{
				superjumpASC(fighter_gobj);
			}
			else
			{
            Fighter_Interrupt_AllGrounded(fighter_gobj);
			Fighter_AdvanceScript(fighter_gobj);
			}
        }
		break;

	default:
		break;
	}

	return;
}


/////////////////////////
//      Superjump      //
/////////////////////////

void superjump_IASA(GOBJ *fighter_gobj)
{
	FighterData *fp = fighter_gobj->userdata;

	if (fp->state.frame > 8)
    {
       // Fighter_EnterFall(fighter_gobj);
		if ((fp->input.down & (HSD_BUTTON_A) | (fp->input.down & (HSD_BUTTON_B)) || (fabs(fp->input.cstick.X) > 0.1 || fabs(fp->input.cstick.Y) > 0.1)))
		//((fp->input.held & (HSD_BUTTON_A || HSD_BUTTON_B)) || (fabs(fp->input.cstick.X) > 0.1 || fabs(fp->input.cstick.Y) > 0.1))
        {
            Fighter_Interrupt_AllAerial(fighter_gobj);
        }
    }
}

void superjump_phys(GOBJ *fighter_gobj)
{
	FighterData *fp = fighter_gobj->userdata;
	Fighter_PhysAir_ApplyGravityDecayX(fighter_gobj);

	if (fabs(fp->input.lstick.X) > 0.1 && fp->state.frame < 8 && fp->state.frame > 1)
	{
		// fp->phys.self_vel.X = fp->input.lstick.X * (3/fp->state.frame);
		fp->phys.self_vel.X = fp->input.lstick.X;
	}
	
}

void superjump_coll(GOBJ *fighter_gobj)
{
	FighterData *fp = fighter_gobj->userdata;
	Fighter_CollAir_IgnoreLedge_NoCB(fighter_gobj);
}

void superjump_anim(GOBJ *fighter_gobj)
{
	FighterData *fp = fighter_gobj->userdata;

	if (FrameTimerCheck(fighter_gobj) == 0)
    {
        Fighter_EnterFall(fighter_gobj);
    }
}

void superjumpASC(GOBJ *fighter_gobj)
{
	FighterData *fp = fighter_gobj->userdata;
	Vec3 zero;
			zero.X = 0;
			zero.Y = 0;
			zero.Z = 0;
		
		ActionStateChange(0, 1, 1, fighter_gobj, ASID_JUMPF, 0, 0);
		Effect_SpawnAsyncLookup(fighter_gobj, 39, 4, 0, 1, &zero, &zero);
		Fighter_SetAirborneNoJumps(fp);
		fp->phys.self_vel.Y = 5;
		fp->cb.Anim = superjump_anim;
		fp->cb.Phys = superjump_phys;
		fp->cb.IASA = superjump_IASA;
		fp->cb.Coll = superjump_coll;
}

void squatsuperjump_IASA(GOBJ *fighter_gobj)
{
	FighterData *fp = fighter_gobj->userdata;

	if (fp->input.down & (HSD_BUTTON_X | HSD_BUTTON_Y) || fp->input.lstick.Y > 0.2)
	{
		superjumpASC(fighter_gobj);
	}
	if ((fp->input.down & (HSD_BUTTON_A | HSD_BUTTON_B)) || (fabs(fp->input.cstick.X) > 0.1 || fabs(fp->input.cstick.Y) > 0.1))
    {
        Fighter_Interrupt_AllGrounded(fighter_gobj);
    }

	return;
}

void Superjump(GOBJ *fighter_gobj)
{
	FighterData *fp = fighter_gobj->userdata;
	
	fp->cb.IASA = squatsuperjump_IASA;
	
	return;
}
