namespace Metamorphosis.Core.Models;

public enum AnimalForm
{
    Human,
    Bird,
    Snake,
    Beetle,
    Fish
}

public static class AnimalFormExtensions
{
    public static AnimalForm FromString(string s) => s.ToLower() switch
    {
        "bird" => AnimalForm.Bird,
        "snake" => AnimalForm.Snake,
        "beetle" => AnimalForm.Beetle,
        "fish" => AnimalForm.Fish,
        _ => AnimalForm.Human
    };

    public static string ToFormString(this AnimalForm form) => form switch
    {
        AnimalForm.Bird => "bird",
        AnimalForm.Snake => "snake",
        AnimalForm.Beetle => "beetle",
        AnimalForm.Fish => "fish",
        _ => "human"
    };

    public static string GetDescriptionMarker(this AnimalForm form) => form switch
    {
        AnimalForm.Bird => "birdx",
        AnimalForm.Snake => "snakex",
        AnimalForm.Beetle => "beetlex",
        AnimalForm.Fish => "fishx",
        _ => ""
    };
}
