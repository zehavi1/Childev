using System.Diagnostics;
using System.Text;

namespace ChildevServer.Services;

public class ChildevProcessService : IHostedService, IDisposable
{
    private readonly IConfiguration _configuration;
    private readonly SemaphoreSlim _lock = new(1, 1);
    private Process? _process;

    public ChildevProcessService(IConfiguration configuration)
    {
        _configuration = configuration;
    }

    public Task StartAsync(CancellationToken cancellationToken)
    {
        StartProcess();
        return Task.CompletedTask;
    }

    private void StartProcess()
    {
        var exePath = _configuration["Childev:ExePath"] ?? "Childev.exe";

        var utf8WithoutBom = new UTF8Encoding(encoderShouldEmitUTF8Identifier: false);

        var startInfo = new ProcessStartInfo
        {
            FileName = exePath,
            Arguments = "--server",
            UseShellExecute = false,
            RedirectStandardInput = true,
            RedirectStandardOutput = true,
            RedirectStandardError = true,
            CreateNoWindow = true,
            StandardInputEncoding = utf8WithoutBom,
            StandardOutputEncoding = utf8WithoutBom,
            StandardErrorEncoding = utf8WithoutBom
        };

        _process = new Process
        {
            StartInfo = startInfo,
            EnableRaisingEvents = true
        };

        _process.Start();

        // כדי שלא ייתקע אם יש הדפסות ל-StandardError
        _ = Task.Run(async () =>
        {
            while (_process != null && !_process.HasExited)
            {
                var line = await _process.StandardError.ReadLineAsync();

                if (line == null)
                {
                    break;
                }

                Console.WriteLine("[Childev stderr] " + line);
            }
        });
    }

    public async Task<string> AnalyzeFileAsync(string inputFilePath)
    {
        if (_process == null || _process.HasExited)
        {
            StartProcess();
        }

        if (_process == null)
        {
            throw new Exception("Childev process is not running");
        }

        await _lock.WaitAsync();

        try
        {
            await _process.StandardInput.WriteLineAsync(inputFilePath);
            await _process.StandardInput.FlushAsync();

            var resultLines = new List<string>();
            var insideResult = false;

            while (true)
            {
                var line = await _process.StandardOutput.ReadLineAsync();

                if (line == null)
                {
                    throw new Exception("Childev process closed unexpectedly");
                }

                if (line == "BEGIN_RESULT")
                {
                    insideResult = true;
                    continue;
                }

                if (line == "END_RESULT")
                {
                    break;
                }

                if (insideResult)
                {
                    resultLines.Add(line);
                }
            }

            return string.Join(Environment.NewLine, resultLines);
        }
        finally
        {
            _lock.Release();
        }
    }

    public async Task StopAsync(CancellationToken cancellationToken)
    {
        if (_process != null && !_process.HasExited)
        {
            try
            {
                await _process.StandardInput.WriteLineAsync("__EXIT__");
                await _process.StandardInput.FlushAsync();

                if (!_process.WaitForExit(2000))
                {
                    _process.Kill(entireProcessTree: true);
                }
            }
            catch
            {
                try
                {
                    _process.Kill(entireProcessTree: true);
                }
                catch
                {
                    // לא לעצור את סגירת השרת בגלל זה
                }
            }
        }
    }

    public void Dispose()
    {
        _lock.Dispose();
        _process?.Dispose();
    }
}