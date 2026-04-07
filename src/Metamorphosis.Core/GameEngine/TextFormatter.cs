using Metamorphosis.Core.Models;

namespace Metamorphosis.Core.GameEngine;

public static class TextFormatter
{
    private static readonly Dictionary<string, GameColor> ColorMap = new()
    {
        ["C"] = GameColor.Cyan,
        ["G"] = GameColor.Green,
        ["L"] = GameColor.Black,
        ["B"] = GameColor.Blue,
        ["M"] = GameColor.Magenta,
        ["R"] = GameColor.Red,
        ["W"] = GameColor.White,
        ["Y"] = GameColor.Yellow,
        ["y"] = GameColor.Brown,
        ["g"] = GameColor.DarkGreen
    };

    public static List<TextSegment> Parse(string text)
    {
        var segments = new List<TextSegment>();
        if (string.IsNullOrEmpty(text))
            return segments;

        // Split on & and #, same as original bprintf
        var parts = text.Split('&', '#');

        foreach (var part in parts)
        {
            if (string.IsNullOrEmpty(part))
                continue;

            if (part.StartsWith("+") && part.Length >= 2)
            {
                var colorCode = part.Substring(1, 1);
                var content = part.Substring(2);
                if (ColorMap.TryGetValue(colorCode, out var color) && content.Length > 0)
                {
                    segments.Add(new TextSegment { Text = content, Color = color });
                }
                else if (content.Length > 0)
                {
                    segments.Add(new TextSegment { Text = content, Color = GameColor.Default });
                }
            }
            else
            {
                segments.Add(new TextSegment { Text = part, Color = GameColor.Default });
            }
        }

        return segments;
    }
}
