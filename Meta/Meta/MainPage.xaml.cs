using System;
using System.IO;
using System.Linq;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using Windows.Storage;
using System.Threading.Tasks;
using Windows.Storage.Search;
using SQLite;
using Windows.UI;

namespace Meta
{
    public sealed partial class MainPage : Windows.UI.Xaml.Controls.Page
    {
        string loc;
        int mynum = 1;

        public MainPage()
        {
            this.InitializeComponent();
            entry_menu(1);
            // copy_Files(newgame);
        }
        
        public void reset()
        {
            border.BorderBrush = new SolidColorBrush(Windows.UI.Color.FromArgb(0xff, 0x00, 0x00, 0x00));
            border.Background = new SolidColorBrush(Windows.UI.Color.FromArgb(0x00, 0x00, 0x00, 0x00));
            routputWindow.Blocks.Clear();
            copy_Files(1);
        }

        private async void copy_Files(int debug)
        {
            routputWindow.Blocks.Clear();
            routputWindow.ToString();
            int exists = 0;
            int dead = 0;
            StorageFolder storageFolder = Windows.Storage.ApplicationData.Current.LocalFolder;        
            var files = await storageFolder.GetFilesAsync();
            // Run through every file in folder
            if (debug == 0)
            {
                border.BorderBrush = new SolidColorBrush(Windows.UI.Color.FromArgb(0xff, 0x3a, 0x35, 0xae));
                border.Background = new SolidColorBrush(Windows.UI.Color.FromArgb(0x00, 0x00, 0x00, 0x00));
                //loading.Visibility = Windows.UI.Xaml.Visibility.Collapsed;
                //routputWindow.Visibility = Windows.UI.Xaml.Visibility.Visible;

                foreach (StorageFile file in files)
                {
                    try
                    {
                        if (file != null)
                        {
                            // If DB exists, don't do anything nasty.
                            if (file.Name == "zone_rooms.sqlite")
                            {
                                var dbPath = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "zone_rooms.sqlite");
                                using (var db = new SQLite.SQLiteConnection(dbPath))
                                {
                                    var d = from x in db.Table<Zone_Ploc>() select x;
                                    foreach (var sd in d)
                                    {
                                        dead = sd.dead;
                                    }
                                }
                                exists = 1;
                                show_ploc();
                                update_inv();
                                update_dead(0);
                                update_entry(0);
                                ascii.FontSize = 11;
                                ascii.Text = "   |\\             //" + "\n" + "    \\\\           _!_" + "\n" + "     \\\\         /___\\" + "\n" + "      \\\\        [+++]" + "\n" + "       \\\\    _ _\\^^^/_ _" + "\n" + "        \\\\/ (    '-'  ( )" + "\n" + "        /( \\/ | {&}   /\\ \\" + "\n" + "          \\  / \\     / _> )" + "\n" + "           \\\"`   >:::;-'`\"\"'-." + "\n" + "               /:::/         \\" + "\n" + "              /  /||   {&}   |" + "\n" + "             (  / (\\        /" + "\n" + "             / /   \'-.___.-'" + "\n" + "           _/ /     \\ \\" + "\n" + "          /___|    /___|";
                                str_mana.Text = "S:999\\999 M:999\\999";


                            }
                        }
                    }
                    catch (Exception ex)
                    {
                        bprintf("Error initialising: " + ex.Message);
                        return;
                    }
                }
            }

            if ((exists == 0) || (debug == 1) || (dead == 1))
            {
                
                border.BorderBrush = new SolidColorBrush(Windows.UI.Color.FromArgb(0x00, 0x00, 0x00, 0x00));
                border.Background = new SolidColorBrush(Windows.UI.Color.FromArgb(0x00, 0x00, 0x00, 0x00));
                await deleteFile("zone_rooms.sqlite");
                await deleteFile("meta.txt");
                StorageFile txtfile = await StorageFile.GetFileFromApplicationUriAsync(new Uri("ms-appx:///meta.txt"));
                await txtfile.CopyAsync(ApplicationData.Current.LocalFolder, "meta.txt");
                StorageFile sqlfile = await StorageFile.GetFileFromApplicationUriAsync(new Uri("ms-appx:///zone_rooms.sqlite"));
                await sqlfile.CopyAsync(ApplicationData.Current.LocalFolder, "zone_rooms.sqlite");
                string dbRootPath = Windows.Storage.ApplicationData.Current.LocalFolder.Path;
                using (SQLite.SQLiteConnection db = new SQLite.SQLiteConnection(Path.Combine(dbRootPath, "zone_rooms.sqlite")))
                {
                    //Create rooms table based on Zone_Rooms class
                    db.CreateTable<Zone_Rooms>();
                    db.CreateTable<Zone_Mobiles>();
                    db.CreateTable<Zone_Ploc>();
                    db.CreateTable<Zone_Objects>();
                }
                getRooms();
                //routputWindow.Visibility = Windows.UI.Xaml.Visibility.Collapsed;
                //loading.Visibility = Windows.UI.Xaml.Visibility.Visible;
                await Task.Delay(1500);
                border.BorderBrush = new SolidColorBrush(Windows.UI.Color.FromArgb(0xff, 0x3a, 0x35, 0xae));
                border.Background = new SolidColorBrush(Windows.UI.Color.FromArgb(0x00, 0x00, 0x00, 0x00));
                //loading.Visibility = Windows.UI.Xaml.Visibility.Collapsed;
                //routputWindow.Visibility = Windows.UI.Xaml.Visibility.Visible;
                scroll();
                ascii.FontSize = 11;
                ascii.Text = "   |\\             //" + "\n" + "    \\\\           _!_" + "\n" + "     \\\\         /___\\" + "\n" + "      \\\\        [+++]" + "\n" + "       \\\\    _ _\\^^^/_ _" + "\n" + "        \\\\/ (    '-'  ( )" + "\n" + "        /( \\/ | {&}   /\\ \\" + "\n" + "          \\  / \\     / _> )" + "\n" + "           \\\"`   >:::;-'`\"\"'-." + "\n" + "               /:::/         \\" + "\n" + "              /  /||   {&}   |" + "\n" + "             (  / (\\        /" + "\n" + "             / /   \'-.___.-'" + "\n" + "           _/ /     \\ \\" + "\n" + "          /___|    /___|";
                str_mana.Text = "S:999\\999 M:999\\999";
            }
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
        }



        private async Task<string> deleteFile(string filename)
        {
            StorageFolder storageFolder = Windows.Storage.ApplicationData.Current.LocalFolder;

            var files = await storageFolder.GetFilesAsync();
            // Run through every file in folder
            foreach (StorageFile file in files)
            {
                try
                {
                    if (file != null)
                    {
                        // If file is file you want to delete... delete it!
                        if (file.Name == filename)
                            await file.DeleteAsync();
                    }
                }
                catch (Exception ex)
                {
                    return ex.Message;
                }
            }
            return "Success"; //Returns a success message in case we care, we probably don't give a shit though
        }

        public class Temp
        {
            [PrimaryKey]
            public int temp { get; set; }
        }


        public class Zone_Rooms
        {
            // Database structure for Rooms table
            [PrimaryKey, Unique]
            public int uid { get; set; }
            [Column("id")]
            public string id { get; set; }
            [Column("name")]
            public string Name { get; set; }
            [Column("desc")]
            public string Desc { get; set; }
            [Column("flags")]
            public string Flags { get; set; }
            [Column("n")]
            public string n { get; set; }
            [Column("s")]
            public string s { get; set; }
            [Column("e")]
            public string e { get; set; }
            [Column("w")]
            public string w { get; set; }
            [Column("u")]
            public string u { get; set; }
            [Column("d")]
            public string d { get; set; }
            [Column("outdoors")]
            public int outdoors { get; set; }
            [Column("nomobiles")]
            public int nomobiles { get; set; }
            [Column("death")]
            public int death { get; set; }
            [Column("underwater")]
            public int underwater { get; set; }
        }

        public class Zone_Ploc
        {
            [PrimaryKey, Unique]
            public int mynum { get; set; }
            [Column("id")]
            public string id { get; set; }
            [Column("ploc")]
            public string ploc { get; set; }
            [Column("type")]
            public string type { get; set; }
            [Column("dead")]
            public int dead { get; set; }
            [Column("entry")]
            public int entry { get; set; }
        }

        public class Zone_Objects
        {
            // Database structure for Objects table
            [PrimaryKey, Unique]
            public int uid { get; set; }
            [Column("id")]
            public string id { get; set; }
            [Column("zname")]
            public string zname { get; set; }
            [Column("loc")] // Location of object
            public int loc { get; set; }
            [Column("name")] // Name of object
            public string name { get; set; }
            [Column("desc")] // Description shown when looking
            public string desc { get; set; }
            [Column("ex")] // Examine shown when examining object
            public string ex { get; set; }
            [Column("noget")] // 1 = can't be picked up, 0 = can be picked up
            public int noget { get; set; }
            [Column("carried")] // string is player or mob carrying item
            public int carried { get; set; }
            [Column("wielded")] // string is player or mob wielding the item
            public int wielded { get; set; }
            [Column("worn")] // string is player or mob wearing the item
            public int worn { get; set; }
            [Column("iscont")] // 1 for container, 0 for not a container
            public int iscont { get; set; }
            [Column("incont")] // string is object containing this object
            public int incont { get; set; }
            [Column("state")] // Various states for objects - if door and state 0, closed, if door and state 1, open
            public int state { get; set; }
            [Column("door")] // Is this a door?
            public int door { get; set; }
            [Column("linked")] // Object this object is linked to
            public int linked { get; set; }
            [Column("weapon")]
            public int weapon { get; set; }
            [Column("wearable")]
            public int wearable { get; set; }
        }

        public class Zone_Mobiles
        {
            // Database structure for Mobiles table
            [PrimaryKey, Unique]
            public int uid { get; set; }
            [Column("id")]
            public string id { get; set; }
            [Column("pname")]
            public string pname { get; set; }
            [Column("altname")]
            public string altname { get; set; }
            [Column("loc")]
            public int loc { get; set; }
            [Column("speed")]
            public int speed { get; set; }
            [Column("desc")]
            public string desc { get; set; }
            [Column("ex")]
            public string ex { get; set; }
        }

        // Function for parsing all input - important one!
        private void send_Command(object sender, RoutedEventArgs e)
        {
            string commandStr = cmdEntry.Text.ToString();
            string commandStrLow = commandStr.ToLower();
            int cmdFound = 0;
            string[] commandArray = { "n", "l", "h", "r", "q", "qu", "qui", "quit", "north", "south", "east", "west", "up", "down", "look", "ploc", "examine", "inventory", "drop", "get", "take", "wield", "goto", "pray", "reset", "kneel", "jump", "help"};

            if (!commandStrLow.Contains(" "))
            {
                foreach (string x in commandArray)
                {
                    if (checkCmd(commandStrLow, x) == 1)
                    {
                        executeCmd(x, null);
                        cmdFound = 1;
                        break;
                    }
                }
            }
            if (commandStrLow.Contains(" "))
            {
                string[] command_parts = commandStrLow.Split(' ');
                foreach (string x in commandArray)
                {
                    if (checkCmd(command_parts.ElementAt(0), x) == 1)
                    {
                        executeCmd(x, command_parts.ElementAt(1));
                        cmdFound = 1;
                        break;
                    }
                }
            }
            if (cmdFound == 0)
            {
                if (cmdEntry.Text != null)
                {
                    cmdEntry.Text = "";
                    bprintf("Pardon?");
                    scroll();
                }
            }
        }

        // This checks if the command being passed is a partial match for a real command - if it is, it executs the first matching command found
        public int checkCmd(string str, string command)
        {
            int length = str.Length;
            int lengthCmd = command.Length;

            if (length == 1)
            {
                if (str == command.Remove(1, lengthCmd - 1))
                {
                    cmdEntry.Text = "";
                    return 1;
                }
            }
            else
            {
                if (length <= lengthCmd)
                {
                    if (str == command.Remove(length, lengthCmd - length))
                    {
                        cmdEntry.Text = "";
                        return 1;
                    }
                }
                else { return 0; } // If command length is greater than string length then it can't be the command being checked.
            }
            return 0;
        }

        private void cmdEntry_KeyDown(object sender, KeyRoutedEventArgs e)
        {
            if (cmdEntry.Text != "")
            {
                if (e.Key.ToString() == "Enter") { send_Command(sender, e); }
            }
        }

        public void scroll()
        {
            var scrollViewer = OutputScroll;
            scrollViewer.Measure(scrollViewer.RenderSize);
            scrollViewer.ScrollToVerticalOffset(scrollViewer.ExtentHeight);
        }

        public void bprintf(string output)
        {
            char[] delimiterChars = { '&', '#' };
            string text = output;
            string txt;
            string[] words = text.Split(delimiterChars);
            int numberofwords = words.Count();
            var p = new Windows.UI.Xaml.Documents.Paragraph();
            foreach (string s in words)
            {
                if (s.StartsWith("+"))
                {
                    txt = s.Remove(0, 2);
                    string colour = s.Remove(0, 1).Substring(0,1);
                    if (colour == "C") { p.Inlines.Add(new Windows.UI.Xaml.Documents.Run { Foreground = new SolidColorBrush(Colors.Cyan), Text = txt });}
                    if (colour == "G") { p.Inlines.Add(new Windows.UI.Xaml.Documents.Run { Foreground = new SolidColorBrush(Colors.Green), Text = txt });}
                    if (colour == "L") { p.Inlines.Add(new Windows.UI.Xaml.Documents.Run { Foreground = new SolidColorBrush(Colors.Black), Text = txt }); }
                    if (colour == "B") { p.Inlines.Add(new Windows.UI.Xaml.Documents.Run { Foreground = new SolidColorBrush(Colors.Blue), Text = txt }); }
                    if (colour == "M") { p.Inlines.Add(new Windows.UI.Xaml.Documents.Run { Foreground = new SolidColorBrush(Colors.Magenta), Text = txt }); }
                    if (colour == "R") { p.Inlines.Add(new Windows.UI.Xaml.Documents.Run { Foreground = new SolidColorBrush(Colors.Red), Text = txt }); }
                    if (colour == "W") { p.Inlines.Add(new Windows.UI.Xaml.Documents.Run { Foreground = new SolidColorBrush(Colors.White), Text = txt }); }
                    if (colour == "Y") { p.Inlines.Add(new Windows.UI.Xaml.Documents.Run { Foreground = new SolidColorBrush(Colors.Yellow), Text = txt }); }
                    if (colour == "y") { p.Inlines.Add(new Windows.UI.Xaml.Documents.Run { Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb(0xFF, 0xBB, 0xBB, 0x00)), Text = txt }); }
                    if (colour == "g") { p.Inlines.Add(new Windows.UI.Xaml.Documents.Run { Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb(0xFF, 0x49, 0x90, 0x13)), Text = txt }); }
                }
                else
                {
                    p.Inlines.Add(new Windows.UI.Xaml.Documents.Run { Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb(0xFF, 0xE4, 0xDE, 0xDE)), Text = s });
                }
            }
            routputWindow.Blocks.Add(p);
            scroll();
        }

        public  void executeCmd(string cmd, string arg)
        {
            if ((dead() == 0) && (entry() == 0))
            {

                if (arg == null)
                {
                    if (cmd == "r") { }
                    if ((cmd == "q") || (cmd == "qu") || (cmd == "qui")) { bprintf("In order to prevent unnecessary screaming, please type 'quit' in its entirety to quit."); }
                    if (cmd == "quit") { routputWindow.Blocks.Clear(); entry_menu(1); }
                    if (cmd == "n") { can_go("n"); }
                    if (cmd == "north") { can_go("n"); }
                    if (cmd == "south") { can_go("s"); }
                    if (cmd == "east") { can_go("e"); }
                    if (cmd == "west") { can_go("w"); }
                    if (cmd == "up") { can_go("u"); }
                    if (cmd == "down") { can_go("d"); }
                    if (cmd == "l") { show_ploc(); }
                    if (cmd == "look") { show_ploc(); }
                    if (cmd == "ploc") { bprintf(loc); }
                    if (cmd == "examine") { examine(null); }
                    if (cmd == "inventory") { inventory(); }
                    if (cmd == "drop") { drop(null); }
                    if ((cmd == "get") || (cmd == "take")) { get(null); }
                    if (cmd == "wield") { wield(null); }
                    if (cmd == "goto") { tele(null); }
                    if ((cmd == "pray") || (cmd == "kneel")) { pray(); }
                    if (cmd == "jump") { jump(); }
                    if (cmd == "h") { help(null); }
                    if (cmd == "help") { help(null); }
                    if (cmd == "reset") { reset(); }
                }

                if (arg != null)
                {
                    if (cmd == "r") { }
                    if ((cmd == "q") || (cmd == "qu") || (cmd == "qui")) { bprintf("In order to prevent unnecessary screaming, please type 'quit' in its entirety to quit."); }
                    if (cmd == "quit") { routputWindow.Blocks.Clear(); entry_menu(1); }
                    if (cmd == "n") { can_go("n"); }
                    if (cmd == "north") { can_go("n"); }
                    if (cmd == "south") { can_go("s"); }
                    if (cmd == "east") { can_go("e"); }
                    if (cmd == "west") { can_go("w"); }
                    if (cmd == "up") { can_go("u"); }
                    if (cmd == "down") { can_go("d"); }
                    if (cmd == "l") { show_ploc(); }
                    if (cmd == "look") { show_ploc(); }
                    if (cmd == "ploc") { bprintf(loc); }
                    if (cmd == "examine") { examine(arg); }
                    if (cmd == "inventory") { inventory(); }
                    if (cmd == "drop") { drop(arg); }
                    if ((cmd == "get") || (cmd == "take")) { get(arg); }
                    if (cmd == "wield") { wield(arg); }
                    if (cmd == "goto") { tele(arg); }
                    if ((cmd == "pray") || (cmd == "kneel")) { pray(); }
                    if (cmd == "jump") { jump(); }
                    if (cmd == "h") { help(null); }
                    if (cmd == "help") { help(arg); }
                    if (cmd == "reset") { reset(); }
                }
                goto EoV;
            }
            if ((dead() == 1) && entry() ==0)
            {
                if (cmd == "r")
                {
                    routputWindow.Blocks.Clear();
                    bprintf("\nThe veil of death draws back, and you find yourself returning to the lands of the living...\n");
                    border.BorderBrush = new SolidColorBrush(Windows.UI.Color.FromArgb(0xff, 0x00, 0x00, 0x00));
                    border.Background = new SolidColorBrush(Windows.UI.Color.FromArgb(0x00, 0x00, 0x00, 0x00));
                    copy_Files(1);
                    goto EoV;
                }
                if (cmd == "q") { routputWindow.Blocks.Clear(); entry_menu(1); }
                else { death_menu(0); }
            }
            if (entry() == 1)
            {
                if (cmd == "n") { routputWindow.Blocks.Clear(); copy_Files(1); goto EoV; }
                if (cmd == "l") {
                    if (dead() == 1) { bprintf("You appear to be dead in your previous game... let's restart, shall we?"); copy_Files(1); } else { copy_Files(0); } goto EoV;
                }
                if (cmd == "h") { show_help(); goto EoV; }
            }
            else { entry_menu(0); }

        EoV:
            arg = null;
        }

        public void show_help()
        {
            bprintf("\nThe Help command is available once you start the game - you'll love it, I promise:)");
            bprintf("...The game that is, not the help command. Frankly, it's not that helpful.");
        }


        public void jump()
        {
            if (loc == "room86")
            {
                bprintf("Without a second thought you leap from the cliff, confident that this is but a trick to keep you from your goal.");
                bprintf("Alas it is not, prompting a highly undignified thought to flash through your brain before it's unceremoniously smashed on the rocks below.");
                kill();
            }
            else { bprintf("Wheeeee..."); }
        }

        public int droom(string loc)
        {
            if (loc != null)
            {
                var dbPath = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "zone_rooms.sqlite");
                using (var db = new SQLite.SQLiteConnection(dbPath))
                {
                    var d = from x in db.Table<Zone_Rooms>() select x;
                    foreach (var sd in d)
                    {
                        if ((sd.death == 1) && (sd.id == loc))
                        {
                            return 1;
                        }
                        else { return 0; }
                    }
                }
                return 0;
            }
            return 0;
        }

 

        public void kill()
        {
            limbo_all();
            death_menu(1);
            update_dead(1);
        }

        public int entry()
        {
            var dbPath = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "zone_rooms.sqlite");
            using (var db = new SQLite.SQLiteConnection(dbPath))
            {
                var d = from x in db.Table<Zone_Ploc>() select x;
                foreach (var sd in d)
                {
                    if (sd.entry == 1)
                    {
                        return 1;
                    }
                    else { return 0; }
                }
            }
            return 0;
        }

        public int dead()
        {
            var dbPath = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "zone_rooms.sqlite");
            using (var db = new SQLite.SQLiteConnection(dbPath))
            {
                var d = from x in db.Table<Zone_Ploc>() select x;
                foreach (var sd in d)
                {
                    if (sd.dead == 1)
                    {
                        return 1;
                    }
                    else { return 0; }
                }
            }
            return 0;
        }

        public void entry_menu(int menu)
        {
            update_entry(1);
            if (menu == 1)
            {
                bprintf("-----------------------------------------------------------------------------\n");
                bprintf("                            &+MWelcome to Metamorphosis!#\n");                     
                bprintf("-----------------------------------------------------------------------------");
            }
                bprintf("                         &+y ___________________________#");
                bprintf("                   &+M  ()==#&+y(__________________________(@#&+M==()#");
                bprintf("                           &+y|                        |#");
                bprintf("                           &+y|# &+YMenu [#&+CEntry#&+Y]#           &+y|#");
                bprintf("                           &+y|                        |#");
                bprintf("                           &+y|# &+CN#&+Y)# &+Wew Game#             &+y|#");
                bprintf("                           &+y|# &+CL#&+Y)# &+Woad Previous Game#   &+y|#");
                bprintf("                           &+y|# &+CH#&+Y)# &+Welp#                 &+y|#");
                bprintf("                           &+y|                        |#");
                bprintf("                          &+y_|________________________|#");
                bprintf("                     &+M()==#&+y(__________________________(@#&+M==()#");
        }

        public void death_menu(int menu)
        {
            if (menu == 1)
            {
                bprintf("------------------------------------------------------------------------------\n");
                bprintf("                &+ROh dear... you seem to be slightly dead.#\n");
                bprintf("------------------------------------------------------------------------------");
            }
            bprintf("         &+y ___________________________#");
            bprintf("     &+R()==#&+y(__________________________(@#&+R==()#");
            bprintf("           &+y|                        |#");
            bprintf("           &+y| &+YMenu [#&+RDeath#&+Y]#           &+y|#");
            bprintf("           &+y|                        |#");
            bprintf("           &+y|# &+CR#&+Y)# &+Westart Meta#         &+y|#");
            bprintf("           &+y|# &+RQ#&+Y)# &+Wuit game#            &+y|#");
            bprintf("          &+y_|________________________|#");
            bprintf("     &+R()==#&+y(__________________________(@#&+R==()#");
        }

        public void examine(string arg)
        {
            int found = 0;
            if (arg == null)
            {
                bprintf("Examine what?");
                found = 1;
            }
            if (arg == "me")
            {
                if (type() == "human") { bprintf("A strapping example of an adventurer if ever there was one!"); }
                if (type() == "beetle") { bprintf("You have been magically transformed into a beetle!"); }
            }
            else
            {
                var dbPath = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "zone_rooms.sqlite");
                using (var db = new SQLite.SQLiteConnection(dbPath))
                {
                    int rloc = ruid(loc);
                    var d = from x in db.Table<Zone_Objects>() where (x.name == arg || x.zname == arg) && ((x.loc == rloc) || (x.carried == mynum) || (x.worn == mynum) || (x.wielded == mynum)) select x;
                    foreach (var sd in d)
                    {
                                bprintf(sd.ex);
                                found = 1;
                    }
                    var m = from y in db.Table<Zone_Mobiles>() where (y.id == arg || y.altname == arg) select y;
                    foreach (var sd in m)
                    {
                        if (sd.loc == ploc(mynum))
                        {
                            bprintf(sd.ex);
                            found = 1;
                        }
                    }
                    if (found == 0)
                    {
                        bprintf("It's not here.");
                    }
                }
            }
        }

        public void update_oloc(string obj, string loc)
        {
            var dbPath = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "zone_rooms.sqlite");
            using (var db = new SQLite.SQLiteConnection(dbPath))
            {
                var d = from x in db.Table<Zone_Objects>() select x;
                foreach (var sd in d)
                {
                    if (sd.id == obj)
                    {
                        if (ruid(loc) > 0) { sd.loc = ruid(loc); }
                        if (muid(loc) > 0) { sd.loc = muid(loc); }
                        if (ouid(loc) > 0) { sd.loc = ouid(loc); }
                        db.Update(sd);
                    }
                }
            }
        }

        public void update_ploc(string room_id)
        {
            var dbPath = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "zone_rooms.sqlite");
            using (var db = new SQLite.SQLiteConnection(dbPath))
            {
                var d = from x in db.Table<Zone_Ploc>() select x;
                foreach (var sd in d)
                {
                    sd.ploc = room_id;
                    db.Update(sd);
                }
            }
        }

        public void help(string subj)
        {
            if (subj == null)
            {
                bprintf("Basic commands are:");
                bprintf("   north, south, east, west, up, down - movement commands.");
                bprintf("   look - see the room you are in.");
                bprintf("   examine - look closely at an object.");
                bprintf("   inventory - see what items you currently have.");
                bprintf("   get, drop, wield, wear, push, pull - manipulate objects.");
                bprintf("   pray - if all else fails...");
                bprintf("\nThere are many other commands to be found, lots of which are critical to your quest :)");
                bprintf("\nFor your convenience, all commands can be shortened - e.g. 'l, lo, loo (despite appearances) and look' all perform the same function.");
            }
        }

        public void ch_type(string animal)
        {
            if (animal != null)
            {
                if (animal == "bird") { update_type("bird"); }
                if (animal == "snake") { update_type("snake"); }
                if (animal == "beetle")
                {
                    str_mana.Text = "S:5\\5\nM:0\\0";
                    update_type("beetle");
                    update_inv();
                    ascii.FontSize = 8;
                    ascii.Text = "\n\n\n" + "              ,_    /) (\\    _," + "\n" + "               >>  <<,_,>>  <<" + "\n" + "              //   _0.-.0_   \\\\" + "\n" + "              \\'._/       \\_.'/" + "\n" + "               '-.\\.--.--./.-'" + "\n" + "               __/ : :Y: : \\ _" + "\n" + "       ';,  .-(_| : : | : : |_)-.  ,:'" + "\n" + "         \\\\/.'  |: : :|: : :|  `.\\//" + "\n" + "          (/    |: : :|: : :|    \\)" + "\n" + "                |: : :|: : :;" + "\n" + "               /\\ : : | : : /\\" + "\n" + "              (_/'.: :.: :.'\\_)" + "\n" + "               \\\\  `\"\"`\"\"`  //" + "\n" + "                \\\\         //" + "\n" + "                 ':.     .:'";
                }
                if (animal == "fish") { update_type("fish"); }
            }
        }

        public string type()
        {
            var dbPath = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "zone_rooms.sqlite");
            using (var db = new SQLite.SQLiteConnection(dbPath))
            {
                var d = from x in db.Table<Zone_Ploc>() select x;
                foreach (var sd in d)
                {
                    if ((sd.type != "") && (sd.type != null))
                    {
                        return sd.type;
                    }
                }
            }
            return "Unknown";
        }

        public void update_entry(int state)
        {
            var dbPath = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "zone_rooms.sqlite");
            using (var db = new SQLite.SQLiteConnection(dbPath))
            {
                var d = from x in db.Table<Zone_Ploc>() select x;
                foreach (var sd in d)
                {
                    sd.entry = state;
                    db.Update(sd);
                }
            }
        }

        public void update_dead(int state)
        {
            var dbPath = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "zone_rooms.sqlite");
            using (var db = new SQLite.SQLiteConnection(dbPath))
            {
                var d = from x in db.Table<Zone_Ploc>() select x;
                foreach (var sd in d)
                {
                    sd.dead = state;
                    db.Update(sd);
                }
            }
        }

        public void update_type(string arg)
        {
            var dbPath = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "zone_rooms.sqlite");
            using (var db = new SQLite.SQLiteConnection(dbPath))
            {
                var d = from x in db.Table<Zone_Ploc>() select x;
                foreach (var sd in d)
                {
                    sd.type = arg;
                    db.Update(sd);
                }
            }
        }

        public void can_go(string dir)
        {

            var dbpath = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "zone_rooms.sqlite");
            using (var db = new SQLite.SQLiteConnection(dbpath))
            {
                var d = from x in db.Table<Zone_Ploc>() select x;
                foreach (var sd in d)
                {
                    if (sd.ploc != null)
                    {
                        loc = sd.ploc;
                    }
                }
            }
            using (var db = new SQLite.SQLiteConnection(dbpath))
            {
                var d = from x in db.Table<Zone_Rooms>() select x;
                foreach (var sd in d)
                {
                    if (sd.id == loc)
                    {
                        if (dir == "n")
                        {
                            if ((sd.n != null) && (sd.n != ""))
                            {
                                setploc(mynum, ruid(sd.n));
                                show_ploc();
                                break;
                            }
                            else
                            {
                                bprintf("You cannot go that way!");
                            }
                        }
                        if (dir == "s")
                        {
                            if ((sd.s != null) && (sd.s != ""))
                            {
                                setploc(mynum, ruid(sd.s));
                                show_ploc();
                                break;
                            }
                            else
                            {
                                bprintf("You cannot go that way!");
                            }
                        }
                        if (dir == "e")
                        {
                            if ((sd.e != null) && (sd.e != ""))
                            {
                                setploc(mynum, ruid(sd.e));
                                show_ploc();
                                break;
                            }
                            else
                            {
                                bprintf("You cannot go that way!");
                            }
                        }
                        if (dir == "w")
                        {
                            if ((sd.w != null) && (sd.w != ""))
                            {
                                setploc(mynum, ruid(sd.w));
                                show_ploc();
                                break;
                            }
                            if (sd.w == "")
                            {
                                bprintf("You cannot go that way!");
                            }
                        }
                        if (dir == "u")
                        {
                            if ((sd.u != null) && (sd.u != ""))
                            {
                                setploc(mynum, ruid(sd.u));
                                show_ploc();
                                break;
                            }
                            else
                            {
                                bprintf("You cannot go that way!");
                            }
                        }
                        if (dir == "d")
                        {
                            if ((sd.d != null) && (sd.d != ""))
                            {
                                setploc(mynum, ruid(sd.d));
                                show_ploc();
                                break;
                            }
                            else
                            {
                                bprintf("You cannot go that way!");
                            }
                        }

                    }
                }
            };
        }

        public void show_ploc()
        {
            string desc;
            string name;
            int spec_shown = 0;
            int length = 0;
            if (type() == "bird")
            {
                try
                {
                    var dbpath = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "zone_rooms.sqlite");
                    using (var db = new SQLite.SQLiteConnection(dbpath))
                    {
                        var e = from y in db.Table<Zone_Ploc>() select y;
                        foreach (var sd in e)
                        {
                            if (sd.ploc != null)
                            {
                                loc = sd.ploc;
                            }
                        }
                        var d = from x in db.Table<Zone_Rooms>() select x;
                        foreach (var sd in d)
                        {
                            if (sd.id == loc)
                            {
                                desc = (sd.Desc.ToString());

                                if (desc.Contains("$bird"))
                                {
                                    int x;
                                    string[] descs = desc.Split('$');
                                    int count = descs.Count();
                                    for (x = 0; x < count; x++)
                                    {
                                        if (descs.ElementAt(x).Contains("birdx"))
                                        {
                                            desc = descs.ElementAt(x).Remove(0, 6);
                                            length = desc.Length;
                                            desc = desc.Remove(length - 1, length);
                                            spec_shown = 1;
                                            break;
                                        }
                                    }
                                }

                                name = (sd.Name.ToString());
                                bprintf("-= " + "&+g" + name + "#" + " =-");
                                bprintf(desc);
                                if (sd.death == 1)
                                {
                                    kill();
                                    goto EndOfVoid;
                                }
                                else
                                {
                                    show_objects(loc);
                                    show_mobiles(loc);
                                    show_directions();
                                    bprintf(prompt());
                                }
                            }
                        }
                    }
                }
                catch { }
            }

            if (type() == "snake")
            {
                try
                {
                    var dbpath = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "zone_rooms.sqlite");
                    using (var db = new SQLite.SQLiteConnection(dbpath))
                    {
                        var e = from y in db.Table<Zone_Ploc>() select y;
                        foreach (var sd in e)
                        {
                            if (sd.ploc != null)
                            {
                                loc = sd.ploc;
                            }
                        }
                        var d = from x in db.Table<Zone_Rooms>() select x;
                        foreach (var sd in d)
                        {
                            if (sd.id == loc)
                            {
                                desc = (sd.Desc.ToString());

                                if (desc.Contains("$snake"))
                                {
                                    int x;
                                    string[] descs = desc.Split('$');
                                    int count = descs.Count();
                                    for (x = 0; x < count; x++)
                                    {
                                        if (descs.ElementAt(x).Contains("snakex"))
                                        {
                                            desc = descs.ElementAt(x).Remove(0, 7);
                                            length = desc.Length;
                                            desc = desc.Remove(length - 1, length);
                                            spec_shown = 1;
                                            break;
                                        }
                                    }
                                }

                                name = (sd.Name.ToString());
                                bprintf("-= " + "&+g" + name + "#" + " =-");
                                bprintf(desc);
                                if (sd.death == 1)
                                {
                                    kill();
                                    goto EndOfVoid;
                                }
                                else
                                {
                                    show_objects(loc);
                                    show_mobiles(loc);
                                    show_directions();
                                    bprintf(prompt());
                                }
                            }
                        }
                    }
                }
                catch { }
            }


            if (type() == "fish")
            {
                try
                {
                    var dbpath = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "zone_rooms.sqlite");
                    using (var db = new SQLite.SQLiteConnection(dbpath))
                    {
                        var e = from y in db.Table<Zone_Ploc>() select y;
                        foreach (var sd in e)
                        {
                            if (sd.ploc != null)
                            {
                                loc = sd.ploc;
                            }
                        }
                        var d = from x in db.Table<Zone_Rooms>() select x;
                        foreach (var sd in d)
                        {
                            if (sd.id == loc)
                            {
                                desc = (sd.Desc.ToString());

                                if (desc.Contains("$fish"))
                                {
                                    int x;
                                    string[] descs = desc.Split('$');
                                    int count = descs.Count();
                                    for (x = 0; x < count; x++)
                                    {
                                        if (descs.ElementAt(x).Contains("fishx"))
                                        {
                                            desc = descs.ElementAt(x).Remove(0, 6);
                                            length = desc.Length;
                                            desc = desc.Remove(length - 1, length);
                                            spec_shown = 1;
                                            break;
                                        }
                                    }
                                }

                                name = (sd.Name.ToString());
                                bprintf("-= " + "&+g" + name + "#" + " =-");
                                bprintf(desc);
                                if (sd.death == 1)
                                {
                                    kill();
                                }
                                else
                                {
                                    show_objects(loc);
                                    show_mobiles(loc);
                                    show_directions();
                                    bprintf(prompt());
                                }
                            }
                        }
                    }
                }
                catch { }
            }
            if (type() == "beetle")
            {
                try
                {
                    var dbpath = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "zone_rooms.sqlite");
                    using (var db = new SQLite.SQLiteConnection(dbpath))
                    {
                        var e = from y in db.Table<Zone_Ploc>() select y;
                        foreach (var sd in e)
                        {
                            if (sd.ploc != null)
                            {
                                loc = sd.ploc;
                            }
                        }
                        var d = from x in db.Table<Zone_Rooms>() select x;
                        foreach (var sd in d)
                        {
                            if (sd.id == loc)
                            {
                                desc = (sd.Desc.ToString());

                                if (desc.Contains("$beetle"))
                                {
                                    int x;
                                    string[] descs = desc.Split('$');
                                    int count = descs.Count();
                                    for (x = 0; x < count; x++)
                                    {
                                        if (descs.ElementAt(x).Contains("beetlex"))
                                        {
                                            desc = descs.ElementAt(x);
                                            desc = desc.Remove(0, 8);
                                            spec_shown = 1;
                                            name = (sd.Name.ToString());
                                            bprintf("-= " + "&+g" + name + "#" + " =-");
                                            length = desc.Length;
                                            bprintf(desc.TrimEnd('\r', '\n'));
                                            if (sd.death == 1)
                                            {
                                                kill();
                                            }
                                            else
                                            {
                                                show_objects(loc);
                                                show_mobiles(loc);
                                                show_directions();
                                                bprintf(prompt());
                                            }
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                catch { }
            }
            if (spec_shown == 0)
            {
                try
                {
                    var dbpath = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "zone_rooms.sqlite");
                    using (var db = new SQLite.SQLiteConnection(dbpath))
                    {
                        var e = from y in db.Table<Zone_Ploc>() select y;
                        foreach (var sd in e)
                        {
                            if (sd.ploc != null)
                            {
                                loc = sd.ploc;
                            }
                        }

                        var d = from x in db.Table<Zone_Rooms>() select x;
                        foreach (var sd in d)
                        {
                            if (sd.id == loc)
                            {
                                desc = (sd.Desc.ToString());
                                name = (sd.Name.ToString());
                                bprintf("-= " + "&+g" + name + "#" + " =-");
                                bprintf(desc);
                                if (sd.death == 1)
                                {
                                    kill();
                                }
                                else
                                {
                                    show_objects(loc);
                                    show_mobiles(loc);
                                    show_directions();
                                    bprintf(prompt());
                                }
                            }
                        }
                        db.Dispose();
                        db.Close();
                    }
                }
                catch
                {
                }
            }
        EndOfVoid:
            spec_shown = 0;
        }

        public int ploc(int plr)
        {
            var dbPath = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "zone_rooms.sqlite");
            var db = new SQLite.SQLiteConnection(dbPath);

            if (plr == 1)
            {
                using (db)
                {
                    var d = from x in db.Table<Zone_Ploc>() select x;
                    foreach (var sd in d) { return ruid(sd.ploc); }
                }
            }

            using (db)
            {
                var d = from x in db.Table<Zone_Mobiles>() select x;
                foreach (var sd in d)
                {
                    if (sd.uid == plr)
                    {
                        return sd.loc;
                    }
                }
            }
            return 0;
        }

        public int oloc(int obj)
        {
            var dbPath = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "zone_rooms.sqlite");
            var db = new SQLite.SQLiteConnection(dbPath);
            using (db)
            {
                var d = from x in db.Table<Zone_Objects>() select x;
                foreach (var sd in d)
                {
                    if (sd.uid == obj)
                    {
                        return sd.loc;
                    }
                }
            }
            return 0;
        }

        public void setoloc(string obj, string oloc)
        {
            var dbPath = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "zone_rooms.sqlite");
            var db = new SQLite.SQLiteConnection(dbPath);
            var db2 = new SQLite.SQLiteConnection(dbPath);

            using (db)
            {
                var d = from x in db.Table<Zone_Objects>() select x;
                foreach (var sd in d)
                {
                    if (sd.id == obj)
                    {
                        string loc_n = string.Empty;
                        using (db2)
                        {
                            var e = from y in db2.Table<Zone_Rooms>() select y; foreach (var sdf in e) { if (sdf.id == oloc) { update_oloc(obj, sdf.id); } }
                            var f = from z in db2.Table<Zone_Mobiles>() select z; foreach (var sdg in f) { if (sdg.pname == oloc) { update_oloc(obj, sdg.id); } }
                            var g = from a in db2.Table<Zone_Objects>() select a; foreach (var sdh in g) { if (sdh.zname == oloc) { update_oloc(obj, sdh.id); } }
                        }
                    }
                }
            }
        }

        public void setploc(int plr, int ploc)
        {
            var dbPath = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "zone_rooms.sqlite");
            var db = new SQLite.SQLiteConnection(dbPath);
            var db2 = new SQLite.SQLiteConnection(dbPath);
            using (db)
            {
                if (plr == 1)
                {
                    using (db2)
                    {
                        var e = from y in db2.Table<Zone_Rooms>() select y; foreach (var sdf in e)
                        {
                            if (sdf.uid == ploc)
                            {
                                loc = sdf.id;
                                update_ploc(sdf.id);
                                goto EoV;
                            }
                        }
                    }
                }
                var d = from x in db.Table<Zone_Mobiles>() select x;
                foreach (var sd in d)
                {
                    if (sd.uid == plr)
                    {
                        string loc_n = string.Empty;
                        using (db2)
                        {
                            var e = from y in db2.Table<Zone_Rooms>() select y; foreach (var sdf in e) { if (sdf.uid == ploc) { sd.loc = ruid(sdf.Name); } }
                        }
                    }
                }
            }
        EoV:
            mynum = 1;
        }

        public void show_objects(string loc)
        {
            var dbpath = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "zone_rooms.sqlite");
            using (var db = new SQLite.SQLiteConnection(dbpath)) 
            {
                int rloc = ruid(loc);
                var e = from y in db.Table<Zone_Objects>() where y.loc == rloc select y ;
               
                foreach (var sd in e)
                {
                    if ((sd.desc != null) && (sd.desc != string.Empty)) {bprintf(sd.desc);}
                }
            }
        }

        public void show_mobiles(string loc)
        {
            var dbpath = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "zone_rooms.sqlite");
            using (var db = new SQLite.SQLiteConnection(dbpath))
            {
                int rloc = ruid(loc);
                var e = from y in db.Table<Zone_Mobiles>() where y.loc == rloc select y;
                foreach (var sd in e)
                {
                    if (sd.desc != "")
                    {
                        string carried = string.Empty;
                        bprintf(sd.desc);
                        var f = from z in db.Table<Zone_Objects>() where z.loc == sd.uid select z;
                        foreach (var sdf in f)
                        {
                                if (carried == string.Empty) { carried = sd.pname + " is carrying: " + sdf.name; }
                                else { carried = carried + ", " + sdf.name; }
                        }
                        bprintf(carried + ".");
                    }
                }
            }

        }

        public void show_directions()
        {
            {
                string north, south, east, west, up, down;
                try
                {
                    var dbpath = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "zone_rooms.sqlite");
                    using (var db = new SQLite.SQLiteConnection(dbpath))
                    {
                        var d = from x in db.Table<Zone_Rooms>() select x;
                        foreach (var sd in d)
                        {
                            if (sd.id == loc)
                            {
                                bprintf("\nObvious exits are:");
                                if (sd.n != null)
                                {
                                    north = (sd.n.ToString());
                                    foreach (var sdn in d)
                                    {
                                        if (sdn.id == north)
                                        {
                                            bprintf("&+yNorth# :  " + "&+M" + sdn.Name + "#");
                                        }
                                    }
                                }
                                if (sd.s != null)
                                {
                                    south = (sd.s.ToString());
                                    foreach (var sdn in d)
                                    {
                                        if (sdn.id == south)
                                        {
                                            bprintf("&+ySouth# :  " + "&+M" + sdn.Name + "#");
                                        }
                                    }
                                }
                                if (sd.e != null)
                                {
                                    east = (sd.e.ToString());
                                    foreach (var sdn in d)
                                    {
                                        if (sdn.id == east)
                                        {
                                            bprintf(" &+yEast# :  " + "&+M" + sdn.Name + "#");
                                        }
                                    }
                                }
                                if (sd.w != null)
                                {
                                    west = (sd.w.ToString());
                                    foreach (var sdn in d)
                                    {
                                        if (sdn.id == west)
                                        {
                                            bprintf(" &+yWest# :  " + "&+M" + sdn.Name + "#");
                                        }
                                    }
                                }
                                if (sd.u != null)
                                {
                                    up = (sd.u.ToString());
                                    foreach (var sdn in d)
                                    {
                                        if (sdn.id == up)
                                        {
                                            bprintf("   &+yUp# :  " + "&+M" + sdn.Name + "#");
                                        }
                                    }
                                }
                                if (sd.d != null)
                                {
                                    down = (sd.d.ToString());
                                    foreach (var sdn in d)
                                    {
                                        if (sdn.id == down)
                                        {
                                            bprintf(" &+yDown# :  " + "&+M" + sdn.Name + "#");
                                        }
                                    }
                                }
                            }

                        }
                        db.Dispose();
                        db.Close();
                    }
                }
                catch
                {
                }

            }
        }

        public string prompt()
        {
            if (type() == "beetle") { return "&+YStr:# &+M5/5# >"; }
            else
            {
                return "&+YStr:# &+M999/999# >";
            }
        }

        public string update_inv()
        {
            string inv, inv_worn = "Worn: ", inv_carried = "Carried: ", inv_wielded = "Wielded: ";
            var dbpath = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "zone_rooms.sqlite");
            using (var db = new SQLite.SQLiteConnection(dbpath))
            {
                var d = from x in db.Table<Zone_Objects>() select x;
                foreach (var sd in d)
                {
                    if (sd.worn == mynum)
                    {
                        inv_worn = inv_worn + sd.name + " ";
                    }
                    if (sd.carried == mynum)
                    {
                        inv_carried = inv_carried + sd.name + " ";
                    }
                    if (sd.wielded == mynum)
                    {
                        inv_wielded = inv_wielded + sd.name + " ";
                    }
                }
            }
            inv = inv_worn + "\n" + inv_carried + "\n" + inv_wielded;
            inv_box.Text = inv_worn + "\n\n" + inv_carried + "\n\n" + inv_wielded;
            inv = inv.TrimEnd('\n', '\r');
            inv = inv.TrimStart('\n', '\r');
            return inv;
        }

        public void inventory()
        {
            string inv = update_inv();
            bprintf(inv.TrimEnd('\n', '\r'));
        }

        public void drop(string item)
        {
            if (item == null) { bprintf("Drop what?"); }

            else
            {
                var dbpath = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "zone_rooms.sqlite");
                using (var db = new SQLite.SQLiteConnection(dbpath))
                {
                    var d = from x in db.Table<Zone_Objects>() select x;
                    foreach (var sd in d)
                    {
                        if (((sd.carried == mynum) || (sd.worn == mynum) || (sd.wielded == mynum)) && (sd.name == item))
                        {
                            bprintf("You drop the " + sd.name + " to the ground.");
                            sd.loc = ruid(loc);
                            sd.carried = 0;
                            sd.worn = 0;
                            sd.wielded = 0;
                            db.Update(sd);
                            update_inv();
                        }
                    }
                }
            }

        }

        public void get(string item)
        {
            int found = 0;
            if (item == null) { bprintf("Get what?"); }
            else
            {
                var dbpath = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "zone_rooms.sqlite");
                using (var db = new SQLite.SQLiteConnection(dbpath))
                {
                    var d = from x in db.Table<Zone_Objects>() select x;
                    foreach (var sd in d)
                    {
                        if (((sd.loc == ruid(loc)) && (sd.noget == 0)) && (sd.name == item))
                        {
                            bprintf("You take the " + sd.name);
                            found = 1;
                            sd.loc = mynum;
                            sd.carried = mynum;
                            db.Update(sd);
                            update_inv();
                            break;
                        }
                        if (((sd.loc == ruid(loc)) && (sd.noget == 1)) && (sd.name == item))
                        {
                            bprintf("You cannot take that.");
                            found = 1;
                            break;
                        }
                    }
                }
            }
            if (found == 0) { bprintf("It's not here to get."); }
        }

        public void tele(string location)
        {
            int found = 0;
            if (location == null) { bprintf("Goto where?"); }
            else
            {
                var dbpath = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "zone_rooms.sqlite");
                using (var db = new SQLite.SQLiteConnection(dbpath))
                {
                    var d = from x in db.Table<Zone_Rooms>() select x;
                    foreach (var sd in d)
                    {
                        if (sd.id == location)
                        {
                            found = 1;
                            bprintf("You teleport out in a flash of light!");
                            setploc(mynum, sd.uid);
                            show_ploc();
                            break;
                        }
                    }
                }
            }
            if (found == 0) { bprintf("No such location."); }
        }

        public void wield(string item)
        {
            int found = 0;
            if (item == null) { bprintf("Wield what?"); }
            else
            {
                var dbpath = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "zone_rooms.sqlite");
                using (var db = new SQLite.SQLiteConnection(dbpath))
                {
                    var d = from x in db.Table<Zone_Objects>() select x;
                    foreach (var sd in d)
                    {
                        if ((sd.wielded == mynum) && (sd.name == item)) { found = 1; bprintf("You are already wielding it."); }
                        if (((sd.carried == mynum) || (sd.worn == mynum)) && (sd.name == item))
                        {
                            if (sd.weapon != 1)
                            {
                                bprintf("The " + sd.name + " is not a weapon.");
                                found = 1;
                                break;
                            }
                            var e = from y in db.Table<Zone_Objects>() select y;
                            foreach (var sda in e)
                            {
                                if (sda.wielded == mynum)
                                {
                                    bprintf("You stop wielding the " + sda.name);
                                    sda.wielded = 0;
                                    sda.carried = mynum;
                                    db.Update(sda);
                                    update_inv();
                                }
                            }
                            bprintf("You grasp the " + sd.name + " in your hands and prepare for battle.");
                            found = 1;
                            sd.carried = 0;
                            sd.wielded = mynum;
                            db.Update(sd);
                            update_inv();
                        }
                    }
                }
            }
            if (found == 0) { bprintf("You aren't carrying it."); }
        }

        public async void getRooms()
        {
            Windows.Storage.StorageFolder installedLocation = Windows.Storage.ApplicationData.Current.LocalFolder;
            var files = await installedLocation.GetFilesAsync(CommonFileQuery.OrderByName);
            var file = files.FirstOrDefault(x => x.Name == "meta.txt");
            if (file != null)
            {
                var dbPath2 = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "zone_rooms.sqlite");

                using (var db = new SQLite.SQLiteConnection(dbPath2))
                {
                    db.CreateTable<Zone_Ploc>();
                    db.RunInTransaction(() =>
                    {
                        db.Insert(new Zone_Ploc() { mynum = 1, id = "1", ploc = "room85", type = "human" });
                    });
                }
                loc = "room85"; //sets starting room for player
                System.Diagnostics.Debug.WriteLine("We have the file!");
                string myFile = Path.Combine(installedLocation.Path, "meta.txt");
                using (Stream stream = await installedLocation.OpenStreamForReadAsync(Path.GetFileName(myFile)))
                {
                    using (StreamReader sr = new StreamReader(stream))
                    {
                        int uid = 1;
                        string line = string.Empty;
                        int room_no = 0;
                        int obj_no = 0;
                        int lastLine = 0;
                        int objLine = 0;
                        string room_name = string.Empty;
                        string obj_name = string.Empty;
                        string room_flags = string.Empty;
                        string room_desc = string.Empty;
                        string obj_desc = string.Empty;
                        string obj_ex = string.Empty;
                        string room_id = string.Empty;
                        string obj_id = string.Empty;
                        string new_class = string.Empty;
                        int room_id_no = 0;
                        int obj_id_no = 0;
                        string room_id_name = string.Empty;
                        string obj_id_name = string.Empty;
                        int obj_loc = 0;
                        string ndir = string.Empty;
                        string sdir = string.Empty;
                        string edir = string.Empty;
                        string wdir = string.Empty;
                        string udir = string.Empty;
                        string ddir = string.Empty;
                        int obj_noget = 0, obj_iscont = 0, obj_state = 0, obj_door = 0, obj_weapon = 0;
                        int obj_carried = 0;
                        int obj_wielded = 0;
                        int obj_worn = 0;
                        int obj_incont = 0;
                        int obj_linked = 0;
                        string obj_zname = string.Empty;
                        int obj_wearable = 0;
                        int r_outdoors = 0, r_nomobiles = 0, r_death = 0, r_underwater = 0;
                        int x;
                        int mobLine = 0;
                        int mob_speed = 0;
                        string mob_pname = string.Empty;
                        int mob_no = 0;
                        int mob_loc = 0;
                        string mob_desc = string.Empty;
                        string mob_ex = string.Empty;
                        string mob_id = string.Empty;
                        int mob_id_no = 0;
                        string mob_id_name = string.Empty;
                        string mob_altname = string.Empty;
                        do
                        {
                            room_no++;
                            line = sr.ReadLine();
                            if (line != null)
                            {
                                // Room line in format id:room1 etc
                                room_id = "room" + room_no;
                                string room = "id:" + "room" + room_no;
                                if (line.StartsWith("id:"))
                                {

                                    string[] dirs = line.Split('\\');
                                    ndir = dirs.ElementAt(1).Remove(dirs.ElementAt(1).Length - 3);
                                    sdir = dirs.ElementAt(2).Remove(dirs.ElementAt(2).Length - 3);
                                    edir = dirs.ElementAt(3).Remove(dirs.ElementAt(3).Length - 3);
                                    wdir = dirs.ElementAt(4).Remove(dirs.ElementAt(4).Length - 3);
                                    udir = dirs.ElementAt(5).Remove(dirs.ElementAt(5).Length - 3);
                                    ddir = dirs.ElementAt(6);
                                }

                                if (line.StartsWith("flags:"))
                                {
                                    r_outdoors = 0;
                                    r_nomobiles = 0;
                                    r_death = 0;
                                    r_underwater = 0;
                                    int y;
                                    string all_flags = line.Remove(0, 6);
                                    if (all_flags.Length < 4)
                                    {
                                    }
                                    else
                                    {
                                        if (all_flags.Contains(" "))
                                        {
                                            string[] flags = line.Split(' ');
                                            for (y = 0; y < flags.Count(); y++)
                                            {
                                                if (flags.ElementAt(y).ToLower().Contains("outdoors")) { r_outdoors = 1; }
                                                if (flags.ElementAt(y).ToLower().Contains("nomobiles")) { r_nomobiles = 1; }
                                                if (flags.ElementAt(y).ToLower().Contains("death")) { r_death = 1; }
                                                if (flags.ElementAt(y).ToLower().Contains("underwater")) { r_underwater = 1; }
                                            }

                                        }
                                        else
                                        {
                                            if (all_flags.ToLower().Contains("outdoors")) { r_outdoors = 1; }
                                            if (all_flags.ToLower().Contains("nomobiles")) { r_nomobiles = 1; }
                                            if (all_flags.ToLower().Contains("death")) { r_death = 1; }
                                            if (all_flags.ToLower().Contains("underwater")) { r_underwater = 1; }

                                        }
                                    }
                                }
                                if (line.StartsWith("name:"))
                                {
                                    room_name = line.Remove(0, 5);
                                }
                                if (line.StartsWith("\""))
                                {
                                    room_desc = line.Remove(0, 1);
                                    lastLine = 0;
                                    do
                                    {
                                        line = sr.ReadLine();
                                        if (line.EndsWith("\""))
                                        {
                                            room_id_no++;
                                            uid++;
                                            room_id_name = "room" + room_id_no;
                                            char[] MyChar = { '"' };
                                            lastLine = 1;
                                            room_desc = room_desc + "\n" + line.TrimEnd(MyChar);
                                            var dbPath = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "zone_rooms.sqlite");
                                            using (var db = new SQLite.SQLiteConnection(dbPath))
                                            {
                                                db.CreateTable<Zone_Rooms>();
                                                db.RunInTransaction(() =>
                                                {
                                                    db.Insert(new Zone_Rooms() { uid = uid, id = room_id_name, Name = room_name, Desc = room_desc, Flags = "flags", n = ndir, s = sdir, e = edir, w = wdir, u = udir, d = ddir, outdoors = r_outdoors, nomobiles = r_nomobiles, death = r_death, underwater = r_underwater });
                                                });
                                            }
                                        }
                                        else
                                        {
                                            room_desc = room_desc + "\n" + line;
                                        }
                                    } while (lastLine == 0);

                                }
                            }
                        }

                        while (line != "MOBILES");
                        if (line == "MOBILES")
                        {
                            do
                            {
                                line = sr.ReadLine();
                                mob_no++;
                                mob_ex = string.Empty;
                                if (line != null)
                                {
                                    // Mob line in format id:name etc
                                    string mob = "id:" + "mob" + mob_no;

                                    if (line.StartsWith("id:")) { mob_id = line.Remove(0, 3); }
                                    if (line.StartsWith("pname:")) { mob_pname = line.Remove(0, 6); }
                                    if (line.StartsWith("altname:")) { mob_altname = line.Remove(0, 8); }
                                    if (line.StartsWith("speed:")) { mob_speed = Convert.ToInt32(line.Remove(0, 6)); }
                                    if (line.StartsWith("loc:")) { mob_loc = ruid(line.Remove(0, 4)); }
                                    if (line.StartsWith("desc:")) { mob_desc = line.Remove(0, 5); }
                                    if (line.StartsWith("ex:"))
                                    {
                                        mobLine = 0;
                                        do
                                        {
                                            line = sr.ReadLine();
                                            if (line.EndsWith("/"))
                                            {
                                                mob_id_no++;
                                                uid++;
                                                mob_id_name = "mob" + mob_id_no;
                                                char[] MyChar = { '/' };
                                                mobLine = 1;
                                                if (mob_ex != "")
                                                {
                                                    mob_ex = mob_ex + "\n" + line.TrimEnd(MyChar);
                                                }
                                                else { mob_ex = line.TrimEnd(MyChar); }
                                                var dbPath = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "zone_rooms.sqlite");
                                                using (var db = new SQLite.SQLiteConnection(dbPath))
                                                {
                                                    db.CreateTable<Zone_Mobiles>();
                                                    db.RunInTransaction(() =>
                                                    {
                                                        db.Insert(new Zone_Mobiles() { uid = uid, id = mob_id, pname = mob_pname, altname = mob_altname, loc = mob_loc, desc = mob_desc, ex = mob_ex, speed = mob_speed });
                                                    });
                                                    mob_id = string.Empty;
                                                    mob_pname = string.Empty;
                                                    mob_loc = 0;
                                                    mob_desc = string.Empty;
                                                    mob_ex = string.Empty;
                                                    mob_speed = 0;
                                                }
                                            }
                                            else
                                            {
                                                if (mob_ex != "")
                                                {
                                                    mob_ex = mob_ex + "\n" + line;
                                                }
                                                else { mob_ex = line; }
                                            }
                                        } while (mobLine == 0);

                                    }
                                }
                            }
                            while (line != "OBJECTS");
                        }
                        if (line == "OBJECTS")
                        {
                            do
                            {
                                line = sr.ReadLine();
                                obj_no++;
                                obj_ex = string.Empty;
                                if (line != null)
                                {
                                    // Obj line in format id:obj1 etc
                                    obj_id = "obj" + obj_no;
                                    string obj = "id:" + "obj" + obj_no;

                                    if (line.StartsWith("id:")) { obj_zname = line.Remove(0, 3); }

                                    if (line.StartsWith("flags:"))
                                    {
                                        string all_flags = line.Remove(0, 6);
                                        string[] flags = all_flags.Split(' ');
                                        int flag_no = flags.Length;
                                        for (x = 0; x <= flag_no - 1; x++)
                                        {
                                            if (flags.ElementAt(x) == "noget") { obj_noget = 1; }
                                            if (flags.ElementAt(x) == "iscont") { obj_iscont = 1; }
                                            if (flags.ElementAt(x) == "door") { obj_door = 1; }
                                            if (flags.ElementAt(x) == "weapon") { obj_weapon = 1; }
                                            if (flags.ElementAt(x) == "wearable") { obj_wearable = 1; }
                                        }
                                    }
                                    if (line.StartsWith("loc:"))
                                    {
                                        if (ruid(line.Remove(0, 4)) > 0) { obj_loc = ruid(line.Remove(0, 4)); }
                                        if (muid(line.Remove(0, 4)) > 0) { obj_loc = muid(line.Remove(0, 4)); }
                                        if (ouid(line.Remove(0, 4)) > 0) { obj_loc = ouid(line.Remove(0, 4)); }
                                    }
                                    if (line.StartsWith("wielded:")) { obj_wielded = muid(line.Remove(0, 8)); obj_loc = muid(line.Remove(0, 8)); }
                                    if (line.StartsWith("name:")) { obj_name = line.Remove(0, 5); }
                                    if (line.StartsWith("desc:")) { obj_desc = line.Remove(0, 5); }
                                    if (line.StartsWith("worn:")) { obj_worn = muid(line.Remove(0, 5)); obj_loc = muid(line.Remove(0, 5)); }
                                    if (line.StartsWith("carried:")) { obj_carried = muid(line.Remove(0, 8)); obj_loc = muid(line.Remove(0, 8)); }
                                    if (line.StartsWith("ex:"))
                                    {
                                        objLine = 0;
                                        do
                                        {
                                            line = sr.ReadLine();
                                            if (line.EndsWith("/"))
                                            {
                                                obj_id_no++;
                                                uid++;
                                                obj_id_name = "obj" + obj_id_no;
                                                char[] MyChar = { '/' };
                                                objLine = 1;
                                                if (obj_ex != "")
                                                {
                                                    obj_ex = obj_ex + "\n" + line.TrimEnd(MyChar);
                                                }
                                                else { obj_ex = line.TrimEnd(MyChar); }
                                                var dbPath = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "zone_rooms.sqlite");
                                                using (var db = new SQLite.SQLiteConnection(dbPath))
                                                {
                                                    db.CreateTable<Zone_Objects>();
                                                    db.RunInTransaction(() =>
                                                    {
                                                        db.Insert(new Zone_Objects() { uid = uid, id = obj_id_name, zname = obj_zname, loc = obj_loc, name = obj_name, desc = obj_desc, ex = obj_ex, noget = obj_noget, iscont = obj_iscont, door = obj_door, carried = obj_carried, wielded = obj_wielded, worn = obj_worn, incont = obj_incont, linked = obj_linked, state = obj_state, weapon = obj_weapon, wearable = obj_wearable });
                                                    });
                                                    obj_id_name = string.Empty;
                                                    obj_zname = string.Empty;
                                                    obj_loc = 0;
                                                    obj_name = string.Empty;
                                                    obj_desc = string.Empty;
                                                    obj_ex = string.Empty;
                                                    obj_noget = 0;
                                                    obj_iscont = 0;
                                                    obj_door = 0;
                                                    obj_carried = 0;
                                                    obj_wielded = 0;
                                                    obj_worn = 0;
                                                    obj_incont = 0;
                                                    obj_linked = 0;
                                                    obj_state = 0;
                                                    obj_weapon = 0;
                                                    obj_wearable = 0;
                                                }
                                            }
                                            else
                                            {
                                                if (obj_ex != "")
                                                {
                                                    obj_ex = obj_ex + "\n" + line;
                                                }
                                                else { obj_ex = line; }
                                            }
                                        } while (objLine == 0);

                                    }
                                }
                            }
                            while (line != null);
                        }
                    }
                }
                show_ploc();
                update_inv();

                bprintf("If you are new to text adventures, type 'help' for some commands to get started.");
            }
            else
            {
                System.Diagnostics.Debug.WriteLine("No File. Use default.");
            }

        }

        public int ruid(string room)
        {
            var dbpath = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "zone_rooms.sqlite");
            using (var db = new SQLite.SQLiteConnection(dbpath))
            {
                var d = from x in db.Table<Zone_Rooms>() select x;
                foreach (var sd in d)
                {
                    if (sd.id == room) { return sd.uid; }
                }
            }
            return 0;
        }

        public int muid(string plr)
        {
            if (plr == "player")
            {
                return 1;
            }

            var dbpath = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "zone_rooms.sqlite");
            using (var db = new SQLite.SQLiteConnection(dbpath))
            {
                var d = from x in db.Table<Zone_Mobiles>() select x;
                foreach (var sd in d)
                {
                    if (sd.id == plr) { return sd.uid; }
                }
            }
            return 0;
        }

        public int ouid(string obj)
        {
            var dbpath = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "zone_rooms.sqlite");
            using (var db = new SQLite.SQLiteConnection(dbpath))
            {
                var d = from x in db.Table<Zone_Objects>() select x;
                foreach (var sd in d)
                {
                    if (sd.id == obj) { return sd.uid; }
                }
            }
            return 0;
        }

        public void pray()
        {
            if (loc == "room86")
            {
                bprintf("You &+Ckneel# humbly to the ground in the centre of the &+Mt#&+Wo#&+Ma#&+Wd#&+Ms#&+Wt#&+Mo#&+Wo#&+Ml# ring to pray.");
                bprintf("As you do so, the &+Bskies# darken to a pure jet black, giving way to a &+Rterrible# booming voice...");
                bprintf("'&+WAnother insect come to test his mettle against me! Pathetic. All your weapons and armours are nothing to one such as me!#'\n");
                bprintf("A bolt of &+Ylightning# arcs from the &+Csky#, striking you square in the forehead, sending you reeling across the &+gground#.");
                bprintf("\nJust before you lose consciousness, a voice whispers as though from within your own head...");
                bprintf("'&+WGoodbye, little insect. I think we shall not meet again.#'");
                setploc(mynum, ruid("room72"));
                limbo_all();
                ch_type("beetle");
            }
            else { bprintf("You say a silent prayer to the old gods and the new."); }
        }

        public void limbo_all()
        {
            var dbpath = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "zone_rooms.sqlite");
            using (var db = new SQLite.SQLiteConnection(dbpath))
            {
                var d = from x in db.Table<Zone_Objects>() select x;
                foreach (var sd in d)
                {
                    if ((sd.carried == mynum) || (sd.wielded == mynum) || (sd.worn == mynum))
                    {
                        sd.carried = 0;
                        sd.wielded = 0;
                        sd.worn = 0;
                        sd.loc = ruid("room74");
                        db.Update(sd);
                    }
                }
            }
        }

        private void tap_n(object sender, RoutedEventArgs e)
        {
            can_go("n");
        }

        private void tap_s(object sender, RoutedEventArgs e)
        {
            can_go("s");
        }

        private void tap_e(object sender, RoutedEventArgs e)
        {
            can_go("e");
        }

        private void tap_u(object sender, RoutedEventArgs e)
        {
            can_go("u");
        }

        private void tap_d(object sender, RoutedEventArgs e)
        {
            can_go("d");
        }

        private void getRooms(object sender, RoutedEventArgs e)
        {
            border.BorderBrush = new SolidColorBrush(Windows.UI.Color.FromArgb(0xff, 0x00, 0x00, 0x00));
            border.Background = new SolidColorBrush(Windows.UI.Color.FromArgb(0x00, 0x00, 0x00, 0x00));
            routputWindow.Blocks.Clear();
            reset();
        }

        private void tap_w(object sender, RoutedEventArgs e)
        {
            can_go("w");
        }

        private void soundsToggle_Toggled(object sender, RoutedEventArgs e)
        {

        }
    }
}