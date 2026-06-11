using ChildevServer.Data;
using ChildevServer.Dtos;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;

namespace ChildevServer.Controllers;

[ApiController]
[Route("api/[controller]")]
public class ExamplesController : ControllerBase
{
    private readonly AppDbContext _db;

    public ExamplesController(AppDbContext db)
    {
        _db = db;
    }

    [HttpGet]
    public async Task<ActionResult<List<CodeExampleDto>>> GetExamples()
    {
        var examples = await _db.CodeExamples
            .OrderBy(e => e.Id)
            .Select(e => new CodeExampleDto
            {
                Id = e.Id,
                Title = e.Title,
                Description = e.Description,
                Code = e.Code
            })
            .ToListAsync();

        return Ok(examples);
    }
}
