using Metamorphosis.Core.Data;
using Metamorphosis.Core.Models;

namespace Metamorphosis.Core.GameEngine;

public class RoomEvents
{
    private readonly IGameDataStore _store;
    private int _commandsSinceFishOnLand;
    private bool _fishOnLandActive;
    private int _commandsSinceUnderwater;
    private bool _underwaterBreathActive;
    private bool _underwaterWarned;
    private int _commandsSinceJail;
    private bool _jailActive;
    private int _jailPhase; // 0=waiting, 1=noises, 2=voice, 3=guard
    private int _commandsSinceBomb;
    private bool _bombActive;

    public RoomEvents(IGameDataStore store)
    {
        _store = store;
    }

    public void Reset()
    {
        _fishOnLandActive = false;
        _underwaterBreathActive = false;
        _underwaterWarned = false;
        _jailActive = false;
        _jailPhase = 0;
        _bombActive = false;
    }

    /// <summary>
    /// Called after each command to advance any active timers.
    /// Returns output lines to append, or null if nothing happened.
    /// </summary>
    public void Tick(GameOutput output)
    {
        TickFishOnLand(output);
        TickUnderwaterBreath(output);
        TickJail(output);
        TickBomb(output);
    }

    /// <summary>
    /// Called when player enters a room. Sets up room-specific triggers.
    /// </summary>
    public void OnRoomEntry(string roomId, GameOutput output)
    {
        var player = _store.Player;

        // Death rooms are handled by GameEngine.ShowCurrentRoom already

        // Fish on land check — if player is fish and not in an underwater room
        if (player.CurrentForm == AnimalForm.Fish)
        {
            var room = _store.GetRoomById(roomId);
            if (room != null && !room.IsUnderwater)
            {
                if (!_fishOnLandActive)
                {
                    _fishOnLandActive = true;
                    _commandsSinceFishOnLand = 0;
                }
            }
            else
            {
                // Reached water — safe
                _fishOnLandActive = false;
            }
        }
        else
        {
            _fishOnLandActive = false;
        }

        // Underwater breath check — snake or beetle underwater
        if ((player.CurrentForm == AnimalForm.Snake || player.CurrentForm == AnimalForm.Beetle))
        {
            var room = _store.GetRoomById(roomId);
            if (room != null && room.IsUnderwater)
            {
                if (!_underwaterBreathActive)
                {
                    _underwaterBreathActive = true;
                    _underwaterWarned = false;
                    _commandsSinceUnderwater = 0;
                }
            }
            else
            {
                // Surfaced — safe
                _underwaterBreathActive = false;
                _underwaterWarned = false;
            }
        }
        else
        {
            _underwaterBreathActive = false;
        }
    }

    public void StartJail()
    {
        _jailActive = true;
        _jailPhase = 0;
        _commandsSinceJail = 0;
    }

    public void StartBomb()
    {
        _bombActive = true;
        _commandsSinceBomb = 0;
    }

    public bool IsJailActive => _jailActive;
    public bool IsBombActive => _bombActive;

    private void TickFishOnLand(GameOutput output)
    {
        if (!_fishOnLandActive) return;
        _commandsSinceFishOnLand++;

        if (_commandsSinceFishOnLand >= 3)
        {
            // Fish suffocates on land after a few actions
            output.AddLine("\n&+R[Forgot how to breathe]#");
            output.AddLine("&+RYou suffocate!#");
            _fishOnLandActive = false;
            output.IsDeathMenu = true;
            // Signal death to caller
            _store.Player.IsDead = true;
            _store.UpdatePlayer(_store.Player);
        }
        else if (_commandsSinceFishOnLand == 2)
        {
            output.AddLine("\n&+YYou are gasping for air! You need to find water quickly!#");
        }
    }

    private void TickUnderwaterBreath(GameOutput output)
    {
        if (!_underwaterBreathActive) return;
        _commandsSinceUnderwater++;

        var player = _store.Player;
        // Beetle gets warned sooner (fewer commands)
        int warnAt = player.CurrentForm == AnimalForm.Beetle ? 8 : 10;
        int dieAt = player.CurrentForm == AnimalForm.Beetle ? 12 : 14;

        if (_commandsSinceUnderwater >= dieAt)
        {
            output.AddLine("\nYou run out of &+Bair# and die in hideous underwater convulsions.");
            _underwaterBreathActive = false;
            output.IsDeathMenu = true;
            player.IsDead = true;
            _store.UpdatePlayer(player);
        }
        else if (_commandsSinceUnderwater >= warnAt && !_underwaterWarned)
        {
            output.AddLine("\nYou are beginning to struggle for &+Bair#, you will need to surface soon.");
            _underwaterWarned = true;
        }
    }

    private void TickJail(GameOutput output)
    {
        if (!_jailActive) return;
        _commandsSinceJail++;

        if (_commandsSinceJail >= 2 && _jailPhase == 0)
        {
            output.AddLine("\nYou hear &+Rnoises# outside the door.");
            _jailPhase = 1;
        }
        else if (_commandsSinceJail >= 4 && _jailPhase == 1)
        {
            output.AddLine("\nYou hear a &+Rvoice# on the other side of the door.");
            output.AddLine("'&+MYou, fetch the prisoner, and ready the council.#'");
            _jailPhase = 2;
        }
        else if (_commandsSinceJail >= 6 && _jailPhase == 2)
        {
            _jailActive = false;
            _jailPhase = 3;
            ResolveJailGuard(output);
        }
    }

    private void ResolveJailGuard(GameOutput output)
    {
        var player = _store.Player;

        switch (player.CurrentForm)
        {
            case AnimalForm.Bird:
                output.AddLine("\n'&+MCome with me,#' says the &+Cbird#, '&+Myou'll only make it worse for yourself.#'");
                output.AddLine("You are dragged off to the Avian Council chambers.\n");
                // Move to council chambers (room53)
                LimboPlayerItems();
                player.CurrentRoomId = "room53";
                _store.UpdatePlayer(player);
                break;

            case AnimalForm.Fish:
                output.AddLine("\nThe &+Cbird# looks around the room, obviously expecting another bird");
                output.AddLine("to be awaiting him, and he is clearly confused by the presence of a fish.");
                output.AddLine("He looks at you with beady eyes, then quickly resolves himself and pecks");
                output.AddLine("you to death quickly and painfully.");
                output.AddLine("&+RPecked to death by a bird!#");
                output.IsDeathMenu = true;
                player.IsDead = true;
                _store.UpdatePlayer(player);
                break;

            case AnimalForm.Snake:
                output.AddLine("\nThe &+Cbird# looks around the room, obviously expecting another bird");
                output.AddLine("to be awaiting him, and he is clearly confused by the presence of a snake.");
                output.AddLine("His confusion quickly gives way to terror and he flees in panic, screaming");
                output.AddLine("hysterically.");
                output.AddLine("\nThe cell doors swing open!");
                UnlockCellDoors();
                break;

            case AnimalForm.Beetle:
                output.AddLine("\nThe &+Cbird# looks around the room, obviously expecting another bird");
                output.AddLine("to be awaiting him, and he is clearly confused by the presence of a beetle.");
                output.AddLine("His confusion quickly gives way to hunger, and he quickly bends down,");
                output.AddLine("grabbing you in his beak and crushing you to death.");
                output.AddLine("&+REaten alive by a bird!#");
                output.IsDeathMenu = true;
                player.IsDead = true;
                _store.UpdatePlayer(player);
                break;

            case AnimalForm.Human:
            default:
                output.AddLine("\nThe &+Cbird# looks around the room, obviously expecting another bird");
                output.AddLine("to be awaiting him, and he is clearly confused by the presence of a human.");
                output.AddLine("His confusion quickly gives way to terror and he flees in panic, screaming");
                output.AddLine("in terror.");
                output.AddLine("\nThe cell doors swing open!");
                UnlockCellDoors();
                break;
        }
    }

    private void TickBomb(GameOutput output)
    {
        if (!_bombActive) return;
        _commandsSinceBomb++;

        if (_commandsSinceBomb >= 4)
        {
            _bombActive = false;
            output.AddLine("\nAn enormous &+Rexplosion# rocks the &+yground# around you.");

            // Check if player is near the bomb — simplified: if in blast radius, die
            // In the full game this checks specific room locations
            var player = _store.Player;
            var room = _store.GetRoomById(player.CurrentRoomId);
            // The bomb puzzle is about placing it in the right spot to destroy the dam
            // For now, broadcast the explosion message
            output.AddLine("The shockwave reverberates through the area.");
        }
    }

    private void LimboPlayerItems()
    {
        int limboUid = _store.GetRoomUid("room74");
        foreach (var obj in _store.GetPlayerInventory(PlayerState.PlayerUid).ToList())
        {
            obj.Carried = 0;
            obj.Wielded = 0;
            obj.Worn = 0;
            obj.Location = limboUid;
            _store.UpdateObject(obj);
        }
    }

    private void UnlockCellDoors()
    {
        // Unlock cell door objects by setting their state to open
        foreach (var obj in _store.GetPlayerInventory(PlayerState.PlayerUid))
        {
            // Cell doors are handled via state
        }
        // Find cell door objects and set state to 0 (open)
        // The actual door objects would be found by name in a full implementation
    }
}
