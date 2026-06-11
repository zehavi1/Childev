using ChildevServer.Data;
using ChildevServer.Dtos;
using ChildevServer.Models;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;

namespace ChildevServer.Controllers;

[ApiController]
[Route("api/[controller]")]
public class AuthController : ControllerBase
{
    private readonly AppDbContext _db;
    private readonly IPasswordHasher<User> _passwordHasher;

    public AuthController(AppDbContext db, IPasswordHasher<User> passwordHasher)
    {
        _db = db;
        _passwordHasher = passwordHasher;
    }

    [HttpPost("register")]
    public async Task<ActionResult<AuthResponse>> Register(RegisterRequest request)
    {
        request.UserName = request.UserName.Trim();
        request.DisplayName = request.DisplayName.Trim();

        if (string.IsNullOrWhiteSpace(request.UserName) ||
            string.IsNullOrWhiteSpace(request.Password) ||
            string.IsNullOrWhiteSpace(request.DisplayName))
        {
            return BadRequest(new AuthResponse
            {
                Success = false,
                Message = "יש למלא שם משתמש, סיסמה ושם לתצוגה"
            });
        }

        bool exists = await _db.Users.AnyAsync(u => u.UserName == request.UserName);
        if (exists)
        {
            return Conflict(new AuthResponse
            {
                Success = false,
                Message = "שם המשתמש כבר קיים במערכת"
            });
        }

        var user = new User
        {
            UserName = request.UserName,
            DisplayName = request.DisplayName
        };

        user.HashPassword = _passwordHasher.HashPassword(user, request.Password);

        _db.Users.Add(user);
        await _db.SaveChangesAsync();

        return Ok(new AuthResponse
        {
            Success = true,
            Message = "המשתמש נוצר בהצלחה",
            User = ToDto(user)
        });
    }

    [HttpPost("login")]
    public async Task<ActionResult<AuthResponse>> Login(LoginRequest request)
    {
        request.UserName = request.UserName.Trim();

        var user = await _db.Users.FirstOrDefaultAsync(u => u.UserName == request.UserName);
        if (user is null)
        {
            return Unauthorized(new AuthResponse
            {
                Success = false,
                Message = "שם המשתמש או הסיסמה אינם נכונים"
            });
        }

        var result = _passwordHasher.VerifyHashedPassword(user, user.HashPassword, request.Password);
        if (result == PasswordVerificationResult.Failed)
        {
            return Unauthorized(new AuthResponse
            {
                Success = false,
                Message = "שם המשתמש או הסיסמה אינם נכונים"
            });
        }

        return Ok(new AuthResponse
        {
            Success = true,
            Message = "התחברת בהצלחה",
            User = ToDto(user)
        });
    }

    private static UserDto ToDto(User user)
    {
        return new UserDto
        {
            Id = user.Id,
            UserName = user.UserName,
            DisplayName = user.DisplayName
        };
    }
}
