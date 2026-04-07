namespace Metamorphosis.Core.Models;

public class TextSegment
{
    public string Text { get; set; } = "";
    public GameColor Color { get; set; } = GameColor.Default;
}

public enum GameColor
{
    Default,
    Black,
    Blue,
    Cyan,
    Green,
    Magenta,
    Red,
    White,
    Yellow,
    Brown,
    DarkGreen
}
