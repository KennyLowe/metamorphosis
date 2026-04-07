namespace Metamorphosis.Core.Models;

public class GameObject
{
    public int Uid { get; set; }
    public string Id { get; set; } = "";
    public string ZName { get; set; } = "";
    public int Location { get; set; }
    public string Name { get; set; } = "";
    public string Description { get; set; } = "";
    public string Examine { get; set; } = "";
    public bool NoGet { get; set; }
    public int Carried { get; set; }
    public int Wielded { get; set; }
    public int Worn { get; set; }
    public bool IsContainer { get; set; }
    public int InContainer { get; set; }
    public int State { get; set; }
    public bool IsDoor { get; set; }
    public int Linked { get; set; }
    public bool IsWeapon { get; set; }
    public bool IsWearable { get; set; }

    public GameObject Clone()
    {
        return (GameObject)MemberwiseClone();
    }
}
