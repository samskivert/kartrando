/**
 * Kart Rando! A handy Palm Pilot application for choosing random
 * sequences of Mario Kart 64 tracks to be played by randomly chosen
 * characters. It saves a whole lot of writing down and/or getting up and
 * walking to the computer between races.
 *
 * $Id$
 */

#pragma pack(2)

#include <Common.h>
#include <System/SysAll.h>
#include <UI/UIAll.h>

#include "kartrando_ids.h"

#define KartRandoAppID  'KRnd'
#define KartRandoDBType 'Data'

/*  static char* characters[] = */
/*  { "Mario", "Luigi", "Peach", "Toad", */
/*    "Yoshi", "D.K.", "Wario", "Bowser" }; */

static char* characters[] =
{ "Ma", "Lu", "Pe", "To", "Yo", "DK", "Wa", "Bo" };

#define CHAR_COUNT 8

static char* tracks[] =
{ "Luigi Raceway", "Moo Moo Farm", "Koopa Troopa Beach", "Kalimari Desert",
  "Toad's Turnpike", "Frappe Snowland", "Choco Mountain", "Mario Raceway",
  "Wario Stadium", "Sherbet Land", "Royal Raceway", "Bowser's Castle",
  "D.K.'s Jungle Parkway", "Yoshi Valley", "Banshee Boardwalk",
  "Rainbow Road" };

#define TRACK_COUNT 16

static ListPtr listPtr_races;

static ControlPtr buttonPtr_players2;
static ControlPtr buttonPtr_players3;
static ControlPtr buttonPtr_players4;

static ControlPtr buttonPtr_races3;
static ControlPtr buttonPtr_races5;
static ControlPtr buttonPtr_races7;
static ControlPtr buttonPtr_races9;

static char* racestrs[11];
static char racedata[11*128];

static UInt players = 2;
static UInt races = 5;

static UInt repeats = 1;
static UInt extra = 0;
static UInt rainbow = 0;

static int
StartApplication (void)
{
    int i;

    // initialize the race strings data
    for (i = 0; i < 11; i++) {
	racestrs[i] = racedata+(128*i);
    }

    FrmGotoForm(fid_kartrando);
    return 0;
}

static void
StopApplication (void)
{
}

static void
rando ()
{
    int ccount, cskip, tcount = TRACK_COUNT, tskip;
    int r, p, c, t;
    char* tmptracks[TRACK_COUNT];
    char* chars[CHAR_COUNT];

    // if we're not allowing rainbow road, decrement the tcount to 15 as
    // if the track doesn't exist
    if (!rainbow) tcount--;

    // make a temporary copy of the tracks array
    for (r = 0; r < TRACK_COUNT; r++) {
	tmptracks[r] = tracks[r];
    }

    // now select some random tracks
    for (r = 0; r < races; r++) {
	ccount = CHAR_COUNT;

	// make a temporary copy of the characters array
	for (c = 0; c < CHAR_COUNT; c++) {
	    chars[c] = characters[c];
	}

	for (p = 0; p < players; p++) {
	    // pick a random position
	    cskip = SysRandom(0) % ccount;

	    // now iterate over the list skipping already chosen characters
	    for (c = 0; c < cskip; c++) {
		while (chars[c] == 0) c++;
	    }

	    // skip any trailing used characters
	    while (chars[c] == 0) c++;

	    // decrement the character count for next time
	    ccount--;

	    if (p > 0) {
		StrCat(racestrs[r], chars[c]);
	    } else {
		StrCopy(racestrs[r], chars[c]);
	    }

	    if (p < players-1) {
		StrCat(racestrs[r], " ");
	    }

	    // mark the player as used
	    chars[c] = 0;
	}

	// pick a random position
	if (repeats) {
	    t = SysRandom(0) % tcount;

	} else {
	    tskip = SysRandom(0) % tcount;

	    // now iterate over the list skipping already chosen characters
	    for (t = 0; t < tskip; t++) {
		while (tmptracks[t] == 0) t++;
	    }

	    // skip any trailing used characters
	    while (tmptracks[t] == 0) t++;

	    // decrement the character count for next time
	    tcount--;
	}

	StrCat(racestrs[r], ": ");

	if (extra) {
	    StrCat(racestrs[r], SysRandom(0) % 100 > 50 ? "* " : "");
	}

	StrCat(racestrs[r], tmptracks[t]);

	// clear out this track if we're allowing no repeats
	if (!repeats) {
	    tmptracks[t] = 0;
	}
    }

    // update the list with the new choices
    LstSetListChoices(listPtr_races, &racestrs[0], races);

    // clear the list selection
    LstSetSelection(listPtr_races, -1);

    // and draw it
    LstDrawList(listPtr_races);
}

static Boolean
mainEventHandler (EventPtr event)
{
    int handled = 0;
    FormPtr form;
    ControlPtr active = 0;

    switch (event->eType) {
    case frmOpenEvent:
	form = FrmGetActiveForm();
	listPtr_races =
	    FrmGetObjectPtr(form, FrmGetObjectIndex(form, lid_races));

	// clear the list selection
	LstSetSelection(listPtr_races, -1);

	// set up the player and races selection buttons
	buttonPtr_players2 =
	    FrmGetObjectPtr(form, FrmGetObjectIndex(form, pbid_players2));
	buttonPtr_players3 =
	    FrmGetObjectPtr(form, FrmGetObjectIndex(form, pbid_players3));
	buttonPtr_players4 =
	    FrmGetObjectPtr(form, FrmGetObjectIndex(form, pbid_players4));
	buttonPtr_races3 =
	    FrmGetObjectPtr(form, FrmGetObjectIndex(form, pbid_races3));
	buttonPtr_races5 =
	    FrmGetObjectPtr(form, FrmGetObjectIndex(form, pbid_races5));
	buttonPtr_races7 =
	    FrmGetObjectPtr(form, FrmGetObjectIndex(form, pbid_races7));
	buttonPtr_races9 =
	    FrmGetObjectPtr(form, FrmGetObjectIndex(form, pbid_races9));

	// activate the appropriate options
	switch (players) {
	case 2: active = buttonPtr_players2; break;
	case 3: active = buttonPtr_players3; break;
	case 4: active = buttonPtr_players4; break;
	}

	if (active != 0) CtlSetValue(active, 1);

	switch (races) {
	case 3: active = buttonPtr_races3; break;
	case 5: active = buttonPtr_races5; break;
	case 7: active = buttonPtr_races7; break;
	case 9: active = buttonPtr_races9; break;
	}

	if (active != 0) CtlSetValue(active, 1);

	FrmDrawForm(form);
	handled = 1;
	break;

    case ctlSelectEvent:  // a button was pressed and released
	switch (event->data.ctlSelect.controlID) {
	case pbid_players2: players = 2; handled = 1; break;
	case pbid_players3: players = 3; handled = 1; break;
	case pbid_players4: players = 4; handled = 1; break;

	case pbid_races3: races = 3; handled = 1; break;
	case pbid_races5: races = 5; handled = 1; break;
	case pbid_races7: races = 7; handled = 1; break;
	case pbid_races9: races = 9; handled = 1; break;

	case bid_rando: rando(); handled = 1; break;
	}
	break;

    case menuEvent:
	switch (event->data.menu.itemID) {
	case miid_about:
	    FrmAlert(aid_about);
	    break;

	case miid_prefs:
	    FrmGotoForm(fid_prefs);
	    break;
	}
	handled = 1;
	break;

    default:
	break;
    }

    return handled;
}

static Boolean
prefsEventHandler (EventPtr event)
{
    int handled = 0;
    FormPtr form;
    ControlPtr ptr;

    switch (event->eType) {
    case frmOpenEvent:
	form = FrmGetActiveForm();
	ptr = FrmGetObjectPtr(form, FrmGetObjectIndex(form, cbid_repeats));
	CtlSetValue(ptr, repeats);
	ptr = FrmGetObjectPtr(form, FrmGetObjectIndex(form, cbid_rainbow));
	CtlSetValue(ptr, rainbow);
	ptr = FrmGetObjectPtr(form, FrmGetObjectIndex(form, cbid_extra));
	CtlSetValue(ptr, extra);
	FrmDrawForm(form);
	handled = 1;
	break;

    case ctlSelectEvent:  // a button was pressed and released
	switch (event->data.ctlSelect.controlID) {
	case cbid_repeats:
	    repeats = event->data.ctlSelect.on; handled = 1; break;
	case cbid_rainbow:
	    rainbow = event->data.ctlSelect.on; handled = 1; break;
	case cbid_extra:
	    extra = event->data.ctlSelect.on; handled = 1; break;

	case bid_ok:
	    FrmGotoForm(fid_kartrando); handled = 1; break;

	case bid_repeats_help: FrmAlert(aid_repeats_help); break;
	case bid_rainbow_help: FrmAlert(aid_rainbow_help); break;
	case bid_extra_help: FrmAlert(aid_extra_help); break;
	}
	break;

    default:
	break;
    }

    return handled;
}

static void
EventLoop (void)
{
    short err;
    int fid;
    FormPtr form;
    EventType event;

    do
    {
	EvtGetEvent(&event, 200);

	// let the system handle it's own events
	if (SysHandleEvent(&event)) continue;

	// same for menu events
	if (MenuHandleEvent((void *)0, &event, &err)) continue;

	// set up our handler when our main form is loaded
	if (event.eType == frmLoadEvent) {
	    fid = event.data.frmLoad.formID;
	    form = FrmInitForm(fid);
	    FrmSetActiveForm(form);

	    switch (fid) {
	    case fid_kartrando:
		FrmSetEventHandler(form, mainEventHandler);
		break;
	    case fid_prefs:
		FrmSetEventHandler(form, prefsEventHandler);
		break;
	    }
	}

	FrmDispatchEvent(&event);

    } while (event.eType != appStopEvent);
}

DWord
PilotMain (Word cmd, Ptr cmdPBP, Word launchFlags)
{
    int error;

    if (cmd == sysAppLaunchCmdNormalLaunch) {
	error = StartApplication();
	if (error) return error;
	EventLoop();
	StopApplication();
    }

    return 0;
}
