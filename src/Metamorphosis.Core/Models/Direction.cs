namespace Metamorphosis.Core.Models;

public enum Direction
{
    North,
    South,
    East,
    West,
    Up,
    Down
}

public static class DirectionExtensions
{
    public static string ToShortString(this Direction dir) => dir switch
    {
        Direction.North => "n",
        Direction.South => "s",
        Direction.East => "e",
        Direction.West => "w",
        Direction.Up => "u",
        Direction.Down => "d",
        _ => ""
    };

    public static Direction? FromString(string s) => s.ToLower() switch
    {
        "n" or "north" => Direction.North,
        "s" or "south" => Direction.South,
        "e" or "east" => Direction.East,
        "w" or "west" => Direction.West,
        "u" or "up" => Direction.Up,
        "d" or "down" => Direction.Down,
        _ => null
    };
}
