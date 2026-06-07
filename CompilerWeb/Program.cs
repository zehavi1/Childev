using System.Diagnostics;

var builder = WebApplication.CreateBuilder(args);
var app = builder.Build();

app.UseDefaultFiles();
app.UseStaticFiles();

app.MapPost("/api/compile", async (CompileRequest request) =>
{
    string tempFile = Path.Combine(Path.GetTempPath(), "input.childev");
    await File.WriteAllTextAsync(tempFile, request.Code ?? "");

    string compilerPath = Path.Combine(AppContext.BaseDirectory, "Compiler", "ChildevCompiler.exe");

    var process = new Process();
    process.StartInfo.FileName = compilerPath;
    process.StartInfo.Arguments = $"\"{tempFile}\"";
    process.StartInfo.RedirectStandardOutput = true;
    process.StartInfo.RedirectStandardError = true;
    process.StartInfo.UseShellExecute = false;
    process.StartInfo.CreateNoWindow = true;

    process.Start();

    string output = await process.StandardOutput.ReadToEndAsync();
    string errorOutput = await process.StandardError.ReadToEndAsync();

    await process.WaitForExitAsync();

    var errors = ParseErrors(output + "\n" + errorOutput);

    return Results.Ok(new CompileResponse(errors));
});

app.Run();

static List<CompileError> ParseErrors(string output)
{
    var errors = new List<CompileError>();

    foreach (var line in output.Split('\n'))
    {
        if (!line.StartsWith("ERROR|"))
            continue;

        var parts = line.Trim().Split('|');

        if (parts.Length < 5)
            continue;

        errors.Add(new CompileError(
            Line: int.Parse(parts[1]),
            Column: int.Parse(parts[2]),
            Type: parts[3],
            Message: parts[4]
        ));
    }

    return errors;
}

record CompileRequest(string Code);

record CompileResponse(List<CompileError> Errors);

record CompileError(
    int Line,
    int Column,
    string Type,
    string Message
);