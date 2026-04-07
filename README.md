# Metamorphosis

A text-based adventure game where you explore a rich world, solve puzzles, and transform into different animal forms (bird, snake, beetle, fish).

Originally created as a Windows 8.1 Store app by Kenny Lowe in 2013, now modernized as a Blazor WebAssembly web application.

## Features

- Explore 96+ rooms across forests, rivers, caves, and towns
- Transform into animal forms with unique abilities
- Solve puzzles that require creative thinking
- Rich color-coded text output
- Classic text adventure command parser with shorthand support

## Project Structure

```
Metamorphosis.sln
  src/
    Metamorphosis.Core/          # Game engine, models, data parser (no UI dependencies)
    Metamorphosis.Web/           # Blazor WebAssembly frontend
  tests/
    Metamorphosis.Core.Tests/    # xUnit tests (64 tests)
  Meta/                          # Original legacy WinRT source (preserved)
```

## Build & Run

Requires [.NET 8 SDK](https://dotnet.microsoft.com/download/dotnet/8.0).

```bash
# Build
dotnet build Metamorphosis.sln

# Run tests
dotnet test

# Run locally
dotnet run --project src/Metamorphosis.Web
```

Then open `https://localhost:5001` in your browser.

## Commands

- **Movement**: north, south, east, west, up, down (or n, s, e, w, u, d)
- **Look**: look (or l) - see your surroundings
- **Examine**: examine [object] - inspect something closely
- **Inventory**: inventory - see what you're carrying
- **Objects**: get, drop, wield - interact with items
- **Special**: pray, jump - context-sensitive actions
- **Meta**: help, quit, reset

All commands can be shortened (e.g., "lo" for "look", "ex" for "examine").

## Architecture

The modernization separated the original monolithic 2,088-line code-behind into clean layers:

- **Models**: `Room`, `Mobile`, `GameObject`, `PlayerState`, enums for `Direction` and `AnimalForm`
- **Data**: `MetaTxtParser` parses the game world from `meta.txt`; `InMemoryGameDataStore` replaces SQLite
- **Engine**: `GameEngine` handles all game logic; `CommandParser` for input; `TextFormatter` for color codes
- **UI**: Blazor components (`GameOutputPanel`, `CommandInput`, `DirectionButtons`, `StatusPanel`)

## License

Original game content by Kenny Lowe.
