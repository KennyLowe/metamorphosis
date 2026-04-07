using Metamorphosis.Core.Models;

namespace Metamorphosis.Core.Data;

public class InMemoryGameDataStore : IGameDataStore
{
    private Dictionary<int, Room> _rooms = new();
    private Dictionary<string, int> _roomIdToUid = new();
    private Dictionary<int, Mobile> _mobiles = new();
    private Dictionary<string, int> _mobileIdToUid = new();
    private Dictionary<int, GameObject> _objects = new();
    private Dictionary<string, int> _objectIdToUid = new();

    public PlayerState Player { get; private set; } = new();

    public void Reset(List<Room> rooms, List<Mobile> mobiles, List<GameObject> objects)
    {
        _rooms = rooms.ToDictionary(r => r.Uid);
        _roomIdToUid = rooms.ToDictionary(r => r.Id, r => r.Uid);
        _mobiles = mobiles.ToDictionary(m => m.Uid);
        _mobileIdToUid = mobiles.ToDictionary(m => m.Id, m => m.Uid);
        _objects = objects.Select(o => o.Clone()).ToDictionary(o => o.Uid);
        _objectIdToUid = objects.ToDictionary(o => o.Id, o => o.Uid);
        Player = new PlayerState();
    }

    public Room? GetRoomByUid(int uid) => _rooms.GetValueOrDefault(uid);
    public Room? GetRoomById(string id) => _roomIdToUid.TryGetValue(id, out var uid) ? _rooms.GetValueOrDefault(uid) : null;
    public int GetRoomUid(string roomId) => _roomIdToUid.GetValueOrDefault(roomId, 0);
    public IEnumerable<Room> GetAllRooms() => _rooms.Values;

    public Mobile? GetMobileByUid(int uid) => _mobiles.GetValueOrDefault(uid);
    public int GetMobileUid(string mobileId)
    {
        if (mobileId == "player") return PlayerState.PlayerUid;
        return _mobileIdToUid.GetValueOrDefault(mobileId, 0);
    }
    public IEnumerable<Mobile> GetMobilesAtLocation(int locationUid) =>
        _mobiles.Values.Where(m => m.Location == locationUid);

    public GameObject? GetObjectByUid(int uid) => _objects.GetValueOrDefault(uid);
    public int GetObjectUid(string objectId) => _objectIdToUid.GetValueOrDefault(objectId, 0);
    public IEnumerable<GameObject> GetObjectsAtLocation(int locationUid) =>
        _objects.Values.Where(o => o.Location == locationUid && o.Carried == 0 && o.Wielded == 0 && o.Worn == 0);
    public IEnumerable<GameObject> GetObjectsByName(string name, int locationUid, int playerUid) =>
        _objects.Values.Where(o =>
            (o.Name == name || o.ZName == name) &&
            (o.Location == locationUid || o.Carried == playerUid || o.Worn == playerUid || o.Wielded == playerUid));
    public IEnumerable<GameObject> GetPlayerInventory(int playerUid) =>
        _objects.Values.Where(o => o.Carried == playerUid || o.Worn == playerUid || o.Wielded == playerUid);

    public void UpdateObject(GameObject obj)
    {
        _objects[obj.Uid] = obj;
    }

    public void UpdateMobile(Mobile mob)
    {
        _mobiles[mob.Uid] = mob;
    }

    public void UpdatePlayer(PlayerState player)
    {
        Player = player;
    }
}
