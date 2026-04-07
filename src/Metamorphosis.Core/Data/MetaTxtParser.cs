using Metamorphosis.Core.Models;

namespace Metamorphosis.Core.Data;

public class MetaTxtParser
{
    public record ParseResult(List<Room> Rooms, List<Mobile> Mobiles, List<GameObject> Objects);

    public ParseResult Parse(string content)
    {
        var rooms = new List<Room>();
        var mobiles = new List<Mobile>();
        var objects = new List<GameObject>();

        var lines = new Queue<string>(content.Split('\n').Select(l => l.TrimEnd('\r')));
        int uid = 1; // UID 1 is reserved for the player

        // Parse rooms until MOBILES marker
        ParseRooms(lines, rooms, ref uid);

        // Parse mobiles until OBJECTS marker
        ParseMobiles(lines, mobiles, rooms, ref uid);

        // Parse objects until EOF
        ParseObjects(lines, objects, rooms, mobiles, ref uid);

        return new ParseResult(rooms, mobiles, objects);
    }

    public ParseResult ParseFromEmbeddedResource()
    {
        var assembly = typeof(MetaTxtParser).Assembly;
        using var stream = assembly.GetManifestResourceStream("Metamorphosis.Core.Resources.meta.txt")
            ?? throw new InvalidOperationException("Embedded resource meta.txt not found.");
        using var reader = new StreamReader(stream);
        return Parse(reader.ReadToEnd());
    }

    private void ParseRooms(Queue<string> lines, List<Room> rooms, ref int uid)
    {
        int roomIdNo = 0;
        string name = "", ndir = "", sdir = "", edir = "", wdir = "", udir = "", ddir = "";
        bool outdoors = false, nomobiles = false, death = false, underwater = false;

        while (lines.Count > 0)
        {
            var line = lines.Dequeue();

            if (line == "MOBILES")
                return;

            if (line.StartsWith("id:"))
            {
                // Format: id:room1 n:\room2 s:\room72 e:\room3 w:\room4 u:\ d:\mounds
                var parts = line.Split('\\');
                ndir = parts.Length > 1 ? StripDirSuffix(parts[1]) : "";
                sdir = parts.Length > 2 ? StripDirSuffix(parts[2]) : "";
                edir = parts.Length > 3 ? StripDirSuffix(parts[3]) : "";
                wdir = parts.Length > 4 ? StripDirSuffix(parts[4]) : "";
                udir = parts.Length > 5 ? StripDirSuffix(parts[5]) : "";
                ddir = parts.Length > 6 ? parts[6] : "";
            }
            else if (line.StartsWith("flags:"))
            {
                outdoors = false; nomobiles = false; death = false; underwater = false;
                var flagStr = line.Substring(6).ToLower();
                if (flagStr.Contains("outdoors")) outdoors = true;
                if (flagStr.Contains("nomobiles")) nomobiles = true;
                if (flagStr.Contains("death")) death = true;
                if (flagStr.Contains("underwater")) underwater = true;
            }
            else if (line.StartsWith("name:"))
            {
                name = line.Substring(5);
            }
            else if (line.StartsWith("\""))
            {
                // Begin room description (multi-line, ends with line ending in ")
                var desc = line.Substring(1); // Remove opening quote
                while (lines.Count > 0)
                {
                    var descLine = lines.Dequeue();
                    if (descLine.EndsWith("\""))
                    {
                        desc += "\n" + descLine.TrimEnd('"');
                        break;
                    }
                    else
                    {
                        desc += "\n" + descLine;
                    }
                }

                roomIdNo++;
                uid++;
                var room = new Room
                {
                    Uid = uid,
                    Id = "room" + roomIdNo,
                    Name = name,
                    Description = desc,
                    IsOutdoors = outdoors,
                    NoMobiles = nomobiles,
                    IsDeath = death,
                    IsUnderwater = underwater,
                    Exits = BuildExits(ndir, sdir, edir, wdir, udir, ddir)
                };
                rooms.Add(room);
            }
        }
    }

    private void ParseMobiles(Queue<string> lines, List<Mobile> mobiles, List<Room> rooms, ref int uid)
    {
        int mobIdNo = 0;
        string id = "", pname = "", altname = "", desc = "";
        int speed = 0, loc = 0;

        while (lines.Count > 0)
        {
            var line = lines.Dequeue();

            if (line == "OBJECTS")
                return;

            if (line.StartsWith("id:")) id = line.Substring(3);
            else if (line.StartsWith("pname:")) pname = line.Substring(6);
            else if (line.StartsWith("altname:")) altname = line.Substring(8);
            else if (line.StartsWith("speed:")) int.TryParse(line.Substring(6), out speed);
            else if (line.StartsWith("loc:")) loc = GetRoomUidByName(rooms, line.Substring(4));
            else if (line.StartsWith("desc:")) desc = line.Substring(5);
            else if (line.StartsWith("ex:"))
            {
                var ex = ReadMultiLineBlock(lines, '/');

                mobIdNo++;
                uid++;
                mobiles.Add(new Mobile
                {
                    Uid = uid,
                    Id = id,
                    PName = pname,
                    AltName = altname,
                    Location = loc,
                    Speed = speed,
                    Description = desc,
                    Examine = ex
                });

                id = ""; pname = ""; altname = ""; desc = "";
                speed = 0; loc = 0;
            }
        }
    }

    private void ParseObjects(Queue<string> lines, List<GameObject> objects, List<Room> rooms, List<Mobile> mobiles, ref int uid)
    {
        int objIdNo = 0;
        string zname = "", name = "", desc = "";
        int loc = 0, carried = 0, wielded = 0, worn = 0, incont = 0, linked = 0, state = 0;
        bool noget = false, iscont = false, door = false, weapon = false, wearable = false;

        while (lines.Count > 0)
        {
            var line = lines.Dequeue();
            if (string.IsNullOrEmpty(line)) continue;

            if (line.StartsWith("id:")) zname = line.Substring(3);
            else if (line.StartsWith("flags:"))
            {
                var flags = line.Substring(6).Split(' ');
                foreach (var f in flags)
                {
                    if (f == "noget") noget = true;
                    if (f == "iscont") iscont = true;
                    if (f == "door") door = true;
                    if (f == "weapon") weapon = true;
                    if (f == "wearable") wearable = true;
                }
            }
            else if (line.StartsWith("loc:"))
            {
                var locName = line.Substring(4);
                loc = ResolveLoc(locName, rooms, mobiles, objects);
            }
            else if (line.StartsWith("wielded:"))
            {
                var mobName = line.Substring(8);
                wielded = GetMobileUidByName(mobiles, mobName);
                loc = wielded;
            }
            else if (line.StartsWith("worn:"))
            {
                var mobName = line.Substring(5);
                worn = GetMobileUidByName(mobiles, mobName);
                loc = worn;
            }
            else if (line.StartsWith("carried:"))
            {
                var mobName = line.Substring(8);
                carried = GetMobileUidByName(mobiles, mobName);
                loc = carried;
            }
            else if (line.StartsWith("name:")) name = line.Substring(5);
            else if (line.StartsWith("desc:")) desc = line.Substring(5);
            else if (line.StartsWith("state:")) int.TryParse(line.Substring(6), out state);
            else if (line.StartsWith("ex:"))
            {
                var ex = ReadMultiLineBlock(lines, '/');

                objIdNo++;
                uid++;
                objects.Add(new GameObject
                {
                    Uid = uid,
                    Id = "obj" + objIdNo,
                    ZName = zname,
                    Location = loc,
                    Name = name,
                    Description = desc,
                    Examine = ex,
                    NoGet = noget,
                    Carried = carried,
                    Wielded = wielded,
                    Worn = worn,
                    IsContainer = iscont,
                    InContainer = incont,
                    State = state,
                    IsDoor = door,
                    Linked = linked,
                    IsWeapon = weapon,
                    IsWearable = wearable
                });

                zname = ""; name = ""; desc = "";
                loc = 0; carried = 0; wielded = 0; worn = 0; incont = 0; linked = 0; state = 0;
                noget = false; iscont = false; door = false; weapon = false; wearable = false;
            }
        }
    }

    private static string ReadMultiLineBlock(Queue<string> lines, char terminator)
    {
        var result = "";
        while (lines.Count > 0)
        {
            var line = lines.Dequeue();
            if (line.EndsWith(terminator.ToString()))
            {
                var trimmed = line.TrimEnd(terminator);
                result = result == "" ? trimmed : result + "\n" + trimmed;
                break;
            }
            else
            {
                result = result == "" ? line : result + "\n" + line;
            }
        }
        return result;
    }

    private static string StripDirSuffix(string dirPart)
    {
        // Direction parts have trailing " x:\" format — strip last 3 chars (e.g., " s:")
        return dirPart.Length > 3 ? dirPart.Substring(0, dirPart.Length - 3) : dirPart;
    }

    private static Dictionary<Direction, string> BuildExits(string n, string s, string e, string w, string u, string d)
    {
        var exits = new Dictionary<Direction, string>();
        if (!string.IsNullOrEmpty(n)) exits[Direction.North] = n;
        if (!string.IsNullOrEmpty(s)) exits[Direction.South] = s;
        if (!string.IsNullOrEmpty(e)) exits[Direction.East] = e;
        if (!string.IsNullOrEmpty(w)) exits[Direction.West] = w;
        if (!string.IsNullOrEmpty(u)) exits[Direction.Up] = u;
        if (!string.IsNullOrEmpty(d)) exits[Direction.Down] = d;
        return exits;
    }

    private static int GetRoomUidByName(List<Room> rooms, string roomId)
    {
        return rooms.FirstOrDefault(r => r.Id == roomId)?.Uid ?? 0;
    }

    private static int GetMobileUidByName(List<Mobile> mobiles, string mobId)
    {
        if (mobId == "player") return PlayerState.PlayerUid;
        return mobiles.FirstOrDefault(m => m.Id == mobId)?.Uid ?? 0;
    }

    private static int ResolveLoc(string locName, List<Room> rooms, List<Mobile> mobiles, List<GameObject> objects)
    {
        var roomUid = rooms.FirstOrDefault(r => r.Id == locName)?.Uid ?? 0;
        if (roomUid > 0) return roomUid;
        var mobUid = GetMobileUidByName(mobiles, locName);
        if (mobUid > 0) return mobUid;
        var objUid = objects.FirstOrDefault(o => o.Id == locName)?.Uid ?? 0;
        return objUid;
    }
}
