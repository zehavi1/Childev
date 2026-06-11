using System.ComponentModel.DataAnnotations;

namespace ChildevServer.Dtos;

public class CheckCodeRequest
{
    [Required]
    public int UserId { get; set; }

    [Required]
    public string Code { get; set; } = string.Empty;
}

public class CodeErrorDto
{
    public int Line { get; set; }
    public int Column { get; set; }
    public string Type { get; set; } = string.Empty;
    public string Message { get; set; } = string.Empty;
}

public class CheckCodeResponse
{
    public bool IsSuccess { get; set; }
    public int? RunId { get; set; }
    public List<CodeErrorDto> Errors { get; set; } = new();
}

public class CodeRunSummaryDto
{
    public int Id { get; set; }
    public DateTime CreatedAt { get; set; }
    public bool IsSuccess { get; set; }
    public int ErrorsCount { get; set; }
}

public class CodeRunDetailsDto
{
    public int Id { get; set; }
    public DateTime CreatedAt { get; set; }
    public bool IsSuccess { get; set; }
    public string Code { get; set; } = string.Empty;
    public List<CodeErrorDto> Errors { get; set; } = new();
}
