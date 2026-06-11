using ChildevServer.Data;
using ChildevServer.Dtos;
using ChildevServer.Models;
using ChildevServer.Services;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;

namespace ChildevServer.Controllers;

[ApiController]
[Route("api/[controller]")]
public class ChildevController : ControllerBase
{
    private readonly AppDbContext _db;
    private readonly ChildevRunnerService _runner;

    public ChildevController(AppDbContext db, ChildevRunnerService runner)
    {
        _db = db;
        _runner = runner;
    }

    [HttpPost("check")]
    public async Task<ActionResult<CheckCodeResponse>> Check(CheckCodeRequest request)
    {
        if (request.UserId <= 0)
        {
            return BadRequest(new CheckCodeResponse
            {
                IsSuccess = false,
                Errors = new List<CodeErrorDto>
                {
                    new() { Line = 0, Column = 0, Type = "Validation", Message = "מזהה משתמש אינו תקין" }
                }
            });
        }

        bool userExists = await _db.Users.AnyAsync(u => u.Id == request.UserId);
        if (!userExists)
        {
            return NotFound(new CheckCodeResponse
            {
                IsSuccess = false,
                Errors = new List<CodeErrorDto>
                {
                    new() { Line = 0, Column = 0, Type = "User", Message = "המשתמש לא נמצא במערכת" }
                }
            });
        }

        var result = await _runner.RunAsync(request.Code);

        var run = new CodeRun
        {
            UserId = request.UserId,
            Code = request.Code,
            CreatedAt = DateTime.UtcNow,
            IsSuccess = result.IsSuccess,
            Errors = result.Errors.Select(e => new CodeError
            {
                Line = e.Line,
                Column = e.Column,
                Type = e.Type,
                Message = e.Message
            }).ToList()
        };

        _db.CodeRuns.Add(run);
        await _db.SaveChangesAsync();

        result.RunId = run.Id;
        return Ok(result);
    }

    [HttpGet("runs/{userId:int}")]
    public async Task<ActionResult<List<CodeRunSummaryDto>>> GetRuns(int userId)
    {
        bool userExists = await _db.Users.AnyAsync(u => u.Id == userId);
        if (!userExists)
        {
            return NotFound("המשתמש לא נמצא במערכת");
        }

        var runs = await _db.CodeRuns
            .Where(r => r.UserId == userId)
            .OrderByDescending(r => r.CreatedAt)
            .Select(r => new CodeRunSummaryDto
            {
                Id = r.Id,
                CreatedAt = r.CreatedAt,
                IsSuccess = r.IsSuccess,
                ErrorsCount = r.Errors.Count
            })
            .ToListAsync();

        return Ok(runs);
    }

    [HttpGet("runs/details/{runId:int}")]
    public async Task<ActionResult<CodeRunDetailsDto>> GetRunDetails(int runId)
    {
        var run = await _db.CodeRuns
            .Include(r => r.Errors)
            .FirstOrDefaultAsync(r => r.Id == runId);

        if (run is null)
        {
            return NotFound("ההרצה לא נמצאה במערכת");
        }

        return Ok(new CodeRunDetailsDto
        {
            Id = run.Id,
            CreatedAt = run.CreatedAt,
            IsSuccess = run.IsSuccess,
            Code = run.Code,
            Errors = run.Errors.Select(e => new CodeErrorDto
            {
                Line = e.Line,
                Column = e.Column,
                Type = e.Type,
                Message = e.Message
            }).ToList()
        });
    }
}
