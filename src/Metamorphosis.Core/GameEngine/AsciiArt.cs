using Metamorphosis.Core.Models;

namespace Metamorphosis.Core.GameEngine;

public static class AsciiArt
{
    public static string GetArt(AnimalForm form) => form switch
    {
        AnimalForm.Beetle =>
            "\n\n\n" +
            "              ,_    /) (\\    _,\n" +
            "               >>  <<,_,>>  <<\n" +
            "              //   _0.-.0_   \\\\\n" +
            "              \\'._/       \\_./'\n" +
            "               '-.\\.--.--./.-'\n" +
            "               __/ : :Y: : \\ _\n" +
            "       ';,  .-(_| : : | : : |_)-.  ,:'\n" +
            "         \\\\/.'  |: : :|: : :|  `.\\//" +
            "\n          (/    |: : :|: : :|    \\)\n" +
            "                |: : :|: : :;\n" +
            "               /\\ : : | : : /\\\n" +
            "              (_/'.: :.: :.'\\_)\n" +
            "               \\\\  `\"\"`\"\"`  //\n" +
            "                \\\\         //\n" +
            "                 ':.     .:'",
        _ =>
            "   |\\             //\n" +
            "    \\\\           _!_\n" +
            "     \\\\         /___\\\n" +
            "      \\\\        [+++]\n" +
            "       \\\\    _ _\\^^^/_ _\n" +
            "        \\\\/ (    '-'  ( )\n" +
            "        /( \\/ | {&}   /\\ \\\n" +
            "          \\  / \\     / _> )\n" +
            "           \\\"`   >:::;-'`\"\"'-.\n" +
            "               /:::/         \\\n" +
            "              /  /||   {&}   |\n" +
            "             (  / (\\        /\n" +
            "             / /   \\'-.___.-'\n" +
            "           _/ /     \\ \\\n" +
            "          /___|    /___|"
    };

    public static int GetFontSize(AnimalForm form) => form switch
    {
        AnimalForm.Beetle => 8,
        _ => 11
    };
}
