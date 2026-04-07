using Metamorphosis.Core.Data;
using Metamorphosis.Core.Models;
using Microsoft.Extensions.Logging;

namespace Metamorphosis.Core.GameEngine;

public class GameEngine
{
    private readonly IGameDataStore _store;
    private readonly MetaTxtParser _parser;
    private readonly ILogger<GameEngine>? _logger;
    private readonly RoomEvents _events;
    private readonly GameCommands _commands;
    private List<Room> _baseRooms = new();
    private List<Mobile> _baseMobiles = new();
    private List<GameObject> _baseObjects = new();

    public GameEngine(IGameDataStore store, MetaTxtParser parser, ILogger<GameEngine>? logger = null)
    {
        _store = store;
        _parser = parser;
        _logger = logger;
        _events = new RoomEvents(store);
        _commands = new GameCommands(store);
    }

    public GameOutput Initialize()
    {
        var result = _parser.ParseFromEmbeddedResource();
        _baseRooms = result.Rooms;
        _baseMobiles = result.Mobiles;
        _baseObjects = result.Objects;

        _store.Reset(_baseRooms, _baseMobiles, _baseObjects);
        _logger?.LogInformation("Game initialized: {Rooms} rooms, {Mobiles} mobiles, {Objects} objects",
            result.Rooms.Count, result.Mobiles.Count, result.Objects.Count);

        return ShowEntryMenu(true);
    }

    public GameOutput ProcessCommand(string rawInput)
    {
        _logger?.LogDebug("Command: {Input}", rawInput);

        var parsed = CommandParser.Parse(rawInput);
        if (parsed == null)
            return CreateOutput(o => o.AddLine("Pardon?"));

        var player = _store.Player;

        // Entry menu state
        if (player.IsInEntryMenu)
            return HandleEntryMenuCommand(parsed);

        // Death state
        if (player.IsDead)
            return HandleDeathCommand(parsed);

        // Normal gameplay
        var result = HandleGameCommand(parsed);

        // Advance any active event timers
        if (!_store.Player.IsDead && !_store.Player.IsInEntryMenu)
            _events.Tick(result);

        return result;
    }

    public PlayerState GetCurrentState() => _store.Player;

    private GameOutput HandleEntryMenuCommand(ParsedCommand cmd)
    {
        switch (cmd.Verb)
        {
            case "n":
                return StartNewGame();
            case "l":
                if (_store.Player.IsDead)
                {
                    var output = CreateOutput(o =>
                        o.AddLine("You appear to be dead in your previous game... let's restart, shall we?"));
                    output.ShouldClearOutput = true;
                    return MergeOutput(output, StartNewGame());
                }
                return LoadGame();
            case "h":
                return CreateOutput(o =>
                {
                    o.AddLine("\nThe Help command is available once you start the game - you'll love it, I promise:)");
                    o.AddLine("...The game that is, not the help command. Frankly, it's not that helpful.");
                });
            default:
                return ShowEntryMenu(false);
        }
    }

    private GameOutput HandleDeathCommand(ParsedCommand cmd)
    {
        if (cmd.Verb == "r")
        {
            var output = CreateOutput(o =>
            {
                o.ShouldClearOutput = true;
                o.AddLine("\nThe veil of death draws back, and you find yourself returning to the lands of the living...\n");
            });
            return MergeOutput(output, StartNewGame());
        }
        if (cmd.Verb == "q" || cmd.Verb == "quit")
        {
            return ShowEntryMenu(true);
        }
        return ShowDeathMenu(false);
    }

    private GameOutput HandleGameCommand(ParsedCommand cmd)
    {
        return cmd.Verb switch
        {
            // Navigation
            "n" or "north" => Move(Direction.North),
            "south" => Move(Direction.South),
            "east" => Move(Direction.East),
            "west" => Move(Direction.West),
            "up" => Move(Direction.Up),
            "down" => Move(Direction.Down),
            // View
            "l" or "look" => ShowCurrentRoom(),
            "ploc" => CreateOutput(o => o.AddLine(_store.Player.CurrentRoomId)),
            "examine" => Examine(cmd.Argument),
            "inventory" => ShowInventory(),
            // Basic interaction
            "get" or "take" => Get(cmd.Argument),
            "drop" => Drop(cmd.Argument),
            "wield" => Wield(cmd.Argument),
            "wear" => _commands.Wear(cmd.Argument),
            "remove" => _commands.Remove(cmd.Argument),
            // Advanced interaction
            "eat" or "drink" => _commands.Eat(cmd.Argument),
            "dig" => _commands.Dig(),
            "fly" => _commands.Fly(),
            "climb" => _commands.Climb(cmd.Argument),
            "enter" => _commands.Enter(cmd.Argument),
            "open" => _commands.Open(cmd.Argument),
            "close" => _commands.Close(cmd.Argument),
            "push" => _commands.Push(cmd.Argument),
            "pull" => _commands.Pull(cmd.Argument),
            "turn" => _commands.Turn(cmd.Argument),
            "peck" => _commands.Peck(cmd.Argument),
            "bite" => _commands.Bite(cmd.Argument),
            "skin" => _commands.Skin(cmd.Argument),
            "kick" => _commands.Kick(cmd.Argument),
            "cut" => _commands.Cut(cmd.Argument),
            "fill" => _commands.Fill(cmd.Argument),
            "pour" => _commands.Pour(cmd.Argument),
            "light" => _commands.Light(cmd.Argument),
            "smell" => _commands.Smell(),
            "tickle" => CreateOutput(o => o.AddLine("Tee hee!")),
            "give" => _commands.Give(cmd.Argument),
            "kill" or "attack" => _commands.Kill(cmd.Argument),
            "cast" => CreateOutput(o =>
            {
                if (_store.Player.CurrentForm != AnimalForm.Human)
                    o.AddLine("Your tiny brain can't cope with the intricacies of magic in this form.");
                else
                    o.AddLine("You mutter an incantation but nothing happens.");
            }),
            "say" => _commands.Say(cmd.Argument),
            "empty" => CreateOutput(o => o.AddLine("You empty it out.")),
            // Special
            "goto" => Teleport(cmd.Argument),
            "pray" or "kneel" => Pray(),
            "jump" => Jump(),
            // Meta
            "h" or "help" => ShowHelp(cmd.Argument),
            "reset" => StartNewGame(),
            "r" => CreateOutput(o => {}),
            "q" or "qu" or "qui" => CreateOutput(o =>
                o.AddLine("In order to prevent unnecessary screaming, please type 'quit' in its entirety to quit.")),
            "quit" => ShowEntryMenu(true),
            _ => CreateOutput(o => o.AddLine("Pardon?"))
        };
    }

    private GameOutput Move(Direction dir)
    {
        var player = _store.Player;
        var room = _store.GetRoomById(player.CurrentRoomId);
        if (room == null)
            return CreateOutput(o => o.AddLine("You are lost in the void."));

        var exit = room.GetExit(dir);
        if (exit == null)
            return CreateOutput(o => o.AddLine("You cannot go that way!"));

        // Try direct room first
        var targetRoom = _store.GetRoomById(exit);

        // If not a room, check if it's a door/marker that links to another room
        if (targetRoom == null)
            targetRoom = ResolveDoorExit(exit);

        if (targetRoom == null)
            return CreateOutput(o => o.AddLine("You cannot go that way!"));

        player.CurrentRoomId = targetRoom.Id;
        _store.UpdatePlayer(player);

        _logger?.LogInformation("Moved {Direction} to {Room}", dir, targetRoom.Id);
        var output = ShowCurrentRoom();
        _events.OnRoomEntry(targetRoom.Id, output);
        return output;
    }

    private Room? ResolveDoorExit(string exitId)
    {
        // Markers/doors are objects with a "linked" field pointing to another marker
        // The linked marker's location is the destination room
        var doorUid = _store.GetObjectUid(exitId);
        if (doorUid == 0) return null;

        var door = _store.GetObjectByUid(doorUid);
        if (door == null || door.Linked == 0) return null;

        // The linked object is in the destination room
        var linkedDoor = _store.GetObjectByUid(door.Linked);
        if (linkedDoor == null) return null;

        // The linked door's location is the UID of the destination room
        return _store.GetRoomByUid(linkedDoor.Location);
    }

    private GameOutput ShowCurrentRoom()
    {
        var player = _store.Player;
        var room = _store.GetRoomById(player.CurrentRoomId);
        if (room == null)
            return CreateOutput(o => o.AddLine("You are nowhere."));

        var output = CreateOutput(o =>
        {
            o.AddLine("-= " + "&+g" + room.Name + "#" + " =-");
            o.AddLine(room.GetDescriptionForForm(player.CurrentForm));

            if (room.IsDeath)
            {
                Kill(o);
                return;
            }

            ShowObjectsInRoom(o, player.CurrentRoomId);
            ShowMobilesInRoom(o, player.CurrentRoomId);
            ShowDirections(o, room);
            o.AddLine(GetPrompt());
        });

        output.Inventory = BuildInventory();
        output.AsciiArt = AsciiArt.GetArt(player.CurrentForm);
        output.StatusText = player.CurrentForm.ToFormString();
        output.StrengthMana = player.CurrentForm == AnimalForm.Beetle ? "S:5\\5\nM:0\\0" : "S:999\\999 M:999\\999";
        return output;
    }

    private void ShowObjectsInRoom(GameOutput output, string roomId)
    {
        int rloc = _store.GetRoomUid(roomId);
        foreach (var obj in _store.GetObjectsAtLocation(rloc))
        {
            if (!string.IsNullOrEmpty(obj.Description))
                output.AddLine(obj.Description);
        }
    }

    private void ShowMobilesInRoom(GameOutput output, string roomId)
    {
        int rloc = _store.GetRoomUid(roomId);
        foreach (var mob in _store.GetMobilesAtLocation(rloc))
        {
            if (!string.IsNullOrEmpty(mob.Description))
            {
                output.AddLine(mob.Description);
                var carried = _store.GetObjectsAtLocation(mob.Uid)
                    .Select(o => o.Name)
                    .ToList();
                if (carried.Any())
                    output.AddLine(mob.PName + " is carrying: " + string.Join(", ", carried) + ".");
            }
        }
    }

    private void ShowDirections(GameOutput output, Room room)
    {
        output.AddLine("\nObvious exits are:");
        var dirLabels = new (Direction Dir, string Label)[]
        {
            (Direction.North, "&+yNorth#"),
            (Direction.South, "&+ySouth#"),
            (Direction.East, " &+yEast#"),
            (Direction.West, " &+yWest#"),
            (Direction.Up, "   &+yUp#"),
            (Direction.Down, " &+yDown#"),
        };

        foreach (var (dir, label) in dirLabels)
        {
            var exit = room.GetExit(dir);
            if (exit != null)
            {
                var targetRoom = _store.GetRoomById(exit);
                if (targetRoom != null)
                    output.AddLine(label + " :  " + "&+M" + targetRoom.Name + "#");
            }
        }
    }

    private GameOutput Examine(string? arg)
    {
        if (arg == null)
            return CreateOutput(o => o.AddLine("Examine what?"));

        if (arg == "me")
        {
            return CreateOutput(o =>
            {
                var form = _store.Player.CurrentForm;
                if (form == AnimalForm.Human) o.AddLine("A strapping example of an adventurer if ever there was one!");
                else if (form == AnimalForm.Beetle) o.AddLine("You have been magically transformed into a beetle!");
                else o.AddLine("You have been transformed into a " + form.ToFormString() + "!");
            });
        }

        return CreateOutput(o =>
        {
            int rloc = _store.GetRoomUid(_store.Player.CurrentRoomId);
            bool found = false;

            foreach (var obj in _store.GetObjectsByName(arg, rloc, PlayerState.PlayerUid))
            {
                o.AddLine(obj.Examine);
                found = true;
            }

            foreach (var mob in _store.GetMobilesAtLocation(rloc))
            {
                if (mob.Id == arg || mob.AltName == arg)
                {
                    o.AddLine(mob.Examine);
                    found = true;
                }
            }

            if (!found) o.AddLine("It's not here.");
        });
    }

    private GameOutput ShowInventory()
    {
        var inv = BuildInventory();
        return CreateOutput(o =>
        {
            o.AddLine("Worn: " + (inv.Worn.Any() ? string.Join(" ", inv.Worn) : ""));
            o.AddLine("Carried: " + (inv.Carried.Any() ? string.Join(" ", inv.Carried) : ""));
            o.AddLine("Wielded: " + (inv.Wielded.Any() ? string.Join(" ", inv.Wielded) : ""));
            o.Inventory = inv;
        });
    }

    private GameOutput Drop(string? item)
    {
        if (item == null)
            return CreateOutput(o => o.AddLine("Drop what?"));

        return CreateOutput(o =>
        {
            int rloc = _store.GetRoomUid(_store.Player.CurrentRoomId);
            foreach (var obj in _store.GetPlayerInventory(PlayerState.PlayerUid))
            {
                if (obj.Name == item)
                {
                    o.AddLine("You drop the " + obj.Name + " to the ground.");
                    obj.Location = rloc;
                    obj.Carried = 0;
                    obj.Worn = 0;
                    obj.Wielded = 0;
                    _store.UpdateObject(obj);
                    o.Inventory = BuildInventory();
                    return;
                }
            }
            o.AddLine("You aren't carrying it.");
        });
    }

    private GameOutput Get(string? item)
    {
        if (item == null)
            return CreateOutput(o => o.AddLine("Get what?"));

        // Check form-based inventory limits
        var restriction = _commands.CheckGetRestriction();
        if (restriction != null)
            return CreateOutput(o => o.AddLine(restriction));

        return CreateOutput(o =>
        {
            int rloc = _store.GetRoomUid(_store.Player.CurrentRoomId);
            foreach (var obj in _store.GetObjectsAtLocation(rloc))
            {
                if (obj.Name == item)
                {
                    if (obj.NoGet)
                    {
                        o.AddLine("You cannot take that.");
                        return;
                    }
                    o.AddLine("You take the " + obj.Name);
                    obj.Location = PlayerState.PlayerUid;
                    obj.Carried = PlayerState.PlayerUid;
                    _store.UpdateObject(obj);
                    o.Inventory = BuildInventory();
                    return;
                }
            }
            o.AddLine("It's not here to get.");
        });
    }

    private GameOutput Wield(string? item)
    {
        if (item == null)
            return CreateOutput(o => o.AddLine("Wield what?"));

        // Check form-based wield restrictions
        var restriction = _commands.CheckWieldRestriction();
        if (restriction != null)
            return CreateOutput(o => o.AddLine(restriction));

        return CreateOutput(o =>
        {
            foreach (var obj in _store.GetPlayerInventory(PlayerState.PlayerUid))
            {
                if (obj.Name == item)
                {
                    if (obj.Wielded == PlayerState.PlayerUid)
                    {
                        o.AddLine("You are already wielding it.");
                        return;
                    }
                    if (!obj.IsWeapon)
                    {
                        o.AddLine("The " + obj.Name + " is not a weapon.");
                        return;
                    }

                    // Unwield current weapon
                    foreach (var other in _store.GetPlayerInventory(PlayerState.PlayerUid))
                    {
                        if (other.Wielded == PlayerState.PlayerUid)
                        {
                            o.AddLine("You stop wielding the " + other.Name);
                            other.Wielded = 0;
                            other.Carried = PlayerState.PlayerUid;
                            _store.UpdateObject(other);
                        }
                    }

                    o.AddLine("You grasp the " + obj.Name + " in your hands and prepare for battle.");
                    obj.Carried = 0;
                    obj.Wielded = PlayerState.PlayerUid;
                    _store.UpdateObject(obj);
                    o.Inventory = BuildInventory();
                    return;
                }
            }
            o.AddLine("You aren't carrying it.");
        });
    }

    private GameOutput Teleport(string? location)
    {
        if (location == null)
            return CreateOutput(o => o.AddLine("Goto where?"));

        var room = _store.GetRoomById(location);
        if (room == null)
            return CreateOutput(o => o.AddLine("No such location."));

        var player = _store.Player;
        player.CurrentRoomId = room.Id;
        _store.UpdatePlayer(player);

        var output = CreateOutput(o => o.AddLine("You teleport out in a flash of light!"));
        return MergeOutput(output, ShowCurrentRoom());
    }

    private GameOutput Pray()
    {
        if (_store.Player.CurrentRoomId == "room86")
        {
            var output = CreateOutput(o =>
            {
                o.AddLine("You &+Ckneel# humbly to the ground in the centre of the &+Mt#&+Wo#&+Ma#&+Wd#&+Ms#&+Wt#&+Mo#&+Wo#&+Ml# ring to pray.");
                o.AddLine("As you do so, the &+Bskies# darken to a pure jet black, giving way to a &+Rterrible# booming voice...");
                o.AddLine("'&+WAnother insect come to test his mettle against me! Pathetic. All your weapons and armours are nothing to one such as me!#'\n");
                o.AddLine("A bolt of &+Ylightning# arcs from the &+Csky#, striking you square in the forehead, sending you reeling across the &+gground#.");
                o.AddLine("\nJust before you lose consciousness, a voice whispers as though from within your own head...");
                o.AddLine("'&+WGoodbye, little insect. I think we shall not meet again.#'");
            });

            var player = _store.Player;
            var room72Uid = _store.GetRoomUid("room72");
            player.CurrentRoomId = "room72";
            _store.UpdatePlayer(player);
            LimboAll();
            ChangeForm(AnimalForm.Beetle);

            output.AsciiArt = AsciiArt.GetArt(AnimalForm.Beetle);
            output.StatusText = "beetle";
            output.StrengthMana = "S:5\\5\nM:0\\0";
            output.Inventory = BuildInventory();
            _logger?.LogInformation("Player transformed to beetle at room86");
            return output;
        }

        return CreateOutput(o => o.AddLine("You say a silent prayer to the old gods and the new."));
    }

    private GameOutput Jump()
    {
        if (_store.Player.CurrentRoomId == "room86")
        {
            return CreateOutput(o =>
            {
                o.AddLine("Without a second thought you leap from the cliff, confident that this is but a trick to keep you from your goal.");
                o.AddLine("Alas it is not, prompting a highly undignified thought to flash through your brain before it's unceremoniously smashed on the rocks below.");
                Kill(o);
            });
        }

        return CreateOutput(o => o.AddLine("Wheeeee..."));
    }

    private GameOutput ShowHelp(string? subject)
    {
        return CreateOutput(o =>
        {
            o.AddLine("Basic commands are:");
            o.AddLine("   north, south, east, west, up, down - movement commands.");
            o.AddLine("   look - see the room you are in.");
            o.AddLine("   examine - look closely at an object.");
            o.AddLine("   inventory - see what items you currently have.");
            o.AddLine("   get, drop, wield, wear, push, pull - manipulate objects.");
            o.AddLine("   pray - if all else fails...");
            o.AddLine("\nThere are many other commands to be found, lots of which are critical to your quest :)");
            o.AddLine("\nFor your convenience, all commands can be shortened - e.g. 'l, lo, loo (despite appearances) and look' all perform the same function.");
        });
    }

    private void Kill(GameOutput output)
    {
        LimboAll();
        var player = _store.Player;
        player.IsDead = true;
        _store.UpdatePlayer(player);
        _logger?.LogInformation("Player died");

        output.IsDeathMenu = true;
        output.AddLines(
            "------------------------------------------------------------------------------\n",
            "                &+ROh dear... you seem to be slightly dead.#\n",
            "------------------------------------------------------------------------------",
            "         &+y ___________________________#",
            "     &+R()==#&+y(__________________________(@#&+R==()#",
            "           &+y|                        |#",
            "           &+y| &+YMenu [#&+RDeath#&+Y]#           &+y|#",
            "           &+y|                        |#",
            "           &+y|# &+CR#&+Y)# &+Westart Meta#         &+y|#",
            "           &+y|# &+RQ#&+Y)# &+Wuit game#            &+y|#",
            "          &+y_|________________________|#",
            "     &+R()==#&+y(__________________________(@#&+R==()#"
        );
    }

    private GameOutput ShowDeathMenu(bool header)
    {
        var output = CreateOutput(o =>
        {
            o.IsDeathMenu = true;
            if (header)
            {
                o.AddLine("------------------------------------------------------------------------------\n");
                o.AddLine("                &+ROh dear... you seem to be slightly dead.#\n");
                o.AddLine("------------------------------------------------------------------------------");
            }
            o.AddLines(
                "         &+y ___________________________#",
                "     &+R()==#&+y(__________________________(@#&+R==()#",
                "           &+y|                        |#",
                "           &+y| &+YMenu [#&+RDeath#&+Y]#           &+y|#",
                "           &+y|                        |#",
                "           &+y|# &+CR#&+Y)# &+Westart Meta#         &+y|#",
                "           &+y|# &+RQ#&+Y)# &+Wuit game#            &+y|#",
                "          &+y_|________________________|#",
                "     &+R()==#&+y(__________________________(@#&+R==()#"
            );
        });
        return output;
    }

    private GameOutput ShowEntryMenu(bool header)
    {
        var player = _store.Player;
        player.IsInEntryMenu = true;
        _store.UpdatePlayer(player);

        return CreateOutput(o =>
        {
            o.IsEntryMenu = true;
            o.ShouldClearOutput = header;
            if (header)
            {
                o.AddLine("-----------------------------------------------------------------------------\n");
                o.AddLine("                            &+MWelcome to Metamorphosis!#\n");
                o.AddLine("-----------------------------------------------------------------------------");
            }
            o.AddLines(
                "                         &+y ___________________________#",
                "                   &+M  ()==#&+y(__________________________(@#&+M==()#",
                "                           &+y|                        |#",
                "                           &+y|# &+YMenu [#&+CEntry#&+Y]#           &+y|#",
                "                           &+y|                        |#",
                "                           &+y|# &+CN#&+Y)# &+Wew Game#             &+y|#",
                "                           &+y|# &+CL#&+Y)# &+Woad Previous Game#   &+y|#",
                "                           &+y|# &+CH#&+Y)# &+Welp#                 &+y|#",
                "                           &+y|                        |#",
                "                          &+y_|________________________|#",
                "                     &+M()==#&+y(__________________________(@#&+M==()#"
            );
        });
    }

    private GameOutput StartNewGame()
    {
        _store.Reset(_baseRooms, _baseMobiles, _baseObjects);
        _events.Reset();
        var player = _store.Player;
        player.IsInEntryMenu = false;
        player.IsDead = false;
        _store.UpdatePlayer(player);
        _logger?.LogInformation("New game started");

        // Show room85 (story prologue) then move player to room1 (game start)
        var prologueRoom = _store.GetRoomById(PlayerState.StartingRoom);
        var prologue = CreateOutput(o =>
        {
            o.ShouldClearOutput = true;
            if (prologueRoom != null)
            {
                o.AddLine("-= " + "&+g" + prologueRoom.Name + "#" + " =-");
                o.AddLine(prologueRoom.Description);
                o.AddLine("");
            }
        });

        // Move to room87 (Sleeping Quarters) — where the adventure begins
        player.CurrentRoomId = "room87";
        _store.UpdatePlayer(player);

        var roomOutput = ShowCurrentRoom();
        var helpLine = CreateOutput(o =>
            o.AddLine("If you are new to text adventures, type 'help' for some commands to get started."));

        return MergeOutput(MergeOutput(prologue, roomOutput), helpLine);
    }

    private GameOutput LoadGame()
    {
        // In the browser version, this would load from localStorage.
        // For now, just start showing current room.
        var player = _store.Player;
        player.IsInEntryMenu = false;
        _store.UpdatePlayer(player);

        return ShowCurrentRoom();
    }

    private void LimboAll()
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

    private void ChangeForm(AnimalForm form)
    {
        var player = _store.Player;
        player.CurrentForm = form;
        _store.UpdatePlayer(player);
        _logger?.LogInformation("Form changed to {Form}", form);
    }

    private InventoryState BuildInventory()
    {
        var inv = new InventoryState();
        foreach (var obj in _store.GetPlayerInventory(PlayerState.PlayerUid))
        {
            if (obj.Worn == PlayerState.PlayerUid) inv.Worn.Add(obj.Name);
            if (obj.Carried == PlayerState.PlayerUid) inv.Carried.Add(obj.Name);
            if (obj.Wielded == PlayerState.PlayerUid) inv.Wielded.Add(obj.Name);
        }
        return inv;
    }

    private string GetPrompt()
    {
        return _store.Player.CurrentForm == AnimalForm.Beetle
            ? "&+YStr:# &+M5/5# >"
            : "&+YStr:# &+M999/999# >";
    }

    private static GameOutput CreateOutput(Action<GameOutput> configure)
    {
        var output = new GameOutput();
        configure(output);
        return output;
    }

    private static GameOutput MergeOutput(GameOutput first, GameOutput second)
    {
        first.Lines.AddRange(second.Lines);
        first.AsciiArt ??= second.AsciiArt;
        first.StatusText ??= second.StatusText;
        first.Inventory ??= second.Inventory;
        first.StrengthMana ??= second.StrengthMana;
        first.AudioTrack ??= second.AudioTrack;
        first.IsEntryMenu = second.IsEntryMenu || first.IsEntryMenu;
        first.IsDeathMenu = second.IsDeathMenu || first.IsDeathMenu;
        return first;
    }
}
