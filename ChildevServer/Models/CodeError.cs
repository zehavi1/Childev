using System.ComponentModel.DataAnnotations;

namespace ChildevServer.Models;

public class CodeError
{
    public int Id { get; set; }

    public int CodeRunId { get; set; }
    public CodeRun? CodeRun { get; set; }

    public int Line { get; set; }
    public int Column { get; set; }

    [Required]
    [MaxLength(50)]
    public string Type { get; set; } = string.Empty;

    [Required]
    public string Message { get; set; } = string.Empty;
}
