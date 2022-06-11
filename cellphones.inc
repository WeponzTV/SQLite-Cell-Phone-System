/*
	> Project: [Include] SQLite Cell Phone System v1.0 (SA-MP)
	> Released: June 2022
	> Developer(s): Weponz + realdiegopoptart
	> Functions:

	1) GetPlayerPhone(playerid); - Check if the player has a cell phone.
	2) GivePlayerPhone(playerid); - Give the player a cell phone.
	3) RemovePlayerPhone(playerid); - Remove the player's cell phone.
	4) ShowPlayerPhone(playerid); - Show the player their cell phone.

	*** As Seen On YouTube: https://www.youtube.com/WeponzTV ***
*/
#if !defined _samp_included
	#error Please include <a_samp> before <cellphones>.
#endif

#define PHONE_DB_LOCATION "cellphones.db"

#define PHONE_SHOW_HOMEPAGE_DIALOG 10001
#define PHONE_SHOW_CONTACTS_DIALOG 10002
#define PHONE_SHOW_MESSAGES_DIALOG 10003

#define PHONE_LIST_CONTACTS_DIALOG 10004
#define PHONE_ADD_CONTACTS_DIALOG 10005
#define PHONE_DELETE_CONTACTS_DIALOG 10006
#define PHONE_VERIFY_ADD_DIALOG 10007
#define PHONE_VERIFY_DELETE_DIALOG 10008

#define PHONE_LIST_MESSAGES_DIALOG 10009
#define PHONE_SEND_MESSAGES_DIALOG 10010
#define PHONE_WRITE_MESSAGES_DIALOG 10011
#define PHONE_INBOX_MESSAGES_DIALOG 10012
#define PHONE_OUTBOX_MESSAGES_DIALOG 10013
#define PHONE_INBOX_READ_DIALOG 10014
#define PHONE_INBOX_DELETE_DIALOG 10015
#define PHONE_OUTBOX_READ_DIALOG 10016
#define PHONE_OUTBOX_DELETE_DIALOG 10017

forward OnAlertDisappear(playerid);

new DB:phone_database;
new DBResult:phone_result;

new Text:phone_textdraw;

new large_string1[5120];
new large_string2[5120];

enum phone_data
{
	phone_user_id,
	phone_contact_id,
	phone_message_id,
	phone_number[16],
	phone_subject[32],
	phone_owned,
	phone_alert_timer,
	PlayerText:phone_alerts
};
new PhoneData[MAX_PLAYERS][phone_data];

stock SetPlayerNameEx(playerid, name[])
{
	new query[128];
    format(query, sizeof(query), "UPDATE `PHONES` SET `OWNER` = '%q' WHERE `USER_ID` = '%i'", name, PhoneData[playerid][phone_user_id]);
	phone_result = db_query(phone_database, query);
	db_free_result(phone_result);
	return SetPlayerName(playerid, name);
}

#if defined _ALS_SetPlayerName
    #undef SetPlayerName
#else
    #define _ALS_SetPlayerName
#endif

#define SetPlayerName SetPlayerNameEx

stock SendPhoneAlert(playerid, message[])
{
	KillTimer(PhoneData[playerid][phone_alert_timer]);

	PlayerTextDrawShow(playerid, PhoneData[playerid][phone_alerts]);
	PlayerTextDrawSetString(playerid, PhoneData[playerid][phone_alerts], message);

	PhoneData[playerid][phone_alert_timer] = SetTimerEx("OnAlertDisappear", 6000, false, "d", playerid);
	return 1;
}

stock CreateAlertTextdraw(playerid)
{
	PhoneData[playerid][phone_alerts] = CreatePlayerTextDraw(playerid, 320.000000, 420.000000, " ");
	PlayerTextDrawFont(playerid, PhoneData[playerid][phone_alerts], 1);
	PlayerTextDrawLetterSize(playerid, PhoneData[playerid][phone_alerts], 0.291666, 1.250000);
	PlayerTextDrawTextSize(playerid, PhoneData[playerid][phone_alerts], 438.000000, 142.000000);
	PlayerTextDrawSetOutline(playerid, PhoneData[playerid][phone_alerts], 0);
	PlayerTextDrawSetShadow(playerid, PhoneData[playerid][phone_alerts], 1);
	PlayerTextDrawAlignment(playerid, PhoneData[playerid][phone_alerts], 2);
	PlayerTextDrawColor(playerid, PhoneData[playerid][phone_alerts], -1);
	PlayerTextDrawBackgroundColor(playerid, PhoneData[playerid][phone_alerts], 255);
	PlayerTextDrawBoxColor(playerid, PhoneData[playerid][phone_alerts], 0x00000066);
	PlayerTextDrawUseBox(playerid, PhoneData[playerid][phone_alerts], 1);
	PlayerTextDrawSetProportional(playerid, PhoneData[playerid][phone_alerts], 1);
	PlayerTextDrawSetSelectable(playerid, PhoneData[playerid][phone_alerts], 0);
	return 1;
}

stock CreateCellPhoneTextDraw()
{
	phone_textdraw = TextDrawCreate(25.000000, 135.000000, "Preview_Model");
	TextDrawFont(phone_textdraw, 5);
	TextDrawLetterSize(phone_textdraw, 0.600000, 2.000000);
	TextDrawTextSize(phone_textdraw, 117.000000, 155.000000);
	TextDrawSetOutline(phone_textdraw, 0);
	TextDrawSetShadow(phone_textdraw, 0);
	TextDrawAlignment(phone_textdraw, 1);
	TextDrawColor(phone_textdraw, -1);
	TextDrawBackgroundColor(phone_textdraw, 0);
	TextDrawBoxColor(phone_textdraw, 255);
	TextDrawUseBox(phone_textdraw, 0);
	TextDrawSetProportional(phone_textdraw, 1);
	TextDrawSetSelectable(phone_textdraw, 0);
	TextDrawSetPreviewModel(phone_textdraw, 2967);
	TextDrawSetPreviewRot(phone_textdraw, 90.000000, 180.000000, 0.000000, 0.899999);
	TextDrawSetPreviewVehCol(phone_textdraw, 1, 1);
	return 1;
}

stock GetPlayerPhone(playerid) return PhoneData[playerid][phone_owned];

stock GivePlayerPhone(playerid)
{
	if(GetPlayerPhone(playerid)) return SendPhoneAlert(playerid, "You already own a cell phone");

	PhoneData[playerid][phone_owned] = 1;

	new query[128];
	format(query, sizeof(query), "UPDATE `PHONES` SET `OWNED` = '1' WHERE `USER_ID` = '%i'", PhoneData[playerid][phone_user_id]);
	phone_result = db_query(phone_database, query);
	db_free_result(phone_result);
	return SendPhoneAlert(playerid, "You now own a cell phone");
}

stock RemovePlayerPhone(playerid)
{
	if(!GetPlayerPhone(playerid)) return SendPhoneAlert(playerid, "You don't own a cell phone");

	PhoneData[playerid][phone_owned] = 0;

	new query[128];
	format(query, sizeof(query), "UPDATE `PHONES` SET `OWNED` = '0' WHERE `USER_ID` = '%i'", PhoneData[playerid][phone_user_id]);
	phone_result = db_query(phone_database, query);
	db_free_result(phone_result);

	format(query, sizeof(query), "DELETE FROM `CONTACTS` WHERE `OWNER_ID` = '%i'", PhoneData[playerid][phone_user_id]);
	phone_result = db_query(phone_database, query);
	db_free_result(phone_result);

	format(query, sizeof(query), "DELETE FROM `INBOX` WHERE `RECEIVER_ID` = '%i'", PhoneData[playerid][phone_user_id]);
	phone_result = db_query(phone_database, query);
	db_free_result(phone_result);

	format(query, sizeof(query), "DELETE FROM `OUTBOX` WHERE `SENDER_ID` = '%i'", PhoneData[playerid][phone_user_id]);
	phone_result = db_query(phone_database, query);
	db_free_result(phone_result);

	HidePlayerPhone(playerid);
	return SendPhoneAlert(playerid, "Your cell phone was removed");
}

stock ShowPlayerPhone(playerid)
{
	if(!GetPlayerPhone(playerid)) return SendPhoneAlert(playerid, "You don't own a cell phone");
	TextDrawShowForPlayer(playerid, phone_textdraw);
	return ShowHomepageDialog(playerid);
}

stock HidePlayerPhone(playerid)
{
	return TextDrawHideForPlayer(playerid, phone_textdraw);
}

stock GenerateNewPhoneNumber(playerid)
{
	new query[128], number[16], small = (random(899) + 100), big = (random(8999) + 1000);
	format(number, sizeof(number), "%i-%i", small, big);

	format(query, sizeof(query), "SELECT `NUMBER` FROM `PHONES` WHERE `NUMBER` = '%s' LIMIT 1", number);
	phone_result = db_query(phone_database, query);
	if(db_num_rows(phone_result))
	{
		db_free_result(phone_result);
		return GenerateNewPhoneNumber(playerid);
	}
	db_free_result(phone_result);
	return number;
}

stock ReturnPlayerUserID(playerid)
{
	new query[128], name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));
	format(query, sizeof(query), "SELECT `USER_ID` FROM `PHONES` WHERE `OWNER` = '%q' COLLATE NOCASE LIMIT 1", name);
	phone_result = db_query(phone_database, query);
	if(db_num_rows(phone_result))
	{
		new id = db_get_field_assoc_int(phone_result, "USER_ID");
		db_free_result(phone_result);
		return id;
	}
	db_free_result(phone_result);
	return 0;
}

stock ReturnContactNumber(userid)
{
	new query[128], number[16];
	format(query, sizeof(query), "SELECT `NUMBER` FROM `PHONES` WHERE `USER_ID` = '%i' LIMIT 1", userid);
	phone_result = db_query(phone_database, query);
	if(db_num_rows(phone_result))
	{
		db_get_field_assoc(phone_result, "NUMBER", number, sizeof(number));
	}
	db_free_result(phone_result);
	return number;
}

stock ReturnContactName(userid)
{
	new query[128], name[MAX_PLAYER_NAME];
	format(query, sizeof(query), "SELECT `OWNER` FROM `PHONES` WHERE `USER_ID` = '%i' LIMIT 1", userid);
	phone_result = db_query(phone_database, query);
	if(db_num_rows(phone_result))
	{
		db_get_field_assoc(phone_result, "OWNER", name, sizeof(name));
	}
	db_free_result(phone_result);
	return name;
}

stock ReturnContactCount(playerid)
{
	new query[128], count = 0;
	format(query, sizeof(query), "SELECT `CONTACT_ID` FROM `CONTACTS` WHERE `OWNER_ID` = '%i'", PhoneData[playerid][phone_user_id]);
	phone_result = db_query(phone_database, query);
	count = db_num_rows(phone_result);
	db_free_result(phone_result);
	return count;
}

stock ReturnMessageCount(playerid)
{
	new inbox_count = ReturnInboxCount(playerid), outbox_count = ReturnOutboxCount(playerid);
	return (inbox_count + outbox_count);
}

stock ReturnInboxCount(playerid)
{
	new query[128], count = 0;
	format(query, sizeof(query), "SELECT `INBOX_ID` FROM `INBOX` WHERE `RECEIVER_ID` = '%i'", PhoneData[playerid][phone_user_id]);
	phone_result = db_query(phone_database, query);
	count = db_num_rows(phone_result);
	db_free_result(phone_result);
	return count;
}

stock ReturnOutboxCount(playerid)
{
	new query[128], count = 0;
	format(query, sizeof(query), "SELECT `OUTBOX_ID` FROM `OUTBOX` WHERE `SENDER_ID` = '%i'", PhoneData[playerid][phone_user_id]);
	phone_result = db_query(phone_database, query);
	count = db_num_rows(phone_result);
	db_free_result(phone_result);
	return count;
}

stock GetMutualContact(userid, targetid)
{
	new query[128];
	format(query, sizeof(query), "SELECT `USER_ID` FROM `CONTACTS` WHERE `OWNER_ID` = '%i'", userid);
	phone_result = db_query(phone_database, query);
	if(db_num_rows(phone_result))
	{
		do
		{
			new id = db_get_field_assoc_int(phone_result, "USER_ID");
			db_free_result(phone_result);
			if(id == targetid) return 1;
		}
		while(db_next_row(phone_result));
	}
	db_free_result(phone_result);
	return 0;
}

stock IsContactInList(userid, targetid)
{
	new query[128];
	format(query, sizeof(query), "SELECT `OWNER_ID` FROM `CONTACTS` WHERE `USER_ID` = '%i' LIMIT 1", userid);
	phone_result = db_query(phone_database, query);
	if(db_num_rows(phone_result))
	{
		new id = db_get_field_assoc_int(phone_result, "OWNER_ID");
		db_free_result(phone_result);
		if(id == targetid) return 1;
	}
	db_free_result(phone_result);
	return 0;
}

stock CheckOwnedStatus(userid)
{
	new query[128];
	format(query, sizeof(query), "SELECT `OWNED` FROM `PHONES` WHERE `USER_ID` = '%i' LIMIT 1", userid);
	phone_result = db_query(phone_database, query);
	if(db_num_rows(phone_result))
	{
		new owned = db_get_field_assoc_int(phone_result, "OWNED");
		db_free_result(phone_result);
		
		if(owned) return 1;
        else return 0;
	}
	db_free_result(phone_result);
	return 0;
}

stock ShowListDialog(playerid)
{
	new query[128], string1[128], name[MAX_PLAYER_NAME], number[16], total = 0;
	format(query, sizeof(query), "SELECT `CONTACT_ID`, `OWNER`, `NUMBER` FROM `CONTACTS` LEFT JOIN `PHONES` USING (USER_ID) WHERE `OWNER_ID` = '%i'", PhoneData[playerid][phone_user_id]);
	phone_result = db_query(phone_database, query);
	if((total = db_num_rows(phone_result)))
	{
		large_string1[0] = EOS;
		do
		{
			db_get_field_assoc(phone_result, "OWNER", name, sizeof(name));
			db_get_field_assoc(phone_result, "NUMBER", number, sizeof(number));

			new contactid = db_get_field_assoc_int(phone_result, "CONTACT_ID");

			format(query, sizeof(query), "{FFFFFF}%i\t{FFFFFF}%s\t{FFFFFF}%s\n", contactid, name, number);
			strcat(large_string1, query);
		}
		while(db_next_row(phone_result));
	}
	db_free_result(phone_result);

	if(total == 0)
	{
		SendPhoneAlert(playerid, "You don't have any contacts");
		return HidePlayerPhone(playerid);
	}

	format(string1, sizeof(string1), "{FFFFFF}Cell Phone > My Contacts [Number: %s]", PhoneData[playerid][phone_number]);
	format(large_string2, sizeof(large_string2), "ID\tName\tNumber\n%s", large_string1);
	return ShowPlayerDialog(playerid, PHONE_LIST_CONTACTS_DIALOG, DIALOG_STYLE_TABLIST_HEADERS, string1, large_string2, "Message", "Back");
}

stock ShowHomepageDialog(playerid)
{
	new string1[128], string2[128], contacts = ReturnContactCount(playerid), messages = ReturnMessageCount(playerid);
	format(string1, sizeof(string1), "{FFFFFF}Cell Phone > Homepage [Number: %s]", PhoneData[playerid][phone_number]);
	format(string2, sizeof(string2), "{FFFFFF}Contacts (%i)\nMessages (%i)", contacts, messages);
	return ShowPlayerDialog(playerid, PHONE_SHOW_HOMEPAGE_DIALOG, DIALOG_STYLE_LIST, string1, string2, "Select", "Close");
}

stock ShowContactsDialog(playerid)
{
	new string1[128];
	format(string1, sizeof(string1), "{FFFFFF}Cell Phone > Contacts [Number: %s]", PhoneData[playerid][phone_number]);
	return ShowPlayerDialog(playerid, PHONE_SHOW_CONTACTS_DIALOG, DIALOG_STYLE_LIST, string1, "{FFFFFF}My Contacts\nAdd Contact\nDelete Contact", "Select", "Back");
}

stock ShowMessagesDialog(playerid)
{
	new string1[128];
	format(string1, sizeof(string1), "{FFFFFF}Cell Phone > Messages [Number: %s]", PhoneData[playerid][phone_number]);
	return ShowPlayerDialog(playerid, PHONE_SHOW_MESSAGES_DIALOG, DIALOG_STYLE_LIST, string1, "{FFFFFF}My Messages\nSend Message", "Select", "Back");
}

stock ShowMessageListDialog(playerid)
{
	new string1[128], string2[64], inbox_count = ReturnInboxCount(playerid), outbox_count = ReturnOutboxCount(playerid);
	format(string1, sizeof(string1), "{FFFFFF}Cell Phone > My Messages [Number: %s]", PhoneData[playerid][phone_number]);
	format(string2, sizeof(string2), "{FFFFFF}Inbox (%i)\nOutbox (%i)", inbox_count, outbox_count);
	return ShowPlayerDialog(playerid, PHONE_LIST_MESSAGES_DIALOG, DIALOG_STYLE_LIST, string1, string2, "Select", "Back");
}

stock ShowAddDialog(playerid)
{
	new string[128];
	format(string, sizeof(string), "{FFFFFF}Cell Phone > Add Contact [Number: %s]", PhoneData[playerid][phone_number]);
	return ShowPlayerDialog(playerid, PHONE_ADD_CONTACTS_DIALOG, DIALOG_STYLE_INPUT, string, "{FFFFFF}Please enter the cell phone number [ex. 123-4567] of the player below:", "Add", "Back");
}

stock OpenCellPhoneDatabase()
{
	phone_database = db_open(PHONE_DB_LOCATION);
    db_query(phone_database, "CREATE TABLE IF NOT EXISTS `PHONES` (`USER_ID` INTEGER PRIMARY KEY AUTOINCREMENT, `OWNER`, `NUMBER`, `OWNED`)");
    db_query(phone_database, "CREATE TABLE IF NOT EXISTS `CONTACTS` (`CONTACT_ID` INTEGER PRIMARY KEY AUTOINCREMENT, `USER_ID`, `OWNER_ID`)");

    db_query(phone_database, "CREATE TABLE IF NOT EXISTS `INBOX` (`INBOX_ID` INTEGER PRIMARY KEY AUTOINCREMENT, `USER_ID`, `RECEIVER_ID`, `READ`, `SUBJECT`, `MESSAGE`)");
    db_query(phone_database, "CREATE TABLE IF NOT EXISTS `OUTBOX` (`OUTBOX_ID` INTEGER PRIMARY KEY AUTOINCREMENT, `USER_ID`, `SENDER_ID`, `READ`, `SUBJECT`, `MESSAGE`)");
    return 1;
}

stock CloseCellPhoneDatabase()
{
	db_close(phone_database);
	return 1;
}

#if defined FILTERSCRIPT
	public OnFilterScriptInit()
	{
	    OpenCellPhoneDatabase();
		CreateCellPhoneTextDraw();
		return CallLocalFunction("cellphones_OnFilterScriptInit", "");
	}

	#if defined _ALS_OnFilterScriptInit
		#undef OnFilterScriptInit
	#else
		#define _ALS_OnFilterScriptInit
	#endif

	#define OnFilterScriptInit cellphones_OnFilterScriptInit
	#if defined cellphones_OnFilterScriptInit
		forward cellphones_OnFilterScriptInit();
	#endif

	public OnFilterScriptExit()
	{
	    CloseCellPhoneDatabase();
		return CallLocalFunction("cellphones_OnFilterScriptExit", "");
	}

	#if defined _ALS_OnFilterScriptExit
		#undef OnFilterScriptExit
	#else
		#define _ALS_OnFilterScriptExit
	#endif

	#define OnFilterScriptExit cellphones_OnFilterScriptExit
	#if defined cellphones_OnFilterScriptExit
		forward cellphones_OnFilterScriptExit();
	#endif
#else
	public OnGameModeInit()
	{
	    OpenCellPhoneDatabase();
		CreateCellPhoneTextDraw();
	    return CallLocalFunction("cellphones_OnGameModeInit", "");
	}

	#if defined _ALS_OnGameModeInit
		#undef OnGameModeInit
	#else
		#define _ALS_OnGameModeInit
	#endif

	#define OnGameModeInit cellphones_OnGameModeInit
	#if defined cellphones_OnGameModeInit
		forward cellphones_OnGameModeInit();
	#endif

	public OnGameModeExit()
	{
	    CloseCellPhoneDatabase();
	    return CallLocalFunction("cellphones_OnGameModeExit", "");
	}

	#if defined _ALS_OnGameModeExit
		#undef OnGameModeExit
	#else
		#define _ALS_OnGameModeExit
	#endif

	#define OnGameModeExit cellphones_OnGameModeExit
	#if defined cellphones_OnGameModeExit
		forward cellphones_OnGameModeExit();
	#endif
#endif

public OnPlayerConnect(playerid)
{
	if(!IsPlayerNPC(playerid))
	{
		new query[128], name[MAX_PLAYER_NAME], number[16];
		GetPlayerName(playerid, name, sizeof(name));

		format(query, sizeof(query), "SELECT * FROM `PHONES` WHERE `OWNER` = '%q' COLLATE NOCASE LIMIT 1", name);
		phone_result = db_query(phone_database, query);
		if(db_num_rows(phone_result))
		{
			PhoneData[playerid][phone_user_id] = db_get_field_assoc_int(phone_result, "USER_ID");
			PhoneData[playerid][phone_owned] = db_get_field_assoc_int(phone_result, "OWNED");

			db_get_field_assoc(phone_result, "NUMBER", number, sizeof(number));
			PhoneData[playerid][phone_number] = number;
		}
		else
		{
			db_free_result(phone_result);

			PhoneData[playerid][phone_number] = GenerateNewPhoneNumber(playerid);

			PhoneData[playerid][phone_owned] = 0;

		    format(query, sizeof(query), "INSERT INTO `PHONES` (`OWNER`, `NUMBER`, `OWNED`) VALUES ('%q', '%s', '0')", name, PhoneData[playerid][phone_number]);
			phone_result = db_query(phone_database, query);
			db_free_result(phone_result);

			PhoneData[playerid][phone_user_id] = ReturnPlayerUserID(playerid);
			if(PhoneData[playerid][phone_user_id] == 0)
			{
				print("DEBUG: Function 'ReturnPlayerUserID' failed under callback 'OnPlayerConnect' in include: cellphones.inc (Shutting down server...)");
				return SendRconCommand("exit");
			}
		}
		db_free_result(phone_result);

		CreateAlertTextdraw(playerid);
	}
	return CallLocalFunction("cellphones_OnPlayerConnect", "d", playerid);
}

#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect cellphones_OnPlayerConnect
#if defined cellphones_OnPlayerConnect
	forward cellphones_OnPlayerConnect(playerid);
#endif

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	switch(dialogid)
	{
		case PHONE_LIST_MESSAGES_DIALOG:
		{
			if(response)
			{
				switch(listitem)
				{
					case 0://Inbox
					{
						new query[200], string[128], subject[32], name[MAX_PLAYER_NAME], total = 0;
						format(query, sizeof(query), "SELECT `INBOX_ID`, `SUBJECT`, `OWNER`, `READ` FROM `INBOX` LEFT JOIN `PHONES` USING (USER_ID) WHERE `RECEIVER_ID` = '%i'", PhoneData[playerid][phone_user_id]);
					    phone_result = db_query(phone_database, query);
					    if((total = db_num_rows(phone_result)))
					    {
					        large_string1[0] = EOS;
					        do
					        {
					        	db_get_field_assoc(phone_result, "SUBJECT", subject, sizeof(subject));
					        	db_get_field_assoc(phone_result, "OWNER", name, sizeof(name));

						        new messageid = db_get_field_assoc_int(phone_result, "INBOX_ID");
						       	new read = db_get_field_assoc_int(phone_result, "READ");

						       	if(read)
								{
						         	format(query, sizeof(query), "{808080}%i\t{808080}%s\t{808080}%s\n", messageid, name, subject);
						      	}
						     	else 
							  	{
						         	format(query, sizeof(query), "{FFFFFF}%i\t{FFFFFF}%s\t{FFFFFF}%s\n", messageid, name, subject);
						      	}
								strcat(large_string1, query);
							}
					        while(db_next_row(phone_result));
					    }
					    db_free_result(phone_result);

					    if(total == 0)
					    {
					    	SendPhoneAlert(playerid, "You don't have any messages");
					    	return HidePlayerPhone(playerid);
					    }

						format(string, sizeof(string), "{FFFFFF}Cell Phone > Inbox [Number: %s]", PhoneData[playerid][phone_number]);
						format(large_string2, sizeof(large_string2), "ID\tFrom\tSubject\n%s", large_string1);
					    return ShowPlayerDialog(playerid, PHONE_INBOX_MESSAGES_DIALOG, DIALOG_STYLE_TABLIST_HEADERS, string, large_string2, "Read", "Back");
					}
					case 1://Outbox
					{
					    new query[200], string[128], subject[32], name[MAX_PLAYER_NAME], total = 0;
						format(query, sizeof(query), "SELECT `OUTBOX_ID`, `SUBJECT`, `OWNER`, `READ` FROM `OUTBOX` LEFT JOIN `PHONES` USING (USER_ID) WHERE `SENDER_ID` = '%i'", PhoneData[playerid][phone_user_id]);
					    phone_result = db_query(phone_database, query);
					    if((total = db_num_rows(phone_result)))
					    {
					        large_string1[0] = EOS;
					        do
					        {
					        	db_get_field_assoc(phone_result, "SUBJECT", subject, sizeof(subject));
					        	db_get_field_assoc(phone_result, "OWNER", name, sizeof(name));

						        new messageid = db_get_field_assoc_int(phone_result, "OUTBOX_ID");
						       	new read = db_get_field_assoc_int(phone_result, "READ");

						       	if(read)
								{
						         	format(query, sizeof(query), "{808080}%i\t{808080}%s\t{808080}%s\n", messageid, name, subject);
						      	}
						     	else 
							  	{
						         	format(query, sizeof(query), "{FFFFFF}%i\t{FFFFFF}%s\t{FFFFFF}%s\n", messageid, name, subject);
						      	}
								strcat(large_string1, query);
					        }
					        while(db_next_row(phone_result));
					    }
					    db_free_result(phone_result);

					    if(total == 0)
					    {
					    	SendPhoneAlert(playerid, "You don't have any messages");
					    	return HidePlayerPhone(playerid);
					    }

						format(string, sizeof(string), "{FFFFFF}Cell Phone > Outbox [Number: %s]", PhoneData[playerid][phone_number]);
						format(large_string2, sizeof(large_string2), "ID\tTo\tSubject\n%s", large_string1);
					    return ShowPlayerDialog(playerid, PHONE_OUTBOX_MESSAGES_DIALOG, DIALOG_STYLE_TABLIST_HEADERS, string, large_string2, "Read", "Back");
					}
				}
				return 1;
			}
			else
			{
				ShowMessagesDialog(playerid);
			}
			return 1;
		}
		case PHONE_INBOX_MESSAGES_DIALOG:
		{
			if(response)
			{
				new query[128], string1[128], string2[256], id = strval(inputtext), subject[32], message[128];
				format(query, sizeof(query), "SELECT * FROM `INBOX` WHERE `INBOX_ID` = '%i' LIMIT 1", id);
				phone_result = db_query(phone_database, query);
				if(db_num_rows(phone_result))
				{
					db_get_field_assoc(phone_result, "SUBJECT", subject, sizeof(subject));
					db_get_field_assoc(phone_result, "MESSAGE", message, sizeof(message));

					new senderid = db_get_field_assoc_int(phone_result, "USER_ID");
					new read = db_get_field_assoc_int(phone_result, "READ");
						
					db_free_result(phone_result);

					if(!read)
					{
						format(query, sizeof(query), "UPDATE `INBOX` SET `READ` = '1' WHERE `INBOX_ID` = '%i'", id);
						phone_result = db_query(phone_database, query);
						db_free_result(phone_result);
					}

					PhoneData[playerid][phone_message_id] = id;

					format(string1, sizeof(string1), "{FFFFFF}Cell Phone > Inbox [Number: %s]", PhoneData[playerid][phone_number]);
					format(string2, sizeof(string2), "{FFFFFF}From: %s <%s>\nSubject: '%s'\nMessage: %s", ReturnContactName(senderid), ReturnContactNumber(senderid), subject, message);
					return ShowPlayerDialog(playerid, PHONE_INBOX_READ_DIALOG, DIALOG_STYLE_MSGBOX, string1, string2, "Delete", "Back");
				}
				db_free_result(phone_result);
				return 1;
			}
			else
			{
				ShowMessageListDialog(playerid);
			}
			return 1;
		}
		case PHONE_OUTBOX_MESSAGES_DIALOG:
		{
			if(response)
			{
				new query[128], string1[128], string2[256], id = strval(inputtext), subject[32], message[128];
				format(query, sizeof(query), "SELECT * FROM `OUTBOX` WHERE `OUTBOX_ID` = '%i' LIMIT 1", id);
				phone_result = db_query(phone_database, query);
				if(db_num_rows(phone_result))
				{
					db_get_field_assoc(phone_result, "SUBJECT", subject, sizeof(subject));
					db_get_field_assoc(phone_result, "MESSAGE", message, sizeof(message));

					new receiverid = db_get_field_assoc_int(phone_result, "USER_ID");
					new read = db_get_field_assoc_int(phone_result, "READ");
						
					db_free_result(phone_result);

					if(!read)
					{
						format(query, sizeof(query), "UPDATE `OUTBOX` SET `READ` = '1' WHERE `OUTBOX_ID` = '%i'", id);
						phone_result = db_query(phone_database, query);
						db_free_result(phone_result);
					}

					PhoneData[playerid][phone_message_id] = id;

					format(string1, sizeof(string1), "{FFFFFF}Cell Phone > Outbox [Number: %s]", PhoneData[playerid][phone_number]);
					format(string2, sizeof(string2), "{FFFFFF}To: %s <%s>\nSubject: '%s'\nMessage: %s", ReturnContactName(receiverid), ReturnContactNumber(receiverid), subject, message);
					return ShowPlayerDialog(playerid, PHONE_OUTBOX_READ_DIALOG, DIALOG_STYLE_MSGBOX, string1, string2, "Delete", "Back");
				}
				db_free_result(phone_result);
				return 1;
			}
			else
			{
				ShowMessageListDialog(playerid);
			}
			return 1;
		}
		case PHONE_INBOX_READ_DIALOG:
		{
			if(response)
			{
				new string[128];
				format(string, sizeof(string), "{FFFFFF}Cell Phone > Delete Message [Number: %s]", PhoneData[playerid][phone_number]);
				return ShowPlayerDialog(playerid, PHONE_INBOX_DELETE_DIALOG, DIALOG_STYLE_MSGBOX, string, "{FFFFFF}Are you sure you want to delete this message from your inbox?", "Yes", "No");
			}
			else
			{
				ShowMessageListDialog(playerid);
			}
			return 1;
		}
		case PHONE_OUTBOX_READ_DIALOG:
		{
			if(response)
			{
				new string[128];
				format(string, sizeof(string), "{FFFFFF}Cell Phone > Delete Message [Number: %s]", PhoneData[playerid][phone_number]);
				return ShowPlayerDialog(playerid, PHONE_OUTBOX_DELETE_DIALOG, DIALOG_STYLE_MSGBOX, string, "{FFFFFF}Are you sure you want to delete this message from your outbox?", "Yes", "No");
			}
			else
			{
				ShowMessageListDialog(playerid);
			}
			return 1;
		}
		case PHONE_INBOX_DELETE_DIALOG:
		{
			if(response)
			{
				new query[128], id = PhoneData[playerid][phone_message_id];
				format(query, sizeof(query), "SELECT `INBOX_ID` FROM `INBOX` WHERE `INBOX_ID` = '%i' LIMIT 1", id);
				phone_result = db_query(phone_database, query);
				if(db_num_rows(phone_result))
				{
					db_free_result(phone_result);

					format(query, sizeof(query), "DELETE FROM `INBOX` WHERE `INBOX_ID` = '%i'", id);
					phone_result = db_query(phone_database, query);
					db_free_result(phone_result);

					SendPhoneAlert(playerid, "You deleted ~r~(1) ~w~inbox message");
					return ShowMessageListDialog(playerid);
				}
				db_free_result(phone_result);
				return 1;
			}
			else
			{
				ShowMessagesDialog(playerid);
			}
			return 1;
		}
		case PHONE_OUTBOX_DELETE_DIALOG:
		{
			if(response)
			{
				new query[128], id = PhoneData[playerid][phone_message_id];
				format(query, sizeof(query), "SELECT `OUTBOX_ID` FROM `OUTBOX` WHERE `OUTBOX_ID` = '%i' LIMIT 1", id);
				phone_result = db_query(phone_database, query);
				if(db_num_rows(phone_result))
				{
					db_free_result(phone_result);

					format(query, sizeof(query), "DELETE FROM `OUTBOX` WHERE `OUTBOX_ID` = '%i'", id);
					phone_result = db_query(phone_database, query);
					db_free_result(phone_result);

					SendPhoneAlert(playerid, "You deleted ~r~(1) ~w~outbox message");
					return ShowMessageListDialog(playerid);
				}
				db_free_result(phone_result);
				return 1;
			}
			else
			{
				ShowMessagesDialog(playerid);
			}
			return 1;
		}
		case PHONE_SHOW_MESSAGES_DIALOG:
		{
			if(response)
			{
				switch(listitem)
				{
					case 0://My Messages
					{
						return ShowMessageListDialog(playerid);
					}
					case 1://Send Message
					{
						return ShowListDialog(playerid);
					}
				}
				return 1;
			}
			else
			{
				ShowHomepageDialog(playerid);
			}
			return 1;
		}
		case PHONE_SHOW_CONTACTS_DIALOG:
		{
			if(response)
			{
				switch(listitem)
				{
					case 0://My Contacts
					{
						return ShowListDialog(playerid);
					}
					case 1://Add Contact
					{
						return ShowAddDialog(playerid);
					}
					case 2://Delete Contact
					{
						new query[128], string1[128], name[MAX_PLAYER_NAME], number[16], total = 0;
						format(query, sizeof(query), "SELECT `CONTACT_ID`, `OWNER`, `NUMBER` FROM `CONTACTS` LEFT JOIN `PHONES` USING (USER_ID) WHERE `OWNER_ID` = '%i'", PhoneData[playerid][phone_user_id]);
					    phone_result = db_query(phone_database, query);
					    if((total = db_num_rows(phone_result)))
					    {
					        large_string1[0] = EOS;
					        do
					        {
					        	db_get_field_assoc(phone_result, "OWNER", name, sizeof(name));
					        	db_get_field_assoc(phone_result, "NUMBER", number, sizeof(number));

						        new contactid = db_get_field_assoc_int(phone_result, "CONTACT_ID");

					            format(query, sizeof(query), "{FFFFFF}%i\t{FFFFFF}%s\t{FFFFFF}%s\n", contactid, name, number);
								strcat(large_string1, query);
					        }
					        while(db_next_row(phone_result));
					    }
					    db_free_result(phone_result);

					    if(total == 0)
					    {
					    	SendPhoneAlert(playerid, "You don't have any contacts");
					    	return HidePlayerPhone(playerid);
					    }

						format(string1, sizeof(string1), "{FFFFFF}Cell Phone > Delete Contact [Number: %s]", PhoneData[playerid][phone_number]);
						format(large_string2, sizeof(large_string2), "ID\tName\tNumber\n%s", large_string1);
					    return ShowPlayerDialog(playerid, PHONE_DELETE_CONTACTS_DIALOG, DIALOG_STYLE_TABLIST_HEADERS, string1, large_string2, "Delete", "Back");
					}
				}
				return 1;
			}
			else
			{
				ShowHomepageDialog(playerid);
			}
			return 1;
		}
		case PHONE_DELETE_CONTACTS_DIALOG:
		{
			if(response)
			{
				new query[128], string1[128], string2[128], id = strval(inputtext);
				format(query, sizeof(query), "SELECT `USER_ID` FROM `CONTACTS` WHERE `CONTACT_ID` = '%i' LIMIT 1", id);
				phone_result = db_query(phone_database, query);
				if(db_num_rows(phone_result))
				{
					new target_id = db_get_field_assoc_int(phone_result, "USER_ID");
					db_free_result(phone_result);

					PhoneData[playerid][phone_contact_id] = id;

					format(string1, sizeof(string1), "{FFFFFF}Cell Phone > Delete Contact [Number: %s]", PhoneData[playerid][phone_number]);
					format(string2, sizeof(string2), "{FFFFFF}Are you sure you want to delete '%s' from your contacts?", ReturnContactName(target_id));
					return ShowPlayerDialog(playerid, PHONE_VERIFY_DELETE_DIALOG, DIALOG_STYLE_MSGBOX, string1, string2, "Yes", "No");
				}
				db_free_result(phone_result);
				return 1;
			}
			else
			{
				ShowContactsDialog(playerid);
			}
			return 1;
		}
		case PHONE_VERIFY_DELETE_DIALOG:
		{
			if(response)
			{
				new query[128], id = PhoneData[playerid][phone_contact_id];
				format(query, sizeof(query), "DELETE FROM `CONTACTS` WHERE `CONTACT_ID` = '%i'", id);
				phone_result = db_query(phone_database, query);
				db_free_result(phone_result);

				SendPhoneAlert(playerid, "You deleted ~r~(1) ~w~contact");
				return ShowContactsDialog(playerid);
			}
			else
			{
				ShowContactsDialog(playerid);
			}
			return 1;
		}
		case PHONE_LIST_CONTACTS_DIALOG:
		{
			if(response)
			{
				new query[128], string1[128], string2[128], id = strval(inputtext);
				format(query, sizeof(query), "SELECT `USER_ID` FROM `CONTACTS` WHERE `CONTACT_ID` = '%i' LIMIT 1", id);
				phone_result = db_query(phone_database, query);
				if(db_num_rows(phone_result))
				{
					PhoneData[playerid][phone_contact_id] = db_get_field_assoc_int(phone_result, "USER_ID");
				
					db_free_result(phone_result);

					if(!GetMutualContact(PhoneData[playerid][phone_contact_id], PhoneData[playerid][phone_user_id]))
					{
						SendPhoneAlert(playerid, "Player not in contacts");
						return HidePlayerPhone(playerid);
					}

					if(PhoneData[playerid][phone_contact_id] == PhoneData[playerid][phone_user_id])
					{
						SendPhoneAlert(playerid, "That contact is yourself");
						return HidePlayerPhone(playerid);
					}

					if(!CheckOwnedStatus(PhoneData[playerid][phone_contact_id]))
					{
						SendPhoneAlert(playerid, "Player doesn't own cell phone");
						return HidePlayerPhone(playerid);
					}

					format(string1, sizeof(string1), "{FFFFFF}Cell Phone > Send Message [Number: %s]", PhoneData[playerid][phone_number]);
					format(string2, sizeof(string2), "{FFFFFF}Please type a subject for the message to send to '%s' below:", ReturnContactName(PhoneData[playerid][phone_contact_id]));
					return ShowPlayerDialog(playerid, PHONE_SEND_MESSAGES_DIALOG, DIALOG_STYLE_INPUT, string1, string2, "Enter", "Back");
				}
				db_free_result(phone_result);
				return 1;
			}
			else
			{
				ShowHomepageDialog(playerid);
			}
			return 1;
		}
		case PHONE_SEND_MESSAGES_DIALOG:
		{
			if(response)
			{
				if(strlen(inputtext) < 1 || strlen(inputtext) > 32)
				{
					SendPhoneAlert(playerid, "Invalid length: 1-32");
					return HidePlayerPhone(playerid);
				}

				new string1[128], string2[128], subject[32];
				format(subject, sizeof(subject), "%s", inputtext);
				PhoneData[playerid][phone_subject] = subject;
				
				format(string1, sizeof(string1), "{FFFFFF}Cell Phone > Send Message [Number: %s]", PhoneData[playerid][phone_number]);
				format(string2, sizeof(string2), "{FFFFFF}Please type the message you want to send to '%s' below:", ReturnContactName(PhoneData[playerid][phone_contact_id]));
				return ShowPlayerDialog(playerid, PHONE_WRITE_MESSAGES_DIALOG, DIALOG_STYLE_INPUT, string1, string2, "Send", "Back");
			}
			else
			{
				ShowMessagesDialog(playerid);
			}
			return 1;
		}
		case PHONE_WRITE_MESSAGES_DIALOG:
		{
			if(response)
			{
				if(strlen(inputtext) < 1 || strlen(inputtext) > 128)
				{
					SendPhoneAlert(playerid, "Invalid length: 1-128");
					return HidePlayerPhone(playerid);
				}

				new query[400];
				format(query, sizeof(query), "INSERT INTO `INBOX` (`USER_ID`, `RECEIVER_ID`, `READ`, `SUBJECT`, `MESSAGE`) VALUES ('%i', '%i', '0', '%q', '%q')", PhoneData[playerid][phone_user_id], PhoneData[playerid][phone_contact_id], PhoneData[playerid][phone_subject], inputtext);
				phone_result = db_query(phone_database, query);
				db_free_result(phone_result);

				format(query, sizeof(query), "INSERT INTO `OUTBOX` (`USER_ID`, `SENDER_ID`, `READ`, `SUBJECT`, `MESSAGE`) VALUES ('%i', '%i', '0', '%q', '%q')", PhoneData[playerid][phone_contact_id], PhoneData[playerid][phone_user_id], PhoneData[playerid][phone_subject], inputtext);
				phone_result = db_query(phone_database, query);
				db_free_result(phone_result);

				for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
				{
				    if(IsPlayerConnected(i) && !IsPlayerNPC(i) && GetPlayerPhone(i))
					{
						if(PhoneData[playerid][phone_contact_id] == PhoneData[i][phone_user_id])
						{
							PlayerPlaySound(i, 3600, 0.0, 0.0, 0.0);

							SendPhoneAlert(i, "You have ~g~(1) ~w~new message");
							break;
						}
					}
				}

				SendPhoneAlert(playerid, "You sent ~g~(1) ~w~new message");
				return HidePlayerPhone(playerid);
			}
			else
			{
				ShowMessagesDialog(playerid);
			}
			return 1;
		}
		case PHONE_ADD_CONTACTS_DIALOG:
		{
			if(response)
			{
				new query[128], string1[128], string2[128], name[MAX_PLAYER_NAME], bool:found_user;
				format(query, sizeof(query), "SELECT `USER_ID`, `OWNER`, `OWNED` FROM `PHONES` WHERE `NUMBER` = '%s' LIMIT 1", inputtext);
				phone_result = db_query(phone_database, query);
				if(db_num_rows(phone_result))
				{
					db_get_field_assoc(phone_result, "OWNER", name, sizeof(name));

					PhoneData[playerid][phone_contact_id] = db_get_field_assoc_int(phone_result, "USER_ID");
					
					new owned = db_get_field_assoc_int(phone_result, "OWNED");

					db_free_result(phone_result);

					if(PhoneData[playerid][phone_contact_id] == PhoneData[playerid][phone_user_id])
					{
						SendPhoneAlert(playerid, "That contact is yourself");
						return HidePlayerPhone(playerid);
					}

					if(!owned)
					{
						SendPhoneAlert(playerid, "Player doesn't own cell phone");
						return HidePlayerPhone(playerid);
					}

					if(IsContactInList(PhoneData[playerid][phone_contact_id], PhoneData[playerid][phone_user_id]))
					{
						SendPhoneAlert(playerid, "Contact already added");
						return HidePlayerPhone(playerid);
					}

					found_user = true;
				}
				db_free_result(phone_result);

				if(!found_user)
				{
					SendPhoneAlert(playerid, "Number does not exist");
					return HidePlayerPhone(playerid);
				}

				format(string1, sizeof(string1), "{FFFFFF}Cell Phone > Add Contact [Number: %s]", PhoneData[playerid][phone_number]);
				format(string2, sizeof(string2), "{FFFFFF}Would you like to add '%s' to your contacts?", name);
				return ShowPlayerDialog(playerid, PHONE_VERIFY_ADD_DIALOG, DIALOG_STYLE_MSGBOX, string1, string2, "Yes", "No");
			}
			else
			{
				ShowContactsDialog(playerid);
			}
			return 1;
		}
		case PHONE_VERIFY_ADD_DIALOG://here
		{
			if(response)
			{
				new query[128];
				format(query, sizeof(query), "INSERT INTO `CONTACTS` (`USER_ID`, `OWNER_ID`) VALUES ('%i', '%i')", PhoneData[playerid][phone_contact_id], PhoneData[playerid][phone_user_id]);
				phone_result = db_query(phone_database, query);
				db_free_result(phone_result);

				SendPhoneAlert(playerid, "You added ~g~(1) ~w~new contact");
				return HidePlayerPhone(playerid);
			}
			else
			{
				ShowAddDialog(playerid);
			}
			return 1;
		}
		case PHONE_SHOW_HOMEPAGE_DIALOG:
		{
			if(response)
			{
				switch(listitem)
				{
					case 0://Contacts
					{
						return ShowContactsDialog(playerid);
					}
					case 1://Messages
					{
						return ShowMessagesDialog(playerid);
					}
				}
				return 1;
			}
			else
			{
				HidePlayerPhone(playerid);
			}
			return 1;
		}
	}
	return CallLocalFunction("cellphones_OnDialogResponse", "dddds", playerid, dialogid, response, listitem, inputtext);
}

#if defined _ALS_OnDialogResponse
	#undef OnDialogResponse
#else
	#define _ALS_OnDialogResponse
#endif

#define OnDialogResponse cellphones_OnDialogResponse
#if defined cellphones_OnDialogResponse
	forward cellphones_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
#endif

public OnAlertDisappear(playerid)
{
	PlayerTextDrawSetString(playerid, PhoneData[playerid][phone_alerts], " ");
	PlayerTextDrawHide(playerid, PhoneData[playerid][phone_alerts]);
	return 1;
}
