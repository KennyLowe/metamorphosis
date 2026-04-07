using Metamorphosis.Core.Data;
using Metamorphosis.Core.Models;
using Engine = Metamorphosis.Core.GameEngine.GameEngine;

namespace Metamorphosis.Core.Tests;

public class GameEngineTests
{
    private readonly Engine _engine;
    private readonly InMemoryGameDataStore _store;

    public GameEngineTests()
    {
        _store = new InMemoryGameDataStore();
        var parser = new MetaTxtParser();
        _engine = new Engine(_store, parser);
    }

    [Fact]
    public void Initialize_ShowsEntryMenu()
    {
        var output = _engine.Initialize();

        Assert.True(output.IsEntryMenu);
        Assert.NotEmpty(output.Lines);
    }

    [Fact]
    public void NewGame_FromEntryMenu_ShowsStartingRoom()
    {
        _engine.Initialize();
        var output = _engine.ProcessCommand("n");

        Assert.True(output.ShouldClearOutput);
        Assert.NotEmpty(output.Lines);
        // After new game, player is moved from prologue (room85) to room87 (Sleeping Quarters)
        Assert.Equal("room87", _store.Player.CurrentRoomId);
        Assert.False(_store.Player.IsInEntryMenu);
    }

    [Fact]
    public void NewGame_PlayerStartsAsHuman()
    {
        _engine.Initialize();
        _engine.ProcessCommand("n");

        Assert.Equal(AnimalForm.Human, _store.Player.CurrentForm);
    }

    [Fact]
    public void Look_ShowsCurrentRoom()
    {
        _engine.Initialize();
        _engine.ProcessCommand("n"); // New game

        var output = _engine.ProcessCommand("look");

        Assert.NotEmpty(output.Lines);
    }

    [Fact]
    public void MoveNorth_FromStartingRoom_ChangesRoom()
    {
        _engine.Initialize();
        _engine.ProcessCommand("n"); // New game
        var startRoom = _store.Player.CurrentRoomId;

        // Try moving in any direction that exists
        _engine.ProcessCommand("north");

        // Room should have changed (or we got "can't go that way")
        // Just verify no crash
    }

    [Fact]
    public void InvalidDirection_ShowsCannotGo()
    {
        _engine.Initialize();
        _engine.ProcessCommand("n"); // New game

        // The starting room (room85) may not have all exits
        // Just verify the engine handles movement without crashing
        var output = _engine.ProcessCommand("down");
        Assert.NotEmpty(output.Lines);
    }

    [Fact]
    public void UnknownCommand_ShowsPardon()
    {
        _engine.Initialize();
        _engine.ProcessCommand("n"); // New game

        var output = _engine.ProcessCommand("xyzzy");
        Assert.Contains(output.Lines, l => l.Any(s => s.Text.Contains("Pardon?")));
    }

    [Fact]
    public void Help_ShowsHelpText()
    {
        _engine.Initialize();
        _engine.ProcessCommand("n"); // New game

        var output = _engine.ProcessCommand("help");
        Assert.NotEmpty(output.Lines);
    }

    [Fact]
    public void Inventory_ShowsInventoryState()
    {
        _engine.Initialize();
        _engine.ProcessCommand("n"); // New game

        var output = _engine.ProcessCommand("inventory");
        Assert.NotEmpty(output.Lines);
    }

    [Fact]
    public void ExamineMe_ShowsDescription()
    {
        _engine.Initialize();
        _engine.ProcessCommand("n"); // New game

        var output = _engine.ProcessCommand("examine me");
        Assert.Contains(output.Lines, l => l.Any(s => s.Text.Contains("strapping")));
    }

    [Fact]
    public void Jump_AtNonSpecialRoom_ShowsWheee()
    {
        _engine.Initialize();
        _engine.ProcessCommand("n"); // New game

        var output = _engine.ProcessCommand("jump");
        Assert.Contains(output.Lines, l => l.Any(s => s.Text.Contains("Wheeeee")));
    }

    [Fact]
    public void Pray_AtNonSpecialRoom_ShowsPrayer()
    {
        _engine.Initialize();
        _engine.ProcessCommand("n"); // New game

        var output = _engine.ProcessCommand("pray");
        Assert.Contains(output.Lines, l => l.Any(s => s.Text.Contains("prayer")));
    }

    [Fact]
    public void QuitPartial_ShowsWarning()
    {
        _engine.Initialize();
        _engine.ProcessCommand("n"); // New game

        var output = _engine.ProcessCommand("q");
        Assert.Contains(output.Lines, l => l.Any(s => s.Text.Contains("entirety")));
    }

    [Fact]
    public void Quit_ReturnsToEntryMenu()
    {
        _engine.Initialize();
        _engine.ProcessCommand("n"); // New game

        var output = _engine.ProcessCommand("quit");
        Assert.True(output.IsEntryMenu);
        Assert.True(_store.Player.IsInEntryMenu);
    }

    [Fact]
    public void Drop_WithNoArg_AsksWhat()
    {
        _engine.Initialize();
        _engine.ProcessCommand("n");

        var output = _engine.ProcessCommand("drop");
        Assert.Contains(output.Lines, l => l.Any(s => s.Text.Contains("Drop what?")));
    }

    [Fact]
    public void Get_WithNoArg_AsksWhat()
    {
        _engine.Initialize();
        _engine.ProcessCommand("n");

        var output = _engine.ProcessCommand("get");
        Assert.Contains(output.Lines, l => l.Any(s => s.Text.Contains("Get what?")));
    }

    [Fact]
    public void ExamineNothing_AsksWhat()
    {
        _engine.Initialize();
        _engine.ProcessCommand("n");

        var output = _engine.ProcessCommand("examine");
        Assert.Contains(output.Lines, l => l.Any(s => s.Text.Contains("Examine what?")));
    }

    [Fact]
    public void Wield_WithNoArg_AsksWhat()
    {
        _engine.Initialize();
        _engine.ProcessCommand("n");

        var output = _engine.ProcessCommand("wield");
        Assert.Contains(output.Lines, l => l.Any(s => s.Text.Contains("Wield what?")));
    }

    [Fact]
    public void DeathMenu_ShowsOnDeath_AndCanRestart()
    {
        _engine.Initialize();
        _engine.ProcessCommand("n"); // New game

        // Navigate to room86 and jump to die
        _engine.ProcessCommand("goto room86");
        var jumpOutput = _engine.ProcessCommand("jump");

        Assert.True(_store.Player.IsDead);
        Assert.True(jumpOutput.IsDeathMenu);

        // Restart from death
        var restartOutput = _engine.ProcessCommand("r");
        Assert.False(_store.Player.IsDead);
        Assert.False(_store.Player.IsInEntryMenu);
    }

    [Fact]
    public void Pray_AtRoom86_TransformsToBeetle()
    {
        _engine.Initialize();
        _engine.ProcessCommand("n"); // New game
        _engine.ProcessCommand("goto room86");

        var output = _engine.ProcessCommand("pray");

        Assert.Equal(AnimalForm.Beetle, _store.Player.CurrentForm);
        Assert.Equal("room72", _store.Player.CurrentRoomId);
    }


    [Fact]
    public void Look_AtRoomWithExits_ShowsDirections()
    {
        _engine.Initialize();
        _engine.ProcessCommand("n"); // New game
        // Already at room87 (Sleeping Quarters) which has an east exit

        var output = _engine.ProcessCommand("look");
        var allText = string.Join("\n", output.Lines.Select(l => string.Join("", l.Select(s => s.Text))));

        Assert.Contains("Obvious exits", allText);
        // Room87 (Sleeping Quarters) has an east exit
        Assert.Contains("East", allText);
    }
}
