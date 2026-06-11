using System.ComponentModel.DataAnnotations;

namespace ChildevServer.Models;

public class User
{
    public int Id { get; set; }

    [Required]
    [MaxLength(50)]
    public string UserName { get; set; } = string.Empty;

    [Required]
    public string HashPassword { get; set; } = string.Empty;

    [Required]
    [MaxLength(100)]
    public string DisplayName { get; set; } = string.Empty;

    public List<CodeRun> CodeRuns { get; set; } = new();
}
