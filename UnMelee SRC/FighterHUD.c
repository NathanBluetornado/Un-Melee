#include "MatchCodes.h";

// #include "TimeStop.c"

// #include <stdio.h>
// #include <stdlib.h>
int (*Fighter_Interrupt_AllAerial)(GOBJ *fighter_gobj) = (void *)0x800CCAAC;
int (*Fighter_Interrupt_AllGrounded)(GOBJ *fighter_gobj) = (void *)0x8008A4D4;

float GetDistance(float x1, float y1, float x2, float y2)
{
	return sqrtf(pow(fabs(x2 - x1), 2) + pow(fabs(y2 - y1), 2));
}

typedef struct FighterVars
{
    Effect *GFX;
} FighterVars;

int GetEnemyPosition(FighterData *fighter_data, int ply, Vec3 *dest)
{
	Vec3 pos;
	//Check if player exists
	if (Fighter_GetGObj(ply) != 0)
	{
		FighterData *player_data = Fighter_GetGObj(ply)->userdata;

		//Check if player is Sonic
		if (fighter_data->ply != ply)
		{
			//Check if player is on Sonic's team
			if (!(Match_CheckIfTeams() && player_data->team == fighter_data->team))
			{
				int hip_bone = Fighter_BoneLookup(player_data, HipN);
				JOBJ_GetWorldPosition(player_data->bones[hip_bone].joint, 0, dest);
				return 1;
			}
		}
	}
	return 0;
}

#include "Roman Cancels.c"
// #include "HitfallandJC.c"
#include "Relax.c"
#include "Pull.c"
#include "Join.c"
#include "Switch.c"
#include "TimeStop.c"
// #include "Gear.c"
#include "Ghost.c"
#include "Rise.c"
// #include "Invert.c"
#include "Random.c"

void DelaytheMf()
{
    GOBJ *gobj_Delay = GObj_Create(3, 0, 0); //Creates a GOBJ to store the Ledge Grab Limit function
    GObj_AddProc(gobj_Delay, DelayLogic, 17);
    void *UserData = calloc(sizeof(DelayVars));          //Set up a pointer for the UserData which will be based on the Combo Variables struct
    GObj_AddUserData(gobj_Delay, 4, HSD_Free, UserData);
    DelayVars *Delay_data = gobj_Delay->userdata;
    Delay_data->FrameDelay = 0;
}

void *DelayLogic(GOBJ *gobj_Delay)
{
    DelayVars *Delay_data = gobj_Delay->userdata;
    // OSReport("%d\n", Delay_data->FrameDelay);
    if (Delay_data->FrameDelay == 2)
    {
        SetupBS();
        GObj_Destroy(gobj_Delay);
    }
    else
    {
        // for (int i = 0; i < 4; i++)
        // {
        //     FighterData *fp = Fighter_GetGObj(i)->userdata;
        //     if (Fighter_GetGObj(i) != 0)
        //     {
        //         if (stc_css_pad[fp->player_controller_number].held & HSD_BUTTON_UP){        //this function checks for inputs on go, and changes number depending on the input
        //             Delay_data->prenumber[i] = 0;
        //         }
        //         else if (stc_css_pad[fp->player_controller_number].held & HSD_BUTTON_DOWN){
        //             Delay_data->prenumber[i] = 1;
        //         }
        //         else if (stc_css_pad[fp->player_controller_number].held & HSD_BUTTON_LEFT){
        //             Delay_data->prenumber[i] = 2;
        //         }
        //         else if (stc_css_pad[fp->player_controller_number].held & HSD_BUTTON_RIGHT){
        //             Delay_data->prenumber[i] = 3;
        //         }
        //         else if (stc_css_pad[fp->player_controller_number].held & HSD_BUTTON_A){
        //             Delay_data->prenumber[i] = 7;
        //         }
        //         else if (stc_css_pad[fp->player_controller_number].held & HSD_TRIGGER_L){
        //             Delay_data->prenumber[i] = 6;
        //         }
        //         else if (stc_css_pad[fp->player_controller_number].held & HSD_TRIGGER_R){
        //             Delay_data->prenumber[i] = 5;
        //         }
        //         else if (stc_css_pad[fp->player_controller_number].held & HSD_TRIGGER_Z){
        //             Delay_data->prenumber[i] = 4;
        //         }
        //     }
        // }
        Delay_data->FrameDelay += 1;
    }
}

void SetupBS()
{
    ArchiveInfo *wpind_archive = File_Load("Meters.dat");
    // ArchiveInfo *wpind_archive = File_GetPreloadedFile("GrNLa.dat");

    for (int i = 0; i < 4; i++) //you're gonna see this used alot, this checks for each player's data, i being the player number
    {
        if (Fighter_GetGObj(i) != 0)
        {
            int randomnumber;
            // randomnumber = HSD_Randi(8);
            // OSReport("%d\n", randomnumber);
            randomnumber = 69;

            // Fighter_InitInputs(Fighter_GetGObj(i));

            FighterData *fp = Fighter_GetGObj(i)->userdata;


            if (fp->input.held & HSD_BUTTON_UP){      //this function checks for inputs on go, and changes number depending on the input
                randomnumber = 0;
            }
            else if (fp->input.held & HSD_BUTTON_DOWN){
                // randomnumber = 1;
                randomnumber = 1;
            }
            else if (fp->input.held & HSD_BUTTON_LEFT){
                randomnumber = 2;
            }
            else if (fp->input.held & HSD_BUTTON_RIGHT){
                randomnumber = 3;
            }
            // else if (stc_css_pad[fp->player_controller_number].held & HSD_BUTTON_B){
            //     randomnumber = 8;
            // }
            else if (fp->input.held & HSD_TRIGGER_L){
                randomnumber = 6;
            }
            else if (fp->input.held & HSD_TRIGGER_R){
                randomnumber = 5;
            }
            else if (fp->input.held & HSD_TRIGGER_Z){
                randomnumber = 4;
            }
            else if (fp->input.held & HSD_BUTTON_A){
                randomnumber = 7;
            }
            else if (randomnumber = 69){
                randomnumber = HSD_Randi(8);                //this spits out a random number if nothing is selected
                OSReport("its random\n");
                LoadRandom(Fighter_GetGObj(i), wpind_archive);
            }

            switch (randomnumber)       //this switch function loads a meter depending on which number you choose
            {
            case 0:
                //RCs 
                //green meter
                LoadTension(Fighter_GetGObj(i), wpind_archive);
                break;
            // case 1:
            //     //HitfallandJC
            //     //No meter (black BG)
            //     // LoadGear(Fighter_GetGObj(i));
            //     // LoadRivals(Fighter_GetGObj(i));
            //     break;
            case 2:
                //Relax
                //teal meter
                LoadRelax(Fighter_GetGObj(i), wpind_archive);
                break;
            case 3:
                //TimeStop; twice per game 100 frame time stop
                //gold meter
                LoadWorld(Fighter_GetGObj(i), wpind_archive);
                break;
            case 4:
                //Switch; twice per stock fullscreen position switch
                //dark blue meter
                LoadSwap(Fighter_GetGObj(i), wpind_archive);
                break;
            case 5:
                //I am on the inside of your walls; twice per kill teleport to the oppoenent
                //dark purple
                LoadJoin(Fighter_GetGObj(i), wpind_archive);
                break;
            case 6:
                //pull; once per stock teleport your oppoent to you
                //red meter
                LoadPull(Fighter_GetGObj(i), wpind_archive);
                break;
            case 7:
                //ghost; turn into ghost lmao
                //white
                LoadGhost(Fighter_GetGObj(i), wpind_archive);
                break;
            case 1:
                //Rise: they get popped up fullscreen
                //orange
                LoadRise(Fighter_GetGObj(i), wpind_archive);
                break;
            default:
                break;
            }
        }
    }
}
