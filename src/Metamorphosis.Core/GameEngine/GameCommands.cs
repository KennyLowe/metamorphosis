using Metamorphosis.Core.Data;
using Metamorphosis.Core.Models;

namespace Metamorphosis.Core.GameEngine;

/// <summary>
/// Handles all game commands beyond basic movement/look/get/drop.
/// Implements mechanics from the original C MUD engine.
/// </summary>
public class GameCommands
{
    private readonly IGameDataStore _store;

    public GameCommands(IGameDataStore store)
    {
        _store = store;
    }

    public GameOutput Eat(string? arg)
    {
        if (arg == null)
            return Output(o => o.AddLine("Eat what?"));

        var player = _store.Player;
        int rloc = _store.GetRoomUid(player.CurrentRoomId);

        // Find the item to eat/drink
        foreach (var obj in _store.GetObjectsByName(arg, rloc, PlayerState.PlayerUid))
        {
            // Toadstool in the toadstool clearing (room75)
            if (obj.ZName == "toadstool" && player.CurrentRoomId == "room75")
            {
                return Output(o =>
                {
                    o.AddLine("As you eat the &+Ctoadstool#, a strange feeling overpowers you and you black out.");
                    o.AddLine("");
                    o.AddLine("You awaken in a &+Gclearing# identical to the one you were in, but the way you");
                    o.AddLine("entered is now gone, and a new exit is revealed to the &+Gnorth#, a &+Ccauldron#");
                    o.AddLine("now dominating the centre of the ring.");
                    // Dump equipment and move to room72
                    DumpPlayerItems(player.CurrentRoomId);
                    player.CurrentRoomId = "room72";
                    _store.UpdatePlayer(player);
                    // Destroy the toadstool
                    obj.Location = _store.GetRoomUid("room74"); // limbo
                    _store.UpdateObject(obj);
                });
            }

            // Snake vial — transform to snake
            if (obj.ZName == "vial_snake" || obj.Name == "vial")
            {
                return DrinkSnakeVial(player, obj);
            }

            // Human liquid — transform back to human
            if (obj.ZName == "liquid_human" || obj.Name == "potion")
            {
                return DrinkHumanPotion(player, obj);
            }

            return Output(o => o.AddLine("You can't eat that."));
        }

        return Output(o => o.AddLine("You don't have that."));
    }

    public GameOutput Drink(string? arg)
    {
        return Eat(arg); // Same mechanic
    }

    private GameOutput DrinkSnakeVial(PlayerState player, GameObject vial)
    {
        if (player.CurrentForm == AnimalForm.Snake)
            return Output(o => o.AddLine("You are already a &+Gsnake#!"));

        return Output(o =>
        {
            string bodyPart = player.CurrentForm switch
            {
                AnimalForm.Bird => "Your &+Cwings# &+Msubsume# into your &+Cbody#",
                AnimalForm.Beetle => "Your &+Ccarapace# softens and &+Melongates#",
                _ => "Your arms &+Msubsume# into your &+Cbody#"
            };

            o.AddLine("As you consume the &+Ggreen# liquid, your &+Cbody# begins to " +
                (player.CurrentForm == AnimalForm.Human ? "shrink" : "grow") + ".");
            o.AddLine(bodyPart + ", and your &+Clegs# &+Mmerge# into one");
            o.AddLine("limb which rapidly becomes more a part of your &+Ctorso# than a separate");
            o.AddLine("limb. Within seconds, your &+Cbody# has changed into that of a &+Gsnake#.");

            DumpPlayerItems(player.CurrentRoomId);
            player.CurrentForm = AnimalForm.Snake;
            _store.UpdatePlayer(player);
            o.StatusText = "snake";
            o.AsciiArt = AsciiArt.GetArt(AnimalForm.Snake);
            o.StrengthMana = "S:999\\999 M:999\\999";
        });
    }

    private GameOutput DrinkHumanPotion(PlayerState player, GameObject potion)
    {
        if (player.CurrentForm == AnimalForm.Human)
            return Output(o => o.AddLine("You are already &+Chuman#!"));

        return Output(o =>
        {
            o.AddLine("As you take a sip of the &+Cliquid#, a violent spasm shoots down your throat,");
            o.AddLine("choking off your screams of pain as your body cracks and breaks, reforming");
            o.AddLine("into the form of a human.");

            // Special case: room72 toadstool ring — fairy gives staff
            if (player.CurrentRoomId == "room72")
            {
                o.AddLine("");
                o.AddLine("The &+Ctoadstool# &+Mring# shimmers and sparkles, and in a flash of &+Ylight# a &+Mfairy#");
                o.AddLine("appears in front of you.");
                o.AddLine("'&+MSo the legends were true and you have finally come to rid this");
                o.AddLine("land of the scourge of the Chimera. Take this staff, for only it can");
                o.AddLine("hope to penetrate the evil of the Chimera and slay it once and for all!#'");
                o.AddLine("She hands you a large &+Cstaff#, &+Rkisses# you lightly on the &+Mcheek# and");
                o.AddLine("vanishes as quickly as she arrived.");
                // Give player the staff
                GivePlayerItem("staff");
            }

            player.CurrentForm = AnimalForm.Human;
            _store.UpdatePlayer(player);
            o.StatusText = "human";
            o.AsciiArt = AsciiArt.GetArt(AnimalForm.Human);
            o.StrengthMana = "S:999\\999 M:999\\999";
        });
    }

    public GameOutput Dig()
    {
        var player = _store.Player;

        // Room75 (toadstool clearing): dig up toadstool
        if (player.CurrentRoomId == "room75")
        {
            return Output(o =>
            {
                o.AddLine("You &+Cuproot# a large &+Mpurple# &+Ctoadstool#.");
                SpawnItem("toadstool", player.CurrentRoomId);
            });
        }

        // Room1 (woodland clearing): dig up snake vial
        if (player.CurrentRoomId == "room1")
        {
            return Output(o =>
            {
                o.AddLine("Rooting around in the &+ydirt# you find a strange &+Cvial#.");
                SpawnItem("vial_snake", player.CurrentRoomId);
            });
        }

        return Output(o => o.AddLine("You dig around but find nothing of interest."));
    }

    public GameOutput Fly()
    {
        var player = _store.Player;
        if (player.CurrentForm != AnimalForm.Bird)
            return Output(o => o.AddLine("You can't fly!"));

        // Room-specific flight
        if (player.CurrentRoomId == "room7" || player.CurrentRoomId == "room58")
        {
            return Output(o =>
            {
                o.AddLine("You spread your wings and soar between the branches.");
                var target = player.CurrentRoomId == "room7" ? "room58" : "room7";
                player.CurrentRoomId = target;
                _store.UpdatePlayer(player);
            });
        }

        return Output(o =>
        {
            o.AddLine("You spread your wings and soar briefly through the air before landing again.");
        });
    }

    public GameOutput Climb(string? arg)
    {
        var player = _store.Player;

        if (player.CurrentForm == AnimalForm.Fish)
            return Output(o => o.AddLine("You're a fish. You can't climb."));

        if (player.CurrentForm == AnimalForm.Beetle)
            return Output(o =>
            {
                o.AddLine("A large bird swoops down, snatches you up in its beak and devours you whole.");
                o.AddLine("&+REaten alive!#");
                o.IsDeathMenu = true;
                player.IsDead = true;
                _store.UpdatePlayer(player);
            });

        if (player.CurrentForm == AnimalForm.Bird)
            return Output(o => o.AddLine("You don't need to climb, you can fly!"));

        return Output(o => o.AddLine("The surface is too smooth to get a handhold."));
    }

    public GameOutput Peck(string? arg)
    {
        var player = _store.Player;
        if (player.CurrentForm != AnimalForm.Bird)
            return Output(o => o.AddLine("You don't have a beak to peck with!"));

        return Output(o => o.AddLine("You peck at it but nothing happens."));
    }

    public GameOutput Bite(string? arg)
    {
        var player = _store.Player;
        if (player.CurrentForm != AnimalForm.Snake)
            return Output(o => o.AddLine("You don't have fangs to bite with!"));

        if (arg == null)
            return Output(o => o.AddLine("Bite what?"));

        // Find target mobile
        int rloc = _store.GetRoomUid(player.CurrentRoomId);
        foreach (var mob in _store.GetMobilesAtLocation(rloc))
        {
            if (mob.Id == arg || mob.AltName == arg || mob.PName.ToLower().Contains(arg))
            {
                return Output(o =>
                {
                    o.AddLine("You lunge forward and bite down hard on the " + mob.PName + "!");
                    o.AddLine("The " + mob.PName + " collapses, lifeless.");
                    mob.Location = _store.GetRoomUid("room74"); // limbo
                    _store.UpdateMobile(mob);
                });
            }
        }

        return Output(o => o.AddLine("There's nothing here to bite."));
    }

    public GameOutput Skin(string? arg)
    {
        var player = _store.Player;
        if (player.CurrentForm != AnimalForm.Bird)
            return Output(o => o.AddLine("You need a beak sharp enough to skin something."));

        return Output(o => o.AddLine("There's nothing here to skin."));
    }

    public GameOutput Kick(string? arg)
    {
        var player = _store.Player;
        if (player.CurrentForm == AnimalForm.Snake)
            return Output(o => o.AddLine("You don't have any legs!"));

        return Output(o => o.AddLine("You kick around but accomplish nothing."));
    }

    public GameOutput Open(string? arg)
    {
        if (arg == null)
            return Output(o => o.AddLine("Open what?"));

        var player = _store.Player;
        int rloc = _store.GetRoomUid(player.CurrentRoomId);

        foreach (var obj in _store.GetObjectsByName(arg, rloc, PlayerState.PlayerUid))
        {
            if (obj.IsDoor || obj.IsContainer)
            {
                if (obj.State == 1)
                    return Output(o => o.AddLine("It's already open."));

                obj.State = 1;
                _store.UpdateObject(obj);
                return Output(o => o.AddLine("You open the " + obj.Name + "."));
            }
        }

        return Output(o => o.AddLine("You can't open that."));
    }

    public GameOutput Close(string? arg)
    {
        if (arg == null)
            return Output(o => o.AddLine("Close what?"));

        var player = _store.Player;
        int rloc = _store.GetRoomUid(player.CurrentRoomId);

        foreach (var obj in _store.GetObjectsByName(arg, rloc, PlayerState.PlayerUid))
        {
            if (obj.IsDoor || obj.IsContainer)
            {
                if (obj.State == 0)
                    return Output(o => o.AddLine("It's already closed."));

                obj.State = 0;
                _store.UpdateObject(obj);
                return Output(o => o.AddLine("You close the " + obj.Name + "."));
            }
        }

        return Output(o => o.AddLine("You can't close that."));
    }

    public GameOutput Push(string? arg)
    {
        if (arg == null)
            return Output(o => o.AddLine("Push what?"));
        return Output(o => o.AddLine("Nothing happens."));
    }

    public GameOutput Pull(string? arg)
    {
        if (arg == null)
            return Output(o => o.AddLine("Pull what?"));
        return Output(o => o.AddLine("Nothing happens."));
    }

    public GameOutput Wear(string? arg)
    {
        var player = _store.Player;
        if (player.CurrentForm != AnimalForm.Human)
            return Output(o => o.AddLine("You can't wear anything in your current form."));

        if (arg == null)
            return Output(o => o.AddLine("Wear what?"));

        foreach (var obj in _store.GetPlayerInventory(PlayerState.PlayerUid))
        {
            if (obj.Name == arg)
            {
                if (!obj.IsWearable)
                    return Output(o => o.AddLine("You can't wear that."));

                if (obj.Worn == PlayerState.PlayerUid)
                    return Output(o => o.AddLine("You're already wearing it."));

                obj.Worn = PlayerState.PlayerUid;
                obj.Carried = 0;
                _store.UpdateObject(obj);
                return Output(o =>
                {
                    o.AddLine("You put on the " + obj.Name + ".");
                    o.Inventory = BuildInventory();
                });
            }
        }

        return Output(o => o.AddLine("You aren't carrying it."));
    }

    public GameOutput Remove(string? arg)
    {
        if (arg == null)
            return Output(o => o.AddLine("Remove what?"));

        foreach (var obj in _store.GetPlayerInventory(PlayerState.PlayerUid))
        {
            if (obj.Name == arg && obj.Worn == PlayerState.PlayerUid)
            {
                obj.Worn = 0;
                obj.Carried = PlayerState.PlayerUid;
                _store.UpdateObject(obj);
                return Output(o =>
                {
                    o.AddLine("You remove the " + obj.Name + ".");
                    o.Inventory = BuildInventory();
                });
            }
        }

        return Output(o => o.AddLine("You aren't wearing that."));
    }

    public GameOutput Say(string? arg)
    {
        if (arg == null)
            return Output(o => o.AddLine("Say what?"));

        return Output(o => o.AddLine("You say '" + arg + "'."));
    }

    public GameOutput Give(string? arg)
    {
        if (arg == null)
            return Output(o => o.AddLine("Give what to whom?"));
        return Output(o => o.AddLine("Give what to whom?"));
    }

    public GameOutput Enter(string? arg)
    {
        if (arg == null)
            return Output(o => o.AddLine("Enter what?"));

        var player = _store.Player;
        int rloc = _store.GetRoomUid(player.CurrentRoomId);

        foreach (var obj in _store.GetObjectsByName(arg, rloc, PlayerState.PlayerUid))
        {
            // Hole mechanics — form-specific
            if (obj.Name == "hole" || obj.ZName == "hole")
            {
                if (player.CurrentForm == AnimalForm.Snake)
                {
                    return Output(o =>
                    {
                        o.AddLine("You &+Gslither# down the hole.");
                        player.CurrentRoomId = "room35";
                        _store.UpdatePlayer(player);
                    });
                }
                if (player.CurrentForm == AnimalForm.Beetle)
                {
                    return Output(o =>
                    {
                        o.AddLine("You &+Cscuttle# down the hole.");
                        player.CurrentRoomId = "room35";
                        _store.UpdatePlayer(player);
                    });
                }
                return Output(o => o.AddLine("You're too big to fit in there."));
            }
        }

        return Output(o => o.AddLine("You can't enter that."));
    }

    public GameOutput Smell()
    {
        var player = _store.Player;

        if (player.CurrentForm == AnimalForm.Beetle)
            return Output(o => o.AddLine("Your antennae twitch as you sense the air around you."));

        return Output(o => o.AddLine("You sniff the air but detect nothing unusual."));
    }

    public GameOutput Fill(string? arg)
    {
        if (arg == null)
            return Output(o => o.AddLine("Fill what?"));
        return Output(o => o.AddLine("There's nothing to fill it with here."));
    }

    public GameOutput Pour(string? arg)
    {
        if (arg == null)
            return Output(o => o.AddLine("Pour what?"));
        return Output(o => o.AddLine("You pour it out. It seeps into the earth."));
    }

    public GameOutput Light(string? arg)
    {
        if (arg == null)
            return Output(o => o.AddLine("Light what?"));
        return Output(o => o.AddLine("You don't have anything to light it with."));
    }

    public GameOutput Cut(string? arg)
    {
        if (arg == null)
            return Output(o => o.AddLine("Cut what?"));
        return Output(o => o.AddLine("You can't cut that."));
    }

    public GameOutput Turn(string? arg)
    {
        if (arg == null)
            return Output(o => o.AddLine("Turn what?"));
        return Output(o => o.AddLine("Nothing happens."));
    }

    public GameOutput Kill(string? arg)
    {
        if (arg == null)
            return Output(o => o.AddLine("Kill what?"));

        var player = _store.Player;
        if (player.CurrentForm != AnimalForm.Human)
            return Output(o => o.AddLine("You're too puny in this form to fight anything."));

        int rloc = _store.GetRoomUid(player.CurrentRoomId);
        foreach (var mob in _store.GetMobilesAtLocation(rloc))
        {
            if (mob.Id == arg || mob.AltName == arg || mob.PName.ToLower().Contains(arg))
            {
                return Output(o =>
                {
                    o.AddLine("You attack the " + mob.PName + "!");
                    o.AddLine("After a fierce battle, the " + mob.PName + " is defeated.");
                    mob.Location = _store.GetRoomUid("room74"); // limbo
                    _store.UpdateMobile(mob);
                });
            }
        }

        return Output(o => o.AddLine("There's nothing here to fight."));
    }

    // Wield restriction check — called from GameEngine before wielding
    public string? CheckWieldRestriction()
    {
        var player = _store.Player;
        return player.CurrentForm switch
        {
            AnimalForm.Snake => "You don't have any hands!",
            AnimalForm.Fish => "You don't have any hands!",
            AnimalForm.Bird => "You can't grasp a weapon in your wings.",
            AnimalForm.Beetle => "Your legs are too small to wield anything.",
            _ => null // Human can wield
        };
    }

    // Get restriction check — form-based inventory limits
    public string? CheckGetRestriction()
    {
        var player = _store.Player;
        var inventory = _store.GetPlayerInventory(PlayerState.PlayerUid).ToList();
        int count = inventory.Count;

        return player.CurrentForm switch
        {
            AnimalForm.Fish when count >= 1 => "Your mouth is already full.",
            AnimalForm.Beetle when count >= 1 => "You can only strap one item to your back.",
            AnimalForm.Snake when count >= 2 => "You can't carry any more.",
            AnimalForm.Bird when count >= 2 => "Your beak is already full.",
            _ => null
        };
    }

    private void DumpPlayerItems(string roomId)
    {
        int rloc = _store.GetRoomUid(roomId);
        foreach (var obj in _store.GetPlayerInventory(PlayerState.PlayerUid).ToList())
        {
            obj.Carried = 0;
            obj.Wielded = 0;
            obj.Worn = 0;
            obj.Location = rloc;
            _store.UpdateObject(obj);
        }
    }

    private void SpawnItem(string zname, string roomId)
    {
        // Find item in limbo and move to room
        int rloc = _store.GetRoomUid(roomId);
        int limbo = _store.GetRoomUid("room74");
        foreach (var obj in _store.GetObjectsAtLocation(limbo))
        {
            if (obj.ZName == zname)
            {
                obj.Location = rloc;
                _store.UpdateObject(obj);
                return;
            }
        }
    }

    private void GivePlayerItem(string name)
    {
        // Find item by name and give to player
        foreach (var room in _store.GetAllRooms())
        {
            foreach (var obj in _store.GetObjectsAtLocation(room.Uid))
            {
                if (obj.Name == name || obj.ZName == name)
                {
                    obj.Location = PlayerState.PlayerUid;
                    obj.Carried = PlayerState.PlayerUid;
                    _store.UpdateObject(obj);
                    return;
                }
            }
        }
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

    private static GameOutput Output(Action<GameOutput> configure)
    {
        var output = new GameOutput();
        configure(output);
        return output;
    }
}
