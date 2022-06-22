# [Include] SQLite Cell Phone System

<p><em>Credits: <a href="https://github.com/WeponzTV" target="_blank">Weponz</a> & <a href="https://github.com/realdiegopoptart" target="_blank">realdiegopoptart</a></em></p>

## Video Preview

<p><a href="https://youtu.be/VlGSOE9n5To" target="_blank"><img src="https://i.imgur.com/lzpZMZk.jpg" width="420px" height="236px" /></a></p>

<p>Link: https://youtu.be/VlGSOE9n5To</p>

## Support

<a href="https://discord.gg/fugKZrqBth" target="_blank"><img src="https://img.shields.io/discord/986844024126210078?label=SA-MP%20Community"/></a>

## How To Install

Download & move 'cellphones.inc' into your includes folder.

```c
GetPlayerPhone(playerid); // Check if the player has a cell phone.
GivePlayerPhone(playerid); // Give the player a cell phone.
RemovePlayerPhone(playerid); // Remove the player's cell phone.
ShowPlayerPhone(playerid); // Show the player their cell phone.
```

## Where To Start
```c
#include <a_samp>
#include <cellphones>
```

## Example Code
```c
public OnPlayerCommandText(playerid, cmdtext[])
{
	if(!strcmp("/givephone", cmdtext, true))
	{
		if(GetPlayerPhone(playerid)) return SendClientMessage(playerid, -1, "SERVER: You already have a phone.");
		GivePlayerPhone(playerid);
		return 1;
	}
	else if(!strcmp("/removephone", cmdtext, true))
	{
		if(!GetPlayerPhone(playerid)) return SendClientMessage(playerid, -1, "SERVER: You don't have a phone.");
		RemovePlayerPhone(playerid);
		return 1;
	}
	else if(!strcmp("/showphone", cmdtext, true))
	{
		if(!GetPlayerPhone(playerid)) return SendClientMessage(playerid, -1, "SERVER: You don't have a phone.");
		ShowPlayerPhone(playerid);
		return 1;
	}
	return 0;
}
```
