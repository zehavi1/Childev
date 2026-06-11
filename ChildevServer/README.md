# ChildevServer

שרת ASP.NET Core עבור פרויקט Childev.

השרת מבצע:

- הרשמה והתחברות של משתמשים
- שמירת משתמשים במסד SQLite
- קבלת קוד מ־React דרך API
- שמירת הקוד לקובץ זמני
- הפעלת `Childev.exe` כתהליך חיצוני
- קריאת הפלט מה־Standard Output
- המרת שורות `ERROR|line|column|type|message` לאובייקטים
- שמירת הרצות ושגיאות במסד הנתונים
- החזרת JSON ללקוח

## מבנה הפרויקט

```text
ChildevServer/
  Controllers/
    AuthController.cs
    ChildevController.cs
    ExamplesController.cs
  Data/
    AppDbContext.cs
  Dtos/
    AuthDtos.cs
    ChildevDtos.cs
    ExampleDtos.cs
  Models/
    User.cs
    CodeRun.cs
    CodeError.cs
    CodeExample.cs
  Services/
    ChildevRunnerService.cs
  Program.cs
  appsettings.json
  ChildevServer.csproj
```

## הרצה ראשונית

מתוך תיקיית הפרויקט:

```bash
dotnet restore
dotnet run
```

בפעם הראשונה ייווצר קובץ מסד נתונים בשם:

```text
childev.db
```

היצירה נעשית על ידי:

```csharp
db.Database.EnsureCreated();
```

לפרויקט לימודי זה נוח. בפרויקט אמיתי מומלץ להשתמש ב־migrations.

## חיבור לקובץ Childev.exe

בקובץ `appsettings.json` יש להגדיר את הנתיב לקובץ ההרצה של הקומפיילר:

```json
"Childev": {
  "ExePath": "Childev.exe",
  "TempFolder": "temp",
  "TimeoutSeconds": 10
}
```

אם הקובץ נמצא בתיקייה אחרת, לדוגמה ב־Windows:

```json
"ExePath": "C:\\Childev\\Childev.exe"
```

או פשוט לשים את `Childev.exe` ליד קובץ ההרצה של השרת.

## פורמט הפלט המצופה מ־Childev.exe

במקרה תקין:

```text
OK
```

במקרה של שגיאה:

```text
ERROR|3|5|Syntax|חסר ערך אחרי סימן ההשמה
```

אפשר להחזיר כמה שורות ERROR:

```text
ERROR|3|5|Syntax|חסר ערך אחרי סימן ההשמה
ERROR|4|1|Syntax|חסר סוף בלוק
```

## API

### הרשמה

```http
POST /api/auth/register
```

```json
{
  "userName": "tamar",
  "password": "1234",
  "displayName": "תמר"
}
```

### התחברות

```http
POST /api/auth/login
```

```json
{
  "userName": "tamar",
  "password": "1234"
}
```

### בדיקת קוד

```http
POST /api/Childev/check
```

```json
{
  "userId": 1,
  "code": "x <- 5\nwrite(\"hello\", x)"
}
```

תשובה לדוגמה:

```json
{
  "isSuccess": false,
  "runId": 7,
  "errors": [
    {
      "line": 3,
      "column": 5,
      "type": "Syntax",
      "message": "חסר ערך אחרי סימן ההשמה"
    }
  ]
}
```

### קבלת דוגמאות קוד

```http
GET /api/examples
```

### קבלת היסטוריית הרצות

```http
GET /api/Childev/runs/1
```

### קבלת פרטי הרצה מלאה

```http
GET /api/Childev/runs/details/7
```

## דוגמת קריאה מ־React

```javascript
const response = await fetch("https://localhost:7000/api/Childev/check", {
  method: "POST",
  headers: {
    "Content-Type": "application/json"
  },
  body: JSON.stringify({
    userId: 1,
    code: codeText
  })
});

const result = await response.json();

if (result.isSuccess) {
  alert("הקוד תקין");
} else {
  console.log(result.errors);
}
```

## טבלאות במסד הנתונים

בשרת הזה קיימות ארבע טבלאות:

- Users
- CodeRuns
- CodeErrors
- CodeExamples

הוספתי את `CodeErrors` כי הדרישה כוללת שמירת רשימת שגיאות לכל הרצה. בלי טבלה זו ניתן לדעת רק האם ההרצה הצליחה, אבל לא לשמור את השגיאות עצמן בצורה מסודרת.

קשרים:

```text
Users 1 ---- * CodeRuns
CodeRuns 1 ---- * CodeErrors
CodeExamples היא טבלה עצמאית של דוגמאות מוכנות
```
