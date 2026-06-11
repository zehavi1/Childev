using System.ComponentModel.DataAnnotations;

namespace ChildevServer.Models;

public class CodeExample
{
    public int Id { get; set; }

    [Required]
    [MaxLength(100)]
    public string Title { get; set; } = string.Empty;

    [Required]
    public string Description { get; set; } = string.Empty;

    [Required]
    public string Code { get; set; } = string.Empty;
}
