using Metamorphosis.Core.GameEngine;
using Metamorphosis.Core.Models;

namespace Metamorphosis.Core.Tests;

public class TextFormatterTests
{
    [Fact]
    public void Parse_PlainText_ReturnsSingleDefaultSegment()
    {
        var result = TextFormatter.Parse("Hello world");

        Assert.Single(result);
        Assert.Equal("Hello world", result[0].Text);
        Assert.Equal(GameColor.Default, result[0].Color);
    }

    [Fact]
    public void Parse_CyanText_ReturnsColoredSegment()
    {
        var result = TextFormatter.Parse("&+Cexample#");

        Assert.Contains(result, s => s.Color == GameColor.Cyan && s.Text == "example");
    }

    [Fact]
    public void Parse_MixedText_ReturnsMultipleSegments()
    {
        var result = TextFormatter.Parse("This is an &+Cexample# of a &+Rstring# with color.");

        Assert.True(result.Count >= 3);
        Assert.Contains(result, s => s.Color == GameColor.Cyan && s.Text == "example");
        Assert.Contains(result, s => s.Color == GameColor.Red && s.Text == "string");
    }

    [Fact]
    public void Parse_AllColorCodes_Recognized()
    {
        var tests = new Dictionary<string, GameColor>
        {
            ["&+Ltest#"] = GameColor.Black,
            ["&+Btest#"] = GameColor.Blue,
            ["&+Ctest#"] = GameColor.Cyan,
            ["&+Gtest#"] = GameColor.Green,
            ["&+Mtest#"] = GameColor.Magenta,
            ["&+Rtest#"] = GameColor.Red,
            ["&+Wtest#"] = GameColor.White,
            ["&+Ytest#"] = GameColor.Yellow,
            ["&+ytest#"] = GameColor.Brown,
            ["&+gtest#"] = GameColor.DarkGreen,
        };

        foreach (var (input, expectedColor) in tests)
        {
            var result = TextFormatter.Parse(input);
            Assert.Contains(result, s => s.Color == expectedColor);
        }
    }

    [Fact]
    public void Parse_EmptyString_ReturnsEmptyList()
    {
        var result = TextFormatter.Parse("");
        Assert.Empty(result);
    }

    [Fact]
    public void Parse_NullString_ReturnsEmptyList()
    {
        var result = TextFormatter.Parse(null!);
        Assert.Empty(result);
    }

    [Fact]
    public void Parse_RoomNameFormat_WorksCorrectly()
    {
        // This is the actual format used for room names: "-= &+gRoom Name# =-"
        var result = TextFormatter.Parse("-= &+gA Woodland Clearing# =-");

        Assert.Contains(result, s => s.Color == GameColor.DarkGreen && s.Text == "A Woodland Clearing");
        Assert.Contains(result, s => s.Text.Contains("-="));
    }
}
