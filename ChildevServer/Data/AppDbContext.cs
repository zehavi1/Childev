using ChildevServer.Models;
using Microsoft.EntityFrameworkCore;

namespace ChildevServer.Data;

public class AppDbContext : DbContext
{
    public AppDbContext(DbContextOptions<AppDbContext> options) : base(options)
    {
    }

    public DbSet<User> Users => Set<User>();
    public DbSet<CodeRun> CodeRuns => Set<CodeRun>();
    public DbSet<CodeError> CodeErrors => Set<CodeError>();
    public DbSet<CodeExample> CodeExamples => Set<CodeExample>();

    protected override void OnModelCreating(ModelBuilder modelBuilder)
    {
        base.OnModelCreating(modelBuilder);

        modelBuilder.Entity<User>()
            .HasIndex(u => u.UserName)
            .IsUnique();

        modelBuilder.Entity<User>()
            .HasMany(u => u.CodeRuns)
            .WithOne(r => r.User)
            .HasForeignKey(r => r.UserId)
            .OnDelete(DeleteBehavior.Cascade);

        modelBuilder.Entity<CodeRun>()
            .HasMany(r => r.Errors)
            .WithOne(e => e.CodeRun)
            .HasForeignKey(e => e.CodeRunId)
            .OnDelete(DeleteBehavior.Cascade);

        modelBuilder.Entity<CodeExample>().HasData(
            new CodeExample
            {
                Id = 1,
                Title = "שלום עולם",
                Description = "דוגמה פשוטה להדפסה",
                Code = "x <- 5\nwrite(\"hello\", x)"
            },
            new CodeExample
            {
                Id = 2,
                Title = "תנאי",
                Description = "דוגמה לשימוש בתנאי",
                Code = "if x = 5 :\nwrite(\"good job\")\n||"
            }
        );
    }
}
