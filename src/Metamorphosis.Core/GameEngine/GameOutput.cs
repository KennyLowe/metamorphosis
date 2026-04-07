using Metamorphosis.Core.Models;

namespace Metamorphosis.Core.GameEngine;

public class GameOutput
{
    public List<List<TextSegment>> Lines { get; set; } = new();
    public string? AsciiArt { get; set; }
    public string? StatusText { get; set; }
    public InventoryState? Inventory { get; set; }
    public string? StrengthMana { get; set; }
    public string? AudioTrack { get; set; }
    public bool IsEntryMenu { get; set; }
    public bool IsDeathMenu { get; set; }
    public bool ShouldClearOutput { get; set; }

    public void AddLine(string text)
    {
        Lines.Add(TextFormatter.Parse(text));
    }

    public void AddLines(params string[] texts)
    {
        foreach (var text in texts)
            AddLine(text);
    }
}

public class InventoryState
{
    public List<string> Worn { get; set; } = new();
    public List<string> Carried { get; set; } = new();
    public List<string> Wielded { get; set; } = new();
}
