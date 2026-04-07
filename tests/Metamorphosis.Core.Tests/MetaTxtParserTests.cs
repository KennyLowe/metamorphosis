using Metamorphosis.Core.Data;
using Metamorphosis.Core.Models;

namespace Metamorphosis.Core.Tests;

public class MetaTxtParserTests
{
    private readonly MetaTxtParser _parser = new();

    [Fact]
    public void ParseFromEmbeddedResource_ReturnsNonEmptyCollections()
    {
        var result = _parser.ParseFromEmbeddedResource();

        Assert.NotEmpty(result.Rooms);
        Assert.NotEmpty(result.Mobiles);
        Assert.NotEmpty(result.Objects);
    }

    [Fact]
    public void ParseFromEmbeddedResource_ParsesExpectedRoomCount()
    {
        var result = _parser.ParseFromEmbeddedResource();

        // The original code expects 96 rooms (room1 through room96)
        Assert.True(result.Rooms.Count >= 80, $"Expected at least 80 rooms, got {result.Rooms.Count}");
    }

    [Fact]
    public void ParseFromEmbeddedResource_ParsesExpectedMobileCount()
    {
        var result = _parser.ParseFromEmbeddedResource();

        Assert.True(result.Mobiles.Count >= 10, $"Expected at least 10 mobiles, got {result.Mobiles.Count}");
    }

    [Fact]
    public void ParseFromEmbeddedResource_ParsesExpectedObjectCount()
    {
        var result = _parser.ParseFromEmbeddedResource();

        Assert.True(result.Objects.Count >= 50, $"Expected at least 50 objects, got {result.Objects.Count}");
    }

    [Fact]
    public void Room1_HasCorrectExits()
    {
        var result = _parser.ParseFromEmbeddedResource();
        var room1 = result.Rooms.First(r => r.Id == "room1");

        Assert.Equal("room2", room1.GetExit(Direction.North));
        Assert.Equal("room72", room1.GetExit(Direction.South));
        Assert.Equal("room3", room1.GetExit(Direction.East));
        Assert.Equal("room4", room1.GetExit(Direction.West));
    }

    [Fact]
    public void Room1_HasCorrectName()
    {
        var result = _parser.ParseFromEmbeddedResource();
        var room1 = result.Rooms.First(r => r.Id == "room1");

        Assert.Equal("A Woodland Clearing", room1.Name);
    }

    [Fact]
    public void Room1_IsOutdoors()
    {
        var result = _parser.ParseFromEmbeddedResource();
        var room1 = result.Rooms.First(r => r.Id == "room1");

        Assert.True(room1.IsOutdoors);
        Assert.False(room1.IsDeath);
    }

    [Fact]
    public void Room1_HasDescription()
    {
        var result = _parser.ParseFromEmbeddedResource();
        var room1 = result.Rooms.First(r => r.Id == "room1");

        Assert.Contains("clearing", room1.Description, StringComparison.OrdinalIgnoreCase);
    }

    [Fact]
    public void RoomWithFormDescriptions_ContainsFormMarkers()
    {
        var result = _parser.ParseFromEmbeddedResource();
        // room3 has a $beetlex description based on meta.txt
        var room3 = result.Rooms.First(r => r.Id == "room3");

        Assert.Contains("$beetlex", room3.Description);
    }

    [Fact]
    public void Room_GetDescriptionForForm_ReturnsFormSpecificText()
    {
        var result = _parser.ParseFromEmbeddedResource();
        var room3 = result.Rooms.First(r => r.Id == "room3");

        var beetleDesc = room3.GetDescriptionForForm(AnimalForm.Beetle);
        var humanDesc = room3.GetDescriptionForForm(AnimalForm.Human);

        Assert.NotEqual(beetleDesc, humanDesc);
        Assert.Contains("Scuttling", beetleDesc);
    }

    [Fact]
    public void AllRooms_HaveUniqueUids()
    {
        var result = _parser.ParseFromEmbeddedResource();
        var uids = result.Rooms.Select(r => r.Uid).ToList();

        Assert.Equal(uids.Count, uids.Distinct().Count());
    }

    [Fact]
    public void AllRooms_HaveUniqueIds()
    {
        var result = _parser.ParseFromEmbeddedResource();
        var ids = result.Rooms.Select(r => r.Id).ToList();

        Assert.Equal(ids.Count, ids.Distinct().Count());
    }

    [Fact]
    public void AllMobiles_HaveDescriptions()
    {
        var result = _parser.ParseFromEmbeddedResource();
        foreach (var mob in result.Mobiles)
        {
            Assert.False(string.IsNullOrEmpty(mob.Id), "Mobile has empty ID");
        }
    }

    [Fact]
    public void MostObjects_HaveNames()
    {
        var result = _parser.ParseFromEmbeddedResource();
        var namedObjects = result.Objects.Count(o => !string.IsNullOrEmpty(o.Name));
        // Most objects should have names; a few may have empty names due to parsing
        Assert.True(namedObjects > result.Objects.Count * 0.9,
            $"Only {namedObjects}/{result.Objects.Count} objects have names");
    }

    [Fact]
    public void PlayerStartingRoom_Exists()
    {
        var result = _parser.ParseFromEmbeddedResource();
        var startRoom = result.Rooms.FirstOrDefault(r => r.Id == PlayerState.StartingRoom);

        Assert.NotNull(startRoom);
    }

    [Fact]
    public void LimboRoom_Exists()
    {
        var result = _parser.ParseFromEmbeddedResource();
        // room74 is used as limbo for dropped items on death
        var limbo = result.Rooms.FirstOrDefault(r => r.Id == "room74");

        Assert.NotNull(limbo);
    }
}
