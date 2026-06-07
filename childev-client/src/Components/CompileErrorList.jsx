function getFriendlyType(type) {
    switch (type) {
        case "Lexical error":
            return "שגיאת כתיב";
        case "Syntax error":
            return "שגיאת תחביר";
        case "Semantic error":
            return "שגיאת סמנטית";
        default:
            return "תיקון קטן";
    }
}

function ErrorList({ checked, errors }) {
    if (!checked) {
        return (
            <div className="friendly-info">
                👀 אחרי שתלחצו על בדיקת קוד, התוצאה תופיע כאן.
            </div>
        );
    }

    if (errors.length === 0) {
        return (
            <div className="success-box">
                🎉 כל הכבוד! הקוד נראה תקין.
            </div>
        );
    }
    return (
        <div className="errors-list">
            <div className="error-summary">
                🛠️ מצאנו {errors.length} דברים קטנים לתקן
            </div>

            {errors.map((error, index) => (
                <div className="error-card" key={index}>
                    <div className="error-icon">😊</div>

                    <div className="error-content">
                        <h3>צריך תיקון קטן</h3>

                        <p>
                            <strong>איפה?</strong>{" "}
                            שורה {error.line}
                            {error.column ? `, עמודה ${error.column}` : ""}
                        </p>

                        <p>
                            <strong>סוג:</strong>{" "}
                            {getFriendlyType(error.type)}
                        </p>

                        <p>
                            <strong>מה לתקן?</strong>{" "}
                            {error.message}
                        </p>
                    </div>
                </div>
            ))}
        </div>
    );
}

export default ErrorList;