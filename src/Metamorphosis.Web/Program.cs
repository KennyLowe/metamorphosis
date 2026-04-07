using Microsoft.AspNetCore.Components.Web;
using Microsoft.AspNetCore.Components.WebAssembly.Hosting;
using Metamorphosis.Web;
using Metamorphosis.Core.Data;
using Metamorphosis.Core.GameEngine;

var builder = WebAssemblyHostBuilder.CreateDefault(args);
builder.RootComponents.Add<App>("#app");
builder.RootComponents.Add<HeadOutlet>("head::after");

builder.Services.AddSingleton<MetaTxtParser>();
builder.Services.AddSingleton<IGameDataStore, InMemoryGameDataStore>();
builder.Services.AddSingleton<GameEngine>();

builder.Services.AddLogging();

await builder.Build().RunAsync();
