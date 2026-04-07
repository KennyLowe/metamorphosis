namespace Metamorphosis.Core.Models;

public class Room
{
    public int Uid { get; set; }
    public string Id { get; set; } = "";
    public string Name { get; set; } = "";
    public string Description { get; set; } = "";
    public Dictionary<Direction, string> Exits { get; set; } = new();
    public bool IsOutdoors { get; set; }
    public bool NoMobiles { get; set; }
    public bool IsDeath { get; set; }
    public bool IsUnderwater { get; set; }

    public string GetDescriptionForForm(AnimalForm form)
    {
        if (form == AnimalForm.Human)
            return Description;

        var marker = form.GetDescriptionMarker();
        if (string.IsNullOrEmpty(marker) || !Description.Contains($"${marker}"))
            return Description;

        var segments = Description.Split('$');
        foreach (var segment in segments)
        {
            if (segment.StartsWith(marker))
            {
                var desc = segment.Substring(marker.Length);
                // Trim the leading newline if present (from the marker line)
                if (desc.StartsWith("\n"))
                    desc = desc.Substring(1);
                return desc.TrimEnd('\r', '\n');
            }
        }

        return Description;
    }

    public string? GetExit(Direction dir)
    {
        return Exits.TryGetValue(dir, out var roomId) && !string.IsNullOrEmpty(roomId) ? roomId : null;
    }
}
