using Metamorphosis.Core.Models;

namespace Metamorphosis.Core.Data;

public interface IGameDataStore
{
    PlayerState Player { get; }

    Room? GetRoomByUid(int uid);
    Room? GetRoomById(string id);
    int GetRoomUid(string roomId);
    IEnumerable<Room> GetAllRooms();

    Mobile? GetMobileByUid(int uid);
    int GetMobileUid(string mobileId);
    IEnumerable<Mobile> GetMobilesAtLocation(int locationUid);

    GameObject? GetObjectByUid(int uid);
    int GetObjectUid(string objectId);
    IEnumerable<GameObject> GetObjectsAtLocation(int locationUid);
    IEnumerable<GameObject> GetObjectsByName(string name, int locationUid, int playerUid);
    IEnumerable<GameObject> GetPlayerInventory(int playerUid);

    void UpdateObject(GameObject obj);
    void UpdateMobile(Mobile mob);
    void UpdatePlayer(PlayerState player);

    void Reset(List<Room> rooms, List<Mobile> mobiles, List<GameObject> objects);
}
