using Metamorphosis.Core.GameEngine;

namespace Metamorphosis.Core.Tests;

public class CommandParserTests
{
    [Theory]
    [InlineData("l", "l")]       // "l" matches "l" first in command list (shortcut for look)
    [InlineData("lo", "look")]
    [InlineData("loo", "look")]
    [InlineData("look", "look")]
    public void Parse_PrefixMatching_MatchesLook(string input, string expectedVerb)
    {
        var result = CommandParser.Parse(input);

        Assert.NotNull(result);
        Assert.Equal(expectedVerb, result.Verb);
    }

    [Theory]
    [InlineData("n", "n")]
    [InlineData("north", "north")]
    [InlineData("south", "south")]
    [InlineData("east", "east")]
    [InlineData("west", "west")]
    [InlineData("up", "up")]
    [InlineData("down", "down")]
    public void Parse_DirectionCommands_Recognized(string input, string expectedVerb)
    {
        var result = CommandParser.Parse(input);

        Assert.NotNull(result);
        Assert.Equal(expectedVerb, result.Verb);
    }

    [Fact]
    public void Parse_WithArgument_SplitsCorrectly()
    {
        var result = CommandParser.Parse("examine sword");

        Assert.NotNull(result);
        Assert.Equal("examine", result.Verb);
        Assert.Equal("sword", result.Argument);
    }

    [Fact]
    public void Parse_UnknownCommand_ReturnsNull()
    {
        var result = CommandParser.Parse("xyzzy");

        Assert.Null(result);
    }

    [Fact]
    public void Parse_EmptyString_ReturnsNull()
    {
        var result = CommandParser.Parse("");

        Assert.Null(result);
    }

    [Fact]
    public void Parse_GetCommand_Recognized()
    {
        var result = CommandParser.Parse("get sword");

        Assert.NotNull(result);
        Assert.Equal("get", result.Verb);
        Assert.Equal("sword", result.Argument);
    }

    [Fact]
    public void Parse_TakeCommand_Recognized()
    {
        var result = CommandParser.Parse("take sword");

        Assert.NotNull(result);
        Assert.Equal("take", result.Verb);
        Assert.Equal("sword", result.Argument);
    }

    [Fact]
    public void Parse_QuitPartial_Recognized()
    {
        Assert.NotNull(CommandParser.Parse("q"));
        Assert.NotNull(CommandParser.Parse("qu"));
        Assert.NotNull(CommandParser.Parse("qui"));
        Assert.NotNull(CommandParser.Parse("quit"));
    }

    [Theory]
    [InlineData("look", "look", true)]
    [InlineData("l", "look", true)]
    [InlineData("lookaround", "look", false)]
    [InlineData("x", "look", false)]
    public void IsMatch_WorksCorrectly(string input, string command, bool expected)
    {
        Assert.Equal(expected, CommandParser.IsMatch(input, command));
    }
}
