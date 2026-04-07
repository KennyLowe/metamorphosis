namespace Metamorphosis.Core.GameEngine;

public record ParsedCommand(string Verb, string? Argument);

public static class CommandParser
{
    private static readonly string[] KnownCommands =
    {
        // Single-char shortcuts (must come first to avoid prefix conflicts)
        "n", "l", "h", "r", "q", "qu", "qui",
        // Navigation
        "north", "south", "east", "west", "up", "down",
        // View
        "look", "examine", "ploc", "inventory",
        // Basic interaction
        "get", "take", "drop", "wield", "wear", "remove",
        // Advanced interaction
        "eat", "drink", "dig", "fly", "climb", "enter",
        "open", "close", "push", "pull", "turn",
        "peck", "bite", "skin", "kick", "cut",
        "fill", "pour", "light", "smell", "tickle",
        "give", "kill", "attack", "cast", "say",
        "empty",
        // Special
        "goto", "pray", "kneel", "jump",
        // Meta
        "help", "reset", "quit"
    };

    public static ParsedCommand? Parse(string input)
    {
        if (string.IsNullOrWhiteSpace(input))
            return null;

        var lower = input.Trim().ToLower();
        string verb;
        string? argument = null;

        if (lower.Contains(' '))
        {
            var parts = lower.Split(' ', 2);
            verb = parts[0];
            argument = parts.Length > 1 ? parts[1] : null;
        }
        else
        {
            verb = lower;
        }

        foreach (var cmd in KnownCommands)
        {
            if (IsMatch(verb, cmd))
                return new ParsedCommand(cmd, argument);
        }

        return null;
    }

    public static bool IsMatch(string input, string command)
    {
        if (input.Length == 0 || input.Length > command.Length)
            return false;

        return command.StartsWith(input, StringComparison.OrdinalIgnoreCase);
    }
}
