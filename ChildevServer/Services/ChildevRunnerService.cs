using ChildevServer.Dtos;
using System.Diagnostics;
using System.Text;

namespace ChildevServer.Services;

public class ChildevRunnerService
{
    private readonly IConfiguration _configuration;
    private readonly ILogger<ChildevRunnerService> _logger;
    private readonly ChildevProcessService _processService;

    public ChildevRunnerService(IConfiguration configuration, ILogger<ChildevRunnerService> logger, ChildevProcessService processService)
    {
        _configuration = configuration;
        _logger = logger;
        _processService = processService;
    }
    public async Task<CheckCodeResponse> RunAsync(string code)
    {
        var tempFolder = _configuration["Childev:TempFolder"] ?? "temp";
        Directory.CreateDirectory(tempFolder);

        var inputFilePath = Path.Combine(
            tempFolder,
            $"childev_{Guid.NewGuid():N}.txt"
        );

        try
        {
            var utf8WithoutBom = new UTF8Encoding(false);
            code = code.TrimStart('\uFEFF');

            await File.WriteAllTextAsync(inputFilePath, code, utf8WithoutBom);

            // חשוב: לשלוח ל-C++ נתיב מוחלט
            var fullPath = Path.GetFullPath(inputFilePath);

            var output = await _processService.AnalyzeFileAsync(fullPath);

            return ParseOutput(output, "");
        }
        catch (Exception ex)
        {
            return new CheckCodeResponse
            {
                IsSuccess = false,
                Errors = new List<CodeErrorDto>
            {
                new CodeErrorDto
                {
                    Line = 0,
                    Column = 0,
                    Type = "Server",
                    Message = "שגיאה בהרצת ניתוח הקוד: " + ex.Message
                }
            }
            };
        }
        finally
        {
            try
            {
                if (File.Exists(inputFilePath))
                {
                    File.Delete(inputFilePath);
                }
            }
            catch
            {
                // לא קריטי
            }
        }
    }
    public async Task<CheckCodeResponse> RunAsync_____Remove(string code)
    {
        string exePath = _configuration["Childev:ExePath"] ?? "Childev.exe";
        string tempFolder = _configuration["Childev:TempFolder"] ?? "temp";
        int timeoutSeconds = int.TryParse(_configuration["Childev:TimeoutSeconds"], out int seconds)
            ? seconds
            : 10;

        Directory.CreateDirectory(tempFolder);

        string inputFilePath = Path.Combine(tempFolder, $"childev_{Guid.NewGuid():N}.txt");

        try
        {
            //await File.WriteAllTextAsync(inputFilePath, code, Encoding.UTF8);
            var utf8WithoutBom = new UTF8Encoding(encoderShouldEmitUTF8Identifier: false);

            code = code.TrimStart('\uFEFF');

            await File.WriteAllTextAsync(inputFilePath, code, utf8WithoutBom);
            var startInfo = new ProcessStartInfo
            {
                FileName = exePath,
                UseShellExecute = false,
                RedirectStandardOutput = true,
                RedirectStandardError = true,
                CreateNoWindow = true,
                StandardOutputEncoding = Encoding.UTF8,
                StandardErrorEncoding = Encoding.UTF8
            };

            startInfo.ArgumentList.Add(inputFilePath);

            using var process = new Process { StartInfo = startInfo };

            if (!process.Start())
            {
                return CreateServerError("לא ניתן להפעיל את תהליך ניתוח הקוד");
            }

            Task<string> outputTask = process.StandardOutput.ReadToEndAsync();
            Task<string> errorTask = process.StandardError.ReadToEndAsync();

            using var timeout = new CancellationTokenSource(TimeSpan.FromSeconds(timeoutSeconds));

            try
            {
                await process.WaitForExitAsync(timeout.Token);
            }
            catch (OperationCanceledException)
            {
                TryKill(process);
                return CreateServerError("זמן הרצת ניתוח הקוד הסתיים. ייתכן שהתהליך נתקע");
            }

            string output = await outputTask;
            string stderr = await errorTask;

            if (process.ExitCode != 0 && string.IsNullOrWhiteSpace(output))
            {
                _logger.LogError("Childev process failed. ExitCode={ExitCode}, Error={Error}", process.ExitCode, stderr);
                return CreateServerError(string.IsNullOrWhiteSpace(stderr)
                    ? "תהליך ניתוח הקוד הסתיים בשגיאה"
                    : stderr.Trim());
            }

            return ParseOutput(output, stderr);
        }
        catch (Exception ex)
        {
            _logger.LogError(ex, "Failed to run Childev analyzer");
            return CreateServerError($"שגיאה בהרצת ניתוח הקוד: {ex.Message}");
        }
        finally
        {
            TryDelete(inputFilePath);
        }
    }

    private static CheckCodeResponse ParseOutput(string output, string stderr)
    {
        var response = new CheckCodeResponse();

        var lines = output
            .Split(new[] { "\r\n", "\n" }, StringSplitOptions.RemoveEmptyEntries)
            .Select(line => line.Trim())
            .Where(line => !string.IsNullOrWhiteSpace(line))
            .ToList();

        if (lines.Count == 0)
        {
            string message = string.IsNullOrWhiteSpace(stderr)
                ? "לא התקבל פלט מתהליך ניתוח הקוד"
                : stderr.Trim();

            return CreateServerError(message);
        }

        foreach (string line in lines)
        {
            if (line == "OK")
            {
                continue;
            }

            if (line.StartsWith("ERROR|"))
            {
                string[] parts = line.Split('|', 5);

                if (parts.Length == 5 &&
                    int.TryParse(parts[1], out int lineNumber) &&
                    int.TryParse(parts[2], out int columnNumber))
                {
                    response.Errors.Add(new CodeErrorDto
                    {
                        Line = lineNumber,
                        Column = columnNumber,
                        Type = parts[3],
                        Message = parts[4]
                    });
                }
                else
                {
                    response.Errors.Add(new CodeErrorDto
                    {
                        Line = 0,
                        Column = 0,
                        Type = "Output",
                        Message = $"פלט שגיאה לא תקין: {line}"
                    });
                }
            }
            else
            {
                response.Errors.Add(new CodeErrorDto
                {
                    Line = 0,
                    Column = 0,
                    Type = "Output",
                    Message = $"פלט לא מזוהה מתהליך הניתוח: {line}"
                });
            }
        }

        response.IsSuccess = response.Errors.Count == 0;
        return response;
    }

    private static CheckCodeResponse CreateServerError(string message)
    {
        return new CheckCodeResponse
        {
            IsSuccess = false,
            Errors = new List<CodeErrorDto>
            {
                new()
                {
                    Line = 0,
                    Column = 0,
                    Type = "Server",
                    Message = message
                }
            }
        };
    }

    private static void TryKill(Process process)
    {
        try
        {
            if (!process.HasExited)
            {
                process.Kill(entireProcessTree: true);
            }
        }
        catch
        {
            // Best effort only.
        }
    }

    private static void TryDelete(string filePath)
    {
        try
        {
            if (File.Exists(filePath))
            {
                File.Delete(filePath);
            }
        }
        catch
        {
            // Best effort only.
        }
    }
}
