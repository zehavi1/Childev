using System.ComponentModel.DataAnnotations;

namespace ChildevServer.Models;

public class CodeRun
{
    public int Id { get; set; }

    public int UserId { get; set; }
    public User? User { get; set; }

    [Required]
    public string Code { get; set; } = string.Empty;

    public DateTime CreatedAt { get; set; } = DateTime.UtcNow;

    public bool IsSuccess { get; set; }

    public List<CodeError> Errors { get; set; } = new();
}
