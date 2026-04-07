#include <stdlib.h>
#include <unistd.h>
#include "types.h"
#include "sflags.h"
#include "sendsys.h" 
#include "kernel.h"
#include "cflags.h" 
#include "eflags.h"
#include "locations.h"
#include "mudmacros.h"
#include "bprintf.h"
#include "objsys.h"
#include "objects.h"
#include "mobiles.h"
#include "mobile.h"
#include "oflags.h"
#include "quests.h"
#include "rooms.h"
#include "nfis.h"
#include "meta.h"
#include "lflags.h"
#include "bflags.h"
#include "log.h"
#include "combat.h"
#include "commands.h"
#include "climate.h"
#include "classes.h"

#ifdef LOCMIN_META

Boolean meta_dig(void)
{
	if ( (ploc(mynum) == LOC_META_META75)
	&&   (oloc(OBJ_META_TOADSTOOL)==LOC_LIMBO_LIMBO)
	    )
	{
		setoloc(OBJ_META_TOADSTOOL, ploc(mynum), IN_ROOM);
		bprintf(
"You &+Cuproot&* a large &+Mpurple &+Ctoadstool&*.\n"
			);
		send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s &+Cuproots&* a large, &+Mpurple &+Ctoadstool&*.\n",pname(mynum)
			);
		return True;
	}

	if ( (ploc(mynum) == LOC_META_META1)
 	&&   (oloc(OBJ_META_VIAL_SNAKE)==LOC_LIMBO_LIMBO)
           )
	{
		setoloc(OBJ_META_VIAL_SNAKE, ploc(mynum), IN_ROOM);
		bprintf(
"Rooting around in the &+ydirt&* you find a strange &+Cvial&*.\n"); 
		send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s digs around in the &+ydirt&* and finds a &+Cvial&*.\n", pname(mynum));
		return True;
	}

    	return False;
}

void meta_eat(int b)
{
int k;
int i;
	if ( (b==OBJ_META_LIQUID_HUMAN)
	&&   (pmetatype(mynum)==META_HUMAN)
	    )
	{
		bprintf("You are already &+Chuman&*!\n");
		return;
	}

	if ( (b==OBJ_META_LIQUID_HUMAN)
	&&   (pmetatype(mynum)!=META_HUMAN)
	&&   (ploc(mynum)!=LOC_META_META72)
	    )
	{
		bprintf(
"As you take a sip of the &+Cliquid&*, a violent spasm shoots down your throat,\n"
"choking off your screams of pain as your body cracks and breaks, reforming\n"
"into the form of a human.\n"
				);
                for (i=OBJ_META_MARKEROBJ1;i<=OBJ_META_MARKEROBJ3;i++)
                {
                        if ( (oloc(i)!=LOC_LIMBO_LIMBO)
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, LOC_LIMBO_LIMBO, IN_ROOM);
                        }
                }
                send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s sips some &+Cliquid&* and reforms into a human.\n", pname(mynum)
				);
		setpmetatype(mynum, META_HUMAN);
		return;
	}
	if ( (b==OBJ_META_LIQUID_HUMAN)
	&&   (pmetatype(mynum)!=META_HUMAN)
	&&   (ploc(mynum)==LOC_META_META72)
	&&   (oloc(OBJ_META_STAFF)!=LOC_LIMBO_LIMBO)
	    )
	{
		bprintf(
"As you take a sip of the &+Cliquid&*, a violent spasm shoots down your throat,\n"
"choking off your screams of pain as your body cracks and breaks, reforming\n"
"into the form of a human.\n"
				);
                for (i=OBJ_META_MARKEROBJ1;i<=OBJ_META_MARKEROBJ3;i++)
                {
                        if ( (oloc(i)!=LOC_LIMBO_LIMBO)
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, LOC_LIMBO_LIMBO, IN_ROOM);
                        }
                }
                send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s sips some &+Cliquid&* and reforms into a human.\n", pname(mynum)
                                );

		setpmetatype(mynum, META_HUMAN);
		return;
	}

	if ( (b==OBJ_META_LIQUID_HUMAN)
	&&   (pmetatype(mynum)!=META_HUMAN)
	&&   (ploc(mynum)==LOC_META_META72)
	&&   (oloc(OBJ_META_STAFF)==LOC_LIMBO_LIMBO)
	    )
	{
		bprintf(
"As you take a sip of the &+Cliquid&*, a violent spasm shoots down your throat,\n"
"choking off your screams of pain as your body cracks and breaks, reforming\n"
"into the form of a human.\n"
				);
		bprintf(
"The &+Ctoadstool &+Mring&* shimmers and sparkles, and in a flash of &+Ylight&* a &+Mf&+Ca&+Mi&+Cr&+My&*\n"
"appears in front of you.\n"
"'&+C%s&+M! So the legends were true and you have finally come to rid this\n"
"&+Mland &+Mof the scourge of the Chimera. Take this &+Cstaff&+M, for only it can\n"
"&+Mhope to &+Mpenetrate the evil of the Chimera and slay it once and for all!&*'\n"
"She hands you a large &+Cstaff&*, &+Rkisses&* you lightly on the &+Mcheek&* and\n"
"vanishes as quickly as she arrived.\n", pname(mynum)
				);
		setoloc(OBJ_META_STAFF, mynum, CARRIED_BY);
                for (i=OBJ_META_MARKEROBJ1;i<=OBJ_META_MARKEROBJ3;i++)
                {
                        if ( (oloc(i)!=LOC_LIMBO_LIMBO)
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, LOC_LIMBO_LIMBO, IN_ROOM);
                        }
                }
                send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s sips some &+Cliquid&* and reforms into a human.\n", pname(mynum)
                                );
		setpmetatype(mynum, META_HUMAN);
		return;
	}

	if ( (b==OBJ_META_TOADSTOOL)
	&&   (ploc(mynum)==LOC_META_META75)
	    )
	{
		bprintf(
"As you eat the &+Ctoadstool&*, a strange feeling overpowers you and you black out.\n"
"\n"
"You awaken in a &+Gclearing&* identical to the one you were in, but the way you\n"
"entered is now gone, and a new exit is revealed to the &+Gnorth&*, a &+Ccauldron&*\n"
"now dominating the centre of the ring.\n"
			);
if(pnumobs(mynum)>0)
{
bprintf(
"Looking down, you see your equipment is gone\n"
	);
dumpstuff(mynum, LOC_META_META75);
}
		send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s devours a &+Ctoadstool&* and vanishes before your eyes.\n",pname(mynum)
			);
		trapch(LOC_META_META72);
		eat (b);
		return;
	}

        if ( (b==OBJ_META_VIAL_SNAKE)
	&&   (pmetatype(mynum)==META_HUMAN)
	   )
	{
		bprintf(
"As you consume the &+Ggreen&* liquid, your &+Cbody&* begins to &+mshrink&*.\n"
"Your arms &+msubsume&* into your &+Cbody&*, and your &+Clegs&* &+mmerge&* into one\n"
"limb which rapidly becomes more a part of your &+Ctorso&* than a seperate\n"
"limb. Within seconds, your &+Cbody&* has changed into that of a &+Gsnake&*.\n"
			);
		send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
"&+C%s&* greedily drinks from the &+Cvial&*, and shortly thereafter you notice\n"
"&+C%s&* form changing into that of a &+Csnake&*.\n", pname(mynum), his_or_her(mynum)
			);
		setpmetatype(mynum,META_SNAKE);
		dumpstuff(mynum, ploc(mynum));
                for (i=OBJ_META_MARKEROBJ2;i<=OBJ_META_MARKEROBJ3;i++)
                {
                        if ( (oloc(i)==LOC_LIMBO_LIMBO)
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, mynum, CARRIED_BY);
				setpwpn(mynum, i);
                                return;
                        }
                }
		return;
	}

        if ( (b==OBJ_META_VIAL_SNAKE)
	&&   ((pmetatype(mynum)==META_SNAKE)
	||    (pmetatype(mynum)==META_SNAKE_UNDERWATER)
	      )
	    )
        {
                bprintf("You are already a &+Gsnake&*!\n");
                return;
        }


        if ( (b==OBJ_META_VIAL_SNAKE)
	&&   (pmetatype(mynum)==META_BIRD)
	    )
        {
                bprintf(
"As you consume the &+Ggreen&* liquid, your &+Cbody&* begins to grow.\n"
"Your &+Cwings &+msubsume&* into your &+Cbody&*, and your &+Clegs &+mmerge&* into one\n"
"limb which rapidly becomes more a part of your &+Ctorso&* than a seperate\n"
"limb. Within seconds, your &+Cbody&* has changed into that of a &+Gsnake&*.\n"
                        );
		send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s consumes some &+Ggreen&* liquid, and turns into a &+Csnake&*.\n", pname(mynum)
				);
		setpmetatype(mynum, META_SNAKE);
		dumpstuff(mynum, ploc(mynum));
                for (i=OBJ_META_MARKEROBJ1;i<=OBJ_META_MARKEROBJ2;i++)
                {
                        if ( (oloc(i)==ploc(mynum))
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
				setoloc(i, LOC_LIMBO_LIMBO, IN_ROOM);
                        }
                }
                for (i=OBJ_META_MARKEROBJ2;i<=OBJ_META_MARKEROBJ3;i++)
                {
                        if ( (oloc(i)==LOC_LIMBO_LIMBO)
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, mynum, CARRIED_BY);
                                setpwpn(mynum, i);
                                return;
                        }
                }
	return;       
        }

        if ( (b==OBJ_META_VIAL_SNAKE)
	&&   (pmetatype(mynum)==META_BEETLE)
	&&   (ploc(mynum)!=LOC_META_FISHSTOMACH)
	&&   (ploc(mynum)!=LOC_META_BIRDSTOMACH)
	    )
        {
                bprintf(
"As you consume the &+Ggreen&* liquid, your &+Cbody&* begins to grow.\n"
"Your &+Ccarapace&* softens and &+melongates&*, and your &+Clegs &+mmerge&* into one\n"
"limb which rapidly becomes more a part of your &+Ctorso&* than a seperate\n"
"limb. Within seconds, your &+Cbody&* has changed into that of a &+Gsnake&*.\n"
                        );
		setpmetatype(mynum, META_SNAKE);
		dumpstuff(mynum, ploc(mynum));
                for (i=OBJ_META_MARKEROBJ2;i<=OBJ_META_MARKEROBJ3;i++)
                {
                        if ( (oloc(i)==LOC_LIMBO_LIMBO)
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, mynum, CARRIED_BY);
                                setpwpn(mynum, i);
                                return;
                        }
                }
                return;
        }

        if ( (b==OBJ_META_VIAL_SNAKE)
        &&   (pmetatype(mynum)==META_BEETLE)
        &&   (ploc(mynum)==LOC_META_FISHSTOMACH)
            )
        {
                bprintf(
"As you consume the &+Ggreen&* liquid, your &+Cbody&* begins to grow.\n"
"Your &+Ccarapace&* softens and &+melongates&*, and your &+Clegs &+mmerge&* into one\n"
"limb which rapidly becomes more a part of your &+Ctorso&* than a seperate\n"
"limb. Within seconds, your &+Cbody&* has changed into that of a &+Gsnake&*.\n"
"Unable to take the sudden pressure change, the &+Cfish &+mexplodes&*,\n"
"catapulting you through the &+Bair&*.\n"
                        );
                setpmetatype(mynum, META_SNAKE);
                dumpstuff(mynum, ploc(mynum));
		k = find_loc_by_id (phome (mynum));		
		trapch(k);
                for (i=OBJ_META_MARKEROBJ2;i<=OBJ_META_MARKEROBJ3;i++)
                {
                        if ( (oloc(i)==LOC_LIMBO_LIMBO)
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, mynum, CARRIED_BY);
                                setpwpn(mynum, i);
                                return;
                        }
                }
                return;
        }

        if ( (b==OBJ_META_VIAL_SNAKE)
        &&   (pmetatype(mynum)==META_BEETLE)
        &&   (ploc(mynum)==LOC_META_BIRDSTOMACH)
            )
        {
                bprintf(
"As you consume the &+Ggreen&* liquid, your &+Cbody&* begins to grow.\n"
"Your &+Ccarapace&* softens and &+melongates&*, and your &+Clegs &+mmerge&* into one\n"
"limb which rapidly becomes more a part of your &+Ctorso&* than a seperate\n"
"limb. Within seconds, your &+Cbody&* has changed into that of a &+Gsnake&*.\n"
"Unable to take the sudden pressure change, the &+Cbird &+mexplodes&*,\n"
"catapulting you through the &+Bair&*.\n"
                        );
                setpmetatype(mynum, META_SNAKE);
                dumpstuff(mynum, ploc(mynum));
                k = find_loc_by_id (phome (mynum));
                trapch(k);
                for (i=OBJ_META_MARKEROBJ2;i<=OBJ_META_MARKEROBJ3;i++)
                {
                        if ( (oloc(i)==LOC_LIMBO_LIMBO)
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, mynum, CARRIED_BY);
                                setpwpn(mynum, i);
                                return;
                        }
                }
                return;
        }

        if ( (b==OBJ_META_VIAL_SNAKE)
        &&   (pmetatype(mynum)==META_BEETLE_UNDERWATER)
            )
        {
                bprintf(
"As you consume the &+Ggreen&* liquid, your &+Cbody&* begins to grow.\n"
"Your &+Ccarapace&* softens and &+melongates&*, and your &+Clegs &+mmerge&* into one\n"
"limb which rapidly becomes more a part of your &+Ctorso&* than a seperate\n"
"limb. Within seconds, your &+Cbody&* has changed into that of a &+Gsnake&*.\n"
                        );
                setpmetatype(mynum, META_SNAKE_UNDERWATER);
                dumpstuff(mynum, ploc(mynum));
		meta_timer_beetle(False,True);
		meta_timer_snake(True,False);
                for (i=OBJ_META_MARKEROBJ2;i<=OBJ_META_MARKEROBJ3;i++)
                {
                        if ( (oloc(i)==LOC_LIMBO_LIMBO)
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, mynum, CARRIED_BY);
                                setpwpn(mynum, i);
                                return;
                        }
                }
                return;
        }

        if ( (b==OBJ_META_VIAL_SNAKE)
	&&   (pmetatype(mynum)==META_FISH_ONLAND)
	    )
        {
                bprintf(
"As you consume the &+Ggreen&* liquid, your &+Cbody&* begins to narrow.\n"
"Your &+Cfins &+melongate&*, and quickly &+mmerge&* into one long, &+Gscaly&*\n"
"limb which rapidly becomes more a part of your &+Ctorso&* than a seperate\n"
"limb. Within seconds, your &+Cbody&* has changed into that of a &+Gsnake&*.\n"
                        );
		setpmetatype(mynum, META_SNAKE);
                dumpstuff(mynum, ploc(mynum));
		meta_timer_become_fish(False,True);
                for (i=OBJ_META_MARKEROBJ2;i<=OBJ_META_MARKEROBJ3;i++)
                {
                        if ( (oloc(i)==LOC_LIMBO_LIMBO)
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, mynum, CARRIED_BY);
                                setpwpn(mynum, i);
                                return;
                        }
                }
                return;
        }

        if ( (b==OBJ_META_VIAL_FISH)
	&&   (pmetatype(mynum)==META_HUMAN)
	&&   (strcmp(pname(mynum),"Vampyra")!=0)
	   )
	{
		bprintf(
"As you consume the &+mvile &+Cliquid&*, your &+Cbody&* begins to &+mshrink&*.\n"
"Your &+Carms &+msubsume&* into your &+Cbody&*, and your &+Clegs &+mmerge&* into a\n"
"large &+Cfin&*. Other &+Cfins &+msprout&* from your &+Cbody&* and within seconds\n"
"your &+Cbody&* is that of a &+Bfish&*. You begin to &+Cs&+Bp&+Cl&+Bu&+Ct&+Bt&+Ce&+Br&*, gasping for\n"
"&+Bair&*, you need to find &+Cwater&* soon, or you will surely die!\n"
			);
		send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s greedily drinks from the &+Cvial&*, and shortly thereafter you notice\n"
"%s form changing into that of a &+Bfish&*.\n", pname(mynum), his_or_her(mynum)
			);
		setpmetatype(mynum, META_FISH_ONLAND);
                dumpstuff(mynum, ploc(mynum));
                meta_timer_become_fish(True,False);
		return;
	}

        if ( (b==OBJ_META_VIAL_FISH)
        &&   (pmetatype(mynum)==META_HUMAN)
        &&   (strcmp(pname(mynum),"Vampyra")==0)
           )
        {
                bprintf(
"You take a &+msip&* from the &+Cvial&* and transform from a &+Yhuman&* into a\n"
"beautiful &+Ygoldfish&*!\n"
			);
                send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s greedily drinks from the &+Cvial&*, and shortly thereafter you notice\n"
"%s form changing into that of a &+Bfish&*.\n", pname(mynum), his_or_her(mynum)
                        );
                setpmetatype(mynum, META_FISH_ONLAND);
                dumpstuff(mynum, ploc(mynum));
                meta_timer_become_fish(True,False);
                return;
        }


        if ( (b==OBJ_META_VIAL_FISH)
	&&   ((pmetatype(mynum)==META_FISH_ONLAND)
	||    (pmetatype(mynum)==META_FISH_UNDERWATER)
	    )
	   )
        {
                bprintf("You are already a &+Bfish&*!\n");
                return;
        }

        if ( (b==OBJ_META_VIAL_FISH)
	&&   ( (pmetatype(mynum)==META_BIRD)
	      )
	    )
        {
                bprintf(
"As you consume the &+mvile &+Cliquid&*, your &+Cbody&* begins to &+mgrow&*.\n"
"Your &+Cwings &+msubsume&* into your &+Cbody&*, and your &+Clegs &+mmerge&* into a\n"
"&+Cfin&*. More &+Cfins &+msprout&* from your &+Cbody&*, and within seconds you have\n"
"become a &+Bfish&*!\n"
                        );
		setpmetatype(mynum, META_FISH_ONLAND);
                for (i=OBJ_META_MARKEROBJ1;i<=OBJ_META_MARKEROBJ2;i++)
                {
                        if ( (oloc(i)!=LOC_LIMBO_LIMBO)
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, LOC_LIMBO_LIMBO, IN_ROOM);
                                return;
                        }
                }
                dumpstuff(mynum, ploc(mynum));
		meta_timer_become_fish(True,False);
                return;
        }

        if ( (b==OBJ_META_VIAL_FISH)
	&&   (pmetatype(mynum)==META_BEETLE)
	&&   (ploc(mynum)!=LOC_META_FISHSTOMACH)
	&&   (ploc(mynum)!=LOC_META_BIRDSTOMACH)
	    )
        {
                bprintf(
"As you consume the &+mvile &+Cliquid&*, your &+Cbody begins to &+mgrow&*.\n"
"Your &+Ccarapace&* softens and &+melongates&*, and your &+Clegs &+mmerge&* into one\n"
"&+Cfin&*. More &+Cfins &+msprout&* from your body, and in seconds you have\n"
"become a &+Bfish&*!\n"
                        );
		meta_timer_become_fish(True, False);
		setpmetatype(mynum, META_FISH_ONLAND);
                dumpstuff(mynum, ploc(mynum));
                return;
        }

        if ( (b==OBJ_META_VIAL_FISH)
        &&   (pmetatype(mynum)==META_BEETLE)
        &&   (ploc(mynum)==LOC_META_FISHSTOMACH)
            )
        {
                bprintf(
"As you consume the &+Cvile &+mliquid&*, your &+Cbody&* begins to &+mgrow&*.\n"
"Your &+Ccarapace&* softens and &+melongates&*, and your &+Clegs &+mmerge&* into one\n"
"&+Cfin&*. More &+Cfins&* &+msprout&* from your &+Cbody&*, and in seconds you have\n"
"become a fish!\n"
"Unable to withstand the sudden &+Ypressure&* change, the &+Bfish&* you\n"
"are inside explodes, catapulting you across the &+yland&*.\n"
                        );
                meta_timer_become_fish(True, False);
                setpmetatype(mynum, META_FISH_ONLAND);
                dumpstuff(mynum, ploc(mynum));
                k = find_loc_by_id (phome (mynum));
		trapch(k);
                return;
        }

        if ( (b==OBJ_META_VIAL_FISH)
        &&   (pmetatype(mynum)==META_BEETLE)
        &&   (ploc(mynum)==LOC_META_BIRDSTOMACH)
            )
        {
                bprintf(
"As you consume the &+mvile &+Cliquid&*, your &+Cbody&* begins to grow.\n"
"Your carapace softens and &+melongates&*, and your &+Clegs&* merge into one\n"
"fin. More &+Cfins&* &+msprout&* from your &+Cbody&*, and in seconds you have\n"
"become a fish!\n"
"Unable to withstand the sudden pressure change, the &+Gbird&* you\n"
"are inside explodes, catapulting you across the land.\n"
                        );
                meta_timer_become_fish(True, False);
                setpmetatype(mynum, META_FISH_ONLAND);
                dumpstuff(mynum, ploc(mynum));
                k = find_loc_by_id (phome (mynum));
                trapch(k);
                return;
        }

        if ( (b==OBJ_META_VIAL_FISH)
	&&   (pmetatype(mynum)==META_SNAKE)
	    )
        {
                bprintf(
"As you consume the vile liquid, your &+Cbody&* begins to &+mshrink&* and fatten.\n"
"Your &+Cfins&* &+msprout&* from various &+Cbody&* parts, and within seconds you\n"
"have become a fish!\n"
                        );
		setpmetatype(mynum, META_FISH_ONLAND);
                for (i=OBJ_META_MARKEROBJ2;i<=OBJ_META_MARKEROBJ3;i++)
                {
			dumpstuff(mynum, ploc(mynum));
                        if ( (oloc(i)==ploc(mynum))
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, LOC_LIMBO_LIMBO, IN_ROOM);
                        }
                }
		meta_timer_become_fish(True,False);
		dumpstuff(mynum, ploc(mynum));
                return;
        }

        if ( (b==OBJ_META_VIAL_FISH)
        &&   (pmetatype(mynum)==META_SNAKE_UNDERWATER)
            )
        {
                bprintf(
"As you consume the vile liquid, your &+Cbody&* begins to &+mshrink&* and fatten.\n"
"Your &+Cfins&* &+msprout&* from various body parts, and within seconds you\n"
"have become a &+Bfish&*!\n"
                        );
                setpmetatype(mynum, META_FISH_UNDERWATER);
		dumpstuff(mynum, ploc(mynum));
                for (i=OBJ_META_MARKEROBJ2;i<=OBJ_META_MARKEROBJ3;i++)
                {
                        if ( (oloc(i)==ploc(mynum))
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, LOC_LIMBO_LIMBO, IN_ROOM);
                        }
                }
                meta_timer_snake(False,True);
                return;
        }

	if ( (b == OBJ_META_VIAL_BEETLE)
	&&   (pmetatype(mynum)==META_HUMAN)
	    )
        {
		bprintf(
"You take a sip from the &+Gvial&* and feel your &+Cbody&* &+yhardening&*. With a\n"
"rush of air, you &+mshrink&* greatly in size and when you open your eyes\n"
"you find that you are a &+Rbeetle&*.\n"
			);
		setpmetatype(mynum, META_BEETLE);
                dumpstuff(mynum, ploc(mynum));
		return;
	  }

	if ( (b == OBJ_META_VIAL_BEETLE)
	&&   (pmetatype(mynum)==META_BIRD)
	    )
	{
		bprintf(
"You take a sip from the &+Gvial&* and feel your &+Cbody&* &+mshrink&*. You close your\n"
"eyes and flap your &+Cwings&* wildly, trying to rid yourself of the feeling\n"
"only to discover that your &+Cwings&* are now spindly &+Clegs&*. You open your eyes\n"
"to find that you are a &+Rbeetle&*.\n"
			);
		setpmetatype(mynum, META_BEETLE);
                for (i=OBJ_META_MARKEROBJ1;i<=OBJ_META_MARKEROBJ2;i++)
                {
                        if ( (oloc(i)!=LOC_LIMBO_LIMBO)
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, LOC_LIMBO_LIMBO, IN_ROOM);
                                return;
                        }
                }
                dumpstuff(mynum, ploc(mynum));
		return;
	}

	if ( (b == OBJ_META_VIAL_BEETLE)
	&&   ( (pmetatype(mynum)==META_BEETLE)
	||     (pmetatype(mynum)==META_BEETLE_UNDERWATER)
	      )
	    )
	{
		bprintf("You are already a &+Cbeetle&*!\n");
		return;
	}

	if ( (b == OBJ_META_VIAL_BEETLE)
	&&   (pmetatype(mynum)==META_FISH_ONLAND)
	    )
	{
		bprintf(
"You take a sip of the &+Gvial&* and feel your &+Cfins&* retract into your body which\n"
"starts to &+mshrink&* and harden. You close your eyes and try and banish the strange\n"
"sensation, when abruptly it stops. You open your eyes to find that you are a\n"
"&+Rbeetle&*.\n"
			);
		setpmetatype(mynum, META_BEETLE);
		dumpstuff(mynum, ploc(mynum));
		meta_timer_become_fish(False, True);
		return;
	}

	if ( (b == OBJ_META_VIAL_BEETLE)
	&&   (pmetatype(mynum)==META_SNAKE)
	    )
	{
		bprintf(
"You take a sip from the &+Gvial&* and feel a wave of &+Yenergy&* pass through you,\n"
"limbs &+msprout&* from your rapidly &+mshrinking&* &+Cbody&*, and an armoured shell &+msprouts&*\n"
"from your back, shielding you from harm. Within seconds you have transformed\n"
"into a &+Rbeetle&*.\n"
			);
		setpmetatype(mynum, META_BEETLE);
		dumpstuff(mynum, ploc(mynum));
                for (i=OBJ_META_MARKEROBJ2;i<=OBJ_META_MARKEROBJ3;i++)
                {
                        if ( (oloc(i)==ploc(mynum))
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, LOC_LIMBO_LIMBO, IN_ROOM);
                        }
                }
		return;
	}

        if ( (b == OBJ_META_VIAL_BEETLE)
        &&   (pmetatype(mynum)==META_SNAKE_UNDERWATER)
            )
        {
                bprintf(
"You take a sip from the &+Gvial&* and feel a wave of &+Yenergy&* pass through you,\n"
"limbs &+msprout&* from your rapidly &+mshrinking&* &+Cbody&*, and an armoured shell &+msprouts&*\n"
"from your back, shielding you from harm. Within seconds you have transformed\n"
"into a &+Rbeetle&*.\n"
                        );
                setpmetatype(mynum, META_BEETLE_UNDERWATER);
		dumpstuff(mynum, ploc(mynum));
                for (i=OBJ_META_MARKEROBJ2;i<=OBJ_META_MARKEROBJ3;i++)
                {
                        if ( (oloc(i)==ploc(mynum))
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, LOC_LIMBO_LIMBO, IN_ROOM);
                        }
                }
		meta_timer_beetle(True,False);
                return;
        }
	
	if ( (b == OBJ_META_VIAL_BEETLE)
	&&   (pmetatype(mynum)==META_FISH_UNDERWATER)
        &&   (ploc(mynum)!=LOC_META_META73)
	    )
	{
		bprintf(
"You take a sip from the &+Gvial&* and a wave of &+Yenergy&* shoots through you.\n"
"Your &+Cfins&* retract into your &+Cbody&* which is already &+mshrinking&*. A hard shell\n"
"forms over your &+Cbody&* and spindly &+Clegs&* grow from your underside. Within seconds\n"
"you have transformed into a &+Rbeetle&*.\n"
			);
		setpmetatype(mynum, META_BEETLE_UNDERWATER);
		dumpstuff(mynum, ploc(mynum));
		meta_timer_beetle(True, False);
		return;
	}

      	if ( (b == OBJ_META_VIAL_BIRD)
	&&   (pmetatype(mynum)==META_HUMAN)
	    )
        {
		bprintf(
"You take a sip from the &+Gvial&* and feel a sudden wave of &+Yenergy&* pass through you.\n"
"Your arms &+mshrink&* and widen and &+Cfeathers&* &+msprout&* all over your &+Cbody&*. Within\n"
"moments you have transformed into a &+Gbird&*.\n"
			);
                dumpstuff(mynum, ploc(mynum));
		setpmetatype(mynum, META_BIRD);
        	for (i=OBJ_META_MARKEROBJ1;i<=OBJ_META_MARKEROBJ2;i++)
        	{
                	if ( (oloc(i)==LOC_LIMBO_LIMBO)
			&&   (!otstbit(i, OFL_NOGET))
		   	    )
                	{
 				setoloc(i, mynum, CARRIED_BY);
				setpwpn (mynum, i);
	        		return;
                	}
        	}
		return;
        }

        if ( (b == OBJ_META_VIAL_BIRD)
	&&   (pmetatype(mynum)==META_SNAKE)
	    )
        {
		bprintf(
"You take a sip from the &+Gvial&* and feel a sudden wave of &+Yenergy&* pass through you.\n"
"Your &+Cwings&* &+msprout&* from your arms and you find yourself suddenly covered with\n"
"&+Cfeathers&*. Within moments you have transformed into a &+Gbird&*.\n"
			);
		setpmetatype(mynum, META_BIRD);
		dumpstuff(mynum, ploc(mynum));
                for (i=OBJ_META_MARKEROBJ2;i<OBJ_META_MARKEROBJ3;i++)
                {
                        if ( (oloc(i)==ploc(mynum))
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, LOC_LIMBO_LIMBO, IN_ROOM);
                        }
                }
                for (i=OBJ_META_MARKEROBJ1;i<OBJ_META_MARKEROBJ2;i++)
                {
                        if ( (oloc(i)==LOC_LIMBO_LIMBO)
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, mynum, CARRIED_BY);
                                setpwpn (mynum, i);
                                return;
                        }
                }
		return;
        }

      	if ( (b == OBJ_META_VIAL_BIRD)
	&&   (pmetatype(mynum)==META_BEETLE)
	&&   (ploc(mynum)!=LOC_META_FISHSTOMACH)
	&&   (ploc(mynum)!=LOC_META_BIRDSTOMACH)
	    )
        {
		bprintf(
"You take a sip from the &+Gvial&* and feel a sudden wave of &+Yenergy&* pass through you.\n"
"Your &+Clegs&* retract into your &+Cbody&* which &+mgrows&* rapidly, &+Cfeathers&* &+msprouting&* all\n"
"over. &+CWings&* grow from your &+Ctorso&*, and a beak appears on your face. Within\n"
"seconds you have transformed into a &+Gbird&*.\n"
			);
                dumpstuff(mynum, ploc(mynum));
		setpmetatype(mynum, META_BIRD);
                for (i=OBJ_META_MARKEROBJ1;i<=OBJ_META_MARKEROBJ2;i++)
                {
                        if ( (oloc(i)==LOC_LIMBO_LIMBO)
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, mynum, CARRIED_BY);
                                setpwpn (mynum, i);
                                return;
                        }
                }
		return;
        }

        if ( (b == OBJ_META_VIAL_BIRD)
        &&   (pmetatype(mynum)==META_BEETLE)
        &&   (ploc(mynum)==LOC_META_FISHSTOMACH)
            )
        {
                bprintf(
"You take a sip from the &+Gvial&* and feel a sudden wave of &+Yenergy&* pass through you.\n"
"Your &+Clegs&* retract into your &+Cbody&* which &+mgrows&* rapidly, &+Cfeathers&* &+msprouting&* all\n"
"over. &+CWings&* grow from your &+Ctorso&*, and a beak appears on your face. Within\n"
"seconds you have transformed into a &+Gbird&*.\n"
"Unable to cope with the sudden pressure change, the fish you were inside\n"
"explodes, catapulting you across the land.\n"

                        );
                dumpstuff(mynum, ploc(mynum));
                setpmetatype(mynum, META_BIRD);
		k = find_loc_by_id (phome (mynum));
		trapch(k);
                for (i=OBJ_META_MARKEROBJ1;i<=OBJ_META_MARKEROBJ2;i++)
                {
                        if ( (oloc(i)==LOC_LIMBO_LIMBO)
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, mynum, CARRIED_BY);
                                setpwpn (mynum, i);
                                return;
                        }
                }
		return;
	}

        if ( (b == OBJ_META_VIAL_BIRD)
        &&   (pmetatype(mynum)==META_BEETLE)
        &&   (ploc(mynum)==LOC_META_BIRDSTOMACH)
            )
        {
                bprintf(
"You take a sip from the &+Gvial&* and feel a sudden wave of &+Yenergy&* pass through you.\n"
"Your &+Clegs&* retract into your &+Cbody&* which &+mgrows&* rapidly, &+Cfeathers&* &+msprouting&* all\n"
"over. &+CWings&* grow from your &+Ctorso&*, and a beak appears on your face. Within\n"
"seconds you have transformed into a &+Gbird&*.\n"
"Unable to cope with the sudden pressure change, the &+Gbird&* you were inside\n"
"explodes, catapulting you across the land.\n"
                        );
                dumpstuff(mynum, ploc(mynum));
                setpmetatype(mynum, META_BIRD);
                k = find_loc_by_id (phome (mynum));
                trapch(k);
                for (i=OBJ_META_MARKEROBJ1;i<=OBJ_META_MARKEROBJ2;i++)
                {
                        if ( (oloc(i)==LOC_LIMBO_LIMBO)
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, mynum, CARRIED_BY);
                                setpwpn (mynum, i);
                                return;
                        }
                }
                return;
	}

        if ( (b == OBJ_META_VIAL_BIRD)
	&&   (pmetatype(mynum)==META_FISH_ONLAND)
	    )
        {
		bprintf(
"You take a sip from the &+Gvial&* and feel a sudden wave of &+Yenergy&* pass through you.\n"
"Your &+Cfins&* grow and widen and &+Cfeathers&* &+msprout&* all over your &+Cbody&*. Within\n"
"moments you have transformed into a &+Gbird&*.\n"
			);
                dumpstuff(mynum, ploc(mynum));
		meta_timer_become_fish(False, True);
		setpmetatype(mynum, META_BIRD);
                for (i=OBJ_META_MARKEROBJ1;i<=OBJ_META_MARKEROBJ2;i++)
                {
                        if ( (oloc(i)==LOC_LIMBO_LIMBO)
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, mynum, CARRIED_BY);
                                setpwpn (mynum, i);
                                return;
                        }
                }
		return;
        }

        if ( (b == OBJ_META_VIAL_BIRD)
	&&   (pmetatype(mynum)==META_FISH_UNDERWATER)
	&&   (!carries_breather(mynum))
	&&   (ploc(mynum)!=LOC_META_META73)
	    )
        {
		bprintf(
"You take a sip from the &+Gvial&* and feel a sudden wave of &+Yenergy&* pass through you.\n"
"Your &+Cfins&* &+mshrink&* and widen and &+Cfeathers&* &+msprout&* all over your &+Cbody&*. Within\n"
"moments you have transformed into a &+Gbird&*. You cough and splutter, unable to\n"
"breathe and die quickly and painfully.\n"
			);
    send_msg (DEST_ALL, MODE_QUIET, max (pvis (mynum), LVL_WIZARD), LVL_MAX,mynum,NOBODY,
"&+B[&+RForgot how to breathe&+B]\n");
		p_crapup(mynum, "&+RForgot how to breathe!\n", CRAP_SAVE);
        }
        if ( (b == OBJ_META_VIAL_BIRD)
        &&   (pmetatype(mynum)==META_SNAKE_UNDERWATER)
        &&   (!carries_breather(mynum))
            )
        {
                bprintf(
"You take a sip from the &+Gvial&* and feel a sudden wave of &+Yenergy&* pass through you\n"
"&+CWings&* &+msprout&* from your &+Cbody&*, and &+Cfeathers&* pop up all over you. Within\n"
"moments you have transformed into a &+Gbird&*. You cough and splutter, unable to\n"
"breathe and die quickly and painfully.\n"
                        );
    send_msg (DEST_ALL, MODE_QUIET, max (pvis (mynum), LVL_WIZARD), LVL_MAX,mynum,NOBODY,
"&+B[&+RForgot how to breathe&+B]\n");
                p_crapup(mynum, "&+RForgot how to breathe!\n", CRAP_SAVE);
		meta_timer_snake(False, True);
        }


        if ( (b == OBJ_META_VIAL_BIRD)
        &&   (pmetatype(mynum)==META_FISH_UNDERWATER)
        &&   (ploc(mynum)==LOC_META_META73)
            )
        {
		bprintf(
"You take a sip from the &+Gvial&* and feel a wave of &+Yenergy&* passing through you.\n"
"Your &+Cfins&* reform into &+Cwings&* and your &+Cbody&* changes form, &+Cfeathers&* &+msprouting&*\n"
"all over it. You gasp for air, unable to breathe underwater in this form and\n"
"struggle to drag yourself up through the water to the shore, coughing and\n"
"spluttering. Still, at least you're alive.\n");
		setploc(mynum, LOC_META_BESIDEWATERFALL);
		lookin(ploc(mynum), 0);
		setpmetatype(mynum, META_BIRD);
                for (i=OBJ_META_MARKEROBJ1;i<=OBJ_META_MARKEROBJ2;i++)
                {
                        if ( (oloc(i)==LOC_LIMBO_LIMBO)
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, mynum, CARRIED_BY);
                                setpwpn (mynum, i);
                                return;
                        }
                }
		return;
	}

        if ( (b == OBJ_META_VIAL_BIRD)
        &&   (pmetatype(mynum)==META_SNAKE_UNDERWATER)
        &&   (ploc(mynum)==LOC_META_META73)
            )
        {
                bprintf(
"You take a sip from the &+Gvial&* and feel a wave of &+Yenergy&* passing through you.\n"
"Your begin to grow &+Cwings&* and your &+Cbody&* changes form, &+Cfeathers&* &+msprouting&*\n"
"all over it. You gasp for air, unable to breathe underwater in this form and\n"
"struggle to drag yourself up through the water to the shore, coughing and\n"
"spluttering. Still, at least you're alive.\n");
                setploc(mynum, LOC_META_BESIDEWATERFALL);
                lookin(ploc(mynum), 0);
		dumpstuff(mynum, ploc(mynum));
                for (i=OBJ_META_MARKEROBJ2;i<=OBJ_META_MARKEROBJ3;i++)
                {
                        if ( (oloc(i)==ploc(mynum))
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, LOC_LIMBO_LIMBO, IN_ROOM);
                        }
                }
                setpmetatype(mynum, META_BIRD);
                for (i=OBJ_META_MARKEROBJ1;i<=OBJ_META_MARKEROBJ2;i++)
                {
                        if ( (oloc(i)==LOC_LIMBO_LIMBO)
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, mynum, CARRIED_BY);
                                setpwpn (mynum, i);
				meta_timer_snake(False, True);
				return;                        }
                }
		meta_timer_snake(False, True);
                return;
        }

	if ( (b == OBJ_META_VIAL_BIRD)
	&&   ( (pmetatype(mynum)==META_BIRD)
	      )
	    )
	{
		bprintf("You are already a &+Cbird&*!\n");
		return;
	}

        if ( (b == OBJ_META_VIAL_SNAKE)
	&&   (pmetatype(mynum)==META_FISH_UNDERWATER)
	&&   (!carries_breather(mynum))
	    )
        {
		bprintf(
"You take a sip from the &+Gvial&* and feel a sudden wave of &+Yenergy&* pass through you.\n"
"Your &+Cfins&* &+mshrink&* into your &+Cbody&* which &+melongates&* considerably. Within moments\n"
"you have transformed into a snake.\n"
			);
		setpmetatype(mynum, META_SNAKE_UNDERWATER);
		meta_timer_snake(True,False);
		return;
        }
        if ( (b == OBJ_META_VIAL_BEETLE)
	&&   (pmetatype(mynum)==META_FISH_UNDERWATER)
	&&   (ploc(mynum)==LOC_META_META73)
	    )
        {
		bprintf(
"You take a sip from the &+Gvial&* and feel a sudden wave of &+Yenergy&* pass through you.\n"
"Your &+Cfins&* &+mshrink&* into your &+Cbody&* which &+mshrinks&* considerably too. Within moments\n"
"you have transformed into a &+Rbeetle&*. Suddenly lacking the strength to fight\n"
"against the current which you had as a fish, you are swept down the waterfall\n"
"and land on a rock at the bottom of the waterfall.\n"
			);
		setploc(mynum, LOC_META_BEETLEWATERFALL);
		setpmetatype(mynum, META_BEETLE);
		lookin(ploc(mynum), 0);
        }

	if ( (b == OBJ_META_VIAL_SNAKE)
	&&   (pmetatype(mynum)==META_BEETLE_UNDERWATER)
	    )
	{
		bprintf(
"You sip the &+Cvial&* and immediately your carapace begins to soften. Your\n"
"whole &+Cbody&* &+melongates&* and your limbs subsume into your &+Cbody&*. In short order\n"
"you have reformed into a snake.\n"
			);
		meta_timer_beetle(False, True);
		setpmetatype(mynum, META_SNAKE_UNDERWATER);
		meta_timer_snake(True, False);
		dumpstuff(mynum, ploc(mynum));
	}

	if ( (b == OBJ_META_VIAL_BIRD)
	&&   (pmetatype(mynum)==META_BEETLE_UNDERWATER)
	    )
	{
		bprintf(
"You sip the &+Cvial&* and immediately your carapace begins to soften. &+CWings&*\n"
"&+msprout&* from your &+Cbody&*, and &+Cfeathers&* &+msprout&* all over you. In short order\n"
"you have reformed into a &+Gbird&*. You cough and splutter, unable to breathe\n"
"and die quickly and painfully.\n"
			);
    send_msg (DEST_ALL, MODE_QUIET, max (pvis (mynum), LVL_WIZARD), LVL_MAX,mynum,NOBODY,
"&+B[&+RForgot how to breathe&+B]\n");
		p_crapup(mynum,
"Forgot how to breathe!\n",CRAP_SAVE
				);
	}

	if ( (b == OBJ_META_VIAL_FISH)
	&&   (pmetatype(mynum)==META_BEETLE_UNDERWATER)
	    )
	{
		bprintf(
"You sip the &+Cvial&* and immediately your carapace begins to soften. Your\n"
"whole &+Cbody&* rapidly expands and &+Cfins&* grow from your &+Cbody&*. In short order\n"
"you have reformed into a fish.\n"
			);
		setpmetatype(mynum, META_FISH_UNDERWATER);
		meta_timer_beetle(False, True);
		dumpstuff(mynum, ploc(mynum));
	}


}

Boolean meta_move(int n)
{
int newch, drnum, droff;
int i;
int j;
int obj;
char mood[14];

static char *exittxt[] =
{
  "north", "east", "south", "west", "up", "down",
  "n", "e", "s", "w", "u", "d", 0
};

static char *arrivetxt[] =
{
"the &+Gsouth&*",
"the &+Gwest&*",
"the &+Gnorth&*",
"the &+Geast&*",
"&+Gabove&*",
"&+Gbelow&*"
};

	if (mynum > max_players)
	{
		return False;
	}
	if ( (n == EX_DOWN)
	&&   (ploc(mynum)==LOC_META_META8)
	&&   (pmetatype(mynum)==META_FISH_ONLAND)
	    )
	{
                setpmetatype(mynum, META_FISH_UNDERWATER);
                bprintf(
"You make a colossal &+Cleap&* and land with a splash in the &+Bpool&*!\n"
                        );
                send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s &+Cleaps&* into the &+Bpool&*.\n",pname(mynum)
                        );
                send_msg(LOC_META_META20, 0, pvis(mynum), LVL_MAX, mynum,
NOBODY,
"%s &+Centers&* from &+Gabove&* with a huge &+Bsplash&*!\n",pname(mynum)
                        );
                setploc(mynum, LOC_META_META20);
                lookin(ploc(mynum), 0);
                meta_timer_become_fish(False,True);
                for (obj = 0; obj < pnumobs (mynum); obj++)
                {
                        if ( (iscarrby (pobj_nr (obj, mynum), mynum))
                        &&   (otstbit(pobj_nr (obj, mynum), OFL_LIT))
                            )
                        {
                                bprintf(
"The &+Bwater&* extinguishes your &+C%s&*\n", oname(pobj_nr(obj, mynum))
                                                );
                                setobjstate(pobj_nr(obj, mynum), 1);
                                oclrbit(pobj_nr(obj, mynum), OFL_LIT);
                        }
                        if ( (iscarrby (pobj_nr (obj, mynum), mynum))
                        &&   (pobj_nr(obj,mynum)==OBJ_META_BOTTLE)
                        &&   ( (iscontin(OBJ_META_RED_FLAME, OBJ_META_BOTTLE))
                        ||     (iscontin(OBJ_META_BLUE_FLAME, OBJ_META_BOTTLE))
                              )
                            )
                        {
                                bprintf(
"The &+Bwater&* extinguishes the &+Cflame&* in the &+Cbottle&*.\n"
                                        );
                                if (iscontin(OBJ_META_RED_FLAME, OBJ_META_BOTTLE))
                                { destroy(OBJ_META_RED_FLAME); }
                                if (iscontin(OBJ_META_BLUE_FLAME, OBJ_META_BOTTLE))
                                { destroy(OBJ_META_BLUE_FLAME); }
                        }

                }
                return True;
        }
	
	if ( (n == EX_SOUTH)
	&&   (ploc(mynum)==LOC_META_META1)
	&&   (pmetatype(mynum)==META_HUMAN)
	&&   (pstr(MOB_META_CHIMERA+max_players)==-1)
	&&   (!etstflg(mynum, EFL_WEREWOLF))
	    )
	{
                bprintf("You go &+Gsouth&*\n");
		trapch(LOC_META_META72);
                bprintf(
"The &+Ctoadstool &+Mring&* shimmers and sparkles, and in a flash of &+Ylight&* a &+Mf&+Ca&+Mi&+Cr&+My&*\n"
"appears in &+Gfront&* of you.\n"
"'&+C%s&+M! You have destroyed the Chimera and freed these lands from his\n"
"&+Mevil rule! Take &+Mthis gift as a sign of our eternal thanks.&*'\n"
"She waves her &+Ywand&* and the air around you blurs, imbuing you with the power\n"
"of a &+Lwerewolf&*!\n"
"'&+MGo now, &+C%s&+M, we shall never forget you.&*'\n"
"She waves her &+Ywand&* one last time, and the exit north blurs, a new one appearing\n"
"to the west.\n", pname(mynum), pname(mynum));
		dumpstuff(mynum, ploc(mynum));
		setoloc(OBJ_META_STAFF, mynum, CARRIED_BY);
         /*      set_quest(mynum,Q_META); */
		setpwpn(mynum, -1);
		setploc(mynum, LOC_META_META75);
		esetflg(mynum, EFL_WEREWOLF);
		return True;
	}

        if ( (n == EX_SOUTH)
        &&   (ploc(mynum)==LOC_META_META1)
        &&   (pmetatype(mynum)!=META_HUMAN)
        &&   (pstr(MOB_META_CHIMERA+max_players)==-1)
        &&   (!etstflg(mynum, EFL_WEREWOLF))
            )
        {
                bprintf("You go &+Gsouth&*\n");
                trapch(LOC_META_META72);
                bprintf(
"The &+Ctoadstool &+Mring&* shimmers and sparkles, and in a flash of &+Ylight&* a &+Mf&+Ca&+Mi&+Cr&+My&*\n"
"appears in &+Gfront&* of you.\n"
"'&+C%s&+M! You have destroyed the Chimera and freed these lands from his\n"
"&+Mevil rule! Take this gift as a sign of our eternal thanks.&*'\n"
"She waves her &+Ywand&* and the air around you blurs, imbuing you with the power\n"
"of a &+Lwerewolf&*!\n"
"'&+MGo now, &+C%s&+M, we shall never forget you.&*'\n"
"She waves her &+Ywand&* one last time, and the exit north blurs, a new one appearing\n"
"to the west, in the same instant your body reforming into that of a human.\n", pname(mynum), pname(mynum));
                dumpstuff(mynum, ploc(mynum));
                setoloc(OBJ_META_STAFF, mynum, CARRIED_BY);
                setpwpn(mynum, -1);  
                setploc(mynum, LOC_META_META75);
/*                set_quest(mynum,Q_META); */
                esetflg(mynum, EFL_WEREWOLF);
		setpmetatype(mynum, META_HUMAN);
                return True;
        }
        if ( (n == EX_SOUTH)
        &&   (pmetatype(mynum)!=META_HUMAN)
        &&   (ploc(mynum)==LOC_META_META1)
        &&   (pstr(MOB_META_CHIMERA+max_players)==-1)
        &&   (etstflg(mynum, EFL_WEREWOLF))
            )
        {
                bprintf("You go &+Gsouth&*\n");
                trapch(LOC_META_META72);
                bprintf(
"The &+Ctoadstool &+Mring&* shimmers and sparkles, and in a flash of &+Ylight&* a &+Mf&+Ca&+Mi&+Cr&+My&*\n"
"appears in &+Gfront&* of you.\n"
"'&+C%s&+M! You have destroyed the Chimera and freed these lands from his evil\n"
"&+Mrule! Keep the staff as a sign of our thanks! Go now, &+C%s&+M, we shall\n"
"&+Mnever forget you.&*'\n"
"She waves her &+Ywand&* one last time, and the exit north blurs, a new one appearing\n"
"to the west, in the same instant your body reforming into that of a human.\n", pname(mynum), pname(mynum)
				);
                dumpstuff(mynum, ploc(mynum));
                setoloc(OBJ_META_STAFF, mynum, CARRIED_BY);
/*                set_quest(mynum,Q_META); */
                setploc(mynum, LOC_META_META75);
                setpwpn(mynum, -1);
                setpmetatype(mynum, META_HUMAN);
                return True;
        }
        if ( (n == EX_SOUTH)
        &&   (pmetatype(mynum)==META_HUMAN)
        &&   (ploc(mynum)==LOC_META_META1)
        &&   (pstr(MOB_META_CHIMERA+max_players)==-1)
        &&   (etstflg(mynum, EFL_WEREWOLF))
            )
        {
                bprintf("You go &+Gsouth&*\n");
                trapch(LOC_META_META72);
                bprintf(
"The &+Ctoadstool &+Mring&* shimmers and sparkles, and in a flash of &+Ylight&* a &+Mf&+Ca&+Mi&+Cr&+My&*\n"
"appears in &+Gfront&* of you.\n"
"'&+C%s&+M! You have destroyed the Chimera and freed these lands from his evil\n"
"&+Mrule! Keep the staff as a sign of our thanks! Go now, &+C%s&+M, we shall\n"
"&+Mnever forget you.&*'\n"
"She waves her &+Ywand&* one last time, and the exit north blurs, a new one appearing\n"
"to the west.\n", pname(mynum), pname(mynum)
			);
                dumpstuff(mynum, ploc(mynum));
                setoloc(OBJ_META_STAFF, mynum, CARRIED_BY);
/*                set_quest(mynum,Q_META); */
                setpwpn(mynum, -1);
                setploc(mynum, LOC_META_META75);
                return True;
        }

	if ( (n == EX_SOUTH)
	&&   (ploc(mynum)==LOC_META_META1)
	&&   (oloc(OBJ_META_STAFF)==LOC_LIMBO_LIMBO)
	&&   (oloc(OBJ_META_LIQUID_HUMAN)!=LOC_LIMBO_LIMBO)
	    )
	{
		bprintf("You go &+Gsouth&*\n");
		trapch(LOC_META_META72);
		bprintf(
"The &+Ctoadstool &+Mring&* shimmers and sparkles, and in a flash of &+Ylight&* a &+Mf&+Ca&+Mi&+Cr&+My&*\n"
"appears in &+Gfront&* of you.\n"
"'&+C%s&+M! So the legends were true and you have finally come to rid this\n"
"&+Mland &+Mof the scourge of the Chimera. Take this &+Cstaff&+M, for only it can\n"
"&+Mhope to &+Mpenetrate the evil of the Chimera and slay it once and for all!&*'\n"
"She hands you a large &+Cstaff&*, &+Rkisses&* you lightly on the &+Mcheek&* and\n"
"vanishes as quickly as she arrived.\n", pname(mynum)
				);
		setoloc(OBJ_META_STAFF, mynum, CARRIED_BY);
		return True;
	}


        if ( (n == EX_NORTH)
        &&   (ploc(mynum)==LOC_META_META27)
	&&   (pmetatype(mynum)==META_FISH_UNDERWATER) 
        &&   (ploc(MOB_META_FISH_KING+max_players)==LOC_LIMBO_LIMBO)
        &&   (state(OBJ_META_DAM)==0)
            )
        {
                bprintf("You &+Bswim&* north&*\n");
                setploc(MOB_META_FISH_KING+max_players, LOC_META_META30);         
    		trapch(LOC_META_META30);
                bprintf(
"Noticing that you are a stranger, the &+Cking&* swims up to you. '&+MAre you to\n"
"&+Mbe our benefactor in this dark time? Legend has foretold of a hero who will\n"
"&+Msave us from the beavers and their wretched dam, could it be that you are the\n"
"&+Mhero so long ago prophesised?&*'\n"
				);
		return True;
	}


	if ( (n == EX_NORTH)
	&&   (ploc(mynum)==LOC_META_META27)
	&&   (oloc(OBJ_META_DIAMOND)==LOC_LIMBO_LIMBO)
	&&   (state(OBJ_META_DAM)==1)
	    )
	{
		bprintf("You &+Bswim&* north&*\n");
		setploc(MOB_META_FISH_KING+max_players, LOC_LIMBO_LIMBO);
		setploc(MOB_META_FISH_KING2+max_players, LOC_META_META30);
		trapch(LOC_META_META30);
		bprintf(
"As you enter the &+Ythrone room&*, a loud cheer goes up, hundreds of fish mill\n"
"around you, thanking you for destroying the Beaver's dam.\n"
"The Fish &+CKing&* swims up to you. '&+MThank you for this great service,&*' he says,\n"
"'&+MYou shall forever be remembered by my people, now please take this trinket so\n"
"you &+Mshall not forget us either.\n"
"He hands you a &+Clarge&* multifaceted &+Cdiamond&* and swims away, thanking you all\n"
"the while.\n"
				);
		setoloc(OBJ_META_DIAMOND, mynum, CARRIED_BY);
		return True;
	}


	if ( (n == EX_EAST)
	&&   (ploc(mynum) == LOC_META_META68)
	&&   (pmetatype(mynum)==META_BEETLE)
	    )
	{
		bprintf(
"You &+Cscuttle&* under the door.\n"
				);
		trapch(LOC_META_META70);
            send_msg(LOC_META_META68, 0, pvis(mynum), LVL_MAX, NOBODY,
NOBODY, "%s &+Cscuttles&* under the door&*.\n", pname(mynum)
                        );
		return True;
	}

	if ( (n == EX_WEST)
	&&   (ploc(mynum) == LOC_META_META70)
	&&   (pmetatype(mynum)==META_BEETLE)
	    )
	{
		bprintf(
"You &+Cscuttle&* under the door.\n"
				);
		trapch(LOC_META_META68);
            send_msg(LOC_META_META70, 0, pvis(mynum), LVL_MAX, NOBODY,
NOBODY, "%s &+Cscuttles&* under the door&*.\n", pname(mynum)
                        );
		return True;
	}
	if ( (n == EX_WEST)
	&&   (ploc(mynum) == LOC_META_META69)
	&&   (pmetatype(mynum)==META_BEETLE)
        &&   (ploc(MOB_META_HASEEM+max_players)==LOC_META_META71)
	    )
	{
		bprintf(
"You &+Cscuttle&* under the door.\n"
				);
		trapch(LOC_META_META71);
            send_msg(LOC_META_META69, 0, pvis(mynum), LVL_MAX, NOBODY,
NOBODY, "%s &+Cscuttles&* under the door&*.\n", pname(mynum)
                        );
		bprintf(
"Looking up in alarm, the Nal Haseem beetles quickly rush over and dismember you,\n"
"clearly intruders are not welcome.\n"
				);
		p_crapup(mynum, 
"&+RTorn to shreds by angry beetles!\n", CRAP_SAVE
					);	
		send_msg (DEST_ALL, MODE_QUIET, max (pvis (mynum), LVL_WIZARD), LVL_MAX,mynum,NOBODY,
"&+B[&+RTorn to Shreds&+B]\n");
		return True;
	}

	if ( (n == EX_WEST)
	&&   (ploc(mynum) == LOC_META_META69)
	&&   (pmetatype(mynum)==META_SNAKE)
	&&   (ploc(MOB_META_HASEEM+max_players)==LOC_META_META71)
	    )
	{
		bprintf(
"You &+Gslither&* under the door.\n"
				);
		trapch(LOC_META_META71);
            send_msg(LOC_META_META69, 0, pvis(mynum), LVL_MAX, NOBODY,
NOBODY, "%s &+Gslithers&* under the door&*.\n", pname(mynum)
                        );
		bprintf(
"Looking up in alarm at your intrusion, the Nal Haseem beetles shriek in terror\n"
"and bolt for the door, scrambling under it and running away as fast as their\n"
"little legs can carry them.\n"
				);
		setploc(MOB_META_HASEEM+max_players, LOC_LIMBO_LIMBO);
		return True;
	}
	if ( (n == EX_WEST)
	&&   (ploc(mynum) == LOC_META_META70)
        &&   (pmetatype(mynum)!=META_BEETLE)
        &&   (plev(mynum)<LVL_WIZARD)
	    )
	{
                bprintf(
"You can't go that way.\n"
                        );
                return True;
        }

        if ( (n == EX_WEST)
        &&   (ploc(mynum) == LOC_META_META69)
        &&   (pmetatype(mynum)!=META_BEETLE)
        &&   (pmetatype(mynum)!=META_SNAKE)
        &&   (plev(mynum)<LVL_WIZARD)
            )
        {
                bprintf(
"You can't go that way.\n"
                        );
                return True;
        }


        if ( (n == EX_EAST)
        &&   (ploc(mynum) == LOC_META_META71)
        &&   (pmetatype(mynum)!=META_BEETLE)
        &&   (pmetatype(mynum)!=META_SNAKE)
        &&   (plev(mynum)<LVL_WIZARD)
            )
        {
                bprintf(
"You can't go that way.\n"
                        );
                return True;
	}

        if ( (n == EX_EAST)
        &&   (ploc(mynum) == LOC_META_META68)
        &&   (pmetatype(mynum)!=META_BEETLE)
        &&   (plev(mynum)<LVL_WIZARD)
            )
        {
                bprintf(
"You can't go that way.\n"
                        );
                return True;
        }


	if ( (n == EX_DOWN)
	&&   ( (ploc(mynum)==LOC_META_META59)
	||     (ploc(mynum)==LOC_META_META66)
	      )
	&&   (pmetatype(mynum)!=META_BEETLE)
	&&   (plev(mynum)<LVL_WIZARD)
	    )
	{
		bprintf(
"You can't go that way.\n"
			);
		return True;
	}

        if ( (n == EX_DOWN)
        &&   (ploc(mynum) == LOC_META_META6)
        &&   (pmetatype(mynum)!=META_BEETLE)
        &&   (plev(mynum)<LVL_WIZARD)
            )
        {
                bprintf(
"You can't go that way.\n"
                        );
                return True;
        }

        if ( (n == EX_WEST)
        &&   (ploc(mynum) == LOC_META_META71)
        &&   (pmetatype(mynum)!=META_BEETLE) && (pmetatype(mynum)!=META_SNAKE)
        &&   (plev(mynum)<LVL_WIZARD)
            )
        {
                bprintf(
"You can't go that way.\n"
                        );
                return True;
        }


        if ( (n == EX_UP)
        &&   (ploc(mynum) == LOC_META_META35)
        &&   (pmetatype(mynum)!=META_BEETLE) && (pmetatype(mynum)!=META_SNAKE)
        &&   (plev(mynum)<LVL_WIZARD)
            )
        {
                bprintf(
"You can't go that way.\n"
                        );
                return True;
        }

	if ( (n == EX_DOWN)
	&&   (ploc(mynum) == LOC_META_META1)
	&&   (pmetatype(mynum)!=META_BEETLE) && (pmetatype(mynum)!=META_SNAKE)
	&&   (plev(mynum)<LVL_WIZARD)
	    )
	{
		bprintf(
"You can't go that way.\n"
			);
		return True;
	}

        if ( (n == EX_UP)
        &&   (ploc(mynum) == LOC_META_META35)
        &&   (pmetatype(mynum)!=META_BEETLE) && (pmetatype(mynum)!=META_SNAKE)
	&&   (plev(mynum)<LVL_WIZARD) 
           )
        {
                bprintf(
"You can't go that way.\n"
                        );
                return True;
        }


	if ( (n == EX_DOWN)
	&&   (pmetatype(mynum)!=META_BEETLE)
	&&   (ploc(mynum)==LOC_META_META66)
	    )
	{
		bprintf("You can't go that way.\n");
		return True;
	}

	if ( (n == EX_DOWN)
	&&   (ploc(mynum)==LOC_META_META66)
	&&   (pmetatype(mynum)==META_BEETLE)
	&&   (!iscarrby(OBJ_META_DUNG_STOLEN,mynum))
	    )
	{
		bprintf("You &+Cscuttle&* down.\n");
		trapch(LOC_META_META64);
		for (i=0;i<numobs;i++)
		{
			if ( (whocarries(i)==mynum)
			&&   (otstbit(i, OFL_CONTAINER))
		  	    )
			{
				if (iscontin(OBJ_META_DUNG_STOLEN, i))
				{
					bprintf(
"Upon entering the chamber, you are met by curious looks. Beetles run over to\n"
"you, sniffing at your equipment.\n"
"'&+MHe has returned the holy dung! Praise be to the stranger&*' shouts a voice.\n"
"The king of the beetles waddles up to you, holding out a small piece of dung.\n"
"'&+MPlease take this as a token of my thanks,&*' he hands you the dung, '&+Mit\n"
"&+Mmay not be much, but it has been in my family for generations, and will bring\n"
"&+Myou good fortune. Again, thank you kind stranger, thank you from the bottom of\n"
"&+Mour hearts.&*'\n"
"He removes the &+Cdung&* you retrieved from your &+C%s&*.\n", oname(i)
						);

			                setoloc(OBJ_META_DUNG_STOLEN, LOC_META_META64, IN_ROOM);
           			    	osetbit(OBJ_META_DUNG_STOLEN, OFL_NOGET);
               			 	setobjstate(OBJ_META_DUNG_STOLEN, 1);
                			setoloc(OBJ_META_DUNG_OLD, mynum, CARRIED_BY);
					return True;
				}
			}
		}
					
                return True;
        }

	if ( (n == EX_DOWN)
	&&   (ploc(mynum)==LOC_META_META66)
	&&   (pmetatype(mynum)==META_BEETLE)
	&&   (iscarrby(OBJ_META_DUNG_STOLEN,mynum))
	    )
	{
		bprintf("You &+Cscuttle&* down.\n");
		trapch(LOC_META_META64);
		bprintf(
"Upon entering the chamber, you are met by startled gasps.\n"
"'&+MHe has returned the holy dung! Praise be to the stranger&*' shouts a voice.\n"
"The king of the beetles waddles up to you, holding out a small piece of dung.\n"
"'&+MPlease take this as a token of my thanks,&*' he hands you the dung, '&+Mit\n"
"&+Mmay not be much, but it has been in my family for generations, and will bring\n"
"&+Myou good fortune. Again, thank you kind stranger, thank you from the bottom of\n"
"&+Mour hearts.&*'\n"
				);
		setoloc(OBJ_META_DUNG_STOLEN, LOC_META_META64, IN_ROOM);
		osetbit(OBJ_META_DUNG_STOLEN, OFL_NOGET);
		setobjstate(OBJ_META_DUNG_STOLEN, 1);
		setoloc(OBJ_META_DUNG_OLD, mynum, CARRIED_BY);
		return True;
	}

        if ( (n==EX_UP)
        &&   (ploc(mynum)==LOC_META_META60)
        &&   (pmetatype(mynum)==META_BEETLE)
            )
        {
                bprintf("You scuttle &+Gup&* the &+Chole&*.\n");
                setploc(mynum, LOC_META_META59);
                lookin(ploc(mynum), 1);
                send_msg(LOC_META_META60, 0, pvis(mynum), LVL_MAX, NOBODY,
NOBODY, "%s &+Gscuttles&* up&*.\n", pname(mynum)
                        );
                send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s scuttles in from &+Gbelow&*.\n", pname(mynum)
                        );
                return True;
        }

	if ( (n==EX_DOWN)
	&&   (ploc(mynum)==LOC_META_META59)
	&&   (pmetatype(mynum)==META_BEETLE)
	    )
	{
                bprintf("You scuttle &+Gdown&* the &+Chole&*.\n");
                setploc(mynum, LOC_META_META60);
                lookin(ploc(mynum), 1);
                send_msg(LOC_META_META59, 0, pvis(mynum), LVL_MAX, NOBODY,
NOBODY, "%s &+Gscuttles&* down&*.\n", pname(mynum)
                        );
                send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s scuttles in from &+Gabove&*.\n", pname(mynum)
                        );
		return True;
	}

	if ( (n==EX_SOUTH)
	&&   (ploc(mynum)==LOC_META_META53)
	&&   (state(OBJ_META_DOCK)==1)
	    )
	{
		bprintf(
"You make to &+Mleave&* the dock, but a large &+Chawk&* flies down, blocking your path.\n"
"You will not be allowed to leave.\n"
			);
		return True;
	}

	if ( (n==EX_WEST)
	&&   ( (pmetatype(mynum)==META_BEETLE_UNDERWATER)
	||     (pmetatype(mynum)==META_SNAKE_UNDERWATER)
	      )
	&&   (ploc(mynum)==LOC_META_META20)
	    )
	{
		bprintf("You are &+Runable&* to &+Bswim&* against such a fierce current in this &+Cform&*.\n");
		return True;
	}

	if ( (n==EX_UP)
	&&   (ploc(mynum)==LOC_META_META20)
	&&   (pmetatype(mynum)==META_SNAKE_UNDERWATER)
	    )
	{
		bprintf("You &+Gslither&* up onto the &+Cbank&*.\n");
		send_msg(LOC_META_META8, MODE_NOBLIND, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s &+Gslithers&* in from &+Gbelow&*.\n", pname(mynum)
			);
		send_msg(ploc(mynum), MODE_NOBLIND, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s &+Gslithers &*up onto the &+Cbank&*.\n", pname(mynum)
			);
		trapch(LOC_META_META8);
		meta_timer_snake(False, True);
		setpmetatype(mynum, META_SNAKE);
		return True;
	}

        if ( (n==EX_UP)
        &&   (ploc(mynum)==LOC_META_META20)
        &&   (pmetatype(mynum)==META_BEETLE_UNDERWATER)
            )
        {
                bprintf("You &+Mscuttle&+G up&* onto the &+Cbank&*.\n");
                send_msg(LOC_META_META8, MODE_NOBLIND, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s &+Mscuttles&* in from &+Gbelow&*.\n", pname(mynum)
                        );
                send_msg(ploc(mynum), MODE_NOBLIND, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s &+Mscuttles &+Gup&* onto the &+Cbank&*.\n", pname(mynum)
                        );
                trapch(LOC_META_META8);
                meta_timer_beetle(False, True);
                setpmetatype(mynum, META_BEETLE);
                return True;
        }

	if ( (n==EX_DOWN)
	&&   (ploc(mynum)==LOC_META_META8)
	&&   (pmetatype(mynum)==META_BEETLE)
	    )
	{
                    setpmetatype(mynum, META_BEETLE_UNDERWATER);
		bprintf(
"You &+Mscuttle &+Gdown&* into the &+Bwater&* which takes hold of you and drags you under,\n"
"you estimate that you have about &+Rtwo minutes&* until you &+Rdrown&* under here,\n"
"is assuming that you can avoid being &+Ceaten&* by a &+Bfish&* in the meantime!\n"
			);
		send_msg(ploc(mynum), MODE_NOBLIND, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s &+Mscuttles&+G down&* into the &+Bwater&*.\n", pname(mynum)
			);
		send_msg(LOC_META_META20, MODE_NOBLIND, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s &+Mscuttles&* in from &+Gabove&*.\n", pname(mynum)
			);
		trapch(LOC_META_META20);
		meta_timer_beetle(True,False);
		for (obj = 0; obj < pnumobs (mynum); obj++)
		{
			if ( (iscarrby (pobj_nr (obj, mynum), mynum))
			&&   (otstbit(pobj_nr (obj, mynum), OFL_LIT))
			    )
			{
				bprintf(
"The &+Bwater&* extinguishes your &+C%s&*.\n", oname(pobj_nr(obj, mynum))
						);
				setobjstate(pobj_nr(obj, mynum), 1);
				oclrbit(pobj_nr(obj, mynum), OFL_LIT);
			}
			if ( (iscarrby (pobj_nr (obj, mynum), mynum))
			&&   (pobj_nr(obj,mynum)==OBJ_META_BOTTLE)
	   		&&   ( (iscontin(OBJ_META_RED_FLAME, OBJ_META_BOTTLE))
			||     (iscontin(OBJ_META_BLUE_FLAME, OBJ_META_BOTTLE))
	 		      )
			    )
			{
				bprintf(
"The &+Bwater&* extinguishes the &+Cflame&* in the &+Cbottle&*.\n"
					);
				if (iscontin(OBJ_META_RED_FLAME, OBJ_META_BOTTLE))
				{ destroy(OBJ_META_RED_FLAME); }
				if (iscontin(OBJ_META_BLUE_FLAME, OBJ_META_BOTTLE))
				{ destroy(OBJ_META_BLUE_FLAME); }
			}
		}
		return True;
	}

        if ( (n==EX_DOWN)
        &&   (ploc(mynum)==LOC_META_META8)
        &&   (pmetatype(mynum)==META_SNAKE)
            )
        {
                    setpmetatype(mynum, META_SNAKE_UNDERWATER);
                bprintf(
"You &+Mslither &+Gdown&* into the &+Bwater&* which takes hold of you and drags you\n"
"under. You estimate that you have about &+Rthree minutes&* until you &+Rdrown&* under\n"
"here.\n"
                        );
                send_msg(ploc(mynum), MODE_NOBLIND, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s &+Mslithers&+G down&* into the &+Bwater&*.\n", pname(mynum)
                        );
                send_msg(LOC_META_META20, MODE_NOBLIND, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s &+Mslithers&* in from &+Gabove&*.\n", pname(mynum)
                        );
                setploc(mynum,LOC_META_META20);
		lookin(ploc(mynum), 0);
                meta_timer_snake(True,False);
		for (obj = 0; obj < pnumobs (mynum); obj++)
		{
			if ( (iscarrby (pobj_nr (obj, mynum), mynum))
			&&   (otstbit(pobj_nr (obj, mynum), OFL_LIT))
			    )
			{
				bprintf(
"The &+Bwater&* extinguishes your %&+Cs&*\n", oname(pobj_nr(obj, mynum))
						);
				setobjstate(pobj_nr(obj, mynum), 1);
				oclrbit(pobj_nr(obj, mynum), OFL_LIT);
			}
                        if ( (iscarrby (pobj_nr (obj, mynum), mynum))
                        &&   (pobj_nr(obj,mynum)==OBJ_META_BOTTLE)
                        &&   ( (iscontin(OBJ_META_RED_FLAME, OBJ_META_BOTTLE))
                        ||     (iscontin(OBJ_META_BLUE_FLAME, OBJ_META_BOTTLE))
                              )
                            )
                        {
                                bprintf(
"The &+Bwater&* extinguishes the &+Cflame&* in the &+Cbottle&*.\n"
                                        );
                                if (iscontin(OBJ_META_RED_FLAME, OBJ_META_BOTTLE))
                                { destroy(OBJ_META_RED_FLAME); }
                                if (iscontin(OBJ_META_BLUE_FLAME, OBJ_META_BOTTLE))
                                { destroy(OBJ_META_BLUE_FLAME); }
                        }

		}
                return True;
        }

        if ( (n==EX_NORTH)
        &&   (ploc(mynum)==LOC_META_META45)
	&&   (pmetatype(mynum)==META_SNAKE)
	&&   (pnumobs(mynum)==1)
	&&   (state(OBJ_META_HOLE_VOLES)==1)
	    )
	{
                bprintf("You &+Gslither&* north.\n");
                trapch(LOC_META_META46);
                send_msg(LOC_META_META45, 0, pvis(mynum), LVL_MAX, mynum,NOBODY,
"%s &+Gslithers&* north.\n",pname(mynum)
                        );
                bprintf(
"As you enter the castle, the King looks you up and down.\n"
"'&+MMy young friend,&*' he says, '&+Myou have freed us from the threat of the\n"
"&+Mvoles! Take this as a token of our gratitude, it is our most prized of\n"
"&+Mposessions, and an integral part of the potion you will need to return yourself\n"
"&+Mto your human form.&*' He winks, '&+MYes, I know who you are, and wish you\n"
"&+Mluck on your quest, may the gods be with you on your journey.&*'\n"
"\n"
"He hands you a &+Ygolden&* scale.\n"
			);
		setoloc(OBJ_META_SCALE, mynum, CARRIED_BY);
		return True;
	}
        if ( (n==EX_NORTH)
	&&   (pmetatype(mynum)==META_SNAKE)
	&&   (pnumobs(mynum)>1)
        &&   (ploc(mynum)==LOC_META_META45)
        &&   (state(OBJ_META_HOLE_VOLES)==1)
            )
        {
                bprintf("You &+Gslither&* north.\n");
                trapch(LOC_META_META46);
                send_msg(LOC_META_META45, 0, pvis(mynum), LVL_MAX, mynum,NOBODY,
"%s &+Gslithers&* north.\n",pname(mynum)
                        );
                bprintf(
"As you enter the castle, the King looks you up and down.\n"
"'&+MMy young friend,&*' he says, '&+Myou have freed us from the threat of the\n"
"&+Mvoles! Take this as a token of our gratitude, it is our most prized of\n"
"&+Mposessions, and an integral part of the potion you will need to return yourself\n"
"&+Mto your human form.&*' He winks, '&+MYes, I know who you are, and wish you\n"
"&+Mluck on your quest, may the gods be with you on your journey.&*'\n"
"\n"
"Seeing that your inventory is already full, he places a &+Ygolden&* scale\n"
"in front of you.\n"
                        );
                setoloc(OBJ_META_SCALE, ploc(mynum), IN_ROOM);
                return True;
        }

	if ( (n==EX_NORTH)
	&&   (ploc(mynum)==LOC_META_META45)
	&&   (oloc(OBJ_META_FLASK_FULL)==LOC_LIMBO_LIMBO)
	&&   (oloc(OBJ_META_FLASK_EMPTY)==LOC_LIMBO_LIMBO)
	&&   (pmetatype(mynum)==META_SNAKE)
	&&   (pnumobs(mynum)==1)
	    )
	{
		bprintf("You &+Gslither&* north.\n");
		trapch(LOC_META_META46);
		send_msg(LOC_META_META45, 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s &+Gslithers&* north.\n",pname(mynum)
			);
		bprintf(
"As you enter the castle, the King looks you up and down.\n"
"'&+MSurely you are the one for whom we have been waiting&*,' he says,\n"
"'&+Mfor you are clearly not from these parts. Legend foretold of your\n"
"&+Marrival, here, take this flask, you may find it useful in your journey\n"
"&+Mahead.  The voles have plagued our lands for weeks now, stealing our eggs\n"
"&+Mand young in the night, you must seal the hole to their realm for us!\n"
"&+MHelp us, please, you're our only hope...&*'\n"
"You take the &+Cflask&* and bow your head to the &+CKing&*, resolving to do what\n"
"must be done to save the snake kingdom.\n"
			);
		setoloc(OBJ_META_FLASK_EMPTY, mynum, CARRIED_BY);
		return True;
	}

	if ( (n==EX_DOWN)
	&&   (ploc(mynum)==LOC_META_META6)
	&&   ( (!iscontin(OBJ_META_RED_FLAME, OBJ_META_BOTTLE))
	||     ((iscontin(OBJ_META_RED_FLAME, OBJ_META_BOTTLE))&&(!iscarrby(OBJ_META_BOTTLE, mynum))))
	&&   (pmetatype(mynum)==META_BEETLE)
	    )
	{
		bprintf(
"As you make to enter the &+Chole&*, a &+Ldark&* spirit knocks you backwards\n"
"away from it.\n"
			);
		return True;
	}


	  if ( (n == EX_DOWN)
	  &&   (ploc(mynum)==LOC_META_META6)
	  &&   (iscarrby(OBJ_META_BOTTLE, mynum))
	  &&   (iscontin(OBJ_META_RED_FLAME, OBJ_META_BOTTLE))
	      )
	  {
		bprintf(
"Armed with the &+Ybright light&* streaming from the &+Cbottle&*, the dark spirit\n"
"shies away and doesn't attack you.\n");
		osetbit(OBJ_META_BOTTLE, OFL_LIT);
	      trapch(LOC_META_VERY_FOOLISH);
     	return True;
	}

        if ( (n==EX_NORTH)
        &&   (ploc(mynum)==LOC_META_META45)
        &&   (oloc(OBJ_META_FLASK_FULL)==LOC_LIMBO_LIMBO)
        &&   (oloc(OBJ_META_FLASK_EMPTY)==LOC_LIMBO_LIMBO)
        &&   (pmetatype(mynum)==META_SNAKE)
        &&   (pnumobs(mynum)>1)
            )
        {
                bprintf("You &+Gslither&* north.\n");
		trapch(LOC_META_META46);
                send_msg(LOC_META_META45, 0, pvis(mynum), LVL_MAX, NOBODY, NOBODY,
"%s &+Gslithers&* north.\n",pname(mynum)
                        );
                bprintf(
"As you enter the castle, the King looks you up and down.\n"
"'&+MSurely you are the one for whom we have been waiting,&*' he says,\n"
"'&+Mfor you are clearly not from these parts. Legend foretold of your\n"
"&+Marrival, here, take this flask, you may find it useful in your journey\n"
"&+Mahead.  The voles have plagued our lands for weeks now, stealing our eggs\n"
"&+Mand young in the night, you must seal the hole to their realm for us!\n"
"&+MHelp us, please, you're our only hope...&*'\n"
"Seeing that your mouth is already full, he places the &+Cflask&* in front\n"
"of you.\n"
                        );
                setoloc(OBJ_META_FLASK_EMPTY, ploc(mynum), IN_ROOM);
                return True;
        }

        if ( (n==EX_DOWN)
	&&   (ploc(mynum)==LOC_META_META1)
	&&   (pmetatype(mynum)==META_SNAKE)
	    )
	{
		bprintf("You slither &+Gdown&* the &+Chole&*.\n");
		setploc(mynum, LOC_META_META35);
		lookin(ploc(mynum), 1);
		send_msg(LOC_META_META1, 0, pvis(mynum), LVL_MAX, NOBODY,
NOBODY, "%s &+Gslithers&* down&*.\n", pname(mynum)
			);
		send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s slithers in from &+Gabove&*.\n", pname(mynum)
			);
		return True;
	}

        if ( (n==EX_DOWN)
        &&   (ploc(mynum)==LOC_META_META1)
        &&   (pmetatype(mynum)==META_BEETLE)
            )
        {
                bprintf("You scuttle &+Gdown&* the &+Chole&*.\n");
                setploc(mynum, LOC_META_META35);
                lookin(ploc(mynum), 1);
                send_msg(LOC_META_META1, 0, pvis(mynum), LVL_MAX, NOBODY,
NOBODY, "%s &+Gscuttles&* down&*.\n", pname(mynum)
                        );
                send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s scuttles in from &+Gabove&*.\n", pname(mynum)
                        );
                return True;
        }

        if ( (n==EX_UP)
        &&   (ploc(mynum)==LOC_META_META35)
	&&   (pmetatype(mynum)==META_SNAKE)
	&&   (plev(mynum)<LVL_WIZARD)
            )
        {
                bprintf("You slither &+Gup&* the &+Chole&*.\n");
                setploc(mynum, LOC_META_META1);
                lookin(ploc(mynum), 1);
                send_msg(LOC_META_META1, 0, pvis(mynum), LVL_MAX, mynum,
NOBODY, "%s &+Gslithers&* in from below.\n", pname(mynum)
                        );
		send_msg(LOC_META_META35, 0, pvis(mynum), LVL_MAX, mynum,
NOBODY, "%s &+gslithers&* &+Gupwards&*.\n", pname(mynum)
			);
                return True;
	}

        if ( (n==EX_UP)
        &&   (ploc(mynum)==LOC_META_META35)
        &&   (pmetatype(mynum)==META_BEETLE)
            )
        {
                bprintf("You scuttle &+Gup&* the &+Chole&*.\n");
                setploc(mynum, LOC_META_META1);
                lookin(ploc(mynum), 1);
                send_msg(LOC_META_META1, 0, pvis(mynum), LVL_MAX, mynum,
NOBODY, "%s &+Cscuttles&* in from below.\n", pname(mynum)
                        );
                send_msg(LOC_META_META35, 0, pvis(mynum), LVL_MAX, mynum,
NOBODY, "%s &+Cscuttles&* &+Gupwards&*.\n", pname(mynum)
                        );
                return True;
        }

	if (pmetatype(mynum)==META_SNAKE)
	{
		newch=getexit (ploc (mynum), n);

	    	if (newch >= DOOR && newch < EDOOR)
		{
     			drnum = newch - DOOR;
      			droff = olinked (drnum);   /* other door side */
     			if (state (drnum))
			{
        			if (!EQ (oname (drnum), "door") || isdark ()
            			|| EMPTY (olongt (drnum, state (drnum))))
          				bprintf ("You can't go that way.\n");
        			else
          				bprintf ("The door is closed.\n");
        				return -1;
      			}
     		 newch = obj_loc (droff);
    		}

                if (!exists (newch))
                {
                        bprintf ("You can't go that way.\n");
                        return -1;
                }

 for (j = 0; j < lnumchars (ploc(mynum)) && (i = lmob_nr (j, ploc(mynum)),1); j++)
  {
  if(i>=max_players)
  {
  if((pgoodbye(i))[0]!='\0')
  {
  strcpy(mood," says ");
         if ( (strstr(pgoodbye(i), "!") != NULL))
         {
         strcpy(mood," exclaims ");
         }
         if ( (strstr(pgoodbye(i), "?") != NULL))
         {
         strcpy(mood," asks ");
         }
  bprintf("&+C%s&*%s'&*%s&*'\n\n",pname(i),mood,pgoodbye(i));
  }
  }
}


		send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s &+Gslithers&* %s.\n",pname(mynum),exittxt[n]
			);
       		sendf(newch,
"%s &+Gslithers&* in from %s.\n" ,pname(mynum),arrivetxt[n]
			);
		bprintf("You &+Gslither &*%s&*.\n\n",exittxt[n]);
  if(ltstflg(newch,LFL_NOREGEN))
	{
	bprintf("&+RYou feel uneasy and will gain no rest here.\n\n");
	}
  if((ltstflg(newch,LFL_NEGREGEN))&&(plev(mynum)<LVL_WIZARD))
	{
	bprintf("&+RYou're getting weaker every second you stay here.\n\n");
	}
  trapch (newch);
  do_follow ();

 for (j = 0; j < lnumchars (newch) && (i = lmob_nr (j,newch),1); j++)
  {
  if (i>=max_players)
  {
  if(((pgreeting(i))[0]!='\0')&&(pstr(i)>0))
  {
  strcpy(mood," says ");
         if ( (strstr(pgreeting(i), "!") != NULL))
         {
         strcpy(mood," exclaims ");
         }
         if ( (strstr(pgreeting(i), "?") != NULL))
         {
         strcpy(mood," asks ");
         }
  bprintf("&+C%s&*%s'&*%s&*'\n",pname(i),mood,pgreeting(i));
  }
  if((pgreetingaction(i))[0]!='\0')
  {
  bprintf("%s\n",pgreetingaction(i));
  }
  }
}
		return True;
	}

	if ( (pmetatype(mynum)==META_FISH_UNDERWATER)
	||   (pmetatype(mynum)==META_SNAKE_UNDERWATER)
	||   (pmetatype(mynum)==META_BEETLE_UNDERWATER)
	    )
	{
		newch=getexit (ploc (mynum), n);
                if (newch >= DOOR && newch < EDOOR)
                {
                        drnum = newch - DOOR;
                        droff = olinked (drnum);   /* other door side */
                        if (state (drnum))
                        {
                                if (!EQ (oname (drnum), "door") || isdark ()
                                || EMPTY (olongt (drnum, state (drnum))))
                                        bprintf ("You can't go that way.\n");
                                else
                                        bprintf ("The door is closed.\n");
                                        return -1;
                        }
                 newch = obj_loc (droff);
                }

		if (!exists (newch))
		{
    			bprintf ("You can't go that way.\n");
    			return -1;
  		}
 for (j = 0; j < lnumchars (ploc(mynum)) && (i = lmob_nr (j, ploc(mynum)),1); j++)
  {
  if(i>=max_players)
  {
  if((pgoodbye(i))[0]!='\0')
  {
  strcpy(mood," says ");
         if ( (strstr(pgoodbye(i), "!") != NULL))
         {
         strcpy(mood," exclaims ");
         }
         if ( (strstr(pgoodbye(i), "?") != NULL))
         {
         strcpy(mood," asks ");
         }
  bprintf("&+C%s&*%s'&*%s&*'\n\n",pname(i),mood,pgoodbye(i));
  }
  }
}


		send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s &+Bswims&* %s.\n",pname(mynum),exittxt[n]
			);
       		sendf(newch,
"%s &+Bswims&* in from %s.\n" ,pname(mynum),arrivetxt[n]
			);
		bprintf("You &+Bswim &*%s&*.\n\n",exittxt[n]);
  if(ltstflg(newch,LFL_NOREGEN))
	{
	bprintf("&+RYou feel uneasy and will gain no rest here.\n\n");
	}
  if((ltstflg(newch,LFL_NEGREGEN))&&(plev(mynum)<LVL_WIZARD))
	{
	bprintf("&+RYou're getting weaker every second you stay here.\n\n");
	}
  trapch (newch);
if ( (iscarrby(OBJ_META_CHEST,mynum))
&&   (state(OBJ_META_CHEST)==0)
&&   (odamage(OBJ_META_CHEST)==mynum+1)
    )
{
	bprintf("Your movement causes the &+Cair&* in the chest to escape!\n");
	destroy(OBJ_META_CHEST);
	setoloc(OBJ_META_CHEST2, mynum, CARRIED_BY);
	if (iscontin(OBJ_META_LIMESTONE, OBJ_META_CHEST))
	{
		bprintf("The &+Climestone&* dissolves in the water!\n");
		destroy(OBJ_META_LIMESTONE);
	}
}

  do_follow ();

 for (j = 0; j < lnumchars (newch) && (i = lmob_nr (j,newch),1); j++)
  {
  if (i>=max_players)
  {
  if(((pgreeting(i))[0]!='\0')&&(pstr(i)>0))
  {
  strcpy(mood," says ");
         if ( (strstr(pgreeting(i), "!") != NULL))
         {
         strcpy(mood," exclaims ");
         }
         if ( (strstr(pgreeting(i), "?") != NULL))
         {
         strcpy(mood," asks ");
         }
  bprintf("&+C%s&*%s'&*%s&*'\n",pname(i),mood,pgreeting(i));
  }
  if((pgreetingaction(i))[0]!='\0')
  {
  bprintf("%s\n",pgreetingaction(i));
  }
  }
}
		return True;
	}

	if (pmetatype(mynum)==META_FISH_ONLAND)
        {
		newch=getexit (ploc (mynum), n);
                if (newch >= DOOR && newch < EDOOR)
                {
                        drnum = newch - DOOR;
                        droff = olinked (drnum);   /* other door side */
                        if (state (drnum))
                        {
                                if (!EQ (oname (drnum), "door") || isdark ()
                                || EMPTY (olongt (drnum, state (drnum))))
                                        bprintf ("You can't go that way.\n");
                                else
                                        bprintf ("The door is closed.\n");
                                        return -1;
                        }
                 newch = obj_loc (droff);
                }

		if (!exists (newch))
		{
    			bprintf ("You can't go that way.\n");
    			return -1;
  		}
	bprintf(
"You &+Cflap&* around vainly but are unable to leave the room as a &+Bfish&*!\n");
	return True;
	}

	if (pmetatype(mynum)==META_BIRD)
	{
		newch=getexit (ploc (mynum), n);
                if (newch >= DOOR && newch < EDOOR)
                {
                        drnum = newch - DOOR;
                        droff = olinked (drnum);   /* other door side */
                        if (state (drnum))
                        {
                                if (!EQ (oname (drnum), "door") || isdark ()
                                || EMPTY (olongt (drnum, state (drnum))))
                                        bprintf ("You can't go that way.\n");
                                else
                                        bprintf ("The door is closed.\n");
                                        return -1;
                        }
                 newch = obj_loc (droff);
                }

		if (!exists (newch))
		{
    			bprintf ("You can't go that way.\n");
    			return -1;
  		}
 for (j = 0; j < lnumchars (ploc(mynum)) && (i = lmob_nr (j, ploc(mynum)),1); j++)
  {
  if(i>=max_players)
  {
  if((pgoodbye(i))[0]!='\0')
  {
  strcpy(mood," says ");
         if ( (strstr(pgoodbye(i), "!") != NULL))
         {
         strcpy(mood," exclaims ");
         }
         if ( (strstr(pgoodbye(i), "?") != NULL))
         {
         strcpy(mood," asks ");
         }
  bprintf("&+C%s&*%s'&*%s&*'\n\n",pname(i),mood,pgoodbye(i));
  }
  }
}


		send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s &+Mflies&* %s.\n",pname(mynum),exittxt[n]
			);
       		sendf(newch,
"%s &+Mflies&* in from %s.\n" ,pname(mynum),arrivetxt[n]
			);
		bprintf("You &+Mfly &*%s&*.\n\n",exittxt[n]);
  if(ltstflg(newch,LFL_NOREGEN))
	{
	bprintf("&+RYou feel uneasy and will gain no rest here.\n\n");
	}
  if((ltstflg(newch,LFL_NEGREGEN))&&(plev(mynum)<LVL_WIZARD))
	{
	bprintf("&+RYou're getting weaker every second you stay here.\n\n");
	}
  trapch (newch);
  do_follow ();

 for (j = 0; j < lnumchars (newch) && (i = lmob_nr (j,newch),1); j++)
  {
  if (i>=max_players)
  {
  if(((pgreeting(i))[0]!='\0')&&(pstr(i)>0))
  {
  strcpy(mood," says ");
         if ( (strstr(pgreeting(i), "!") != NULL))
         {
         strcpy(mood," exclaims ");
         }
         if ( (strstr(pgreeting(i), "?") != NULL))
         {
         strcpy(mood," asks ");
         }
  bprintf("&+C%s&*%s'&*%s&*'\n",pname(i),mood,pgreeting(i));
  }
  if((pgreetingaction(i))[0]!='\0')
  {
  bprintf("%s\n",pgreetingaction(i));
  }
  }
}
		return True;
	}

	if (pmetatype(mynum)==META_BEETLE)
	{
		newch=getexit (ploc (mynum), n);

	    	if (newch >= DOOR && newch < EDOOR)
		{
     			drnum = newch - DOOR;
      			droff = olinked (drnum);   /* other door side */
     			if (state (drnum))
			{
        			if (!EQ (oname (drnum), "door") || isdark ()
            			|| EMPTY (olongt (drnum, state (drnum))))
          				bprintf ("You can't go that way.\n");
        			else
          				bprintf ("The door is closed.\n");
        				return -1;
      			}
     		 newch = obj_loc (droff);
    		}

                if (!exists (newch))
                {
                        bprintf ("You can't go that way.\n");
                        return -1;
                }

 for (j = 0; j < lnumchars (ploc(mynum)) && (i = lmob_nr (j, ploc(mynum)),1); j++)
  {
  if(i>=max_players)
  {
  if((pgoodbye(i))[0]!='\0')
  {
  strcpy(mood," says ");
         if ( (strstr(pgoodbye(i), "!") != NULL))
         {
         strcpy(mood," exclaims ");
         }
         if ( (strstr(pgoodbye(i), "?") != NULL))
         {
         strcpy(mood," asks ");
         }
  bprintf("&+C%s&*%s'&*%s&*'\n\n",pname(i),mood,pgoodbye(i));
  }
  }
}


		send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s &+Cscuttles&* %s.\n",pname(mynum),exittxt[n]
			);
       		sendf(newch,
"%s &+Cscuttles&* in from %s.\n" ,pname(mynum),arrivetxt[n]
			);
		bprintf("You &+Cscuttle &*%s&*.\n\n",exittxt[n]);
  if(ltstflg(newch,LFL_NOREGEN))
	{
	bprintf("&+RYou feel uneasy and will gain no rest here.\n\n");
	}
  if((ltstflg(newch,LFL_NEGREGEN))&&(plev(mynum)<LVL_WIZARD))
	{
	bprintf("&+RYou're getting weaker every second you stay here.\n\n");
	}
  trapch (newch);
  do_follow ();

 for (j = 0; j < lnumchars (newch) && (i = lmob_nr (j,newch),1); j++)
  {
  if (i>=max_players)
  {
  if(((pgreeting(i))[0]!='\0')&&(pstr(i)>0))
  {
  strcpy(mood," says ");
         if ( (strstr(pgreeting(i), "!") != NULL))
         {
         strcpy(mood," exclaims ");
         }
         if ( (strstr(pgreeting(i), "?") != NULL))
         {
         strcpy(mood," asks ");
         }
  bprintf("&+C%s&*%s'&*%s&*'\n",pname(i),mood,pgreeting(i));
  }
  if((pgreetingaction(i))[0]!='\0')
  {
  bprintf("%s\n",pgreetingaction(i));
  }
  }
}
		return True;
	}


return False;
}

void meta_timer_become_fish(Boolean start_fish_become, Boolean stop_fish_become)
{
	static int timestarted_fish_become; 
	static Boolean running_fish_become;
	int pl;	

        if ( (start_fish_become==True)
	    )
	{ 
	        running_fish_become=True; 
                timestarted_fish_become=global_clock;
                return;
	} 

        if ( (stop_fish_become==True) 
	      )
        { 
 	        running_fish_become=False; 
		timestarted_fish_become=0;
		return; 
	} 

	if (!running_fish_become) return;


	if ( (global_clock-timestarted_fish_become >= 15)
	&&   (global_clock-timestarted_fish_become <= 17)
	    )
	{ 
	for (pl=0; pl < max_players;pl++)
		{
			if ( (pmetatype(pl)==META_FISH_ONLAND)
			    )
			{
    send_msg (DEST_ALL, MODE_QUIET, max (pvis (pl), LVL_WIZARD), LVL_MAX,pl,NOBODY,
"&+B[&+RForgot how to breathe&+B]\n");
                		p_crapup(pl,
"&+RYou suffocate!\n",CRAP_SAVE
			);
			}
		}
	}
}

Boolean meta_jump(void)
{
int obj;
	if ( (ploc(mynum) == LOC_META_META17)
	&&   ( (pmetatype(mynum)==META_SNAKE)
	||     (pmetatype(mynum)==META_FISH_ONLAND)
	||     (pmetatype(mynum)==META_HUMAN)
	      )
	    )
	{
		bprintf(
"You &+Cjump&* up and down on the rotten &+Cplanks&*.\n"
"Unable to take the strain, the &+Cplanks&* shatter under you and you fall to\n"
"your death.\n"
			);
    send_msg (DEST_ALL, MODE_QUIET, max (pvis (mynum), LVL_WIZARD),LVL_MAX,mynum,NOBODY,
"&+B[&+RFell a long way&+B]\n");
                                p_crapup(mynum,
"&+RYou fall and fall and fall, and splatter!\n",CRAP_SAVE
                        );
		return True;
	}

        if ( (ploc(mynum) == LOC_META_META17)
        &&   (pmetatype(mynum)==META_BEETLE)
	    )
	{
		bprintf(
"You &+Cjump&* up and down on the rotten &+Cplanks&*.\n"
"Nothing happens, it appears that you are too light to have any effect.\n"
			);
		return True;
	}
	
	if ( (ploc(mynum) == LOC_META_META17)
	&&   (pmetatype(mynum)==META_BIRD)
	    )
	{
		bprintf(
"You &+Cjump&* up and down on the rotten &+Cplanks&*.\n"
"Unable to take the strain, the &+Cplanks&* shatter under you and you start\n"
"to fall. Suddenly, remembering your &+Cwings&*, you flap hard and fast and soar\n"
"back up and out of the hole you just created. You fly out of the storeroom\n"
"just in time as it collapses behind you.\n"
			);
		setobjstate(OBJ_META_MARKERDOOR, 1);
		setobjstate(OBJ_META_MARKERDOOR2, 1);
		trapch(LOC_META_META14);
		return True;
	}

	if ( (ploc(mynum)==LOC_META_META73)
	&&   (pmetatype(mynum)==META_FISH_UNDERWATER)
	    )
	{
		p_crapup(mynum,
"You sail into the air with an almighty leap! Unfortunately you misjudge\n"
"the jump and splatter on a rock at the bottom of the waterfall.\n"
,CRAP_SAVE);
		return True;
	}
	
	if ( (ploc(mynum)==LOC_META_META8)
	&&   (pmetatype(mynum)==META_FISH_ONLAND)
	    )
	{
		setpmetatype(mynum, META_FISH_UNDERWATER);
		bprintf(
"You make a colossal &+Cleap&* and land with a splash in the &+Bpool&*!\n"
			);
		send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s &+Cleaps&* into the &+Bpool&*.\n",pname(mynum)
			);
		send_msg(LOC_META_META20, 0, pvis(mynum), LVL_MAX, mynum,
NOBODY,
"%s &+Centers&* from &+Gabove&* with a huge &+Bsplash&*!\n",pname(mynum)
			);
		setploc(mynum, LOC_META_META20);
		lookin(ploc(mynum), 0);
		meta_timer_become_fish(False,True);
		for (obj = 0; obj < pnumobs (mynum); obj++)
		{
			if ( (iscarrby (pobj_nr (obj, mynum), mynum))
			&&   (otstbit(pobj_nr (obj, mynum), OFL_LIT))
			    )
			{
				bprintf(
"The &+Bwater&* extinguishes your &+C%s&*\n", oname(pobj_nr(obj, mynum))
						);
				setobjstate(pobj_nr(obj, mynum), 1);
				oclrbit(pobj_nr(obj, mynum), OFL_LIT);
			}
                        if ( (iscarrby (pobj_nr (obj, mynum), mynum))
                        &&   (pobj_nr(obj,mynum)==OBJ_META_BOTTLE)
                        &&   ( (iscontin(OBJ_META_RED_FLAME, OBJ_META_BOTTLE))
                        ||     (iscontin(OBJ_META_BLUE_FLAME, OBJ_META_BOTTLE))
                              )
                            )
                        {
                                bprintf(
"The &+Bwater&* extinguishes the &+Cflame&* in the &+Cbottle&*.\n"
                                        );
                                if (iscontin(OBJ_META_RED_FLAME, OBJ_META_BOTTLE))
                                { destroy(OBJ_META_RED_FLAME); }
                                if (iscontin(OBJ_META_BLUE_FLAME, OBJ_META_BOTTLE))
                                { destroy(OBJ_META_BLUE_FLAME); }
                        }

		}
		return True;
	}
        if ( (ploc(mynum)==LOC_META_META20)
	&&   (pmetatype(mynum)==META_FISH_UNDERWATER)
            )
        {
		setpmetatype(mynum, META_FISH_ONLAND);
                bprintf(
"You make a colossal &+Cleap&* and land with a thud on the &+Gbank&*!\n"
                        );
                send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s &+Cleaps&* out of the &+Bpool&*.\n",pname(mynum)
                        );
                send_msg(LOC_META_META8, 0, pvis(mynum), LVL_MAX, mynum,
NOBODY,
"%s &+Cleaps&* in from the &+Bpool&* with a huge &+Gthud&*!\n",pname(mynum)
                        );
                setploc(mynum, LOC_META_META8);
                lookin(ploc(mynum), 0);
                meta_timer_become_fish(True,False);
	   return True;
        }

	if ( (ploc(mynum)==LOC_META_META32)
	&&   (pmetatype(mynum)==META_FISH_UNDERWATER)
	    )
	{
		bprintf(
"You make a colossal leap and land halfway up the waterfall. You start to\n"
"slip and slide back down, unable to swim against the might of the waterfall.\n"
			);
		meta_timer_jump(True, False);
		setploc(mynum, LOC_META_HALFWAY);
		lookin(ploc(mynum), 0);
		send_msg(LOC_META_META32, 0, pvis(mynum), LVL_MAX, NOBODY, NOBODY,
"%s leaps up the waterfall.\n", pname(mynum)
			);
		return True;
	}

	if ( (ploc(mynum)==LOC_META_HALFWAY)
	&&   (pmetatype(mynum)==META_FISH_UNDERWATER)
	    )
	{
		bprintf(
"You make another enormous leap and land with a splash at the top of the\n"
"waterfall.\n");
		trapch(LOC_META_META73);
		send_msg(LOC_META_HALFWAY, 0, pvis(mynum), LVL_MAX, NOBODY, NOBODY,
"%s leaps up the waterfall.\n", pname(mynum)
			);
		meta_timer_jump(False, True);
		return True;
	}
return False;
}

Boolean meta_get(int ob)
{
	if (otstbit(ob, OFL_NOGET))
	{
		bprintf("You can't take that!\n");
		return True;
	}

	if (iscarrby(ob,mynum))
	{
		bprintf("It's not here.\n");
		return True;
	}

        if ( (pmetatype(mynum)==META_FISH_ONLAND)
        &&   (pnumobs(mynum) > 0)
           )
        {
                bprintf("Your mouth is already full, you can't carry any more!\n");
                return True;
        }

        if ( (pmetatype(mynum)==META_FISH_UNDERWATER)
        &&   (pnumobs(mynum) > 0)
	&&   (ob != OBJ_META_LIMESTONE)
	&&   (!iscontin(ob, OBJ_META_PANEL))
           )
        {
                bprintf("Your mouth is already full, you can't carry any more!\n");
                return True;
        }

        if ( ((pmetatype(mynum)==META_BEETLE)
	  ||    (pmetatype(mynum)==META_BEETLE_UNDERWATER)
		  )
        &&   (pnumobs(mynum) > 0)
	  &&    (ob != OBJ_META_LIMESTONE)
           )
        {
		bprintf(
"You already have something strapped to your back, you can't carry any more!\n"
			);
		return True;
	}

	if ( ( (pmetatype(mynum)==META_SNAKE)
	||     (pmetatype(mynum)==META_SNAKE_UNDERWATER)
	      )
	&&   (pnumobs(mynum) > 1)
	  &&    (ob != OBJ_META_LIMESTONE)
	    )
	{
		bprintf(
"You are already carrying something in your mouth and can't carry any more.\n");
		return True;
	}

	if ( (pmetatype(mynum)==META_BIRD)
	&&   (pnumobs(mynum) > 1)
	    )
	{
		bprintf(
"You already have something in your beak and can't carry any more.\n"
			);
		return True;
	}
	
	if ( (ob == OBJ_META_BLUE_FLAME)
	    )
	{
		bprintf(
"You try to pick up the &+Cflame&* but it is paradoxically too cold to touch.\n");
		return True;
	}
	
	if (ob == OBJ_META_RED_FLAME)
	{
		bprintf(
"The &+Cflame&* is too &+Rhot&* to touch.\n"
				);
		return True;
	}

	if ( (ob == OBJ_META_WATER)
	||   (ob == OBJ_META_WATER_2)
	||   (ob == OBJ_META_WATER_3)
	||   (ob == OBJ_META_WATER_4)
	||   (ob == OBJ_META_ALKALINE)
	    )
	{
		bprintf(
"You try to handle the &+C%s&* but it slips through your fingers.\n", oname(ob)
			);
		return True;
	}

	if (ob == OBJ_META_DUNG)
	{
		bprintf(
"Guards step in front of you, barring you from the &+Cdung&*.\n"
				);
		return True;
	}

	if ( (ob==OBJ_META_VINE)
	&&   (iscontin(ob, OBJ_META_VINES))
	&&   (pmetatype(mynum)!=META_BIRD)
	    )
	{
		bprintf(
"You are too far out of reach.\n"
			);
		return True;
	}
        if ( (ob == OBJ_META_VINE)
        &&   (iscontin(ob, OBJ_META_VINES))
        &&   (pmetatype(mynum)==META_BIRD)
        &&   (pnumobs(mynum)==1)
            )
        {
                bprintf(
"You &+Mfly&* up to the &+Cvines&* and remove a &+Csmall vine&* from them.\n"
                                );
                setoloc(ob, mynum, CARRIED_BY);
                return True;
        }


	if ( (ob == OBJ_META_GUNPOWDER)
	&&   (iscontin(ob, OBJ_META_GUN))
	&&   (pmetatype(mynum)!=META_BIRD)
	    )
	{
		bprintf(
"You are too far out of reach.\n"
				);
		return True;
	}

	if ( (ob == OBJ_META_GUNPOWDER)
	&&   (iscontin(ob, OBJ_META_GUN))
	&&   (pmetatype(mynum)==META_BIRD)
	&&   (pnumobs(mynum)==1)
	    )
	{
		bprintf(
"You &+Mfly&* up to the &+Cgun&* and take the &+Cgunpowder&* from it.\n"
				);
		setoloc(OBJ_META_GUNPOWDER, mynum, CARRIED_BY);
		return True;
	}
	if ( (ob == OBJ_META_GUNPOWDER)
	&&   (iscontin(ob, OBJ_META_GUN))
	&&   (pmetatype(mynum)==META_BIRD)
	&&   (pnumobs(mynum)>1)
	    )
	{
		bprintf(
"You are already carrying something in your &+Cbeak&* and cannot carry any\n"
"more\n"
				);
		return True;
	}
        if ( (ob == OBJ_META_LIMESTONE)
        &&   (iscontin(ob, OBJ_META_PANEL))
        &&   (ltstflg(ploc(mynum), LFL_IN_WATER))
        &&   (iscarrby(OBJ_META_CHEST, mynum))
        &&   (state(OBJ_META_CHEST)==0)
            )
        {
                bprintf(
"You remove the &+Climestone&* from the &+Cpanel&* and watch in horror as it\n"
"starts to dissolve in the water. You quickly shove it into the chest\n"
"and breathe a sigh of relief as it stops dissolving, now it's in air.\n"
                        );
                setoloc(OBJ_META_LIMESTONE, OBJ_META_CHEST, IN_CONTAINER);
                return True;
        }

        if ( (ob == OBJ_META_LIMESTONE)
        &&   (iscontin(ob, OBJ_META_PANEL))
        &&   (ltstflg(ploc(mynum), LFL_IN_WATER))
        &&   (!iscarrby(OBJ_META_CHEST, mynum))
            )
        {
                bprintf(
"You remove the &+Climestone&* from the &+Cpanel&* and watch in horror as it\n"
"starts to dissolve in the water. You watch helplessly as it dissolves away\n"
"to nothingness.\n"
                        );
                destroy(ob);
                return True;
        }

        if ( (ob == OBJ_META_LIMESTONE)
        &&   (iscontin(ob, OBJ_META_PANEL))
        &&   (ltstflg(ploc(mynum), LFL_IN_WATER))
        &&   (iscarrby(OBJ_META_CHEST, mynum))
        &&   (state(OBJ_META_CHEST)==1)
            )
        {
                bprintf(
"You remove the &+Climestone&* from the &+Cpanel&* and watch in horror as it\n"
"starts to dissolve in the water. You watch helplessly as it dissolves away\n"
"to nothingness.\n"
                        );
                destroy(ob);
                return True;
        }

return False;
}

Boolean meta_wield(void)
{
	if ( (pmetatype(mynum)==META_SNAKE)
	||   (pmetatype(mynum)==META_SNAKE_UNDERWATER)
	    )
        {
                bprintf("What are you trying to wield it with? You have no hands!\n");
                return True;
        }

	if (pmetatype(mynum)==META_FISH_ONLAND)
	{
		bprintf("What are you trying to wield it with? You have no hands!\n");
		return True;
	}

        if (pmetatype(mynum)==META_FISH_UNDERWATER)
        {
                bprintf("What are you trying to wield it with? You have no hands!\n");
                return True;
        }


	if (pmetatype(mynum)==META_BIRD)
	{
		bprintf(
"You try to grasp it in your &+Cwings&* but fail miserably.\n"
			);
		return True;
	}

	if ( (pmetatype(mynum)==META_BEETLE)
	||   (pmetatype(mynum)==META_BEETLE_UNDERWATER)
	    )
	{
		bprintf(
"Your &+Clegs&* are much too small to wield it!'\n"
			);
		return True;
	}

	return False;
}

Boolean meta_fly(void)
{
	if (ploc(mynum)==LOC_META_META14)
	{
		bprintf(
"You &+Csoar&* into the air to try and cross the &+Bmoat&*, but are &+Rbeaten&* back by\n"
"sentry birds. Apparently only entry by &+Cfoot&* is possible.\n"
			);
		return True;
	}

	if ( (ploc(mynum)==LOC_META_META53)
	&&   (state(OBJ_META_DOCK)==1)
	    )
	{
		bprintf(
"You spread your &+Cwings&* to &+Cfly&* away, but the hawks circling above give\n"
"you pause for thought, and you decide to stay where you are.\n"
			);
		return True;
	}

        if ( (pmetatype(mynum)==META_BIRD)
        &&   (ploc(mynum)==LOC_META_BESIDEWATERFALL)
            )
        {
		bprintf(
"You soar into the air and fly above the forest, circling until you see familiar\n"
"ground, which you make for and land on.\n"
			);
		setploc(mynum, LOC_META_META1);
		lookin(ploc(mynum), 0);
		return True;
	}

        if ( (pmetatype(mynum)==META_BIRD)
        &&   (ploc(mynum)==LOC_META_META7)
            )
        {
                bprintf(
"You &+Csoar&* into the lower branches of the tree.\n"
                        );
                setploc(mynum, LOC_META_META51);
                lookin(ploc(mynum), 0);
                return True;
        }

        if ( (pmetatype(mynum)==META_BIRD)
        &&   (ploc(mynum)==LOC_META_META58)
            )
        {
                bprintf(
"You &+Csoar&* down into the lower branches of the tree.\n"
                        );
                setploc(mynum, LOC_META_META51);
                lookin(ploc(mynum), 0);
                return True;
        }

        if ( (pmetatype(mynum)==META_BIRD)
        &&   (ploc(mynum)==LOC_META_META51)
            )
        {
                bprintf(
"You &+Csoar&* into the upper branches of the tree.\n"
                        );
                setploc(mynum, LOC_META_META58);
                lookin(ploc(mynum), 0);
                return True;
        }

	if ( (pmetatype(mynum)==META_BIRD)
	&&   (ploc(mynum)!=LOC_META_BESIDEWATERFALL)
	&&   (ploc(mynum)!=LOC_META_META7)
	    )
	{
		bprintf(
"You soar briefly into the &+Cair&* before landing again.\n"
			);
		return True;
	}

return False;
}

Boolean meta_equip(void)
{
	if ( (pmetatype(mynum)==META_SNAKE)
	&&   ( (EMPTY(item1)) || (pl1 == mynum))
	    )
	{
		bprintf(
"\nYou're a &+Gsnake&* and can't wear &+Marmour&*!\n\n"
"      &+YHead&+G: &+MNone&*.\n"
"      &+YBody&+G: &+MNone&*.\n"
"    &+RWeapon&+G: &+CTeeth\n"
			);
		return True;
	}

        if ( (pmetatype(pl1)==META_SNAKE)
        &&   (!EMPTY(item1))
	    )
        {
                bprintf(
"\n%s is a &+Gsnake&* and can't wear &+Marmour&*!\n\n"
"      &+YHead&+G: &+MNone&*.\n"
"      &+YBody&+G: &+MNone&*.\n"
"    &+RWeapon&+G: &+CTeeth\n", pname(pl1)
                        );
                return True;
        }

	if ( (pmetatype(mynum)==META_BIRD)
	&&   ((EMPTY(item1)) || (pl1 == mynum))
	    ) 
	{
		bprintf(
"\nYou're a &+Gbird&* and can't wear &+Marmour&*!\n\n"
"      &+YHead&+G: &+MNone&*.\n"
"      &+YBody&+G: &+MNone&*.\n"
"     &+YWings&+G: &+MNone&*.\n"
"      &+Ylegs&+G: &+MNone&*.\n"
"      &+YFeet&+G: &+MNone&*.\n"
"    &+RWeapon&+G: &+CBeak\n"
			);
		return True;
	}

        if ( (pmetatype(pl1)==META_BIRD)
        &&   (!EMPTY(item1))
	    )
        {
		bprintf(
"\n%s is a &+Gbird&* and can't wear &+Marmour&*!\n\n"
"      &+YHead&+G: &+MNone&*.\n"
"      &+YBody&+G: &+MNone&*.\n"
"     &+YWings&+G: &+MNone&*.\n"
"      &+Ylegs&+G: &+MNone&*.\n"
"      &+YFeet&+G: &+MNone&*.\n"
"    &+RWeapon&+G: &+CBeak\n", pname(pl1)
			);
		return True;
        }

        if ( (pmetatype(mynum)==META_FISH_ONLAND)
        &&   ((EMPTY(item1)) || (pl1 == mynum))
            )
        {
                bprintf(
"\nYou're a &+Bfish&* and can't wear &+Marmour&*!\n\n"
"      &+YHead&+G: &+MNone&*.\n"
"      &+YBody&+G: &+MNone&*.\n"
"      &+YFins&+G: &+MNone&*.\n"
"    &+RWeapon&+G: &+CNone.\n"
                        );
                return True;
        }

        if ( (pmetatype(pl1)==META_FISH_ONLAND)
        &&   (!EMPTY(item1))
            )
        {
                bprintf(
"\n%s is a &+Bfish&* and can't wear &+Marmour&*!\n\n"
"      &+YHead&+G: &+MNone&*.\n"
"      &+YBody&+G: &+MNone&*.\n"
"      &+YFins&+G: &+MNone&*.\n"
"    &+RWeapon&+G: &+CNone.\n", pname(pl1)
                        );
                return True;
        }

        if ( (pmetatype(mynum)==META_FISH_UNDERWATER)
        &&   ((EMPTY(item1)) || (pl1 == mynum))
            )
        {
                bprintf(
"\nYou're a &+Bfish&* and can't wear &+Marmour&*!\n\n"
"      &+YHead&+G: &+MNone&*.\n"
"      &+YBody&+G: &+MNone&*.\n"
"      &+YFins&+G: &+MNone&*.\n"
"    &+RWeapon&+G: &+CNone.\n"
                        );
                return True;
        }

        if ( (pmetatype(pl1)==META_FISH_UNDERWATER)
        &&   (!EMPTY(item1))
            )
        {
                bprintf(
"\n%s is a &+Bfish&* and can't wear &+Marmour&*!\n\n"
"      &+YHead&+G: &+MNone&*.\n"
"      &+YBody&+G: &+MNone&*.\n"
"      &+YFins&+G: &+MNone&*.\n"
"    &+RWeapon&+G: &+CNone.\n", pname(pl1)
                        );
                return True;
        }

        if ( ((pmetatype(mynum)==META_BEETLE)
	  ||    (pmetatype(mynum)==META_BEETLE_UNDERWATER)
		  )
        &&   ((EMPTY(item1)) || (pl1 == mynum))
            )
        {
                bprintf(
"\nYou're a &+ybeetle&* and can't wear &+Marmour&*!\n\n"
"      &+YHead&+G: &+MNone&*.\n"
"  &+YCarapace&+G: &+MNone&*.\n"
"      &+Ylegs&+G: &+MNone&*.\n"
"    &+RWeapon&+G: &+CNone.\n"
                        );
                return True;
        }

        if ( ((pmetatype(pl1)==META_BEETLE)
	  ||    (pmetatype(pl1)==META_BEETLE_UNDERWATER)
		  )
        &&   (!EMPTY(item1))
            )
        {
                bprintf(
"\n%s is a &+Bbeetle&* and can't wear &+Marmour&*!\n\n"
"      &+YHead&+G: &+MNone&*.\n"
"  &+YCarapace&+G: &+MNone&*.\n"
"      &+Ylegs&+G: &+MNone&*.\n"
"    &+RWeapon&+G: &+CNone.\n", pname(pl1)
                        );
                return True;
        }
return False;
}

Boolean meta_put(int a,int c)
{
int b;

	if ( (c == OBJ_META_MECHANISM)
	&&   (a == OBJ_META_STAFF)
	    )
	{
		bprintf(
"The &+Mfairy&* has given you a much more pressing task for the &+Cstaff&* than\n"
"as a lever!\n"
			);
		return True;
	}

	if ( (c == OBJ_META_CAULDRON)
	&&   (a != OBJ_META_SCALE)
	&&   (a != OBJ_META_SILVER_FEATHER)
	&&   (a != OBJ_META_DUNG_OLD)
	&&   (a != OBJ_META_DIAMOND)
	&&   (a != OBJ_META_LIMESTONE)
	&&   (a != OBJ_META_ALKALINE)
	    )
	{
		bprintf(
"The &+Ccauldron&* shudders and spits the &+C%s&* out.\n", oname(a)
			);
		setoloc(a, ploc(mynum), IN_ROOM);
		return True;
	}

	if ( (c == OBJ_META_WATERFALL)
	&&   (!otstbit(a, OFL_NOGET))
	&&   (!otstbit(a, OFL_PLURAL))
	    )
	{
		bprintf(
"You place the &+C%s&* in the &+Bwaterfall&* and watch as it is quickly swept\n"
"away downstream.\n",oname(a)
			);
		setoloc(a, LOC_META_META20, IN_ROOM);
		return True;
	}

        if ( (c == OBJ_META_WATERFALL)
        &&   (!otstbit(a, OFL_NOGET))
        &&   (otstbit(a, OFL_PLURAL))
            )
        {
                bprintf(
"You place the &+C%s&* in the &+Bwaterfall&* and watch as they are quickly swept away\n"
"downstream.\n",oname(a)
                        );
                setoloc(a, LOC_META_META20, IN_ROOM);
                return True;
        }

	if (a == OBJ_META_DUNG)
	{
		bprintf(
"Guards step in front of you, barring you from the &+Cdung&*.\n"
                                );
                return True;
        }

	if ( (a == OBJ_META_BOTTLE)
	&&   (c == OBJ_META_FLASK_EMPTY)
	    )
	{
		bprintf("It won't fit.\n");
		return True;
	}
        if ( (a == OBJ_META_FLASK_EMPTY)
        &&   (c == OBJ_META_BOTTLE)
            )
        {
                bprintf("It won't fit.\n");
                return True;
        }
        if ( ( (a == OBJ_META_RED_FLAME)
        ||     (a == OBJ_META_BLUE_FLAME)
              )
        &&   (c == OBJ_META_BOTTLE)
        &&   ((iscontin(OBJ_META_WATER, c))
        ||    (iscontin(OBJ_META_WATER_2, c))
        ||    (iscontin(OBJ_META_WATER_3, c))
        ||    (iscontin(OBJ_META_WATER_4, c))
        ||    (iscontin(OBJ_META_ALKALINE, c)))
            )
        {
                bprintf(
"You put the &+Cflame&* in the &+Cbottle&*.\n"
"It is &+Rextinguished&* by the &+Cliquid&* in the bottle!\n"
			);
		destroy(a);
                return True;
        }

	if ( ( (a == OBJ_META_RED_FLAME)
	||     (a == OBJ_META_BLUE_FLAME)
	      )
	&&   (c == OBJ_META_BOTTLE)
	&&   (!iscontin(OBJ_META_WATER, c))
	&&   (!iscontin(OBJ_META_WATER_2, c))
	&&   (!iscontin(OBJ_META_WATER_3, c))
	&&   (!iscontin(OBJ_META_WATER_4, c))
	&&   (!iscontin(OBJ_META_ALKALINE, c))
	    )
	{
		bprintf(
"You put the &+Cflame&* in the &+Cbottle&*.\n");
		setoloc(a,c,IN_CONTAINER);
		return True;
	} 

	if ( (c == OBJ_META_CHEST)
	&&   (a == OBJ_META_BOMB)
	&&   (state(c)==0)
	    )
	{
		bprintf(
"You put the &+Cbomb&* in the &+Cchest&*.\n"
			);
		setoloc(a, c, IN_CONTAINER);
		return True;
	}

	if ( (c == OBJ_META_CAULDRON)
	&&   (a == OBJ_META_ALKALINE)
	&&   (iscontin(OBJ_META_SCALE,c))
	&&   (iscontin(OBJ_META_DUNG_OLD,c))
	&&   (iscontin(OBJ_META_SILVER_FEATHER,c))
	&&   (iscontin(OBJ_META_DIAMOND,c))
	&&   (otstbit(OBJ_META_KINDLING, OFL_LIT))
	    )
	{
		destroy(a);
		bprintf(
"As you add the &+Calkaline&* solution to the cauldron, the ingredients\n"
"shimmer and merge into a deep &+rcrimson&* liquid which gathers in the\n"
"bottom of the cauldron.\n"
				);
		for (b = ofirst_obj (c); b != SET_END; b = onext_obj (c))
		{
			if (iscontin (b, c))
			{
				destroy(b);
			}
		}
		setoloc(OBJ_META_LIQUID_HUMAN, OBJ_META_CAULDRON, IN_CONTAINER);
		return True;
	}

	if ( (c == OBJ_META_CAULDRON)
	&&   (a == OBJ_META_SCALE)
	&&   (iscontin(OBJ_META_ALKALINE,c))
	&&   (iscontin(OBJ_META_DUNG_OLD,c))
	&&   (iscontin(OBJ_META_SILVER_FEATHER,c))
	&&   (iscontin(OBJ_META_DIAMOND,c))
	&&   (otstbit(OBJ_META_KINDLING, OFL_LIT))
	    )
	{
		destroy(a);
		bprintf(
"As you add the &+Ygolden &+Cscale to the cauldron, the ingredients\n"
"shimmer and merge into a deep &+rcrimson&* liquid which gathers in the\n"
"bottom of the cauldron. A &+Cvial&* appears in the cauldron, and the\n"
"liquid flows into it. Clearly powerful magic is at work here.\n"
				);
		for (b = ofirst_obj (c); b != SET_END; b = onext_obj (c))
		{
			if (iscontin (b, c))
			{
				destroy(b);
			}
		}
		setoloc(OBJ_META_LIQUID_HUMAN, OBJ_META_CAULDRON, IN_CONTAINER);
		return True;
	}

	if ( (c == OBJ_META_CAULDRON)
	&&   (a == OBJ_META_DUNG_OLD)
	&&   (iscontin(OBJ_META_SCALE,c))
	&&   (iscontin(OBJ_META_ALKALINE,c))
	&&   (iscontin(OBJ_META_SILVER_FEATHER,c))
	&&   (iscontin(OBJ_META_DIAMOND,c))
	&&   (otstbit(OBJ_META_KINDLING, OFL_LIT))
	    )
	{
		destroy(a);
		bprintf(
"As you add the &+Wancient &+Cdung&* to the cauldron, the ingredients\n"
"shimmer and merge into a deep &+rcrimson&* liquid which gathers in the\n"
"bottom of the cauldron. A &+Cvial&* appears in the cauldron, and the\n"
"liquid flows into it. Clearly powerful magic is at work here.\n"
				);
		for (b = ofirst_obj (c); b != SET_END; b = onext_obj (c))
		{
			if (iscontin (b, c))
			{
				destroy(b);
			}
		}
		setoloc(OBJ_META_LIQUID_HUMAN, OBJ_META_CAULDRON, IN_CONTAINER);
		return True;
	}

	if ( (c == OBJ_META_CAULDRON)
	&&   (a == OBJ_META_SILVER_FEATHER)
	&&   (iscontin(OBJ_META_SCALE,c))
	&&   (iscontin(OBJ_META_DUNG_OLD,c))
	&&   (iscontin(OBJ_META_ALKALINE,c))
	&&   (iscontin(OBJ_META_DIAMOND,c))
	&&   (otstbit(OBJ_META_KINDLING, OFL_LIT))
	    )
	{
		destroy(a);
		bprintf(
"As you add the &+Wsilver &+Cfeather&* to the cauldron, the ingredients\n"
"shimmer and merge into a deep &+rcrimson&* liquid which gathers in the\n"
"bottom of the cauldron. A &+Cvial&* appears in the cauldron, and the\n"
"liquid flows into it. Clearly powerful magic is at work here.\n"
				);
		for (b = ofirst_obj (c); b != SET_END; b = onext_obj (c))
		{
			if (iscontin (b, c))
			{
				destroy(b);
			}
		}
		setoloc(OBJ_META_LIQUID_HUMAN, OBJ_META_CAULDRON, IN_CONTAINER);
		return True;
	}

	if ( (c == OBJ_META_CAULDRON)
	&&   (a == OBJ_META_DIAMOND)
	&&   (iscontin(OBJ_META_SCALE,c))
	&&   (iscontin(OBJ_META_DUNG_OLD,c))
	&&   (iscontin(OBJ_META_SILVER_FEATHER,c))
	&&   (iscontin(OBJ_META_ALKALINE,c))
	&&   (otstbit(OBJ_META_KINDLING, OFL_LIT))
	    )
	{
		destroy(a);
		bprintf(
"As you add the &+Menormous &+Cdiamond&* to the cauldron, the ingredients\n"
"shimmer and merge into a deep &+rcrimson&* liquid which gathers in the\n"
"bottom of the cauldron.\n"
				);
		for (b = ofirst_obj (c); b != SET_END; b = onext_obj (c))
		{
			if (iscontin (b, c))
			{
				destroy(b);
			}
		}
		setoloc(OBJ_META_LIQUID_HUMAN, OBJ_META_CAULDRON, IN_CONTAINER);
		return True;
	}


	if ( (c == OBJ_META_BOTTLE)
	&&   (a == OBJ_META_LIMESTONE)
	&&   ( (iscontin(OBJ_META_WATER, c))
	||     (iscontin(OBJ_META_WATER_2, c))
	||     (iscontin(OBJ_META_WATER_3, c))
	||     (iscontin(OBJ_META_WATER_4, c))
		)
	    )
	{
		bprintf(
"You add the &+Climestone&* to the bottle of &+Bwater&* and watch as it dissolves\n"
"away to nothingness, leaving you with an alkaline solution.\n"
				);
		destroy(a);
		for (b=ofirst_obj(c);b!=SET_END;b=onext_obj(c)) 
		{
    			if (iscontin (b, c))
			{
				destroy(b);
			}
		}
		setoloc(OBJ_META_ALKALINE, OBJ_META_BOTTLE, IN_CONTAINER);
		return True;
	}

	if ( (c == OBJ_META_BOTTLE)
	&&   ( (a == OBJ_META_WATER)
	||     (a == OBJ_META_WATER_2)
	||     (a == OBJ_META_WATER_3)
	||     (a == OBJ_META_WATER_4)
	      )
	&&   (iscontin(OBJ_META_LIMESTONE, OBJ_META_BOTTLE))
	    )
	{
		bprintf(
"You add the &+Cwater&* into the bottle with the &+Climestone&* and watch as the\n"
"limestone dissolves to nothingness, leaving you with an alkaline solution.\n"
				);
		destroy(a);
		for (b=ofirst_obj(c);b!=SET_END;b=onext_obj(c)) 
		{
    			if (iscontin (b, c))
			{
				destroy(b);
			}
		}
		setoloc(OBJ_META_ALKALINE, OBJ_META_BOTTLE, IN_CONTAINER);
		return True;
	}

	if ( (c == OBJ_META_BOTTLE)
	&&   (a == OBJ_META_VINE)
	&&   (!iscontin(OBJ_META_GUNPOWDER, c))
	    )
	{
		bprintf(
"That would be pointless with nothing &+Rexplosive&* in the &+Cbottle&*.\n"
				);
		return True;
	}

	if ( (c == OBJ_META_BOTTLE)
	&&   (a == OBJ_META_VINE)
	&&   (iscontin(OBJ_META_GUNPOWDER, c))
	    )
	{
		bprintf("You attach the &+Cvine&* to the bottle, making yourself a bomb.\n");
		destroy(c);
		destroy(a);
		setoloc(OBJ_META_BOMB, mynum, CARRIED_BY);
		return True;
	}

	if ( (c == OBJ_META_MECHANISM)
	&&   (a == OBJ_META_STONE_BRANCH)
	    )
	{
		bprintf(
"You attach the &+Cstone branch&* to the &+Cmechanism&*.\n"
			);
		setoloc(a, ploc(mynum), IN_ROOM);
		destroy(c);
		setoloc(OBJ_META_MECHANISM_LEVER, ploc(mynum), IN_ROOM);
		osetbit(a, OFL_NOGET);
		setobjstate(a, 1);
		return True;
	}

	if ( (c == OBJ_META_MECHANISM)
	&&   (a == OBJ_META_BRANCH)
	&&   (otstbit(a, OFL_LIT))
	    )
	{
		bprintf(
"The &+Cbranch&* is &+Ylit&*!\n"
			);
		return True;
	}

	if ( (c == OBJ_META_MECHANISM)
	&&   (a == OBJ_META_BRANCH)
	&&   (!otstbit(a, OFL_LIT))
	    )
	{
		bprintf(
"You attach the &+Cbranch&* to the &+Cmechanism&*.\n"
			);
		destroy(a);
		setoloc(OBJ_META_BRANCH_MECHANISM, ploc(mynum), IN_ROOM);
		destroy(c);
		setoloc(OBJ_META_MECHANISM_LEVER, ploc(mynum), IN_ROOM);
		return True;
	}

	if ( (c == OBJ_META_HOLE_VOLES)
	&&   ( ( (a == OBJ_META_PEBBLE3)
	&&       (state(a)==0)
	        )
	||     ( (a == OBJ_META_PEBBLE1)
	||       (a == OBJ_META_PEBBLE2)
	        )
	      )
	    )
	{
		bprintf(
"You place the &+Cpebble&* in the hole, blocking it up.\n"
"\n"
"You hear a scrabbling behind the &+Cpebble&*, and suddenly a &+Rvole&*\n"
"knocks the pebble back out of the hole. It runs into the room and pounces\n"
"on you, ripping you to shreds in moments.\n"
			);
    send_msg (DEST_ALL, MODE_QUIET, max (pvis (mynum), LVL_WIZARD), LVL_MAX,mynum,NOBODY,
"&+B[&+RTorn to Shreds&+B]\n");

		p_crapup(mynum,
"&+REaten by a vole!\n", CRAP_SAVE
			);
		return True;
	}

	if ( (c == OBJ_META_HOLE_VOLES)
	&&   (a == OBJ_META_PEBBLE3)
	&&   (state(a)==1)
	    )
	{
		bprintf(
"You place the &+Cpebble&* in the hole, blocking it up.\n"
"\n"
"You hear a scrabbling behind the &+Cpebble&*, and something begins to batter\n"
"against the pebble. Unable to withstand the strain, the &+Ccobwebs&* give way\n"
"and a &+Rvole&* knocks the pebble back out of the hole. It runs into the room and\n"
"pounces on you, ripping you to shreds in moments.\n"
                        );
    send_msg (DEST_ALL, MODE_QUIET, max (pvis (mynum), LVL_WIZARD), LVL_MAX,mynum,NOBODY,
"&+B[&+RTorn to Shreds&+B]\n");
                p_crapup(mynum,
"&+REaten by a vole!\n", CRAP_SAVE
                        );
                return True;
        }

        if ( (c == OBJ_META_HOLE_VOLES)
        &&   (a == OBJ_META_PEBBLE3)
        &&   (state(a)==2)
            )
        {
                bprintf(
"You place the &+Cpebble&* in the hole, blocking it up.\n"
"\n"
"You hear a scrabbling behind the &+Cpebble&*, and something begins to batter\n"
"against the pebble. The &+Csap&* is able to hold firm against the assailant though\n"
"and they give up after a short time.\n"
			);
		destroy(a);
		setobjstate(c, 1);
		return True;
	}

	if ( (c == OBJ_META_PEBBLE3)
	&&   (a == OBJ_META_COBWEB)
	&&   (state(c)==0)
	    )
	{
		bprintf(
"You coat the &+Cpebble&* with the sticky &+Ccobwebs&*.\n"
			);
		setobjstate(c, 1);
		destroy(a);
		return True;
	}

        if ( (c == OBJ_META_PEBBLE3)
        &&   (a == OBJ_META_COBWEB)
        &&   (state(c)==2)
            )
        {
                bprintf(
"The &+Cpebble&* is already coated in &+Csap&*, surely that's enough?\n"
			);
		return True;
	}

	if ( (c == OBJ_META_PEBBLE3)
	&&   (a == OBJ_META_SAP)
	&&   (state(c)==0)
	    )
	{
		bprintf(
"You coat the &+Cpebble&* with the sticky &+Csap&*.\n"
			);
		setobjstate(c, 2);
		destroy(a);
		return True;
	}

        if ( (c == OBJ_META_PEBBLE3)
        &&   (a == OBJ_META_SAP)
	&&   (state(c)==1)
            )
        {
                bprintf(
"The &+Cpebble&* already has &+Ccobwebs&* on it, surely that's enough?\n"
			);
		return True;
	}

	if ( (c == OBJ_META_BOTTLE)
	&&   (a == OBJ_META_BLUE_FLAME)
	&&   (!iscontin(OBJ_META_RED_FLAME, c))
	    )
	{
		bprintf(
"You gather the &+Cflame&* into the &+Cbottle&*.\n");
		setoloc(a, c, IN_CONTAINER);
		return True;
	}

	if ( (c == OBJ_META_BOTTLE)
	&&   (a == OBJ_META_BLUE_FLAME)
	&&   (iscontin(OBJ_META_RED_FLAME, c))
	    )
	{
		bprintf(
"You gather the &+Cflame&* into the &+Cbottle&*.\n"
"The &+Bblue&* flame and &+Rred&* flame react together, destroying each other.\n"
				);
		destroy(a);
		destroy(OBJ_META_RED_FLAME);
		return True;
	}

	if ( (c == OBJ_META_BOTTLE)
	&&   (a == OBJ_META_RED_FLAME)
	&&   (iscontin(OBJ_META_BLUE_FLAME, c))
	    )
	{
		bprintf(
"You gather the &+Cflame&* into the &+Cbottle&*.\n"
"The &+Bblue&* flame and &+Rred&* flame react together, destroying each other.\n"
				);
		destroy(a);
		destroy(OBJ_META_BLUE_FLAME);
		return True;
	}

	if ( (c != OBJ_META_BOTTLE)
	&&   (a == OBJ_META_BLUE_FLAME)
	&&   (otstbit(c, OFL_CONTAINER))
	&&   (state(c)==0)
	    )
	{
		bprintf("The &+Bflame&* burns the &+C%s&* away to ashes, destroying its contents.\n", oname(c));
		destroy(c);
		return True;
	}

	if ( (a == OBJ_META_BEAK_1)
	||   (a == OBJ_META_BEAK_2)
        ||   (a == OBJ_META_BEAK_3)
        ||   (a == OBJ_META_BEAK_4)
        ||   (a == OBJ_META_BEAK_5)
        ||   (a == OBJ_META_BEAK_6)
        ||   (a == OBJ_META_BEAK_7)
        ||   (a == OBJ_META_BEAK_8)
	    )
	{
		bprintf("You can't take your &+Rbeak&* off your face!\n");
		return True;
	}

        if ( (a == OBJ_META_TEETH_1)
        ||   (a == OBJ_META_TEETH_2)
        ||   (a == OBJ_META_TEETH_3)
        ||   (a == OBJ_META_TEETH_4)
        ||   (a == OBJ_META_TEETH_5)
        ||   (a == OBJ_META_TEETH_6)
        ||   (a == OBJ_META_TEETH_7)
        ||   (a == OBJ_META_TEETH_8)
            )
        {
                bprintf("You can't take your &+Rteeth&* out!\n");
                return True;
        }
return False;
}

void meta_timer_jump(Boolean jump_start, Boolean jump_stop)
{
	static int timestarted; 
	static Boolean running;
	int pl;	

        if ( (jump_start==True)
	    )
	{ 
	        running=True; 
                timestarted=global_clock;
                return;
	} 

        if ( (jump_stop==True) 
	    )
        { 
 	        running=False; 
		timestarted=0;
		return; 
	} 

	if (!running) return;


	if ( (global_clock-timestarted >= 4)
	&&   (global_clock-timestarted <=7)
	    )
	{
	for (pl=0; pl < max_players;pl++)
		{	
			if (pmetatype(pl)==META_FISH_UNDERWATER)
			{
				sendf(pl,
"You tumble back down to the bottom of the waterfall.\n\n"
					);
				trapch_plr(pl, LOC_META_META32);
				meta_timer_jump(False, True);
			}
		}
	}
}

Boolean meta_drop(int ob)
{
	if ( (ploc(mynum) == LOC_META_META53)
	&&   (pmetatype(mynum)==META_BIRD)
	    )
	{
		bprintf(
"A &+Chawk&* swoops down, warning you against littering the Council Chambers.\n"
			);
		return True;
	}

        if ( (ploc(mynum) == LOC_META_META53)
        &&   (pmetatype(mynum)!=META_BIRD)
            )
        {
                bprintf(
"You eye the &+Chawks&* circling above and decide against dropping anything here.\n"
                        );
                return True;
        }


	if ( ( (ob == OBJ_META_BEAK_1)
	||     (ob == OBJ_META_BEAK_2)
        ||     (ob == OBJ_META_BEAK_3)
        ||     (ob == OBJ_META_BEAK_4)
        ||     (ob == OBJ_META_BEAK_5)
        ||     (ob == OBJ_META_BEAK_6)
        ||     (ob == OBJ_META_BEAK_7)
        ||     (ob == OBJ_META_BEAK_8)
	      )
	&&   (pmetatype(mynum)==META_BIRD)
	    )
	{
		bprintf(
"You can't drop your &+Rbeak&*, it's attached to your face!\n"
			);
		return True;
	}

        if ( ( (ob == OBJ_META_TEETH_1)
        ||     (ob == OBJ_META_TEETH_2)
        ||     (ob == OBJ_META_TEETH_3)
        ||     (ob == OBJ_META_TEETH_4)
        ||     (ob == OBJ_META_TEETH_5)
        ||     (ob == OBJ_META_TEETH_6)
        ||     (ob == OBJ_META_TEETH_7)
        ||     (ob == OBJ_META_TEETH_8)
              )
        &&   ( (pmetatype(mynum)==META_SNAKE)
	||     (pmetatype(mynum)==META_SNAKE_UNDERWATER)
	      )
            )
        {
                bprintf(
"You can't drop your &+Rteeth&*, they're firmly attached to your gums!\n"
                        );
                return True;
        }

	if (ploc(mynum)==LOC_META_META73)
	{
		bprintf(
"You drop the &+C%s&* and it is carried away by the water and goes over the\n"
"waterfall.\n", oname(ob)
			);
		setoloc(ob, LOC_META_META32, IN_ROOM);
		return True;
	}

	if (ploc(mynum) == LOC_META_META17)
	{
		bprintf(
"You drop the &+C%s&*.\n"
"\n"
"The &+Cplanks&* on the floor creak slightly, then shatter under you,\n"
"plunging you into darkness, the whole structure crushing you to death\n"
"before you can even think.\n", oname(ob)
				);
    send_msg (DEST_ALL, MODE_QUIET, max (pvis (mynum), LVL_WIZARD),LVL_MAX,mynum,NOBODY,
"&+B[&+RCrushed to Death&+B]\n");
                                p_crapup(mynum,
"&+RThe storeroom caves in on you, crushing you instantly!\n", CRAP_SAVE
                        );
		return True;
	}

	if ( (ob == OBJ_META_CHEST)
	&&   (odamage(OBJ_META_CHEST)!=0)
	&&   (state(OBJ_META_CHEST)==1)
	&&   (ltstflg(ploc(mynum), LFL_IN_WATER))
	    )
	{
		bprintf(
"You drop the chest and it settles the right way up on the ground.\n");
		osetdamage(OBJ_META_CHEST, 0);
		setoloc(OBJ_META_CHEST, ploc(mynum), IN_ROOM);
		return True;
	}
	if ( (ob == OBJ_META_CHEST)
	&&   (odamage(OBJ_META_CHEST)!=0)
	&&   (state(OBJ_META_CHEST)==1)
	&&   (!ltstflg(ploc(mynum), LFL_IN_WATER))
	    )
	{
		bprintf(
"You drop the &+Cchest&* and it clatters to the ground, landing the right way up.\n");
		osetdamage(OBJ_META_CHEST, 0);
		setoloc(OBJ_META_CHEST, ploc(mynum), IN_ROOM);
		return True;
	}

        if ( (ob == OBJ_META_CHEST)
        &&   (odamage(OBJ_META_CHEST)!=0)
        &&   (state(OBJ_META_CHEST)==0)
	  &&   (ltstflg(ploc(mynum), LFL_IN_WATER))
	  &&   (!iscontin(OBJ_META_LIMESTONE, ob))
            )
        {
            bprintf(
"You drop the chest and it settles the right way up on the ground.\n"
"All the &+Bair&* in the chest escapes.\n"
				);
		destroy(ob);
            setoloc(OBJ_META_CHEST2, ploc(mynum), IN_ROOM);
            return True;
        }
        if ( (ob == OBJ_META_CHEST)
        &&   (odamage(OBJ_META_CHEST)!=0)
        &&   (state(OBJ_META_CHEST)==0)
	  &&   (ltstflg(ploc(mynum), LFL_IN_WATER))
	  &&   (iscontin(OBJ_META_LIMESTONE, ob))
            )
        {
            bprintf(
"You drop the chest and it settles the right way up on the ground.\n"
"All the &+Bair&* in the chest escapes.\n"
"The &+Climestone&* dissolves in the &+Bwater&*.\n"
				);
		destroy(ob);
		destroy(OBJ_META_LIMESTONE);
            setoloc(OBJ_META_CHEST2, ploc(mynum), IN_ROOM);
            return True;
        }

	if ( (ob == OBJ_META_PEBBLE1)
	&&   (ploc(mynum)==LOC_META_META51)
	    )
	{
		bprintf("You drop the &+Cpebble&* and it shatters on the ground below.\n");
		destroy(ob);
		setoloc(OBJ_META_BLUE_FLAME, LOC_META_META7, IN_ROOM);
		return True;
	}

	if ( (ob == OBJ_META_PEBBLE2)
	&&   (ploc(mynum)==LOC_META_META51)
	    )
	{
		bprintf("You drop the &+Cpebble&* and it shatters on the ground below.\n");
		destroy(ob);
		setoloc(OBJ_META_RED_FLAME, LOC_META_META7, IN_ROOM);
		return True;
	}

	if ( (ploc(mynum)==LOC_META_META51)
	&&   (ob != OBJ_META_PEBBLE1)
	&&   (ob != OBJ_META_PEBBLE2)
	    )
	{
		bprintf("You drop the &+C%s&* over the edge of the branches and it lands with a thud below.\n",
			oname(ob)
			);
		setoloc(ob, LOC_META_META7, IN_ROOM);
		return True;
	}

	return False;
}

Boolean meta_open(int obj)
{
	if ( (obj==OBJ_META_CHEST)
	&&   (odamage(OBJ_META_CHEST)==0)
	&&   (ocarrf(OBJ_META_CHEST)==CARRIED_BY)
	&&   (ltstflg(ploc(mynum), LFL_IN_WATER))
	&&   (!iscontin(OBJ_META_LIMESTONE, obj))
	    )
	{
		if (!iscontin(OBJ_META_BOMB, obj))
		{
		bprintf(
"With a rush, all the &+Bair&* trapped inside the chest escapes.\n"
			);
		}
		if (iscontin(OBJ_META_BOMB, obj))
		{
		bprintf(
"With a rush, all the &+Bair&* trapped inside the chest escapes.\n"
"&+BWater&* rushes in, &+Yextinguishing&* your &+Rbomb&*!\n"
			);
		setobjstate(OBJ_META_BOMB, 1);
		setoloc(OBJ_META_BOMB, OBJ_META_CHEST2, IN_CONTAINER);
		}
	setoloc(OBJ_META_CHEST, LOC_LIMBO_LIMBO, IN_ROOM);
	setoloc(OBJ_META_CHEST2, mynum, CARRIED_BY);
	return True;
	}

	if ( (obj==OBJ_META_CHEST)
	&&   (odamage(OBJ_META_CHEST)==0)
	&&   (ocarrf(OBJ_META_CHEST)==CARRIED_BY)
	&&   (ltstflg(ploc(mynum), LFL_IN_WATER))
	&&   (iscontin(OBJ_META_LIMESTONE, obj))
	    )
	{
		if (!iscontin(OBJ_META_BOMB, obj))
		{
		bprintf(
"With a rush, all the &+Bair&* trapped inside the chest escapes.\n"
"The &+Climestone&* dissolves in the &+Bwater&*.\n"
			);
		}
		if (iscontin(OBJ_META_BOMB, obj))
		{
			bprintf(
"With a rush, all the &+Bair&* trapped inside the chest escapes.\n"
"The &+Climestone&* dissolves in the &+Bwater&*.\n"
"&+BWater&* rushes in, &+Yextinguishing&* your &+Rbomb&*!\n"
					);
			setobjstate(OBJ_META_BOMB, 1);
			setoloc(OBJ_META_BOMB, OBJ_META_CHEST2, IN_CONTAINER);
		}
	setoloc(OBJ_META_CHEST, LOC_LIMBO_LIMBO, IN_ROOM);
	setoloc(OBJ_META_CHEST2, mynum, CARRIED_BY);
	destroy(OBJ_META_LIMESTONE);
	return True;
	}

        if ( (obj==OBJ_META_CHEST)
        &&   (ocarrf(OBJ_META_CHEST)==IN_ROOM)
        &&   (ltstflg(ploc(mynum), LFL_IN_WATER))
	      )
        {
		if (!iscontin(OBJ_META_LIMESTONE, obj))
		{
                	bprintf(
"With a rush, all the &+Bair&* trapped inside the chest escapes.\n"
                  	       );
	             	setoloc(OBJ_META_CHEST, LOC_LIMBO_LIMBO, IN_ROOM);
      	      	     	setoloc(OBJ_META_CHEST2, ploc(mynum), IN_ROOM);
		}

   	        if ( (iscontin(OBJ_META_LIMESTONE, obj))
		  && (!iscontin(OBJ_META_BOMB, obj))
	           )
	        {
			bprintf(
"With a rush, all the &+Bair&* trapped inside the chest escapes.\n"
"The &+Climestone&* dissolves in the &+Bwater&*.\n"
				);
			setoloc(OBJ_META_CHEST, LOC_LIMBO_LIMBO, IN_ROOM);
			setoloc(OBJ_META_CHEST2, mynum, CARRIED_BY);
			destroy(OBJ_META_LIMESTONE);
		}
		if ( (iscontin(OBJ_META_LIMESTONE, obj))
		  && (iscontin(OBJ_META_BOMB, obj))
			   )
		{
		bprintf(
"With a rush, all the &+Bair&* trapped inside the chest escapes.\n"
"The &+Climestone&* dissolves in the &+Bwater&*.\n"
"&+BWater&* rushes in a &+Yextinguishes&* your &+Rbomb&*!\n");
		setobjstate(OBJ_META_BOMB, 1);
		setoloc(OBJ_META_BOMB, OBJ_META_CHEST2, IN_CONTAINER);
		}
	setoloc(OBJ_META_CHEST, LOC_LIMBO_LIMBO, IN_ROOM);
	setoloc(OBJ_META_CHEST2, mynum, CARRIED_BY);
	destroy(OBJ_META_LIMESTONE);
        return True;
	}

        if ( (obj==OBJ_META_CHEST)
        &&   (odamage(OBJ_META_CHEST)!=0)
        &&   (ocarrf(OBJ_META_CHEST)==CARRIED_BY)
  	&&   (state(OBJ_META_CHEST)==1)
        &&   (ltstflg(ploc(mynum), LFL_IN_WATER))
           )
        {
                bprintf(
"You open the upside down chest, and manage to keep all the &+Bair&* in it.\n"
                        );
	        setobjstate(OBJ_META_CHEST, 0);
                return True;
        }

return False;
}

Boolean meta_turn(int obj)
{
	if ( (obj == OBJ_META_CHEST)
	&&   (odamage(OBJ_META_CHEST)==0)
	&&   (ltstflg(ploc(mynum), LFL_IN_WATER))
	&&   (iscarrby(obj, mynum))
	    )
	{
		bprintf("You turn the &+Cchest&* upside down.\n");
		osetdamage(OBJ_META_CHEST, mynum+1);
		return True;
	}

	if ( (obj == OBJ_META_CHEST)
	&&   (ocarrf(obj)==CARRIED_BY)
	&&   (odamage(OBJ_META_CHEST)!=0)
	&&   (ltstflg(ploc(mynum), LFL_IN_WATER))
	&&   (state(OBJ_META_CHEST)==0)
	&&   (iscarrby(obj, mynum))
	&&   (!iscontin(OBJ_META_LIMESTONE, obj))
	    )
	{
		bprintf(
"You turn the &+Cchest&* the right way up and the &+Bair&* escapes!\n"
			);
		setoloc(obj, LOC_LIMBO_LIMBO, IN_ROOM);
		setoloc(OBJ_META_CHEST2, mynum, CARRIED_BY);
		return True;
	}
	if ( (obj == OBJ_META_CHEST)
	&&   (ocarrf(obj)==CARRIED_BY)
	&&   (odamage(OBJ_META_CHEST)!=0)
	&&   (ltstflg(ploc(mynum), LFL_IN_WATER))
	&&   (state(OBJ_META_CHEST)==0)
	&&   (iscarrby(obj, mynum))
	&&   (iscontin(OBJ_META_LIMESTONE, obj))
	    )
	{
		bprintf(
"You turn the &+Cchest&* the right way up and the &+Bair&* escapes!\n"
"The &+Climestone&* dissolves in the &+Bwater&*.\n"
			);
		setoloc(obj, LOC_LIMBO_LIMBO, IN_ROOM);
		setoloc(OBJ_META_CHEST2, mynum, CARRIED_BY);
		destroy(OBJ_META_LIMESTONE);
		return True;
	}


        if ( (obj == OBJ_META_CHEST)
        &&   (ocarrf(obj)==IN_ROOM)
	  &&   (ltstflg(ploc(mynum), LFL_IN_WATER))
        &&   (odamage(OBJ_META_CHEST)!=0)
	  &&   (iscarrby(obj, mynum))
        &&   (state(OBJ_META_CHEST)==0)
	  &&   (!iscontin(OBJ_META_LIMESTONE, obj))
            )
        {
                bprintf(
"You turn the &+Cchest&* the right way up and the &+Bair&* escapes!\n"
                        );
                setoloc(obj, LOC_LIMBO_LIMBO, IN_ROOM);
                setoloc(OBJ_META_CHEST2, ploc(mynum), IN_ROOM);
                return True;
        }

        if ( (obj == OBJ_META_CHEST)
        &&   (ocarrf(obj)==IN_ROOM)
	  &&   (ltstflg(ploc(mynum), LFL_IN_WATER))
        &&   (odamage(OBJ_META_CHEST)!=0)
	  &&   (iscarrby(obj, mynum))
        &&   (state(OBJ_META_CHEST)==0)
	  &&   (iscontin(OBJ_META_LIMESTONE, obj))
            )
        {
                bprintf(
"You turn the &+Cchest&* the right way up and the &+Bair&* escapes!\n"
"The &+Climestone&* dissolves in the &+Bwater&*.\n"
                        );
                setoloc(obj, LOC_LIMBO_LIMBO, IN_ROOM);
                setoloc(OBJ_META_CHEST2, ploc(mynum), IN_ROOM);
		    destroy(OBJ_META_LIMESTONE);
                return True;
        }

        if ( (obj == OBJ_META_CHEST)
        &&   (odamage(OBJ_META_CHEST)!=0)
	  &&   (ltstflg(ploc(mynum), LFL_IN_WATER))
        &&   (state(OBJ_META_CHEST)==1)
	  &&   (iscarrby(obj, mynum))
            )
        {
                bprintf(
"You turn the &+Cchest&* the right way up!\n"
                        );
		osetdamage(OBJ_META_CHEST, 0);
                return True;
        }
return False;
}

Boolean meta_close(int obj)
{
	if ( (obj== OBJ_META_CHEST)
	&&   (state(OBJ_META_CHEST)==0)
	&&   (ltstflg(ploc(mynum), LFL_IN_WATER))
	    )
	{
		bprintf("You close the &+Cchest&*, managing to keep the &+Bair&* in.\n");
		setobjstate(OBJ_META_CHEST, 1);
		return True;
	}
return False;
}

Boolean meta_type(void)
{
int i;
	if (pl1 >= max_players)
	{
		bprintf("Mobiles don't have types.\n");
		return True;
	}	

	if ( (pl1 == -1)
	&&   (!EMPTY(item1))
	    )
	{
		bprintf("They aren't here.\n");
		return True;
	}

	if ( (pl1==mynum)
	&&   (EMPTY(item2))
	&&   (pmetatype(mynum)==META_HUMAN)
	    )
	{
		bprintf("You are a &+Chuman&*.\n");
		return True;
	}

        if ( (pl1==mynum)
	&&   (EMPTY(item2))
        &&   (pmetatype(mynum)==META_SNAKE)
	      )
        {
                bprintf("You are a &+Csnake&*.\n");
                return True;
        }

	if ( (pl1==mynum)
	&&   (EMPTY(item2))
	&&   (pmetatype(mynum)==META_SNAKE_UNDERWATER)
	    )
	{
		bprintf("You are a &+Csnake&* and &+Bunderwater&*.\n");
		return True;
	}

	if ( (pl1==mynum)
	&&   (EMPTY(item2))
	&&   ( (pmetatype(mynum)==META_FISH_ONLAND)
	||     (pmetatype(mynum)==META_FISH_UNDERWATER)
	      )
	    )
	{
		bprintf("You are a &+Cfish&*.\n");
		return True;
	}

        if ( (pl1==mynum)
	&&   (EMPTY(item2))
        &&   ( (pmetatype(mynum)==META_BIRD)
              )
            )
        {
                bprintf("You are a &+Cbird&*.\n");
                return True;
        }

	if ( (pl1==mynum)
	&&   (EMPTY(item2))
	&&   (pmetatype(mynum)==META_BEETLE)
	    )
	{
		bprintf("You are a &+Mbeetle&*.\n");
		return True;
	}

	if ( (pl1==mynum)
	&&   (EMPTY(item2))
	&&   (pmetatype(mynum)==META_BEETLE_UNDERWATER)
	    )
	{
		bprintf("You are a &+Cbeetle&* and &+Bunderwater&*.\n");
		return True;
	}

	if ( (pmetatype(pl1)==META_HUMAN)
	&&   (EMPTY(item2))
	    )
	{
		bprintf("%s is a &+Chuman&*.\n", pname(pl1));
		return True;
	}

	if ( (pmetatype(pl1)==META_SNAKE)
	&&   (EMPTY(item2))
	    )
	{
		bprintf("%s is a &+Csnake&*.\n", pname(pl1));
		return True;
	}
	if ( (pmetatype(pl1)==META_SNAKE_UNDERWATER)
	&&   (EMPTY(item2))
	    )
	{
		bprintf("%s is a &+Csnake&* and &+Bunderwater&*.\n", pname(pl1));
		return True;
	}

	if ( ( (pmetatype(pl1)==META_FISH_UNDERWATER)
	||     (pmetatype(pl1)==META_FISH_ONLAND)
	      )
	&&     (EMPTY(item2))
	    )
	{
		bprintf("%s is a &+Cfish&*.\n", pname(pl1));
		return True;
	}

	if ( (pmetatype(pl1)==META_BIRD)
	&&   (EMPTY(item2))
	    )
	{
		bprintf("%s is a &+Cbird&*.\n", pname(pl1));
		return True;
	}

	if ( (pmetatype(pl1)==META_BEETLE)
	&&   (EMPTY(item2))
	    )
	{
		bprintf("%s is a &+Cbeetle&*.\n", pname(pl1));
		return True;
	} 

	if ( (pmetatype(pl1)==META_BEETLE_UNDERWATER)
	&&   (EMPTY(item2))
	    )
	{
		bprintf("%s is a &+Cbeetle&* and &+Bunderwater&*.\n", pname(pl1));
		return True;
	} 

        if ( (pl1==mynum)
        &&   (!EMPTY(item2))
        &&   (pmetatype(mynum)!=META_HUMAN)
	&&   (!ltstflg(ploc(mynum), LFL_IN_WATER))
        &&   (strcmp(item2,"human")==0)
            )
        {
                bprintf("You turn yourself into a &+Yhuman&*.\n");
		dumpstuff(mynum, ploc(mynum));
                for (i=OBJ_META_MARKEROBJ1;i<=OBJ_META_MARKEROBJ3;i++)
                {
                        if ( (oloc(i)==ploc(mynum))
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, LOC_LIMBO_LIMBO, IN_ROOM);
                        }
                }
                setpmetatype(mynum, META_HUMAN);
		meta_timer_beetle(False,True);
		meta_timer_snake(False,True);
		meta_timer_become_fish(False,True);
                return True;
        }

        if ( (pl1==mynum)
        &&   (!EMPTY(item2))
        &&   (pmetatype(mynum)!=META_HUMAN)
        &&   (ltstflg(ploc(mynum), LFL_IN_WATER))
	&&   (carries_breather(mynum))
        &&   (strcmp(item2,"human")==0)
            )
        {
                bprintf("You turn yourself into a &+Yhuman&*.\n");
		dumpstuff(mynum, ploc(mynum));
                for (i=OBJ_META_MARKEROBJ1;i<=OBJ_META_MARKEROBJ3;i++)
                {
                        if ( (oloc(i)==ploc(mynum))
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, LOC_LIMBO_LIMBO, IN_ROOM);
                        }
                }
                setpmetatype(mynum, META_HUMAN);
                meta_timer_beetle(False,True);
                meta_timer_snake(False,True);
                meta_timer_become_fish(False,True);
                return True;
        }

        if ( (pl1==mynum)
        &&   (!EMPTY(item2))
        &&   (pmetatype(mynum)!=META_HUMAN)
        &&   (ltstflg(ploc(mynum), LFL_IN_WATER))
        &&   (!carries_breather(mynum))
        &&   (strcmp(item2,"human")==0)
            )
        {
                bprintf("You turn yourself into a &+Yhuman&*.\n");
		dumpstuff(mynum, ploc(mynum));
                for (i=OBJ_META_MARKEROBJ1;i<=OBJ_META_MARKEROBJ3;i++)
                {
                        if ( (oloc(i)==ploc(mynum))
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, LOC_LIMBO_LIMBO, IN_ROOM);
                        }
                }
                setpmetatype(mynum, META_HUMAN);
                meta_timer_beetle(False,True);
                meta_timer_snake(False,True);
                meta_timer_become_fish(False,True);
    send_msg (DEST_ALL, MODE_QUIET, max (pvis (mynum), LVL_WIZARD), LVL_MAX,mynum,NOBODY,
"&+B[&+RForgot how to breathe&+B]\n");
		p_crapup(mynum,
"&+RForgot how to breathe!\n", CRAP_SAVE
			);
                return True;
        }

        if ( (pl1==mynum)
        &&   (!EMPTY(item2))
        &&   (pmetatype(mynum)==META_HUMAN)
        &&   (strcmp(item2,"human")==0)
            )
        {
                bprintf("You are already &+Yhuman&*.\n");
                return True;
        }

	if ( (pl1==mynum)
	&&   (!EMPTY(item2))
	&&   (pmetatype(mynum)!=META_SNAKE)
	&&   (pmetatype(mynum)!=META_SNAKE_UNDERWATER)
	&&   (!ltstflg(ploc(mynum), LFL_IN_WATER))
	&&   (strcmp(item2,"snake")==0)
	    )
	{
		bprintf("You turn yourself into a &+Gsnake&*.\n");
                for (i=OBJ_META_MARKEROBJ1;i<=OBJ_META_MARKEROBJ2;i++)
                {
                        if ( (oloc(i)!=LOC_LIMBO_LIMBO)
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, LOC_LIMBO_LIMBO, IN_ROOM);
                        }
                }
		setpmetatype(mynum, META_SNAKE);
		meta_timer_beetle(False,True);
		meta_timer_snake(False,True);
		meta_timer_become_fish(False,True);
		dumpstuff(mynum, ploc(mynum));
                for (i=OBJ_META_MARKEROBJ2;i<=OBJ_META_MARKEROBJ3;i++)
                {
                        if ( (oloc(i)==LOC_LIMBO_LIMBO)
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, mynum, CARRIED_BY);
                                setpwpn(mynum, i);
				return True;
                        }
                }
		return True;
	}

	if ( (pl1==mynum)
	&&   (!EMPTY(item2))
	&&   (pmetatype(mynum)!=META_SNAKE)
	&&   (pmetatype(mynum)!=META_SNAKE_UNDERWATER)
	&&   (ltstflg(ploc(mynum), LFL_IN_WATER))
	&&   (strcmp(item2,"snake")==0)
	    )
	{
		bprintf("You turn yourself into a &+Gsnake&*.\n");
                for (i=OBJ_META_MARKEROBJ1;i<=OBJ_META_MARKEROBJ2;i++)
                {
                        if ( (oloc(i)!=LOC_LIMBO_LIMBO)
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, LOC_LIMBO_LIMBO, IN_ROOM);
                        }
                }
		setpmetatype(mynum, META_SNAKE_UNDERWATER);
		meta_timer_snake(True,False);
		meta_timer_beetle(False,True);
		meta_timer_become_fish(False,True);
		dumpstuff(mynum, ploc(mynum));
                for (i=OBJ_META_MARKEROBJ2;i<=OBJ_META_MARKEROBJ3;i++)
                {
                        if ( (oloc(i)==LOC_LIMBO_LIMBO)
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, mynum, CARRIED_BY);
                                setpwpn(mynum, i);
                                return True;
                        }
                }
		return True;
	}


        if ( (pl1==mynum)
        &&   (!EMPTY(item2))
	&&   ( (pmetatype(mynum)==META_SNAKE)
	||     (pmetatype(mynum)==META_SNAKE_UNDERWATER)
	      )
        &&   (strcmp(item2,"snake")==0)
            )
        {
                bprintf("You are already a &+Gsnake&*.\n");
                return True;
        }


	if ( (pl1==mynum)
	&&   (!EMPTY(item2))
	&&   (pmetatype(mynum)!=META_BIRD)
	&&   (strcmp(item2,"bird")==0)
	&&   (!ltstflg(ploc(mynum), LFL_IN_WATER))
	    )
	{
		bprintf("You turn yourself into a &+Cbird&*.\n");
		setpmetatype(mynum, META_BIRD);
		meta_timer_beetle(False,True);
		meta_timer_snake(False,True);
		meta_timer_become_fish(False,True);
		dumpstuff(mynum, ploc(mynum));
		for (i=OBJ_META_MARKEROBJ2;i<=OBJ_META_MARKEROBJ3;i++)
		{
			if ( (oloc(i)==ploc(mynum))
			&&   (!otstbit(i, OFL_NOGET))
			    )
			{
				setoloc(i, LOC_LIMBO_LIMBO, IN_ROOM);
			}
		}
                for (i=OBJ_META_MARKEROBJ1;i<=OBJ_META_MARKEROBJ2;i++)
                {
                        if ( (oloc(i)==LOC_LIMBO_LIMBO)
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, mynum, CARRIED_BY);
                                setpwpn (mynum, i);
				return True;
                        }
                }
		return True;
	}


        if ( (pl1==mynum)
        &&   (!EMPTY(item2))
        &&   (pmetatype(mynum)!=META_BIRD)
        &&   (strcmp(item2,"bird")==0)
        &&   (ltstflg(ploc(mynum), LFL_IN_WATER))
            )
        {
                bprintf("You turn yourself into a &+Cbird&*.\n");
                setpmetatype(mynum, META_BIRD);
                meta_timer_beetle(False,True);
                meta_timer_snake(False,True);
                meta_timer_become_fish(False,True);
    send_msg (DEST_ALL, MODE_QUIET, max (pvis (mynum), LVL_WIZARD), LVL_MAX,mynum,NOBODY,
"&+B[&+RForgot how to breathe&+B]\n");
		p_crapup(mynum,
"&+RForgot how to breathe!\n", CRAP_SAVE
			); 
                return True;
        }

        if ( (pl1==mynum)
        &&   (!EMPTY(item2))
        &&   (pmetatype(mynum)==META_BIRD)
        &&   (strcmp(item2,"bird")==0)
            )
        {
                bprintf("You are already a &+Cbird&*.\n");
                return True;
        }

	if ( (pl1==mynum)
	&&   (!EMPTY(item2))
	&&   (pmetatype(mynum)!=META_BEETLE)
	&&   (pmetatype(mynum)!=META_BEETLE_UNDERWATER)
	&&   (!ltstflg(ploc(mynum), LFL_IN_WATER))
	&&   (strcmp(item2,"beetle")==0)
	    )
	{
		bprintf("You turn yourself into a &+Mbeetle&*.\n");
		dumpstuff(mynum, ploc(mynum));
                for (i=OBJ_META_MARKEROBJ1;i<=OBJ_META_MARKEROBJ3;i++)
                {
                        if ( (oloc(i)==ploc(mynum))
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, LOC_LIMBO_LIMBO, IN_ROOM);
                        }
                }
		setpmetatype(mynum, META_BEETLE);
		meta_timer_beetle(False,True);
		meta_timer_snake(False,True);
		meta_timer_become_fish(False,True);
		return True;
	}

	if ( (pl1==mynum)
	&&   (!EMPTY(item2))
	&&   (pmetatype(mynum)!=META_BEETLE)
	&&   (pmetatype(mynum)!=META_BEETLE_UNDERWATER)
	&&   (strcmp(item2,"beetle")==0)
	&&   (ltstflg(ploc(mynum), LFL_IN_WATER))
	    )
	{
		bprintf("You turn yourself into a &+Mbeetle&*.\n");
		dumpstuff(mynum, ploc(mynum));
                for (i=OBJ_META_MARKEROBJ1;i<=OBJ_META_MARKEROBJ3;i++)
                {
                        if ( (oloc(i)==ploc(mynum))
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, LOC_LIMBO_LIMBO, IN_ROOM);
                        }
                }
		setpmetatype(mynum, META_BEETLE_UNDERWATER);
		meta_timer_beetle(True,False);
		meta_timer_snake(False,True);
		meta_timer_become_fish(False,True);
		return True;
	}

        if ( (pl1==mynum)
        &&   (!EMPTY(item2))
        &&   ( (pmetatype(mynum)==META_BEETLE)
        ||     (pmetatype(mynum)==META_BEETLE_UNDERWATER)
	      )
        &&   (strcmp(item2,"beetle")==0)
            )
        {
                bprintf("You are already a &+Mbeetle&*.\n");
                return True;
        }


	if ( (pl1==mynum)
	&&   (!EMPTY(item2))
	&&   (strcmp(item2,"fish")==0)
	&&   (pmetatype(mynum)!=META_FISH_ONLAND)
	&&   (pmetatype(mynum)!=META_FISH_UNDERWATER)
	&&   (ltstflg(ploc(mynum), LFL_IN_WATER))
	    )
	{
		bprintf("You turn yourself into a &+Bfish&*.\n");
		dumpstuff(mynum, ploc(mynum));
                for (i=OBJ_META_MARKEROBJ1;i<=OBJ_META_MARKEROBJ3;i++)
                {
                        if ( (oloc(i)==ploc(mynum))
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, LOC_LIMBO_LIMBO, IN_ROOM);
                        }
                }
		setpmetatype(mynum, META_FISH_UNDERWATER);
		meta_timer_beetle(False,True);
		meta_timer_snake(False,True);
		meta_timer_become_fish(False,True);
		return True;
	}

	if ( (pl1==mynum)
	&&   (!EMPTY(item2))
	&&   (strcmp(item2,"fish")==0)
	&&   (pmetatype(mynum)!=META_FISH_ONLAND)
	&&   (pmetatype(mynum)!=META_FISH_UNDERWATER)
	&&   (!ltstflg(ploc(mynum), LFL_IN_WATER))
	    )
	{
		bprintf("You turn yourself into a &+Bfish&*.\n");
		dumpstuff(mynum, ploc(mynum));
                for (i=OBJ_META_MARKEROBJ1;i<=OBJ_META_MARKEROBJ3;i++)
                {
                        if ( (oloc(i)==ploc(mynum))
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, LOC_LIMBO_LIMBO, IN_ROOM);
                        }
                }
		setpmetatype(mynum, META_FISH_ONLAND);
		meta_timer_become_fish(True,False);
		meta_timer_beetle(False,True);
		meta_timer_snake(False,True);
		return True;
	}

        if ( (pl1==mynum)
        &&   (!EMPTY(item2))
        &&   (strcmp(item2,"fish")==0)
        &&   ( (pmetatype(mynum)==META_FISH_ONLAND)
        ||     (pmetatype(mynum)==META_FISH_UNDERWATER)
	      )
            )
        {
                bprintf("You are already a &+Bfish&*.\n");
                return True;
        }


	if ( (pl1!=mynum)
	&&   (!EMPTY(item2))
	&&   (strcmp(item2,"snake")==0)
	&&   (!ltstflg(ploc(pl1), LFL_IN_WATER))
	    )
	{
		bprintf("You turn &+C%s&* into a &+Gsnake&*.\n",pname(pl1));
                for (i=OBJ_META_MARKEROBJ1;i<=OBJ_META_MARKEROBJ2;i++)
                {
                        if ( (oloc(i)!=LOC_LIMBO_LIMBO)
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, LOC_LIMBO_LIMBO, IN_ROOM);
                        }
                }
		setpmetatype(pl1, META_SNAKE);
		sendf(pl1,"&+C%s &*turns you into a &+Gsnake&*.\n", pname(mynum));
		mudlog("TYPE: %s changed type on %s to snake.", pname(mynum), pname(pl1));
		meta_timer_beetle(False,True);
		meta_timer_snake(False,True);
		meta_timer_become_fish(False,True);
		dumpstuff(pl1, ploc(pl1));
                for (i=OBJ_META_MARKEROBJ2;i<=OBJ_META_MARKEROBJ3;i++)
                {
                        if ( (oloc(i)==LOC_LIMBO_LIMBO)
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, pl1, CARRIED_BY);
                                setpwpn(pl1, i);
                                return True;
                        }
                }
		return True;
	}

	if ( (pl1!=mynum)
	&&   (!EMPTY(item2))
	&&   (strcmp(item2,"snake")==0)
	&&   (ltstflg(ploc(pl1), LFL_IN_WATER))
	    )
	{
		bprintf("You turn &+C%s&* into a &+Gsnake&*.\n",pname(pl1));
                for (i=OBJ_META_MARKEROBJ1;i<=OBJ_META_MARKEROBJ2;i++)
                {
                        if ( (oloc(i)!=LOC_LIMBO_LIMBO)
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, LOC_LIMBO_LIMBO, IN_ROOM);
                        }
                }
		setpmetatype(pl1, META_SNAKE_UNDERWATER);
		sendf(pl1,"&+C%s &*turns you into a &+Gsnake&*.\n", pname(mynum));
		mudlog("TYPE: %s changed type on %s to snake. %s was &+Bunderwater&*.", pname(mynum), pname(pl1), pname(pl1));
		meta_timer_snake(True,False);
		meta_timer_beetle(False,True);
		meta_timer_become_fish(False,True);
		dumpstuff(pl1, ploc(pl1));
                for (i=OBJ_META_MARKEROBJ2;i<=OBJ_META_MARKEROBJ3;i++)
                {
                        if ( (oloc(i)==LOC_LIMBO_LIMBO)
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, pl1, CARRIED_BY);
                                setpwpn(pl1, i);
                                return True;
                        }
                }
		return True;
	}

	if ( (pl1!=mynum)
	&&   (!EMPTY(item2))
	&&   (!ltstflg(ploc(pl1), LFL_IN_WATER))
	&&   (strcmp(item2,"bird")==0)
	    )
	{
		bprintf("You turn &+C%s&* into a &+Cbird&*.\n",pname(pl1));
		setpmetatype(pl1, META_BIRD);
		sendf(pl1,"&+C%s &*turns you into a &+Cbird&*.\n", pname(mynum));
		mudlog("TYPE: %s changed type on %s to bird.", pname(mynum), pname(pl1));
		meta_timer_beetle(False,True);
		meta_timer_snake(False,True);
		meta_timer_become_fish(False,True);
		dumpstuff(pl1, ploc(pl1));
                for (i=OBJ_META_MARKEROBJ2;i<=OBJ_META_MARKEROBJ3;i++)
                {
                        if ( (oloc(i)==ploc(pl1))
			&&   (!otstbit(i, OFL_NOGET))
			    )
			{
				setoloc(i, LOC_LIMBO_LIMBO, IN_ROOM);
			}
		}

                for (i=OBJ_META_MARKEROBJ1;i<=OBJ_META_MARKEROBJ2;i++)
                {
                        if ( (oloc(i)==LOC_LIMBO_LIMBO)
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, pl1, CARRIED_BY);
                                setpwpn (pl1, i);
				return True;
                        }
                }
		return True;
	}

        if ( (pl1!=mynum)
        &&   (!EMPTY(item2))
        &&   (ltstflg(ploc(pl1), LFL_IN_WATER))
        &&   (strcmp(item2,"bird")==0)
            )
        {
                bprintf("You turn &+C%s&* into a &+Cbird&*.\n",pname(pl1));
                setpmetatype(pl1, META_BIRD);
                sendf(pl1,"&+C%s &*turns you into a &+Cbird&*.\n", pname(mynum));
                mudlog("TYPE: %s changed type on %s to bird. %s was &+Bunderwater.",
pname(mynum), pname(pl1), pname(pl1));
                meta_timer_beetle(False,True);
                meta_timer_snake(False,True);
                meta_timer_become_fish(False,True);
    send_msg (DEST_ALL, MODE_QUIET, max (pvis (pl1), LVL_WIZARD), LVL_MAX,pl1,NOBODY,
"&+B[&+RForgot how to breathe&+B]\n");

		p_crapup(pl1,
"&+RForgot how to breathe!\n", CRAP_SAVE
			);
                return True;
        }
        if ( (pl1!=mynum)
        &&   (!EMPTY(item2))
        &&   (!ltstflg(ploc(pl1), LFL_IN_WATER))
        &&   (strcmp(item2,"human")==0)
            )
        {
                bprintf("You turn &+C%s&* into a &+Yhuman&*.\n",pname(pl1));
		dumpstuff(pl1, ploc(pl1));
                for (i=OBJ_META_MARKEROBJ1;i<=OBJ_META_MARKEROBJ3;i++)
                {
                        if ( (oloc(i)==ploc(pl1))
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, LOC_LIMBO_LIMBO, IN_ROOM);
                        }
                }
                setpmetatype(pl1, META_HUMAN);
                sendf(pl1,"&+C%s &*turns you into a &+Yhuman&*.\n", pname(mynum));
                mudlog("TYPE: %s changed type on %s to human.", pname(mynum),
pname(pl1));
                meta_timer_beetle(False,True);
                meta_timer_snake(False,True);
                meta_timer_become_fish(False,True);
                return True;
        }

        if ( (pl1!=mynum)
        &&   (!EMPTY(item2))
        &&   (ltstflg(ploc(pl1), LFL_IN_WATER))
	&&   (!carries_breather(pl1))
        &&   (strcmp(item2,"human")==0)
            )
        {
                bprintf("You turn &+C%s&* into a &+Yhuman&*.\n",pname(pl1));
                setpmetatype(pl1, META_HUMAN);
                sendf(pl1,"&+C%s &*turns you into a &+Yhuman&*.\n", pname(mynum));
                mudlog("TYPE&*: %s changed type on %s to human&*. %s was
underwater and had no breather.",
pname(mynum), pname(pl1), pname(pl1));
                meta_timer_beetle(False,True);
                meta_timer_snake(False,True);
                meta_timer_become_fish(False,True);
    send_msg (DEST_ALL, MODE_QUIET, max (pvis (pl1), LVL_WIZARD), LVL_MAX,pl1,NOBODY,
"&+B[&+RForgot how to breathe&+B]\n");
                p_crapup(pl1, "&+RForgot how to breathe!\n", CRAP_SAVE);
                return True;
        }
        if ( (pl1!=mynum)
        &&   (!EMPTY(item2))
        &&   (ltstflg(ploc(pl1), LFL_IN_WATER))
        &&   (carries_breather(pl1))
        &&   (strcmp(item2,"human")==0)
            )
        {
                bprintf("You turn &+C%s&* into a &+Yhuman&*.\n",pname(pl1));
                dumpstuff(pl1, ploc(pl1));
                for (i=OBJ_META_MARKEROBJ1;i<=OBJ_META_MARKEROBJ3;i++)
                {
                        if ( (oloc(i)==ploc(pl1))
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, LOC_LIMBO_LIMBO, IN_ROOM);
                        }
                }
                setpmetatype(pl1, META_HUMAN);
                sendf(pl1,"&+C%s &*turns you into a &+Yhuman&*.\n", pname(mynum));
                mudlog("TYPE: %s changed type on %s to human. %s was
underwater and had a breather.",
pname(mynum), pname(pl1), pname(pl1));
                meta_timer_beetle(False,True);
                meta_timer_snake(False,True);
                meta_timer_become_fish(False,True);
                return True;
        }


	if ( (pl1!=mynum)
	&&   (!EMPTY(item2))
	&&   (!ltstflg(ploc(pl1), LFL_IN_WATER))
	&&   (strcmp(item2,"beetle")==0)
	    )
	{
		bprintf("You turn &+C%s&* into a &+Mbeetle&*.\n",pname(pl1));
                dumpstuff(pl1, ploc(pl1));
                for (i=OBJ_META_MARKEROBJ1;i<=OBJ_META_MARKEROBJ3;i++)
                {
                        if ( (oloc(i)==ploc(pl1))
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, LOC_LIMBO_LIMBO, IN_ROOM);
                        }
                }
		setpmetatype(pl1, META_BEETLE);
		sendf(pl1,"&+C%s &*turns you into a &+Mbeetle&*.\n", pname(mynum));
		mudlog("TYPE: %s changed type on %s to beetle.", pname(mynum), pname(pl1));
		meta_timer_beetle(False,True);
		meta_timer_snake(False,True);
		meta_timer_become_fish(False,True);
		dumpstuff(pl1, ploc(pl1));
		return True;
	}	

	if ( (pl1!=mynum)
	&&   (!EMPTY(item2))
	&&   (ltstflg(ploc(pl1), LFL_IN_WATER))
	&&   (strcmp(item2,"beetle")==0)
	    )
	{
		bprintf("You turn &+C%s&* into a &+Mbeetle&*.\n",pname(pl1));
                dumpstuff(pl1, ploc(pl1));
                for (i=OBJ_META_MARKEROBJ1;i<=OBJ_META_MARKEROBJ3;i++)
                {
                        if ( (oloc(i)==ploc(pl1))
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, LOC_LIMBO_LIMBO, IN_ROOM);
                        }
                }
		setpmetatype(pl1, META_BEETLE_UNDERWATER);
		sendf(pl1,"&+C%s &*turns you into a &+Mbeetle&*.\n", pname(mynum));
		mudlog("TYPE: %s changed type on %s to beetle. %s was underwater.", pname(mynum), pname(pl1), pname(pl1));
		meta_timer_beetle(True,False);
		meta_timer_snake(False,True);
		meta_timer_become_fish(False,True);
		dumpstuff(pl1, ploc(pl1));
		return True;
	}	

	if ( (pl1!=mynum)
	&&   (!EMPTY(item2))
	&&   (strcmp(item2,"fish")==0)
	&&   (ltstflg(ploc(pl1), LFL_IN_WATER))
	    )
	{
		bprintf("You turn &+C%s&* into a &+Bfish&*.\n",pname(pl1));
                dumpstuff(pl1, ploc(pl1));
                for (i=OBJ_META_MARKEROBJ1;i<=OBJ_META_MARKEROBJ3;i++)
                {
                        if ( (oloc(i)==ploc(pl1))
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, LOC_LIMBO_LIMBO, IN_ROOM);
                        }
                }
		setpmetatype(pl1, META_FISH_UNDERWATER);
		sendf(pl1,"&+C%s &*turns you into a &+Bfish&*.\n", pname(mynum));
		mudlog("TYPE: %s changed type on %s to fish.", pname(mynum), pname(pl1));
		meta_timer_beetle(False,True);
		meta_timer_snake(False,True);
		meta_timer_become_fish(False,True);
		dumpstuff(pl1, ploc(pl1));
		return True;
	}


	if ( (pl1!=mynum)
	&&   (!EMPTY(item2))
	&&   (strcmp(item2,"fish")==0)
	&&   (!ltstflg(ploc(pl1), LFL_IN_WATER))
	    )
	{
		bprintf("You turn &+C%s&* into a &+Bfish&*.\n",pname(pl1));
                dumpstuff(pl1, ploc(pl1));
                for (i=OBJ_META_MARKEROBJ1;i<=OBJ_META_MARKEROBJ3;i++)
                {
                        if ( (oloc(i)==ploc(pl1))
                        &&   (!otstbit(i, OFL_NOGET))
                            )
                        {
                                setoloc(i, LOC_LIMBO_LIMBO, IN_ROOM);
                        }
                }
		setpmetatype(pl1, META_FISH_ONLAND);
		sendf(pl1,"&+C%s &*turns you into a &+Bfish&*.\n", pname(mynum));
		mudlog("TYPE: %s changed type on %s to fish. %s was on land.", pname(mynum), pname(pl1), pname(pl1));
		meta_timer_become_fish(True,False);
		meta_timer_beetle(False,True);
		meta_timer_snake(False,True);
		return True;
	}

        if ( (pl1!=mynum)
        &&   (!EMPTY(item2))
        &&   (pmetatype(pl1)==META_HUMAN)
        &&   (strcmp(item2,"human")==0)
            )
        {
                bprintf("%s is already &+Yhuman&*.\n", pname(pl1));
                return True;
        }

        if ( (pl1!=mynum)
        &&   (!EMPTY(item2))
        &&   (pmetatype(pl1)==META_BIRD)
        &&   (strcmp(item2,"bird")==0)
            )
        {
                bprintf("%s is already a &+Cbird&*.\n", pname(pl1));
                return True;
        }

        if ( (pl1!=mynum)
        &&   (!EMPTY(item2))
        &&   ( (pmetatype(pl1)==META_SNAKE)
	||     (pmetatype(pl1)==META_SNAKE_UNDERWATER)
	      )
        &&   (strcmp(item2,"snake")==0)
            )
        {
                bprintf("%s is already a &+Gsnake&*.\n", pname(pl1));
                return True;
        }

        if ( (pl1!=mynum)
        &&   (!EMPTY(item2))
        &&   ( (pmetatype(pl1)==META_BEETLE)
	||     (pmetatype(pl1)==META_BEETLE_UNDERWATER)
	      )
        &&   (strcmp(item2,"beetle")==0)
            )
        {
                bprintf("%s is already a &+Mbeetle&*.\n", pname(pl1));
                return True;
        }

        if ( (pl1!=mynum)
        &&   (!EMPTY(item2))
        &&   ( (pmetatype(pl1)==META_FISH_ONLAND)
	||     (pmetatype(pl1)==META_FISH_UNDERWATER)
	      )
        &&   (strcmp(item2,"fish")==0)
            )
        {
                bprintf("%s is already a &+Bfish&*.\n", pname(pl1));
                return True;
        }

return False;
}

void meta_timer_snake(Boolean snake_start, Boolean snake_stop)
{
        static int timestarted;
        static Boolean running;
        int pl;

        if ( (snake_start==True)
            )
        {
                running=True;
                timestarted=global_clock;
                return;
        }

        if ( (snake_stop==True)
            )
        {
                running=False;
                timestarted=0;
                return;
        }

        if (!running) return;


        if ( (global_clock-timestarted == 180)
            )
        {
	        for (pl=0; pl < max_players;pl++)
                {
			if (pmetatype(pl)==META_SNAKE_UNDERWATER)
			{
				sendf(pl,
"You are beginning to struggle for &+Bair&*, you will need to surface soon.\n"
					);
			}
		}
	}

	if ( (global_clock-timestarted == 200)
	    )
	{
		for (pl=0; pl < max_players;pl++)
		{
			if (pmetatype(pl)==META_SNAKE_UNDERWATER)
			{
				sendf(pl,
"You run out of &+Bair&* and die in hideous underwater convulsions.\n"
					);
				p_crapup(pl,
"Ran out of &+Bair&*!\n",CRAP_SAVE
					);
				meta_timer_snake(False,True);
			}
		}
	}
}

void meta_timer_beetle(Boolean beetle_start, Boolean beetle_stop)
{
        static int timestarted;
        static Boolean running;
        int pl;

        if ( (beetle_start==True)
            )
        {
                running=True;
                timestarted=global_clock;
                return;
        }

        if ( (beetle_stop==True)
            )
        {
                running=False;
                timestarted=0;
                return;
        }

        if (!running) return;


        if ( (global_clock-timestarted >= 120)
        &&   (global_clock-timestarted <= 122)
            )
        {
	        for (pl=0; pl < max_players;pl++)
                {
			if (pmetatype(pl)==META_BEETLE_UNDERWATER)
			{
				sendf(pl,
"You are beginning to struggle for &+Bair&*, you will need to surface soon.\n"
					);
			}
		}
	}

	if ( (global_clock-timestarted >= 200)
	&&   (global_clock-timestarted <= 202)
	    )
	{
		for (pl=0; pl < max_players;pl++)
		{
			if (pmetatype(pl)==META_BEETLE_UNDERWATER)
			{
				sendf(pl,
"You run out of &+Bair&* and die in hideous underwater convulsions.\n"
					);
				p_crapup(pl,
"Ran out of &+Bair&*!\n",CRAP_SAVE
					);
				meta_timer_beetle(False, True);
			}
		}
	}
}

void meta_beetledie(void)
{
	int i;
	int b;
	for (b=0;b<max_players;b++)
	{
	 	for (i=0;i<=MOBMAX_META+max_players;i++)
		{
			if ( (btstflg(i,BFL_FISH))
			&&   (ploc(b)==ploc(i))
			&&   (pmetatype(b)==META_BEETLE_UNDERWATER)
			&&   (plev(b)<LVL_WIZARD)
	    	    	    )
		 	{
				sendf(b,
"You look around and are confronted by the beady eye of &+C%s&*.\n"
"%s swims towards you and devours you in one gulp. As you digest in %s\n"
"stomach you contemplate the wisdom of swimming around the fish kingdom\n"
"as a beetle.\n", pname(i), he_or_she(i), his_or_her(i)
					);
    send_msg (DEST_ALL, MODE_QUIET, max (pvis (b), LVL_WIZARD), LVL_MAX,b,NOBODY,
"&+B[&+REaten Alive&+B]\n");
                p_crapup(b, "&+REaten by a fish!\n", CRAP_SAVE);
			}

                        if ( (btstflg(i,BFL_FISH))
                        &&   (ploc(b)==ploc(i))
                        &&   (pmetatype(b)==META_BEETLE_UNDERWATER)
                        &&   (plev(b)>=LVL_WIZARD)
                            )
                        {
                                sendf(b,
"You look around and are confronted by the beady eye of &+C%s&*.\n"
"%s swims towards you and devours you in one gulp. As you digest in %s\n"
"stomach you contemplate the wisdom of swimming around the fish kingdom\n"
"as a beetle.\n", pname(i), he_or_she(i), his_or_her(i)
                                        );
				setpmetatype(b, META_BEETLE);
				meta_timer_beetle(False,True);
				trapch_plr(b, LOC_META_FISHSTOMACH);
                        }


                        if ( (btstflg(i,BFL_BIRD))
                        &&   (ploc(b)==ploc(i))
                        &&   (pmetatype(b)==META_BEETLE)
			&&   (plev(b)<LVL_WIZARD)
                            )
                        {
                                sendf(b,
"You look around and are confronted by the beady eye of &+C%s&*.\n"
"%s flies towards you and devours you in one gulp. As you digest in %s\n"
"stomach you contemplate the wisdom of wandering around the &+Gbird&* kingdom\n"
"as a beetle.\n", pname(i), he_or_she(i), his_or_her(i)
                                        );
    send_msg (DEST_ALL, MODE_QUIET, max (pvis (b), LVL_WIZARD), LVL_MAX,b,NOBODY,
"&+B[&+REaten Alive&+B]\n");
                p_crapup(b, "&+REaten by a bird!\n", CRAP_SAVE);

                        }


                        if ( (btstflg(i,BFL_BIRD))
                        &&   (ploc(b)==ploc(i))
                        &&   (pmetatype(b)==META_BEETLE)
                        &&   (plev(b)>=LVL_WIZARD)
                            )
                        {
                                sendf(b,
"You look around and are confronted by the beady eye of &+C%s&*.\n"
"%s flies towards you and devours you in one gulp. As you digest in %s\n"
"stomach you contemplate the wisdom of wandering around the &+Gbird&* kingdom\n"
"as a beetle.\n", pname(i), he_or_she(i), his_or_her(i)
                                        );
				setpmetatype(b, META_BEETLE);
				meta_timer_beetle(False, True);
				trapch_plr(b, LOC_META_BIRDSTOMACH);
                        }

			if ( (btstflg(i,BFL_FISH))
			&&   (ploc(b)==ploc(i))
			&&   (pmetatype(b)==META_SNAKE_UNDERWATER)
			    )
			{
				sendf(b,
"As you &+Bswim&* into the room, &+C%s&* takes one look at you and bolts in &+Rterror&*.\n",
pname(i)
					);
				setploc(i, LOC_LIMBO_LIMBO);
			}
			
			if ( (btstflg(i,BFL_BIRD))
			&&   (ploc(b)==ploc(i))
			&&   (pmetatype(b)==META_SNAKE)
			    )
			{
				sendf(b,
"As you &+Gslither&* into the room, &+C%s&* takes one look at you and bolts in &+Rterror&*.\n"
,pname(i)
					);
				setploc(i, LOC_LIMBO_LIMBO);
			}

		}
	}
}	

Boolean meta_peck(int obj)
{
	if (pmetatype(mynum)!=META_BIRD)
	{
		bprintf(
"You can't &+Cpeck&* if you're not a &+Mbird&*.\n"
			);
		return True;
	}

        if ( (pmetatype(mynum)==META_BIRD)
	&&   (obj == OBJ_META_TREE)
        &&   (otstbit(OBJ_META_SAP, OFL_DESTROYED))
            )
        {
                bprintf(
"You &+Rpeck&* at the &+Ctree&* with your beak, and before long some sap begins\n"
"running down the trunk.\n"
"\n"
"Seeing your vicious attack on the &+Ctree&*, the birds above swoop down and\n"
"angrily carry you off to jail.\n"
                        );
                oclrbit(OBJ_META_SAP, OFL_DESTROYED);
                setobjstate(OBJ_META_CELL_DOOR_OUT, 1);
                setobjstate(OBJ_META_CELL_DOOR_IN, 1);
                trapch(LOC_META_META52);
                meta_timer_jail(True,False);
                return True;
        }
return False;
}

void meta_smash(void)
{
        if ( (pmetatype(mynum)==META_BIRD)
        &&   (otstbit(OBJ_META_SAP, OFL_DESTROYED))
            )
        {
                bprintf(
"You &+Rpeck&* at the &+Ctree&* with your beak, and before long some sap begins\n"
"running down the trunk.\n"
"\n"
"Seeing your vicious attack on the &+Ctree&*, the birds above swoop down and\n"
"angrily carry you off to jail.\n"
                        );
                oclrbit(OBJ_META_SAP, OFL_DESTROYED);
                setobjstate(OBJ_META_CELL_DOOR_OUT, 1);
                setobjstate(OBJ_META_CELL_DOOR_IN, 1);
                trapch(LOC_META_META52);
		meta_timer_jail(True,False);
                return;
	}
	else
	{
		bprintf(
"You can't do that.\n"
			);
		return;
	}
}

Boolean meta_kill(int pl)
{
	if ( (pmetatype(mynum)!=META_HUMAN)
	&&   (pl!=MOB_META_SNAKE_BEGGAR+max_players)
	&&   (pl!=MOB_META_CHIMERA+max_players)
	&&   (pl!=MOB_META_COTTONTAIL+max_players)
	&&   (pl!=MOB_META_FLOPSY+max_players)
	&&   (pl!=MOB_META_MOPSY+max_players)
	    )
	{
		bprintf(
"You are too puny to offer much of a threat to anyone.\n"
			);
		return True;
	}

        if ( (pl == MOB_META_FLOPSY+max_players)
        &&   (pmetatype(mynum)==META_SNAKE)
        &&   (ploc(mynum)==ploc(MOB_META_FLOPSY+max_players))
	&&   (pnumobs(mynum)==1)
            )
        {
                bprintf(
"&+CFlopsy&* shrieks in &+Rterror&* and runs away, screaming for her siblings to follow."
                                );
                setploc(MOB_META_FLOPSY+max_players, LOC_LIMBO_LIMBO);
                setploc(MOB_META_MOPSY+max_players, LOC_LIMBO_LIMBO);
                setploc(MOB_META_COTTONTAIL+max_players, LOC_LIMBO_LIMBO);
                return True;
        }

        if ( ( (pl == MOB_META_FLOPSY+max_players)
	||     (pl == MOB_META_MOPSY+max_players)
	||     (pl == MOB_META_COTTONTAIL+max_players)
	      )
        &&   (pmetatype(mynum)==META_SNAKE)
        &&   (ploc(mynum)==ploc(pl))
        &&   (pnumobs(mynum)>1)
            )
        {
                bprintf("You cannot &+Rbite&* with something in your mouth.\n");
		return True;
	}

        if ( (pl == MOB_META_MOPSY+max_players)
        &&   (pmetatype(mynum)==META_SNAKE)
        &&   (ploc(mynum)==ploc(MOB_META_MOPSY+max_players))
	&&   (pnumobs(mynum)==1)
            )
        {
                bprintf(
"&+CMopsy&* shrieks in &+Rterror&* and runs away, screaming for her siblings to follow."
                                );
                setploc(MOB_META_FLOPSY+max_players, LOC_LIMBO_LIMBO);
                setploc(MOB_META_MOPSY+max_players, LOC_LIMBO_LIMBO);
                setploc(MOB_META_COTTONTAIL+max_players, LOC_LIMBO_LIMBO);
                return True;
        }
        if ( (pl == MOB_META_COTTONTAIL+max_players)
        &&   (pmetatype(mynum)==META_SNAKE)
        &&   (ploc(mynum)==ploc(MOB_META_COTTONTAIL+max_players))
	&&   (pnumobs(mynum)==1)
            )
        {
                bprintf(
"You &+Rbite&* down hard on &+CCottontail's&* neck, biting clean through her spine,\n"
"killing her instantly.\n"
                                );
                setploc(MOB_META_COTTONTAIL+max_players, LOC_LIMBO_LIMBO);
                setoloc(OBJ_META_COTTONTAIL, ploc(mynum), IN_ROOM);
                return True;
        }

	if ( (pmetatype(mynum)!=META_SNAKE)
	&&   (pmetatype(mynum)!=META_HUMAN)
	&&   ( (pl == MOB_META_FLOPSY+max_players)
	||     (pl == MOB_META_MOPSY+max_players)
	||     (pl == MOB_META_COTTONTAIL+max_players)
	      )
	    )
	{
		bprintf(
"You are too puny to offer much of a threat to anyone.\n"
                        );
                return True;
        }

	if ( (pmetatype(mynum)!=META_HUMAN)
	&&   (pl==MOB_META_CHIMERA+max_players)
	   )
	{
		bprintf(
"The &+CChimera&* laughs at you with impunity and crushes you with a single massive\n"
"hairy paw.\n"
			);
		    send_msg (DEST_ALL, MODE_QUIET, max (pvis (pl1), LVL_WIZARD), LVL_MAX,mynum,NOBODY,
"&+B[&+RBit Off More Than %s Could Swallow&+B]\n", he_or_she(mynum));
                p_crapup(mynum, "&+RCrushed by the Chimera!\n", CRAP_SAVE);
		return True;
	}

	if ( (pl==MOB_META_SNAKE_BEGGAR+max_players)
	&&   (pmetatype(mynum)!=META_BEETLE)
	&&   (pmetatype(mynum)!=META_FISH_ONLAND)
	&&   (iscarrby(OBJ_META_PEBBLE1, pl))
	&&   (iscarrby(OBJ_META_PEBBLE2, pl))
	&&   (iscarrby(OBJ_META_PEBBLE3, pl))
	    )
	{
		bprintf(
"As you go to attack the &+Cbeggar&* he screams in &+Rpanic&* and flees,\n"
"dropping one of his &+Cpebbles&* in his haste to get away.\n"
			);
		setploc(pl, LOC_LIMBO_LIMBO);
		setoloc(OBJ_META_PEBBLE1, ploc(mynum), IN_ROOM);
		return True;
	}

        if ( (pl==MOB_META_SNAKE_BEGGAR+max_players)
        &&   (pmetatype(mynum)!=META_BEETLE)
        &&   (pmetatype(mynum)!=META_FISH_ONLAND)
        &&   (!iscarrby(OBJ_META_PEBBLE1, pl))
        &&   (iscarrby(OBJ_META_PEBBLE2, pl))
        &&   (iscarrby(OBJ_META_PEBBLE3, pl))
            )
        {
                bprintf(
"As you move to attack the &+Cbeggar&* he screams in &+Rpanic&* and flees,\n"
"dropping one of his &+Cpebbles&* in his haste to get away.\n"
                        );
                setploc(pl, LOC_LIMBO_LIMBO);
                setoloc(OBJ_META_PEBBLE2, ploc(mynum), IN_ROOM);
                return True;
        }

        if ( (pl==MOB_META_SNAKE_BEGGAR+max_players)
        &&   (pmetatype(mynum)!=META_BEETLE)
        &&   (pmetatype(mynum)!=META_FISH_ONLAND)
        &&   (!iscarrby(OBJ_META_PEBBLE1, pl))
        &&   (!iscarrby(OBJ_META_PEBBLE2, pl))
        &&   (iscarrby(OBJ_META_PEBBLE3, pl))
            )
        {
                bprintf(
"As you move to attack the &+Cbeggar&* he screams in &+Rpanic&* and flees,\n"
"dropping his last &+Cpebble&* in his haste to get away.\n"
                        );
                setploc(pl, LOC_LIMBO_LIMBO);
                setoloc(OBJ_META_PEBBLE3, ploc(mynum), IN_ROOM);
                return True;
        }
	if ( (pl==MOB_META_SNAKE_BEGGAR+max_players)
	&&   ( (pmetatype(mynum)==META_FISH_ONLAND)
	||     (pmetatype(mynum)==META_BEETLE)
	      )
	    )
	{
		bprintf(
"You are too puny to offer much of a threat to anyone.\n"
                        );
                return True;
        }


return False;
}

Boolean meta_give(int pl, int ob)
{
	if ( (pl == MOB_META_SHOPKEEPER+max_players)
	&&   (ob == OBJ_META_SKIN)
	    )
	{
		bprintf(
"The shopkeeper looks with interest at the rabbit skin.\n"
"'&+MBeautiful!&*' he exclaims, '&+MExactly what I was looking for! Here, take\n"
"&+Mthis trinket as a token of my thanks.&*'\n"
"He hands you a small &+Corb&*.\n"
				);
		destroy(ob);
		setoloc(OBJ_META_ORB, mynum, CARRIED_BY);
		return True;
	}

	if ( (pl == MOB_META_JUDGE+max_players)
	&&   (ob == OBJ_META_ORB)
	    )
	{
		bprintf(
"You explain at some length how you came to acquire the &+Yorb&*, and where to find\n"
"the &+Cshopkeeper&* who stole it. The &+CJudge&* considers your story at length before\n"
"thanking you and giving you a precious &+Wsilver &+Mfeather&* by way of thanks.\n"
				);
		destroy(ob);
		setoloc(OBJ_META_SILVER_FEATHER, mynum, CARRIED_BY);
		setoloc(OBJ_META_DOOR_SHOP, LOC_META_SHOP_DESTROYED, IN_ROOM);
		setploc(MOB_META_SHOPKEEPER+max_players, LOC_LIMBO_LIMBO);
		setploc(MOB_META_SHOPKEEPER_BROKEN+max_players,LOC_META_JAIL);
		return True;
	}

	if ( (pl == MOB_META_SNAKE_BEGGAR+max_players)
	&&   (ob == OBJ_META_FLASK_EMPTY)
	&&   ( (iscontin(OBJ_META_WATER, ob))
	||     (iscontin(OBJ_META_WATER_2, ob))
	||     (iscontin(OBJ_META_WATER_3, ob))
	||     (iscontin(OBJ_META_WATER_4, ob))
	      )
	&&   (iscarrby(OBJ_META_PEBBLE1, pl))
	&&   (pmetatype(mynum)==META_SNAKE)
	    )
	{
		bprintf(
"The &+Cbeggar &+gthanks&* you for your generosity and swaps one of his\n"
"&+Cpebbles&* for the &+Bwater &*flask.\n"
			);
		setoloc(OBJ_META_FLASK_EMPTY, MOB_META_SNAKE_BEGGAR+max_players, CARRIED_BY);
		setoloc(OBJ_META_PEBBLE1, mynum, CARRIED_BY);
		return True;
	}

        if ( (pl == MOB_META_SNAKE_BEGGAR+max_players)
        &&   (ob == OBJ_META_FLASK_EMPTY)
        &&   ( (iscontin(OBJ_META_WATER, ob))
        ||     (iscontin(OBJ_META_WATER_2, ob))
        ||     (iscontin(OBJ_META_WATER_3, ob))
        ||     (iscontin(OBJ_META_WATER_4, ob))
              )

        &&   (!iscarrby(OBJ_META_PEBBLE1, pl))
	  &&   (iscarrby(OBJ_META_PEBBLE2, pl))
        &&   (pmetatype(mynum)==META_SNAKE)
            )
        {
                bprintf(
"The &+Cbeggar &+gthanks&* you for your generosity and swaps one of his\n"
"&+Cpebbles&* for the &+Bwater &*flask.\n"
                        );
                setoloc(OBJ_META_FLASK_EMPTY, MOB_META_SNAKE_BEGGAR+max_players, CARRIED_BY);
                setoloc(OBJ_META_PEBBLE2, mynum, CARRIED_BY);
                return True;
        }

        if ( (pl == MOB_META_SNAKE_BEGGAR+max_players)
        &&   (ob == OBJ_META_FLASK_EMPTY)
        &&   ( (iscontin(OBJ_META_WATER, ob))
        ||     (iscontin(OBJ_META_WATER_2, ob))
        ||     (iscontin(OBJ_META_WATER_3, ob))
        ||     (iscontin(OBJ_META_WATER_4, ob))
              )

        &&   (iscarrby(OBJ_META_PEBBLE3, pl))
        &&   (pmetatype(mynum)==META_SNAKE)
            )
        {
                bprintf(
"The &+Cbeggar &+gthanks&* you for your generosity and swaps one of his\n"
"&+Cpebbles&* for the &+Bwater &*flask.\n"
                        );
                setoloc(OBJ_META_FLASK_EMPTY, MOB_META_SNAKE_BEGGAR+max_players, CARRIED_BY);
                setoloc(OBJ_META_PEBBLE3, mynum, CARRIED_BY);
                return True;
        }

        if ( (pl == MOB_META_SNAKE_BEGGAR+max_players)
        &&   (pmetatype(mynum)!=META_SNAKE)
            )
        {
                bprintf(
"The &+Cbeggar&* looks at you in disgust. '&+MDo you really think I'm so desperate\n"
"&+Mas to accept something from the likes of you? Begone vermin!&*' And with that\n"
"he resumes his juggling.\n"
                        );
                return True;
        }
return False;
}

Boolean meta_tickle(void)
{
	if ( (pmetatype(mynum)==META_BEETLE)
	&&   (pl1 == MOB_META_SNAKE_BEGGAR+max_players)
	&&   (ploc(mynum)==ploc(MOB_META_SNAKE_BEGGAR+max_players))
	    )
	{
		bprintf(
"You run up and down the &+Csnake's&* &+Cbody&* in an attempt to tickle him, but you\n"
"succeed only in angering him and he drops a pebble on your tiny form, crushing you\n"
"instantly.\n"
			);
    send_msg (DEST_ALL, MODE_QUIET, max (pvis (mynum), LVL_WIZARD),LVL_MAX,mynum,NOBODY,
"&+B[&+RCrushed to Death&+B]\n");
                                p_crapup(mynum,
"&+RCrushed by a juggling snake!\n", CRAP_SAVE
                        );
		return True;
	}

	if ( (pmetatype(mynum)==META_BIRD)
	&&   (pl1 == MOB_META_SNAKE_BEGGAR+max_players)
	&&   (iscarrby(OBJ_META_PEBBLE1, MOB_META_SNAKE_BEGGAR+max_players))
	&&   (ploc(mynum)==ploc(pl1))
	&&   (odamage(OBJ_META_PEBBLE1)==0)
	&&   (odamage(OBJ_META_PEBBLE2)==0)
	&&   (odamage(OBJ_META_PEBBLE3)==0)
	    )
	{
		bprintf(
"You &+Ctickle&* the snake with your &+Cfeathers&* and he drops a pebble.\n"
"He &+Gglares&* at you and carries on juggling his other two pebbles.\n"
			);
		setoloc(OBJ_META_PEBBLE1, ploc(mynum), IN_ROOM);
		osetdamage(OBJ_META_PEBBLE1, mynum+1);
		return True;
	}

	if ( (pmetatype(mynum)!=META_BIRD)
	&&   (pl1 == MOB_META_SNAKE_BEGGAR+max_players)
	&&   (ploc(mynum)==ploc(pl1))
	    )
	{
		bprintf(
"You aren't really equipped to tickle anything, in your current form.\n"
			);
		return True;
	}

        if ( (pmetatype(mynum)==META_BIRD)
        &&   (pl1 == MOB_META_SNAKE_BEGGAR+max_players)
        &&   (!iscarrby(OBJ_META_PEBBLE1, MOB_META_SNAKE_BEGGAR+max_players))
	  &&   (iscarrby(OBJ_META_PEBBLE2, MOB_META_SNAKE_BEGGAR+max_players))
	  &&   (ploc(mynum)==ploc(pl1))
        &&   (odamage(OBJ_META_PEBBLE1)==0)
	  &&   (odamage(OBJ_META_PEBBLE2)==0)
	  &&   (odamage(OBJ_META_PEBBLE3)==0)
            )
        {
                bprintf(
"You &+Ctickle&* the snake with your &+Cfeathers&* and he drops a pebble.\n"
"He &+Gglares&* at you and carries on juggling his one remaining pebble.\n"
                        );
                setoloc(OBJ_META_PEBBLE2, ploc(mynum), IN_ROOM);
		    osetdamage(OBJ_META_PEBBLE2, mynum+1);
                return True;
        }

        if ( (pmetatype(mynum)==META_BIRD)
        &&   (pl1 == MOB_META_SNAKE_BEGGAR+max_players)
        &&   (!iscarrby(OBJ_META_PEBBLE2, MOB_META_SNAKE_BEGGAR+max_players))
        &&   (iscarrby(OBJ_META_PEBBLE3, MOB_META_SNAKE_BEGGAR+max_players))
        &&   (odamage(OBJ_META_PEBBLE1)==0)
	  &&   (ploc(mynum)==ploc(pl1))
	  &&   (odamage(OBJ_META_PEBBLE2)==0)
        &&   (odamage(OBJ_META_PEBBLE3)==0)
            )
        {
                bprintf(
"You &+Ctickle&* the snake with your &+Cfeathers&* and he drops a pebble.\n"
"He &+Gglares&* at you and hobbles off to find more pebbles.\n"
                        );
                setoloc(OBJ_META_PEBBLE3, ploc(mynum), IN_ROOM);
		    osetdamage(OBJ_META_PEBBLE3, mynum+1);
                return True;
        }
        if ( (pmetatype(mynum)==META_BIRD)
        &&   (pl1 == MOB_META_SNAKE_BEGGAR+max_players)
	  &&   (ploc(mynum)==ploc(pl1))
        &&   ( (odamage(OBJ_META_PEBBLE1)!=0)
	  ||     (odamage(OBJ_META_PEBBLE2)!=0)
	  ||     (odamage(OBJ_META_PEBBLE3)!=0)
              )
	    )
        {
                bprintf(
"You &+Ctickle&* the snake with your &+Cfeathers&* and he almost drops a pebble,\n"
"but he manages to get it back in the &+Bair&* at the last moment. Glaring at you\n"
"he continues juggling.\n"
				);
		return True;
	  } 


return False;
}

Boolean meta_smell(void)
{
	if ( (pmetatype(mynum)==META_BEETLE)
	&&   (ob1 == OBJ_META_PEBBLE3)
	    )
	{
		bprintf(
"You can detect a faint trace of &+Ctree sap&* around the edges.\n"
			);
		return True;
	}

	if ( (EMPTY(item1))
	&&   (ploc(mynum)==LOC_META_META59)
	&&   (pmetatype(mynum)==META_HUMAN)
	    )
	{
		bprintf(
"Eugh, the smell is almost intolerable, and reminds you of a cattle stall.\n"
			);
		return True;
	}

        if ( (EMPTY(item1))
        &&   (ploc(mynum)==LOC_META_META59)
        &&   (pmetatype(mynum)==META_BEETLE)
            )
        {
                bprintf(
"The smell of &+ydung&* floats to you on the breeze, sweet nectar to your senses.\n"
                        );
                return True;
        }

	if ( (ploc(mynum) == LOC_META_META60)
	&&   (EMPTY(item1))
	    )
	{
		bprintf(
"You can smell lingering traces of &+ydung&* to the &+Geast&* and &+Gwest&*.\n"
				);
		return True;
	}
return False;
}

Boolean meta_look(int loc)
{
	char *pos1;
	char *pos2=NULL;

	if (pmetatype(mynum)==META_HUMAN) return False;
	
	if ( ((pos1=strstr(ldesc(loc),"\n$beetle"))!=NULL)
	&&   ( (pmetatype(mynum)==META_BEETLE)
	||     (pmetatype(mynum)==META_BEETLE_UNDERWATER)
	      )
	   )
	{
		pos1=strstr(pos1+1,"\n")+1;
		pos2=strstr(pos1,"\n$");
		if (pos2!=NULL) *++pos2='\0';
		bprintf("%s",pos1);
		if (pos2!=NULL) *pos2='$';
		return True;
	}

	if ( ((pos1=strstr(ldesc(loc),"\n$bird"))!=NULL)
	&&   (pmetatype(mynum)==META_BIRD)
	   )
	{
		pos1=strstr(pos1+1,"\n")+1;
		pos2=strstr(pos1,"\n$");
		if (pos2!=NULL) *++pos2='\0';
		bprintf("%s",pos1);
		if (pos2!=NULL) *pos2='$';
		return True;
	}

        if ( ((pos1=strstr(ldesc(loc),"\n$fish"))!=NULL)
        &&   ( (pmetatype(mynum)==META_FISH_ONLAND)
        ||     (pmetatype(mynum)==META_FISH_UNDERWATER)
              )
           )
        {
                pos1=strstr(pos1+1,"\n")+1;
                pos2=strstr(pos1,"\n$");
                if (pos2!=NULL) *++pos2='\0';
                bprintf("%s",pos1);
                if (pos2!=NULL) *pos2='$';
                return True;
        }

        if ( ((pos1=strstr(ldesc(loc),"\n$snake"))!=NULL)
        &&   ( (pmetatype(mynum)==META_SNAKE)
        ||     (pmetatype(mynum)==META_SNAKE_UNDERWATER)
              )
           )
        {
                pos1=strstr(pos1+1,"\n")+1;
                pos2=strstr(pos1,"\n$");
                if (pos2!=NULL) *++pos2='\0';
                bprintf("%s",pos1);
                if (pos2!=NULL) *pos2='$';
                return True;
        }
return False;
}

Boolean meta_enter(int obj)
{
        if ( (obj==OBJ_META_HOLE_BEETLE)
        &&   (pmetatype(mynum)==META_BEETLE)
            )
        {
                bprintf("You scuttle &+Gdown&* the &+Chole&*.\n");
                setploc(mynum, LOC_META_META60);
                lookin(ploc(mynum), 1);
                send_msg(LOC_META_META59, 0, pvis(mynum), LVL_MAX, NOBODY,
NOBODY, "%s &+Gscuttles&* down&*.\n", pname(mynum)
                        );
                send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s scuttles in from &+Gabove&*.\n", pname(mynum)
                        );
                return True;
        }

        if ( (obj==OBJ_META_HOLE_BEETLE_UP)
        &&   (pmetatype(mynum)==META_BEETLE)
            )
        {
                bprintf("You scuttle &+Gup&* the &+Chole&*.\n");
                setploc(mynum, LOC_META_META59);
                lookin(ploc(mynum), 1);
                send_msg(LOC_META_META60, 0, pvis(mynum), LVL_MAX, NOBODY,
NOBODY, "%s &+Gscuttles&* up&*.\n", pname(mynum)
                        );
                send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s scuttles in from &+Gbelow&*.\n", pname(mynum)
                        );
                return True;
        }

	if ( (obj == OBJ_META_HOLE)
	&&   (pmetatype(mynum)==META_SNAKE)
	    )
	{
		bprintf("You &+Gslither&* down the hole.\n");
		trapch(LOC_META_META35);
		send_msg(LOC_META_META1, MODE_NOBLIND, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s &+Gslithers&* down the hole.\n", pname(mynum)
			);
		send_msg(LOC_META_META35, MODE_NOBLIND, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s &+Gslithers&* in from above.\n", pname(mynum)
			);
		return True;
	}

        if ( (obj == OBJ_META_HOLE)
        &&   (pmetatype(mynum)==META_BEETLE)
            )
        {
                bprintf("You &+Cscuttle&* down the hole.\n");
                trapch(LOC_META_META35);
                send_msg(LOC_META_META1, MODE_NOBLIND, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s &+Cscuttles&* down the hole.\n", pname(mynum)
                        );
                send_msg(LOC_META_META35, MODE_NOBLIND, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s &+Cscuttles&* in from above.\n", pname(mynum)
                        );
                return True;
        }

        if ( (obj == OBJ_META_HOLE)
        &&   ( (pmetatype(mynum)==META_BIRD)
	||     (pmetatype(mynum)==META_FISH_ONLAND)
	||     (pmetatype(mynum)==META_HUMAN)
	      )
            )
        {
		bprintf("You are too big to fit in the &+Chole&*.\n");
		return True;
        }

        if ( (obj == OBJ_META_HOLE_UP)
        &&   (pmetatype(mynum)==META_SNAKE)
            )
        {
                bprintf("You &+Gslither&* up the hole.\n");
                trapch(LOC_META_META1);
                send_msg(LOC_META_META35, MODE_NOBLIND, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s &+Gslithers&* up the hole.\n", pname(mynum)
                        );
                send_msg(LOC_META_META1, MODE_NOBLIND, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s &+Gslithers&* in from below.\n", pname(mynum)
                        );
                return True;
        }

        if ( (obj == OBJ_META_HOLE_UP)
        &&   (pmetatype(mynum)==META_BEETLE)
            )
        {
                bprintf("You &+Cscuttle&* up the hole.\n");
                trapch(LOC_META_META1);
                send_msg(LOC_META_META35, MODE_NOBLIND, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s &+Cscuttles&* up the hole.\n", pname(mynum)
                        );
                send_msg(LOC_META_META1, MODE_NOBLIND, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s &+Cscuttles&* in from below.\n", pname(mynum)
                        );
                return True;
	}

        if ( (obj == OBJ_META_HOLE_UP)
        &&   ( (pmetatype(mynum)==META_BIRD)
        ||     (pmetatype(mynum)==META_FISH_ONLAND)
        ||     (pmetatype(mynum)==META_HUMAN)
              )
            )
        {
                bprintf("You are too big to fit in the &+Chole&*.\n");
                return True;
        }

        if ( (obj == OBJ_META_HOLE_HERRING)
	  &&   (!iscarrby(OBJ_META_BOTTLE, mynum))
            )
        {
		bprintf(
"As you go to enter the &+Chole&*, a &+Ldark&* force knocks you backwards, preventing\n"
"you from entering.\n"
			);
           	return True;
        }

	  if ( (obj == OBJ_META_HOLE_HERRING)
	  &&   (iscarrby(OBJ_META_BOTTLE, mynum))
	  &&   (iscontin(OBJ_META_RED_FLAME, OBJ_META_BOTTLE))
	      )
	  {
		bprintf(
"Armed with the &+Ybright light&* streaming from the &+Cbottle&*, the dark spirit\n"
"doesn't attack as you enter the hole.\n");
	      trapch(LOC_META_VERY_FOOLISH);
                send_msg (DEST_ALL, MODE_QUIET, max (pvis (mynum),
LVL_WIZARD),LVL_MAX,mynum,NOBODY,
"&+B[&+RHeart Gave Up&+B]\n"
                                );

		p_crapup(mynum,
"&+RTerror takes a firm grip of you, it seems the spirit was prevending you from\n"
"&+Rentering for your own protection.  All the very worst demons from your nightmares\n"
"&+RConverge on you, tearing your soul forcibly from your body, inflicting unimaginable\n"
"&+Rpain on you until your heart finally gives up the go and you surrender to oblivion.\n",
CRAP_SAVE
				);
		return True;
	}
return False;
}

Boolean meta_wear(int obj)
{
	if (mynum >= max_players) return False;
	
	if ( (pmetatype(mynum)==META_FISH_ONLAND)
	||   (pmetatype(mynum)==META_FISH_UNDERWATER)
	||   (pmetatype(mynum)==META_BEETLE)
	||   (pmetatype(mynum)==META_BEETLE_UNDERWATER)
	||   (pmetatype(mynum)==META_SNAKE)
	||   (pmetatype(mynum)==META_SNAKE_UNDERWATER)
	||   (pmetatype(mynum)==META_BIRD)
            )
        {
                bprintf("You can't wear anything in your current &+Cform&*.\n");
                return True;
        }
        return False;
}

Boolean meta_wearall(int obj)
{
	if (mynum >= max_players) return False;
	
        if ( (pmetatype(mynum)==META_FISH_ONLAND)
        ||   (pmetatype(mynum)==META_FISH_UNDERWATER)
        ||   (pmetatype(mynum)==META_BEETLE)
        ||   (pmetatype(mynum)==META_BEETLE_UNDERWATER)
        ||   (pmetatype(mynum)==META_SNAKE)
        ||   (pmetatype(mynum)==META_SNAKE_UNDERWATER)
        ||   (pmetatype(mynum)==META_BIRD)
            )
        {
                return True;
        }
        return False;
}

Boolean meta_empty(int obj)
{
int b;
	if ( ((pmetatype(mynum)==META_BEETLE)
	||    (pmetatype(mynum)==META_BEETLE_UNDERWATER)
	||    (pmetatype(mynum)==META_FISH_ONLAND)
	||    (pmetatype(mynum)==META_FISH_UNDERWATER))
	&&   (pnumobs(mynum)>0)
	&&   (!iscarrby(obj, mynum))
	&&   (obj!=OBJ_META_PANEL)
	    )
	{
		bprintf("You cannot empty the &+C%s&*, you are carrying something.\n",oname(obj)
			);
		return True;
	}

        if ( ((pmetatype(mynum)==META_SNAKE)
        ||    (pmetatype(mynum)==META_SNAKE_UNDERWATER)
        ||    (pmetatype(mynum)==META_BIRD))
        &&   (pnumobs(mynum)>1)
        &&   (!iscarrby(obj, mynum))
            )
        {
                bprintf("You cannot empty the &+C%s&*, you are carrying something.\n", oname(obj)
                        );
                return True;
        }

	if ( (obj == OBJ_META_PANEL)
	&&   (iscontin(OBJ_META_LIMESTONE, OBJ_META_PANEL))
	    )
	{
		bprintf(
"You empty the &+Climestone&* from the panel.\n"
"You watch in horror as it dissolves away to nothingness in the water.\n"
				);
		destroy (OBJ_META_LIMESTONE);
		for (b = ofirst_obj (OBJ_META_PANEL); b != SET_END; b = onext_obj (OBJ_META_PANEL))
		{
			if (iscontin (b, obj))
			{
      			setoloc (b, mynum, CARRIED_BY);
				bprintf ("You empty %s&+C%s&* from %s&+C%s&*.\n", 
				the(b),oname (b), the(obj),oname (obj));      
				dropobj(b);
			}
		}
		return True;
	}

	if (obj == OBJ_META_CAULDRON)
	{
                for (b = ofirst_obj (obj); b != SET_END; b = onext_obj (obj))
                {
                        if ( (b == OBJ_META_WATER)
                        ||   (b == OBJ_META_WATER_2)
                        ||   (b == OBJ_META_WATER_3)
                        ||   (b == OBJ_META_WATER_4)
                        ||   (b == OBJ_META_ALKALINE)
                            )
                        {
				bprintf(
"You &+Cempty&* the &+C%s&* from the &+C%s&*, and watch it trickle away into the\n"
"&+gground&*, vanishing quickly.\n", oname(b), oname(obj)
                                        );
                                destroy(b);
                        }
                        else
                        {
                                bprintf ("You empty %s&+C%s&* from %s&+C%s&*.\n",
                                the(b),oname (b), the(obj),oname (obj));
                                dropobj(b);
                        }
                }
        return True;
        }

	if ( ((obj == OBJ_META_FLASK_EMPTY)||(obj==OBJ_META_BOTTLE))
	&&   ( (iscontin(OBJ_META_WATER, obj))
	||     (iscontin(OBJ_META_WATER_2, obj))
	||     (iscontin(OBJ_META_WATER_3, obj))
	||     (iscontin(OBJ_META_WATER_4, obj))
	||     (iscontin(OBJ_META_ALKALINE, obj))
	      )
	    )
	{
                for (b = ofirst_obj (obj); b != SET_END; b = onext_obj (obj))
                {
			if ( (b == OBJ_META_WATER)
			||   (b == OBJ_META_WATER_2)
			||   (b == OBJ_META_WATER_3)
			||   (b == OBJ_META_WATER_4)
			||   (b == OBJ_META_ALKALINE)
			    )
			{
				bprintf(
"You &+Cempty&* the &+C%s&* from the &+C%s&*, and watch it trickle away into the\n"
"&+gground&*, vanishing quickly.\n", oname(b), oname(obj)
					);
				destroy(b);
			}
			else
			{
	                        bprintf ("You empty %s&+C%s&* from %s&+C%s&*.\n",
                                the(b),oname (b), the(obj),oname (obj));
                                dropobj(b);
			}
		}
	return True;
	}

	  
	if ( (pmetatype(mynum)==META_BIRD)
	&&   (obj == OBJ_META_GUN)
	&&   (iscontin(OBJ_META_GUNPOWDER, obj))
	    )
	{
		bprintf("You fly up and empty some &+Cgunpowder&* from the gun.\n");
		setoloc(OBJ_META_GUNPOWDER, ploc(mynum), IN_ROOM);
		return True;
	}

        if ( (pmetatype(mynum)==META_BIRD)
        &&   (obj == OBJ_META_VINES)
        &&   (iscontin(OBJ_META_VINE, obj))
            )
        {
                bprintf("You fly up and empty a &+Cvine&* from the vines.\n");
                setoloc(OBJ_META_VINE, ploc(mynum), IN_ROOM);
                return True;
        }


	if ( (pmetatype(mynum)==META_BIRD)
	&&   (obj == OBJ_META_GUN)
	&&   (!iscontin(OBJ_META_GUNPOWDER, obj))
	    )
	{
		bprintf("You have already emptied the &+Cgun&*.\n");
		return True;
	}
        if ( (pmetatype(mynum)==META_BIRD)
        &&   (obj == OBJ_META_VINES)
        &&   (!iscontin(OBJ_META_VINE, obj))
            )
        {
                bprintf("There is nothing else in the &+Cvines&*.\n");
		return True;
	}

        if ( (pmetatype(mynum)!=META_BIRD)
        &&   (obj == OBJ_META_VINES)
	    )
	{
		bprintf("It is too far out of reach.\n");
		return True;
	}

	if ( (pmetatype(mynum)!=META_BIRD)
	&&   (obj == OBJ_META_GUN)
	    )
	{
		bprintf("It is too far out of reach.\n");
		return True;
	}

return False;
}

void meta_timer_bomb(Boolean bomb_start, Boolean bomb_stop)
{
        static int timestarted;
        static Boolean running;
        int pl;

        if ( (bomb_start==True)
            )
        {
                running=True;
                timestarted=global_clock;
                return;
        }

        if ( (bomb_stop==True)
            )
        {
                running=False;
                timestarted=0;
                return;
        }

        if (!running) return;
        if ( (global_clock-timestarted >=30)
            )
        {
		if ( (oloc(OBJ_META_CHEST)!=LOC_META_META34)
		&&   (state(OBJ_META_BOMB)==0)
		    )
		{
                broad(
"An enormous &+Rexplosion&* rocks the&+y ground&* around you.\n"
                        );
			for (pl=0;pl<max_players;pl++)
			{
                                if ( (ploc(pl)==LOC_META_META30)
                                &&   (plev(pl)<LVL_WIZARD)
				&&   (pmetatype(pl)==META_FISH_UNDERWATER)
                                    )
                                {
                                        sendf(pl,
"At the sound of the &+Rexplosion&*, a loud cheer goes up, hundreds of fish mill\n"
"around you, thanking you for destroying the Beaver's dam.\n"
"The Fish &+CKing&* swims up to you. '&+MThank you for this great service,&*' he says,\n"
"'&+MYou shall forever be remembered by my people, now please take this trinket so\n"
"you &+Mshall not forget us either.\n"
"He hands you a &+Clarge&* multifaceted &+Cdiamond&* and swims away, thanking you all\n"
"the while.\n"
                                );
                                        setploc(MOB_META_FISH_KING+max_players, LOC_LIMBO_LIMBO); 
					setoloc(OBJ_META_DIAMOND, pl, CARRIED_BY);
					setploc(MOB_META_FISH_KING2+max_players, ploc(pl));
				}

				if(ploc(pl)==oloc(OBJ_META_BOMB))
				{
					sendf(pl,
"The bomb &+Rexplodes&*, the enormous &+Yexplosion&* ripping through you like you weren't\n"
"even there, ending your &+Mlife&* instantly.\n");
					p_crapup(pl,
"                  Blown to tiny, tiny pieces\n", CRAP_SAVE
							);
				}
			}
		if (iscontin(OBJ_META_BOMB, OBJ_META_CHEST))
		{
			destroy(OBJ_META_BOMB);
			destroy(OBJ_META_CHEST);
			meta_timer_bomb(False, True);
		}		
		if (!iscontin(OBJ_META_BOMB, OBJ_META_CHEST))
		{
			destroy(OBJ_META_BOMB);
			meta_timer_bomb(False, True);
		}
	  	}
		if ( (oloc(OBJ_META_CHEST)==LOC_META_META34)
		&&   (state(OBJ_META_BOMB)==0)
		&&   (iscontin(OBJ_META_BOMB, OBJ_META_CHEST))
		    )
		{
                broad(
"An enormous &+Rexplosion&* rocks the&+y ground&* around you.\n"
                        );


			for (pl=0;pl<max_players;pl++)
			{
				if ( (ploc(pl)==LOC_META_META30)
				&&   (plev(pl)<LVL_WIZARD)
                                &&   (pmetatype(pl)==META_FISH_UNDERWATER)
				    )
				{
              				sendf(pl,
"At the sound of the &+Rexplosion&*, a loud cheer goes up, hundreds of fish mill\n"
"around you, thanking you for destroying the Beaver's dam.\n"
"The Fish &+CKing&* swims up to you. '&+MThank you for this great service,&*' he says,\n"
"'&+MYou shall forever be remembered by my people, now please take this trinket so\n"
"you &+Mshall not forget us either.\n"
"He hands you a &+Clarge&* multifaceted &+Cdiamond&* and swims away, thanking you all\n"
"the while.\n"
                                );
			                setoloc(OBJ_META_DIAMOND, pl, CARRIED_BY);
					setploc(MOB_META_FISH_KING+max_players, LOC_LIMBO_LIMBO);
					setploc(MOB_META_FISH_KING2+max_players, ploc(pl));
				}
				if(ploc(pl)==oloc(OBJ_META_BOMB))
				{
					sendf(pl,
"The bomb &+Rexplodes&*, the enormous &+Yexplosion&* ripping through you like you weren't\n"
"even there, ending your &+Mlife&* instantly.\n");
					p_crapup(pl,
"                  Blown to tiny, tiny pieces\n", CRAP_SAVE
							);
				}
			}
			meta_timer_bomb(False, True);
			destroy(OBJ_META_BOMB);
			destroy(OBJ_META_CHEST);
			setobjstate(OBJ_META_DAM, 1);
		}
	}
}

Boolean meta_light(void)
{
int b, pl;
int c = OBJ_META_CAULDRON;

	if ( (ob1 == OBJ_META_GUNPOWDER)
        &&   ( (ohany (1 << OFL_LIT))
        ||     (ststflg (mynum, SFL_LIT))
              )
	    )
	{
                send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s &+Ylights&* some gunpowder.\n", pname(mynum)
				);
		if (ocarrf(ob1)!=CARRIED_BY)
		{
                        for (pl=0;pl<max_players;pl++)
                        {
                                if((ploc(pl)==oloc(OBJ_META_GUNPOWDER))
				&& (is_in_game(pl))) 
                               {
    send_msg (DEST_ALL, MODE_QUIET, max (pvis (mynum), LVL_WIZARD), LVL_MAX,pl,NOBODY,
"&+B[&+RCompletely Blown Away&+B]\n");

                                        sendf(pl,
"The gunpowder &+Rexplodes&*, the enormous &+Yexplosion&* ripping through you like you\n"
"weren't even there, ending your &+Mlife&* instantly.\n");
                                        p_crapup(pl,
"                  Blown to tiny, tiny pieces\n", CRAP_SAVE
                                                        );
                                }
                        }
		return True;
		}
	if (ocarrf(ob1)==CARRIED_BY)
	{
                send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
"&+YLuckily&* &+C%s&* is carrying it and &+C%s&* body shields you from the &+Rexplosion&*.\n", (psex(mynum) ? "she" : "he"), his_or_her(mynum)
				);
		    send_msg (DEST_ALL, MODE_QUIET, max (pvis (mynum), LVL_WIZARD), LVL_MAX,mynum,NOBODY,
"&+B[&+RCompletely Blown Away&+B]\n");

                                        sendf(mynum,
"The gunpowder &+Rexplodes&*, the enormous &+Yexplosion&* ripping through you like you\n"
"weren't even there, ending your &+Mlife&* instantly.\n");
                                        p_crapup(mynum,
"                  Blown to tiny, tiny pieces\n", CRAP_SAVE
                                                        );
		return True;
	}
		
	return True;
	}

	if ( (ob1 == OBJ_META_KINDLING)
        &&   ( (ohany (1 << OFL_LIT))
        ||     (ststflg (mynum, SFL_LIT))
              )
	&&   (iscontin(OBJ_META_ALKALINE, OBJ_META_CAULDRON))
	&&   (iscontin(OBJ_META_SCALE, OBJ_META_CAULDRON))
	&&   (iscontin(OBJ_META_DUNG_OLD, OBJ_META_CAULDRON))
	&&   (iscontin(OBJ_META_SILVER_FEATHER, OBJ_META_CAULDRON))
	&&   (iscontin(OBJ_META_DIAMOND, OBJ_META_CAULDRON))
	    )
	{
		bprintf(
"You &+Ylight&* the &+Ckindling&*.\n"
"\n"
"The ingredients in the &+Ccauldron&* shimmer and blur into a deep &+rcrimson&*\n"
"&+Cliquid&* which collects in the bottom of the cauldron.\n"
				);
		for (b = ofirst_obj (c); b != SET_END; b = onext_obj (c))
		{
			if (iscontin (b, c))
			{
				destroy(b);
			}
		}
		setoloc(OBJ_META_LIQUID_HUMAN, OBJ_META_CAULDRON, IN_CONTAINER);
		return True;
	}

	if ( (ob1 == OBJ_META_BRANCH)
	&&   (ob2 == OBJ_META_BLUE_FLAME)
	&&   (!otstbit(ob1, OFL_LIT))
	&&   (iscarrby(ob1, mynum))
	    )
	{
		bprintf(
"You &+Ylight&* the branch with the &+Cflame&*.\n\n"
"The &+Cflame&* flickers along the length of the branch, petrifying it in\n"
"seconds. Alarmingly, it doesn't stop there, and starts the same process\n"
"on you. Within moments you are nothing but a statue.\n"
			);
    send_msg (DEST_ALL, MODE_QUIET, max (pvis (mynum), LVL_WIZARD),LVL_MAX,mynum,NOBODY,
"&+B[&+RTurned to stone&+B]\n");

		p_crapup(mynum,
"&+RTurned to stone by a flame!\n", CRAP_SAVE
			); 

		return True;
	}

        if ( (ob1 == OBJ_META_BRANCH)
        &&   (ob2 == OBJ_META_BLUE_FLAME)
        &&   (!otstbit(ob1, OFL_LIT))
        &&   (!iscarrby(ob1, mynum))
            )
        {
                bprintf(
"You &+Ylight&* the branch with the &+Cflame&* by rolling the branch into it.\n"
"\n"
"The &+Cflame&* flickers along the length of the branch, petrifying it in\n"
"seconds.\n"
			);
		destroy(ob1);
		destroy(ob2);
		setoloc(OBJ_META_STONE_BRANCH, ploc(mynum), IN_ROOM);
		return True;
	}

        if ( (ob1 == OBJ_META_BRANCH)
        &&   (ob2 == OBJ_META_BLUE_FLAME)
        &&   (otstbit(ob1, OFL_LIT))
            )
        {
                bprintf(
"You try to &+Ylight&* the &+Cbranch&* with the &+Cflame&*.\n"
"\n"
"The heat of the existing &+Yflame&* and the cold of the &+Bblue &+Yflame&*\n"
"succeed only in destroying each other.\n"
			);
		oclrbit(ob1, OFL_LIT);
		destroy(ob2);
		return True;
	}

	if ( (ob1 == OBJ_META_BOMB)
	&&   (state(OBJ_META_BOMB)==1)
	&&   ( (ohany (1 << OFL_LIT))
	||     (ststflg (mynum, SFL_LIT))
	      )
	    )
	{
		bprintf("You &+Ylight&* the fuse which begins to sputter and crackle.\n");
                send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
"%s &+Ylights&* a bomb which begins to sputter and crackle.\n",pname(mynum)
			);
		osetbit(OBJ_META_BOMB, OFL_LIT);
		setobjstate(OBJ_META_BOMB, 0);
		meta_timer_bomb(True,False);
		return True;
	}

	if ( (ob1 == OBJ_META_BOMB)
	&&   (state(OBJ_META_BOMB)==0)
	    )
	{
		bprintf("It's already &+Ylit&*!\n");
		return True;
	}
return False;
}

void meta_timer_jail(Boolean jail_start, Boolean jail_stop)
{
        static int timestarted;
        static Boolean running;
        int pl;

        if ( (jail_start==True)
            )
        {
                running=True;
                timestarted=global_clock;
                return;
        }

        if ( (jail_stop==True)
            )
        {
                running=False;
                timestarted=0;
                return;
        }

        if (!running) return;


        if ( (global_clock-timestarted == 20)
            )
        {
        for (pl=0; pl < max_players;pl++)
                {
                        if (ploc(pl) == LOC_META_META52)
			{
				sendf(pl,
"You hear &+Rnoises&* outside the door.\n"
					);
			}
		}
	}

	if ( (global_clock-timestarted == 40)
	    )
	{
	for (pl=0; pl < max_players; pl++)
	{
		if (ploc(pl) == LOC_META_META52)
		{
			sendf(pl,
"You hear a &+Rvoice&* on the other side of the door.\n"
"'&+MYou, fetch the prisoner, and ready the council.&*'\n"
				);
		}
	}
	}

	if ( (global_clock-timestarted == 60)
	    )
	{
		for (pl=0; pl< max_players; pl++)
		{
			if (ploc(pl)== LOC_META_META52)
			{
				sendf(pl,
"The &+Cdoor&* opens and a &+Rbird&* steps in.\n"
					);
				if (pmetatype(pl)==META_BIRD)
				{
					sendf(pl,
"'&+MCome with me,&*' says the &+Cbird&*, '&+Mand don't try and struggle,\n"
"&+Myou'll only make it worse for yourself.&*'\n"
"You are dragged off to the Avian Council chambers.\n\n"
						);
					dumpstuff(pl, LOC_LIMBO_LIMBO);
					trapch_plr(pl, LOC_META_META53);
					setobjstate(OBJ_META_DOCK, 1);
					meta_timer_jail(False,True);
				}
				
				if (pmetatype(pl)==META_FISH_ONLAND)
				{
                                        sendf(pl,
"The &+Cbird&* looks around the room, obviously expecting another bird\n"
"to be awaiting him, and he is clearly confused by the presence of a fish.\n"
"He looks at you with beady eyes, then quickly resolves himself and pecks\n"
"you to death quickly and painfully.\n"
						);
        send_msg (DEST_ALL, MODE_QUIET, max(pvis (mynum), LVL_WIZARD),LVL_MAX,mynum,NOBODY,
"&+B[&+RPecked to Death&+B]\n"
                                                );
					p_crapup(pl,
"&+RPecked to death by a bird!\n", CRAP_SAVE
						);
					meta_timer_jail(False,True);
				}
		
				if (pmetatype(pl)==META_SNAKE)
                                {
                                        sendf(pl,
"The &+Cbird&* looks around the room, obviously expecting another bird\n"
"to be awaiting him, and he is clearly confused by the presence of a snake.\n"
"His confusion quickly gives way to terror and he flees in panic, screaming\n"
"hysterically.\n"
						);
					setobjstate(OBJ_META_CELL_DOOR_OUT, 0);
					setobjstate(OBJ_META_CELL_DOOR_IN, 0);
					meta_timer_jail(False,True);
				}

				if (pmetatype(pl)==META_BEETLE)
                                {
                                        sendf(pl,
"The &+Cbird&* looks around the room, obviously expecting another bird\n"
"to be awaiting him, and he is clearly confused by the presence of a beetle.\n"
"His confusion quickly gives way to hunger, and he quickly bends down, \n"
"grabbing you in his beak and crushing you to death.\n"
						);
              send_msg (DEST_ALL, MODE_QUIET,max(pvis (mynum),LVL_WIZARD),LVL_MAX,mynum,NOBODY,
"&+B[&+REaten Alive&+B]\n"
                                                );
                                        p_crapup(pl,
"&+REaten alive by a bird!\n", CRAP_SAVE
                                                );
					meta_timer_jail(False, True);
				}
                                if (pmetatype(pl)==META_HUMAN)
                                {
                                        sendf(pl,
"The &+Cbird&* looks around the room, obviously expecting another bird\n"
"to be awaiting him, and he is clearly confused by the presence of a human.\n"
"His confusion quickly gives way to terror and he flees in panic, screaming\n"
"in terror.\n"
                                                );
                                        setobjstate(OBJ_META_CELL_DOOR_OUT, 0);
                                        setobjstate(OBJ_META_CELL_DOOR_IN, 0);
                                        meta_timer_jail(False,True);
                                }
			}
		}
	}
}

void meta_say(char *foo)
{
        char bar[MAX_COM_LEN+1];

        if ( (ploc(mynum)==LOC_META_META53)
	&&   (state(OBJ_META_DOCK)==1)
	&&   (odamage(OBJ_META_DOCK)==0)
            )
        {
                strcpy(bar,foo);
                lowercase(bar);

                if (strstr(bar,"guilty")!=NULL)
                {
			bprintf(
"The &+CJudge&* looks at you in surprise. Clearly not many birds confess their\n"
"crimes. He peers down at you for a moment, before nodding.\n"
"'&+MSo be it.&*' he says, '&+MYou are hereby sentenced to life imprisonment\n"
"&+Mfor your atrocities, and may the gods have mercy on your soul.&*'\n"
"\n"
"Clearly &+Mhonesty&* isn't always the best policy.\n"
"You are taken away to a maximum security holding cell, and forced to watch\n"
"while the key is melted down. There is no escape.\n"
				);
			trapch(LOC_META_JAIL);
			if (ploc(MOB_META_SHOPKEEPER+max_players)==LOC_META_JAIL)
			{
				bprintf(
"The current occupant of the cell looks up in surprise as you enter and you\n"
"squint in the dim light to try and see his face. It is with a mixture of\n"
"shock and apprehension that you realise that it is none other than the \n"
"thieving Shopkeeper whom you had incarcerated earlier. He appears to show\n"
"little interest in you though, and sits in a corner babbling to himself\n"
"incoherently. It appears as though he has been broken properly by the justice\n"
"system.\n"
					);
			}
			return;
		}

		if (strstr(bar,"innocent")!=NULL)
		{
                        bprintf(
"The &+CJudge&* looks at you with contempt. '&+MYour crimes were witnessed\n"
"&+Mby several citizens who have testified against you. Your refusal to\n"
"&+Madmit to your guilt is testimony to your criminal nature.&*'\n"
"He peers down at you for a moment, before nodding.\n"
"'&+MSo be it.&*' he says, '&+MYou are hereby sentenced to life imprisonment\n"
"&+Mfor your atrocities, and may the gods have mercy on your soul.&*'\n"
"\n"
"You are taken away to a maximum security holding cell, and forced to watch\n"
"while the key is melted down. There is no escape.\n"
                                );
                        trapch(LOC_META_JAIL);
                        if (ploc(MOB_META_SHOPKEEPER+max_players)==LOC_META_JAIL)
                        {
                                bprintf(
"The current occupant of the cell looks up in surprise as you enter and you\n"
"squint in the dim light to try and see his face. It is with a mixture of\n"
"shock and apprehension that you realise that it is none other than the \n"
"thieving Shopkeeper whom you had incarcerated earlier. He appears to show\n"
"little interest in you though, and sits in a corner babbling to himself\n"
"incoherently. It appears as though he has been broken properly by the justice\n"
"system.\n"
                                        );
			}
                        return;
                }
	
		if ( (strstr(bar,"innocent")==NULL)
		&&   (strstr(bar,"guilty")==NULL)
		    )
		{
		bprintf(
"The &+CBailiff&* stares at you.\n"
"'&+MInnocent or Guilty, or you will be held in contempt of this court.&*'\n"
			);
		return;
		}
	}
}

Boolean meta_cut(int obj)
{
	if ( (obj == OBJ_META_ROPES)
	&&   (state(OBJ_META_MECHANISM_LEVER)==0)
	&&   (oloc(OBJ_META_DRAWBRIDGE_IN)==LOC_META_META14)
	&&   (pmetatype(mynum)!=META_BIRD)
	&&   (pmetatype(mynum)!=META_SNAKE)
	    )
	{
		bprintf("You have nothing to &+Rcut&* it with.\n");
		return True;
	}

        if ( (obj == OBJ_META_ROPES)
        &&   (state(OBJ_META_MECHANISM_LEVER)==0)
        &&   (oloc(OBJ_META_DRAWBRIDGE_IN)==LOC_META_META14)
        &&   (pmetatype(mynum)==META_BIRD)
            )
        {
                bprintf(
"You &+Rcut&* the &+Cropes&* with your beak.\n"
"\n"
"The &+Cdrawbridge&* teeters slightly, then falls with a crash, shattering on\n"
"impact with the other side of the moat, destroying your only way in.\n"
			);
		destroy(OBJ_META_DRAWBRIDGE_IN);
		setobjstate(OBJ_META_ROPES, 1);
		return True;
	}

        if ( (obj == OBJ_META_ROPES)
        &&   (state(OBJ_META_MECHANISM_LEVER)==0)
        &&   (oloc(OBJ_META_DRAWBRIDGE_IN)==LOC_META_META14)
        &&   (pmetatype(mynum)==META_SNAKE)
            )
        {
                bprintf(
"You &+Rcut&* the &+Cropes&* with your teeth.\n"
"\n"
"The &+Cdrawbridge&* teeters slightly, then falls with a crash, shattering on\n"
"impact with the other side of the moat, destroying your only way in.\n"
                        );
                destroy(OBJ_META_DRAWBRIDGE_IN);
		setobjstate(OBJ_META_ROPES, 1);
                return True;
        }

        if ( (obj == OBJ_META_ROPES)
        &&   (state(OBJ_META_MECHANISM_LEVER)==1)
        &&   (oloc(OBJ_META_DRAWBRIDGE_IN)==LOC_META_META14)
        &&   (pmetatype(mynum)==META_SNAKE)
            )
        {
                bprintf(
"You &+Rcut&* the &+Cropes&* with your teeth.\n"
			);
		setobjstate(OBJ_META_ROPES, 1);
		return True;
	}

        if ( (obj == OBJ_META_ROPES)
        &&   (state(OBJ_META_MECHANISM_LEVER)==1)
        &&   (oloc(OBJ_META_DRAWBRIDGE_IN)==LOC_META_META14)
        &&   (pmetatype(mynum)==META_BIRD)
            )
        {
                bprintf(
"You &+Rcut&* the &+Cropes&* with your beak.\n"
			);
		setobjstate(OBJ_META_ROPES, 1);
                return True;
	}
return False;
}

void meta_pull(int x)
{
        switch (x)
        {
	case OBJ_META_BRANCH_MECHANISM:
	{
		if (state(x)==0)
		{
			if ( (state(OBJ_META_MECHANISM_LEVER)==0)
			&&   (state(OBJ_META_ROPES)==0)
			    )
			{
                                bprintf(
"You pull the &+Cbranch&* and the drawbridge lowers.\n"
                                        );
                                setobjstate(OBJ_META_DRAWBRIDGE_IN, 0);
                                setobjstate(OBJ_META_DRAWBRIDGE_OUT, 0);
                                setobjstate(OBJ_META_MECHANISM_LEVER, 1);
                                return;
                        }

                        if ( (state(OBJ_META_MECHANISM_LEVER)==1)
                        &&   (state(OBJ_META_ROPES)==0)
                            )

                        {
                                bprintf(
"You pull the &+Cbranch&* and the drawbridge raises.\n"
                                        );
                                setobjstate(OBJ_META_DRAWBRIDGE_IN, 1);
				setobjstate(OBJ_META_BRANCH_MECHANISM, 1);
                                setobjstate(OBJ_META_DRAWBRIDGE_OUT, 1);
                                setobjstate(OBJ_META_MECHANISM_LEVER, 0);
                                return;
                        }
                        if ( (state(OBJ_META_MECHANISM_LEVER)==0)
                        &&   (state(OBJ_META_ROPES)==1)
                            )
                        {
                                bprintf(
"The &+Cdrawbridge&* is destroyed, pulling the branch can do no good!\n"
                                        );
                                return;
                        }

                        if ( (state(OBJ_META_MECHANISM_LEVER)==1)
                        &&   (state(OBJ_META_ROPES)==1)
                            )
                        {
                                bprintf(
"You cannot raise the &+Cdrawbridge&*, the ropes have been cut!\n"
                                        );
                                return;
                        }
		}

		if (state(x)==1)
		{
                        if ( (state(OBJ_META_MECHANISM_LEVER)==0)
                        &&   (state(OBJ_META_ROPES)==0)
                            )
                        {
                                bprintf(
"You pull the &+Cbranch&*, which splinters and breaks from too much use.\n"
					);
				destroy(x);
				return;
			}
		}
	}
	break;
        case OBJ_META_STONE_BRANCH:

                if (state(x)==1)
		{
			if ( (state(OBJ_META_MECHANISM_LEVER)==0)
			&&   (state(OBJ_META_ROPES)==0)
			    )
			{
				bprintf(
"You pull the &+Cbranch&* and the drawbridge lowers.\n"
					);
				setobjstate(OBJ_META_DRAWBRIDGE_IN, 0);
				setobjstate(OBJ_META_DRAWBRIDGE_OUT, 0);
				setobjstate(OBJ_META_MECHANISM_LEVER, 1);
				return;
			}
		
			if ( (state(OBJ_META_MECHANISM_LEVER)==1)
                        &&   (state(OBJ_META_ROPES)==0)
                            )

			{
				bprintf(
"You pull the &+Cbranch&* and the drawbridge raises.\n"
					);
                                setobjstate(OBJ_META_DRAWBRIDGE_IN, 1);
                                setobjstate(OBJ_META_DRAWBRIDGE_OUT, 1);
				setobjstate(OBJ_META_MECHANISM_LEVER, 0);
                                return;
			}

                        if ( (state(OBJ_META_MECHANISM_LEVER)==0)
                        &&   (state(OBJ_META_ROPES)==1)
                            )
                        {
                                bprintf(
"The &+Cdrawbridge&* is destroyed, pulling the branch can do no good!\n"
					);
				return;
			}

			if ( (state(OBJ_META_MECHANISM_LEVER)==1)
			&&   (state(OBJ_META_ROPES)==1)
			    )
			{
				bprintf(
"You cannot raise the &+Cdrawbridge&*, the ropes have been cut!\n"
					);
				return;
			}
		}
	break;
	}
}

Boolean meta_climb(int obj)
{
	if ( (obj == OBJ_META_TREE)
	&&   (pmetatype(mynum)==META_SNAKE)
	    )
	{
		bprintf(
"You attempt to &+Bclimb&* the &+Ctree&*, but are beaten back by sentry\n"
"birds circling it.\n"
			);
		return True;
	}

        if ( (obj == OBJ_META_TREE)
        &&   (pmetatype(mynum)==META_FISH_ONLAND)
	    )
	{
		bprintf(
"You cannot &+Bclimb&* as a &+Cfish&*!\n"
			);
		return True;
	}
	
        if ( (obj == OBJ_META_TREE)
        &&   (pmetatype(mynum)==META_BEETLE)
	    )
	{
		bprintf(
"You quickly scurry up the &+Ctree&*, hoping not to be seen and it appears\n"
"that you will make it when you look up to see the enormous maw of a bird\n"
"looming over you! He snatches you off the branch, and swallows you in a quick\n"
"single gulp."
			);
    send_msg (DEST_ALL, MODE_QUIET, max (pvis (mynum), LVL_WIZARD), LVL_MAX,mynum,NOBODY,
"&+B[&+REaten Alive&+B]\n");
                p_crapup(mynum, "&+REaten by a bird!\n", CRAP_SAVE);
		return True;
	}

	if ( (obj == OBJ_META_TREE)
	&&   (pmetatype(mynum)==META_BIRD)
	    )
	{
		bprintf(
"You don't have to &+Bclimb&* as a bird, you can &+Cfly&*.\n"
			);
		return True;
	}

	if ( (obj == OBJ_META_TREE)
	&&   (pmetatype(mynum)==META_HUMAN)
	    )
	{
		bprintf(
"You try to shimmy up the &+Ctree&*, but the circumference is too great for you\n"
"to get a decent handhold.\n"
			);
		return True;
	}
return False;
}

Boolean meta_examine(int a)
{
	if (pl1 == mynum)
	{
		if(pmetatype(mynum)==META_HUMAN)
		{
			return False;
		}
		if((pmetatype(mynum)==META_SNAKE_UNDERWATER)
		|| (pmetatype(mynum)==META_SNAKE))
		{
			bprintf("You are currently a &+Gsnake&*.\n");
			return True;
		}
		if((pmetatype(mynum)==META_BEETLE)
		|| (pmetatype(mynum)==META_BEETLE_UNDERWATER))
		{
			bprintf("You are currently a &+Mbeetle&*.\n");
			return True;;
		}
		if(pmetatype(mynum)==META_BIRD)
		{
			bprintf("You are currently a &+Cbird&*.\n");
			return True;
		}
		if((pmetatype(mynum)==META_FISH_ONLAND)
		|| (pmetatype(mynum)==META_FISH_UNDERWATER))
		{
			bprintf("You are currently a &+Bfish&*.\n");
			return True;
		}
	return False;
	}

	if ( (a == OBJ_META_MECHANISM)
	&&   (state(OBJ_META_STONE_BRANCH)==1)
	    )
	{
		bprintf(
"This &+Mmechanism&* allows the &+ydrawbridge&* to be lowered, there's a &+Chole&* for a &+mlever&*\n"
"in the side, with a stone &+Cbranch&* in it.\n");
		return True;
	}

	if ( (a == OBJ_META_CHEST)
	&&   (state(a)==0)
	&&   (odamage(a)==1)
	    )
	{
		bprintf(
"This &+Cchest&* can hold a large amount, and is currently &+Gopen&* and &+Bupside down&*,\n"
"keeping all the &+Cair&* in.\n"
				);
		return True;
	}
	if ( (a == OBJ_META_CHEST2)
	&&   (state(a)==0)
	    )
	{
		bprintf(
"This &+ychest&* can hold a large amount, and is currently &+Gopen&*."
				);
		return True;
	}
	if ( (a == OBJ_META_GUN)
	&&   (pmetatype(mynum)==META_BIRD)
	    )
	{
		bprintf(
"The antique &+Cgun&* is affixed to the wall here. You should be able to fly up and empty it.\n"
				);
		return True;
	}

	if ( (a == OBJ_META_VINES)
	&&   (pmetatype(mynum)!=META_BIRD)
	    )
	{
		bprintf(
"The vines are just out of reach for you, and you are unable to give them\n"
"a proper examination.\n"
			);
		return True;
	}

        if ( (a == OBJ_META_VINES)
        &&   (pmetatype(mynum)==META_BIRD)
            )
        {
                bprintf(
"The vines are just out of reach for you, perhaps if you flew up, you could\n"
"get a better look.\n"
			);
		return True;
	}
	
	if ( (a == OBJ_META_CRACK)
	&&   (pmetatype(mynum)==META_BEETLE)
	    )
	{
		bprintf(
"The crack is tiny, barely large enough to allow a sliver of light through, but\n"
"you think you could fit under it.\n"
				);
		return True;
	}

	if ( (a == OBJ_META_TOME)
	&&   (state(OBJ_META_TOME)==0)
	    )
	{
		bprintf(
"An abridged copy of the history of the Dung Beetles, it tells of how\n"
"the Jal T'Voth and Nal Haseem beetle clans forged a mighty alliance,\n"
"utterly destroying all other beetle clans and claiming the dung of the\n"
"forest for themselves. The tome goes on to tell how the Nal Haseem were\n"
"not content with sharing their wealth and power, and greed overcame them.\n"
"They sacked and pillaged the lands of the Jal T'Voth and stole all their\n"
"dung, leaving but one piece, the sacred dung of the ages, for they could\n"
"carry no more.\n"
			);
		bprintf(
"\nSeeing your interest in the &+Ctome&*, the king scuttles over to you.\n"
"'&+MYou do not appear to be from around these parts, why is it that you\n"
"&+Mcome to our lands?&*' He stops you before you can reply, '&+MNever\n"
"&+Mmind, it is of no consequence. You are not of the Nal Haseem, that\n"
"&+Mmuch is obvious. Could it be then that you are a survivor from the\n"
"&+MOld Clans, come to seek retribution? Take heed, we are not your enemy,\n"
"&+Mthe Nal Haseem are our common foe. My people are weak and dying without\n"
"&+Many dung to eat, or we would attack them ourselves. I can only hope that\n"
"&+Myou can sneak into their encampment and retrieve our dung before they\n"
"&+Mnotice you. Godspeed to you, stranger, and may the gods smile on you.&*'\n"
			);
		setobjstate(a, 1);
		return True;
	}

return False;
}

void meta_beetle_area_die(void)
{
int pl;
 for (pl=0;pl<max_players;pl++)
 {
  if ( (pmetatype(pl)!=META_BEETLE)
  &&  (ploc(pl) <= LOC_META_META60)
  &&  (ploc(pl) >= LOC_META_META65)
  &&  (plev(pl) < LVL_ARCHWIZARD)
          )
	{
                send_msg (DEST_ALL, MODE_QUIET, max (pvis (pl),LVL_WIZARD), LVL_MAX,pl,NOBODY,
"&+B[&+RCrushed to Death&+B]\n"
                                );

		sendf(pl,
"You suddenly find yourself too &+Rlarge&* for this area and die as your rib cage\n"
"is crushed by the &+yearth&* around you.\n");
		p_crapup(pl, 
"&+RCrushed to death!\n", CRAP_SAVE
				);
	}
    }
}
Boolean meta_bite(void)
{
	if ( (pl1 == MOB_META_FLOPSY+max_players)
	&&   (pmetatype(mynum)==META_SNAKE)
	&&   (ploc(mynum)==ploc(MOB_META_FLOPSY+max_players))
	&&   (pnumobs(mynum)==1)
	    )
	{
		bprintf(
"&+CFlopsy&* shrieks in &+Rterror&* and runs away, screaming for her siblings to follow."
				);
		setploc(MOB_META_FLOPSY+max_players, LOC_LIMBO_LIMBO);
		setploc(MOB_META_MOPSY+max_players, LOC_LIMBO_LIMBO);
		setploc(MOB_META_COTTONTAIL+max_players, LOC_LIMBO_LIMBO);
		return True;
	}

	if ( (pl1 == MOB_META_MOPSY+max_players)
	&&   (pmetatype(mynum)==META_SNAKE)
	&&   (ploc(mynum)==ploc(MOB_META_MOPSY+max_players))
	&&   (pnumobs(mynum)==1)
	    )
	{
		bprintf(
"&+CMopsy&* shrieks in &+Rterror&* and runs away, screaming for her siblings to follow."
				);
		setploc(MOB_META_FLOPSY+max_players, LOC_LIMBO_LIMBO);
		setploc(MOB_META_MOPSY+max_players, LOC_LIMBO_LIMBO);
		setploc(MOB_META_COTTONTAIL+max_players, LOC_LIMBO_LIMBO);
		return True;
	}
	if ( (pl1 == MOB_META_COTTONTAIL+max_players)
	&&   (pmetatype(mynum)==META_SNAKE)
	&&   (ploc(mynum)==ploc(MOB_META_COTTONTAIL+max_players))
	&&   (pnumobs(mynum)==1)
	    )
	{
		bprintf(
"You &+Rbite&* down hard on &+CCottontail's&* neck, biting clean through her spine,\n"
"killing her instantly.\n"
				);
		setploc(MOB_META_COTTONTAIL+max_players, LOC_LIMBO_LIMBO);
		setoloc(OBJ_META_COTTONTAIL, ploc(mynum), IN_ROOM);
		return True;
	}
        if ( ( (pl1 == MOB_META_FLOPSY+max_players)
        ||     (pl1 == MOB_META_MOPSY+max_players)
        ||     (pl1 == MOB_META_COTTONTAIL+max_players)
              )
        &&   (pmetatype(mynum)==META_SNAKE)
        &&   (ploc(mynum)==ploc(pl1))
        &&   (pnumobs(mynum)>1)
            )
        {
                bprintf("You cannot &+Rbite&* with something in your mouth.\n");
                return True;
        }

return False;
}

Boolean meta_skin(int obj)
{
	if (pmetatype(mynum)!=META_BIRD)
	{
		bprintf("You aren't equipped for &+Cskinning&* anything.\n");
		return True;
	}
	
	if ( (pmetatype(mynum)==META_BIRD)
	&&   (obj == OBJ_META_COTTONTAIL)
	&&   (ploc(mynum)==oloc(obj))
	    )
	{
		bprintf("You expertly &+Cskin&* Cottontail's corpse.\n");
		destroy(obj);
		setoloc(OBJ_META_SKIN, ploc(mynum), IN_ROOM);
		return True;
	}
return False;
}

Boolean meta_kick(void)
{
	if (pmetatype(mynum)==META_SNAKE)
	{
		bprintf(
"You can't kick as a &+Gsnake&*!\n"
				);
		return True;
	}
return False;
}
Boolean meta_cast_spell(int plr)
{
 if ( (pmetatype(mynum)!=META_HUMAN)
 &   (plr!=MOB_META_SNAKE_BEGGAR+max_players)
     )
 {
  bprintf(
"Your &+Ctiny&* brain can't cope with the &+Mmagic&* and the spell &+Rsputters&* out.\n"
    );
  return True;
 }
 if ( (plr==MOB_META_SNAKE_BEGGAR+max_players)
 &   (iscarrby(OBJ_META_PEBBLE1, plr))
 &   (iscarrby(OBJ_META_PEBBLE2, plr))
 &   (iscarrby(OBJ_META_PEBBLE3, plr))
     )
 {
  bprintf(
"As you go to attack the &+Cbeggar&* he screams in &+Rpanic&* and flees,\n"
"dropping one of his &+Cpebbles&* in his haste to get away.\n"
   );
  setploc(plr, LOC_LIMBO_LIMBO);
  setoloc(OBJ_META_PEBBLE1, ploc(mynum), IN_ROOM);
  return True;
 }
        if ( (plr==MOB_META_SNAKE_BEGGAR+max_players)
        &   (!iscarrby(OBJ_META_PEBBLE1, plr))
        &   (iscarrby(OBJ_META_PEBBLE2, plr))
        &   (iscarrby(OBJ_META_PEBBLE3, plr))
            )
        {
                bprintf(
"As you move to attack the &+Cbeggar&* he screams in &+Rpanic&* and flees,\n"
"dropping one of his &+Cpebbles&* in his haste to get away.\n"
                        );
                setploc(plr, LOC_LIMBO_LIMBO);
                setoloc(OBJ_META_PEBBLE2, ploc(mynum), IN_ROOM);
                return True;
        }
        if ( (plr==MOB_META_SNAKE_BEGGAR+max_players)
        &   (!iscarrby(OBJ_META_PEBBLE1, plr))
        &   (!iscarrby(OBJ_META_PEBBLE2, plr))
        &   (iscarrby(OBJ_META_PEBBLE3, plr))
            )
        {
                bprintf(
"As you move to attack the &+Cbeggar&* he screams in &+Rpanic&* and flees,\n"
"dropping his last &+Cpebble&* in his haste to get away.\n"
                        );
                setploc(plr, LOC_LIMBO_LIMBO);
                setoloc(OBJ_META_PEBBLE3, ploc(mynum), IN_ROOM);
                return True;
        }
return False;
}

Boolean meta_wound(int attacker, int victim, int weap)
{
        if ( (victim==MOB_META_CHIMERA+max_players)
            )
        {
		if ( (weap == OBJ_META_STAFF)
		&&   (pmetatype(attacker)==META_HUMAN)
		&&   (pstr(victim)>50)
		    )
		{
			sendf(attacker,
"The &+CChimera&* shrieks in pain at the sight of the staff, seeming to wither\n"
"before your very eyes.\n"
                        );
                setpstr(victim, 50);
		setpdam(victim, 15);
		return True;
		}
                if (pmetatype(attacker)!=META_HUMAN)
                {
                        sendf(attacker,
"The &+CChimera&* laughs at you with impunity and crushes you with a single massive\n"
"hairy paw.\n"
                	        );
                        send_msg (DEST_ALL, MODE_QUIET, max (pvis (attacker),
LVL_WIZARD),LVL_MAX,attacker,NOBODY,
"&+B[&+RBit Off More Than %s Could Swallow&+B]\n", he_or_she(attacker));
                	p_crapup(attacker, "&+RCrushed by the Chimera!\n", CRAP_SAVE);
              		return True;
                 }
	return False;
	}
return False;
}

Boolean meta_exits(void)
{

	if ( (ploc(mynum)==LOC_META_META1)
	&&   (pmetatype(mynum)!=META_BEETLE)
	&&   (pmetatype(mynum)!=META_SNAKE)
	&&   (plev(mynum)<LVL_WIZARD)
	    )
	{
		bprintf(
"&+GNorth&+M:&+C Rainbow Clearing\n"
"&+G East&+M:&+C In the Forest\n"
"&+GSouth&+M:&+C Toadstool Clearing\n"
" &+GWest&+M:&+C In the Forest\n"
	);
		return True;
	}

        if ( (ploc(mynum)==LOC_META_META35)
        &&   (pmetatype(mynum)!=META_BEETLE)
        &&   (pmetatype(mynum)!=META_SNAKE)
        &&   (plev(mynum)<LVL_WIZARD)
            )
        {
                bprintf(
"&+GNorth&+M:&+C The Tunnel\n"
"&+G   Up&+M:&+C A Woodland Clearing\n"
			);
		return True;
	}

        if ( (ploc(mynum)==LOC_META_META70)
        &&   (pmetatype(mynum)!=META_BEETLE)
        &&   (plev(mynum)<LVL_WIZARD)
            )
        {
                bprintf(
"&+CNone...\n"
			);
		return True;
	}

        if ( (ploc(mynum)==LOC_META_META71)
        &&   (pmetatype(mynum)!=META_BEETLE)
        &&   (pmetatype(mynum)!=META_SNAKE)
        &&   (plev(mynum)<LVL_WIZARD)
            )
        {
                bprintf(
"&+CNone...\n"
                        );
                return True;
        }

        if ( (ploc(mynum)==LOC_META_META6)
        &&   (pmetatype(mynum)!=META_BEETLE)
        &&   (plev(mynum)<LVL_WIZARD)
            )
        {
                bprintf(
"&+GNorth&+M:&+C The Forest Continues\n"
"&+GSouth&+M:&+C Forbidding Clearing in the Woods\n"
"&+G West&+M:&+C In the Forest\n"
			);
		return True;
	}

	if ( (ploc(mynum)==LOC_META_META59)
	&&   (pmetatype(mynum)!=META_BEETLE)
	&&   (plev(mynum)<LVL_WIZARD)
	    )
	{
		bprintf(
"&+G East&+M:&+C Beneath the Tree\n"
			);
		return True;
	}

	if ( (ploc(mynum)==LOC_META_META66)
	&&   (pmetatype(mynum)!=META_BEETLE)
	&&   (plev(mynum)<LVL_WIZARD)
	    )
	{
		bprintf(
"&+GNorth&+M:&+C Farmyard\n"
			);
		return True;
	}

	if ( (ploc(mynum)==LOC_META_META69)
	&&   (pmetatype(mynum)!=META_BEETLE)
	&&   (pmetatype(mynum)!=META_SNAKE)
	&&   (plev(mynum)<LVL_WIZARD)
	    )
	{
		bprintf(
"&+G East&+M:&+C Farmyard\n"
			);
		return True;
	}

        if ( (ploc(mynum)==LOC_META_META68)
        &&   (pmetatype(mynum)!=META_BEETLE)
        &&   (plev(mynum)<LVL_WIZARD)
            )
        {
                bprintf(
"&+G West&+M:&+C Farmyard\n"
			);
		return True;
	}

	if ( (ploc(mynum)==LOC_META_META20)
	&&   ( (pmetatype(mynum)==META_BEETLE_UNDERWATER)
	||     (pmetatype(mynum)==META_SNAKE_UNDERWATER)
	||     (plev(mynum)>=LVL_WIZARD)
	      )
	    )
	{
		bprintf(
"&+GNorth&+M:&+C The High Stream\n"
"&+G East&+M:&+C Hidden Crack\n"
"&+G West&+M:&+C Up the Stream\n"
"&+G   Up&+M:&+C The Crystal Pool\n"
			);
		return True;
	}

return False;
}

Boolean meta_fill(int obj)
{
int a;
	if ( (obj == OBJ_META_BOTTLE)
	&&   (iscontin(OBJ_META_LIMESTONE, obj))
	&&   ( (ploc(mynum)==LOC_META_META8)
	||     (ploc(mynum)==LOC_META_META2)
	||     (ploc(mynum)==LOC_META_META5)
	      )
	&&   (!iscontin(OBJ_META_WATER, obj))
      &&   (!iscontin(OBJ_META_WATER_2, obj))
      &&   (!iscontin(OBJ_META_WATER_3, obj))
      &&   (!iscontin(OBJ_META_WATER_4, obj))
	    )
	{
		bprintf(
"You fill the &+C%s&* with &+Bwater&*.\n"
"The &+Climestone&* dissolves, creating an alkaline solution.\n", oname(obj)
			);
		destroy(OBJ_META_LIMESTONE);
		setoloc(OBJ_META_ALKALINE, obj, IN_CONTAINER);
		return True;
	}

	if ( ( ( (obj == OBJ_META_BOTTLE)
	&&       (!iscontin(OBJ_META_LIMESTONE, obj))
	&&       (!iscontin(OBJ_META_ALKALINE, obj))
	        )  
	||     (obj == OBJ_META_FLASK_EMPTY)
	      )
	&&   ( (ploc(mynum)==LOC_META_META8)
	||     (ploc(mynum)==LOC_META_META2)
	||     (ploc(mynum)==LOC_META_META5)
	      )
	&&   (!iscontin(OBJ_META_WATER, obj))
      &&   (!iscontin(OBJ_META_WATER_2, obj))
      &&   (!iscontin(OBJ_META_WATER_3, obj))
      &&   (!iscontin(OBJ_META_WATER_4, obj))
	    )
	{
		bprintf(
"You fill the &+C%s&* with &+Bwater&*.\n", oname(obj)
			);
		for (a=OBJ_META_WATERMARKER;a<OBJ_META_WATERMARKER2;a++)
		{
			if ( (oloc(a)==LOC_LIMBO_LIMBO)
			&&   (!otstbit(a, OFL_NOGET)))
			{
				setoloc(a, obj, IN_CONTAINER);
				return True;
			}
			if ( (otstbit(a, OFL_DESTROYED))
                        &&   (!otstbit(a, OFL_NOGET)))
			{
				oclrbit(a, OFL_DESTROYED);
				setoloc(a, obj, IN_CONTAINER);
				return True;
			}
		}
	return True;
	}

	if ( (( (obj == OBJ_META_BOTTLE)
	&&     (!iscontin(OBJ_META_LIMESTONE, obj))
	      ) 
	||    (obj == OBJ_META_FLASK_EMPTY))
        &&   ( (iscontin(OBJ_META_WATER, obj))
        ||     (iscontin(OBJ_META_WATER_2, obj))
        ||     (iscontin(OBJ_META_WATER_3, obj))
        ||     (iscontin(OBJ_META_WATER_4, obj))
              )
	&&   ( (ploc(mynum)==LOC_META_META8)
	||     (ploc(mynum)==LOC_META_META2)
	||     (ploc(mynum)==LOC_META_META5)
	      )
	     )
        {
		bprintf("There is already &+Bwater&* in it.\n");
		return True;
	}

	if ( (obj == OBJ_META_BOTTLE)
	&&   (iscontin(OBJ_META_ALKALINE, obj))
	&&   ( (ploc(mynum)==LOC_META_META2)
	||     (ploc(mynum)==LOC_META_META5)
	||     (ploc(mynum)==LOC_META_META8)
	      )
	    )
	{
		bprintf(
"Putting more &+Bwater&* in the &+Cbottle&* would &+Mdilute&* the&+C solution&* too much.\n"
			);
		return True;
	}

        if ( ( (obj == OBJ_META_BOTTLE)
        ||     (obj == OBJ_META_FLASK_EMPTY)
              )
	&&   ( (ploc(mynum)!=LOC_META_META8)
	&&     (ploc(mynum)!=LOC_META_META2)
	&&     (ploc(mynum)!=LOC_META_META5)
	      )
	    )
	{
		bprintf("There is nothing to &+Cfill&* it from.\n");
		return True;
	}
return False;
}

Boolean meta_pour(int obj)
{
int a, b;
	if ( (obj == OBJ_META_BOTTLE)
	&&   (EMPTY(item2))
	&&   (iscontin(OBJ_META_ALKALINE, obj))
	    )
	{
		bprintf(
"You &+Cpour&* the &+Malkaline &+Csolution&* on the &+gground&*. It seeps into the &+yearth&*\n"
"and is soon gone.\n"
			);
		destroy(OBJ_META_ALKALINE);
		return True;
	}

	if ( (obj == OBJ_META_BOTTLE)
	&&   ( (iscontin(OBJ_META_WATER, obj))
	||     (iscontin(OBJ_META_WATER_2, obj))
	||     (iscontin(OBJ_META_WATER_3, obj))
	||     (iscontin(OBJ_META_WATER_4, obj))
	      )
	&&   (EMPTY(item2))
	    )
	{
		bprintf(
"You &+Cpour&* the &+Cwater&* on the &+gground&*. It seeps into the &+yearth&* and is soon gone.\n"
			);
		for (a = ofirst_obj (obj); a != SET_END; a = onext_obj (obj))
		{
			if ((a==OBJ_META_WATER)
			||  (a==OBJ_META_WATER_2)
			||  (a==OBJ_META_WATER_3)
			||  (a==OBJ_META_WATER_4)
			   )
			{
				destroy(a);
			}
		}
	return True;
	}

        if ( (obj == OBJ_META_FLASK_EMPTY)
        &&   ( (iscontin(OBJ_META_WATER, obj))
        ||     (iscontin(OBJ_META_WATER_2, obj))
        ||     (iscontin(OBJ_META_WATER_3, obj))
        ||     (iscontin(OBJ_META_WATER_4, obj))
              )
        &&   (EMPTY(item2))
            )
        {
                bprintf(
"You &+Cpour&* the &+Cwater&* on the &+gground&*. It seeps into the &+yearth&* and is soon gone.\n"
                        );
                for (a = ofirst_obj (obj); a != SET_END; a = onext_obj (obj))
                {
                        if ((a==OBJ_META_WATER)
                        ||  (a==OBJ_META_WATER_2)
                        ||  (a==OBJ_META_WATER_3)
                        ||  (a==OBJ_META_WATER_4)
                           )
                        {
                                destroy(a);
                        }
                }
        return True;
        }
        if ( (obj == OBJ_META_FLASK_EMPTY)
        &&   ( (iscontin(OBJ_META_WATER, obj))
        ||     (iscontin(OBJ_META_WATER_2, obj))
        ||     (iscontin(OBJ_META_WATER_3, obj))
        ||     (iscontin(OBJ_META_WATER_4, obj))
              )
	&&   (otstbit(ob2, OFL_CONTAINER))
	&&   (state(ob2)==0)
	&&   (ob2!=OBJ_META_CAULDRON)
            )
        {
		bprintf(
"You &+Cpour &*the &+Bwater&* into the &+C%s&*, and watch it leak out and seep away\n"
"into the &+gground&*.\n"
			);
	        for (a = ofirst_obj (obj); a != SET_END; a = onext_obj (obj))
                {
                        if ((a==OBJ_META_WATER)
                        ||  (a==OBJ_META_WATER_2)
                        ||  (a==OBJ_META_WATER_3)
                        ||  (a==OBJ_META_WATER_4)
                           )
                        {
                                destroy(a);
                        }
                }
        return True;
        }

        if ( (obj == OBJ_META_BOTTLE)
        &&   (ob2 == OBJ_META_CAULDRON)
        &&   (iscontin(OBJ_META_ALKALINE, obj))
        &&   (iscontin(OBJ_META_SCALE,ob2))
        &&   (iscontin(OBJ_META_DUNG_OLD,ob2))
        &&   (iscontin(OBJ_META_SILVER_FEATHER,ob2))
        &&   (iscontin(OBJ_META_DIAMOND,ob2))
        &&   (otstbit(OBJ_META_KINDLING, OFL_LIT))
            )
        {
                destroy(OBJ_META_ALKALINE);
                bprintf(
"As you add the &+Calkaline&* solution to the cauldron, the ingredients\n"
"shimmer and merge into a deep &+rcrimson&* liquid which gathers in the\n"
"bottom of the cauldron.\n"
                                );
                for (b = ofirst_obj (ob2); b != SET_END; b = onext_obj (ob2))
                {
                        if (iscontin (b, ob2))
                        {
                                destroy(b);
                        }
                }
                setoloc(OBJ_META_LIQUID_HUMAN, OBJ_META_CAULDRON, IN_CONTAINER);
                return True;
        }
        if ( (obj == OBJ_META_BOTTLE)
        &&   (ob2 == OBJ_META_CAULDRON)
        &&   (iscontin(OBJ_META_ALKALINE, obj))
        &&   ( ( (!iscontin(OBJ_META_SCALE,ob2))
        ||       (!iscontin(OBJ_META_DUNG_OLD,ob2))
        ||       (!iscontin(OBJ_META_SILVER_FEATHER,ob2))
        ||       (!iscontin(OBJ_META_DIAMOND,ob2))
	        )
        ||     (otstbit(OBJ_META_KINDLING, OFL_LIT))
              )
	    )
        {
		bprintf(
"You &+Cpour&* the &+Calkaline solution&* into the &+Ccauldron&*.\n");
		setoloc(OBJ_META_ALKALINE, OBJ_META_CAULDRON, IN_CONTAINER);
		return True;
	}

        if ( (obj == OBJ_META_BOTTLE)
        &&   (ob2 == OBJ_META_CAULDRON)
        &&   (iscontin(OBJ_META_ALKALINE, obj))
        &&   ( (!iscontin(OBJ_META_SCALE,ob2))
        &&     (!iscontin(OBJ_META_DUNG_OLD,ob2))
        &&     (!iscontin(OBJ_META_SILVER_FEATHER,ob2))
        &&     (!iscontin(OBJ_META_DIAMOND,ob2))
              )
	    )
        {
                bprintf(
"You &+Cpour&* the &+Calkaline solution&* into the &+Ccauldron&*.\n");
                setoloc(OBJ_META_ALKALINE, OBJ_META_CAULDRON, IN_CONTAINER);
                return True;
        }

	if ( (obj == OBJ_META_BOTTLE)
	&&   (ob2 == OBJ_META_CAULDRON)
	&&   ( (iscontin(OBJ_META_WATER, obj))
	||     (iscontin(OBJ_META_WATER_2, obj))
	||     (iscontin(OBJ_META_WATER_3, obj))
	||     (iscontin(OBJ_META_WATER_4, obj))
	      )
	    )
	{
		if (iscontin(OBJ_META_ALKALINE, ob2))
		{
			bprintf(
"The &+Ccauldron&* shudders and coughs the &+Bwater&* back up.\n"
"It &+Mdrains&* away into the &+yearth&* and is soon gone.\n"
				);
			for (b = ofirst_obj (obj); b != SET_END; b = onext_obj (obj))
			{
				if ( (b > OBJ_META_WATERMARKER)
				&&   (b < OBJ_META_WATERMARKER2)
				    )
				{
					destroy(obj);
				}
			}
		return True;
		}

		if (!iscontin(OBJ_META_ALKALINE, ob2))
		{
			if (!iscontin(OBJ_META_LIMESTONE, ob2))
			{
                                bprintf(
"You &+Cpour&* the &+Bwater&* into the cauldron.\n"
					);
                        for (b = ofirst_obj (obj); b != SET_END; b = onext_obj (obj))
                        {
                                if ( (b > OBJ_META_WATERMARKER)
                                &&   (b < OBJ_META_WATERMARKER2)
                                    )
                                {
					setoloc(b, OBJ_META_CAULDRON, IN_CONTAINER);
				}
			}
			}

			if (iscontin(OBJ_META_LIMESTONE, ob2))
			{
				bprintf(
"You &+Cpour&* the &+Bwater&* into the cauldron.\n"
"The &+Climestone&* dissolves in the &+Bwater&*.\n"
					);
				destroy(OBJ_META_LIMESTONE);
                        for (b = ofirst_obj (obj); b != SET_END; b = onext_obj (obj))
                        {
                                if ( (b > OBJ_META_WATERMARKER)
                                &&   (b < OBJ_META_WATERMARKER2)
                                    )
                                {
                                        destroy(b);
                                }
			}
			setoloc(OBJ_META_ALKALINE, OBJ_META_CAULDRON, IN_CONTAINER);

				if ( (iscontin(OBJ_META_SCALE, ob2))
				&&   (iscontin(OBJ_META_DUNG_OLD, ob2))
				&&   (iscontin(OBJ_META_SILVER_FEATHER, ob2))
				&&   (iscontin(OBJ_META_DIAMOND, ob2))
				&&   (otstbit(OBJ_META_KINDLING, OFL_LIT))
				    )
				{
               				 bprintf(
"As you add the &+Calkaline&* solution to the cauldron, the ingredients\n"
"shimmer and merge into a deep &+rcrimson&* liquid which gathers in the\n"
"bottom of the cauldron.\n"
                                );

	
        		        for (b = ofirst_obj (ob2); b != SET_END; b = onext_obj (ob2))
              			{
                       			if (iscontin (b, ob2))
                       			{
                               			destroy(b);
                        		}
				}
		                setoloc(OBJ_META_LIQUID_HUMAN, OBJ_META_CAULDRON, IN_CONTAINER);
			}
		}
	return True;
	}
}
return False;
}
#endif

