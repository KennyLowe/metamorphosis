namespace Metamorphosis.Core.Models;

public class PlayerState
{
    public const int PlayerUid = 1;
    public const string StartingRoom = "room85";

    public string CurrentRoomId { get; set; } = StartingRoom;
    public AnimalForm CurrentForm { get; set; } = AnimalForm.Human;
    public bool IsDead { get; set; }
    public bool IsInEntryMenu { get; set; } = true;
}
