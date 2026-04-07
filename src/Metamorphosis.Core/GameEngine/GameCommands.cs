using Metamorphosis.Core.Data;
using Metamorphosis.Core.Models;

namespace Metamorphosis.Core.GameEngine;

/// <summary>
/// Handles all game commands from the original C MUD engine (meta.c).
/// Every function here corresponds to a meta_* function in the original code.
/// </summary>
public class GameCommands
{
    private readonly IGameDataStore _store;

    public GameCommands(IGameDataStore store)
    {
        _store = store;
    }

    // ========== DIGGING (meta_dig) ==========

    public GameOutput Dig()
    {
        var player = _store.Player;

        if (player.CurrentRoomId == "room75")
        {
            SpawnItemFromLimbo("toadstool", player.CurrentRoomId);
            return Output(o => o.AddLine("You &+Cuproot# a large &+Mpurple# &+Ctoadstool#."));
        }

        if (player.CurrentRoomId == "room1")
        {
            SpawnItemFromLimbo("vial_snake", player.CurrentRoomId);
            return Output(o => o.AddLine("Rooting around in the &+ydirt# you find a strange &+Cvial#."));
        }

        return Output(o => o.AddLine("You dig around but find nothing of interest."));
    }

    // ========== FLYING (meta_fly) ==========

    public GameOutput Fly()
    {
        var player = _store.Player;
        if (player.CurrentForm != AnimalForm.Bird)
            return Output(o => o.AddLine("You can't fly!"));

        // Room-specific flight paths
        if (player.CurrentRoomId == "room14")
            return Output(o => o.AddLine("Sentry &+Cbirds# beat you back! Only entry by &+Cfoot# is possible."));

        if (player.CurrentRoomId == "room2") // Beside waterfall
        {
            player.CurrentRoomId = "room1";
            _store.UpdatePlayer(player);
            return Output(o => o.AddLine("You spread your wings and soar back to the &+Gclearing#."));
        }

        if (player.CurrentRoomId == "room7")
        {
            player.CurrentRoomId = "room51";
            _store.UpdatePlayer(player);
            return Output(o => o.AddLine("You spread your wings and fly up to the &+Gbranches# above."));
        }

        if (player.CurrentRoomId == "room58")
        {
            player.CurrentRoomId = "room51";
            _store.UpdatePlayer(player);
            return Output(o => o.AddLine("You spread your wings and fly up to the &+Gbranches# above."));
        }

        if (player.CurrentRoomId == "room51")
        {
            player.CurrentRoomId = "room58";
            _store.UpdatePlayer(player);
            return Output(o => o.AddLine("You spread your wings and glide down to the lower &+Gbranches#."));
        }

        return Output(o => o.AddLine("You spread your wings and soar briefly into the &+Cair# before landing again."));
    }

    // ========== CLIMBING (meta_climb) ==========

    public GameOutput Climb(string? arg)
    {
        var player = _store.Player;

        return player.CurrentForm switch
        {
            AnimalForm.Snake => Output(o => o.AddLine("Sentry &+Cbirds# swoop down and beat you back!")),
            AnimalForm.Fish => Output(o => o.AddLine("You can't climb as a &+Bfish#!")),
            AnimalForm.Beetle => Output(o =>
            {
                o.AddLine("A large &+Cbird# swoops down, snatches you up in its beak and devours you whole.");
                o.AddLine("&+REaten alive!#");
                KillPlayer(o);
            }),
            AnimalForm.Bird => Output(o => o.AddLine("You don't need to climb, you can &+Cfly#!")),
            _ => Output(o => o.AddLine("The circumference is too great to get a handhold."))
        };
    }

    // ========== ENTERING (meta_enter) ==========

    public GameOutput Enter(string? arg)
    {
        if (arg == null)
            return Output(o => o.AddLine("Enter what?"));

        var player = _store.Player;
        int rloc = _store.GetRoomUid(player.CurrentRoomId);

        foreach (var obj in _store.GetObjectsByName(arg, rloc, PlayerState.PlayerUid))
        {
            if (obj.ZName == "hole_beetle" && player.CurrentForm == AnimalForm.Beetle)
            {
                player.CurrentRoomId = "room60";
                _store.UpdatePlayer(player);
                return Output(o => o.AddLine("You scuttle &+Gdown# the &+Chole#."));
            }

            if (obj.ZName == "hole_beetle_up" && player.CurrentForm == AnimalForm.Beetle)
            {
                player.CurrentRoomId = "room59";
                _store.UpdatePlayer(player);
                return Output(o => o.AddLine("You scuttle &+Gup# the &+Chole#."));
            }

            if (obj.Name == "hole" || obj.ZName == "hole")
            {
                if (player.CurrentForm == AnimalForm.Snake)
                {
                    player.CurrentRoomId = "room35";
                    _store.UpdatePlayer(player);
                    return Output(o => o.AddLine("You &+Gslither# down the hole."));
                }
                if (player.CurrentForm == AnimalForm.Beetle)
                {
                    player.CurrentRoomId = "room35";
                    _store.UpdatePlayer(player);
                    return Output(o => o.AddLine("You &+Cscuttle# down the hole."));
                }
                return Output(o => o.AddLine("You're too big to fit in there."));
            }

            if (obj.ZName == "hole_up")
            {
                if (player.CurrentForm == AnimalForm.Snake)
                {
                    player.CurrentRoomId = "room1";
                    _store.UpdatePlayer(player);
                    return Output(o => o.AddLine("You &+Gslither# up the hole."));
                }
                if (player.CurrentForm == AnimalForm.Beetle)
                {
                    player.CurrentRoomId = "room1";
                    _store.UpdatePlayer(player);
                    return Output(o => o.AddLine("You &+Cscuttle# up the hole."));
                }
                return Output(o => o.AddLine("You're too big to fit in there."));
            }

            if (obj.ZName == "hole_herring")
            {
                return Output(o =>
                {
                    o.AddLine("Terror takes a firm grip on your heart as you enter the darkness...");
                    o.AddLine("A dark force beyond comprehension overwhelms you.");
                    KillPlayer(o);
                });
            }
        }

        return Output(o => o.AddLine("You can't enter that."));
    }

    // ========== NPC TRADING (meta_give) ==========

    public GameOutput Give(string? arg)
    {
        if (arg == null || !arg.Contains(" to "))
            return Output(o => o.AddLine("Give what to whom? (e.g. 'give skin to shopkeeper')"));

        var parts = arg.Split(" to ", 2);
        var itemName = parts[0].Trim();
        var targetName = parts[1].Trim();

        var player = _store.Player;
        int rloc = _store.GetRoomUid(player.CurrentRoomId);

        // Find the item in inventory
        GameObject? item = null;
        foreach (var obj in _store.GetPlayerInventory(PlayerState.PlayerUid))
        {
            if (obj.Name == itemName || obj.ZName == itemName)
            {
                item = obj;
                break;
            }
        }
        if (item == null)
            return Output(o => o.AddLine("You aren't carrying that."));

        // Find the NPC
        Mobile? target = null;
        foreach (var mob in _store.GetMobilesAtLocation(rloc))
        {
            if (mob.Id == targetName || mob.AltName == targetName ||
                mob.PName.ToLower().Contains(targetName))
            {
                target = mob;
                break;
            }
        }
        if (target == null)
            return Output(o => o.AddLine("They're not here."));

        // Shopkeeper + Skin = Orb
        if (target.Id == "shopkeeper" && item.ZName == "skin")
        {
            return Output(o =>
            {
                o.AddLine("The &+CShopkeeper# takes the skin and examines it carefully.");
                o.AddLine("'&+MAh, a fine rabbit skin! Here, take this orb as payment.#'");
                SendToLimbo(item);
                SpawnItemToPlayer("orb");
                o.Inventory = BuildInventory();
            });
        }

        // Judge + Orb = Silver Feather
        if (target.Id == "judge" && item.ZName == "orb")
        {
            return Output(o =>
            {
                o.AddLine("You present the &+Corb# to the &+CJudge#.");
                o.AddLine("'&+MThis is the shopkeeper's orb! He stole it from the treasury!#'");
                o.AddLine("'&+MAs a reward for returning it, take this silver feather.#'");
                SendToLimbo(item);
                SpawnItemToPlayer("silver_feather");
                o.Inventory = BuildInventory();
            });
        }

        // Snake Beggar + Flask with water = Pebble (snake only)
        if (target.Id == "snake_beggar" && (item.ZName == "flask_empty" || item.Name == "flask"))
        {
            if (player.CurrentForm != AnimalForm.Snake)
                return Output(o => o.AddLine("'&+MDo you really think I'm so desperate as to deal with the likes of you?#'"));

            return Output(o =>
            {
                o.AddLine("The &+Gbeggar# greedily snatches the flask and drinks deeply.");
                o.AddLine("'&+MAhh, water! Here, take this pebble as thanks.#'");
                SendToLimbo(item);
                SpawnItemToPlayer("pebble1");
                o.Inventory = BuildInventory();
            });
        }

        return Output(o => o.AddLine("They don't want that."));
    }

    // ========== COMBAT (meta_kill / meta_wound) ==========

    public GameOutput Kill(string? arg)
    {
        if (arg == null)
            return Output(o => o.AddLine("Kill what?"));

        var player = _store.Player;
        int rloc = _store.GetRoomUid(player.CurrentRoomId);

        foreach (var mob in _store.GetMobilesAtLocation(rloc))
        {
            if (mob.Id == arg || mob.AltName == arg || mob.PName.ToLower().Contains(arg))
            {
                // Chimera — only human with staff can fight
                if (mob.Id == "chimera")
                {
                    if (player.CurrentForm != AnimalForm.Human)
                        return Output(o =>
                        {
                            o.AddLine("The &+CChimera# laughs at you with impunity and crushes you with a single massive hairy paw.");
                            o.AddLine("&+RCrushed by the Chimera!#");
                            KillPlayer(o);
                        });

                    // Check for staff
                    bool hasStaff = _store.GetPlayerInventory(PlayerState.PlayerUid)
                        .Any(obj => obj.ZName == "staff" && obj.Wielded == PlayerState.PlayerUid);
                    if (!hasStaff)
                        return Output(o =>
                        {
                            o.AddLine("The &+CChimera# swats you aside like a fly.");
                            o.AddLine("&+RCrushed by the Chimera!#");
                            KillPlayer(o);
                        });

                    return Output(o =>
                    {
                        o.AddLine("You swing the &+Cstaff# with all your might!");
                        o.AddLine("The &+CChimera# &+Rshrieks# in agony as the staff connects, its dark power withering under the magical blow.");
                        o.AddLine("With a final, devastating strike, the &+CChimera# collapses and dies.");
                        mob.Location = _store.GetRoomUid("room74");
                        _store.UpdateMobile(mob);
                    });
                }

                // Rabbits — snake only
                if (mob.Id == "flopsy" || mob.Id == "mopsy")
                {
                    if (player.CurrentForm != AnimalForm.Snake)
                        return Output(o => o.AddLine("You're not equipped to fight a rabbit in this form."));

                    return Output(o =>
                    {
                        o.AddLine("The " + mob.PName + " sees you coming and bolts in terror!");
                        o.AddLine("All the rabbits scatter and disappear into the undergrowth.");
                        // Remove all rabbits
                        int limbo = _store.GetRoomUid("room74");
                        foreach (var m in _store.GetMobilesAtLocation(rloc).ToList())
                        {
                            if (m.Id == "flopsy" || m.Id == "mopsy" || m.Id == "cottontail")
                            {
                                m.Location = limbo;
                                _store.UpdateMobile(m);
                            }
                        }
                    });
                }

                if (mob.Id == "cottontail")
                {
                    if (player.CurrentForm != AnimalForm.Snake)
                        return Output(o => o.AddLine("You're not equipped to fight a rabbit in this form."));

                    int numItems = _store.GetPlayerInventory(PlayerState.PlayerUid).Count();
                    if (numItems > 1)
                        return Output(o => o.AddLine("You cannot &+Rbite# with something in your mouth."));

                    return Output(o =>
                    {
                        o.AddLine("You lunge forward and bite down hard on the rabbit's neck, biting through the spine.");
                        o.AddLine("The " + mob.PName + " collapses, lifeless.");
                        mob.Location = _store.GetRoomUid("room74");
                        _store.UpdateMobile(mob);
                        // Spawn cottontail corpse
                        SpawnItemFromLimbo("cottontail", player.CurrentRoomId);
                    });
                }

                // Snake beggar — drops pebbles when attacked
                if (mob.Id == "snake_beggar")
                {
                    if (player.CurrentForm == AnimalForm.Beetle || player.CurrentForm == AnimalForm.Fish)
                        return Output(o => o.AddLine("You are far too puny to attack that."));

                    return Output(o =>
                    {
                        o.AddLine("You attack the &+Gbeggar#! He drops a pebble and hobbles away in fright.");
                        SpawnItemFromLimbo("pebble1", player.CurrentRoomId);
                    });
                }

                // Generic combat
                if (player.CurrentForm != AnimalForm.Human)
                    return Output(o => o.AddLine("You're too puny in this form to fight anything."));

                return Output(o =>
                {
                    o.AddLine("You attack the " + mob.PName + "!");
                    o.AddLine("After a fierce battle, the " + mob.PName + " is defeated.");
                    mob.Location = _store.GetRoomUid("room74");
                    _store.UpdateMobile(mob);
                });
            }
        }

        return Output(o => o.AddLine("There's nothing here to fight."));
    }

    // ========== SNAKE BITING (meta_bite) ==========

    public GameOutput Bite(string? arg)
    {
        var player = _store.Player;
        if (player.CurrentForm != AnimalForm.Snake)
            return Output(o => o.AddLine("You don't have fangs to bite with!"));

        if (arg == null)
            return Output(o => o.AddLine("Bite what?"));

        int numItems = _store.GetPlayerInventory(PlayerState.PlayerUid).Count();
        if (numItems > 1)
            return Output(o => o.AddLine("You cannot &+Rbite# with something in your mouth."));

        // Delegate to Kill for rabbits
        return Kill(arg);
    }

    // ========== BIRD PECKING (meta_peck) ==========

    public GameOutput Peck(string? arg, RoomEvents events)
    {
        var player = _store.Player;
        if (player.CurrentForm != AnimalForm.Bird)
            return Output(o => o.AddLine("You don't have a beak to peck with!"));

        if (arg == null)
            return Output(o => o.AddLine("Peck what?"));

        // Pecking the tree triggers bird arrest
        if (arg == "tree" || arg == "sap")
        {
            return Output(o =>
            {
                o.AddLine("You peck at the &+gtree#, causing sap to ooze from the wound.");
                o.AddLine("Suddenly, a flock of angry &+Cbirds# swoops down upon you!");
                o.AddLine("'&+MYou dare damage our sacred tree?! Seize this criminal!#'");
                o.AddLine("You are dragged off to a &+Ccell# and thrown inside.");
                player.CurrentRoomId = "room52";
                _store.UpdatePlayer(player);
                events.StartJail();
            });
        }

        return Output(o => o.AddLine("You peck at it but nothing happens."));
    }

    // ========== BIRD SKINNING (meta_skin) ==========

    public GameOutput Skin(string? arg)
    {
        var player = _store.Player;
        if (player.CurrentForm != AnimalForm.Bird)
            return Output(o => o.AddLine("You are not equipped for skinning."));

        if (arg == null)
            return Output(o => o.AddLine("Skin what?"));

        int rloc = _store.GetRoomUid(player.CurrentRoomId);
        foreach (var obj in _store.GetObjectsByName(arg, rloc, PlayerState.PlayerUid))
        {
            if (obj.ZName == "cottontail")
            {
                SendToLimbo(obj);
                SpawnItemFromLimbo("skin", player.CurrentRoomId);
                return Output(o =>
                {
                    o.AddLine("You use your sharp beak to expertly skin the rabbit carcass.");
                    o.AddLine("A fine &+Crabbit skin# lies on the ground.");
                });
            }
        }

        return Output(o => o.AddLine("There's nothing here to skin."));
    }

    // ========== TICKLING (meta_tickle) ==========

    public GameOutput Tickle()
    {
        var player = _store.Player;
        int rloc = _store.GetRoomUid(player.CurrentRoomId);

        foreach (var mob in _store.GetMobilesAtLocation(rloc))
        {
            if (mob.Id == "snake_beggar")
            {
                if (player.CurrentForm == AnimalForm.Beetle)
                    return Output(o =>
                    {
                        o.AddLine("You tickle the &+Gsnake# with your antennae. He drops a pebble on you, crushing you instantly.");
                        o.AddLine("&+RCrushed by a juggling snake!#");
                        KillPlayer(o);
                    });

                if (player.CurrentForm == AnimalForm.Bird)
                {
                    return Output(o =>
                    {
                        o.AddLine("You tickle the &+Gsnake# with your feathers. He writhes with laughter,");
                        o.AddLine("dropping a &+Cpebble# in the process!");
                        SpawnItemFromLimbo("pebble1", player.CurrentRoomId);
                    });
                }

                return Output(o => o.AddLine("You're not really equipped to tickle effectively."));
            }
        }

        return Output(o => o.AddLine("Tee hee!"));
    }

    // ========== OBJECT INTERACTION: OPEN/CLOSE/TURN (meta_open/close/turn) ==========

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

                var room = _store.GetRoomById(player.CurrentRoomId);
                if (room != null && room.IsUnderwater && obj.ZName == "chest")
                    return Output(o => o.AddLine("You open the &+Cchest#. Air escapes in a rush of &+Bbubbles#."));

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

    public GameOutput Turn(string? arg)
    {
        if (arg == null)
            return Output(o => o.AddLine("Turn what?"));

        var player = _store.Player;
        int rloc = _store.GetRoomUid(player.CurrentRoomId);

        foreach (var obj in _store.GetObjectsByName(arg, rloc, PlayerState.PlayerUid))
        {
            if (obj.ZName == "chest")
            {
                var room = _store.GetRoomById(player.CurrentRoomId);
                if (room != null && room.IsUnderwater)
                    return Output(o => o.AddLine("You turn the &+Cchest# upside down, trapping &+Bair# inside."));
                return Output(o => o.AddLine("You turn the &+Cchest# upside down."));
            }
        }
        return Output(o => o.AddLine("You can't turn that."));
    }

    // ========== PULLING (meta_pull) — drawbridge mechanism ==========

    public GameOutput Pull(string? arg)
    {
        if (arg == null)
            return Output(o => o.AddLine("Pull what?"));

        var player = _store.Player;
        int rloc = _store.GetRoomUid(player.CurrentRoomId);

        foreach (var obj in _store.GetObjectsByName(arg, rloc, PlayerState.PlayerUid))
        {
            if (obj.ZName == "branch_mechanism" || obj.ZName == "stone_branch")
            {
                return Output(o =>
                {
                    o.AddLine("You pull the lever with all your might.");
                    o.AddLine("With a great &+Ycreaking# sound, the &+Cdrawbridge# lowers into place.");
                });
            }
        }
        return Output(o => o.AddLine("Nothing happens."));
    }

    public GameOutput Push(string? arg)
    {
        if (arg == null)
            return Output(o => o.AddLine("Push what?"));
        return Output(o => o.AddLine("Nothing happens."));
    }

    // ========== CUTTING (meta_cut) — rope/drawbridge ==========

    public GameOutput Cut(string? arg)
    {
        if (arg == null)
            return Output(o => o.AddLine("Cut what?"));

        var player = _store.Player;

        if (arg == "rope" || arg == "ropes")
        {
            if (player.CurrentForm == AnimalForm.Bird)
                return Output(o =>
                {
                    o.AddLine("You use your sharp beak to cut through the &+Cropes#.");
                    o.AddLine("The &+Cdrawbridge# comes crashing down with a tremendous &+Ycrash#!");
                });

            if (player.CurrentForm == AnimalForm.Snake)
                return Output(o =>
                {
                    o.AddLine("You use your sharp teeth to gnaw through the &+Cropes#.");
                    o.AddLine("The &+Cdrawbridge# comes crashing down with a tremendous &+Ycrash#!");
                });

            return Output(o => o.AddLine("You don't have the tools to cut that."));
        }

        return Output(o => o.AddLine("You can't cut that."));
    }

    // ========== FILLING (meta_fill) ==========

    public GameOutput Fill(string? arg)
    {
        if (arg == null)
            return Output(o => o.AddLine("Fill what?"));

        var player = _store.Player;
        var room = _store.GetRoomById(player.CurrentRoomId);

        // Check if at a water source
        bool atWater = room != null && (room.IsUnderwater ||
            player.CurrentRoomId == "room2" || player.CurrentRoomId == "room5" ||
            player.CurrentRoomId == "room8");

        if (!atWater)
            return Output(o => o.AddLine("There's nothing here to fill it from."));

        foreach (var obj in _store.GetPlayerInventory(PlayerState.PlayerUid))
        {
            if (obj.Name == arg || obj.ZName == arg)
            {
                if (obj.ZName == "bottle" || obj.ZName == "flask_empty")
                {
                    return Output(o =>
                    {
                        o.AddLine("You fill the " + obj.Name + " with &+Bwater#.");
                    });
                }
            }
        }

        return Output(o => o.AddLine("You can't fill that."));
    }

    // ========== POURING (meta_pour) ==========

    public GameOutput Pour(string? arg)
    {
        if (arg == null)
            return Output(o => o.AddLine("Pour what?"));

        return Output(o => o.AddLine("You pour it out. The liquid seeps into the earth."));
    }

    // ========== LIGHTING (meta_light) ==========

    public GameOutput Light(string? arg, RoomEvents events)
    {
        if (arg == null)
            return Output(o => o.AddLine("Light what?"));

        var player = _store.Player;
        int rloc = _store.GetRoomUid(player.CurrentRoomId);

        foreach (var obj in _store.GetObjectsByName(arg, rloc, PlayerState.PlayerUid))
        {
            if (obj.ZName == "gunpowder")
            {
                return Output(o =>
                {
                    o.AddLine("The &+Ygunpowder# ignites with a tremendous &+REXPLOSION#!");
                    o.AddLine("&+RCompletely blown away!#");
                    KillPlayer(o);
                });
            }

            if (obj.ZName == "bomb")
            {
                events.StartBomb();
                return Output(o => o.AddLine("You light the fuse on the &+Rbomb#. It begins to &+Yhiss# ominously..."));
            }

            if (obj.ZName == "kindling")
            {
                return Output(o => o.AddLine("You light the &+Ykindling# beneath the &+Ccauldron#. Flames lick upward."));
            }
        }

        return Output(o => o.AddLine("You don't have anything to light it with."));
    }

    // ========== SPEAKING (meta_say) ==========

    public GameOutput Say(string? arg)
    {
        if (arg == null)
            return Output(o => o.AddLine("Say what?"));

        var player = _store.Player;

        // Judge's court — guilty/innocent plea
        if (player.CurrentRoomId == "room53")
        {
            var lower = arg.ToLower();
            if (lower.Contains("guilty"))
            {
                return Output(o =>
                {
                    o.AddLine("You say '&+Mguilty#'.");
                    o.AddLine("The &+CJudge# nods solemnly. '&+MThe court finds you guilty as charged.");
                    o.AddLine("You are hereby sentenced to life imprisonment.#'");
                    o.AddLine("Guards drag you away to the dungeons.");
                    // Move to jail
                    player.CurrentRoomId = "room52";
                    _store.UpdatePlayer(player);
                });
            }

            if (lower.Contains("innocent"))
            {
                return Output(o =>
                {
                    o.AddLine("You say '&+Minnocent#'.");
                    o.AddLine("The &+CJudge# raises an eyebrow. '&+MLying in my court?");
                    o.AddLine("The court finds you guilty. You are sentenced to life imprisonment.#'");
                    player.CurrentRoomId = "room52";
                    _store.UpdatePlayer(player);
                });
            }

            return Output(o =>
            {
                o.AddLine("You say '" + arg + "'.");
                o.AddLine("The bailiff barks, '&+MInnocent or Guilty? Answer the question!#'");
            });
        }

        return Output(o => o.AddLine("You say '" + arg + "'."));
    }

    // ========== WEARING ARMOUR (meta_wear) ==========

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

    // ========== SMELLING (meta_smell) ==========

    public GameOutput Smell()
    {
        var player = _store.Player;

        if (player.CurrentForm == AnimalForm.Beetle && player.CurrentRoomId == "room59")
            return Output(o => o.AddLine("The smell of &+ydung# fills the air, but to your beetle senses it is like sweet &+Mnectar#."));

        if (player.CurrentForm == AnimalForm.Human && player.CurrentRoomId == "room59")
            return Output(o => o.AddLine("The smell is absolutely &+Rintolerable#. It is like being trapped in a cattle stall."));

        if (player.CurrentRoomId == "room60")
            return Output(o => o.AddLine("You detect lingering traces of &+ydung# to the &+Geast# and &+Gwest#."));

        if (player.CurrentForm == AnimalForm.Beetle)
            return Output(o => o.AddLine("Your antennae twitch as you sense the air around you."));

        return Output(o => o.AddLine("You sniff the air but detect nothing unusual."));
    }

    // ========== KICKING (meta_kick) ==========

    public GameOutput Kick(string? arg)
    {
        var player = _store.Player;
        if (player.CurrentForm == AnimalForm.Snake)
            return Output(o => o.AddLine("You don't have any legs!"));
        return Output(o => o.AddLine("You kick around but accomplish nothing."));
    }

    // ========== EMPTYING (meta_empty) ==========

    public GameOutput Empty(string? arg)
    {
        if (arg == null)
            return Output(o => o.AddLine("Empty what?"));

        var player = _store.Player;

        if (player.CurrentForm == AnimalForm.Bird && (arg == "gun" || arg == "cannon"))
        {
            SpawnItemFromLimbo("gunpowder", player.CurrentRoomId);
            return Output(o => o.AddLine("You fly up and tip out the &+Ygunpowder# from the gun."));
        }

        if (player.CurrentForm == AnimalForm.Bird && arg == "vines")
        {
            SpawnItemFromLimbo("vine", player.CurrentRoomId);
            return Output(o => o.AddLine("You fly up and pull a &+Gvine# free from the tangle."));
        }

        if (player.CurrentForm != AnimalForm.Bird && (arg == "gun" || arg == "vines" || arg == "cannon"))
            return Output(o => o.AddLine("It's too far out of reach."));

        return Output(o => o.AddLine("You empty it out."));
    }

    // ========== FORM RESTRICTION CHECKS ==========

    public string? CheckWieldRestriction()
    {
        var player = _store.Player;
        return player.CurrentForm switch
        {
            AnimalForm.Snake => "You don't have any hands!",
            AnimalForm.Fish => "You don't have any hands!",
            AnimalForm.Bird => "You can't grasp a weapon in your wings.",
            AnimalForm.Beetle => "Your legs are too small to wield anything.",
            _ => null
        };
    }

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

    public string? CheckGetSpecialObject(string objZName)
    {
        return objZName switch
        {
            "blue_flame" => "It is paradoxically too &+Ccold# to touch.",
            "red_flame" => "It is too &+Rhot# to touch.",
            "water" or "water_2" or "water_3" or "water_4" => "The water slips through your fingers.",
            "dung" => "Guards step in front of you, blocking your way to the dung.",
            _ => null
        };
    }

    public string? CheckGetBirdOnly(string objZName)
    {
        var player = _store.Player;
        if ((objZName == "vine" || objZName == "gunpowder") && player.CurrentForm != AnimalForm.Bird)
            return "It's too far out of reach.";
        return null;
    }

    // ========== HELPER METHODS ==========

    private void KillPlayer(GameOutput output)
    {
        var player = _store.Player;
        int limboUid = _store.GetRoomUid("room74");
        foreach (var obj in _store.GetPlayerInventory(PlayerState.PlayerUid).ToList())
        {
            obj.Carried = 0;
            obj.Wielded = 0;
            obj.Worn = 0;
            obj.Location = limboUid;
            _store.UpdateObject(obj);
        }
        player.IsDead = true;
        _store.UpdatePlayer(player);
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

    private void SpawnItemFromLimbo(string zname, string roomId)
    {
        int rloc = _store.GetRoomUid(roomId);
        int limbo = _store.GetRoomUid("room74");
        // Check limbo first, then search everywhere
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

    private void SpawnItemToPlayer(string zname)
    {
        int limbo = _store.GetRoomUid("room74");
        foreach (var obj in _store.GetObjectsAtLocation(limbo))
        {
            if (obj.ZName == zname)
            {
                obj.Location = PlayerState.PlayerUid;
                obj.Carried = PlayerState.PlayerUid;
                _store.UpdateObject(obj);
                return;
            }
        }
    }

    private void SendToLimbo(GameObject obj)
    {
        obj.Location = _store.GetRoomUid("room74");
        obj.Carried = 0;
        obj.Wielded = 0;
        obj.Worn = 0;
        _store.UpdateObject(obj);
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
