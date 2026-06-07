export const codeExamples = [
    {
        id: 1,
        icon: "👋",
        title: "שלום עולם",
        description: "הדפסה פשוטה למסך",
        code:
`x <- 5
write("hello", x)`
    },
    {
        id: 2,
        icon: "❓",
        title: "תנאי",
        description: "בדיקה אם מספר שווה לערך",
        code:
`x <- 5
if x = 5 :
write("good job")
||`
    },
    {
        id: 3,
        icon: "🔁",
        title: "לולאה",
        description: "חזרה על פעולה כמה פעמים",
        code:
`for i <- 1 to 5 :
write(i)
||`
    },
    {
        id: 4,
        icon: "🛠️",
        title: "דוגמה עם טעות",
        description: "כדי לראות איך מופיעות שגיאות",
        code:
`x <-
if x = 5
write("oops")`
    }
];