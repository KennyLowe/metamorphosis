using Metamorphosis.Core.Data;
using Metamorphosis.Core.Models;

namespace Metamorphosis.Core.GameEngine;

/// <summary>
/// Handles all animal form transformations from the original meta_eat() function.
/// Each vial transforms the player between forms with form-specific messages.
/// Equipment is dropped on transformation; form-specific weapons are equipped.
/// </summary>
public class TransformationSystem
{
    private readonly IGameDataStore _store;
    private readonly RoomEvents _events;

    public TransformationSystem(IGameDataStore store, RoomEvents events)
    {
        _store = store;
        _events = events;
    }

    public GameOutput Eat(string? arg)
    {
        if (arg == null)
            return Output(o => o.AddLine("Eat what?"));

        var player = _store.Player;
        int rloc = _store.GetRoomUid(player.CurrentRoomId);

        foreach (var obj in _store.GetObjectsByName(arg, rloc, PlayerState.PlayerUid))
        {
            // Toadstool — teleport to toadstool ring
            if (obj.ZName == "toadstool" && player.CurrentRoomId == "room75")
                return EatToadstool(player, obj);

            // Snake vial
            if (obj.ZName == "vial_snake")
                return DrinkVial(player, AnimalForm.Snake, obj);

            // Fish vial
            if (obj.ZName == "vial_fish")
                return DrinkVial(player, AnimalForm.Fish, obj);

            // Beetle vial
            if (obj.ZName == "vial_beetle")
                return DrinkVial(player, AnimalForm.Beetle, obj);

            // Bird vial
            if (obj.ZName == "vial_bird")
                return DrinkVial(player, AnimalForm.Bird, obj);

            // Human liquid — transform back to human
            if (obj.ZName == "liquid_human")
                return DrinkHumanPotion(player, obj);

            // Healing potion
            if (obj.Name == "potion" || obj.ZName == "potion")
                return Output(o => o.AddLine("You drink the healing potion. You feel revitalised!"));

            return Output(o => o.AddLine("You can't eat that."));
        }

        return Output(o => o.AddLine("You don't have that."));
    }

    private GameOutput EatToadstool(PlayerState player, GameObject toadstool)
    {
        return Output(o =>
        {
            o.AddLine("As you eat the &+Ctoadstool#, a strange feeling overpowers you and you black out.");
            o.AddLine("");
            o.AddLine("You awaken in a &+Gclearing# identical to the one you were in, but the way you");
            o.AddLine("entered is now gone, and a new exit is revealed to the &+Gnorth#, a &+Ccauldron#");
            o.AddLine("now dominating the centre of the ring.");

            if (HasInventory())
            {
                o.AddLine("Looking down, you see your equipment is gone.");
                DumpPlayerItems(player.CurrentRoomId);
            }

            player.CurrentRoomId = "room72";
            _store.UpdatePlayer(player);

            toadstool.Location = _store.GetRoomUid("room74"); // limbo
            _store.UpdateObject(toadstool);
        });
    }

    private GameOutput DrinkVial(PlayerState player, AnimalForm targetForm, GameObject vial)
    {
        // Already that form?
        if (player.CurrentForm == targetForm)
            return Output(o => o.AddLine("You are already a &+C" + targetForm.ToFormString() + "#!"));

        return Output(o =>
        {
            var currentForm = player.CurrentForm;

            // Generate transformation message based on current -> target
            o.AddLine(GetTransformMessage(currentForm, targetForm));

            // Special death cases
            if (targetForm == AnimalForm.Bird && IsUnderwater(player))
            {
                o.AddLine("You cough and splutter, unable to breathe and die quickly and painfully.");
                o.AddLine("&+RForgot how to breathe!#");
                o.IsDeathMenu = true;
                player.IsDead = true;
                _store.UpdatePlayer(player);
                return;
            }

            if (targetForm == AnimalForm.Human && IsUnderwater(player))
            {
                o.AddLine("Unable to breathe underwater, you drown almost immediately.");
                o.AddLine("&+RDrowned!#");
                o.IsDeathMenu = true;
                player.IsDead = true;
                _store.UpdatePlayer(player);
                return;
            }

            // Drop all equipment
            DumpPlayerItems(player.CurrentRoomId);

            // Set new form
            player.CurrentForm = targetForm;
            _store.UpdatePlayer(player);

            // Fish on land starts suffocation timer
            if (targetForm == AnimalForm.Fish)
            {
                var room = _store.GetRoomById(player.CurrentRoomId);
                if (room != null && !room.IsUnderwater)
                {
                    o.AddLine("You begin to &+Csplutter#, gasping for &+Bair#, you need to find &+Cwater# soon, or you will surely die!");
                }
            }

            // Update UI
            o.StatusText = targetForm.ToFormString();
            o.AsciiArt = AsciiArt.GetArt(targetForm);
            o.StrengthMana = targetForm == AnimalForm.Beetle ? "S:5\\5\nM:0\\0" : "S:999\\999 M:999\\999";
            o.Inventory = BuildInventory();

            // Trigger room entry events for new form
            _events.OnRoomEntry(player.CurrentRoomId, o);
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

            // Special: room72 toadstool ring — fairy gives staff
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
                GivePlayerItemByZName("staff");
            }

            player.CurrentForm = AnimalForm.Human;
            _store.UpdatePlayer(player);
            o.StatusText = "human";
            o.AsciiArt = AsciiArt.GetArt(AnimalForm.Human);
            o.StrengthMana = "S:999\\999 M:999\\999";
            o.Inventory = BuildInventory();
        });
    }

    private string GetTransformMessage(AnimalForm from, AnimalForm to)
    {
        return (from, to) switch
        {
            // To Snake
            (AnimalForm.Human, AnimalForm.Snake) =>
                "As you consume the &+Ggreen# liquid, your &+Cbody# begins to &+Mshrink#.\n" +
                "Your arms &+Msubsume# into your &+Cbody#, and your &+Clegs# &+Mmerge# into one\n" +
                "limb which rapidly becomes more a part of your &+Ctorso# than a separate\n" +
                "limb. Within seconds, your &+Cbody# has changed into that of a &+Gsnake#.",
            (AnimalForm.Bird, AnimalForm.Snake) =>
                "As you consume the &+Ggreen# liquid, your &+Cbody# begins to grow.\n" +
                "Your &+Cwings# &+Msubsume# into your &+Cbody#, and your &+Clegs# &+Mmerge# into one\n" +
                "limb which rapidly becomes more a part of your &+Ctorso# than a separate\n" +
                "limb. Within seconds, your &+Cbody# has changed into that of a &+Gsnake#.",
            (AnimalForm.Beetle, AnimalForm.Snake) =>
                "As you consume the &+Ggreen# liquid, your &+Cbody# begins to grow.\n" +
                "Your &+Ccarapace# softens and &+Melongates#, and your &+Clegs# &+Mmerge# into one\n" +
                "limb which rapidly becomes more a part of your &+Ctorso# than a separate\n" +
                "limb. Within seconds, your &+Cbody# has changed into that of a &+Gsnake#.",
            (AnimalForm.Fish, AnimalForm.Snake) =>
                "As you consume the &+Ggreen# liquid, your &+Cbody# begins to narrow.\n" +
                "Your &+Cfins# &+Melongate#, and quickly &+Mmerge# into one long, &+Gscaly#\n" +
                "limb which rapidly becomes more a part of your &+Ctorso# than a separate\n" +
                "limb. Within seconds, your &+Cbody# has changed into that of a &+Gsnake#.",

            // To Fish
            (AnimalForm.Human, AnimalForm.Fish) =>
                "As you consume the &+Mvile# &+Cliquid#, your &+Cbody# begins to &+Mshrink#.\n" +
                "Your &+Carms# &+Msubsume# into your &+Cbody#, and your &+Clegs# &+Mmerge# into a\n" +
                "large &+Cfin#. Other &+Cfins# &+Msprout# from your &+Cbody# and within seconds\n" +
                "your &+Cbody# is that of a &+Bfish#.",
            (AnimalForm.Bird, AnimalForm.Fish) =>
                "As you consume the &+Mvile# &+Cliquid#, your &+Cbody# begins to &+Mgrow#.\n" +
                "Your &+Cwings# &+Msubsume# into your &+Cbody#, and your &+Clegs# &+Mmerge# into a\n" +
                "&+Cfin#. More &+Cfins# &+Msprout# from your &+Cbody#, and within seconds you have\n" +
                "become a &+Bfish#!",
            (AnimalForm.Beetle, AnimalForm.Fish) =>
                "As you consume the &+Mvile# &+Cliquid#, your &+Cbody# begins to &+Mgrow#.\n" +
                "Your &+Ccarapace# softens and &+Melongates#, and your &+Clegs# &+Mmerge# into one\n" +
                "&+Cfin#. More &+Cfins# &+Msprout# from your body, and in seconds you have\n" +
                "become a &+Bfish#!",
            (AnimalForm.Snake, AnimalForm.Fish) =>
                "As you consume the vile liquid, your &+Cbody# begins to &+Mshrink# and fatten.\n" +
                "Your &+Cfins# &+Msprout# from various &+Cbody# parts, and within seconds you\n" +
                "have become a fish!",

            // To Beetle
            (AnimalForm.Human, AnimalForm.Beetle) =>
                "You take a sip from the &+Gvial# and feel your &+Cbody# &+Yhardening#. With a\n" +
                "rush of air, you &+Mshrink# greatly in size and when you open your eyes\n" +
                "you find that you are a &+Rbeetle#.",
            (AnimalForm.Bird, AnimalForm.Beetle) =>
                "You take a sip from the &+Gvial# and feel your &+Cbody# &+Mshrink#. You close your\n" +
                "eyes and flap your &+Cwings# wildly, trying to rid yourself of the feeling\n" +
                "only to discover that your &+Cwings# are now spindly &+Clegs#. You open your eyes\n" +
                "to find that you are a &+Rbeetle#.",
            (AnimalForm.Snake, AnimalForm.Beetle) =>
                "You take a sip from the &+Gvial# and feel a wave of &+Yenergy# pass through you,\n" +
                "limbs &+Msprout# from your rapidly &+Mshrinking# &+Cbody#, and an armoured shell &+Msprouts#\n" +
                "from your back, shielding you from harm. Within seconds you have transformed\n" +
                "into a &+Rbeetle#.",
            (AnimalForm.Fish, AnimalForm.Beetle) =>
                "You take a sip of the &+Gvial# and feel your &+Cfins# retract into your body which\n" +
                "starts to &+Mshrink# and harden. You close your eyes and try and banish the strange\n" +
                "sensation, when abruptly it stops. You open your eyes to find that you are a\n" +
                "&+Rbeetle#.",

            // To Bird
            (AnimalForm.Human, AnimalForm.Bird) =>
                "You take a sip from the &+Gvial# and feel a sudden wave of &+Yenergy# pass through you.\n" +
                "Your arms &+Mshrink# and widen and &+Cfeathers# &+Msprout# all over your &+Cbody#. Within\n" +
                "moments you have transformed into a &+Gbird#.",
            (AnimalForm.Snake, AnimalForm.Bird) =>
                "You take a sip from the &+Gvial# and feel a sudden wave of &+Yenergy# pass through you.\n" +
                "Your &+Cwings# &+Msprout# from your arms and you find yourself suddenly covered with\n" +
                "&+Cfeathers#. Within moments you have transformed into a &+Gbird#.",
            (AnimalForm.Beetle, AnimalForm.Bird) =>
                "You take a sip from the &+Gvial# and feel a sudden wave of &+Yenergy# pass through you.\n" +
                "Your &+Clegs# retract into your &+Cbody# which &+Mgrows# rapidly, &+Cfeathers# &+Msprouting# all\n" +
                "over. &+CWings# grow from your &+Ctorso#, and a beak appears on your face. Within\n" +
                "seconds you have transformed into a &+Gbird#.",
            (AnimalForm.Fish, AnimalForm.Bird) =>
                "You take a sip from the &+Gvial# and feel a sudden wave of &+Yenergy# pass through you.\n" +
                "Your &+Cfins# grow and widen and &+Cfeathers# &+Msprout# all over your &+Cbody#. Within\n" +
                "moments you have transformed into a &+Gbird#.",

            // Default fallback
            _ => "A strange sensation passes through you as your body transforms."
        };
    }

    private bool IsUnderwater(PlayerState player)
    {
        var room = _store.GetRoomById(player.CurrentRoomId);
        return room != null && room.IsUnderwater;
    }

    private bool HasInventory()
    {
        return _store.GetPlayerInventory(PlayerState.PlayerUid).Any();
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

    private void GivePlayerItemByZName(string zname)
    {
        // Search all rooms for the item
        foreach (var room in _store.GetAllRooms())
        {
            foreach (var obj in _store.GetObjectsAtLocation(room.Uid))
            {
                if (obj.ZName == zname || obj.Name == zname)
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
